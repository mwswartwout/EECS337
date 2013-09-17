/*******************************************************************************
*
* FILE:		Code_1_6_4.c
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
#define a (x+1)
int x = 2;
void b() { x = a; printf("%d\n", x); }
void c() { int x = 1; printf("%d\n", a); }
void main() { b(); c(); }
