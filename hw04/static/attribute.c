/*******************************************************************************
*
* FILE:		attribute.c
*
* DESC:		EECS 337 Assignment 4
*
* AUTHOR:	caseid
*
* DATE:		September 17, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
#include	"yystype.h"
/*
 *	attribute DEBUG functions
 */
#ifdef	YYDEBUG
char	*format_types[] =
{
	"NONE",
	"CHAR",
	"DECIMAL",
	"HEXADECIMAL",
	"OCTAL",
	"FLOAT",
};

/*
 *	print the attribute 
 */
void	print_attribute( int index)
{
	printf( "token: %s\t", tokens[ (data.attributes[ index].token - IDENTIFIER) ]);
	printf( "buffer: %s\t", data.attributes[ index].buffer);
	printf( "length: %d\t", data.attributes[ index].length);
	printf( "format: %s\t", format_types[ data.attributes[ index].format]);
	switch( data.attributes[ index].format)
	{
	case FORMAT_NONE:
	     break;
	case FORMAT_CHAR:
	     printf( "char: %c\t", data.attributes[ index].value.cvalue);
	     break;
	case FORMAT_DECIMAL:
	     printf( "decimal: %Lu\t", data.attributes[ index].value.lvalue);
	     break;
	case FORMAT_HEXADECIMAL:
	     printf( "hex: %Lx\t", data.attributes[ index].value.lvalue);
	     break;
	case FORMAT_OCTAL:
	     printf( "octal: %Lo\t", data.attributes[ index].value.lvalue);
	     break;
	case FORMAT_FLOAT:
	     printf( "float: %Lf\t", data.attributes[ index].value.dvalue);
	     break;
	}
	printf( "\n");
	return;
}
/*
 *	print the attriube table
 */
void	print_attribute_table( void)
{
	int	index;

	printf( "attribute table: \n");
	for( index = 1; index <= data.index; index++)
		print_attribute( index);
	return;
}
#endif

/*
 *	scanner attribute register function
 *	return an index into the attribute table (static)
 */
int	attribute( int token, char *buffer, unsigned int length, int format)
{
	int	index;
/*
 *	find the same string and return the index
 */
	for( index = 1; index <= data.index; index++)
	     if( data.attributes[ index].token == token && data.attributes[ index].length == length + 1 && !strcmp( data.attributes[ index].buffer, buffer))
	     	 return index;
/*
 *	else update to the next attribute field
 */
	data.index++;
	data.attributes[ data.index].token = token;
	strcpy( data.attributes[ data.index].buffer, buffer);
	data.attributes[ data.index].length = length + 1;	// count the null for dynamic version
	data.attributes[ data.index].format = format;
/*
 *	encode the constant string into a value
 */
	switch( format)
	{
	case FORMAT_NONE:
	     break;
	case FORMAT_CHAR: // only works for 'X' not 'XX' or 'XXX' or 'XXXX' or '\X' or '\xHH' or '\DDD' 
	     if( buffer[ 2] == '\'')
	     	 buffer[ 2] = 0;
	     sscanf( &buffer[1], "%c", &data.attributes[ data.index].value.cvalue);
	     break;
	case FORMAT_DECIMAL:
	     sscanf( buffer, "%Lu", &data.attributes[ data.index].value.lvalue);
	     break;
	case FORMAT_HEXADECIMAL:
	     sscanf( buffer, "%Lx", &data.attributes[ data.index].value.lvalue);
	     break;
	case FORMAT_OCTAL:
	     sscanf( buffer, "%Lo", &data.attributes[ data.index].value.lvalue);
	     break;
	case FORMAT_FLOAT:
	     sscanf( buffer, "%Lf", &data.attributes[ data.index].value.dvalue);
	     break;
	}
	return data.index;
};

