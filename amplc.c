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
 * @author  W.H.K. Bester (whkbester@cs.sun.ac.za)
 * @date    2023-07-04
 */

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "token.h"
#include "error.h"
#include "scanner.h"
#include "boolean.h"
#include "errmsg.h"
#include "valtypes.h"
#include "symboltable.h"
/* --- type definitions ----------------------------------------------------- */

/* TODO: Uncomment the following for use during type checking. */

typedef struct variable_s Variable;
struct variable_s {
	char      *id;    /**< variable identifier                                */
	ValType    type;  /**< variable type                                      */
	SourcePos  pos;   /**< position of the variable in the source             */
	Variable  *next;  /**< pointer to the next variable in the list           */
};

/* --- debugging ------------------------------------------------------------ */

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

Token token;          /**< the lookahead token type                           */
ValType return_type;  /**< the return type of the current subroutine          */

/* TODO: Uncomment the previous definition for use during type checking. */

/* --- helper macros -------------------------------------------------------- */

#define STARTS_FACTOR(toktype) \
	(toktype == TOK_ID || toktype == TOK_NUM || toktype == TOK_LPAREN || \
	 toktype == TOK_NOT || toktype == TOK_TRUE || toktype == TOK_FALSE)

#define STARTS_EXPR(toktype) \
	(toktype == TOK_MINUS || STARTS_FACTOR(toktype))

#define IS_ADDOP(toktype) \
	(toktype >= TOK_MINUS && toktype <= TOK_PLUS)

#define IS_MULOP(toktype) \
	(toktype == TOK_AND || toktype == TOK_DIV || toktype == TOK_MUL || \
	toktype == TOK_REM)

#define IS_ORDOP(toktype) \
	(toktype == TOK_GE || toktype == TOK_GT || toktype == TOK_LE \
	toktype == TOK_LT)

#define IS_RELOP(toktype) \
	(toktype == TOK_EQ || toktype == TOK_GE || toktype == TOK_GT || \
	toktype == TOK_LE || toktype == TOK_LT || toktype == TOK_NE)

#define IS_TYPE(toktype) \
	(toktype == TOK_BOOL || toktype == TOK_INT)

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
void parse_arglist(char *id, IDPropt *propt);
void parse_index(char *id);
void parse_expr(ValType *type);
void parse_simple(ValType *type);
void parse_term(ValType *type);
void parse_factor(ValType *type);

/* --- function prototypes: helper routines --------------------------------- */

/* TODO: Uncomment the following commented-out prototypes for use during type
 * checking.
 */

void chktypes(ValType found, ValType expected, SourcePos *pos, ...);
void expect(TokenType type);
void expect_id(char **id);

/* --- function prototypes: constructors ------------------------------------ */

IDPropt *idpropt(ValType type, unsigned int offset, unsigned int nparams,
		ValType *params);
Variable *variable(char *id, ValType type, SourcePos pos);

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
	if (argc != 2) {
		eprintf("usage: %s <filename>", getprogname());
	}

	/* TODO: Uncomment the following code for code generation:
	if ((jasmin_path = getenv("JASMIN_JAR")) == NULL) {
		eprintf("JASMIN_JAR environment variable not set");
	}
	*/
	init_symbol_table();

	setsrcname(argv[1]);

	/* open the source file, and report an error if it cannot be opened */
	if ((src_file = fopen(argv[1], "r")) == NULL) {
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
	release_symbol_table();
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

	while (token.type == TOK_ID) {
		parse_subdef();
	}

	expect(TOK_MAIN);
	expect(TOK_COLON);

	parse_body();

	free(class_name);

	DBG_end("</program>");
}

/*
 * subdef = id "(" type id {"," type id} ")" ["->" type] ":" body .
 */
void parse_subdef(void)
{
	char *id;

	DBG_start("<subdef>");

	ValType t1, *params;
	SourcePos pos;
	Variable *plist, *p, *q;
	int nparams, i;
	plist = NULL;

	expect_id(&id);
	expect(TOK_LPAREN);
	parse_type(&t1);
	pos = position;
	expect_id(&id);
	plist = variable(id, t1, pos);
	q = plist;

	while (token.type == TOK_COMMA) {
		get_token(&token);
		parse_type(&t1);
		pos = position;
		expect_id(&id);
		plist->next = variable(id, t1, pos);
		plist = plist->next;
	}

	plist = q;

	nparams = 0;
	p = plist;

	while (p != NULL) {
		nparams++;
		p = p->next;
	}

	params = (ValType *)malloc(nparams * sizeof(ValType));
	p = plist;

	for (i = 0; i < nparams; i++) {
		params[i] = p->type;
		p = p->next;
	}

	expect(TOK_RPAREN);

	if (token.type == TOK_ARROW) {
		expect(TOK_ARROW);
		parse_type(&t1);

		IDPropt *propt = idpropt(t1, get_variables_width(), nparams, params);
		open_subroutine(id, propt);
	} else {
		IDPropt *propt = idpropt(TYPE_CALLABLE, get_variables_width(), nparams,
					params);
		open_subroutine(id, propt);
	}

	p = plist;

	for (i = 0; i < nparams; i++) {
		insert_name(p->id, idpropt(p->type, get_variables_width(), 0, NULL));
		p = p->next;
	}

	expect(TOK_COLON);

	parse_body();

	//free(id);

	while (plist != NULL) {
		q = plist;
		plist = plist->next;
		free(q);
	}

	close_subroutine();

	DBG_end("</subdef>");
}

/*
 * {vardef} statements .
 */
void parse_body(void)
{
	DBG_start("<body>");

	while (IS_TYPE(token.type)) {
		parse_vardef();
	}

	parse_statements();

	DBG_end("</body>");
}

/*
 * ("bool"|"int")["array"].
 */
void parse_type(ValType *type)
{
	DBG_start("<type>");

	ValType t;
	t = TYPE_NONE;

	if (token.type == TOK_BOOL) {
		expect(TOK_BOOL);
		t |= TYPE_BOOLEAN;
	} else if (token.type == TOK_INT) {
		expect(TOK_INT);
		t |= TYPE_INTEGER;
	} else {
		abort_c(ERR_EXPECTED_TYPE_SPECIFIER, token.type);
	}

	if (token.type == TOK_ARRAY) {
		expect(TOK_ARRAY);
		t |= TYPE_ARRAY;
	} 

	*type = t;

	DBG_end("</type>");
}

/*
 * type id {"," id} ";" .
 */
void parse_vardef(void)
{
	char *id;

	DBG_start("<vardef>");

	ValType t1;
	t1 = TYPE_NONE;

	parse_type(&t1);
	expect_id(&id);

	insert_name(id, idpropt(t1, get_variables_width(), 0, NULL));

	while (token.type == TOK_COMMA) {
		get_token(&token);
		expect_id(&id);
		insert_name(id, idpropt(t1, get_variables_width(), 0, NULL));
	}

	expect(TOK_SEMICOLON);

	free(id);

	DBG_end("</vardef>");
}

/*
 * "chillax" | statement {";" statement} .
 */
void parse_statements(void)
{
	DBG_start("<statements>");

	if (token.type == TOK_CHILLAX) {
		expect(TOK_CHILLAX);
	} else {
		parse_statement();
		while (token.type == TOK_SEMICOLON) {
			get_token(&token);
			parse_statement();
		}
	}

	DBG_end("</statements>");
}

/*
 * assign | call | if | input | output | return | while .
 */
void parse_statement(void)
{
	DBG_start("<statement>");

	switch (token.type) {
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
 * "let" id [index] "=" (expr | "array" simple) .
 */
void parse_assign(void)
{
	char *id;

	DBG_start("<assign>");

	ValType t1, t2;
	t1 = TYPE_NONE;
	t2 = TYPE_NONE;
	IDPropt *prop;
	int index = 0;
	SourcePos pos;

	expect(TOK_LET);
	pos = position;
	expect_id(&id);

	if (find_name(id, &prop)) {
		t2 = prop->type;
	} else {
		abort_c(ERR_UNKNOWN_IDENTIFIER, token.type);
	}

	if (token.type == TOK_LBRACK) {
		parse_index(id);
		index = 1;
	}

	expect(TOK_EQ);

	if (STARTS_EXPR(token.type)) {
		parse_expr(&t1);
		pos = position;
		chktypes(t2, t1, &pos);

		if (IS_ARRAY(t2) && index != 1) {
			abort_c(ERR_ILLEGAL_ARRAY_OPERATION, token.type);
		}
	} else if (token.type == TOK_ARRAY) {
		get_token(&token);
		parse_simple(&t1);
		pos = position;

		chktypes(t1, TYPE_INTEGER, &pos);

		if (!IS_ARRAY(t2) || index == 1) {
			abort_c(ERR_EXPECTED_SCALAR, t1);
		}
	} else {
		abort_c(ERR_EXPECTED_EXPRESSION_OR_ARRAY_ALLOCATION, token.type);
	}

	free(id);

	DBG_end("</assign>");
}

/*
 * id arglist .
 */
void parse_call(void)
{
	char *id;

	DBG_start("<call>");

	IDPropt *propt;

	expect_id(&id);

	find_name(id, &propt);

	if (IS_PROCEDURE(propt->type)) {
		parse_arglist(id, propt);
	} else if (IS_FUNCTION(propt->type)) {
		//error
	} else {
		//error
	}

	free(id);

	DBG_end("</call>");
}

/*
 * "if" expr ":" statements {"elif" expr ":" statements} ["else" ":" statements]
 * "end" .
 */
void parse_if(void)
{
	DBG_start("<if>");

	ValType t1;
	SourcePos pos;

	t1 = TYPE_NONE;

	expect(TOK_IF);
	parse_expr(&t1);
	pos = position;

	chktypes(t1, TYPE_BOOLEAN, &pos);

	expect(TOK_COLON);
	parse_statements();

	while (token.type == TOK_ELIF) {
		DBG_info("<elif>");
		get_token(&token);
		parse_expr(&t1);
		pos = position;

		chktypes(t1, TYPE_BOOLEAN, &pos);

		expect(TOK_COLON);
		parse_statements();
		DBG_info("</elif.");
	}

	if (token.type == TOK_ELSE) {
		DBG_info("<else>");
		get_token(&token);
		expect(TOK_COLON);
		parse_statements();
		DBG_info("</else>");
	}

	expect(TOK_END);

	DBG_end("</if>");
}

/*
 * "input" "(" id [index] ")" .
 */
void parse_input(void)
{
	char *id;

	DBG_start("<input>");

	expect(TOK_INPUT);
	expect(TOK_LPAREN);
	expect_id(&id);

	if (token.type == TOK_LBRACK) {
		parse_index(id);
	}

	expect(TOK_RPAREN);

	free(id);

	DBG_end("</input>");
}

/*
 * "output" "(" (string|expr) {".." (string|expr)} ")" .
 */
void parse_output(void)
{
	DBG_start("<output>");

	ValType t1;

	expect(TOK_OUTPUT);
	expect(TOK_LPAREN);

	if (STARTS_EXPR(token.type)) {
		parse_expr(&t1);
	} else if (token.type == TOK_STR) {
		get_token(&token);
	} else {
		abort_c(ERR_EXPECTED_EXPRESSION_OR_STRING, token.type);
	}

	while (token.type == TOK_DOTDOT) {
		get_token(&token);
		if (STARTS_EXPR(token.type)) {
			parse_expr(&t1);
		} else if (token.type == TOK_STR) {
			get_token(&token);
		} else {
			abort_c(ERR_EXPECTED_EXPRESSION_OR_STRING, token.type);
		}
	}

	expect(TOK_RPAREN);

	DBG_end("</output>");
}

/*
 * "return" [expr] .
 */
void parse_return(void)
{
	DBG_start("<return>");

	ValType t1;

	expect(TOK_RETURN);

	if (STARTS_EXPR(token.type)) {
		parse_expr(&t1);
	}

	DBG_end("</return>");
}

/*
 * "while" expr ":" statements "end" .
 */
void parse_while(void)
{
	DBG_start("<while>");

	ValType t1;
	SourcePos pos;
	t1 = TYPE_NONE;

	expect(TOK_WHILE);
	parse_expr(&t1);
	pos = position;

	chktypes(t1, TYPE_BOOLEAN, &pos);

	expect(TOK_COLON);
	parse_statements();
	expect(TOK_END);

	DBG_end("</while>");
}

/*
 * "(" expr {"," expr} ")" .
 */
void parse_arglist(char *id, IDPropt *propt)
{
	DBG_start("<arglist>");

	ValType t1, t2;
	SourcePos pos;
	unsigned int k0, k1, n;
	k0 = 0;
	t1 = TYPE_NONE;
	t2 = TYPE_NONE;

	find_name(id, &propt);
	t2 = propt->type;
	pos = position;

	chktypes(t2, TYPE_CALLABLE, &pos);

	expect(TOK_LPAREN);
	parse_expr(&t1);

	pos = position;
	n = propt->nparams;
	t2 = propt->params[0];
	chktypes(t1, t2, &pos);

	while (token.type == TOK_COMMA) {
		get_token(&token);
		parse_expr(&t1);
		k1 = k0;
		k0++;
		pos = position;
		t2 = propt->params[n - k1];
		chktypes(t1, t2, &pos);
	}
	
	propt->nparams = k1 + 1;
	propt->type = t2;

	expect(TOK_RPAREN);

	DBG_end("</arglist>");
}

/*
 * "[" simple "]" .
 */
void parse_index(char *id)
{
	DBG_start("<index>");

	ValType t1, t2;
	SourcePos pos;
	IDPropt *propt;
	t1 = TYPE_NONE;
	t2 = TYPE_NONE;

	find_name(id, &propt);
	t2 = propt->type;

	expect(TOK_LBRACK);
	parse_simple(&t1);
	pos = position;

	chktypes(t1, TYPE_INTEGER, &pos);

	if (!IS_ARRAY_TYPE(t2)) {
		//error
	}

	expect(TOK_RBRACK);

	DBG_end("</index>");
}

/*
 * simple [relop simple] .
 */
void parse_expr(ValType *type)
{
	DBG_start("<expr>");
	ValType t1, t2;
	Token t;
	SourcePos pos;
	
	t1 = TYPE_NONE;
	t2 = TYPE_NONE;

	parse_simple(&t1);

	pos = position;
	chktypes(t1, TYPE_INTEGER, &pos);

	if (IS_RELOP(token.type)) {
		t = token;
		get_token(&token);
		parse_simple(&t2);
		pos = position;

		if (t.type == TOK_EQ) {
			chktypes(t1, t2, &pos);
			*type = TYPE_BOOLEAN;
		} else if (t.type == TOK_NE) {
			chktypes(t1, t2, &pos);
			*type = TYPE_BOOLEAN;
		} else if (t.type == TOK_GE) {
			chktypes(t1, t2, &pos);
			*type = TYPE_BOOLEAN;
		} else if (t.type == TOK_GT) {
			chktypes(t1, t2, &pos);
			*type = TYPE_BOOLEAN;
		} else if (t.type == TOK_LE) {
			chktypes(t1, t2, &pos);
			*type = TYPE_BOOLEAN;
		} else if (t.type == TOK_LT) {
			chktypes(t1, t2, &pos);
			*type = TYPE_BOOLEAN;
		}
	} else {
		*type = t1;
	}

	DBG_end("</expr>");
}

/*
 * ["-"] term {addop term} .
 */
void parse_simple(ValType *type)
{
	DBG_start("<simple>");

	ValType t1, t2;
	Token t;
	SourcePos pos1, pos2;

	t1 = TYPE_NONE;
	t2 = TYPE_NONE;

	if (token.type == TOK_MINUS) {
		t = token;
		get_token(&token);
	}

	parse_term(&t1);
	pos1 = position;

	if (IS_ADDOP(token.type)) {
		while (IS_ADDOP(token.type)) {
			t = token;
			get_token(&token);
			parse_term(&t2);
			pos2 = position;

			if (t.type == TOK_PLUS) {
				chktypes(t1, TYPE_INTEGER, &pos1);
				chktypes(t1, t2, &pos2);
				*type = t1;
			} else if (t.type == TOK_MINUS) {
				chktypes(t1, TYPE_INTEGER, &pos1);
				chktypes(t1, t2, &pos2);
				*type = t1;
			} else if (t.type == TOK_OR) {
				chktypes(t1, TYPE_BOOLEAN, &pos1);
				chktypes(t1, t2, &pos2);
				*type = t1;
			}
		}
	} else {
		if (t.type == TOK_MINUS) {
			chktypes(t1, TYPE_INTEGER, &pos1, "for unary minus");
			*type = t1;
		} else {
			*type = t1;
		}
	}

	DBG_end("</simple>");
}

/*
 * factor {mulop factor} .
 */
void parse_term(ValType *type)
{
	DBG_start("<term>");

	ValType t1, t2;
	Token t;
	SourcePos pos1, pos2;
	t1 = TYPE_NONE;
	t2 = TYPE_NONE;

	parse_factor(&t1);
	pos1 = position;

	if (IS_MULOP(token.type)) {
		while (IS_MULOP(token.type)) {
			t = token;
			get_token(&token);
			parse_factor(&t2);
			pos2 = position;

			if (t.type == TOK_MUL) {
				chktypes(t1, TYPE_INTEGER, &pos1);
				chktypes(t1, t2, &pos2);
				*type = t1;
			} else if (t.type == TOK_DIV) {
				chktypes(t1, TYPE_INTEGER, &pos1);
				chktypes(t1, t2, &pos2);
				*type = t1;
			} else if (t.type == TOK_REM) {
				chktypes(t1, TYPE_INTEGER, &pos1);
				chktypes(t1, t2, &pos2);
				*type = t1;
			} else if (t.type == TOK_AND) {
				chktypes(t1, TYPE_BOOLEAN, &pos1);
				chktypes(t1, t2, &pos2);
				*type = t1;
			}
		}
	} else {
		*type = t1;
	}

	DBG_end("</term>");
}

/*
 * id [index | arglist] | num | "(" expr ")" | "not" factor | "true" | "false".
 */
void parse_factor(ValType *type)
{
	char *id;

	DBG_start("<factor>");

	ValType t1;
	t1 = TYPE_NONE;
	IDPropt *prop;
	SourcePos pos;

	switch (token.type) {
		case TOK_ID:
			expect_id(&id);

			if (token.type == TOK_LBRACK || token.type == TOK_LPAREN) {
				switch (token.type) {
					case TOK_LBRACK:
						parse_index(id);
						find_name(id, &prop);
						t1 = prop->type;
						*type = SET_BASE_TYPE(t1);
						break;

					case TOK_LPAREN:
						parse_arglist(id, prop);
						break;

					default:
						abort_c(ERR_UNREACHABLE, token.type);
						break;
				}
			} else {
				find_name(id, &prop);
				t1 = prop->type;
				if (!IS_FUNCTION(t1)) {
					*type = t1;
				}
			}
			break;

		case TOK_NUM:
			get_token(&token);
			*type = TYPE_INTEGER;
			break;

		case TOK_LPAREN:
			get_token(&token);
			parse_expr(&t1);
			expect(TOK_RPAREN);
			*type = t1;
			break;

		case TOK_NOT:
			get_token(&token);
			parse_factor(&t1);
			pos = position;
			chktypes(t1, TYPE_BOOLEAN, &pos, "for 'not'");
			*type = t1;
			break;

		case TOK_TRUE:
			get_token(&token);
			*type = TYPE_BOOLEAN;
			break;

		case TOK_FALSE:
			get_token(&token);
			*type = TYPE_BOOLEAN;
			break;

		default:
			abort_c(ERR_EXPECTED_FACTOR, token.type);
			break;
	}

	DBG_end("</factor>");
}

/* --- helper routines ------------------------------------------------------ */

#define MAX_MSG_LEN 256

/* TODO: Uncomment the following function for use during type checking. */

void chktypes(ValType found, ValType expected, SourcePos *pos, ...)
{
	char buf[MAX_MSG_LEN], *s;
	va_list ap;

	if (found != expected) {
		buf[0] = '\0';
		va_start(ap, pos);
		s = va_arg(ap, char *);
		vsnprintf(buf, MAX_MSG_LEN, s, ap);
		va_end(ap);
		if (pos) {
			position = *pos;
		}
		leprintf("incompatible types (expected %s, found %s) %s",
				get_valtype_string(expected), get_valtype_string(found), buf);
	}
}

void expect(TokenType type)
{
	if (token.type == type) {
		get_token(&token);
	} else {
		abort_c(ERR_EXPECT, type);
	}
}

void expect_id(char **id)
{
	if (token.type == TOK_ID) {
		*id = estrdup(token.lexeme);
		get_token(&token);
	} else {
		abort_c(ERR_EXPECT, TOK_ID);
	}
}

/* --- constructors --------------------------------------------------------- */

/* TODO: Uncomment the following two functions for use during type checking. */

IDPropt *idpropt(ValType type, unsigned int offset, unsigned int nparams,
		ValType *params)
{
	IDPropt *ip = emalloc(sizeof(*ip));

	ip->type = type;
	ip->offset = offset;
	ip->nparams = nparams;
	ip->params = params;

	return ip;
}

Variable *variable(char *id, ValType type, SourcePos pos)
{
	Variable *vp = emalloc(sizeof(*vp));

	vp->id = id;
	vp->type = type;
	vp->pos = pos;
	vp->next = NULL;

	return vp;
}

/* --- error handling routines ---------------------------------------------- */

void _abort_cp(SourcePos *posp, Error err, va_list args)
{
	char expstr[MAX_MSG_LEN], *s;
	int t;

	if (posp) {
		position = *posp;
	}

	snprintf(expstr, MAX_MSG_LEN, "expected %%s, but found %s",
			get_token_string(token.type));

	switch (err) {
		case ERR_EXPECTED_SCALAR:
			leprintf("expected scalar variable instead of %s", s);
			break;
		case ERR_ILLEGAL_ARRAY_OPERATION:
			t = va_arg(args, int);
			leprintf("%s is an illegal array operation", get_token_string(t));
			break;
		case ERR_MULTIPLE_DEFINITION:
			leprintf("multiple definitions of %s", s);
			break;
		case ERR_NOT_AN_ARRAY:
			leprintf("%s is not an array", s);
			break;
		case ERR_NOT_A_FUNCTION:
			leprintf("%s is not a function", s);
			break;
		case ERR_NOT_A_PROCEDURE:
			leprintf("%s is not a procedure", s);
			break;
		case ERR_NOT_A_VARIABLE:
			leprintf("%s is not a variable", s);
			break;
		case ERR_TOO_FEW_ARGUMENTS:
			leprintf("too few arguments for call to %s", s);
			break;
		case ERR_TOO_MANY_ARGUMENTS:
			leprintf("too many arguments for call to %s", s);
			break;
		case ERR_UNKNOWN_IDENTIFIER:
			t = va_arg(args, int);
			leprintf("unknown identifier %s", get_token_string(t));
			break;
		case ERR_RETURN_EXPRESSION_NOT_ALLOWED:
			leprintf("a return expression is not allowed for a procedure");
			break;
		case ERR_MISSING_RETURN_EXPRESSION:
			leprintf("missing return expression for a function");
			break;
		case ERR_UNREACHABLE:
			s = va_arg(args, char *);
			break;
		default:
			break;
	}

	switch (err) {

		/* TODO: Add additional cases here as is necessary, referring to
		 * errmsg.h for all possible errors.  Some errors only become possible
		 * to recognise once we add type checking.  Until you get to type
		 * checking, you can handle such errors by adding the default case.
		 * However, your final submission *must* handle all cases explicitly.
		 */

		case ERR_EXPECT:
			t = va_arg(args, int);
			leprintf(expstr, get_token_string(t));
			break;

		case ERR_EXPECTED_FACTOR:
			leprintf(expstr, "factor");
			break;

		case ERR_UNREACHABLE:
			leprintf("unreachable: %s", s);
			break;

		case ERR_EXPECTED_TYPE_SPECIFIER:
			t = va_arg(args, int);
			leprintf("expected type specifier, but found %s",
			get_token_string(t));
			break;

		case ERR_EXPECTED_STATEMENT:
			t = va_arg(args, int);
			leprintf("expected statement, but found %s", get_token_string(t));
			break;

		case ERR_EXPECTED_EXPRESSION_OR_ARRAY_ALLOCATION:
			t = va_arg(args, int);
			leprintf("expected expression or array allocation, but found %s",
			get_token_string(t));
			break;

		case ERR_EXPECTED_EXPRESSION_OR_STRING:
			t = va_arg(args, int);
			leprintf("expected expression or string, but found %s",
			get_token_string(t));
			break;

		default:
			/* avoid warning default */
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

	for (i = 0; i < indent; i++) {
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
