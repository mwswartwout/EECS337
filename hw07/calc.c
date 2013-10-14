/*******************************************************************************
*
* FILE:		calc.c
*
* DESC:		EECS 337 Assignment 6
*      		main program for the Calculator page 295-296
*
* AUTHOR:	mws85
*
* DATE:		October 8, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>
#include	<string.h>
#include	"yystype.h"

/*******************************************************************************
 *	main program
 ******************************************************************************/
int	main( int argc, char *argv[])
{
	time_t	t;
/*
 *	print start of test time
 */
	time( &t);
	fprintf( stdout, "for mws85 start time: %s", ctime( &t));
/*
 *	check command line
 */
	if( 1 < argc)
	    if( argc == 2 && !strcmp( *++argv, "+yydebug"))
	    	yydebug = 1;
	    else
	    {
		fprintf( stdout, "Usage: calc [+yydebug]\n");
		exit( -1);
	    }
/*
 *	print the sizeof integers, longs, floats and doubles
 */
	printf( "SIZEOF int: %d long: %d long long: %d float: %d double: %d, long double: %d\n", 
		(int)sizeof( int), (int)sizeof( long), (int)sizeof( long long), (int)sizeof( float), (int)sizeof( double), (int)sizeof( long double));
/*
 *	call the parser
 */
	printf( "Enter calculator expression and $ to exit\n");
	yyparse();
/*
 *	return
 */
	return 0;
}

