/*******************************************************************************
*
* FILE:		codegen.c
*
* DESC:		EECS 337 Project
*
* AUTHOR:	mws85
*
* DATE:		December 5, 2013
*******************************************************************************/
#include	"yystype.h"
/*******************************************************************************
 *	pic memory routines
 *	MEMORY REGISTERS
 *	+---------------+
 *	| TOP_MEMORY	| 0X20
 *	+---------------+
 *	| ...		|
 *	+---------------+
 *	| REG MEMORY	| 0X6F
 *	+---------------+
 *	| GLOBAL MEMORY	| 0X70
 *	+---------------+
 *	| ...		|
 *	+---------------+
 *	| BOTTOM_MEMORY	| 0X7F
 *	+---------------+
 ******************************************************************************/
/*
 *	get an address off the top of memory
 */
int	get_address( int size)
{
	int address;
/*
 *	check if this is the very first call
 */
	if( ! data.address)
	    data.address = TOP_MEMORY;
/* 
 *	pop the next PIC physical address
 */
	if( data.address + size > BOTTOM_MEMORY)
	{
		fprintf( stderr, "Error: PIC address allocation failure: 0X%x size: %d\n", data.address, size);
		data.errors++;
		return( 0);
	}
	address = data.address;
	data.address += size;	/* at top of memory go down */
	return( address);	
}

/*
 *	put an address back onto the static memory
 */
int	put_address( int address, int size)
{
/* 
 *	push the previous PIC physical address
 */
	if( data.address - size < TOP_MEMORY || data.address != address + size)
	{
		fprintf( stderr, "Error: address deallocation failure: 0X%x size: %d\n", address, size);
		data.errors++;
		return( 0);
	}
	data.address -= size;	/* at bottom of memory go up */
	return( data.address);	
}

/*******************************************************************************
 *
 *	code generator routines
 *
 ******************************************************************************/
/*
 *	generate the symbol table and temporary address
 *
 *	use MYVAR1 EQU ADDRESS or let the assembler assign
 *
 *    CBLOCK 0x20 ; Define GPR variable register locations
 *        MYVAR1  ; User variables allocated contiguously
 *        MYVAR2  ; 
 *        MYVAR3  ; 
 *    ENDC
 */
void	code_generator_pic_address( void)
{
	TUPLE	*tuple;
	TUPLE	*tuple2;
/*
 *	if address only then return
 */
	if( IS_FLAGS_ADDRESS( data.flags))
	    return;
/*
 *	print the symbol table information
 */
	printf( ";symbol table:\n");
	for( tuple = data.symbol_table; tuple; tuple = tuple->next)
	{
		if( 0 < tuple->level)
		    printf( "%s_%d EQU 0x%x\n", tuple->buffer, tuple->level, tuple->address);
		else if( tuple->length <= 2)		/* assembler does not like one letter labels */
		    printf( "%s_ EQU 0x%x\n", tuple->buffer, tuple->address);
		else
		    printf( "%s EQU 0x%x\n", tuple->buffer, tuple->address);
	}
	printf( ";symbol table free:\n");
	for( tuple = data.symbol_table_free; tuple; tuple = tuple->next)
	{
		if( 0 < tuple->level)
		    printf( "%s_%d EQU 0x%x\n", tuple->buffer, tuple->level, tuple->address);
		else if( tuple->length <= 2)		/* assembler does not like one letter labels */
		    printf( "%s_ EQU 0x%x\n", tuple->buffer, tuple->address);
		else
		    printf( "%s EQU 0x%x\n", tuple->buffer, tuple->address);
	}
	return;
}

/*
 *	generate the pic header information
 */
void	code_generator_pic_prefix( void)
{
/*
 *	generate the PIC assember code header
 */
	printf( "; automatic code generation for PIC16F1827\n");
	printf( "; EECS337 Compiler Design\n");
	printf( "; by: mws85, date: Fall 2013\n");
	printf( "; for PIC16F1827 processor\n");
	printf( "; CPU configuration\n");
	printf( "	list		p=16f1827      ; list directive to define processor\n");
	printf( "	#include	<p16f1827.inc> ; processor specific variable definitions\n");
	printf( "	__CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_OFF & _CLKOUTEN_ON & _IESO_OFF & _FCMEN_OFF\n");
	printf( "	__CONFIG _CONFIG2, _WRT_OFF & _PLLEN_OFF & _STVREN_OFF & _BORV_19 & _LVP_OFF\n");
/*
 *	generate the symbol table and temporary address
 */
	code_generator_pic_address();
/*
 *	generate the PIC power-up assember program
 */
	printf( "; power-up or reset vector\n");
	printf( "	org     0x0000            ; processor reset vector\n");
	printf( "	pagesel init\n");
	printf( "	goto	init	; skip interrupt vector space (reserved)\n");
	printf( "; interrupt vector \n");
	printf( "	org 	0x04\n");
	printf( "inter:			; on interrupt PC set to 0x04 and automatically clears GIE\n");
	printf( "	retfie		; return from interrupt and automatically sets GIE\n");
	printf( "; beginning of program code\n");
	printf( "	org	0x08\n");
	printf( "init:\n");
	printf( "; On reset all ports are inputs.\n");
	printf( "	movlb	1	; switch to bank 1 memory\n");
	printf( ";	clrf	TRISA		; set PORTA to all outputs\n");
	printf( "	clrf	TRISB		; set PORTB to all outputs\n");
	printf( "	movlb	0	; switch to bank 0 memory\n");
/*
 *	generate the PIC assember mloop header code
 */
	printf( "mloop:\n");
	printf( "; here begins the main program\n");
}

/*
 *	generate the pic tail information
 */
void	code_generator_pic_postfix( void)
{
	printf( "	return	; if main does not have a return\n");
	printf( "; only standard library function\n");
	printf( "printf:			\n");
	printf( "	movwf	PORTB	; output w to standard output (stdout)\n");
	printf( "	return\n");
	printf( "	end		; end of program code\n");
}

/*
 *	code generator operand postfix
 */
void	code_generator_operand_postfix( TUPLE *tuple)
{
	if( tuple->token != I_CLR && tuple->token != I_CALL && tuple->token != I_BR)
	{
		if( tuple->mask & MASK_W_REG)
			printf( ",w");
		else if( tuple->mask & MASK_F_REG)
			printf( ",f");
	}
}

/*
 *	code generator operand
 */
void	code_generator_operand( TUPLE *tuple)
{
	if( tuple->mask & (MASK_ADDRESS | MASK_WFC | MASK_WFB))
		printf( "0x%02x", tuple->address);
	switch( tuple->token)
	{
	case I_BCF:
	case I_BSF:
	case I_BTFSC:
	case I_BTFSS:
		if( tuple->mask & MASK_VALUE)
			printf( ",%d", tuple->value);
		break;
	case I_ADD: //Added a case for ADD commands instead of putting them in the default case
		if (tuple->address == FSR0) //Handles ADDFSR
			printf( "0, %d", tuple->value);
		else if (tuple->address == FSR1) //HANDLES THE 2nd FSR 
			printf( "1, %d", tuple->value);
		break;
	case I_MOV: //Added a case for MOV commands instead of putting them in the default case
		if (tuple->mask & (MASK_IW | MASK_WI)) //Handles MOVIW and MOVWI
		{
			if (tuple ->mask & MASK_INDIRECT)
				printf("%i[%i]", tuple->value, tuple-> address);
			else 
			{
				switch (tuple->value) //Checks for whether it's PREINCREMENT, PREDECREMENT, POSTINCREMENT, or POSTDECREMENT
				{
				case PREINCREMENT:
					printf("++%i", tuple->address);
					break;
				case PREDECREMENT:
					printf("--%i", tuple->address);
					break;
				case POSTINCREMENT:
					printf("%i++", tuple->address);
					break;
				case POSTDECREMENT:
					printf("%i--", tuple->address);
					break;
				}
			}
		}
	default:
		if( tuple->mask & MASK_VALUE)
			printf( "0x%02x", tuple->value);
		break;
	}
	if( tuple->mask & MASK_LABEL)
		if( 0 < tuple->level)
			printf( "%s_%d", tuple->buffer, tuple->level);
		else if( tuple->length <= 2)		/* assembler does not like one letter labels */
			printf( "%s_", tuple->buffer);
		else
			printf( "%s", tuple->buffer);
	code_generator_operand_postfix( tuple);
	printf( "\n");
	return;
}

/*
 *	code generator instruction postfix
 */
void	code_generator_instr_postfix( TUPLE *tuple)
{
	//Prints the postfixes for the commands based on what the mask is
	switch( tuple->token)
	{
	case I_MOV:
		if (tuple->mask & MASK_LP)
			printf( "lp\t"); //MOVLP
		else if (tuple->mask & MASK_LB)
			printf( "lb\t"); //MOVLB
		else if( tuple->mask & MASK_VALUE)
			printf( "lw\t"); //MOVLW
		else if( tuple->mask & MASK_W_REG || tuple->mask & MASK_F_REG) 
			printf( "f\t"); //MOVF
		else
			printf( "wf\t"); //MOVWF
		break;
	case I_CLR:
		if( tuple->mask & MASK_W_REG)
			printf( "w"); //CLRW
		else
			printf( "f\t");
		break;
	case I_ADD:
		if (tuple->mask & MASK_VALUE)
			printf( "lw\t"); //ADDLW
		else if (tuple->mask & MASK_WFC)
			printf( "wfc\t"); //ADDWFC
		else if (tuple->mask & MASK_ADDRESS)
			printf( "wf\t"); //ADDWF
		else if (tuple->mask & MASK_FSR)
			printf( "fsr\t"); //ADDFSR
		break;
	case I_CALL:
		if (tuple->mask & MASK_INSTR)
			break;
		else
			printf( "w\t"); //CALLW
		break;
	case I_BR:
		if (tuple->mask & MASK_LABEL)
			printf( "a\t"); //BRA
		else
			printf( "w\t"); //BRW
		break;
	case I_SUB:
		if (tuple->mask & MASK_VALUE)
			printf("lw\t"); //SUBLW
		else if (tuple->mask & MASK_WFB)
			printf("wfb\t"); //SUBWFB
		else
			printf("wf\t"); //SUBWF
		break;
	default:
		if( tuple->mask & MASK_VALUE)
			printf( "lw\t");
		else
			printf( "wf\t");
		break;
	}
}

/*
 *	code generator instruction
 */
void	code_generator_instr( TUPLE *tuple)
{
	switch( tuple->token)
	{
	case I_LABEL:
		if( tuple->length <= 2)		/* assembler does not like one letter labels */
			printf( "%s_:\n", tuple->buffer);
		else
			printf( "%s:\n", tuple->buffer);
		break;
	case I_MOV:
	case I_ADD:
	case I_AND:
	case I_IOR:
	case I_CALL:
	case I_SUB:
	case I_XOR:
	case I_CLR:
	case I_BR:
		printf( "\t%s", instr_table[ tuple->token]);
		code_generator_instr_postfix( tuple);
		code_generator_operand( tuple);
		break;
	case I_COMF:
	case I_DECF:
	case I_DECFSZ:
	case I_INCF:
	case I_INCFSZ:
	case I_RLF:
	case I_RRF:
	case I_SWAPF:
	case I_GOTO:
	case I_TRIS:
	case I_RETLW:
	case I_BCF:
	case I_BSF:
	case I_BTFSC:
	case I_BTFSS:
	case I_ARSF:
	case I_LSLF:
	case I_LSRF:
		printf( "\t%s\t", instr_table[ tuple->token]);
		code_generator_operand( tuple);
		break;
	case I_CLRWDT:
	case I_NOP:
	case I_OPTION:
	case I_RETFIE:
	case I_RETURN:
	case I_SLEEP:
	case I_RESET:
		printf( "\t%s\n", instr_table[ tuple->token]);
		break;
	}
	return;
}

/*
 *	post process the instruction list
 *	insert post initialization code (goto mloop)
 */
TUPLE	*code_post_process_initialize( TUPLE *tuple_list)
{
	TUPLE *tuple = 0;
	TUPLE *lists;
	TUPLE *tuple_next;
/*
 *	create the instructions based on the main flag
 */
	if( IS_FLAGS_MAIN( data.flags))
	{
		tuple = new_tuple( I_MOV, 0, 0, MASK_LABEL | MASK_W_REG, "PORTA", sizeof( "PORTA") + 1);	/* read from stdin */
		tuple->next = new_tuple( I_CALL, 0, 0, MASK_LABEL, "main", sizeof( "main") + 1);		/* call main */
	}
	lists = new_tuple( I_GOTO, 0, 0, MASK_LABEL, "mloop", sizeof( "mloop") + 1);			/* goto mloop */
	tuple = tuple_tail_to_head( tuple, lists);
/*
 *	check if no declaration or just function body
 */
	if( ! tuple_list || tuple_list->token == I_LABEL)
	{
		tuple = tuple_tail_to_head( tuple, tuple_list);
		return( tuple);
	}
/*
 *	check if declaration list [ function body ]
 */
	for( lists = tuple_list; lists; lists = lists->next)
	{
		if( ! lists->next || lists->next->token == I_LABEL)	/* start of function body */
		{
			tuple_next = lists->next;
			lists->next = tuple;
			tuple_list = tuple_tail_to_head( tuple_list, tuple_next);
			return( tuple_list);
		}
	}
	return( tuple_list);
}

/*
 *	post process the instruction list
 *	insert RETURNs for subroutines without returns
 */
TUPLE	*code_post_process_return( TUPLE *tuple_list)
{
	TUPLE *lists;
	TUPLE *tuple_next;
/*
 *	check if return, function body
 */
	for( lists = tuple_list; lists; lists = lists->next)
	{
		switch( lists->token)
		{
		case I_RETURN:
		case I_RETFIE:
		case I_RETLW:
			break;
		default:
			if( lists->next && lists->next->token == I_LABEL && ! lists->next->value)	/* start of function body */
			{
				tuple_next = lists->next;
				lists->next = new_tuple( I_RETURN, 0, 0, MASK_INSTR, 0, 0);
				lists->next->next = tuple_next;
			}
		}
	}
	return( tuple_list);
}

/*
 *	post process the instruction list
 *	this gets messy because we have to fix lots of things
 */
TUPLE	*code_post_process( TUPLE *tuple_list)
{
/*
 *	insert post initialization code (goto mloop)
 */
	tuple_list = code_post_process_initialize( tuple_list);
/*
 *	insert RETURNs for subroutines without returns
 */
	tuple_list = code_post_process_return( tuple_list);
/*
 *	return fixed tuple instruction list
 */
	return( tuple_list);
}

/*
 *	code generator for the pic16f1827 processor
		code_generator_pic16f1827( $1.tuple);
 */
void	code_generator_pic16f1827( TUPLE *tuple_list)
{
	TUPLE *tuple;
/*
 *	post process the instruction list
 */
	tuple_list = code_post_process( tuple_list);
/*
 *	debug tuple list
 */
#ifdef	YYDEBUG
	if( IS_FLAGS_DEBUG( data.flags))
	{
		printf( "Debug: code tuples\n");
		print_tuple_list( tuple_list);
	}
#endif
/*
 *	generate the header
 */
	code_generator_pic_prefix();
/*
 *	walk the tuple list and generate PIC16F85 assembler code
 */
	for( tuple = tuple_list; tuple; tuple = tuple->next)
	{
		code_generator_instr( tuple);
	}
/*
 *	generate the end of pic code
 */
	code_generator_pic_postfix();
/*
 *	deallocate the tuple linked list
 */
	free_tuple_list( tuple_list);
	return;
}

/*
 *	Test complete instruction set of PIC16F1827
 */
void	code_generator_instr_test( void)
{
	TUPLE	*tuple;
	TUPLE	*tuple_head = 0;
	int	index;
/*
 *	loop thru the PIC instructions and generate and example of each
 *	instruction format. The generated instruction sequence does NOT
 *	represent a meaningful program!
 */
	for( index = 0; index <=90; index++)
	{
		switch( index)
		{
		case 0:
/*    MOVLW   value        Place value in W */
			tuple = new_tuple( I_MOV, 0x0f, 0, MASK_VALUE, 0, 0);
			break;
		case 1:
/*    MOVF    address,W    Copy contents of address to W */
		  	tuple = new_tuple( I_MOV, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 2:
/*    MOVF    address,F    Copy contents of address to itself (not useless; sets Z flag if zero) */
			tuple = new_tuple( I_MOV, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 3:
/*    MOVWF   address      Copy contents of W to address */
			tuple = new_tuple( I_MOV, 0, get_address( 1), MASK_ADDRESS, 0, 0);
			break;
		case 4:
/*    ADDLW   value           Add W to value, place result in W */
			tuple = new_tuple( I_ADD, 0x0f, 0, MASK_VALUE, 0, 0);
			break;
		case 5:
/*    ADDWF   address,W       Add W to contents of address, place result in W */
			tuple = new_tuple( I_ADD, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 6:
/*    ADDWF   address,F       Add W to contents of address, store result at address */
			tuple = new_tuple( I_ADD, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 7:
/*    ANDLW   value           Logical-AND W with value, place result in W */
			tuple = new_tuple( I_AND, 0x0f, 0, MASK_VALUE, 0, 0);
			break;
		case 8:
/*    ANDWF   address,W       Logical-AND W with contents of address, place result in W */
			tuple = new_tuple( I_AND, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 9:
/*    ANDWF   address,F       Logical-AND W with contents of address, store result at address */
			tuple = new_tuple( I_AND, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 10:
/*    IORLW   value           Logical-OR W with value, place result in W */
			tuple = new_tuple( I_IOR, 0x0f, 0, MASK_VALUE, 0, 0);
			break;
		case 11:
/*    IORWF   address,W       Logical-OR W with contents of address, place result in W */
			tuple = new_tuple( I_IOR, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 12:
/*    IORWF   address,F       Logical-OR W with contents of address, store result at address */
			tuple = new_tuple( I_IOR, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 13:
/*    SUBLW   value        Subtract W from value, place result in W */
			tuple = new_tuple( I_SUB, 0x0f, 0, MASK_VALUE, 0, 0);
			break;
		case 14:
/*    SUBWF   address,W    Subtract W from contents of address, place result in W */
			tuple = new_tuple( I_SUB, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 15:
/*    SUBWF   address,F    Subtract W from contents of address, store result at address */
			tuple = new_tuple( I_SUB, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 16:
/*    XORLW   value        Logical-XOR W with value, place result in W */
			tuple = new_tuple( I_XOR, 0x0f, 0, MASK_VALUE, 0, 0);
			break;
		case 17:
/*    XORWF   address,W    Logical-XOR W with contents of address, place result in W */
			tuple = new_tuple( I_XOR, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 18:
/*    XORWF   address,F    Logical-XOR W with contents of address, store result at address */
			tuple = new_tuple( I_XOR, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 19:
/*    BCF     address,bitnumber       Set specified bit to 0 */
			tuple = new_tuple( I_BCF, 0x07, get_address( 1), MASK_VALUE | MASK_ADDRESS, 0, 0);
			break;
		case 20:
/*    BSF     address,bitnumber       Set specified bit to 1 */
			tuple = new_tuple( I_BSF, 0x07, get_address( 1), MASK_VALUE | MASK_ADDRESS, 0, 0);
			break;
		case 21:
/*    BTFSC   address,bitnumber	Test bit, skip next instruction if bit is 0 */
			tuple = new_tuple( I_BTFSC, 0x07, get_address( 1), MASK_VALUE | MASK_ADDRESS, 0, 0);
			break;
		case 22:
/*    BTFSS   address,bitnumber       Test bit, skip next instruction if bit is 1 */
			tuple = new_tuple( I_BTFSS, 0x07, get_address( 1), MASK_VALUE | MASK_ADDRESS, 0, 0);
			break;
		case 23:
/*    COMF    address,W       Reverse all the bits of contents of address, place result in W */
			tuple = new_tuple( I_COMF, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 24:
/*    COMF    address,F       Reverse all the bits of contents of address, store result at address */
			tuple = new_tuple( I_COMF, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 25:
/*    DECF    address,W       Subtract 1 from contents of address, place result in W */
			tuple = new_tuple( I_DECF, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 26:
/*    DECF    address,F       Subtract 1 from contents of address, store result at address */
			tuple = new_tuple( I_DECF, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 27:
/*    DECFSZ  address,W       Like DECF address,W and skip next instruction if result is 0 */
			tuple = new_tuple( I_DECFSZ, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 28:
/*    DECFSZ  address,F       Like DECF address,F and skip next instruction if result is 0 */
			tuple = new_tuple( I_DECFSZ, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 29:
/*    INCF    address,W       Add 1 to contents of address, place result in W */
			tuple = new_tuple( I_INCF, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 30:
/*    INCF    address,F       Add 1 to contents of address, store result at address */
			tuple = new_tuple( I_INCF, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 31:
/*    INCFSZ  address,W       Like INCF address,W and skip next instruction if result is 0 */
			tuple = new_tuple( I_INCFSZ, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 32:
/*    INCFSZ  address,F       Like INCF address,F and skip next instruction if result is 0 */
			tuple = new_tuple( I_INCFSZ, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 33:
/*    RLF     address,W    Rotate bits left through carry flag, place result in W */
			tuple = new_tuple( I_RLF, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 34:
/*    RLF     address,F    Rotate bits left through carry flag, store result at address */
			tuple = new_tuple( I_RLF, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 35:
/*    RRF     address,W    Rotate bits right through carry flag, place result in W */
			tuple = new_tuple( I_RRF, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 36:
/*    RRF     address,F    Rotate bits right through carry flag, store result at address */
			tuple = new_tuple( I_RRF, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 37:
/*    SWAPF   address,W    Swap half-bytes at address, place result in W */
			tuple = new_tuple( I_SWAPF, 0, get_address( 1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 38:
/*    SWAPF   address,F    Swap half-bytes at address, store result at address */
			tuple = new_tuple( I_SWAPF, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 39:
/*    CALL    label           Call subroutine (will return with RETURN or RETLW) */
			tuple = new_tuple( I_CALL, 0, 0, MASK_LABEL, "label1", sizeof( "label1") + 1);
			break;
		case 40:
/*    GOTO    label           Jump to another location in the program */
			tuple = new_tuple( I_GOTO, 0, 0, MASK_LABEL, "label2", sizeof( "label2") + 1);
			break;
		case 41:
/*    LABEL    label           create label in the program */
			tuple = new_tuple( I_LABEL, 0, 0, MASK_LABEL, "label1", sizeof( "label1") + 1);
			break;
		case 42:
/*    TRIS    PORTA        Copy W into i/o control register for Port A (deprecated) */
			tuple = new_tuple( I_TRIS, 0, 0, MASK_LABEL, "PORTA", sizeof("PORTA") + 1);
			break;
		case 43:
/*    TRIS    PORTB        Copy W into i/o control register for Port B (deprecated) */
			tuple = new_tuple( I_TRIS, 0, 0, MASK_LABEL, "PORTB", sizeof( "PORTB") + 1);
			break;
		case 44:
/*    CLRW                    Set W to binary 00000000 */
			tuple = new_tuple( I_CLR, 0, 0, MASK_W_REG, 0, 0);
			break;
		case 45:
/*    CLRF    address         Set contents of address to binary 00000000 */
			tuple = new_tuple( I_CLR, 0, get_address( 1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 46:
/*    CLRWDT                  Reset (clear) the watchdog timer */
			tuple = new_tuple( I_CLRWDT, 0, 0, MASK_INSTR, 0, 0);
			break;
		case 47:
/*    NOP                  Do nothing */
			tuple = new_tuple( I_NOP, 0, 0, MASK_INSTR, 0, 0);
			break;
		case 48:
/*    OPTION               Copy W to option register (deprecated instruction) */
			tuple = new_tuple( I_OPTION, 0, 0, MASK_INSTR, 0, 0);
			break;
		case 49:
/*    SLEEP                Go into standby mode */
			tuple = new_tuple( I_SLEEP, 0, 0, MASK_INSTR, 0, 0);
			break;
		case 50:
/*    RETURN               Return from subroutine */
			tuple = new_tuple( I_RETURN, 0, 0, MASK_INSTR, 0, 0);
			break;
		case 51:
/*    RETFIE               Return from interrupt */
			tuple = new_tuple( I_RETFIE, 0, 0, MASK_INSTR, 0, 0);
			break;
		case 52:
/*    RETLW   value        Return from subroutine, placing value into W */
			tuple = new_tuple( I_RETLW, 0x0f, 0, MASK_VALUE, 0, 0);
			break;
		case 53:
			//ADDWFC
			tuple = new_tuple( I_ADD, 0, get_address(1), MASK_WFC | MASK_W_REG, 0, 0);
			break;
		case 54:
			//ADDWFC
			tuple = new_tuple( I_ADD, 0, get_address(1), MASK_WFC | MASK_F_REG, 0, 0);
			break;
		case 55:
			//RESET
			tuple = new_tuple( I_RESET, 0, 0, MASK_INSTR, 0, 0);
			break;
		case 56:
			//CALL
			tuple = new_tuple( I_CALL, 0, 0, MASK_W_REG, 0, 0);
			break;
		case 57:
			//BRW
			tuple = new_tuple( I_BR, 0, 0, MASK_W_REG, 0, 0);
			break;
		case 58:
			//BRA
			tuple = new_tuple( I_BR, 0, 0, MASK_LABEL, "label1", sizeof("label1") + 1);
			break;
		case 59:
			//SUBWFB
			tuple = new_tuple( I_SUB, 0, get_address(1), MASK_W_REG | MASK_WFB, 0, 0);
			break;
		case 60:
			tuple = new_tuple( I_SUB, 0, get_address(1), MASK_F_REG | MASK_WFB, 0, 0);
			break;
		case 61:
			//ARSF
			tuple = new_tuple( I_ARSF, 0, get_address(1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 62:
			tuple = new_tuple( I_ARSF, 0, get_address(1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 63:
			//LSLF
			tuple = new_tuple( I_LSLF, 0, get_address(1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 64:
			tuple = new_tuple( I_LSLF, 0, get_address(1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 65:
			//LSRF
			tuple = new_tuple( I_LSRF, 0, get_address(1), MASK_ADDRESS | MASK_F_REG, 0, 0);
			break;
		case 66:
			tuple = new_tuple( I_LSRF, 0, get_address(1), MASK_ADDRESS | MASK_W_REG, 0, 0);
			break;
		case 67:
			//ADDFSR
			tuple = new_tuple( I_ADD, 100, FSR0,		MASK_FSR, 0, 0);
			break;
		case 68:
			tuple = new_tuple( I_ADD, 100, FSR1, MASK_FSR, 0, 0);
			break;
		case 69:
			//MOVLP
			tuple = new_tuple( I_MOV, 3, 0, MASK_VALUE | MASK_LP, 0, 0);
			break;
		case 70:
			//MOVLB
			tuple = new_tuple( I_MOV, 3, 0, MASK_VALUE | MASK_LB, 0, 0);
			break;
		case 71:
			//MOVIW ++0
			tuple = new_tuple(I_MOV, PREINCREMENT, 0, MASK_IW, 0, 0);
			break;
		case 72:
	  		//MOVIW --0
			tuple = new_tuple(I_MOV, PREDECREMENT, 0, MASK_IW, 0, 0);
			break;
		case 73:
			//MOVIW 0++
			tuple = new_tuple(I_MOV, POSTINCREMENT, 0, MASK_IW, 0, 0);
			break;
		case 74:
			//MOVIW 0--
			tuple = new_tuple(I_MOV, POSTDECREMENT, 0, MASK_IW, 0, 0);
			break;
		case 75:
			//MOVIW 4[0]
			tuple = new_tuple(I_MOV, 4, 0, MASK_IW | MASK_INDIRECT, 0, 0);
			break;
		case 76:
			//MOVIW ++1
			tuple = new_tuple(I_MOV, PREINCREMENT, 1, MASK_IW, 0, 0);
			break;
		case 77:
			//MOVIW --1
			tuple = new_tuple(I_MOV, PREDECREMENT, 1, MASK_IW, 0, 0);
			break;
		case 78:
			//MOVIW 1++
			tuple = new_tuple(I_MOV, POSTINCREMENT, 1, MASK_IW, 0, 0);
			break;
		case 79:
			//MOVIW 1--
			tuple = new_tuple(I_MOV, POSTDECREMENT, 1, MASK_IW, 0, 0);
			break;
		case 80:
			//MOVIW 4[1]
			tuple = new_tuple(I_MOV, 4, 1, MASK_IW | MASK_INDIRECT, 0, 0);
			break;
		case 81:
			//MOVWI ++0
			tuple = new_tuple(I_MOV, PREINCREMENT, 0, MASK_WI, 0, 0);
			break;
		case 82:
			//MOVWI --0
			tuple = new_tuple(I_MOV, PREDECREMENT, 0, MASK_WI, 0, 0);
			break;
		case 83:
			//MOVWI 0++
			tuple = new_tuple(I_MOV, POSTINCREMENT, 0, MASK_WI, 0, 0);
			break;
		case 84:
			//MOVWI 0--
			tuple = new_tuple(I_MOV, POSTDECREMENT, 0, MASK_WI, 0, 0);
			break;
		case 85:
			//MOVWI 4[0]
			tuple = new_tuple(I_MOV, 4, 0, MASK_WI | MASK_INDIRECT, 0, 0);
			break; 
		case 86:
			//MOVWI ++1
			tuple = new_tuple(I_MOV, PREINCREMENT, 1, MASK_WI, 0, 0);
			break;
		case 87:
			//MOVWI --1
			tuple = new_tuple(I_MOV, PREDECREMENT, 1, MASK_WI, 0, 0);
			break;
		case 88:
			//MOVWI 1++
			tuple = new_tuple(I_MOV, POSTINCREMENT, 1, MASK_WI, 0, 0);
			break;
		case 89:
			//MOVWI 1--
			tuple = new_tuple(I_MOV, POSTDECREMENT, 1, MASK_WI, 0, 0);
			break;
		case 90:
			//MOVWI 4[1]
			tuple = new_tuple(I_MOV, 4, 1, MASK_WI | MASK_INDIRECT, 0, 0);
			break; 
		}
		tuple_head = tuple_tail_to_head( tuple_head, tuple);
	}
	code_generator_pic16f1827( tuple_head);
	return;
}

