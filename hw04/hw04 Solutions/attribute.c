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
 *	allocate an attribute structure
 */
ATTRIBUTE *new_attribute( int token, char *buffer, unsigned int length, int format)
{
	ATTRIBUTE *attribute;

	if( !(attribute = ( ATTRIBUTE*)malloc( sizeof( ATTRIBUTE))))
		new_error( sizeof( ATTRIBUTE));
/*
 *	initialize the structure
 */
	memset( (void *)attribute, 0, (size_t)sizeof( ATTRIBUTE));
	attribute->token = token;
	attribute->length = length + 1;	// count the null for dynamic version
	attribute->buffer = new_buffer( buffer, attribute->length);
	attribute->format = format;
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
	     sscanf( &buffer[1], "%c", &(attribute->value.cvalue));
	     break;
	case FORMAT_DECIMAL:
	     sscanf( buffer, "%Lu", &(attribute->value.lvalue));
	     break;
	case FORMAT_HEXADECIMAL:
	     sscanf( buffer, "%Lx", &(attribute->value.lvalue));
	     break;
	case FORMAT_OCTAL:
	     sscanf( buffer, "%Lo", &(attribute->value.lvalue));
	     break;
	case FORMAT_FLOAT:
	     sscanf( buffer, "%Lf", &(attribute->value.dvalue));
	     break;
	}
	data.memory += sizeof( ATTRIBUTE);
	return( attribute);
}

/*
 *	deallocate a attribute data structure
 */
void	free_attribute( ATTRIBUTE *attribute)
{
	if( attribute)
	{
		free_buffer( attribute->buffer, attribute->length);
		free( attribute);
		data.memory -= sizeof( ATTRIBUTE);
	}
	return;
}

/*
 *	deallocate a attribute linked list
 */
void	free_attribute_list( ATTRIBUTE *attribute)
{
	ATTRIBUTE	*attribute_next;
/*
 *	deallocate the attribute linked list structure
 */
	while( attribute)
	{
		attribute_next = attribute->next;
		free_attribute( attribute);
		attribute = attribute_next;
	}
	return;
}

/*
 *	free the attribute table 
 */
void	free_attribute_table( void)
{
	free_attribute_list( data.attributes);
	return;
}

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
void	print_attribute( ATTRIBUTE *attribute)
{
	if( ! attribute)
	    return;
	printf( "next: %p ", attribute->next);
	printf( "token: %s\t", tokens[ (attribute->token - IDENTIFIER) ]);
	printf( "buffer: %s\t", attribute->buffer);
	printf( "length: %d\t", attribute->length);
	printf( "format: %s\t", format_types[ attribute->format]);
	switch( attribute->format)
	{
	case FORMAT_NONE:
	     break;
	case FORMAT_CHAR:
	     printf( "char: %c\t", attribute->value.cvalue);
	     break;
	case FORMAT_DECIMAL:
	     printf( "decimal: %Lu\t", attribute->value.lvalue);
	     break;
	case FORMAT_HEXADECIMAL:
	     printf( "hex: %Lx\t", attribute->value.lvalue);
	     break;
	case FORMAT_OCTAL:
	     printf( "octal: %Lo\t", attribute->value.lvalue);
	     break;
	case FORMAT_FLOAT:
	     printf( "float: %Lf\t", attribute->value.dvalue);
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
	ATTRIBUTE	*attribute;

	printf( "attribute table: \n");
	for( attribute = data.attributes; attribute; attribute = attribute->next)
		print_attribute( attribute);
	return;
}
#endif

/*
 *	scanner attribute register function
 *	return pointer into the attribute table (dynamic)
 */
ATTRIBUTE *attribute( int token, char *buffer, unsigned int length, int format)
{
	ATTRIBUTE	*attribute;
/*
 *	find the same string and return the index
 */
	for( attribute = data.attributes; attribute; attribute = attribute->next)
	     if( attribute->token == token && attribute->length == length + 1 && !strcmp( attribute->buffer, buffer))
	     	 return attribute;
/*
 *	else update to the next attribute field
 */
	attribute = new_attribute( token, buffer, length, format);
	attribute->next = data.attributes;
	data.attributes = attribute;
	return attribute;
};

