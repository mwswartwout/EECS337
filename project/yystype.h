/*******************************************************************************
*
* FILE:		yystype.h for ansi_c compiler
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
#ifndef	YYSTYPE_H
#define	YYSTYPE_H	1

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>
/*
 *	define pic instructions
 */
#define I_LABEL		0
#define I_MOV		1
#define I_ADD		2
#define I_AND		3
#define I_IOR		4
#define I_SUB		5
#define I_XOR		6
#define I_COMF		7
#define I_DECF		8
#define I_DECFSZ	9
#define I_INCF		10
#define I_INCFSZ	11
#define I_RLF		12
#define I_RRF		13
#define	I_SWAPF		14
#define I_BCF		15
#define I_BSF		16
#define I_BTFSC		17
#define I_BTFSS		18
#define I_CALL		19
#define I_GOTO		20
#define I_TRIS		21
#define I_CLR		22
#define I_RETLW		23
#define I_CLRWDT	24
#define I_NOP		25
#define I_OPTION	26
#define I_RETFIE	27
#define I_RETURN	28
#define I_SLEEP		29
#define I_ARSF		30
#define I_LSLF		31
#define	I_LSRF		32
#define I_BR		33
#define I_RESET		34
/*
 *	define a tuple structure
 *	supports: CONSTANT STRING_LITERAL IDENTIFIER types
 */
#define	TUPLE	struct tuple
TUPLE
{
	TUPLE		*next;
	int		token;
	unsigned char	value;
#define FSR0	0x81
#define FSR1	0x82	
	int		address;
#define	MASK_VALUE	0x0001
#define	MASK_ADDRESS	0x0002
#define	MASK_LABEL	0x0004
#define	MASK_W_REG	0x0008
#define	MASK_F_REG	0x0010
#define	MASK_INSTR	0x0020
#define	MASK_PRINT	0x0040
#define MASK_WFC	0x0080
#define MASK_WFB	0x0100
#define MASK_FSR	0x0200
#define MASK_LP		0x0400
#define MASK_LB		0x0800
#define MASK_IW		0x1000
#define MASK_WI		0x2000
#define MASK_INDIRECT 0x4000
#define PREINCREMENT 0b00
#define PREDECREMENT 0b01
#define POSTINCREMENT 0b10
#define POSTDECREMENT 0b11
	int		mask;
	char		*buffer;
	int		length;
	int		level;
};

/*
 *	define yystype
 */
#define YYSTYPE struct yyansi_c
YYSTYPE
{
	int	token;
	TUPLE	*tuple;
};

/*
 *	define a global data structure
 */
#define	DATA	struct data
DATA
{
	int	column;
	int	flags;
#define	FLAGS_ECHO	0x0001
#define	FLAGS_DEBUG	0x0002
#define	FLAGS_PARSE	0x0004
#define	FLAGS_SYMBOL	0x0008
#define	FLAGS_ADDRESS	0x0010
#define	FLAGS_MAIN	0x0020
#define	IS_FLAGS_ECHO(a)	(a & FLAGS_ECHO)	
#define	SET_FLAGS_ECHO(a)	(a |= FLAGS_ECHO)
#define	CLR_FLAGS_ECHO(a)	(a &= ~FLAGS_ECHO)
#define	IS_FLAGS_DEBUG(a)	(a & FLAGS_DEBUG)	
#define	SET_FLAGS_DEBUG(a)	(a |= FLAGS_DEBUG)
#define	CLR_FLAGS_DEBUG(a)	(a &= ~FLAGS_DEBUG)
#define	IS_FLAGS_PARSE(a)	(a & FLAGS_PARSE)	
#define	SET_FLAGS_PARSE(a)	(a |= FLAGS_PARSE)
#define	CLR_FLAGS_PARSE(a)	(a &= ~FLAGS_PARSE)
#define	IS_FLAGS_SYMBOL(a)	(a & FLAGS_SYMBOL)	
#define	SET_FLAGS_SYMBOL(a)	(a |= FLAGS_SYMBOL)
#define	CLR_FLAGS_SYMBOL(a)	(a &= ~FLAGS_SYMBOL)
#define	IS_FLAGS_ADDRESS(a)	(a & FLAGS_ADDRESS)	
#define	SET_FLAGS_ADDRESS(a)	(a |= FLAGS_ADDRESS)
#define	CLR_FLAGS_ADDRESS(a)	(a &= ~FLAGS_ADDRESS)
#define	IS_FLAGS_MAIN(a)	(a & FLAGS_MAIN)	
#define	SET_FLAGS_MAIN(a)	(a |= FLAGS_MAIN)
#define	CLR_FLAGS_MAIN(a)	(a &= ~FLAGS_MAIN)
	int	errors;
	int	warnings;
	int	memory;
	TUPLE	*typedef_table;
	int	level;
	TUPLE	*symbol_table;
	TUPLE	*symbol_table_free;
#define	TOP_MEMORY	0x0020
#define	BOTTOM_MEMORY	0x007f
	int	address;
	int	line;
	char	string[ 128];
	int	label;
};


/*
 *	define for yyparser debugging
 */
#define	YYDEBUG	1
/*
 *	externs for lex functions from audit.c file
 */
extern	int	ansi_c_audit_conversion( unsigned char byte, unsigned int integer, char *text, int length);
extern	int	ansi_c_audit_overflow( unsigned char byte, unsigned int integer, char *text, int length);
extern	int	ansi_c_audit_multi_character( unsigned char byte, unsigned int integer, char *text, int length);
extern	int	ansi_c_audit_octal_digits( unsigned char byte, unsigned int integer, char *text, int length);
extern	void	ansi_c_audit_char( unsigned char byte, unsigned int integer, char *text, int length);
extern	void	ansi_c_audit_decimal( unsigned char byte, unsigned int integer, char *text, int length);
extern	void	ansi_c_audit_hex( unsigned char byte, unsigned int integer, char *text, int length);
extern	void	ansi_c_audit_float( unsigned char byte, float f, char *text, int length);
extern	TUPLE	*constant_hex( char *yytext, int yyleng); 
extern	TUPLE	*constant_octal( char *yytext, int yyleng); 
extern	TUPLE	*constant_decimal( char *yytext, int yyleng); 
extern	TUPLE	*constant_char( char *yytext, int yyleng); 
extern	TUPLE	*constant_float( char *yytext, int yyleng); 
extern	TUPLE	*string_literal( char *yytext, int yyleng); 
extern	TUPLE	*identifier( char *yytext, int yyleng); 
extern	int	audit_declaration_specifiers( int type1, int type2);
extern	void	print_typedef_table( void);
extern	TUPLE	*find_typedef( char *text, int length);
extern	TUPLE	*typedef_declaration( int type_specifier, TUPLE *identifier);
/*
 *	externs for parser functions from tuple.c file
 */
extern	void	new_error( int length);
extern	char	*new_buffer( char *text, int length);
extern	void	free_buffer( char *buffer, int length);
extern	TUPLE	*new_tuple( int token, unsigned char value, int address, int mask, char *buffer, int length);
extern	void	free_tuple( TUPLE *tuple);
extern	void	free_tuple_list( TUPLE *tuple);
extern	TUPLE	*end_tuple_list( TUPLE *tuple);
extern	void	print_tuple( TUPLE *tuple);
extern	void	print_tuple_list( TUPLE *tuple);
extern	TUPLE	*tuple_tail_to_head( TUPLE *head1, TUPLE *head2);
extern	int	next_label( char *buffer);
extern	TUPLE	*tuple_and_expr( int instr, TUPLE *and_expr, TUPLE *equality_expr);
extern	TUPLE	*tuple_selection_statement( TUPLE *expr, TUPLE *statement1, TUPLE *statement2);
extern	TUPLE	*tuple_iteration_statement( TUPLE *expr, TUPLE *statement);
extern	TUPLE	*tuple_iteration_statement( TUPLE *expr, TUPLE *statement);
extern	TUPLE	*tuple_jump_statement_goto( TUPLE *identifier);
extern	TUPLE	*tuple_labeled_statement( TUPLE *identifier, TUPLE *statement);
extern	TUPLE	*tuple_postfix_expr( TUPLE *postfix_expr, TUPLE *argument_expr_list);
extern	TUPLE	*tuple_additive_expr( int instr, TUPLE *additive_expr, TUPLE *multiplicative_expr);
extern	TUPLE	*tuple_assignment_expr( TUPLE *unary_expr, int assignment_operator, TUPLE *assignment_expr);
extern	TUPLE	*tuple_parameter_list( TUPLE *parameter_list, TUPLE *parameter_declaration);
extern	TUPLE	*tuple_jump_statement( TUPLE *return_expr);
extern	TUPLE	*tuple_function_definition( int declaration_specifiers, TUPLE *declarator, TUPLE *function_body);
extern	TUPLE	*tuple_primary_expr_identifier( TUPLE *tuple);
extern	TUPLE	*tuple_primary_expr_constant( TUPLE *tuple);
extern	TUPLE	*tuple_primary_expr_string_literal( TUPLE *tuple);
extern	TUPLE	*tuple_declaration( int declaration_specifiers, TUPLE *init_declarator_list);
/*
 *	externs for symbol table functions from symbol.c file
 */
extern	void	print_symbol_table( void);
extern	TUPLE	*find_symbol( int level, char *text, int length);
extern	void	create_symbol( int type, unsigned char value, char *buffer, int length);
extern	TUPLE	*symbol_declaration( int type_specifier, TUPLE *init_declarator_list);
extern	TUPLE	*symbol_init_declarator_list( TUPLE *init_declarator_list, TUPLE *init_declarator);
extern	TUPLE	*symbol_init_declarator( TUPLE *declarator, TUPLE *initializer);
extern	void	symbol_left_bracket( void);
extern	void	symbol_right_bracket( void);
/*
 *	externs for code generator functions from codegen.c file
 */
extern	int	get_address( int size);
extern	int	put_address( int address, int size);
extern	void	code_generator_pic_address( void);
extern	void	code_generator_operand_postfix( TUPLE *tuple);
extern	void	code_generator_pic_prefix( void);
extern	void	code_generator_pic_postfix( void);
extern	void	code_generator_operand_postfix( TUPLE *tuple);
extern	void	code_generator_operand( TUPLE *tuple);
extern	void	code_generator_instr_postfix( TUPLE *tuple);
extern	void	code_generator_instr( TUPLE *tuple);
extern	TUPLE	*code_post_process_initialize( TUPLE *tuple_list);
extern	TUPLE	*code_post_process_return( TUPLE *tuple_list);
extern	TUPLE	*code_post_process( TUPLE *tuple_list);
extern	void	code_generator_pic16f84( TUPLE *tuple_list);
extern	void	code_generator_instr_test( void);
/*
 *	external variables and functions from scan.l
 */
extern	char	*tokens[];
extern	char	*instr_table[];
extern FILE	*yyin;
extern FILE	*yyout;
extern	char	*yytext;
//extern	int	yyleng;
extern	int	yynerrs;
extern	YYSTYPE	yylval;		/* YYSTYPE */
extern	int	yywrap( void);
extern	void	pdefine( void);
extern	void	comment( void);
extern	void	comment2( void);
extern	void	count( void);
extern	int	check_type( void);
/*
 *	external variables and functions from gram.y
 */
extern	int	yydebug;
extern	int	yyparse( void);
extern	void	yyerror( char *s);
/*
 *	external variables and functions from main.c
 */
extern	DATA	data;
extern	int	main_init( void);
extern	int	main_exit( void);
extern	void	main_process_flags( char *command);
extern	int	main( int argc, char *argv[]);

#endif
