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
*		Updated for EECS 337 Assignment 9 November 5, 2013 
*		Updated for EECS 337 Assignment 10 November 12, 2013 
*		Updated for EECS 337 Assignment 11 November 19, 2013 
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
 *	define the relational operator strings
 */
char	*relational[] =
{
	"<=",
	">=",
	"==",
	"!=",
};

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
	case '<':
	case '>':
	     printf( "\t");
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " = ");
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     printf( " %c ", quad->operator);
	     print_quad_operand( quad->op2_type, quad->op2_index);
	     break;
	case LE:
	case GE:
	case EQ:
	case NE:
	     printf( "\t");
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " = ");
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     printf( " %s ", relational[ quad->operator - LE]);
	     print_quad_operand( quad->op2_type, quad->op2_index);
	     break;
	case UMINUS:
	     printf( "\t");
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " = - ");
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     break;
	case '~' :
	case '!' :
	     printf( "\t");
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " = %c ", quad->operator);
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     break;
	case '=':
	     printf( "\t");
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " = ");
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     break;
	case IFTRUE:
	     printf( "\t");
	     printf( "IFTRUE ");
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " GOTO ");
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     break;
	case IFFALSE:
	     printf( "\t");
	     printf( "IFFALSE ");
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " GOTO ");
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     break;
	case LABEL:
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( ": ");
	     break;
	case GOTO:
	     printf( "\t");
	     printf( "GOTO ");
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     break;
	case '[':
	     printf( "\t");
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " [ ");
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     printf( " ] = ");
	     print_quad_operand( quad->op2_type, quad->op2_index);
	     break;
	case ']':
	     printf( "\t");
	     print_quad_operand( quad->dst_type, quad->dst_index);
	     printf( " = ");
	     print_quad_operand( quad->op1_type, quad->op1_index);
	     printf( " [ ");
	     print_quad_operand( quad->op2_type, quad->op2_index);
	     printf( " ] ");
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
	QUAD	*quad1;
	QUAD	*quad2;
/*
 *	check if optimize quad
 */
	if( q1->operator == '=' && q1->dst_type == TYPE_TEMPORARY && ( q1->op1_type == TYPE_IDENTIFIER || q1->op1_type == TYPE_CONSTANT) &&
	    q2->operator == '=' && q2->dst_type == TYPE_TEMPORARY && ( q2->op1_type == TYPE_IDENTIFIER || q2->op1_type == TYPE_CONSTANT))
	{
		data.temp--;
		data.temp--;
		quad1 = new_quad( operator, TYPE_TEMPORARY, next_temp(), q1->op1_type, q1->op1_index, q2->op1_type, q2->op1_index);
		free_quad( q1);
		free_quad( q2);
		return quad1;
	}
/*
 *	check if left side is optimize quad
 */
	if( q1->operator == '=' && q1->dst_type == TYPE_TEMPORARY && ( q1->op1_type == TYPE_IDENTIFIER || q1->op1_type == TYPE_CONSTANT))
	{
		quad2 = end_quad_list( q2);
		data.temp--;
		quad2->next = new_quad( operator, TYPE_TEMPORARY, next_temp(), q1->op1_type, q1->op1_index, quad2->dst_type, quad2->dst_index);
		free_quad( q1);
		return q2;
	}
/*
 *	check if right side is optimize quad
 */
	if( q2->operator == '=' && q2->dst_type == TYPE_TEMPORARY && ( q2->op1_type == TYPE_IDENTIFIER || q2->op1_type == TYPE_CONSTANT))
	{
		quad1 = end_quad_list( q1);
		data.temp--;
		quad1->next = new_quad( operator, TYPE_TEMPORARY, next_temp(), quad1->dst_type, quad1->dst_index, q2->op1_type, q2->op1_index);
		free_quad( q2);
		return q1;
	}
/*
 *	do the previous thing
 */
	quad1 = end_quad_list( q1);
	quad1->next = q2;
	quad2 = end_quad_list( q2);
	quad2->next = new_quad( operator, TYPE_TEMPORARY, next_temp(), quad1->dst_type, quad1->dst_index, quad2->dst_type, quad2->dst_index);
	return q1;
}

/*
 *	allocate a quad2 function
		$$.quad = new_quad2( '~', $1.quad);
 */
QUAD	*new_quad2( int operator, QUAD *q1)
{
	QUAD	*quad1;
/*
 *	check if optimize quad
 */
	if( q1->operator == '=' && q1->dst_type == TYPE_TEMPORARY && ( q1->op1_type == TYPE_IDENTIFIER || q1->op1_type == TYPE_CONSTANT))
	{
		data.temp--;
		quad1 = new_quad( operator, TYPE_TEMPORARY, next_temp(), q1->op1_type, q1->op1_index, 0, 0);
		free_quad( q1);
		return quad1;
	}
/*
 *	do the previous thing
 */
	quad1 = end_quad_list( q1);
	quad1->next = new_quad( operator, TYPE_TEMPORARY, next_temp(), quad1->dst_type, quad1->dst_index, 0, 0);
	return q1;
}
/*
 *	allocate a quad3 function
		$$.quad = new_quad3( '=', $1.index, 0);
		$$.quad = new_quad3( '=', $1.index, $3.quad);
 */
QUAD	*new_quad3( int operator, int index, QUAD *q1)
{
	QUAD	*quad1;

	if( ! q1)
	{
		quad1 = new_quad( operator, TYPE_TEMPORARY, next_temp(), data.st[ index].type, index, 0, 0);
		return quad1;
	}
	quad1 = end_quad_list( q1);
/*
 *	check if optimize quad
 */
	if( quad1->dst_type == TYPE_TEMPORARY)
	{
		data.temp--;
		quad1->dst_type = data.st[ index].type;
		quad1->dst_index = index;
		return q1;
	}
/*
 *	do the previous thing
 */
	quad1->next = new_quad( operator, data.st[ index].type, index, quad1->dst_type, quad1->dst_index, 0, 0);
	return q1;
}

/*
 *	allocate a quad5 function
	  	$$.quad = new_quad5( IFTRUE, $3.quad, $5.quad, 0);
		$$.quad = new_quad5( IFTRUE, $3.quad, $5.quad, $7.quad);
 */
QUAD	*new_quad5( int operator, QUAD *q1, QUAD *q2, QUAD *q3)
{
	QUAD	*quad1;
	QUAD	*quad2;
	QUAD	*quad3;
	int	label1;
	int	label2;

	if( ! q3)
	{
		quad1 = end_quad_list( q1);
/*
 *	backpatch ! quad into iffalse quad
 */
		if( quad1->operator == '!')
		{
			label1 = next_label();
			quad1->operator = IFFALSE;
			quad1->dst_type = quad1->op1_type;
			quad1->dst_index = quad1->op1_index;
			quad1->op1_type = TYPE_LABEL;
			quad1->op1_index = label1;
			quad1->next = q2;
		}
		else
		{
			label1 = next_label();
			quad1->next = new_quad( operator, quad1->dst_type, quad1->dst_index, TYPE_LABEL, label1, 0, 0);
			quad1->next->next = q2;
		}
		quad2 = end_quad_list( q2);
		quad2->next = new_quad( LABEL, TYPE_LABEL, label1, 0, 0, 0, 0);
	}
	else
	{
		quad1 = end_quad_list( q1);
		label1 = next_label();
		label2 = next_label();
		quad1->next = new_quad( operator, quad1->dst_type, quad1->dst_index, TYPE_LABEL, label1, 0, 0);
		quad1->next->next = q2;
		quad2 = end_quad_list( q2);
		quad2->next = new_quad( GOTO, TYPE_LABEL, label2, 0, 0, 0, 0);
		quad2->next->next = new_quad( LABEL, TYPE_LABEL, label1, 0, 0, 0, 0);
		quad2->next->next->next = q3;
		quad3 = end_quad_list( q3);
		quad3->next = new_quad( LABEL, TYPE_LABEL, label2, 0, 0, 0, 0);
	}
	return q1;
}

/*
 *	allocate a quad8 function
		$$.quad = new_quad8( ']', $1.index, $3.quad, 0);
		$$.quad = new_quad8( '[', $1.index, $3.quad, $6.quad);
 */
QUAD	*new_quad8( int operator, int index, QUAD *q1, QUAD *q2)
{
	QUAD	*quad1;
	QUAD	*quad2;

	if( ! q2)
	{
		quad1 = end_quad_list( q1);
		if( data.st[ index].specifier == SPECIFIER_CHAR)
		{
/*
 *	check if optimize quad
 */
			if( q1->operator == '=' && q1->dst_type == TYPE_TEMPORARY && ( q1->op1_type == TYPE_IDENTIFIER || q1->op1_type == TYPE_CONSTANT))
			{
				data.temp--;
				quad1 = new_quad( operator, TYPE_TEMPORARY, next_temp(), data.st[ index].type, index, q1->op1_type, q1->op1_index);
				free_quad( q2);
				return quad1;
			}
			else
				quad1->next = new_quad( operator, TYPE_TEMPORARY, next_temp(), data.st[ index].type, index, quad1->dst_type, quad1->dst_index);
		}
		else
		{
			quad1->next = new_quad( '*', TYPE_TEMPORARY, next_temp(), quad1->dst_type, quad1->dst_index, TYPE_CONSTANT, data.st[ index].sizeofspecifier );
			quad1->next->next = new_quad( operator, TYPE_TEMPORARY, next_temp(), data.st[ index].type, index, quad1->next->dst_type, quad1->next->dst_index);
		}
	}
	else
	{
		quad1 = end_quad_list( q1);
		quad2 = end_quad_list( q2);
		if( data.st[ index].specifier == SPECIFIER_CHAR)
		{
/*
 *	check if optimize quad
 */
			if( q1->operator == '=' && q1->dst_type == TYPE_TEMPORARY && ( q1->op1_type == TYPE_IDENTIFIER || q1->op1_type == TYPE_CONSTANT) &&
	    		    q2->operator == '=' && q2->dst_type == TYPE_TEMPORARY && ( q2->op1_type == TYPE_IDENTIFIER || q2->op1_type == TYPE_CONSTANT))
			{
				data.temp--;
				data.temp--;
				quad1 = new_quad( operator, data.st[ index].type, index, q1->op1_type, q1->op1_index, q2->op1_type, q2->op1_index);
				free_quad( q1);
				free_quad( q2);
				return quad1;
			}
			else
			{
				quad1->next = q2;
				quad2->next = new_quad( operator, data.st[ index].type, index, quad1->dst_type, quad1->dst_index, quad2->dst_type, quad2->dst_index);
			}
		}
		else
		{
			quad1->next = new_quad( '*', TYPE_TEMPORARY, next_temp(), quad1->dst_type, quad1->dst_index, TYPE_CONSTANT, data.st[ index].sizeofspecifier );
			quad1->next->next = q2;
			quad2->next = new_quad( operator, data.st[ index].type, index, quad1->next->dst_type, quad1->next->dst_index, quad2->dst_type, quad2->dst_index);
		}
	}
	return q1;
}

