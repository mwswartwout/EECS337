/*******************************************************************************
*
* FILE:		attribute.c
*
* DESC:		EECS 337 Assignment 4
*
* AUTHOR:	mws85
*
* DATE:		September 23, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
#include	"yystype.h"
/*
 *	attribute DEBUG functions
 */
#ifdef	YYDEBUG

/*
 *	print the attribute 
 */
void	print_attribute( int index)
{
	ATTRIBUTE current = data.attributes[index];
	
	if (current.token == CONSTANT)
	{
		switch (current.format)
		{
			case FORMAT_CHAR:
				fprintf( stdout, "token: CONSTANT\tbuffer: %s\tlength: %d\tformat: CHAR\t char:%s\n", current.buffer, current.length, current.buffer);
				break;
			case FORMAT_DECIMAL:
				fprintf( stdout, "token: CONSTANT\tbuffer: %s\tlength: %d\tformat: DECIMAL\t decimal:%s\n", current.buffer, current.length, current.buffer);
				break;
			case FORMAT_HEXADECIMAL:
				fprintf( stdout, "token: CONSTANT\tbuffer: %s\tlength: %d\tformat: HEXADECIMAL\t hex:%s\n", current.buffer, current.length, current.buffer);
				break;
			case FORMAT_OCTAL:
				fprintf( stdout, "token: CONSTANT\tbuffer: %s\tlength: %d\tformat: OCTAL\t octal:%s\n", current.buffer, current.length, current.buffer);
				break;
			case FORMAT_FLOAT:
				fprintf( stdout, "token: CONSTANT\tbuffer: %s\tlength: %d\tformat: FLOAT\t float:%s\n", current.buffer, current.length, current.buffer);
				break;
			default:
				break;
		}
	}
	else if (current.token == IDENTIFIER)
		fprintf( stdout, "token: IDENTIFIER\tbuffer: %s\tlength: %d\tformat: NONE\n", current.buffer, current.length);
	else if (current.token == STRING_LITERAL)
		fprintf( stdout, "token: STRING_LITERAL\tbuffer: %s\tlength: %d\tformat: NONE\n", current.buffer, current.length);
		
		
	
	return;
}
/*
 *	print the attriube table
 */
void	print_attribute_table( void)
{
	int i;
	for (i = 0; i < data.index; i++)
	{
		print_attribute( i);
	}
	return;
}
#endif

/*
 *	scanner attribute register function
 *	return an index into the attribute table (static)
 */
int	attribute( int token, char *buffer, unsigned int length, int format)
{
/*
 *	find the same string and return the index
 */
/*
 *	else update to the next attribute field
 */
/*
 *	encode the constant string into a value
 */
	int i;
	for (i = 0; i < MAX_ATTRIBUTES; i++)
	{
		if( !strncmp( buffer, data.attributes[i].buffer, length))
		{
			return i;
		}
	}
	
	data.index++;
	data.attributes[data.index].token = token;
	strcpy( data.attributes[data.index].buffer, buffer);
	data.attributes[data.index].length = length;
	data.attributes[data.index].format = format;
	return data.index;
};

