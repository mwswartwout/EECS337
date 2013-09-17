/*******************************************************************************
*
* FILE:		Echo2.c
*
* DESC:		EECS 337 Homework Assignment 1
*
* AUTHOR:	caseid
*
* DATE:		August 27, 2013
*
* EDIT HISTORY:	
*
*		September 3, 2013 update for Assignment 2
*
*******************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>
#include	"tokens.c"
/*
 *	define flags: [+/-]echo 
 */
#define	FLAGS_ECHO		0x00000001
#define	IS_FLAGS_ECHO(a)	(a & FLAGS_ECHO)	
#define	SET_FLAGS_ECHO(a)	(a |= FLAGS_ECHO)
#define	CLR_FLAGS_ECHO(a)	(a &= ~FLAGS_ECHO)

/*
 *	define comment states
 */
#define	START	0
#define	SLASH1	1
#define	SLASH2	2	// to EOL '\n'
#define	STAR1	3
#define	STAR2	4	// to */
#define	POUND	5	// to EOL '\n'
#define	IDENT	6	// keyword or identifier
#define	STRING	7	// "string literal"

/*
 *	main program
 */
int	main( int argc, char *argv[])
{
/*
 *	declare variables
 */
	int i;
	int c;
	int flags = 0;
	int state = START;
	time_t	t;
	char text[ 32];
	int index = 0;
/*
 *	print start of test time
 */
	time( &t);
	fprintf( stdout, "for caseid start time: %s", ctime( &t));
/*
 *	read from the command line and write out parameters
 */
	for( i = 0; i < argc; i++)
	{
	     if( !strcmp( argv[ i], "+echo"))
			SET_FLAGS_ECHO( flags);
	     else if( !strcmp( argv[ i], "-echo"))
			CLR_FLAGS_ECHO( flags);
	}
/*
 *	read from stdin and write to stdout
 */
	while(( c = getchar()) != EOF)	// while input character from stdin does not equal end of file (-1)
	{
		if( IS_FLAGS_ECHO( flags))
			  putchar( c); // put the character to the stdout
		else switch( state)
		{
		case IDENT:
		     if( isalpha( c))
		     {
		     	  text[ index++] = c;
		     	  break;
		     }
		     else
		     {
		     	  state = START;
		     	  text[ index] = 0;
	     		  if( !strcmp( text, "void"))
			      print_token( VOID, text);
	     		  else if( !strcmp( text, "char"))
			      print_token( CHAR, text);
	     		  else if( !strcmp( text, "int"))
			      print_token( INT, text);
	     		  else if( !strcmp( text, "return"))
			      print_token( RETURN, text);
			  else
			      print_token( IDENTIFIER, text);
		     }
		case START:
		     if( isdigit( c))
		     {
		     	  text[ 0] = c;
			  text[ 1] = 0;
		     	  print_token( CONSTANT, text);
		     }
		     else if( isalpha( c))
		     {
			  index = 0;
		     	  text[ index++] = c;
		     	  state = IDENT;
		     }
		     else switch( c)
		     {
		     case '"':
			  index = 0;
		     	  text[ index++] = c;
		     	  state = STRING;
			  break;
		     case '#':
		     	  state = POUND;
			  break;
		     case '/':
		     	  state = SLASH1;
			  break;
		     case '(':
		     case ')':
		     case '*':
		     case '+':
		     case ',':
		     case ';':
		     case '=':
		     case '[':
		     case ']':
		     case '{':
		     case '}':
		     	  text[ 0] = c;
			  text[ 1] = 0;
		     	  print_token( c, text);
			  break;
		     case ' ':
		     case '\t':
		     case '\v':
		     case '\f':
		     default:
			  break;
//			  putchar( c); // put the character to the stdout
		     }
		     break;
		case SLASH1:
		     switch( c)
		     {
		     case '/':
		     	  state = SLASH2;
			  break;
		     case '*':
		     	  state = STAR1;
			  break;
		     default:
		     	  state = START;
			  putchar( '/');
			  putchar( c);
		     }
		     break;
		case SLASH2:
		     switch( c)
		     {
		     case '\n':
		     	  state = START;
			  putchar( c);
		     default:
			  break;
		     }
		     break;
		case STAR1:
		     switch( c)
		     {
		     case '*':
		     	  state = STAR2;
		     default:
			  break;
		     }
		     break;
		case STAR2:
		     switch( c)
		     {
		     case '/':
		     	  state = START;
			  break;
		     case '*':
			  break;
		     default:
		     	  state = STAR1;
		     }
		     break;
		case POUND:
		     switch( c)
		     {
		     case '\n':
		     	  state = START;
			  putchar( c);
		     default:
			  break;
		     }
		     break;
		case STRING:
		     switch( c)
		     {
		     case '"':
		     	  state = START;
		     	  text[ index++] = c;
		     	  text[ index] = 0;
		     	  print_token( STRING_LITERAL, text);
		     default:
		     	  text[ index++] = c;
			  break;
		     }
		     break;
		}		
	}
	return 0;
}
