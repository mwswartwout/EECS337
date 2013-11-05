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
	     	     printf( "index: %d identifier: %s length: %d\n", index, data.st[ index].buffer, data.st[ index].length);
		     break;
	     case TYPE_CONSTANT:
	     	     printf( "index: %d constant: %s length: %d format: %s\n", index, data.st[ index].buffer, data.st[ index].length, formats[ data.st[ index].format]);
		     break;
	     }
	}
	return;
}
