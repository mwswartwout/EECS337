/*******************************************************************************
*
* FILE:		Echo2.c
*
* DESC:		EECS 337 Homework Assignment 1
*
* AUTHOR:		mws85
*
* DATE:		August 27, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
#include	<stdio.h>
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
		     default:
			  putchar( c); // put the charater to the stdout
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
