/**
 * @file    token.h
 * @brief   Data type definitions for the lexical analyser (scanner) of
 *          AMPL-2023.
 * @author  W.H.K. Bester (whkbester@cs.sun.ac.za)
 * @date    2023-06-29
 */

#ifndef TOKEN_H
#define TOKEN_H

/** the maximum length of an identifier */
#define MAX_ID_LEN 32

/** the types of tokens that the scanner recognises */
typedef enum
{

	TOK_EOF, /* end-of-file    */
	TOK_ID,	 /* identifier     */
	TOK_NUM, /* number literal */
	TOK_STR, /* string literal */

	/* keywords: note that the boolean operators AND and OR, and the remainder
	 * operator REM, although written out as string literals, are still
	 * treated as operators
	 */
	TOK_ARRAY,
	TOK_BOOL,
	TOK_CHILLAX,
	TOK_ELIF,
	TOK_ELSE,
	TOK_END,
	TOK_IF,
	TOK_INPUT,
	TOK_INT,
	TOK_LET,
	TOK_MAIN,
	TOK_OUTPUT,
	TOK_PROGRAM,
	TOK_RETURN,
	TOK_WHILE,

	/* boolean constants */
	TOK_FALSE,
	TOK_TRUE,

	/* unary-only operator */
	TOK_NOT,

	/* relational operators: the order of these operators is significant -- it
	 * allows us to do range checking in the parser
	 */
	TOK_EQ, /* equal to                 */
	TOK_GE, /* greater than or equal to */
	TOK_GT, /* (strictly) greater than  */
	TOK_LE, /* less than or equal to    */
	TOK_LT, /* (strictly) less than     */
	TOK_NE, /* not equal to             */

	/* additive operators */
	TOK_MINUS,
	TOK_OR,
	TOK_PLUS,

	/* multiplicative operators */
	TOK_AND,
	TOK_DIV,
	TOK_MUL,
	TOK_REM,

	/* other non-alphabetic operators */
	TOK_ARROW,
	TOK_COLON,
	TOK_COMMA,
	TOK_DOTDOT,
	TOK_LBRACK,
	TOK_LPAREN,
	TOK_RBRACK,
	TOK_RPAREN,
	TOK_SEMICOLON

} TokenType;

/** the token data type */
typedef struct
{
	TokenType type; /**< the type of the token        */
	union
	{
		int value;					 /**< numeric value (for integers) */
		char lexeme[MAX_ID_LEN + 1]; /**< lexeme for identifiers       */
		char *string;				 /**< string (for write)           */
	};
} Token;

/**
 * Return a string representation of the specified token type.
 *
 * @param[in]  type
 *     the token for which to get the string representation
 * @return
 *     a (constant) string representation of the specified token type
 */
const char *get_token_string(TokenType type);

#endif /* TOKEN_H */
