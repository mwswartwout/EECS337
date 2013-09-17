/*******************************************************************************
*
* FILE:		Echo.c
*
* DESC:		EECS 337 Homework Assignment 1
*
* AUTHOR:	caseid
*
* DATE:		August 27, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
#include	<stdio.h>
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
/*
 *	read from the command line and write out parameters
 */
	for( i = 0; i < argc; i++)
	     printf( "argv[ argc: %d]: %s\n", i, argv[ i]);
/*
 *	read from stdin and write to stdout
 */
	while(( c = getchar()) != EOF)	// while input character from stdin does not equal end of file (-1)
		putchar( c); // put the charater to the stdout
	return 0;
}

