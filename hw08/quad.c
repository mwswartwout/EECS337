/*******************************************************************************
*
* FILE:		quad.c
*
* DESC:		EECS 337 Assignment 8
*
* AUTHOR:	caseid
*
* DATE:		October 29, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>
#include	<string.h>
#include 	<ctype.h>
#include 	"yystype.h"
#include 	"y.tab.h"

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
 *	allocate a quad data structure
 */
QUAD	*new_quad( int operator, int dst_type, int dst_index, int op1_type, int op1_index, int op2_type, int op2_index)
{
	QUAD *quad;

	if( !(quad = ( QUAD*)malloc( sizeof( QUAD))))
		new_error( sizeof( QUAD));
	else
	{
		memset( (void *)quad, 0, (size_t)sizeof( QUAD));
		quad->operator = operator;
		quad->dst_type = dst_type;
		quad->dst_index = dst_index;
		quad->op1_type = op1_type;
		quad->op1_index = op1_index;
		quad->op2_type = op2_type;
		quad->op2_index = op2_index;
		data.memory += sizeof( QUAD);
	}
	return( quad);
}

/*
 *	deallocate a quad data structure
 */
void	free_quad( QUAD *quad)
{
	if( quad)
	{
		free( quad);
		data.memory -= sizeof( QUAD);
	}
	return;
}

/*
 *	deallocate a quad linked list
 */
void	free_quad_list( QUAD *quad_list)
{
	QUAD	*quad_next;
/*
 *	deallocate the quad linked list structure
 */
	while( quad_list)
	{
		quad_next = quad_list->next;
		free_quad( quad_list);
		quad_list = quad_next;
	}
/*
 *	set the temporary variable back to zero
 */
	data.temp = 0;
	return;
}

/*
 *	find the last quad structure in linked list
 */
QUAD	*end_quad_list( QUAD *quad_list)
{
	if( quad_list)
		while( quad_list->next)
			quad_list = quad_list->next;
	return quad_list;
}

/*
 *	print a quad operand structure
 */
void	print_quad_operand( int type, int index)
{
/*
 *	switch on the operand type
 */
	switch( type)
	{
	case TYPE_IDENTIFIER:
	case TYPE_CONSTANT:
	     printf( "%s", data.st[ index].buffer);
	     break;
	case TYPE_TEMPORARY:
	     printf( "t%d", index);
	     break;
	case TYPE_LABEL:
	     printf( "label%d", index);
	     break;
	}
	return;
}

/*
 *	print a quad structure
 */
void	print_quad( QUAD *quad)
{
	switch( quad->operator)
	{
	case '|':
	case '^':
	case '&':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " = ");
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     printf( " %c ", quad->operator);
	     print_quad_operand( quad->op2_type, quad->op2_index);
	     break;
	case UMINUS:
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " = - ");
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     break;
	case '~' :
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " = ~ ");
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     break;
	case '=':
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " = ");
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     break;
	}
	printf( "\n");
	//	printf( "\nnext: %08.8x\n", (int)quad->next);
	return;
}

/*
 *	print a quad linked list
 */
void	print_quad_list( QUAD *quad_list)
{
	QUAD	*quad;
/*
 *	print the quad linked list structure
 */
	for( quad = quad_list; quad; quad = quad->next)
	{
		print_quad( quad);
	}
	return;
}

/*
 *	allocate the next temporary location
 */
int	next_temp( void)
{
	return ++data.temp;
}

/*
 *	allocate the next label location
 */
int	next_label( void)
{
	return ++data.label;
}

/*
 *	allocate a quad1 function
		$$.quad = new_quad1( '+', $1.quad, $3.quad);
 */
QUAD	*new_quad1( int operator, QUAD *q1, QUAD *q2)
{
	return q1;
}

/*
 *	allocate a quad2 function
		$$.quad = new_quad2( '~', $1.quad);
 */
QUAD	*new_quad2( int operator, QUAD *q1)
{
	return q1;
}
/*
 *	allocate a quad3 function
		$$.quad = new_quad3( '=', $1.index, $3.quad);
 */
QUAD	*new_quad3( int operator, int index, QUAD *q1)
{
	return q1;
}
