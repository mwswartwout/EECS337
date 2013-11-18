/*******************************************************************************
*
* FILE:		symbol_table.c
*
* DESC:		EECS 337 Assignment 8
*
* AUTHOR:	caseid
*
* DATE:		October 29, 2013
*
*
* EDIT HISTORY:	
*		Updated for EECS 337 Assignment 10 November 12, 2013 
*
*******************************************************************************/
/*
 *	define the format strings
 */
char *formats[] =
{
	" ",
	"decimal",
	"hexadecimal",
	"octal",
	"float",
};

/*
 *	define the specifier strings
 */
char *specifiers[] =
{
	" ",
	"char",
	"short",
	" ",
	"int",
	" ",
	" ",
	" ",
	"long",
};

/*
 *	print the symbol table
 */
void	print_symbol_table( void)
{
	int	index;

	printf( "symbol table: \n");
	for( index = 1; index <= data.index; index++)
	{
	     switch( data.st[ index].type)
	     {
	     case TYPE_IDENTIFIER:
		     if( ! data.st[ index].sizeofarray)
	     	     	 printf( "index: %d identifier: %s length: %d specifier: %s\n", index, data.st[ index].buffer, data.st[ index].length, specifiers[ data.st[ index].specifier]);
		     else
	     	     	 printf( "index: %d identifier: %s[%s] length: %d specifier: %s\n", index, data.st[ index].buffer, data.st[ data.st[ index].sizeofarray].buffer, data.st[ index].length, specifiers[ data.st[ index].specifier]);
		     break;
	     case TYPE_CONSTANT:
	     	     printf( "index: %d constant: %s length: %d format: %s\n", index, data.st[ index].buffer, data.st[ index].length, formats[ data.st[ index].format]);
		     break;
	     }
	}
	return;
}

/*
 *	set the single scalar type specifier and array size for this identifier
 */
int	new_symbol( int specifier, int identifier, int constant)
{
/*
 *	set the single scalar type specifier for this identifier
 */
	switch( specifier)
	{
	case CHAR:
	     data.st[ identifier].specifier = SPECIFIER_CHAR;
//	     data.st[ identifier].sizeofspecifier = install( TYPE_CONSTANT, "1", 2, FORMAT_DECIMAL);
	     break;
	case SHORT:
	     data.st[ identifier].specifier = SPECIFIER_SHORT;
	     data.st[ identifier].sizeofspecifier = install( TYPE_CONSTANT, "2", 2, FORMAT_DECIMAL);
	     break;
	case INT:
	     data.st[ identifier].specifier = SPECIFIER_INT;
	     data.st[ identifier].sizeofspecifier = install( TYPE_CONSTANT, "4", 2, FORMAT_DECIMAL);
	     break;
	case LONG:
	     data.st[ identifier].specifier = SPECIFIER_LONG;
	     data.st[ identifier].sizeofspecifier = install( TYPE_CONSTANT, "8", 2, FORMAT_DECIMAL);
	     break;
	}
/*
 *	set the single scalar array size for this identifier
 */
	switch( data.st[ constant].format)
	{
	case FORMAT_NONE:
	case FORMAT_DECIMAL:
	case FORMAT_HEXADECIMAL:
	case FORMAT_OCTAL:
	     data.st[ identifier].sizeofarray = constant;
	     break;
	case FORMAT_FLOAT:
	     data.errors++;
	     fprintf( stderr, "error: size of array ‘%s’ has non-integer type specifier\n", data.st[ constant].buffer);
	     break;
	}
	return 0;
}

