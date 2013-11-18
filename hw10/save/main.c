/*******************************************************************************
*
* FILE:		main.c (calc.c)
*
* DESC:		EECS 337 Assignment 6
*      		main program for the Calculator page 295-296
*
* AUTHOR:	caseid
*
* DATE:		October 8, 2013
*
* EDIT HISTORY:	
*
*		Updated for EECS 337 Assignment 7 October 15, 2013 
*		Updated for EECS 337 Assignment 8 October 29, 2013 
*		Updated for EECS 337 Assignment 9 November 5, 2013 
*		Updated for EECS 337 Assignment 10 November 12, 2013 
*
*******************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>
#include	<string.h>
#include	"yystype.h"

/*
 *	define the global data structure for the compiler
 */
DATA	data;

/*
 *	initialize all global variables
 */
int	main_init( void)
{
	memset((void*)&data, 0, sizeof( DATA));
	return 0;
}

/*
 *	clean-up and exit the software
 */
int	main_exit( void)
{
/*
 *	print the symbol table 
 */
#ifdef	YYDEBUG
	if( IS_FLAGS_SYMBOL( data.flags))
	{
		print_symbol_table();
	}
#endif
/*
 *	check for a memory leak
 */
	if( data.memory)
		fprintf( stderr, "Error: memory leak: %d\n", data.memory);
/*
 *	check for compiler errors 
 */
	if( data.errors)
		fprintf( stderr, "Errors: %d\n", data.errors);
	return data.errors;
}

/*
 *	process command line flags
 */
void	main_process_flags( char *command)
{
	int	status;

	switch( *command)
	{
	case '-':
		if( !strncmp( command, "-debug", strlen( command)))
		{
			CLR_FLAGS_DEBUG( data.flags);
			return;
		}
		else if( !strncmp( command, "-symbol", strlen( command)))
		{
			CLR_FLAGS_SYMBOL( data.flags);
			return;
		}
		else if( !strncmp( command, "-yydebug", strlen( command)))
		{
			yydebug = 0;
			return;
		}
		break;
	case '+':
		if( !strncmp( command, "+debug", strlen( command)))
		{
			SET_FLAGS_DEBUG( data.flags);
			return;
		}
		else if( !strncmp( command, "+symbol", strlen( command)))
		{
			SET_FLAGS_SYMBOL( data.flags);
			return;
		}
		else if( !strncmp( command, "+yydebug", strlen( command)))
		{
			yydebug = 1;
			return;
		}
		break;
	default:
/*
 *	everything else is treated as an input file name
 */
		if( yyin = fopen( command, "r"))
		{
/*
 *	ok, set the parse flag
 */
			SET_FLAGS_PARSE( data.flags);
/*
 *	call the compiler and check the status
 */
			if( status = yyparse())
			{
				fprintf( stderr, "Error: yyparse %d\n", status);
			}
/*
 *	close file 
 */
			fclose( yyin);
			return;
		}
	}
	fprintf( stdout, "Usage: calc [[+|-]debug] [[+|-]symbol] [[+|-]yydebug] [filename] [...]\n");
	exit( -1);
}

/*******************************************************************************
 *	main program
 ******************************************************************************/
int	main( int argc, char *argv[])
{
	int	status;
	time_t	t;
/*
 *	print start of test time
 */
	time( &t);
	fprintf( stdout, "for caseid start time: %s", ctime( &t));
/*
 *	initialize or constructor, init
 */
	if(( status = main_init()))
	{
		fprintf( stderr, "Error: init: %d\n", status);
		return status;
	}
/*
 *	check command line
 */
	while( --argc)
	       main_process_flags( *++argv);
/*
 *	check if no files parsed
 */
	if( ! IS_FLAGS_PARSE( data.flags))
	{
/*
 *	so use stdin and call the parser
 */
		printf( "Enter calculator expression and $ to exit\n");
		if(( status = yyparse()))
		{
			fprintf( stderr, "Error: yyparse %d\n", status);
		}
	}
/*
 *	deinitialize or destructor, exit
 */
	if(( status = main_exit()))
	{
		fprintf( stderr, "Error: exit: %d\n", status);
	}
	return status;
}

