/*
char: 1
short: 2
int: 4
long: 8
long long: 8
signed: 4
unsigned: 4
float: 4
double: 8
long double: 16
const: 4
volatile: 4
void: 1
struct structS: 8
union unionS: 4
enum boolean: 4
uint: 4
extern1: 4
static1: 4
auto1: 4
register1: 4
hex 0xf: 4
hex 0xfl: 8
hex 0xfu: 4
hex 0xful: 8
octal 0123: 4
octal 0123l: 8
octal 0123u: 4
octal 0123ul: 8
decimal 123: 4
decimal 123l: 8
decimal 123u: 4
decimal 123ul: 8
char 'a': 4
char 'a': 4
char 'a': 4
char 'aa': 4
char 'aaa': 4
char 'aaaa': 4
float 12e3f: 4
float .123f: 4
float 123.f: 4
float 12e3: 8
float .123: 8
float 123.: 8
float 12e3l: 16
float .123l: 16
float 123.l: 16
string "hello": 6
 */

#include <stdio.h>

int main(int argc, char *argv[])
{
/*
 *	declare the types
 */
	char char1;
	short short1;
	int int1;
	long long1;
	long long long2;
	signed signed1;
	unsigned unsigned1;
	float float1;
	double double1;
	long double double2;
	const const1;
	volatile volatile1;
//	void void1;	error: variable or field ‘void1’ declared void
	struct structS
	{
		int int1;
		int int2;
	};
	struct structS structS1;
	union unionS
	{
		int int1;
		int int2;
	};
	union unionS unionS1;
	enum boolean { NO, YES };
	enum boolean boolean1;
	typedef unsigned int uint;
	uint	uint1;
	extern extern1;
	static static1;
	auto   auto1;
	register register1;
/*
 *	check the sizeof variables
 */
	printf( "char: %d\n", (int)sizeof( char));
	printf( "short: %d\n", (int)sizeof( short));
	printf( "int: %d\n", (int)sizeof( int));
	printf( "long: %d\n", (int)sizeof( long));
	printf( "long long: %d\n", (int)sizeof( long long));
	printf( "signed: %d\n", (int)sizeof( signed));
	printf( "unsigned: %d\n", (int)sizeof( unsigned));
	printf( "float: %d\n", (int)sizeof( float));
	printf( "double: %d\n", (int)sizeof( double));
	printf( "long double: %d\n", (int)sizeof( long double));
	printf( "const: %d\n", (int)sizeof( const));
	printf( "volatile: %d\n", (int)sizeof( volatile));
	printf( "void: %d\n", (int)sizeof( void));
	printf( "struct structS: %d\n", (int)sizeof( struct structS));
	printf( "union unionS: %d\n", (int)sizeof( union unionS));
	printf( "enum boolean: %d\n", (int)sizeof( enum boolean));
	printf( "uint: %d\n", (int)sizeof( uint));
//	printf( "extern: %d\n", (int)sizeof( extern)); 
//	printf( "static: %d\n", (int)sizeof( static));
//	printf( "auto: %d\n", (int)sizeof( auto));
//	printf( "register: %d\n", (int)sizeof( register));

	printf( "char1: %d\n", (int)sizeof( char1));
	printf( "short1: %d\n", (int)sizeof( short1));
	printf( "int1: %d\n", (int)sizeof( int1));
	printf( "long1: %d\n", (int)sizeof( long1));
	printf( "long2: %d\n", (int)sizeof( long2));
	printf( "signed1: %d\n", (int)sizeof( signed1));
	printf( "unsigned1: %d\n", (int)sizeof( unsigned1));
	printf( "float1: %d\n", (int)sizeof( float1));
	printf( "double1: %d\n", (int)sizeof( double1));
	printf( "double2: %d\n", (int)sizeof( double2));
	printf( "const1: %d\n", (int)sizeof( const1));
	printf( "volatile1: %d\n", (int)sizeof( volatile1));
//	printf( "void1: %d\n", (int)sizeof( void1));
	printf( "structS1: %d\n", (int)sizeof( structS1));
	printf( "unionS1: %d\n", (int)sizeof( unionS1));
	printf( "boolean1: %d\n", (int)sizeof( boolean1));
	printf( "uint1: %d\n", (int)sizeof( uint1));
	printf( "extern1: %d\n", (int)sizeof( extern1)); 
	printf( "static1: %d\n", (int)sizeof( static1));
	printf( "auto1: %d\n", (int)sizeof( auto1));
	printf( "register1: %d\n", (int)sizeof( register1));
/*
 *	check the sizeof constants
 */
	printf( "hex 0xf: %d\n", (int)sizeof( 0xf));	// 0[xX]{H}+
	printf( "hex 0xfl: %d\n", (int)sizeof( 0xfl));	// 0[xX]{H}+{LS}
	printf( "hex 0xfu: %d\n", (int)sizeof( 0xfu));	// 0[xX]{H}+{US}
	printf( "hex 0xful: %d\n", (int)sizeof( 0xful));	// 0[xX]{H}+{ULS}

	printf( "octal 0123: %d\n", (int)sizeof( 0123));	// 0{D}+
	printf( "octal 0123l: %d\n", (int)sizeof( 0123l));	// 0{D}+{LS}
	printf( "octal 0123u: %d\n", (int)sizeof( 0123u));	// 0{D}+{US}
	printf( "octal 0123ul: %d\n", (int)sizeof( 0123ul));	// 0{D}+{ULS}

	printf( "decimal 123: %d\n", (int)sizeof( 123));	// {D}+
	printf( "decimal 123l: %d\n", (int)sizeof( 123l));	// {D}+{LS}
	printf( "decimal 123u: %d\n", (int)sizeof( 123u));	// {D}+{US}
	printf( "decimal 123ul: %d\n", (int)sizeof( 123ul));	// {D}+{ULS}

	printf( "char 'a': %d\n", (int)sizeof( 'a'));	// '(\\.|[^\\'])+'
	printf( "char '\x61': %d\n", (int)sizeof( '\x61'));	// '(\\.|[^\\'])+'
	printf( "char '\141': %d\n", (int)sizeof( '\141'));	// '(\\.|[^\\'])+'
	printf( "char 'aa': %d\n", (int)sizeof( 'aa'));	// '(\\.|[^\\'])+'
	printf( "char 'aaa': %d\n", (int)sizeof( 'aaa'));	// '(\\.|[^\\'])+'
	printf( "char 'aaaa': %d\n", (int)sizeof( 'aaaa'));	// '(\\.|[^\\'])+'

	printf( "float 12e3f: %d\n", (int)sizeof( 12e3f));	// {D}+{E}{FS}
	printf( "float .123f: %d\n", (int)sizeof( .123f));	// {D}*"."{D}+({E})?{FS}
	printf( "float 123.f: %d\n", (int)sizeof( 123.f));	// {D}+"."{D}*({E})?{FS}

	printf( "float 12e3: %d\n", (int)sizeof( 12e3));	// {D}+{E}
	printf( "float .123: %d\n", (int)sizeof( .123));	// {D}*"."{D}+({E})?
	printf( "float 123.: %d\n", (int)sizeof( 123.));	// {D}+"."{D}*({E})?

	printf( "float 12e3l: %d\n", (int)sizeof( 12e3l));	// {D}+{E}{LS}
	printf( "float .123l: %d\n", (int)sizeof( .123l));	// {D}*"."{D}+({E})?{LS}
	printf( "float 123.l: %d\n", (int)sizeof( 123.l));	// {D}+"."{D}*({E})?{LS}

	printf( "string \"hello\": %d\n", (int)sizeof( "hello"));	// \"(\\.|[^\\"])*\"
	return 0;
}
