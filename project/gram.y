/*******************************************************************************
*
* FILE:		gram.y
*
* DESC:		EECS 337 Project
*
* AUTHOR:	mws85
*
* DATE:		December 5, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
%{
#include "yystype.h"
#include "y.tab.h"

%}
%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELIPSIS RANGE

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%start code

%%
code
	: file
	{
	#ifdef	YYDEBUG
		if( IS_FLAGS_DEBUG( data.flags))
		{
			printf( "Debug: yacc tuples\n");
			print_tuple_list( $1.tuple);
		}
	#endif
		
	code_generator_pic16f1827( $1.tuple);
	}
	;

primary_expr
	: identifier
	{
		$$.tuple = tuple_primary_expr_identifier( $1.tuple);
	}
	| CONSTANT
	{
		$$.tuple = tuple_primary_expr_constant( $1.tuple);
	}
	| STRING_LITERAL
	{
		$$.tuple = tuple_primary_expr_string_literal( $1.tuple);
	}
	| '(' expr ')'
	{
		$$ = $2;
	}
	;

postfix_expr
	: primary_expr
	| postfix_expr '[' expr ']'
	| postfix_expr '(' ')'
	{
		$$.tuple = tuple_postfix_expr( $1.tuple ,0);
	}
	| postfix_expr '(' argument_expr_list ')'
	{
		$$.tuple = tuple_postfix_expr( $1.tuple, $3.tuple);
	}
	| postfix_expr '.' identifier
	| postfix_expr PTR_OP identifier
	| postfix_expr INC_OP
	| postfix_expr DEC_OP
	;

argument_expr_list
	: assignment_expr
	| argument_expr_list ',' assignment_expr
	{
		$$.tuple = tuple_tail_to_head($1.tuple, $3.tuple);
	}
	;

unary_expr
	: postfix_expr
	| INC_OP unary_expr
	| DEC_OP unary_expr
	| unary_operator cast_expr
	| SIZEOF unary_expr
	| SIZEOF '(' type_name ')'
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expr
	: unary_expr
	| '(' type_name ')' cast_expr
	;

multiplicative_expr
	: cast_expr
	| multiplicative_expr '*' cast_expr
	| multiplicative_expr '/' cast_expr
	| multiplicative_expr '%' cast_expr
	;

additive_expr
	: multiplicative_expr
	| additive_expr '+' multiplicative_expr
	{
		$$.tuple = tuple_additive_expr( I_ADD, $1.tuple, $3.tuple);
	}
	| additive_expr '-' multiplicative_expr
	;

shift_expr
	: additive_expr
	| shift_expr LEFT_OP additive_expr
	| shift_expr RIGHT_OP additive_expr
	;

relational_expr
	: shift_expr
	| relational_expr '<' shift_expr
	| relational_expr '>' shift_expr
	| relational_expr LE_OP shift_expr
	| relational_expr GE_OP shift_expr
	;

equality_expr
	: relational_expr
	| equality_expr EQ_OP relational_expr
	| equality_expr NE_OP relational_expr
	;

and_expr
	: equality_expr
	| and_expr '&' equality_expr
	;

exclusive_or_expr
	: and_expr
	| exclusive_or_expr '^' and_expr
	;

inclusive_or_expr
	: exclusive_or_expr
	| inclusive_or_expr '|' exclusive_or_expr
	;

logical_and_expr
	: inclusive_or_expr
	| logical_and_expr AND_OP inclusive_or_expr
	;

logical_or_expr
	: logical_and_expr
	| logical_or_expr OR_OP logical_and_expr
	;

conditional_expr
	: logical_or_expr
	| logical_or_expr '?' logical_or_expr ':' conditional_expr
	;

assignment_expr
	: conditional_expr
	| unary_expr assignment_operator assignment_expr
	{
		$$.tuple = tuple_assignment_expr( $1.tuple, $2.token, $3.tuple);
	}
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expr
	: assignment_expr
	| expr ',' assignment_expr
	;

constant_expr
	: conditional_expr
	;

declaration
	: declaration_specifiers ';'
	{
		$$.tuple = 0;
	}
	| declaration_specifiers init_declarator_list ';'
	{
		$$.tuple = symbol_declaration( $1.token, $2.tuple);
		$$.tuple = tuple_declaration( $1.token, $$.tuple);
	}
	;

declaration_specifiers
	: storage_class_specifier
	{
		$$.token = audit_declaration_specifiers( $1.token, 0);
	}
	| storage_class_specifier declaration_specifiers
	{
		$$.token = audit_declaration_specifiers( $1.token, $2.token);
	}
	| type_specifier
	{
		$$.token = audit_declaration_specifiers( $1.token, 0);
	}	
	| type_specifier declaration_specifiers
	{
		$$.token = audit_declaration_specifiers( $1.token, $2.token);
	}
	;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	{
		$$.tuple = symbol_init_declarator( $1.tuple, $3.tuple);
	}
	;

init_declarator
	: declarator
	| declarator '=' initializer
	{
		$$.tuple = symbol_init_declarator( $1.tuple, $3.tuple);
	}
	;

storage_class_specifier
	: TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

type_specifier
	: CHAR
	| SHORT
	| INT
	| LONG
	| SIGNED
	| UNSIGNED
	| FLOAT
	| DOUBLE
	| CONST
	| VOLATILE
	| VOID
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME
	;

struct_or_union_specifier
	: struct_or_union identifier '{' struct_declaration_list '}'
	| struct_or_union '{' struct_declaration_list '}'
	| struct_or_union identifier
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: type_specifier_list struct_declarator_list ';'
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' constant_expr
	| declarator ':' constant_expr
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM identifier '{' enumerator_list '}'
	| ENUM identifier
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: identifier
	| identifier '=' constant_expr
	;

declarator
	: declarator2
	| pointer declarator2
	{
		$$ = $2;
	}
	;

declarator2
	: identifier
	| '(' declarator ')'
	{
		$$ = $2;
	}
	| declarator2 '[' ']'
	| declarator2 '[' constant_expr ']'
	| declarator2 '(' ')'
	| declarator2 '(' parameter_type_list ')'
	{
		$$.tuple = tuple_tail_to_head( $1.tuple, $3.tuple);
	}
	| declarator2 '(' parameter_identifier_list ')'
	{
		$$.tuple = tuple_tail_to_head( $1.tuple, $3.tuple);
	}
	;

pointer
	: '*'
	| '*' type_specifier_list
	| '*' pointer
	| '*' type_specifier_list pointer
	;

type_specifier_list
	: type_specifier
	| type_specifier_list type_specifier
	;

parameter_identifier_list
	: identifier_list
	| identifier_list ',' ELIPSIS
	;

identifier_list
	: identifier
	| identifier_list ',' identifier
	;

parameter_type_list
	: parameter_list
	| parameter_list ',' ELIPSIS
	;

parameter_list
	: parameter_declaration
	{
		$$.tuple = tuple_parameter_list( $1.tuple, 0);
	}
	| parameter_list ',' parameter_declaration
	{
		$$.tuple = tuple_parameter_list( $1.tuple, $3.tuple);
	}
	;

parameter_declaration
	: type_specifier_list declarator
	{
		$$ = $2;
	}
	| type_name
	;

type_name
	: type_specifier_list
	| type_specifier_list abstract_declarator
	;

abstract_declarator
	: pointer
	| abstract_declarator2
	| pointer abstract_declarator2
	;

abstract_declarator2
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expr ']'
	| abstract_declarator2 '[' ']'
	| abstract_declarator2 '[' constant_expr ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| abstract_declarator2 '(' ')'
	| abstract_declarator2 '(' parameter_type_list ')'
	;

initializer
	: assignment_expr
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement
	: identifier ':' statement
	| CASE constant_expr ':' statement
	| DEFAULT ':' statement
	;

left_bracket
	: '{'
	{
		symbol_left_bracket();
	}
	;

right_bracket
	: '}'
	{
		symbol_right_bracket();
	}
	;

compound_statement	
	: left_bracket right_bracket
	{
		$$.tuple = 0;
	}
	| left_bracket statement_list right_bracket
	{
		$$.tuple = $2.tuple;
	}
	| left_bracket declaration_list right_bracket
	{
		$$.tuple = $2.tuple;
	}
	| left_bracket declaration_list statement_list right_bracket ';'
	{
		$$.tuple = tuple_tail_to_head( $2.tuple, $3.tuple);
	}
	;

declaration_list
	: declaration
	| declaration_list declaration
	{
		$$.tuple = tuple_tail_to_head( $1.tuple, $2.tuple);
	}
	;

statement_list
	: statement
	| statement_list statement
	{
		$$.tuple = tuple_tail_to_head( $1.tuple, $2.tuple);
	}
	;

expression_statement
	: ';'
	| expr ';'
	;

selection_statement
	: IF '(' expr ')' statement
	| IF '(' expr ')' statement ELSE statement
	| SWITCH '(' expr ')' statement
	;

iteration_statement
	: WHILE '(' expr ')' statement
	| DO statement WHILE '(' expr ')' ';'
	| FOR '(' ';' ';' ')' statement
	| FOR '(' ';' ';' expr ')' statement
	| FOR '(' ';' expr ';' ')' statement
	| FOR '(' ';' expr ';' expr ')' statement
	| FOR '(' expr ';' ';' ')' statement
	| FOR '(' expr ';' ';' expr ')' statement
	| FOR '(' expr ';' expr ';' ')' statement
	| FOR '(' expr ';' expr ';' expr ')' statement
	;

jump_statement
	: GOTO identifier ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	{
		$$.tuple = new_tuple( I_RETURN, 0, 0, MASK_INSTR, 0, 0);
	}
	| RETURN expr ';'
	{
		$$.tuple = tuple_jump_statement( $2.tuple);
	}
	;

file
	: external_definition
	| file external_definition
	;

external_definition
	: function_definition
	| declaration
	;

function_definition
	: declarator function_body
	{
		$$.tuple = tuple_function_definition( 0, $1.tuple, $2.tuple);
	}
	| declaration_specifiers declarator function_body
	{
		$$.tuple = tuple_function_definition( $1.token, $2.tuple, $3.tuple);
	}
	;

function_body
	: compound_statement
	| declaration_list compound_statement
	{
		$$.tuple = tuple_tail_to_head( $1.tuple, $2.tuple);
	}
	;

identifier
	: IDENTIFIER
	;
%%

void	yyerror( char *s)
{
	fflush(stdout);
	printf("line: %d\n%s\n", data.line, data.string);
	printf("\n%*s\n%*s\n", data.column, "^", data.column, s);
}
