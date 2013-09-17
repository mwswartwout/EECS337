/*******************************************************************************
*
* FILE:		Echo2.c
*
* DESC:		EECS 337 Homework Assignment 1
*
* AUTHOR:	mws85
*
* DATE:		August 27, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
#include	<stdio.h>
#include 	<stdlib.h>
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
#define HASH	5
#define QUOTE	19
#define IDENTIFY 20

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
	time_t t;
	char text[2];

	//print start of test time

	time( &t);
	fprintf( stdout, "for mws85 start time: %s", ctime( &t));
/*
 *	read from the command line and write out parameters
 */
	for(i = 0; i < argc; i++)
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
		switch( state)
		{
		case START:
		     switch( c)
		     {
		     case '/':
		     	  if( ! IS_FLAGS_ECHO( flags))
			  {
				state = SLASH1;
			  	break;
			  }
			case '#':
				if( ! IS_FLAGS_ECHO( flags))
				{
					state = HASH;
					break;
				}
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
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				text[ 0] = c;
				text[ 1] = 0;
				print_token( CONSTANT, text);
				break;
			case 'v':
				print_token( IDENTIFIER, "void");
				break;
			case 'i':
				print_token( INT, "int");
				break;
			case 'c':
				print_token( CHAR, "char");
				break;
			case 'r':
				print_token( RETURN, "return");
				break;
			case '"':
				print_token( STRING_LITERAL, "%d\n");
				break;
			case 'x':
				text[ 0] = c;
				text[ 1] = 0;
				print_token( IDENTIFIER, text);
				break;
			case 'm':
				print_token( IDENTIFIER, "main");
				break;
			case 'p':
				print_token( IDENTIFIER, "printf");
				break; 
			 default:
				break;
		     }

		
		case HASH:
			switch( c)
			{
				case '\n':
					state = START;
					putchar( c);
					break;
				default:
					break;
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
		}		
	}
	return 0;
}
