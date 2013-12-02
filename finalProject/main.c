/*******************************************************************************
*
* FILE:		main.c
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
	data.address = TOP_MEMORY;
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
 *	deallocate the symbol table list
 */
	free_tuple_list( data.symbol_table);
	free_tuple_list( data.symbol_table_free);
/*
 *	check if memory leak
 */
	if( data.memory)
		fprintf( stderr, "Error: memory deallocation error: %d\n", data.memory);
/*
 *	check if compiler warnings
 */
	if( data.warnings)
		fprintf( stderr, "Warning: compiler warnings: %d\n", data.warnings);
/*
 *	check if compiler errors 
 */
	if( data.errors)
		fprintf( stderr, "Error: compiler errors: %d\n", data.errors);
	return( data.errors);
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
		if( !strncmp( command, "-echo", strlen( command)))
		{
			CLR_FLAGS_ECHO( data.flags);
			return;
		}
		else if( !strncmp( command, "-debug", strlen( command)))
		{
			CLR_FLAGS_DEBUG( data.flags);
			return;
		}
		else if( !strncmp( command, "-yydebug", strlen( command)))
		{
			yydebug = 0;
			return;
		}
		else if( !strncmp( command, "-symbol", strlen( command)))
		{
			CLR_FLAGS_SYMBOL( data.flags);
			return;
		}
		else if( !strncmp( command, "-address", strlen( command)))
		{
			CLR_FLAGS_ADDRESS( data.flags);
			return;
		}
		break;
	case '+':
		if( !strncmp( command, "+echo", strlen( command)))
		{
			SET_FLAGS_ECHO( data.flags);
			return;
		}
		else if( !strncmp( command, "+debug", strlen( command)))
		{
			SET_FLAGS_DEBUG( data.flags);
			return;
		}
		else if( !strncmp( command, "+yydebug", strlen( command)))
		{
			yydebug = 1;
			return;
		}
		else if( !strncmp( command, "+symbol", strlen( command)))
		{
			SET_FLAGS_SYMBOL( data.flags);
			return;
		}
		else if( !strncmp( command, "+address", strlen( command)))
		{
			SET_FLAGS_ADDRESS( data.flags);
			return;
		}
		else if( !strncmp( command, "+test", strlen( command)))
		{
			code_generator_instr_test();
			exit( 0);
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
				data.errors++;
			}
/*
 *	close file 
 */
			fclose( yyin);
			return;
		}
	}
	fprintf( stdout, "Usage: ansi_c [[+|-]echo] [[+|-]debug] [[+|-]yydebug] [[+|-]symbol] [[+|-]address] [+test] [filename] [...]\n");
	exit( -1);
}

/*
 *	main program
 */
int	main( int argc, char *argv[])
{
	int	status;
	time_t	t;
/*
 *	print start of test time
 */
	time( &t);
	fprintf( stdout, "; for caseid start time: %s", ctime( &t));
/*
 *	initialize or constructor, init
 */
	if(( status = main_init()))
	{
		fprintf( stderr, "Error: init: %d\n", status);
		return status;
	}
/*
 *	process the command line args
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
		if(( status = yyparse()))
		{
			fprintf( stderr, "Error: yyparse %d\n", status);
				data.errors++;
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
