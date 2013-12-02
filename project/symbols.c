/*******************************************************************************
*
* FILE:		symbols.c
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
#include "yystype.h"
/*******************************************************************************
 *
 *	symbol table routines
 *
 ******************************************************************************/
/*
 *	print the symbol table
 */
void	print_symbol_table( void)
{
	printf( "symbol table:\n");
	print_tuple_list( data.symbol_table);
	printf( "symbol table free:\n");
	print_tuple_list( data.symbol_table_free);
}

/*
 *	find the identifier in the symbol table linked list at this level
 */
TUPLE	*find_symbol( int level, char *text, int length)
{
	TUPLE	*lists;
/*
 *	search the tuple list using the buffer length and string compare to match
 */
	for( lists = data.symbol_table; lists; lists = lists->next)
	{
		if( lists->level < level)
			return( (TUPLE*)0);
		if( lists->length == length && ! strcmp( lists->buffer, text))
			return( lists);
	}
	return( lists);
}

/*
 *	create a symbol table entry unless one already exists 
 */
void	create_symbol( int type, unsigned char value, char *buffer, int length)
{
	TUPLE	*tuple;
/*
 *	first search for same identifier string
 */
	tuple = find_symbol( data.level, buffer, length);
/*
 *	update symbol type entry or make new symbol table entry
 */
	if( tuple)
	{
		if( 0 < type)
			tuple->token = type;
		if( 0 < value)
			tuple->value = value;
		return;
	}
	tuple = new_tuple( type, value, get_address( 1), 0, buffer, length);
	tuple->level = data.level;
/*
 *	attach it to the head of the symbol table list (LIFO)
 */
	tuple->next = data.symbol_table;
	data.symbol_table = tuple;
	return;
}

/*******************************************************************************
 *
 *	symbol parser production routines
 *
 ******************************************************************************/
/*
 *	type an identifier in symbol table
		$$.tuple = symbol_declaration( $1.token, $2.tuple);
 */
TUPLE	*symbol_declaration( int type_specifier, TUPLE *init_declarator_list)
{
	TUPLE	*lists;
/*
 *	can be a list of identifiers
 */
	for( lists = init_declarator_list; lists; lists = lists->next)
	{
/*
 *	create a symbol with this type
 */
		create_symbol( type_specifier, 0, lists->buffer, lists->length);
	}
/*
 *	return init_declarator_list linked list
 */
	return( init_declarator_list);
}

/*
 *	update the value from last identifier and attach the tuple
		$$.tuple = symbol_init_declarator_list( $1.tuple, $3.tuple);
 */
TUPLE	*symbol_init_declarator_list( TUPLE *init_declarator_list, TUPLE *init_declarator)
{
	TUPLE	*lists;
/*
 *	can be a list of identifiers
 */
	for( lists = init_declarator_list; lists; lists = lists->next)
	{
/*
 *	create a symbol with this value
 */
		if( init_declarator)
		    create_symbol( 0, init_declarator->value, lists->buffer, lists->length);
		else
		    create_symbol( 0, 0, lists->buffer, lists->length);
	}
/*
 *	link in the last init_declarator tuple, return head of list
 */
	tuple_tail_to_head( init_declarator_list, init_declarator);
	return( init_declarator_list);
}

/*
 *	update a value of an identifier in symbol table
		$$.tuple = symbol_init_declarator( $1.tuple, $3.tuple);
 */
TUPLE	*symbol_init_declarator( TUPLE *declarator, TUPLE *initializer)
{
	TUPLE	*lists;
/*
 *	can be a list of identifiers
 */
	for( lists = declarator; lists; lists = lists->next)
	{
/*
 *	create a symbol with this value and copy it to identifier list
 */
		if( initializer)
		{
			create_symbol( 0, initializer->value, lists->buffer, lists->length);
			lists->value = initializer->value;
		}
		else
			create_symbol( 0, 0, lists->buffer, lists->length);
	}
/*
 *	free the initializer linked list
 */
	free_tuple_list( initializer);
	return( declarator);
}

/*
 *	new symbol table identifiers at this level
		symbol_left_bracket();
 */
void	symbol_left_bracket( void)
{
/*
 *	increment the symbol table level
 */
	data.level++;
	return;
}

/*
 *	pop symbol table identifiers at this level
		symbol_right_bracket();
 */
void	symbol_right_bracket( void)
{
	TUPLE	*tuple;
/*
 *	print the complete symbol table before poping symbols off
 */
#ifdef	YYDEBUG
	if( IS_FLAGS_SYMBOL( data.flags))
	{
		printf( "pop symbol table level: %d\n", data.level);
		print_symbol_table();
	}
#endif
/*
 *	check and remove all tuple above level
 */
	for( tuple = data.symbol_table; tuple; tuple = data.symbol_table)
	{
		if( tuple->level < data.level)
			break;
		put_address( tuple->address, 1);
		data.symbol_table = tuple->next;
		tuple->next = data.symbol_table_free;
		data.symbol_table_free = tuple;
	}
/*
 *	decrement the symbol table level
 */
	data.level--;
	return;
}

