/*******************************************************************************
*
* FILE:		audit.c
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
#include	"y.tab.h"
/*******************************************************************************
 *
 *	lowest level ansi_c audit routines
 *
 ******************************************************************************/
/*
 *	check for conversion error
 */
int	ansi_c_audit_conversion( unsigned char byte, unsigned int integer, char *text, int length)
{
	if( byte != integer)
	{
		fprintf( stderr, "Warning: data conversion error\n");
		data.warnings++;
	}
	return 0;
}

/*
 *	check for overflow condition
 */
int	ansi_c_audit_overflow( unsigned char byte, unsigned int integer, char *text, int length)
{
	if( 255 < integer)
	{
		fprintf( stderr, "Warning: overflow in implicit constant conversion\n");
		data.warnings++;
	}
	return 0;
}

/*
 *	check for multi character constant
 */
int	ansi_c_audit_multi_character( unsigned char byte, unsigned int integer, char *text, int length)
{
	switch( text[ 1])
	{
	case '\\':
		switch( text[ 2])
		{
		case 'n':	/* newline */
		case 'r':	/* cr */
		case 't':	/* tab */
		case 'b':	/* backspace */
		case '\\':	
		case '?':
		case '\'':
		case '"':
		case 'x':
		case 'X':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		     break;
		default:
			if( text[ 2] != '\'')
			{
				fprintf( stderr, "Warning:  multi-character character constant\n");
				data.warnings++;
			}
		}
	}
	return 0;
}

	int	i;
/*
 *	check the digits for octal
 */
int	ansi_c_audit_octal_digits( unsigned char byte, unsigned int integer, char *text, int length)
{
	for( i = length - 1; 0 <= i; i--)
	{
		switch( text[ i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case 'l':
		case 'L':
		case 'u':
		case 'U':
		     break;
		default:
		     fprintf( stderr, "Error: invalid digit \"%c\" in octal constant\n", text[ i]);
		     data.errors++;
		     return 1;
		}
	}
	return 0;
}

/*******************************************************************************
 *
 *	highest level ansi_c audit routines
 *
 ******************************************************************************/
/*
 *	audit the char constant
 */
void	ansi_c_audit_char( unsigned char byte, unsigned int integer, char *text, int length)
{
	if( ansi_c_audit_conversion( byte, integer, text, length))
		return;
	if( ansi_c_audit_overflow( byte, integer, text, length))
		return;
	if( ansi_c_audit_multi_character( byte, integer, text, length))
		return;
	return;
}

/*
 *	audit the decimal constant
 */
void	ansi_c_audit_decimal( unsigned char byte, unsigned int integer, char *text, int length)
{
	if( ansi_c_audit_conversion( byte, integer, text, length))
		return;
	if( ansi_c_audit_overflow( byte, integer, text, length))
		return;
	return;
}

/*
 *	audit the hex constant
 */
void	ansi_c_audit_hex( unsigned char byte, unsigned int integer, char *text, int length)
{
	if( ansi_c_audit_conversion( byte, integer, text, length))
		return;
	if( ansi_c_audit_overflow( byte, integer, text, length))
		return;
	return;
}

/*
 *	audit the octal constant
 */
void	ansi_c_audit_octal( unsigned char byte, unsigned int integer, char *text, int length)
{
	if( ansi_c_audit_conversion( byte, integer, text, length))
		return;
	if( ansi_c_audit_overflow( byte, integer, text, length))
		return;
	if( ansi_c_audit_octal_digits( byte, integer, text, length))
		return;
	return;
}

/*
 *	audit the float constant
 */
void	ansi_c_audit_float( unsigned char byte, float f, char *text, int length)
{
	int	integer;
/*
 *	check for floating point errors
 */
	integer = (int)f;
	if( ansi_c_audit_conversion( byte, integer, text, length))
		return;
	if( ansi_c_audit_overflow( byte, integer, text, length))
		return;
	return;
}

/*******************************************************************************
 *
 *	new scanner encoder routines now returning TUPLE data structure
 *
 ******************************************************************************/
/*
 *	encode a constant hex value
 */
TUPLE	*constant_hex( char *text, int length)
{
	TUPLE	*tuple;

	unsigned int	x = 0;
	sscanf( text, "%x", &x);
	tuple = new_tuple( CONST, (unsigned char)x, 0, 0, 0, 0);
	ansi_c_audit_hex( tuple->value, x, text, length);
	return( tuple);
}

/*
 *	encode a constant octal value
 */
TUPLE	*constant_octal( char *text, int length)
{
	TUPLE	*tuple;
	unsigned int	o = 0;
	sscanf( text, "%o", &o);
	tuple = new_tuple( CONST, (unsigned char)o, 0, 0, 0, 0);
	ansi_c_audit_octal( tuple->value, o, text, length);
	return( tuple);
}

/*
 *	encode a constant decimal value
 */
TUPLE	*constant_decimal( char *text, int length)
{
	TUPLE	*tuple;
	unsigned int	u = 0;
	sscanf( text, "%u", &u);
	tuple = new_tuple( CONST, (unsigned char)u, 0, 0, 0, 0);
	ansi_c_audit_decimal( tuple->value, u, text, length);
	return( tuple);
}

/*
 *	encode a constant character value
 */
TUPLE	*constant_char( char *text, int length)
{
	TUPLE	*tuple;
	unsigned int c = 0;
	switch( text[ 1])
	{
	case '\\':
		switch( text[ 2])
		{
		case 'n':	/* newline */
			c = 0x0a;
			break;
		case 'r':	/* cr */
			c = 0x0d;
			break;
		case 't':	/* tab */
			c = 0x09;
			break;
		case 'b':	/* backspace */
			c = 0x08;
			break;
		case '\\':	
			c = 0x5c;
			break;
		case '?':
			c = 0x3f;
			break;
		case '\'':
			c = 0x27;
			break;
		case '"':
			c = 0x22;
			break;
		case 'x':
		case 'X':
			text[1] = '0';
			sscanf( &text[1], "%x", &c);
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
			sscanf( &text[2], "%o", &c);
			break;
		default:
			{
				int i;
				for( i = 2; i < length; i++)
				{
					if( text[ i] == '\'')
						break;
					c = text[ i];
				}
			}
			break;
		}
		break;
	default:
		c = text[1];
		break;
	}
	tuple = new_tuple( CONST, (unsigned char)c, 0, 0, 0, 0);
	ansi_c_audit_char( tuple->value, c, text, length);
	return( tuple);
}

/*
 *	encode a constant floating point value
 */
TUPLE	*constant_float( char *text, int length)
{
	TUPLE	*tuple;
	float	f = 0;
	sscanf( text, "%f", &f);
	tuple = new_tuple( CONST, (unsigned char)f, 0, 0, 0, 0);
	ansi_c_audit_float( tuple->value, f, text, length);
	return( tuple);
}

/*
 *	encode a string literal
 */
TUPLE	*string_literal( char *text, int length)
{
	TUPLE	*tuple;
/*
 *	do not need the beginning " and also remove the ending " but add one for nul (-2 + 1 = -1)
 */
	text[ length - 1] = 0;
	tuple = new_tuple( STRING_LITERAL, 0, 0, 0, &text[1], length - 1);
	return( tuple);
}

/*
 *	encode an identifier
 */
TUPLE	*identifier( char *text, int length)
{
	TUPLE	*tuple;
/*
 *	but add one for nul
 */
	tuple = new_tuple( IDENTIFIER, 0, 0, 0, text, length + 1);
	return( tuple);
}

/*******************************************************************************
 *
 *	ansi_c audit routines
 *	the only types supported by the ansi_c compiler:
 *
 *	Input type1, type2:		Return type:
 *	VOID, NUL			VOID
 *	CHAR, NUL			CHAR
 *	INT, NUL			INT
 *
 ******************************************************************************/
/*
 *	audit the type specifiers
 *	only support VOID, CHAR or INT - everything to one byte CHAR
		$$.token = audit_declaration_specifiers( $1.token, 0);
		$$.token = audit_declaration_specifiers( $1.token, $2.token);
 */
int	audit_declaration_specifiers( int type1, int type2)
{
	switch( type1)
	{
	case VOID:
		if( ! type2)
			return VOID;
		break;
	case CHAR:
		if( ! type2)
			return CHAR;
		break;
	case INT:
		if( ! type2)
			return INT;
		break;
	}
	fprintf( stderr, "Error: unsupported data type: %s %s\n", tokens[ type1], tokens[ type2]);
	data.errors++;
	return VOID;
}

/*******************************************************************************
 *
 *	typedef table routines
 *
 ******************************************************************************/
/*
 *	print the typedef table
 */
void	print_typedef_table( void)
{
	if( data.typedef_table)
	{
		printf( "typedef table:\n");
		print_tuple_list( data.typedef_table);
	}
}

/*
 *	find the identifier in the typedef table linked list 
 */
TUPLE	*find_typedef( char *text, int length)
{
	TUPLE	*lists;
/*
 *	search the tuple list using the buffer length and string compare to match
 */
	for( lists = data.typedef_table; lists; lists = lists->next)
	{
		if( lists->length == length && ! strcmp( lists->buffer, text))
			return( lists);
	}
	return( lists);
}

/*
 *	type an identifier in typedef table
		$$.token = typedef_declaration( $1.token, $2.tuple);
 */
TUPLE	*typedef_declaration( int type_specifier, TUPLE *identifier)
{
	TUPLE	*tuple;
	int	type;
/*
 *	first search for same identifier string
 */
	tuple = find_typedef( identifier->buffer, identifier->length);
	if( tuple)
	{
		fprintf( stderr, "Error: typedef already defined for: %s\n", tuple->buffer);
		data.errors++;
		return( identifier);
	}
	switch( type_specifier)
	{
	default:
		fprintf( stderr, "Warning typedef not supported for: %s\n", tuple->buffer);
		data.warnings++;
		return( identifier);
	}
/*
 *	attach it to the head of the typedef table list (LIFO)
 */
	identifier->next = data.typedef_table;
	data.typedef_table = identifier;
/*
 *	return 
 */
	return( (TUPLE*)0);
}
