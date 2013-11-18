/*
 *	FILE: test1relational.c
 */

#include <stdio.h>

int main(int argc, char *argv[])
{
	int	a = 1;
	int	b = 1;
	printf( "a: %d\n", a);
	printf( "b: %d\n", b);
	printf( "(a == 1): %d\n", (a == 1));
	printf( "(b != 0): %d\n", (b != 0));
	printf( "(a == 1) ^ (b != 0): %d\n", (a == 1) ^ (b != 0));
	return 0;
}
