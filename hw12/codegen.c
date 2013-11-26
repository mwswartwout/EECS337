/*******************************************************************************
*
* FILE:		codegen.c
*
* DESC:		EECS 337 Assignment 12
*
* AUTHOR:	caseid
*
* DATE:		November 26, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
/*******************************************************************************
 *
 *	pic memory routines
 *
 *	MEMORY REGISTERS
 *
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
 *
 ******************************************************************************/
/*
 *	get an address off the top of memory
 */
int	get_address( int size)
{
/*
 *	check if this is the very first call
 */
	if( ! data.address)
	    data.address = TOP_MEMORY;
/* 
 *	get the next PIC physical address
 */
	if( data.address + size > BOTTOM_MEMORY)
	{
	  fprintf( stderr, "Error: PIC address allocation failure: 0X%x size: %d\n", data.address, size);
		data.errors++;
		return( 0);
	}
	return( data.address++);	/* at top of memory go down */
}

/*******************************************************************************
 *
 *	pic instruction table 
 *
 ******************************************************************************/
char	*instr_table[] =
{
	"label", "mov", "add", "and", "ior", "sub", "xor", "comf",
	"decf", "decfsz", "incf", "incfsz", "rlf", "rrf", "swapf",
	"bcf", "bsf", "btfsc", "btfss", "call", "goto", "tris", "clr",
	"retlw", "clrwdt", "nop", "option", "retfie", "return",
	"sleep",
};

/*******************************************************************************
 *
 *	dynamic memory routines
 *
 ******************************************************************************/
/*
 *	allocate buffer memory routine
 */
char	*new_buffer( char *text, int length)
{
	char	*buffer = 0;

	if( 0 < length)
	{
		if( !( buffer = (char*)malloc( length)))
			new_error( length);
		else
		{
/*
 *	initialize the buffer to zero and copy in text
 */
			memset( (void *)buffer, 0, (size_t)length);
			strncpy( buffer, text, length);
			data.memory += length;
		}
	}
	return( buffer);
}

/*
 *	deallocate buffer memory routine
 */
void	free_buffer( char *buffer, int length)
{
	if( 0 < length)
	{
		free( buffer);
		data.memory -= length;
	}
	return;
}

/*
 *	allocate a tuple data structure
 */
TUPLE	*new_tuple( int token, unsigned char value, int address, int mask, char *buffer, int length)
{
	TUPLE *tuple;

	if( !(tuple = ( TUPLE*)malloc( sizeof( TUPLE))))
		new_error( sizeof( TUPLE));
	else
	{
		memset( (void *)tuple, 0, (size_t)sizeof( TUPLE));
		tuple->token = token;
		tuple->value = value;
		tuple->address = address;
		tuple->mask = mask;
		if( 0 < length)
		{
			tuple->length = length;
			tuple->buffer = new_buffer( buffer, length);
		}
		data.memory += sizeof( TUPLE);
	}
	return( tuple);
}

/*
 *	deallocate a tuple data structure
 */
void	free_tuple( TUPLE *tuple)
{
	if( tuple)
	{
		free_buffer( tuple->buffer, tuple->length);
		free( tuple);
		data.memory -= sizeof( TUPLE);
	}
	return;
}

/*
 *	deallocate a tuple linked list
 */
void	free_tuple_list( TUPLE *tuple)
{
	TUPLE	*tuple_next;
/*
 *	deallocate the tuple linked list structure
 */
	while( tuple)
	{
		tuple_next = tuple->next;
		free_tuple( tuple);
		tuple = tuple_next;
	}
	return;
}

/*
 *	find the last tuple structure in linked list
 */
TUPLE	*end_tuple_list( TUPLE *tuple)
{
	if( tuple)
		while( tuple->next)
			tuple = tuple->next;
	return tuple;
}

/*
 *	link the tail of first linked list to second linked list
 */
TUPLE	*tuple_tail_to_head( TUPLE *head1, TUPLE *head2)
{
	TUPLE	*lists;
/*
 *	check if head1 exist else return head2
 */
	if( head1)
	{
		lists = end_tuple_list( head1);
		lists->next = head2;
		return( head1);
	}
	return( head2);
}

/*
 *	print a tuple structure
 */
void	print_tuple( TUPLE *tuple)
{
	if( tuple)
	{
		printf( "next: %p ", tuple->next);
		printf( "instr: %s ", instr_table[ tuple->token % I_SLEEP]);
		printf( "value: %x ", tuple->value);
		printf( "address: %x ", tuple->address);
		printf( "mask: %x ", tuple->mask);
		if( tuple->mask)
		{
			if( tuple->mask & MASK_VALUE)
				printf( "MASK_VALUE ");
			if( tuple->mask & MASK_ADDRESS)
				printf( "MASK_ADDRESS ");
			if( tuple->mask & MASK_LABEL)
				printf( "MASK_LABEL ");
			if( tuple->mask & MASK_W_REG)
				printf( "MASK_W_REG ");
			if( tuple->mask & MASK_F_REG)
				printf( "MASK_F_REG ");
			if( tuple->mask & MASK_INSTR)
				printf( "MASK_INSTR ");
		}
		printf( "level: %d ", tuple->level);
		printf( "length: %d ", tuple->length);
		if( 0 < tuple->length)
			printf( "buffer: %s\n", tuple->buffer);
		else
			printf( "buffer: %p\n", tuple->buffer);
	}
	return;
}

/*
 *	print a tuple linked list
 */
void	print_tuple_list( TUPLE *tuple)
{
/*
 *	print the tuple linked list structure
 */
	while( tuple)
	{
		print_tuple( tuple);
		tuple = tuple->next;
	}
	return;
}

/*******************************************************************************
 *
 *	code generator routines
 *
 ******************************************************************************/
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
	printf( "; by: caseid, date: Fall 2013\n");
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
 *	generate the symbol table initialization code
 */
//	code_generator_pic_address_init();
/*
 *	generate the PIC assember mloop header code
 */
	printf( "mloop:\n");
	printf( "; read from the standard input (stdin)\n");
	printf( "	movf	PORTA,w\n");
}

/*
 *	generate the pic mloop tail code and standard library
 */
void	code_generator_pic_postfix( void)
{
	printf( "	goto	mloop\n");
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
	if( tuple->token != I_CLR)
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
	if( tuple->mask & MASK_ADDRESS)
		printf( "0x%x", tuple->address);
	switch( tuple->token)
	{
	case I_BCF:
	case I_BSF:
	case I_BTFSC:
	case I_BTFSS:
		if( tuple->mask & MASK_VALUE)
			printf( ",%d", tuple->value);
		break;
	default:
		if( tuple->mask & MASK_VALUE)
			printf( "0x%x", tuple->value);
		break;
	}
	if( tuple->mask & MASK_LABEL)
		if( tuple->length <= 2)		/* assembler does not like one letter labels */
			printf( "%s1", tuple->buffer);
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
	switch( tuple->token)
	{
	case I_MOV:
		if( tuple->mask & MASK_VALUE)
			printf( "lw\t");
		else if( tuple->mask & MASK_W_REG || tuple->mask & MASK_F_REG)
			printf( "f\t");
		else
			printf( "wf\t");
		break;
	case I_CLR:
		if( tuple->mask & MASK_W_REG)
			printf( "w");
		else
			printf( "f\t");
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
			printf( "%s1:\n", tuple->buffer);
		else
			printf( "%s:\n", tuple->buffer);
		break;
	case I_MOV:
	case I_ADD:
	case I_AND:
	case I_IOR:
	case I_SUB:
	case I_XOR:
	case I_CLR:
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
	case I_CALL:
	case I_GOTO:
	case I_TRIS:
	case I_RETLW:
	case I_BCF:
	case I_BSF:
	case I_BTFSC:
	case I_BTFSS:
		printf( "\t%s\t", instr_table[ tuple->token]);
		code_generator_operand( tuple);
		break;
	case I_CLRWDT:
	case I_NOP:
	case I_OPTION:
	case I_RETFIE:
	case I_RETURN:
	case I_SLEEP:
		printf( "\t%s\n", instr_table[ tuple->token]);
		break;
	}
	return;
}

/*
 *	code generator for the pic16f1827 processor
		code_generator_pic16f1827( $1.tuple);
 */
void	code_generator_pic16f1827( TUPLE *tuple_list)
{
	TUPLE *tuple;
/*
 *	debug tuple list
 */
#ifdef	YYDEBUG
	if( IS_FLAGS_DEBUG( data.flags))
		print_tuple_list( tuple_list);
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
	for( index = 0; index <= 52; index++)
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
		}
		tuple_head = tuple_tail_to_head( tuple_head, tuple);
	}
	code_generator_pic16f1827( tuple_head);
	return;
}

