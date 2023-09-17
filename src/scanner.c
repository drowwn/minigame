/**
 * @file    scanner.c
 * @brief   The scanner for AMPL-2023.
 * @author  W.H.K. Bester (whkbester@cs.sun.ac.za)
 *  & Josef Emile Oosthuizen (26507404@sun.ac.za)
 * @date    2023-06-29
 */

#include "scanner.h"
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "boolean.h"
#include "error.h"
#include "token.h"

/* --- global static variables ----------------------------------------------
 */

static FILE *src_file;	  /* the source file pointer */
static int ch;			  /* the next source character */
static int column_number; /* the current column number */
static struct
{
	char *word;		/* the reserved word, i.e., the lexeme */
	TokenType type; /* the associated token type */
} reserved[] = {
	{"and", TOK_AND},
	{"array", TOK_ARRAY},
	{"bool", TOK_BOOL},
	{"chillax", TOK_CHILLAX},
	{"elif", TOK_ELIF},
	{"else", TOK_ELSE},
	{"end", TOK_END},
	{"false", TOK_FALSE},
	{"if", TOK_IF},
	{"input", TOK_INPUT},
	{"int", TOK_INT},
	{"let", TOK_LET},
	{"main", TOK_MAIN},
	{"not", TOK_NOT},
	{"or", TOK_OR},
	{"output", TOK_OUTPUT},
	{"program", TOK_PROGRAM},
	{"rem", TOK_REM},
	{"return", TOK_RETURN},
	{"true", TOK_TRUE},
	{"while", TOK_WHILE}};

#define NUM_RESERVED_WORDS (sizeof(reserved) / sizeof(reserved[0]))
#define MAX_INIT_STR_LEN (1024)

/* --- function prototypes --------------------------------------------------
 */

static void next_char(void);
static void process_number(Token *token);
static void process_string(Token *token);
static void process_word(Token *token);
static void skip_comment(void);

/* --- scanner interface ----------------------------------------------------
 */

void init_scanner(FILE *in_file)
{
	src_file = in_file;
	position.line = 1;
	position.col = column_number = 0;
	next_char();
}

void get_token(Token *token)
{
	/* remove whitespace */
	while (isspace(ch))
	{
		next_char();
	}

	/* remember token start */
	position.col = column_number;
	SourcePos start_pos = position;

	/* get the next token */
	if (ch != EOF)
	{
		/* process word literal */
		if (isalpha(ch) || ch == '_')
		{
			process_word(token);
		}
		else if (isdigit(ch))
		{
			process_number(token);
		}
		else
		{
			switch (ch)
			{
				/* process string literal */
			case '"':
				next_char();
				process_string(token);
				break;
				/* process skipping comments */
			case '{':
				skip_comment();
				get_token(token);
				break;
				/* process operators: */
			case ':':
				token->type = TOK_COLON;
				next_char();
				break;
			case ';':
				token->type = TOK_SEMICOLON;
				next_char();
				break;
			case ',':
				token->type = TOK_COMMA;
				next_char();
				break;
			case '(':
				token->type = TOK_LPAREN;
				next_char();
				break;
			case ')':
				token->type = TOK_RPAREN;
				next_char();
				break;
			case '[':
				token->type = TOK_LBRACK;
				next_char();
				break;
			case ']':
				token->type = TOK_RBRACK;
				next_char();
				break;
			case '=':
				token->type = TOK_EQ;
				next_char();
				break;
			case '*':
				token->type = TOK_MUL;
				next_char();
				break;
			case '+':
				token->type = TOK_PLUS;
				next_char();
				break;
				/* process compound operators */
			case '-':
				next_char();
				if (ch == '>')
				{
					token->type = TOK_ARROW;
					next_char();
				}
				else
				{
					token->type = TOK_MINUS;
				}
				break;
			case '.':
				next_char();
				if (ch == '.')
				{
					token->type = TOK_DOTDOT;
					next_char();
				}
				else
				{
					ch = '.';
					goto illegal;
				}
				break;
			case '>':
				next_char();
				if (ch == '=')
				{
					token->type = TOK_GE;
					next_char();
				}
				else
				{
					token->type = TOK_GT;
				}
				break;
			case '<':
				next_char();
				if (ch == '=')
				{
					token->type = TOK_LE;
					next_char();
				}
				else
				{
					token->type = TOK_LT;
				}
				break;
			case '/':
				next_char();
				if (ch == '=')
				{
					token->type = TOK_NE;
					next_char();
				}
				else
				{
					token->type = TOK_DIV;
				}
				break;
				/* if the character is not one of these cases, it is
				 * illegal*/
			illegal:
			default:
				position = start_pos;
				leprintf("illegal character '%c' (ASCII #%d)", ch, ch);
				next_char();
				break;
			}
		}
	}
	else
	{
		token->type = TOK_EOF;
	}
}

/* gets the next character in the input file*/
void next_char(void)
{
	static char last_read = '\0';
	ch = fgetc(src_file);

	if (ch == EOF)
	{
		return;
	}
	/* if a newline character is found, the column and line are set
	 * accordingly*/
	if (last_read == '\n')
	{
		column_number = 1;
		position.line++;
	}
	else
	{
		column_number++;
	}

	last_read = ch;
}

void process_number(Token *token)
{
	int val = 0;
	SourcePos start_pos = position;
	while (isdigit(ch))
	{
		/* make sure the number is not bigger than the max integer size*/
		if ((val * 10 + (ch - '0')) <= (INT_MAX) &&
			(val * 10 + (ch - '0')) > (INT_MIN))
		{
			/* builds number up from characters*/
			val = val * 10 + (ch - '0');
		}
		else
		{
			/* position gets set to before the invalid number was found*/
			position = start_pos;
			leprintf("number too large");
			return;
		}
		next_char();
	}
	token->value = val;
	token->type = TOK_NUM;
}

void process_string(Token *token)
{
	size_t i = 0, nstring = MAX_INIT_STR_LEN;
	char *string = (char *)emalloc(nstring);
	SourcePos start_pos = position;
	int start_col = position.col;
	int start_line = position.line;

	while (ch != '"')
	{
		/* if the string is not closed,
		the position is set to before the string was found*/
		if (ch == EOF)
		{
			position = start_pos;
			leprintf("string not closed");
			return;
		}
		/* resize string (x2) */
		if (i == nstring - 1)
		{
			nstring *= 2;
			string = erealloc(string, nstring);
		}
		/*  the position is set to the invalid character*/
		if (!isprint(ch))
		{
			position.col = start_col + i + 1;
			position.line = start_line;
			leprintf("non-printable character (ASCII #%d) in string", ch);
			return;
		}
		/* the next character is checked for and the legality escape codes*/
		if (ch == '\\')
		{
			string[i++] = ch;
			next_char();

			/*  the position is set to the illegal escape code*/
			if (ch != 'n' && ch != 't' && ch != '\\' && ch != '"')
			{
				position.col = start_col + i;
				position.line = start_line;
				leprintf("illegal escape code '\\%c' in string", ch);
				return;
			}
		}
		string[i++] = ch;
		next_char();
	}

	/* string is closed*/
	string[i] = '\0';
	token->string = string;
	token->type = TOK_STR;
	next_char();
}

void process_word(Token *token)
{
	char lexeme[MAX_ID_LEN + 1];
	int i = 0, cmp, low = 0, mid, high = NUM_RESERVED_WORDS - 1;
	SourcePos start_pos = position;

	/* build up lexeme */
	while ((isalnum(ch) || ch == '_'))
	{
		if (i >= MAX_ID_LEN)
		{
			position = start_pos;
			leprintf("identifier too long");
			return;
		}
		lexeme[i++] = ch;
		next_char();
	}
	lexeme[i] = '\0';

	/* binary searching for matching reserved word*/
	while (low <= high)
	{
		mid = (low + high) / 2;
		cmp = strcmp(lexeme, reserved[mid].word);
		if (cmp < 0)
		{
			high = mid - 1;
		}
		else if (cmp > 0)
		{
			low = mid + 1;
		}
		else if (cmp == 0)
		{
			token->type = reserved[mid].type;
			return;
		}
	}
	/* if the word does not match any reserved words, it is an identifier*/
	token->type = TOK_ID;
	strcpy(token->lexeme, lexeme);
}

void skip_comment(void)
{
	position.col = column_number;
	SourcePos start_pos = position;
	next_char();

	while (ch != EOF)
	{
		if (ch == '{')
		{ /* nested comment */
			skip_comment();
		}
		else if (ch == '}')
		{ /* comment closed */
			next_char();
			return;
		}
		else
		{
			next_char();
		}
	}
	/* EOF has been reached because the comment was not closed*/
	position = start_pos;
	leprintf("comment not closed");
}
