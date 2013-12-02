/*******************************************************************************
*
* FILE:		tuple.c
*
* DESC:		EECS 337 Project
*
* AUTHOR:	caseid
*
* DATE:		December 5, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
#include	"yystype.h"
/*******************************************************************************
 *
 *	dynamic memory routines
 *
 ******************************************************************************/
/*
 *	memory allocation error (FATAL no return!)
 */
void	new_error( int length)
{
	fprintf( stderr, "Fatal error: memory allocation failure: %d\n", length);
	exit( -1);
}

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
 *	print a tuple structure
 */
void	print_tuple( TUPLE *tuple)
{
	if( tuple)
	{
		printf( "next: %p ", tuple->next);
		if( tuple->mask)
			printf( "instr: %s ", instr_table[ tuple->token % I_SLEEP]);
		else
			printf( "token: %s ", tokens[ tuple->token]);
		printf( "value: %d ", tuple->value);
		printf( "address: 0x%x ", tuple->address);
		printf( "mask: 0x%x ", tuple->mask);
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
 *	create the next label
 */
int	next_label( char *buffer)
{
	sprintf( buffer, "label%d", data.label++);
	return( strlen( buffer) + 1);
}

/*******************************************************************************
 *
 *	tuple parser production routines
 *
 ******************************************************************************/
/*
 *	convert and expression into tuple
		$$.tuple = tuple_and_expr( I_AND, $1.tuple, $3.tuple);
 */
TUPLE	*tuple_and_expr( int instr, TUPLE *and_expr, TUPLE *equality_expr)
{
	and_expr->token = instr;
	if( IS_FLAGS_ADDRESS( data.flags))
	    and_expr->mask = MASK_ADDRESS | MASK_W_REG;
	else
	    and_expr->mask = MASK_LABEL | MASK_W_REG;
	tuple_tail_to_head( equality_expr, and_expr);
	return( equality_expr);
}

/*
 *	convert selection statement into tuple
		$$.tuple = tuple_selection_statement( $3.tuple, $5.tuple, 0);
		$$.tuple = tuple_selection_statement( $3.tuple, $5.tuple, $7.tuple);
 */
TUPLE	*tuple_selection_statement( TUPLE *expr, TUPLE *statement1, TUPLE *statement2)
{
	char	buffer1[ 32];
	int	length1;
	char	buffer2[ 32];
	int	length2;
	TUPLE	*tuple;

	length1 = next_label( buffer1);
	tuple = end_tuple_list( expr);
	tuple->next = new_tuple( I_SUB, 0, 0, MASK_VALUE, 0, 0);
	tuple->next->next = new_tuple( I_BTFSC, 0x02, 0x03, MASK_ADDRESS | MASK_VALUE, 0, 0);
	tuple->next->next->next = new_tuple( I_GOTO, 0, 0, MASK_LABEL, buffer1, length1);
	tuple_tail_to_head( tuple, statement1);
	tuple = end_tuple_list( statement1);
	if( ! statement2)
		tuple->next = new_tuple( I_LABEL, length1, 0, MASK_LABEL, buffer1, length1);
	else
	{
		length2 = next_label( buffer2);
		tuple->next = new_tuple( I_GOTO, 0, 0, MASK_LABEL, buffer2, length2);
		tuple->next->next = new_tuple( I_LABEL, length1, 0, MASK_LABEL, buffer1, length1);
		tuple_tail_to_head( tuple, statement2);
		tuple = end_tuple_list( statement2);
		tuple->next = new_tuple( I_LABEL, length2, 0, MASK_LABEL, buffer2, length2);
	}
	return( expr);
}

/*
 *	iteration selection statement into tuple
		$$.tuple = tuple_iteration_statement( $3.tuple, $5.tuple);
 */
TUPLE	*tuple_iteration_statement( TUPLE *expr, TUPLE *statement)
{
	char	buffer1[ 32];
	int	length1;
	char	buffer2[ 32];
	int	length2;
	TUPLE	*tuple;
	TUPLE	*tuple2;
/*
 *	create the next temporary label
 */
	length1 = next_label( buffer1);
	length2 = next_label( buffer2);
	tuple = new_tuple( I_LABEL, length1, 0, MASK_LABEL, buffer1, length1);
	tuple->next = expr;
	tuple2 = end_tuple_list( expr);
	tuple2->next = new_tuple( I_SUB, 0, 0, MASK_VALUE, 0, 0);
	tuple2->next->next = new_tuple( I_BTFSC, 0x02, 0x03, MASK_ADDRESS | MASK_VALUE, 0, 0);
	tuple2->next->next->next = new_tuple( I_GOTO, 0, 0, MASK_LABEL, buffer2, length2);
	tuple_tail_to_head( tuple2, statement);
	tuple2 = end_tuple_list( statement);
	tuple2->next = new_tuple( I_GOTO, 0, 0, MASK_LABEL, buffer1, length1);
	tuple2->next->next = new_tuple( I_LABEL, length2, 0, MASK_LABEL, buffer2, length2);
	return( tuple);
}

/*
 *	convert jump statement goto into tuple
		$$.tuple = tuple_jump_statement_goto( $2.tuple);
 */
TUPLE	*tuple_jump_statement_goto( TUPLE *identifier)
{
	identifier->token = I_GOTO;
	identifier->mask = MASK_LABEL;
	return( identifier);
}

/*
 *	convert labeled statement into tuple
		$$.tuple = tuple_labeled_statement( $2.tuple);
 */
TUPLE	*tuple_labeled_statement( TUPLE *identifier, TUPLE *statement)
{
	identifier->token = I_LABEL;
	identifier->mask = MASK_LABEL;
	identifier->value = 1;	/* set this to not generate a return in front of this label */
	tuple_tail_to_head( identifier, statement);
	return( identifier);
}

/*
 *	convert postfix expression into tuple
		$$.tuple = tuple_postfix_expr( $1.tuple, 0);
		$$.tuple = tuple_postfix_expr( $1.tuple, $3.tuple);
 */
TUPLE	*tuple_postfix_expr( TUPLE *postfix_expr, TUPLE *argument_expr_list)
{
	TUPLE	*lists;
	TUPLE	*tuple_next;
/*
 *	special case is printf, after each argument insert call to printf
 */
	if( ! strcmp( postfix_expr->buffer, "printf"))
	{
		for( lists = argument_expr_list; lists; )
		{
			if( lists->mask & MASK_W_REG || lists->mask & MASK_PRINT)
			{
				tuple_next = lists->next;
				lists->next = new_tuple( I_CALL, 0, 0, MASK_LABEL, "printf", sizeof( "printf") + 1); /* call printf */
				lists->next->next = tuple_next;
				lists = lists->next->next;
			}
			else
			{
				lists = lists->next;
			}
		}
		free_tuple_list( postfix_expr);
		return( argument_expr_list);
	}
	postfix_expr->token = I_CALL;
	postfix_expr->mask = MASK_LABEL;
	tuple_tail_to_head( postfix_expr, argument_expr_list);
	return( postfix_expr);
}

/*
 *	convert additive expression into tuple
		$$.tuple = tuple_additive_expr( I_ADD, $1.tuple, $3.tuple);
		$$.tuple = tuple_additive_expr( I_SUB, $1.tuple, $3.tuple);
 */
TUPLE	*tuple_additive_expr( int instr, TUPLE *additive_expr, TUPLE *multiplicative_expr)
{
	additive_expr->token = instr;
	if( IS_FLAGS_ADDRESS( data.flags))
	    additive_expr->mask = MASK_ADDRESS | MASK_W_REG;
	else
	    additive_expr->mask = MASK_LABEL | MASK_W_REG;
	tuple_tail_to_head( multiplicative_expr, additive_expr);
	return( multiplicative_expr);
}

/*
 *	convert assignment expression into tuple
		$$.tuple = tuple_assignment_expr( $1.tuple, $2.token, $3.tuple);
 */
TUPLE	*tuple_assignment_expr( TUPLE *unary_expr, int assignment_operator, TUPLE *assignment_expr)
{
	unary_expr->token = I_MOV;
	if( IS_FLAGS_ADDRESS( data.flags))
	    unary_expr->mask = MASK_ADDRESS;
	else
	    unary_expr->mask = MASK_LABEL;
	tuple_tail_to_head( assignment_expr, unary_expr);
	return( assignment_expr);
}

/*
 *	if main( int argc, char *argv[]) then delete parameters
		$$.tuple = tuple_parameter_list( $1.tuple, $3.tuple);
 */
TUPLE	*tuple_parameter_list( TUPLE *parameter_list, TUPLE *parameter_declaration)
{
	free_tuple_list( parameter_list);
	free_tuple_list( parameter_declaration);
	return( (TUPLE*)0);
}

/*
 *	convert jump statement into tuple
		$$.tuple = tuple_jump_statement( $2.tuple);
 */
TUPLE	*tuple_jump_statement( TUPLE *return_expr)
{
	TUPLE	*tuple;

	tuple = end_tuple_list( return_expr);
	if( tuple->token == I_MOV && tuple->mask == MASK_VALUE)
		tuple->token = I_RETLW;
	else
		tuple->next = new_tuple( I_RETURN, 0, 0, MASK_INSTR, 0, 0);
	return( return_expr);
}

/*
 *	convert function definition into tuple 
		$$.tuple = tuple_function_definition( 0, $1.tuple, $2.tuple);
		$$.tuple = tuple_function_definition( $1.token, $2.tuple, $3.tuple);
 */
TUPLE	*tuple_function_definition( int declaration_specifiers, TUPLE *declarator, TUPLE *function_body)
{
	TUPLE	*tuple;

	if( ! strcmp( declarator->buffer, "main"))
		SET_FLAGS_MAIN( data.flags);
	declarator->token = I_LABEL;
	declarator->mask = MASK_LABEL;
	tuple = tuple_tail_to_head( declarator, function_body);
	return( tuple);
}

/*
 *	convert primary expression identifier into tuple
		$$.tuple = tuple_primary_expr_identifier( $1.tuple);
 */
TUPLE	*tuple_primary_expr_identifier( TUPLE *tuple)
{
	TUPLE	*symbol;
/*
 *	find the identifier in the symbol table (shall already be there!)
 */
	symbol = find_symbol( 0, tuple->buffer, tuple->length);
/*
 *	generate the intermediate code to mov address to w
 */
	if( symbol)
	{
		tuple->token = I_MOV;
		tuple->address = symbol->address;
		tuple->level = symbol->level;
		if( IS_FLAGS_ADDRESS( data.flags))
			tuple->mask = MASK_ADDRESS | MASK_W_REG;
		else
			tuple->mask = MASK_LABEL | MASK_W_REG;
	}
	return( tuple);
}

/*
 *	convert primary expression constant into tuple
		$$.tuple = tuple_primary_expr_constant( $1.tuple);
 */
TUPLE	*tuple_primary_expr_constant( TUPLE *tuple)
{
/*
 *	generate the intermediate code to mov constant value to w
 */
	tuple->token = I_MOV;
	tuple->mask = MASK_VALUE;
	return( tuple);
}

/*
 *	convert primary expression string literal into tuple
		$$.tuple = tuple_primary_expr_string_literal( $1.tuple);
 */
TUPLE	*tuple_primary_expr_string_literal( TUPLE *tuple)
{
	int index;
	TUPLE	*tuple_head = 0;
	TUPLE	*lists;
/*
 *	loop over string literal length to move char to w_reg then to memory address
 *	post processing will insert a call printf instruction for each character
 */
#ifdef	yuck
	for( index = 0; index < tuple->length; index++)
	{
		lists = new_tuple( I_MOV, tuple->buffer[ index], 0, MASK_VALUE | MASK_PRINT, 0, 0);
		tuple_head = tuple_tail_to_head( tuple_head, lists);
	}
#endif
	free_tuple( tuple);
	return( tuple_head);
}

/*
 *	generate initialize code for this list of identifiers
		$$.tuple = tuple_declaration( $1.token, $$.tuple);
 */
TUPLE	*tuple_declaration( int declaration_specifiers, TUPLE *init_declarator_list)
{
	TUPLE	*lists;
	TUPLE	*symbol;
	TUPLE	*tuple;
	TUPLE	*tuple_head = 0;
/*
 *	can be a list of identifiers
 */
	for( lists = init_declarator_list; lists; lists = lists->next)
	{
/*
 *	find the identifier in the symbol table (shall already be there!)
 */
		symbol = find_symbol( 0, lists->buffer, lists->length);
/*
 *	generate the intermediate code to initialize the variables
 */
		if( symbol)
		{
			if( symbol->level == 0 || symbol->value)
			{
				tuple = new_tuple( I_MOV, symbol->value, 0, MASK_VALUE, 0, 0);
				tuple_head = tuple_tail_to_head( tuple_head, tuple);
			}
			if( IS_FLAGS_ADDRESS( data.flags))
				tuple = new_tuple( I_MOV, 0, symbol->address, MASK_ADDRESS, 0, 0);
			else
				tuple = new_tuple( I_MOV, 0, 0, MASK_LABEL, symbol->buffer, symbol->length);
			tuple->level = symbol->level;
			tuple_head = tuple_tail_to_head( tuple_head, tuple);
		}
	}
	free_tuple_list( init_declarator_list);
	return( tuple_head);
}

