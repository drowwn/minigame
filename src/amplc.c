/**
 * @file    amplc.c
 *
 * A recursive-descent compiler for the AMPL-2023 language.
 *
 * All scanning errors are handled in the scanner.  Parser errors MUST be
 * handled by the <code>abort_c</code> function.  System and environment errors
 * -- for example, running out of memory -- MUST be handled in the unit in which
 * they occur.  Transient errors -- for example, nonexistent files, MUST be
 * reported where they occur.  There are no warnings, which is to say, all
 * errors are fatal and MUST cause compilation to terminate with an abnormal
 * error code.
 *
 * @author  W.H.K. Bester (whkbester@cs.sun.ac.za) & J.E Oosthuizen (26507404@sun.ac.za)
 * @date    2023-07-04
 */

#include "symboltable.h"
#include "token.h"
#include "error.h"
#include "scanner.h"
#include "errmsg.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* --- type definitions ----------------------------------------------------- */
#if 1
typedef struct variable_s Variable;
struct variable_s
{
	char *id;		/**< variable identifier                                */
	ValType type;	/**< variable type                                      */
	SourcePos pos;	/**< position of the variable in the source             */
	Variable *next; /**< pointer to the next variable in the list           */
};
#endif
/* --- debugging ------------------------------------------------------------ */

/* TODO: Your Makefile has a variable called DFLAGS.  If it is set to contain
 * -DDEBUG_PARSER, it will cause the following prototypes to be included, and
 *  the functions to which they refer (given at the end of this file) will be
 *  compiled.  If, on the other hand, this flag is comment out, by setting
 *  DFLAGS to #-DDEBUG_PARSER, these functions will be excluded.  These
 *  definitions should be used at the start and end of every parse function.
 *  For an example, see the provided parse_program function.
 */

#ifdef DEBUG_PARSER
void debug_start(const char *fmt, ...);
void debug_end(const char *fmt, ...);
void debug_info(const char *fmt, ...);
#define DBG_start(...) debug_start(__VA_ARGS__)
#define DBG_end(...) debug_end(__VA_ARGS__)
#define DBG_info(...) debug_info(__VA_ARGS__)
#else
#define DBG_start(...)
#define DBG_end(...)
#define DBG_info(...)
#endif /* DEBUG_PARSER */

/* --- global variables ----------------------------------------------------- */

Token token;		 /**< the lookahead token type                           */
ValType return_type; /**< the return type of the current subroutine          */

/* TODO: Uncomment the previous definition for use during type checking. */

/* --- helper macros -------------------------------------------------------- */

#define STARTS_FACTOR(toktype)                                           \
	(toktype == TOK_ID || toktype == TOK_NUM || toktype == TOK_LPAREN || \
	 toktype == TOK_NOT || toktype == TOK_TRUE || toktype == TOK_FALSE)

#define STARTS_EXPR(toktype) \
	(STARTS_FACTOR(toktype) || toktype == TOK_MINUS)

#define IS_ADDOP(toktype) \
	(toktype >= TOK_MINUS && toktype <= TOK_PLUS)

#define IS_MULOP(toktype) \
	(toktype >= TOK_AND && toktype <= TOK_REM)

#define IS_ORDOP(toktype) \
	(toktype >= TOK_LT && toktype <= TOK_GT)

#define IS_RELOP(toktype) \
	(toktype >= TOK_EQ && toktype <= TOK_NE)

#define IS_TYPE(toktype) \
	(toktype == TOK_INT || toktype == TOK_BOOL)

/* --- function prototypes: parser routines --------------------------------- */

void parse_program(void);
void parse_subdef(void);
void parse_body(void);
void parse_type(ValType *type);
void parse_vardef(void);
void parse_statements(void);
void parse_statement(void);
void parse_assign(void);
void parse_call(void);
void parse_if(void);
void parse_input(void);
void parse_output(void);
void parse_return(void);
void parse_while(void);
void parse_arglist(void);
void parse_index(ValType *type);
void parse_expr(ValType *type);
void parse_simple(ValType *type);
void parse_term(ValType *type);
void parse_factor(ValType *type);

/* --- function prototypes: helper routines --------------------------------- */
#if 1
void chktypes(ValType found, ValType expected, SourcePos *pos, ...);
#endif
void expect(TokenType type);
void expect_id(char **id);

/* --- function prototypes: constructors ------------------------------------ */
#if 1
IDPropt *idpropt(ValType type, unsigned int offset, unsigned int nparams,
				 ValType *params);
Variable *variable(char *id, ValType type, SourcePos pos);
#endif
/* --- function prototypes: error reporting --------------------------------- */

void abort_c(Error err, ...);
void abort_cp(SourcePos *posp, Error err, ...);

/* --- main routine --------------------------------------------------------- */

int main(int argc, char *argv[])
{
#if 0
	char *jasmin_path;
#endif
	FILE *src_file;

	/* TODO: Uncomment the previous definition for code generation. */

	/* set up global variables */
	setprogname(argv[0]);

	/* check command-line arguments and environment */
	if (argc != 2)
	{
		eprintf("usage: %s <filename>", getprogname());
	}

	/* TODO: Uncomment the following code for code generation:
	if ((jasmin_path = getenv("JASMIN_JAR")) == NULL) {
		eprintf("JASMIN_JAR environment variable not set");
	}
	*/
	setsrcname(argv[1]);

	init_symbol_table();

	/* open the source file, and report an error if it cannot be opened */
	if ((src_file = fopen(argv[1], "r")) == NULL)
	{
		eprintf("file '%s' could not be opened:", argv[1]);
	}

	/* initialise all compiler units */
	init_scanner(src_file);

	/* compile */
	get_token(&token);
	parse_program();

	/* produce the object code, and assemble */
	/* TODO: Add calls for code generation. */

	/* release all allocated resources */
	/* TODO: Release the resources of the symbol table and code generation. */
	fclose(src_file);
	freeprogname();
	freesrcname();

#ifdef DEBUG_PARSER
	printf("Success!\n");
#endif

	return EXIT_SUCCESS;
}

/* --- parser routines ------------------------------------------------------ */

/*
 * program = "program" id ":" { subdef } "main" ":" body .
 */
void parse_program(void)
{
	char *class_name;

	DBG_start("<program>");

	/* TODO: For code generation, set the class name inside this function, and
	 * also handle initialising and closing the "main" function.  But from the
	 * perspective of simple parsing, this function is complete.
	 */

	expect(TOK_PROGRAM);
	expect_id(&class_name);
	expect(TOK_COLON);

	while (token.type == TOK_ID)
	{
		parse_subdef();
	}

	expect(TOK_MAIN);
	expect(TOK_COLON);

	parse_body();

	free(class_name);

	DBG_end("</program>");
}

/*
 *  subdef = ⟨id⟩ “(” ⟨type⟩ ⟨id⟩ {“,” ⟨type⟩ ⟨id⟩} “)” [“->” ⟨type⟩] “:” ⟨body⟩ .
 */
void parse_subdef(void)
{
	char *id;
	ValType type;
	Variable *list_params = NULL, *temp = NULL;
	int nparams = 0;
	SourcePos pos;

	DBG_start("<subdef>");

	expect_id(&id);
	expect(TOK_LPAREN);
	parse_type(&type);
	expect_id(&id);

	pos = position;

	list_params = temp;
	temp = variable(id, type, pos);

	while (token.type == TOK_COMMA)
	{
		expect(TOK_COMMA);
		parse_type(&type);
		pos = position;
		expect_id(&id);
		temp->next = variable(id, type, pos);
		temp = temp->next;
		nparams++;
	}

	/* Allocate memory for the params array to hold parameter types */
	ValType *params = emalloc(sizeof(ValType) * ++nparams);

	/* Populate the params array with the types of the parameters */
	temp = list_params;
	while (temp)
	{
		params[nparams] = temp->type;
		temp = temp->next;
		nparams++;
	}

	expect(TOK_RPAREN);

	IDPropt *idp = idpropt(TYPE_CALLABLE, get_variables_width(), nparams, params);
	open_subroutine(id, idp);

	if (token.type == TOK_ARROW)
	{
		expect(TOK_ARROW);
		parse_type(&type);
	}

	expect(TOK_COLON);

	/* Initialize a pointer to the head of the list */
	temp = list_params;

	// insert parameters and local variables into subroutine
	while (temp != NULL)
	{
		if (!insert_name(temp->id, idpropt(temp->type, get_variables_width(), 0, NULL)))
		{
			abort_cp(&pos, ERR_UNKNOWN_IDENTIFIER, temp->id);
		}
		temp = temp->next;
	}

	parse_body();

	/*free the linked list*/
	while (list_params)
	{
		temp = list_params;
		list_params = list_params->next;
		free(temp);
	}

	close_subroutine();
	DBG_end("</subdef>");
}

/*
 *  body = {⟨vardef⟩} ⟨statements⟩ .
 */
void parse_body(void)
{
	DBG_start("<body>");

	while (IS_TYPE(token.type))
	{
		parse_vardef();
	}

	parse_statements();

	DBG_end("</body>");
}

/*
 *  type = (“bool” ∣ “int”) [“array”] .
 */
void parse_type(ValType *type)
{
	DBG_start("<type>");
	if (token.type == TOK_INT || token.type == TOK_BOOL)
	{
		if (token.type == TOK_INT)
		{
			*type = TYPE_INTEGER;
		}
		else if (token.type == TOK_BOOL)
		{
			*type = TYPE_BOOLEAN;
		}
		get_token(&token);
		if (token.type == TOK_ARRAY)
		{
			get_token(&token);
			SET_AS_ARRAY(*type);
		}
	}
	else
	{
		abort_c(ERR_EXPECTED_TYPE_SPECIFIER, token.type);
	}

	DBG_end("</type>");
}

/*
 *  vardef = ⟨type⟩ ⟨id⟩ {“,” ⟨id⟩} “;” .
 */
void parse_vardef(void)
{
	ValType type;
	char *id;
	DBG_start("<vardef>");

	parse_type(&type);
	expect_id(&id);

	insert_name(id, idpropt(type, get_variables_width(), 0, NULL));

	while (token.type == TOK_COMMA)
	{
		expect(TOK_COMMA);
		expect_id(&id);
		insert_name(id, idpropt(type, get_variables_width(), 0, NULL));
	}

	expect(TOK_SEMICOLON);

	DBG_end("</vardef>");
}

/*
 *  statements = “chillax” ∣ ⟨statement⟩ {“;” ⟨statement⟩} .
 */
void parse_statements(void)
{
	DBG_start("<statements>");

	if (token.type == TOK_CHILLAX)
	{
		get_token(&token);
	}
	else
	{
		parse_statement();
		while (token.type == TOK_SEMICOLON)
		{
			get_token(&token);
			parse_statement();
		}
	}

	DBG_end("</statements>");
}

/*
 *  statement = ⟨assign⟩ ∣ ⟨call⟩ ∣ ⟨if⟩ ∣ ⟨input⟩ ∣ ⟨output⟩ ∣ ⟨return⟩ ∣ ⟨while⟩ .
 */
void parse_statement(void)
{
	DBG_start("<statement>");

	switch (token.type)
	{
	case TOK_LET:
		parse_assign();
		break;
	case TOK_ID:
		parse_call();
		break;
	case TOK_IF:
		parse_if();
		break;
	case TOK_INPUT:
		parse_input();
		break;
	case TOK_OUTPUT:
		parse_output();
		break;
	case TOK_RETURN:
		parse_return();
		break;
	case TOK_WHILE:
		parse_while();
		break;
	default:
		abort_c(ERR_EXPECTED_STATEMENT, token.type);
		break;
	}

	DBG_end("</statement>");
}

/*
 *  assign = “let” ⟨id⟩ [⟨index⟩] “=” (⟨expr⟩ ∣ “array” ⟨simple⟩) .
 */
void parse_assign(void)
{
	ValType temp;
	char *id;
	DBG_start("<assign>");
	expect(TOK_LET);
	expect_id(&id);

	if (token.type == TOK_LBRACK)
	{
		parse_index(&temp);
	}

	expect(TOK_EQ);

	if (STARTS_EXPR(token.type))
	{
		parse_expr(&temp);
	}
	else if (token.type == TOK_ARRAY)
	{
		expect(TOK_ARRAY);
		parse_simple(&temp);
	}
	else
	{
		abort_c(ERR_EXPECTED_EXPRESSION_OR_ARRAY_ALLOCATION, token.type);
	}
	free(id);
	DBG_end("</assign>");
}

/*
 *  call = ⟨id⟩ ⟨arglist⟩ .
 */
void parse_call(void)
{
	char *id;
	DBG_start("<call>");

	expect_id(&id);
	parse_arglist();

	free(id);

	DBG_end("</call>");
}

/*
 *  if = “if” ⟨expr⟩ “:” ⟨statements⟩ {“elif” ⟨expr⟩ “:” ⟨statements⟩} [“else” “:” ⟨statements⟩] “end” .
 */
void parse_if(void)
{
	ValType temp;
	DBG_start("<if>");

	expect(TOK_IF);
	parse_expr(&temp);
	expect(TOK_COLON);
	parse_statements();

	while (token.type == TOK_ELIF)
	{
		DBG_info("<elif>");
		expect(TOK_ELIF);
		parse_expr(&temp);
		expect(TOK_COLON);
		parse_statements();
		DBG_info("</elif>");
	}

	if (token.type == TOK_ELSE)
	{
		DBG_info("<else>");
		expect(TOK_ELSE);
		expect(TOK_COLON);
		parse_statements();
		DBG_info("</else>");
	}

	expect(TOK_END);

	DBG_end("</if>");
}

/*
 *  input = “input” “(” ⟨id⟩ [ ⟨index⟩ ] “)” .
 */
void parse_input(void)
{
	ValType temp;
	char *id;
	DBG_start("<input>");

	expect(TOK_INPUT);
	expect(TOK_LPAREN);
	expect_id(&id);

	if (token.type == TOK_LBRACK)
	{
		parse_index(&temp);
	}

	expect(TOK_RPAREN);
	free(id);
	DBG_end("</input>");
}

/*
 *  output = “output” “(” (⟨string⟩ ∣ ⟨expr⟩) {“..” (⟨string⟩ ∣ ⟨expr⟩)} “)” .
 */
void parse_output(void)
{
	ValType temp;
	DBG_start("<output>");

	expect(TOK_OUTPUT);
	expect(TOK_LPAREN);

	if (token.type == TOK_STR)
	{
		expect(TOK_STR);
	}
	else if (STARTS_EXPR(token.type))
	{
		parse_expr(&temp);
	}
	else
	{
		abort_c(ERR_EXPECTED_EXPRESSION_OR_STRING, token.type);
	}

	while (token.type == TOK_DOTDOT)
	{
		expect(TOK_DOTDOT);
		if (token.type == TOK_STR)
		{
			get_token(&token);
		}
		else if (STARTS_EXPR(token.type))
		{
			parse_expr(&temp);
		}
		else
		{
			abort_c(ERR_EXPECTED_EXPRESSION_OR_STRING, token.type);
		}
	}

	expect(TOK_RPAREN);

	DBG_end("</output>");
}

/*
 *  return = “return” [⟨expr⟩] .
 */
void parse_return(void)
{
	ValType temp;
	DBG_start("<return>");

	expect(TOK_RETURN);

	if (STARTS_EXPR(token.type))
	{
		parse_expr(&temp);
	}

	DBG_end("</return>");
}

/*
 *  while = “while” ⟨expr⟩ “:” ⟨statements⟩ “end” .
 */
void parse_while(void)
{
	ValType temp;
	DBG_start("<while>");

	expect(TOK_WHILE);
	parse_expr(&temp);
	expect(TOK_COLON);
	parse_statements();
	expect(TOK_END);

	DBG_end("</while>");
}

/*
 *  arglist = “(” ⟨expr⟩ {“,” ⟨expr⟩ }“)” .
 */
void parse_arglist(void)
{
	ValType temp;
	DBG_start("<arglist>");

	expect(TOK_LPAREN);
	parse_expr(&temp);

	while (token.type == TOK_COMMA)
	{
		expect(TOK_COMMA);
		parse_expr(&temp);
	}

	expect(TOK_RPAREN);

	DBG_end("</arglist>");
}

/*
 *  index = “[” ⟨simple⟩ “]” .
 */
void parse_index(ValType *type)
{
	DBG_start("<index>");

	expect(TOK_LBRACK);
	parse_simple(type);
	expect(TOK_RBRACK);

	DBG_end("</index>");
}

/*
 *  expr = ⟨simple⟩ [⟨relop⟩ ⟨simple⟩] .
 */
void parse_expr(ValType *type)
{
	DBG_start("<expr>");
	ValType temp;
	parse_simple(type);

	if (IS_RELOP(token.type))
	{
		*type = TYPE_BOOLEAN;

		switch (token.type)
		{
		case TOK_EQ:
			get_token(&token);
			parse_simple(&temp);
			break;
		case TOK_NE:
			get_token(&token);
			parse_simple(&temp);
			break;
		case TOK_LT:
			get_token(&token);
			parse_simple(&temp);
			break;
		case TOK_LE:
			get_token(&token);
			parse_simple(&temp);
			break;
		case TOK_GT:
			get_token(&token);
			parse_simple(&temp);
			break;
		case TOK_GE:
			get_token(&token);
			parse_simple(&temp);
			break;
		default:
			break;
		}
	}

	DBG_end("</expr>");
}

/*
 *  simple = [ “-” ] ⟨term⟩ {⟨addop⟩ ⟨term⟩} .
 */
void parse_simple(ValType *type)
{
	DBG_start("<simple>");

	if (token.type == TOK_MINUS)
	{
		get_token(&token);
	}

	parse_term(type);

	while (IS_ADDOP(token.type))
	{
		switch (token.type)
		{
		case TOK_MINUS:
			get_token(&token);
			parse_term(type);
			break;

		case TOK_PLUS:
			get_token(&token);
			parse_term(type);
			break;

		case TOK_OR:
			get_token(&token);
			parse_term(type);
			break;

		default:
			break;
		}
	}
	DBG_end("</simple>");
}

/*
 *  term = ⟨factor⟩ {⟨mulop⟩ ⟨factor⟩} .
 */
void parse_term(ValType *type)
{
	DBG_start("<term>");

	parse_factor(type);

	while (IS_MULOP(token.type))
	{
		switch (token.type)
		{
		case TOK_MUL:
			get_token(&token);
			parse_factor(type);
			break;
		case TOK_DIV:
			get_token(&token);
			parse_factor(type);
			break;
		case TOK_AND:
			get_token(&token);
			parse_factor(type);
			break;
		case TOK_REM:
			get_token(&token);
			parse_factor(type);
			break;

		default:
			break;
		}
	}

	DBG_end("</term>");
}

/*
 *  factor = ⟨id⟩ [ ⟨index⟩ ∣ ⟨arglist⟩ ] ∣ ⟨num⟩ ∣ “(” ⟨expr⟩ “)” ∣ “not” ⟨factor⟩ ∣ “true” ∣ “false” .
 */
void parse_factor(ValType *type)
{
	ValType *type1;
	SourcePos pos;
	DBG_start("<factor>");
	char *id;

	if (token.type == TOK_ID)
	{
		expect_id(&id);

		IDPropt *prop;
		find_name(id, &prop);

		if (token.type == TOK_LBRACK)
		{
			parse_index(type);
			*type = prop->type;
			SET_BASE_TYPE(*type);
		}
		else if (token.type == TOK_LPAREN)
		{
			parse_arglist();
		}
	}
	else if (token.type == TOK_NUM)
	{
		expect(TOK_NUM);
	}
	else if (token.type == TOK_LPAREN)
	{
		expect(TOK_LPAREN);
		parse_expr(type);
		expect(TOK_RPAREN);
	}
	else if (token.type == TOK_NOT)
	{
		expect(TOK_NOT);
		parse_factor(type);
	}
	else if (token.type == TOK_TRUE)
	{
		get_token(&token);
	}
	else if (token.type == TOK_FALSE)
	{
		get_token(&token);
	}
	else
	{
		abort_c(ERR_EXPECTED_FACTOR, token.type);
	}
	DBG_end("</factor>");
}

/* --- helper routines ------------------------------------------------------ */

#define MAX_MSG_LEN 256

/* TODO: Uncomment the following function for use during type checking. */

#if 1
void chktypes(ValType found, ValType expected, SourcePos *pos, ...)
{
	char buf[MAX_MSG_LEN], *s;
	va_list ap;

	if (found != expected)
	{
		buf[0] = '\0';
		va_start(ap, pos);
		s = va_arg(ap, char *);
		vsnprintf(buf, MAX_MSG_LEN, s, ap);
		va_end(ap);
		if (pos)
		{
			position = *pos;
		}
		leprintf("incompatible types (expected %s, found %s) %s",
				 get_valtype_string(expected), get_valtype_string(found), buf);
	}
}
#endif
void expect(TokenType type)
{
	if (token.type == type)
	{
		get_token(&token);
	}
	else
	{
		abort_c(ERR_EXPECT, type);
	}
}

void expect_id(char **id)
{
	if (token.type == TOK_ID)
	{
		*id = estrdup(token.lexeme);
		get_token(&token);
	}
	else
	{
		abort_c(ERR_EXPECT, TOK_ID);
	}
}

/* --- constructors --------------------------------------------------------- */
#if 1
IDPropt *idpropt(ValType type, unsigned int offset, unsigned int nparams,
				 ValType *params)
{
	IDPropt *p;

	p = emalloc(sizeof(IDPropt));
	p->type = type;
	p->offset = offset;
	p->nparams = nparams;
	p->params = params;

	return p;
}

Variable *variable(char *id, ValType type, SourcePos pos)
{
	Variable *v;

	v = emalloc(sizeof(Variable));
	v->id = id;
	v->type = type;
	v->pos = pos;
	v->next = NULL;

	return v;
}
#endif
/* --- error handling routines ---------------------------------------------- */

void _abort_cp(SourcePos *posp, Error err, va_list args)
{
	char expstr[MAX_MSG_LEN], *s;
	int t;

	if (posp)
	{
		position = *posp;
	}

	snprintf(expstr, MAX_MSG_LEN, "expected %%s, but found %s",
			 get_token_string(token.type));

	switch (err)
	{
	case ERR_EXPECTED_SCALAR:
	case ERR_ILLEGAL_ARRAY_OPERATION:
	case ERR_MULTIPLE_DEFINITION:
	case ERR_NOT_AN_ARRAY:
	case ERR_NOT_A_FUNCTION:
	case ERR_NOT_A_PROCEDURE:
	case ERR_NOT_A_VARIABLE:
	case ERR_TOO_FEW_ARGUMENTS:
	case ERR_TOO_MANY_ARGUMENTS:
	case ERR_UNKNOWN_IDENTIFIER:
	case ERR_UNREACHABLE:
		s = va_arg(args, char *);
		break;
	default:
		break;
	}

	switch (err)
	{
	case ERR_EXPECT:
		t = va_arg(args, int);
		leprintf(expstr, get_token_string(t));
		break;

	case ERR_EXPECTED_FACTOR:
		leprintf(expstr, "factor");
		break;

	case ERR_EXPECTED_EXPRESSION_OR_ARRAY_ALLOCATION:
		leprintf(expstr, "expression or array allocation");
		break;

	case ERR_EXPECTED_EXPRESSION_OR_STRING:
		leprintf(expstr, "expression or string");
		break;

	case ERR_EXPECTED_SCALAR:
		leprintf(expstr, "scalar");
		break;

	case ERR_EXPECTED_STATEMENT:
		leprintf(expstr, "statement");
		break;

	case ERR_EXPECTED_TYPE_SPECIFIER:
		leprintf(expstr, "type specifier");
		break;

	case ERR_ILLEGAL_ARRAY_OPERATION:
		leprintf("illegal array operation: %s", s);
		break;

	case ERR_MISSING_RETURN_EXPRESSION:
		leprintf("missing return expression");
		break;

	case ERR_MULTIPLE_DEFINITION:
		leprintf("multiple definition: %s", s);
		break;

	case ERR_NOT_AN_ARRAY:
		leprintf("not an array: %s", s);
		break;

	case ERR_NOT_A_FUNCTION:
		leprintf("not a function: %s", s);
		break;

	case ERR_NOT_A_PROCEDURE:
		leprintf("not a procedure: %s", s);
		break;

	case ERR_NOT_A_VARIABLE:
		leprintf("not a variable: %s", s);
		break;

	case ERR_RETURN_EXPRESSION_NOT_ALLOWED:
		leprintf("return expression not allowed");
		break;

	case ERR_TOO_FEW_ARGUMENTS:
		leprintf("too few arguments: %s", s);
		break;

	case ERR_TOO_MANY_ARGUMENTS:
		leprintf("too many arguments: %s", s);
		break;

	case ERR_UNKNOWN_IDENTIFIER:
		leprintf("unknown identifier: %s", s);
		break;

	case ERR_UNREACHABLE:
		leprintf("unreachable: %s", s);
		break;

	default:
		leprintf("unexpected error code: %d", err);
		break;
	}
}

void abort_c(Error err, ...)
{
	va_list args;

	va_start(args, err);
	_abort_cp(NULL, err, args);
	va_end(args);
}

void abort_cp(SourcePos *posp, Error err, ...)
{
	va_list args;

	va_start(args, err);
	_abort_cp(posp, err, args);
	va_end(args);
}

/* --- debugging output routines -------------------------------------------- */

#ifdef DEBUG_PARSER

static int indent = 0;

void _debug_info(const char *fmt, va_list args)
{
	int i;
	char buf[MAX_MSG_LEN], *buf_ptr;

	buf_ptr = buf;

	for (i = 0; i < indent; i++)
	{
		*buf_ptr++ = ' ';
	}

	vsprintf(buf_ptr, fmt, args);
	buf_ptr += strlen(buf_ptr);
	snprintf(buf_ptr, MAX_MSG_LEN, " at %d:%d.\n", position.line, position.col);
	fflush(stdout);
	fputs(buf, stdout);
	fflush(NULL);
}

void debug_start(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	_debug_info(fmt, args);
	va_end(args);
	indent += 2;
}

void debug_end(const char *fmt, ...)
{
	va_list args;

	indent -= 2;
	va_start(args, fmt);
	_debug_info(fmt, args);
	va_end(args);
}

void debug_info(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	_debug_info(fmt, args);
	va_end(args);
}

#endif /* DEBUG_PARSER */
