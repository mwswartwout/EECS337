/*******************************************************************************
*
* FILE:		Code_1_6_1.c
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
/*
 *	main program
 */
int	main( int argc, char *argv[])
{
	int w, x, y, z;
	int i = 4; int j = 5;
	{	int j = 7;
		i = 6;
		w = i + j;
	}
	x = i + j;
	{
		int i = 8;
		y = i + j;
	}
	z = i + j;

/*
 *	print the results
 */
	printf( "w:%d,\tx:%d,\ty:%d,\tz:%d\n", w, x, y, z);
/*
 *	return success
 */
	return 0;
}
