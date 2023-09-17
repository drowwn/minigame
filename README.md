# minigame

Project spec:

C H A P T E R 3
THIS
DOCUMENT IS
NON
ACADEMIC AND
HAS IS NOT
COPYRIGHTED,
THIS
DOCUMENT IS
FOR PERSONAL
RESEARCH
PURPOSES
ONLY
EBNF and scanner
Te Extended Backus–Naur Form (EBNF) of the 2023 ampl grammer is given in figure 3.1.
Some language properties must be handled in the lexical analyser (scanner), but cannot be
specifed in the grammar; they follow below in §3.1. Tis section concludes with the errors
that may be emitted by the scanner, that is, which are handled by the scanner locally and
terminates the compiler.
3.1
L E X I C A L L A N G U A G E P R O P E R T I E S
3.1.1 The regular part of the grammar
Te scanner is actually a Deterministic Finite State Automaton (DFA), which means it can
recognise a regular language. Tese are just the languages we can write without recursion in
the grammar productions. Terefore, your scannermustrecognise the productions for⟨ id ⟩,⟨ num ⟩ , and ⟨ string ⟩ ; the productions for ⟨ letter ⟩ and ⟨ digit ⟩ (and for parsing, also ⟨ relop ⟩ ,
⟨ addop ⟩, and⟨ mulop ⟩) are just there so that, for example, we do not have to write:
⟨ id ⟩ = (“a”∣ . . .∣ “ z”∣ “ A”∣ . . .∣ “ Z”∣ “ _”) {“a”∣ . . .∣ “ z”∣ “ A”∣ . . .∣ “ Z”∣ “ _”∣ “ 0”∣ . . .∣ “ 9”} .
Tis is confusing to read, which is why we rather create extra EBNF productions. Note that
⟨ id ⟩or⟨ num ⟩lexemesmust notcontain any spaces.
3.1.2 Comments
An ampl source code fle may contain comments, which are skipped by the scanner. A
comment block mustbe started by the character“{”andmustended by the character“}”.
Commentsmaybe nested. Commentssha ll no t be allowed inside literals. In particular,
when the characters“{”and“}”appear inside a string literal, they are to be treated as normal
characters in the string, andmust notbe taken the signify the start and end, respectively, of
a comment. Although counting arguments are possible, the function for skipping comments
mustbe written recursively.
1314
ebnf and scanner
⟨ program ⟩= “program”⟨ id ⟩“:” { ⟨ subdef ⟩ } “main” “:”⟨ body ⟩.
⟨ subdef ⟩= ⟨ id ⟩“(”⟨ type ⟩ ⟨ id ⟩ { “,”⟨ type ⟩ ⟨ id ⟩ } “)” “[ ->”⟨ type ⟩ ] “:”⟨ body ⟩.
⟨ body ⟩= { ⟨ vardef ⟩ ⟨} statements ⟩.
⟨ type ⟩= “( bool” “∣ int”) [“array” .]
⟨ vardef ⟩= ⟨ type ⟩ ⟨ id ⟩ { “,”⟨ id ⟩ } “;” .
⟨ statements ⟩= “chillax” ∣⟨ statement ⟩ { “;”⟨ statement ⟩ } .
⟨ statement ⟩= ⟨ assign ⟩ ⟨∣ call ⟩ ⟨∣ if ⟩ ⟨∣ input ⟩ ⟨∣ output ⟩ ⟨∣ return ⟩ ⟨∣ while ⟩.
⟨ assign ⟩= “let”⟨ id ⟩ ⟨[ index ⟩ ] “=” ( ⟨ expr ⟩ ∣ “ array”⟨ simple ⟩ ) .
⟨ call ⟩= ⟨ id ⟩ ⟨ arglist ⟩.
⟨ if ⟩= “if”⟨ expr ⟩“:”⟨ statements ⟩ { “elif”⟨ expr ⟩“:”⟨ statements ⟩}
[ “else” “:”⟨ statements ⟩ ] “end” .
⟨ input ⟩= “input” “(”⟨ id ⟩ ⟨[ index ⟩ ] “)” .
⟨ output ⟩= “output” “(” ( ⟨ string ⟩ ⟨∣ expr ⟩) {“..” ( ⟨ string ⟩ ⟨∣ expr ⟩) } “)” .
⟨ return ⟩= “return” [ ⟨ expr ⟩ ] .
⟨ while ⟩= “while”⟨ expr ⟩“:”⟨ statements ⟩“end” .
⟨ arglist ⟩= “(”⟨ expr ⟩ { “,”⟨ expr ⟩ } “)” .
⟨ index ⟩= “[”⟨ simple ⟩“]” .
⟨ expr ⟩= ⟨ simple ⟩ ⟨[ relop ⟩ ⟨ simple ⟩ ] .
⟨ relop ⟩= “=” “∣ >=” “∣ > ” “∣ <=” “∣ < ” “∣ /=” .
⟨ simple ⟩=[ “-” ] ⟨ term ⟩ ⟨{ addop ⟩ ⟨ term ⟩ } .
⟨ addop ⟩= “-” “∣ or” “∣ + ” .
⟨ term ⟩= ⟨ factor ⟩ ⟨{ mulop ⟩ ⟨ factor ⟩ } .
⟨ mulop ⟩= “and” “∣ / ” “∣ * ” “∣ rem” .
⟨ factor ⟩= ⟨ id ⟩ ⟨[ index ⟩ ⟨∣ arglist ⟩] ∣⟨ num ⟩ ∣ “ (”⟨ expr ⟩“)” “∣ not” factor “∣ true”∣
“false” .
⟨ id ⟩= ⟨ letter ⟩ ⟨{ letter ⟩ ⟨∣ digit ⟩ } .
⟨ num ⟩= ⟨ digit ⟩ ⟨{ digit ⟩ } .
⟨ string ⟩= “"” { ⟨ printable ASCII ⟩ } “"” .
⟨ letter ⟩= “a” ... “∣ ∣ z ” “∣ A ” ... “∣
⟨ digit ⟩= “0” ... “∣ ∣ 9 ” .
∣ Z ” “∣ _ ” .
figure 3.1:Te EBNF of ampl-2023.
§3.1Lexical language properties
§3.1
15
table 3.1:Te allowed escape sequences in ampl string literals.
escape sequence meaning
\n
\t
\"
\\
Linefeed (end-of-line)
Tab
Double quote (so we can use it in a string)
Backslash (so we can use it in a string)
3.1.3 Valid characters
Except inside comments and string literals, any character not explicitly allowed by the EBNF
mustbe an error anywhere in ampl source code. Comments may contain any character
from the UTF-8 character set, which, for all practical intents and purposes, means you skip *
everything that is not “{” or “}”.
String literalsmustbe handled according to what the Java assembler Jasmin allows. To be
safe, we restrict ourselves to the 7-bit ascii characters, as can be determined by the functions
given in thectype.hheader, and we only allow the escape sequences given in table 3.1.
If a backslash is followed by anything other than specifed in table 3.1, youmustdisplay
an error and exit. Note, however, that you must not convert an escape sequence into its
equivalent ascii code. We must not do the conversion because the Java assembler expects
strings to contain raw escape sequences, that is, with the escape sequences not converted to
ascii codes. Also, the two delimiting double quote characters are not part of the string and
must notbe stored by scanner.
3.1.4 Identifers
Te maximum length for identifersmustbe 32 characters, not including the terminating
null character. As always on a Unix system, identifersmustbe case-sensitive.
3.1.5 Integer domain
Since ampl has the JVM as target architecture, integral numbers must be signed 32-bit
integers. Note that the EBNF does not provide for literals of negative numbers, which is to
say, the scanner itself cannot recognise negative numbers. Tey may, however, be written
by negating a positive number; see the optional minus in the ⟨ simple ⟩ rule. Terefore, the
magnitude of our integers are symmetric about zero (whereas two’s complement signed
compatible with 7-bit ascii in its lower seven bits. However, to encode the full complement of Unicode characters,
it uses a variable length encoding. Letters for most European and Middle Eastern languages ft into one- and
two-byte sequences, whereas ideograms for Eastern languages ofen take up three bytes each. So, technically
speaking, if we allow any UTF-8 character, we have to make sure that whatever character we react to for delimiting
comments are not part of a multibyte sequence. However, since few, if any, of you will know Unicode character
codes and be as anal as I am about setting up a Compose key to produce diacritics, we leave the matter here.16
ebnf and scanner
§3.2
integers are actually asymmetric about zero since zero is treated as a nonnegative number)—
refer to Appendix A in your textbook for more detail; we won’t pay the matter any
further
attention here.
While scanning an integer, youmustensure that
10 v + d ⩽INT_MAX,
(3.1)
wherev is the current value of the integer scanned thus far,d ∈ {0, 1, . . . , 9} is the numeric
face value of the next digit, andINT_MAXis the maximum positive magnitude of the system’s
inttype, as specifed inlimits.h. You must test for overfow before it occurs, and you
must notdo the conversion by converting to a type other thanint. Remember: C does
not guarantee alongvalue is at least 64 bits. Terefore, we cannot simply use thelongand
fag overfow afer it has already occurred. Rewrite Eq.(3.1)into a suitable form, so that you
can test for overfow before it occurs.
3.1.6 Reserved words
Te scannermustrecognised all reserved words in ampl, whichmustbe accomplished by
a binary search of the sorted sequence of reserved words specifed in the EBNF. Areserved
wordis a sequence of symbols that cannot be used as an identifer, typically, because it appears
as a syntactic feature of the language, which, in the case of ampl, means it appears as a
terminal in the EBNF. Examples of ampl reserved words are “if” and “true”.
In ampl, all keywords are reserved words, but not all reserved words are keywords,
strictly speaking. Akeywordis a semantic feature of the language, which is to say, it has
meaning in the language itself. Strictly speaking, although they are reserved words,“input”
and“output”– the identifers of the input and output subroutines which constitute ampl’s
standard library – are not keywords. *
3.2
S C A N N E R E R R O R S
Te scannermustrecognise the errors given in this subsection. Pay particular attention to
the format of the error message as your work will be tested programmatically. In the following,
indicates a single space, and'is a normal single quote character. Also, note that messages
generally do not end on a full stop, and that no message has initial or trailing whitespace
characters.
“input”and“output”can be inserted into the symbol table when the compiler starts, and then treated like any
other identifer in the source code during lookup, even though they refer to subroutines added as boilerplate code
by the code generation unit. Instead, we are going to treat them like keywords, which is to say, we will have logic
inside the compiler to handle them if and when they appear, and we do not go via the symbol table. As an aside,
to make things even crazier and curiouser, the language Fortran allows the user to redefne keywords – yes, even
keywords like “if” and “then” – which means its keywords are not reserved words!Scanner errors
§3.2
17
Very important: Youmustuse theleprintfto display your messages. Tis function
will prefx the line number to your error message. In the following, the line number part of the
message is not shown.
3.2.1 Illegal characters
If any illegal character appears in the input, the following error messagemustbe displayed:
illegal character '⟨ character ⟩' (ASCII #⟨ ascii code ⟩)
Here⟨ character ⟩mustbe the actual character that is illegal, and⟨ ascii code ⟩mustbe its
ascii code. Te token positionmustbe set to the exact position of the ofending character.
3.2.2 Numbers that are too large
If any number is too large to handle, the following error messagemustbe displayed:
number too large
Te token positionmustbe set to the frst digit of the ofending number token.
3.2.3 Identifers that are too long
If any identifer is longer than 32 characters, the following error messagemustbe displayed:
identifier too long
Te token positionmustbe set to the frst digit of the ofending number token.
3.2.4 An illegal escape code found in a string
If an escape not listed in table 3.1 appears in a string, the following error messagemustbe
displayed:
illegal escape code '⟨ escape code ⟩' in string
Here, ⟨ escape code ⟩ mustbe the illegal escape code, including initial backslash . Te token
positionmustbe set to the backslash of the ofending escape code.
3.2.5 Non-printable characters appear in a string
If a string contains a non-printable character, the following error messagemustbe displayed:
non-printable character (ASCII #⟨ ascii code ⟩) in string
Here, ⟨ ascii code ⟩ is the ascii code of the ofending character. Note that youmust not
print the character itself. Te token positionmustbe set to the exact position of the ofending
character. Tis error takes precedence over ¶3.2.6.18
ebnf and scanner
§3.3
3.2.6 A string was not closed
If a string has no double quote that properly closes it, the following error messagemustbe
displayed:
string not closed
Remember, it is possible to escape a double quote character inside a string. Te token position
mustbe set to the double quote character that starts the ofending string.
3.2.7 A comment was not closed
If a comment was not closed or properly nested inside another, the following error message
mustbe produced:
comment not closed
Te token position must be set to the lef (opening) parenthesis that start the ofending
comment. When comments are nested, the innermost comment that is not closedmustbe
indicated as the ofending comment.
3.3
E X E R C I S E S
According to the specifcation, what is the correct error message when a
string literal occupies multiple lines?
E X E R C I S E 3 . 1
E X E R C I S E 3 . 2
What is the diference between areserved wordand akeyword?
How would you go about adding foating-point numbers to the EBNF? Is
it possible to check, in the scanner, the magnitude of foating-point literals? Even if it is, do
you think it would be worth the bother?
E X E R C I S E 3 . 3
Although ampl supportsblock comments, many other programming
and input languages also supportline comments, where everything from a designated line
comment character or sequence to the end of the current line is ignored. For example, Python
and bash have“#”, LT X has A E
“%”, and nasm has“;”, whereas C (some standards), C++, and
E X E R C I S E 3 . 4
Java have the sequence “//” . What would be involved in adding a line comment character
to ampl, especially considering that (1) multiple, consecutive lines could contain only line
comments, and that (2) line comments could be interleaved with block comments?
For the really curious and studious: Have a look on the Internet for how
industrial-strength compilers handle the diference in specifcation between regular tokens
and context-free productions. In particular, search for “compiler compilers” (sic.), “scanner
generators”, and “parser generators”.
E X E R C I S E 3 . 5Exercises
§3.3
E X E R C I S E 3 . 6
19
Regular languages can be specifed concisely byregular expressions. *
Tey are everywhere: Sure, we use them to specify languages and build compilers . . . but
chances are if you type your cell, credit card, or ID number in on some app or website, a
regular expression (“regex”) engine will test whether or not what you have typed in is valid.
Also, if you use the “/” command in Vim’s command mode, you can specify what you are
†
looking for by a regular expression.
Regular expressions are defned over analphabet(informally, a character set), and support
three basic operations, namely (in order of highest to lowest precedence), (1) the(Kleene)
star, written as a postfx“*”, indicating zero or more occurrences of a character or character
sequence, (2)concatenation, written by juxtaposing characters or character sequences, which
means one character or character sequence is followed by another, and (3)alternation, written
as an infx “|” , which means the character or character sequence either before or afer the
operator can be matched. For example, the regular expression ab*|c means that strings
starting with an a and followed by any number (including zero) of b ’s, or just the string “c”
can be matched. In set notation, the language defned thus is{ a,ab,abb, . . . ,c} .
‡
Most regex engines understand other syntax as well: We can override precedence with
parentheses in the usual way—for example,a(b*|c)will match{ a,ab,abb, . . . ,ac} , and yes,
we can distribute concatenation over alternation, so that this regex is equivalent toab*|ac—
the postfx operator “+” means one or more occurrences, the postfx operator “?” means
zero or one occurrence, and a sequence of characters between brackets means that any one
of those characters must match. For example, to match a single digit, we can use the regex
[0123456789]. Fortunately, we can specify a range inside abracket expression, so that we
can abbreviate [0123456789] as simply [0-9]. Now, we can specify an ampl ⟨ num ⟩ as
[0-9][0-9]*, or more succinctly, as[0-9]+. Similarly, we can use multiple ranges inside a
bracket expression to specify an ampl⟨ id ⟩as[a-zA-Z_][a-zA-Z0-9_]*.
Now, consider the following defnition from K&R [11, p. 194]:
A foating[-point] constant consists of an integer part, a decimal point, a fraction
part, aneorE, an optionally signed integer exponent and an optional type sufx,
one of f, F,l, orL. Te integer and fraction parts both consist of a sequence
of digits. Either the integer part of the fraction part (not both) may be missing;
either the decimal point or theeand the exponent (not both) may be missing.
Using the regex notation defned above, give a regex that will match a foating-point constant
in C. And, yes, there is more than one correct way of doing it.
graduate students, I am working on virtual machines for matching regexes (in C, what else?) and looking at the
V8 JavaScript regex engine for inspiration), and we are also working to discover how regexes are used “in the
wild”, by real programmers. Tis involves studying techniques such asmemoisationandcaching, reasoning over
computational models likealternating fnite automata, and from a graph-theoretic and algebraic point of view,
trying to dragsemiringsinto the picture. If I appear unempowered and of more dense verbiage than usual some
mornings, you now know why. But I love to discuss these things; if you’re interested, let me know.C H A P T E R 4
Parsing a m p l
Appendix A contains a lengthy (in second-year terms) description of writing a recursive-
descent parser for anLL( 1) language. Terefore, if you did not follow everything during the
lectures, this should be your frst stop. Also, the slides contain an “animation” of the syntax
recognition process.
Before starting on the parser, frst make sure that your scanner is working perfectly. Use
the test cases released on SUNLearn afer the frst submission.
4.1
O U R A P P R O A C H T O P A R S I N G
Our approach to writing the parser will be simple: Turn the EBNF into a program by the
(almost mechanical) application of the “rules” in table a.1. In the last two parts of the project,
we shall add additional functionality to the parser, so as to accomplish semantic analysis and
code generation.
4.1.1 Parsing functions corresponding to EBNF productions
Te idea is to perform an implicit walk of the parse tree for the ampl source code being
compiled—in other words, we do not build an explicit parse tree. Youmuststart by writing
avoidfunction with avoidparameter list for each production in the EBNF that (1) is not
handled by the scanner, unless (2) its right-hand side consists of terminals only, except (3) in
the case where a terminals-only production appears in the right-hand side of more than one
(other) production. According to these rules, the productions for ⟨ type ⟩ and ⟨ simple ⟩ , for
example, have functions, but those of ⟨ addop ⟩ and ⟨ mulop ⟩ do not. Tere is a global token
variable, calledtoken, and it is the address of this variable thatmustbe passed for each call
toget_token.
Youmustname each functionparse_⟨ production ⟩, where⟨ production ⟩is the name of
the production parsed by that function. For example,⟨ program ⟩will be parsed by a function
calledparse_program. Since a parsing function is not part of the API exported to some
20Debugging
§4.2
21
other program, it is a good idea to limit your function documentation comment to a text-only
version of the production being parsed. Tis should serve as a quick sanity check when
actually writing the function.
Te golden rule for recursive-descent parsing with one lookahead symbol is this:
Ensure that each parsing function leaves the next, not-yet-handled token ready for
the function that follows, that is, the function that will be called next.
Tis is why, in the parser skeleton, there is a call toget_tokenbeforeparse_programis
called. If you follow the rules mechanically, then there will be very small chance of error. Act *
correctly locally (inside a parsing function), and the global results will simply follow as if by
magic.
Do not try to “optimise” your code by skipping testing for tokens, especially if you
believe that you are testing for the same token twice. For example, inside the function for
⟨ statement ⟩ , you have to write a switch to determine which nonterminal to follow, and
accordingly, which function to call. If the token is, say,TOK_IF, do not callget_tokenbefore
the call toparse_if, but leave the consumption ofTOK_IFto the latter. (Tis is not an ad hoc
arrangement, but exactly in accordance with table a.1.) Remember, there is a big diference
between testing for a token and actually consuming that token.
4.1.2 Expecting terminals
Te parser skeleton has a helper function calledexpect, which tests whether the current
token matches an expected token, and if there is match, then callsget_token. Youshould
use this function every time you need to consume a terminal according to the EBNF.
Tere is also a function calledexpect_idthat matches againstTOK_IDand duplicates
the token lexeme afer a successful match. However, this function only becomes necessary
once we do type checking with the symbol table, when the compiler must actually start
“remembering” and doing something with identifers. If you do decide to use expect_id
while writing your parser, and you free (deallocate) the memory allocated by this function,
remember to reconsider your deallocation strategy once you add the symbol table; otherwise,
you might very well free a pointer in the parser that must still be valid in the symbol table.
4.2
D E B U G G I N G
Te parser skeleton contains three macros, conditionally wrapping a function each, for re-
porting debugging status: DBG_start,DBG_info, andDBG_endshouldbe used to display
information messages at the start, during, and at the end of a parsing function. Tese func-
tions take the same parameters asprintf, but write unbufered output to the standard output
stream, just like for the standard error stream. For an example of use, look at the parser
skeleton.22
parsing ampl
§4.3
Te functions underlying these macros are only compiled when the DEBUG_PARSER
constant is defned, say, via the makefle. Tis means that you may leave calls to these functions
in your code afer you are done debugging—but then make sure that DEBUG_PARSER is
undefned in the makefle, and these functions will not produce any output. Yet, they are still
available if some other bugs crop up.
It may seem like a chore to add these functions when you are frst writing your code.
However, you will appreciate them when you have to debug, as then they can be “turned on
and of” by a fick of theDEBUG_PARSERfag.
4.3
P A R S E R E R R O R S
All parser errorsmustbe reported via theabort_cfunction. It takes an error code from
theErrorenumerated type, defned inerrmsg.h, and thereafer a variable argument list
may follow. Use the skeleton code in abort_c to fnish the function. Note that the case
forERR_EXPECThas unique formatting; other cases will be diferent fromERR_EXPECT, but
similar to one another.
Tere is also anabort_cpfunction, which takes an additional parameter, a pointer to a
SourcePosstructure. Tis parameter overrides the current fle position for error reporting.
It is useful when you must somehow read past an error position to trigger the error. In many
cases, especially while still just parsing (and not checking types yet), it is not necessary to
override the fle position; if your scanner sets positions correctly, it will not be necessary to
useabort_cpduring parsing.
For all of the following, the position reported for an errormustbe
the frst character of the token that was actually found instead of the token that was expected.
If the source fle ends prematurely, which is to say that end-of-fle was reached, the position
of the token foundmustbe the last character in the source fle, typically an eol character.
E R R O R P O S I T I O N
Suppose you run the amplc executable on the source fle test.ampl,
which produces the following error report:
E X A M P L E 4 . 1
amplc: test.ampl:2:16: error: ')' expected, but found identifier
If you open the source fle in Vim, and you enter the command “2G16|” – the last character is
this sequence is the vertical bar or pipe character – the cursor should move to this fle position,
and then must be on top of the frst character of the identifer that was found instead of the
expected closing parenthesis.
Note that tab characters in the source flemustbe counted as one column each by the
scanner. Terefore, it isrecommendedthat you use spaces instead of tabs for indentation
of ampl source code. To keep things easy, this is how our test cases are set up.Parser errors
§4.3
23
4.3.1 Missing type specifer
If a type token, TOK_BOOLEAN or TOK_INTEGER, is missing, the following error message
mustbe displayed:
expected type specifier, but found ⟨ token ⟩
Here,⟨ token ⟩mustbe the string representation, as returned byget_token_string, of the
token found instead of the type token. Tis case is handled separately because there is no
suitable “TOK_TYPE” token available.
4.3.2 Missing statement
If a statement is missing, the following error messagemustbe displayed:
expected statement, but found ⟨ token ⟩
Again,⟨ token ⟩is the string representation of the token actually found. Tis case is handled
separately because diferent kinds of statements are given in the EBNF.
4.3.3 Missing factor
If a factor is missing, the following error messagemustbe displayed:
expected factor, but found ⟨ token ⟩
Yet again,⟨ token ⟩is the string representation of the token actually found. Tis case is handled
separately because diferent kinds of factors are given in the EBNF.
4.3.4 Missing expression or array allocation sequence in assignment
If there is no expression or array allocation sequence aferTOK_GETSin an assignment state-
ment, the following error messagemustbe displayed:
expected expression or array allocation, but found ⟨ token ⟩
Again,⟨ token ⟩is the string representation of the token actually found.
4.3.5 Missing string or expression in output statement
If a string or expression is missing in an output statement, the following error must be
displayed:
expected expression or string, but found ⟨ token ⟩
Again,⟨ token ⟩is the string representation of the token actually found.24
parsing ampl
§4.4
SOURCE:
https://xkcd.com/327/
figure 4.1:Exploits of a mom.
4.3.6 Unexpected terminal found
Tis kind of error is handled by the function expect, and besides the correct use of this
function, there is nothing you have to do.
4.3.7 Unreachable code
Tere is an extra error constant for unreachable code. Itshouldbe used in thedefault
cases of switch statements. If your parser works correctly, the control fow of your compiler
will never reach the point at which that error is triggered. However, if you make a mistake,
and some tokens “disappear” or are repeated, having compilation abort on this error is useful
for debugging.
Especially in a program like a compiler, in which you are parsing input that could be
incorrectly formatted, or even malicious by nature (think of all the wonderful things the
demis and I will dream up), it is a good idea in most cases to haveelseclauses for allif
statements, anddefaultcases for all switches. As a rule of thumb, all grouped parts of EBNF
productions that contain one or more choicesshouldhave a default case or else clause.
All this is to say: Test for everything explicitly. For example, if there are only fve possible
cases in a switch, do not test only for the frst four and handle the ffh as the default case.
Rather, test for all fve cases independently, and keep the default case for the seemingly
impossible. Depending on your skill as programmer, the seemingly impossible might actually
be quite probable. Attackers get up to all manner of evil; it is just good practice to be slightly
paranoid about security exploits; refer to figure 4.1, which will make a whole lot of sense
once you do Computer Science 343.
4.4
E X E R C I S E S
E X E R C I S E 4 . 1
Why do you think I did not write the production for⟨ expr ⟩as follows?
⟨ expr ⟩= ⟨ simple ⟩ ⟨{ relop ⟩ ⟨ simple ⟩ } .
(Note the braces instead of brackets.)§4.4
Exercises
25
Probably the most irritating feature of ampl is how semicolons are used.
So, afer studying the EBNF for ampl, would you consider the semicolon to be astatement
terminatoror astatement separator? Is this the same as for languages like C and Java?
E X E R C I S E 4 . 2
Probably the second-most irritating feature of ampl is the“let”keyword
for variable assignment and array creation. Why did I add this? Reason in terms of frst sets;
see §A.2.
E X E R C I S E 4 . 3
Imagine we want to refactor the EBNF so that the “let” keyword is
dropped. Will the resulting grammar still beLL( 1) ? Will it be easier to handle if we were
allowed two lookahead symbols? And if we want to keep the grammarLL( 1) , how would
other productions change?
E X E R C I S E 4 . 4
E X E R C I S E 4 . 5
Although the⟨ if ⟩and⟨ while ⟩productions end with the“end”keyword,
but the⟨ body ⟩does not . Why do you think this is?
(Tis exercise is for enrichment, only for those interested in language
and compiler design. Also, it is not really an exercise, but more a list of ideas for extra
reading and discussion.) LL( 1) grammars are just the tip of the iceberg. Tey are ofen studed
in undergraduate curricula, because they are relatively easy to understand and create, and
recursive-descent parsers are easy to write by hand. Unfortunately, many courses get stuck on
this topic, and there are so many other approaches out there, each with its own pros and cons
in terms of algorithmic complexity and language classes recognised. Here, then, is a list of
some topics to consider:
(a) LR parsing, and the issue of lef recursion;
(b) LL(∗ ) parsing, as used by the antlr framework;
(c) parsing expression grammars (PEGs) and packrat parsers; and
(d) GLL and GLR parsing.
You are most welcome to discuss these with me at the tutorials. I’ll explain what I can, and
point you to some interesting “researchy” reading. Not that everything is researchy—TreeSitter,
for example, uses GLR parsing, and is all the rage for syntax highlighting in the NeoVim
community.
E X E R C I S E 4 . 6C H A P T E R 5
Semantic analysis
Now that the parser is able to determine whether or not a source program has valid syntax, we
can move on to considersemantics, that is, the meaning of the program. Te fnal meaning
of the program will become apparent once we have the equivalent Java bytecode instructions;
this is done during the fnal part of the project, code generation. But frst, we have to check
whether expressions in the source program make sense. Tis is done by evaluating data type
information and comparing it to information gathered into a symbol table.
5.1
T H E S Y M B O L T A B L E
Asymbol table is a data structure that holds information about identifers in source pro-
grams [1]. Typically, such information includes its lexeme, its type, and its position in storage.
For an identifer that denotes a function or a procedure, this also includes information about
its parameter list and its return type.
Symbol tables can be implemented in a number of ways, but almost intuitively, the most
suitable data structure seems to be a hash table, which associates values with keys. Both
key and value are chosen from predetermined, but arbitrary sets. As such, a hash table is a
logical extension of the concept of an array. But where an array can only index over a dense
set of integral values, hash tables can “index” over an arbitrary set of values—provided that
these values can be turned into integral values by some deterministic (and preferably, rapid)
procedure.
For more information on symbol tables in general, and hash tables in particular, refer
to Sedgewick and Wayne [19]. You should note, however, that we employ the term symbol
table here specifcally with respect to use in a compiler, which is to say, for the component
that stores and retrieves data associated with identifers and some other values and symbols.
In contrast, Sedgewick and Wayne use “symbol table” simply to refer to a data structure that
associates values with keys, like the map data types in Java’s libraries, or the dictionary data
structure built into Python. To distinguish between our idea of a symbol table and that of
Sedgewick and Wayne, in the sequel, we use the termmapfor the latter.
26Te symbol table
§5.1
27
In this project, we implement the symbol table in two separate program units:
1. a general hash table, able to store arbitrary key–value pairs, made possible by the use of
voidpointers; and
2. an ampl-specifc symbol table that wraps around the hash table to implement the
association of identifers with their attributes.
5.1.1 Maps and hashing
AMapallows us to store elements so that they can be retrieved rapidly usingkeys.
One efcient way to implement a map is to use ahash table, a key-indexed, table-based search
mechanism that essentially generalises arrays. Key-indexed search uses keys as array indices
rather than comparing them and depends on the keys being distinct integers falling in the
same range as the table indices. If the keys are distinct small integers, for example, we may
store an item with key k at index k in an array. Given the key k , inserting, deleting from, and
searching the array takes O ( 1) time.
If we do not happen to have keys with such fortuitous properties, we usehashing. We still
use an array, but one with length proportional to the number of entries actually stored, not to
the total number of keys available in the key space. From any given key, we then compute the
array index at which an entry will be stored. (When we do hashing, we usually refer to the
underlying array as a “table”.) Under some fairly general and generous conditions, we still
have O ( 1) time performance on average for insertion, search, and deletion (if the latter is
implemented).
Hashing algorithms consist of two separate parts. Te frst step is to compute a hash
function that transforms the key into a table address. Ideally, diferent keys would map
to diferent table entries, but in practice, diferent keys ofen map to the same table entry,
especially if the table size is small. Tus, the second step in a hashing algorithm is to choose a
collision-resolutionprocess that deals with such colliding keys. Open addressingschemes,
for example, attempt to fnd a diferent open slot in the table itself. For this project, however,
we will useseparate chainingfor collision resolution, where each entry in the table is a linked
list , and afer hashing a key, a linear search of the list at the appropriate index may locate the *
M A P S
associated value.
What remains is to consider the hash function, which transforms
keys into table addresses. If we have a table that stores N items, we need a function that
transforms a key into an integer in the range[ 0, N −1] . Our ideal hash function is one that
H A S H F U N C T I O N S
uses red–black trees. Deciding which ancillary data structure to use is a an exercise that is quite subtle for general
libraries, because we have to consider the use cases carefully. In particular, if the map is used more or less
immutably afer an initial batch insertion of key–value pairs, our practical cost accounting would be quite diferent
from the situation where insertions, updates, and deletions occur frequently throughout the lifetime of the data
structure.28
semantic analysis
§5.1
is easy to compute and approximates a uniformly random function: For each input, every
output in[ 0, N −1] should be, in some sense, equally likely.
Te hash function depends on the key type. Integer keys, for example, might be mapped
to the required range simply by applying the remainder (also known as the modulo) operator.
So, for an integer k , we compute k mod N to get an index into the table. However, we have
to bear in mind that the C modulo operator“%”is defned slightly diferently from what we
might expect from pure mathematics: You have to be careful with negative operands.
Also, it would seem that this modular mapping is sensitive to the table sizeN. It is not
unreasonable to assume that there might be some pattern to the keys. If N is a composite
integer, and the keys are multiples of a factor of N, we get many collisions, which we call
clustering. For example, for N =24, any multiple of2,3,4,6,8,12, and24creates clusters—
even integers larger than23would “wrap around”. So, it would seem that a prime N is a good
value for the table size. Unfortunately, convenience rarely is gratis. Primes are intimately
linked to the problem of factorisation, which is hard and therefore the basis of cryptographic *
systems in common use. † If we select the table size once, and then stick to it, no problem.
However, as we shall see later, it is convenient if we could let the table size grow with the
number of items we store in the table. But then we would have to fnd a prime number for the
new table size when we are resizing, which implies that we either have to store a sequence of
prime tables sizes, or that we have a way of rapidly computing a new prime table size.
‡
5.1.2 Implementing a general hash table
Although C provides neither generics nor templates, we are are able to write a general hash
table implementation – one that may have keys and values of arbitrary types – because C
allowsvoidpointers. Since the C compiler does not guarantee anything about the types to
which these pointers point – no run-time type checks are performed – and since all pointer
values (that is, addresses) on a particular architecture is of the same size, we can cast to and
fromvoidpointers with impunity. In a sense, they are assignment compatible with other
hard”—here, “hard” means “difcult”, or for a student, “difcult to understand.” A second meaning could be that it
“requires a lot of efort”, and it is this meaning we usually use when describing computational problems as hard.
Calling a computational problem hard means that the resources required by the known algorithmic solutions
do not scale well over the input size. Tat is, for large inputs, these solutions would either run very slowly (as in
longer than the age of the known universe in extreme cases) or have very large memory space requirements (as in
more atoms than in the known universe in extreme cases), or both. True, many of these problems are difcult to
understand too, but others, like factorisation, we understand, and solve for small examples, already in primary
school.
Computationally speaking, it is relatively easy to multiply two large primes pandq, but hard to factorise their
product pq .
calculations are fast bit twiddling operations, and the “compression” function is just a bitwise mask with the
respect to the lower-order bits of an object’s hash code. It ofsets the problem of clustering by some other advanced
trickery, including the use of red–black trees.Te symbol table
§5.1
29
pointers in the same way that any Java object reference can be stored in a variable of type
Object.
Suppose a functionproducecreates a new string, and therefore, haschar
* as return type. Suppose also that a function consume uses a string in some way. If we
create a data structure which allows storage and retrieval of arbitrary pointers, its insertion
function, say,put, must have avoid *parameter, and the retrieval function, say,getmust
have avoid *return type. Te functionproducecan now create an array of characters in
some way, and pass this toput, because we may pass achar *value to avoid *parameter.
Similary,consumemay retrieve a pointer viaget, and simply cast the retrievedvoid *to
char *before use.
E X A M P L E 5 . 1
We also employ function pointers. When a hash table is initialised, two functions are *
passed as parameters to the initialisation function: (1) one that can hash keys from our
designated keyspace, and (2) another that can compare two particular keys and that returns−1,
0, or1, depending on whether the frst key is less than, equal to, or greater than, respectively,
the second key. Tese functions must be provided by whatever unit creates and manages the
hash table, because the hash table itself is completely blind to the type of information that it
stores.
5.1.3 The load factor
To measure the occupancy of a map, we use a metric called theload factor,
λ =
n
,
N
(5.1)
where n is the number of entries in the map and N is the table size. When we start out,
becausen = 0, also λ = 0, but as we start adding entries, λ starts growing to1. Once we hit
λ >1, by the Pigeonhole Principle, we are 100% sure that there are collisions in the hash table.
(Actually, owing to how we expect keys to be distributed in the keyspace, we typically start
getting collisions much sooner!)
If there are too many collisions, the performance of the hash table starts to degrade:
Too many lookups result in us having to walk down the chains (linked lists) attached to
eachbucket(position) in the table. Terefore, during initialisation, we take a foating-point
parameter that gives the maximum load factor λ max allowed for that particular hash table.
Once λ exceeds λ max , we rehash to a larger underlying table, and the colliding elements spread
out again, statistically speaking.
Torehasha table, we simply (1) choose a size for the new, larger table, (2) allocate space
for the new table, (3) traverse the old table, and for each element, we (4) compute a new
hash value (with respect to the new table), and (5) insert it into the new table. Ten, we
As a matter of fact, the C standard does not explicitly specify all pointers to be of the same size. But it does specify
that we must be able to assign to and fromvoidpointers without loss of information, and making all pointers the
same size is an easy way to accomplish this.30
semantic analysis
§5.1
release the memory occupied by the old table, which works out just fne, because the existing
entries have been copied to the new table. Although steps 2 and 5 seem to suggest creating an
entirely new structure to replace the old structure, things are actually simpler. When, as in
our implementation, we have access to the underlying nodes in the linked list, in step 5, we
can simply unlink the node we are currently rehashing, and then relink it into the new table.
Te code skeleton for the hash table contains an integer array calleddelta, which gives
k
the diference between2 , where k is an index intodelta, and the largest prime less than2 .
4
For example, fork =4, the entry indeltais3, and we get the largest prime less than2 as
4
2 −3 = 13. When rehashing, use this array to compute a new underlying table size that is
both prime and almost double the previous size.
k
5.1.4 Wrapping a symbol table around a hash table
With the heaving lifing done in the hash table implementation, it is now relatively easy
to create a symbol table by rephrasing, as it were, the symbol table functions as calls to
an underlying hash table. As long as we provide suitable hash and comparison function
implementations, all will be well.
Our symbol table associates alphanumeric identifers with their type properties. So, the
identifers in the ampl source fle being compiled are the keys of the hash table. Tis means
we have to create a hash function capable of hashing alphanumeric strings, and similarly,
a comparison function that can compare alphanumeric strings (in lexicographic order).
Fortunately, the latter already exists in the form of strcmp, so we only have to worry about
the hash function.
5.1.5 The hash function
Now, how do we hash keys that are not integers? A naive approach would be simply to sum
the components—in the case of strings, the numeric values of the constituent characters.
However, consider the fve strings “post” , “pots” , “spot” , “stop” , and “tops” . All fve
consist of the same characters and, under a simple summation scheme, would yield the same
result. A better solution needs to take ordering into account.
An alternative to the simple summation scheme that takes position into account is to view
any structured value type x, sensitive to ordering, as a tuple( x 0 , x 1 , . . . , x j −1 ) of primitive,
atomic types, and then to choose a nonzero constant a ≠1so that we can use as hash function
the expression
x a 0
j −1
j −2
+ x a 1
+ ⋯ x j −2 a + x j −1 ,
(5.2)
which is then used, modulo the table size, as table index. Mathematically speaking, this is
simply a polynomial in a that takes the components( x 0 , x 1 , . . . , x j −1 ) of x as its coefcients.
Such a hash function is, therefore, called apolynomial hash function. By Horner’s Rule, this
polynomial can be written
x j −1 + a ( x j −2 + a ( x j −3 + ⋯ + a x ( 2 + a ( x 1 + ax 0 ) ⋯ )))
,
(5.3)Adding type checking to ampl
§5.2
31
and then computed efciently. [Comparing Eq.(5.2)to Eq.(5.3), can you see why computing
the latter is more efcient than computing the former? Count the number of operations.]
Intuitively, a polynomial hash function uses multiplication by the constant a as a way of
“making room” for each component in a tuple of values while also preserving a characterisation
of the previous components.
Irecommendyou use a variant of the polynomial hash function, where multiplication
by a is replaced by thecyclic shifof partial sums by a fxed number of bits. In C, we can
specify an integral variable to be unsigned, so we do not have to worry about overfow afecting
the sign. Compared to other instructions, bit shifs are extremely fast, but with all the good
properties of the polynomial hash functions, which is why I recommend them. Just remember,
whatever hash function h ( x ) you choose, you still have to use k = h ( x ) mod N as the table
index.
5.2
A D D I N G T Y P E C H E C K I N G T O A M P L
With an operational symbol table, we can now extend the parser to do type checking. Tat
is, we add some statements that check types to those functions already checking syntax. A
compiler does type checking by (1) assigning a type expression to each component of the
source program, and then (2) checking whether these type expressions conform to logical
rules.
5.2.1 Type checking of expressions
Ampl isstrongly typedin that our compiler guarantees the compiled code will run without
type errors. To specify typing for expressions, we fold *
⟨ arglist ⟩and⟨ index ⟩into their parent
productions, afer which we rewrite some other productions in the EBNF as an attributed
BNF grammar (see §B). Ten we update the parse functions for⟨ expr ⟩,⟨ simple ⟩,⟨ term ⟩, and
⟨ factor ⟩to be able to propagate typing information to where it can be handled appropriately.
Tis means we have to add reference parameters for these functions.
Te attributed grammar for ampl is given in table 5.1, which is to be interpreted thus:
Te syntax column gives the BNF of a particular production, and the notation “expr( T 0 )” is
read as “⟨ expr ⟩has the type T 0 ”; the attribute column specifes how types are propagated;
and the context provides additional constraints on the types in question. Further notations
are array , bool , function , and int for arrays, booleans, functions, and integers, respectively,
and the use of italics for a grammar element retrieved as a single token from the scanner. Te
functions base( T ) , fnd( w ) , nparams( w ) , and ptype( w , i ) are described below the table.
Te quick (and easy) way to explain how it works is to consider some examples.
E X A M P L E 5 . 2
Te frst row in the table says that if an expression⟨ expr ⟩is just a simple
pointers exist, and integral types can be freely mixed. Python is strongly typed in that there are few implicit type
conversions; but its typing isdynamicin that typing is enforced only at run time.32
semantic analysis
§5.2
table 5.1:Te attributed grammar for ampl expressions.
syntax
attribute
T 0 0 ← T 11
| simple( T )“=” simple( T )
T 0 ← bool
11
2
2
| simple( T ) “<>” simple( T ) T 0 0 ← bool
| simple( T 11 ) “>=” simple( T 22 ) T 0 0 ← bool
| simple( T 11 ) “>” simple( T 2 ) 2
T 00 ← bool
11
2
2
| simple( T ) “<=” simple( T ) T 0 0 ← bool
| simple( T 11 1 ) “<” simple( T 22 ) 2
T 0 00 ← bool
context
expr( T 00 ) = simple( T 11 )
1
2
2 = int
simple( T 00 ) = term( T 11 )
T 0 0 ← T 11
T 00 ← T 11
T ← T
T ← T
T 0 0 ← T 11
| “-” term( T 11 )
| term( T 1 )“+”
T 2 )
| term( T 1 )“-” simple ′ ( T 2 )
| term( T 11 ) “or” simple ′ ( T 22 )
simple ′ (
0
1
0
term( T 00 ) = factor( T 11 )
1
T 1 = T 2
T 1 1 = T 22
T 1 1 = T 2 2 = int
T 11 = T 22 = int
T 11 = T 22 = int
T 1 1 1 = T 2 2
T 1 1 = int
T = T = int
T = T = int
T 11 = T 22 = bool
1
2
1
2
T 0 0 ← T 11
1 2 0
1 2 0
1
factor( T 00 ) = num
| id idf( T 11 , id )
| “(” expr( T 1 )“)”
| “not” factor 1 ( T 11 )
= bool
| “false”
| “true”
idf( T 00 , id ) = “[” simple( T 11 ) “]”
| “(” expr( T 11 ) param( k 11 ) “)”
2
0
1
1
1
1 2
1 2
1 2
T 00 ← int
T 0 0 ← T 11
T 0 ← T 1
T 0 0 0 ← T 11 1
T 11
T 0 ← bool
T 0 0 0 ← bool
T 0 0 ←base( T 22 )
T 00 ← T 22
| ε
function
| ε
T 0 0 ← T 11
k 00 ←0
T 1 1 = int
∧ T 2 ←fnd( id )
∧ T 2 22 ⊆ array
T 1 1 =ptype( id , 0)
∧ T 2 ←fnd( id )
∧ T 2 2 ⊆ function
∧ k 02 ← k 1 +1
∧ k 0 0 =nparams 1
( id )
T 11 ←fnd 0 ( id ) ∧ T 11 ⊆ /
∧ n ←nparams( id )
∧ T 1 1 =ptype( id , n − k 22 )
base( T ) returns the base type of array type T
fnd( w )
nparams( w )
ptype( w , i ) returns the type of w (if w is in the symbol table)
returns the number of parameters of the function named w
returns the type of parameter i of the function named w , where i ∈ [0, nparams( w )−1]§5.2
Adding type checking to ampl
33
expression ⟨ simple ⟩ – that is, it is not a relational expression – then the type of the ⟨ simple ⟩
becomes the type of the⟨ expr ⟩. For example, forba boolean variable, the expression not b
must still have a type of bool ; and forian integer, the expression i + 1 must still be of
type int .
E X A M P L E 5 . 3
Te second row in the table says that if an expression⟨ expr ⟩is a test for
equality on two simple expressions, then the type of⟨ expr ⟩is bool . But there is also the context
that the type of the simple expressions must be the same: We cannot test whether a boolean
value is equal to an integer. For example, for boolean variablesaandb, and integer variablesi
andk, both the expressions a = b and i = k are valid, and are of type boolean. However,
a = k is invalid, becauseais not of the same type ask.
E X A M P L E 5 . 4
Te ffh row in the table says that if an expression⟨ expr ⟩is a test to see
whether the frst simple expression is greater than the second, then the type of the⟨ expr ⟩is
bool , and the context is that both simple expressions must be of type int . Tis is to say it does
not make sense to test whether one boolean value is greater than another.
Note that table 5.1 essentially specifes typing to be propagated bottom-up, that is, from
the leaves of the parse tree. If you think about it some, it does make intuitive sense: A⟨ num ⟩
is defnitely an integer, and “true” and “false” are defnitely boolean values; also, the type
of an identifer can simply be looked up in the symbol table.
Handling identifers is a bit more involved. For example, we have to ensure that we do not
try to “call” variables or “assign to” functions. Te new⟨ idf ⟩production handles everything
that may follow an⟨ id ⟩in the⟨ factor ⟩production.
E X A M P L E 5 . 5
T e ε -option of *
⟨ idf ⟩says that if an⟨ id ⟩is not followed by an index or an
argument list, it must be a non-array variable. We verify this by checking in the symbol table
that the⟨ id ⟩is neither an array nor a function. Its type is simply what is saved in the symbol
table.
E X A M P L E 5 . 6
Te frst option of ⟨ idf ⟩ says that if ⟨ id ⟩ is followed by an index, then it
must be an array name. Also, the index itself must of type integer. However, the retrieved
array element itself cannot be of an array type. Instead, it must have the base type of the array.
For example, if ais an integer array, thena[0]must be an integer.
Te productions for a function call work similarly, except that we check
not only the return type, but also the types and number of parameters. It looks complicated
when stated formally, but it is just the obvious: Te types of the arguments to a function must
correspond to the types of the parameters of that function, and also, we cannot pass too few
or too many arguments when we call a function.
Do not be troubled by the n − k 2 in the one call toptype. It is just because, in a BNF, we
are essentially handling a production from right to lef. (Because EBNFs are lef-to-right, they
E X A M P L E 5 . 734
semantic analysis
§5.2
is easier to implement as a parser by hand.) If anything else bothers you, look at Exercise 5.2.
5.2.2 Type checking of statements
Each ampl statement, except the singleton “chillax” , must also perform type checking.
For example, variable assignment must happen on an expression of the appropriate type, and
the loop condition expression for a“while”statement must be of type bool . An attributed
grammar can be written for these, but as is evident from our machinations with the⟨ id ⟩s, it
will not be as succinct as table 5.1, and we stick with the following list of rules:
1. Only procedures – subroutines without any return type – may be “called” by the⟨ call ⟩
production. Similarly, a procedure may be not be called from inside an expression.
2. Te type of the⟨ simple ⟩expression following the keyword“array”in⟨ assign ⟩must
be of type int . Tat is, an array must be created with an integer size. Do not worry
about negative array sizes. Once the JVM class fle is generated, the JVM will pick that
up as a run-time error, which is not ideal, but we are going to assume that our ampl
programmers are not completely brainless.
3. Te type of the⟨ expr ⟩that may follow the“=”operator in⟨ assign ⟩must match the type
of the ⟨ id ⟩ . Also, the “array” keyword is only allowed when (a) the ⟨ id ⟩ is an array,
and (b) the⟨ id ⟩is not followed by an index. Te reverse is also true: You cannot assign
a scalar boolean or integer value to an “unindexed” array. Of course, you cannot assign
to a function or a procedure identifer either.
4. Te⟨ expr ⟩following the “if” and “elif” keywordsmustbe of type bool .
5. Te⟨ expr ⟩following the “while” keywordmustbe of type bool .
6. T e ⟨ simple ⟩expression between brackets, denoting indexing into an array,mustbe
of type int .
7. When calling a function or a procedure, the types of the argumentsmustmatch the
types of the parameters exactly.
If you are afraid of missing an error, use the error constants inerrmsg.has guidelines. Use
the wwwd principle: What Would Willem Do (to try and break your code)?
5.2.3 Changing the parser functions
Some parser functions’ prototypes must be changed so that type information can be propagated
properly. In particular, the functions parsing the productions ⟨ expr ⟩ , ⟨ simple ⟩ , ⟨ term ⟩ , and
⟨ factor ⟩must each have a single parameter, a pointer to a value type enumeration, instead of
avoidparameter list. We must also add parameters to the functions parsing ⟨ arglist ⟩ and
⟨ index ⟩: Both need the⟨ id ⟩of the subroutine or array being parsed, and in addition,⟨ arglist ⟩
requires the subroutine’s identifer properties, so that subroutine arguments can be checked.Adding type checking to ampl
§5.2
E X A M P L E 5 . 8
35
Te new signature for the function that parses the⟨ expr ⟩production is as
follows.
void parse_expr(ValType *type)
Note that the return type of voidis unchanged. We can use the pointer parameter as both
input and output to the function.
Since theValTypeparameter is a pointer, space for the data must be reserved in some way.
In typical C, there are two places to allocate space: (1) on the heap, via a call tomalloc, and
(2) on the stack, via a normal variable defnition. In the latter case, a pointer to the variable
can passed by using&, the address operator, on an existing local variable, and this is what we
do .
Te question is now: Where do we defne such variables? A little thought shows that each
of the functions in table 5.1 must pass its type to its caller. So, the caller is the logical place
to defne the variables. A quick look at the EBNF should convince that you that, in many
cases, the callers are those functions discussed in §5.2.2.
E X A M P L E 5 . 9
Te function parsing the⟨ while ⟩production must have a local variable of
typeValType. When the parse function for the loop guard, which is an⟨ expr ⟩production,
is called, the address of thisValTypevariable is passed by prefacing its identifer with the
address operator.
Another glance at the EBNF should also convince you that the functions discussed in
§5.2.2 are not the only ones to have such defnitions. For example, the function parsing⟨ expr ⟩
itself gets at least one type from a call to the function parsing⟨ simple ⟩. So, it too has at least
one defnition of aValTypevariable.
A parse function that originates a type, either by recognising a token of a fxed type (like
the token for “true” ) or by a call to the symbol table, can pass this information back to its
called by assignment on the dereferenced pointer. Tis is why we do not change the return
types of the functions involved.
E X A M P L E 5 . 1 0
Te new signature for the function that parser the⟨ arglist ⟩production
is as follows.
void parse_arglist(char *id, IDPropt *propt);
T e idparameter is error reporting, andproptreceives a pointer to a structure that contains
the number and types of the parameters that the subroutine takes.
5.2.4 Representing the AMPL types
A look atsymboltable.handvaltypes.hmight convince you that something funny is
afoot, becauseValType is actually just an enumeration. However, consider the following
defnition fromvaltypes.h, and pay particular attention to the binary forms (only the least
signifcant nibble) of the integers, given in comments:36
semantic analysis
§5.2
typedef enum valtype {
TYPE_NONE
= 0,
/* 0000 */
TYPE_ARRAY
= 1,
/* 0001 */
TYPE_BOOLEAN = 2,
/* 0010 */
TYPE_INTEGER = 4,
/* 0100 */
TYPE_CALLABLE = 8
/* 1000 */
} ValType;
Do you see that none of the 1-bits clash with another 1-bit in the same column, that is,
0
1
2
3
vertically? Mathematically, the array type is2 , boolean is2 , integer is2 , and callable is2 .
We make the defnitions thus so that we can use aValTypevalue as abit feld[24]. Essentially,
we use a bit position in aValType value as a little on/of switch: We are not interested in
the value of aValType variable as a number, but rather in the individual bits. Having a1
at a particular bit position means the value has the type property corresponding to the bit
position; a0means it does not. Usingmasks[25], we can turn a bit on or of, fip it, or get its
value.
To set the type for an ampl boolean variable, we simply assign the
enumerated valueTYPE_BOOLEANto theValTypefeld in the variable’s identifer properties
structure; also see listing 5.1.
E X A M P L E 5 . 1 1
To set the type for an ampl integer array, we need to use the value
5 10 =101 2 . In the binary form, the least signifcant1corresponds to being an array, and the
most signifcant1to being integer-valued. How to set up theValTypevalue in this way is
discussed in the paragraphs below.
E X A M P L E 5 . 1 2
To set the type for an ampl function that returns a boolean value, we
need to use the value 10 10 =1010 . 2
E X A M P L E 5 . 1 3
A procedure is the only construct in ampl without data type; but it still
needsTYPE_CALLABLEso that we know we can call it.
E X A M P L E 5 . 1 4
In memory, we cannot address bits directly. Te best we can do is address a byte. C does
provide portablebit felds, but we are going to use a more old-school approach, which will
also be useful once we start programming in assembly language. *
B I T M A S K I N G
To isolate a single bit, we use a mask. Te mask for bit k – indexing from
k
right to lef in the usual way for numbers – is simply2 . In binary, a power of two has exactly
one bit set to 1, and the rest are 0.
E X A M P L E 5 . 1 5
Consider an 8-bit byte. A bit mask for the fourth bit, counting from
3
the right, is2 =8 10 =000001000 2 ; we use an exponent of3, because we start indexing at0.
weeks, from Java into C and assembly language.Adding type checking to ampl
§5.2
37
table 5.2:C operators for bit manipulation.
operator description
&
|
^
~
arity
bitwise and
binary
bitwise (inclusive) or
binary
bitwise exclusive or (xor) binary
one’s complement
unary
table 5.3:Truth tables for the C bitwise operators.
a b a & b a | b a ^ b
0
0
1
1
0
1
0
1
0
0
0
1
0
1
1
1
0
1
1
0
~ a
1
1
0
0
Also note our notation here. In C, a integer literal that starts with0is treated as octal, and an
integer literal that starts with0xis treated as hexadecimal. Our notation uses a subscript to
give the base. When we write00001000 2 , it is base-2(binary), and we use leading zeros to
emphasise the8-bit width of the byte, not to indicate that it is in any way octal. In the sequel,
such bit patterns are also written in a monospaced (typewriter) typeface.
Finally, note that bit masks can also mask more than one bit. For bits k and ℓ , where k ≠ ℓ ,
k
ℓ
the bit mask is simply2 +2
. [Why?
Just pause a moment, and write out some examples.]
However, when we build and use bit masks, we usually do not use arithmetic expressions.
Instead, we use the bitwise operators, because that is how we implement them, for doing so is
faster to calculate for a computer, and therefore, the low-level thing to do.
C defnes six operators for bit manipulation: Four bitwise logic
operators, and two additional operators for shifing; they are given in table 5.2. When we say
an operator is “bitwise”, it means the operation defned by the operator is applied to each bit
in the operand(s) in parallel. Te truth tables for the bitwise operators are given as table 5.3.
B I T W I S E O P E R AT O R S
E X A M P L E 5 . 1 6 Consider two char variables c1 and c2 containing, respectively, the
values28 10 =00011100 2 and56 10 =00111000 2 . Tenc1&c2=00011000 2 =24 10 ,c1|c2=
00111100 2 = 60 10 , c1 ^ c2 = 00100100 2 = 36 10 , and ~c1 = 11100011 2 = −21 10 . To
understand the last result, remember thatcharis equivalent tosigned char, and also that
C usestwo’s complement representationfor signed integer representation [22, Appendix A].
Te bitwise operators&and|, and the logic operators&&and||are
closely related, but not equivalent. Consider twocharvariablesb1andb2containing the
E X A M P L E 5 . 1 738
semantic analysis
§5.2
decimal values12and3, respectively. Tenb1 & b2=0, butb1 && b2=1. However, as we will
soon see, when compiling to the JVM, we need only the bitwise versions to implement the
logic operators. In addition, short-circuiting behaviour can be added by using explicit jump
instructions.
Te two shif operators are << for lef shif, and >> for right shif. Both require two
operands: Te frst is the variable to be shifed, and the second is the integral number of
positions by which the frst operand is shifed. Lef shif always flls the blanks on the right
with zeros. Right shif of an unsigned quantity is a logical shif, where the blanks on the lef
are flled with zeros. Right shif of a signed quantity is an arithmetic shif, where the blanks
on the lef are flled with sign bit (the original most signifcant bit) of the frst operand. An
important consequence of this is that lef shifs are equivalent to multiplication by a power of
two, and right shifs are equivalent to integer division by a power of two, even when the frst
operand is negative.
E X A M P L E 5 . 1 8 Consider two char variables c1 and c2 containing, respectively, the
values 13 10 = 00001101 2 and −16 10 = 11110000 2 . Ten c1 «2 = 00110100 2 = 52 10 , and
c2 »2=11111100 2 = −4 10 . Remember, we use two’s complement arithmetic. Terefore, to
negate a number, calculate one’s complement and add1. To confrm the bit pattern11111100
is really −4, calculate~11111100 2 +1, which yields 00000011 2 +1 = 00000100 2 = 4 10 =
−(−4) 10 .
k
Assume we are working with a bit feldband a bit mask2 . Since,
k
for a bit b, b &1 = b and b &0 = 0, computingb&2 results
in “masking of” all bits in b,
except thekth bit, which is lef unchanged. Sinceb|1 = 1and b |0 = b, computingb|2
results in “masking on” the kth bit, regardless of whether it it was1before, and leaving the
U S I N G B I T F I E L D S
k
k
other bits unchanged. Since b ^1=~ b and b ^0= b , the efect ofb^2 is “fipping” the k th
bit, and leaving the other bits unchanged.
Setting up the bit mask appropriately, we can isolate more than one bit at the same time.
Also, bitwise and logical operators can be freely mixed— but we have to keep the operator
precedence table in mind .
To check whether aValTypevariabletis boolean array in ampl, the
following C expression must be true:
E X A M P L E 5 . 1 9
t & (TYPE_BOOLEAN | TYPE_ARRAY) == (TYPE_BOOLEAN | TYPE_ARRAY)
Tis is actually tantamount to testing a whether boolean variable, say,is_valid, is true by
writingis_valid == TRUEinstead of just writingis_valid. Terefore, the type expression
above should just be tested ast & (TYPE_BOOLEAN | TYPE_ARRAY).
Te flevaltypes.hcontains a number of type querying and setting macros, which you
must complete and use when adding type checking to the parser.§5.2
Adding type checking to ampl
39
Listing 5.1:Te defnition of the identifer properties type structure.
1
2
3
4
5
6
7
8
9
10
typedef struct {
ValType type; /* variable or subroutine return type
*/
union {
unsigned int offset; /* locals array variable offset */
struct {
/* for subroutines :
*/
unsigned int nparams ; /* number of parameters
*/
ValType
*params ; /* parameter types array */
};
};
} IDPropt ;
5.2.5 Handling identifers
We handle identifers by looking up their properties in the symbol table. Te lookup returns
the property structure given in listing 5.1.
1. Tetypefeld stores the type of the identifer.
2. T e offsetfeld is used during code generation, when we place a variable at a particular
index in the local variables array of a method frame in the JVM. For now, sufce it so say
that when an identifer is inserted, a global counter in the symbol table is incremented,
and this counter is used as ofset value.
3. T e nparams feld stores the number of parameters if the associated identifer is a
function or a procedure.
4. If the associated identifer is a function or procedure, theparamfeld must store an
array of the parameter types.
Also, note the following about how theIDPropttype is structured.
• Te outerstructhas no tag. It is unnecessary, becausestructdefnition sits inside a
type defnition.
• Also, neither the union, nor the innerstructhas a tag. Because the feld names are
unique, the compiler will simply bind them to the outerstruct.
• An identifer names either a variable or a subroutine. Only variables go into the local
variables array of a method frame, and only subroutines have parameters. Terefore,
to save space, we use a union for the felds that are unique to either variables or sub-
routines. If we suppose thatproptis a variable that points to anIDProptstructure,
then we can always access propt->type, but for the same propt address, only ei-
therpropt->offset, orpropt->nparams andpropt->params. Trying to access,
say, propt->offset andpropt->params for the same propt address will lead to
undefned behaviour. Which felds can be accessed are determined by thetypefeld.40
semantic analysis
§5.2
We must distinguish between using an array in a scalar context,
or referring to the array itself. For a reference to an entire array, the only operations allowed
are assigning an existing array to an array variable and the array creation option in the⟨ assign ⟩
production. All other statements must operate on an array element, which means that the
array name must be indexed. In particular, it is impossible in ampl to read into or write
out an array without treating each element separately by index. Note that assignment of an
unindexed array (reference) to a variable will eventually create an alias in the JVM.
A R R AY S A N D I N D I C E S
To declare and initialise an integer array of size10, the following ampl
code should be used:
E X A M P L E 5 . 2 0
int array nums;
let nums = array 10;
(Since the last statement ends on a semicolon, at least one other statement should follow.) Just
like Java, ampl allows the dynamic creation of arrays, which is to say, the⟨ simple ⟩expression
afer the “array” keyword can be an integer expression, that value of which is computed
at runtime. Before an array can be used – accessed by index for value insertion or retrieval
– it must frst be initialised, but this is a kind of error our compiler cannot pick up without
resorting to additional static analysis, which we do not do.
Procedure callsmustonly originate from
the⟨ call ⟩production, whereas functionsmustonly be called from the⟨ factor ⟩production.
A proceduremusthave its return type set toTYPE_CALLABLE. A function called from factor
mustmatch the type of the calling context. For example, if a function call appears inside an
integer expression, it must return a value of type int .
As is the case with other strongly typed languages, the types of the actual parameter list
(the arguments passed to subroutine)mustmatch the formal parameter types entered into
the symbol table when a function is defned. Youmustalso check that neither too many nor
too few arguments are specifed when a function or procedure is called.
F U N C T I O N A N D P R O C E D U R E C A L L S
As always, common sense applies. If, say, the functioninc has been
declared to take one integer parameter and to return this value incremented by one, for
example, the ampl compiler must not allow the factor “not inc(1)”.
E X A M P L E 5 . 2 1
Function and procedure defnitions
deserve a bit more attention and, indeed, necessitates the last change to the signatures of parse
functions. Te function parsing the⟨ type ⟩production must now become:
F U N C T I O N A N D P R O C E D U R E D E F I N I T I O N S
void parse_type(ValType *type);
Additionally, this function must be updated to use the bit manipulations of §5.2.4 to build up
the appropriate type. Te pointer parametertypeis used for returning the calculated type to
the function caller.§5.2
Adding type checking to ampl
41
Listing 5.2:Te defnition of the parameter type structure.
1
2
3
4
5
6
7
8
typedef struct variable_s Variable ;
struct variable_s {
char
*id;
/* variable identifier
ValType
type; /* variable type
SourcePos pos; /* variable position in the source */
Variable *next; /* pointer to the next variable
*
* in the list
};
*/
*/
*/
Type information, however, is only one part of the picture. Te parse functions for
⟨ subdef ⟩and⟨ vardef ⟩, which need to callparse_type, must associate an identifer with its
type, its position in the source fle, and the types of its parameters. TeVariablestructure
of listing 5.2 must be used for this purpose. *
Te presence of thenextfeld suggests thatVariablecan be used as a node in a linked
list. We use this property to build the parameter sequence inparse_subdef. However, these
sequences require more care: To be able to create a dynamically-allocated array ofValTypes
for theparamsfeld of theIDProptstructure in listing 5.1, we must know the number
of parameters. Tis implies frst parsing all parameters, and while doing so, linking all the
variable structures for the parameters into one linked list . Te linked list is necessary, because
(1) we need to read the entire parameter list from the source fle before we can allocate space
for theparamsarray, and (2) since our symbol table does not support removal or updates, we
cannot add something provisionally. Once all parameters have been parsed, and once we have
determined whether this defnition is for a function or a procedure, we can allocate space
to theparamfeld of theIDProptstructure and turn the list of parameters into an array of
parameter types, associated with the function name.
Whereas a procedure has no return type – it only has
TYPE_CALLABLEto show that it can be called – a function does have a return type. Te last
bit of detail to consider then is how a subroutine’s local context is handled. Tat is, how are
U S E O F T H E S Y M B O L TA B L E
the subroutine’s parameters and local variables kept separate from the main program?
Te idea is then to have two diferent symbol tables:
†
1. Te global one is initialised when parsing starts, and stores information about the
main program variables, as well as about all subroutine signatures and return types (for
functions).
use the same structure for handling variable sequences in function defnitions and variable defnition sections.
In particular, the semantics of nested subroutines would require us to compute the closure of the subroutine,
which needs references to state of the subroutine’s lexical context, that is, the subroutines in which it is nested.42
semantic analysis
§5.3
2. When a subroutine is being parsed, we need another local symbol table that stores the
subroutine’s parameter, local variable, and (for functions) return information as well.
T e open_subroutinesymbol table functionmust
1. install the subroutine name – and associated properties, which include the array of
parameter types, but not their identifers – in the top-level symbol table, afer which
2. it saves the top-level symbol table, and
3. creates a new local table wherein the subroutine parameters and local variables are
stored.
A bit of thought will show that the identifers for subroutine parameters and local variables
have no meaning outside of the subroutine. Indeed, parameters are just local variables that
are initialised to the values passed when the subroutine is called. (When parsing a subroutine
call, we only want the parameter types to match.)
Terefore, whereas the subroutine name itself goes into the top-level symbol table, the
parameter and local variables names go into the local subroutine symbol table. Tis also
implies that thefind_namefunction, should a subroutine symbol table currently be active,
must check for subroutine (but not variable) identifers in the top-level table as well. If this
were not done, recursion, for example, will not be possible. Tere are no global variables (akin
to felds in a Java class), and we have to make certain that a local variable name cannot hide a
subroutine name. However, subroutine names are global, and are visible in the second lookup
step.
Once we are done parsing the defnition for a particular subroutine, we no longer need the
local symbol table: Te subroutine identifer lives in the top-level table anyway, and should
we keep the local table around, it would in efect add to the global namespace, and we would
not be able to have variables of the same name in diferent subroutines. Terefore, once we
are done with the local table, we simply remove it and its information, and then we “restore”
the saved top-level symbol table.
Te hash table implementationmust notunder any
circumstances terminate the program. Te hash tablemaylet the symbol table know that
something bad happened, but it is up to the symbol table and the parser to terminate on an
error condition. If this requirement seems harsh, think of it this way: How would you like it if
something bad happened in Java’sHashMap, but instead of letting you know via an exception,
it just decided to terminate the JVM with a call tosystem.exit?
A V E R Y I M P O R TA N T N OT E
5.3
T Y P E C H E C K I N G E R R O R S
Te functionchktypes, defned away in the original parser skeleton,mustbe used for all
simple type checks. In addition to the type found and the expected type, chktypes also
accepts a variable argument list for error reporting. Tis argument list is simply passed alongType checking errors
§5.3
43
to a call on vsnprintf, so detailed and contextual error messages can be originated by
whichever parser function callschktypes.
5.3.1 Illegal array operation
If there is an illegal array operation, for example, using an unindexed array variable in an
expression, the following error messagemustbe produced:
⟨ op ⟩ is an illegal array operation
Here, ⟨ op ⟩ must be the string representation, as returned byget_token_string, of the
ofending operator, and the error positionmustbe the frst character of its lexeme. Note:
An array identifer may appear unindexed on the right-hand side of an assignment operator
on two conditions: (1) Tere is nothing but this identifer on the right-hand side, and (2) the
identifer on the lef-hand side must refer to a variable of the appropriate array type.
5.3.2 Scalar variable expected
If an attempt is to read directly into an array variable, the following error messagemustbe
produced:
expected scalar variable instead of '⟨ id ⟩'
Here,⟨ id ⟩is the ofending identifer, and the error positionmustbe the frst character of its
lexeme.
5.3.3 Not a function
If a function operation is attempted on a diferent type of identifer, the following errormust
be produced:
'⟨ id ⟩' is not a function
Here,⟨ id ⟩refers to the ofending identifer, and the error positionmustbe the frst character
of its lexeme at that place in the source fle where the function operation is attempted. An
example is when an attempt is made to call, from in the⟨ factor ⟩production, an identifer that
is not a function.
5.3.4 Not a procedure
When an identifer that does not refer to a procedure is encountered in the parsing function
for the⟨ call ⟩production, the following errormustbe reproduced:
'⟨ id ⟩' is not a procedure
Here,⟨ id ⟩refers to the ofending identifer, and the error positionmustbe the frst character
of its lexeme at that place in the source fle where the procedure call is attempted.44
semantic analysis
§5.3
5.3.5 Not a variable
When an assignment or input operation is attempted on an identifer that is not a (scalar)
variable, the following error must be produced:
'⟨ id ⟩' is not a variable
Here,⟨ id ⟩refers to the ofending identifer, and the error positionmustbe the frst character
of its lexeme at that place in the source fle where the operation is attempted.
5.3.6 Not an array
When an array operation, like indexing, is attempted on an identifer that is not an array, the
following errormustbe produced:
'⟨ id ⟩' is not an array
Here,⟨ id ⟩refers to the ofending identifer, and the error positionmustbe the frst character
of its lexeme at that place in the source fle where the array operation is attempted.
5.3.7 Too few arguments for a function or procedure call
When a function or procedure is called with too few arguments, the following error message
mustbe produced:
too few arguments for call to '⟨ name ⟩'
Here,⟨ name ⟩is the identifer that is the name of the function or procedure for which too few
arguments were provided. Te error positionmustbe the frst character of the frst token
that is read from the scanner afer the available argument tokens have been exhausted.
5.3.8 Too many arguments for a function or procedure call
When a function or procedure is called with too many arguments, the following error message
mustbe produced:
too many arguments for call to '⟨ name ⟩'
Here, ⟨ name ⟩ is the identifer that is the name of the function or procedure for which too
many arguments were provided. Te error positionmustbe the comma token that appears
afer the last allowed parameter was read.
5.3.9 Procedures must not have return expressions
If an“return”statement inside a procedure is followed by an expression to be evaluated for
return to the caller, the following error messagemustbe displayed:
a return expression is not allowed for a procedure
Te error positionmustbe the frst character of the ofending expression.Type checking errors
§5.3
45
5.3.10 Functions must have leave expressions
If an“return”statement inside a function is not followed by an expression to be evaluated
for return to the caller, the following error messagemustbe displayed:
missing return expression for a function
Te error positionmustbe the frst character of“return”lexeme that is not followed by a
required expression.
5.3.11 Unknown identifer
If any identifer has not been defned, the following errormustbe produced:
unknown identifier '⟨ id ⟩'
Here,⟨ id ⟩refers to the identifer that was not defned, and the error positionmustbe the frst
character of the ofending identifer. Note that this error has precedence above other the other
“not a(n)” errors. Tat is to say, if the identifer in question is written as part of a function call,
but the identifer is not in the symbol table, itmustbe reported as an unknown identifer,
and not as “not a function”.
5.3.12 Multiple defnitions for an identifer
If an attempt is made to declare the same identifer in the same context again, the following
errormustbe produced:
multiple definition of '⟨ id ⟩'
Here,⟨ id ⟩is the ofending identifer, and the error positionmustbe the frst character of this
identifer where it appears for the second time. Note that parameter or variable namemust
not“hide” the name of any previously defned function or procedure, including the name of
the function or procedure currently being parsed .
5.3.13 Type mismatch
Te function chktypes shouldbe used to fag a mismatch between what was expected
and what actually resulted from the parse. Itshouldalso be used to enfore type rules for
a particular operator; for example, the operator “and” must have two type bool operands.
If any context can result in more than one error, the more specifc error from the previous
subsectionsmustbe preferred. In general, using the following order when checking type
should deliver a suitable error message:
1. check for any errors on the identifer, including undefned identifers, using unindexed
arrays in a scalar context, and trying to assign to function or procedure names;46
semantic analysis
§5.3
2. check whether an operator has the correct type of operands, for example, addition must
have operands of type int ; and then
3. check that both operands have the same type.
Te last two items have diferent efects where an operator is allowed for both bool and int
values.
Ifchktypesfags a mismatch, the following error messagemustbe produced:
incompatible types (expected ⟨ expected ⟩, found ⟨ found ⟩) ⟨ detail ⟩
Here, ⟨ found ⟩ is the type that resulted from the parse, and ⟨ expected ⟩ is the type that is
expected by the context. Te⟨ detail ⟩message and error position for each context are given in
the paragraphs that follow.
A R R AY I N D I C E S
An array indexmustbe of type int . If it is not, the⟨ detail ⟩message
mustbe:
for array index of '⟨ array id ⟩'
Te error positionmustbe the frst character of the⟨ simple ⟩expression in the bracket pair
that follows⟨ array id ⟩.
Te size given when an array is createdmustbe of type int .
If it is not, the⟨ detail ⟩messagemustbe:
A R R AY C R E AT I O N S I Z E
for array size of '⟨ array id ⟩'
Te error position must be the frst character of the ⟨ simple ⟩ expression that follows the
“array”keyword for creating⟨ array id ⟩. Note that we cannot conclusively determine whether
the size is nonnegative. Negative array size is a run-time error that is handled by the Java
virtual machine, and not by the ampl compiler.
For an assignment that is not an array creation statement, the type of the
⟨ id ⟩to the lef of the“=”operatormustmatch that of the expression to the right of“=”. If it
does not, the⟨ detail ⟩messagemustbe:
A S S I G N M E N T
for assignment to '⟨ id ⟩'
Te error positionmustbe the frst non-space character that follows the “:=” operator.
If an attempt is made to “assign” a new array to an indexed array variable (on the lef-hand
side of “=”), the⟨ detail ⟩messagemustbe:
for allocation to indexed array '⟨ array id ⟩'
Here, ⟨ array id ⟩ must be the array identifer, and the error position must be the frst
character of the “array” lexeme.Type checking errors
§5.3
47
In a function, the type of the expression afer an “return”
statementmustmatch the return type of the function. If it does not, the ⟨ detail ⟩ message
mustbe:
R E T U R N S TAT E M E N T S
for 'return' statement
Any argument (alternatively, actual parameter) to the function or proce-
dure⟨ name ⟩mustmatch the type of the parameter (alternatively, formal parameter) at the
corresponding position in the function or procedure defnition. If it does not, the ⟨ detail ⟩
messagemustbe:
PA R A M E T E R S
for argument ⟨ parameter position ⟩ of call to '⟨ id ⟩'
Here, ⟨ parameter position ⟩ must be the integral index of the ofending parameter in the
function or procedure parameter list. In addition, the⟨ parameter position ⟩mustbe reported
counting from ofset1, not ofset0. Te error position must be the frst character of the
ofending argument.
Te guard condition expression for anyif
clause, elif clause, or while statement must be of type bool . It if it is not, the ⟨ detail ⟩
messagemustbe:
G U A R D C O N D I T I O N E X P R E S S I O N S
for '⟨ context ⟩' guard
Here,⟨ context ⟩mustbe either“if”,“elif”, or“while”(without quotes) as determined by
the context. Te error positionmustbe the frst character of the guard expression.
A relational operator that tests ordermusthave operands of
type int ;“and”and“or”take operands of type bool ; and all arithmetic operatorsmusthave
operands of type int . If a particular operator does not, the⟨ detail ⟩messagemustbe:
B I N A R Y O P E R AT O R S
for operator ⟨ op ⟩
Here, ⟨ op ⟩ must be the string representation, as returned byget_token_string, of the
operator. Te error positionmustbe the frst character of the operator.
Whatever the binary operator, its operand types must match. If they do not, use the
⟨ detail ⟩message of this subsection.
An important diference between error reporting for binary operators and for assignment
is that for a binary operator, the error position is at the binary operator; for assignment, it is
on the right-hand side of the assignment operator.48
semantic analysis
§5.4
Te unary minus must be followed by an expression of type int . If it does
not, the⟨ detail ⟩messagemustbe:
U N A R Y M I N U S
for unary minus
Te error positionmustbe the frst character of the ofending expression. Note that if the
expression is an array, itmust, rather, be handled as an illegal array operation with⟨ token ⟩
set to “unary minus” (without quotes); see §5.3.1.
Te factor that follows the“not”keywordmustbe of type bool . If it is not,
the⟨ detail ⟩messagemustbe:
N E G AT I O N
for 'not'
Te error positionmustbe the frst character of the ofending factor.
5.4
E X E R C I S E S
Now that we have type checking enabled, you should fgure out if ampl
allows “normal” rules of precedence for the boolean operators“and”and“or”. In Python, for
example we can write:
E X E R C I S E 5 . 1
1
2
if i > 0 and j > 0:
return True
One attempt to rewrite this in ampl is:
1
2
3
if i > 0 and j > 0:
return true
end
Will this, however, pass the type checking tests?
Are the rules foridf andparamentirely correct? For example, what about
empty parameter lists? Do we allow them?
E X E R C I S E 5 . 2
Ofen, when encountering multiple defnition error, compilers will show
the fle position of the original defnition and the attempted redefnition. Our ampl compiler
does not. How can we update our data structures to accommodate showing both positions
when there is an error?
E X E R C I S E 5 . 3C H A P T E R 6
Code generation for the Java
virtual machine
With the symbol table and type checking complete, the addition of code generation should be
relatively painless.
6.1
R U L E S
1. Youmustuse Jasmin [15] to assemble your bytecode stream to valid JVM class fles.
To understand how the mechanism behind fork-and-exec works (to spawn a child
process), refer tohttps://ece.uwaterloo.ca/~dwharder/icsrts/Tutorials/
fork_exec/.
2. Youmustplace the appropriategen* commands in your parser functions to generate
code.
3. Debugging outputmust notbe turned on in your fnal submission.
4. Your compilermustbe compiled with the provided makefle.
5. Te name of the generated class flemustbe the identifer specifed afer the“source”
keyword in the ampl source code fle.
6. Any temporary fles used during compilation and assembly – for example, the Jasmin
assembly language fle –mustbe unlinked from the fle system before the compiler
terminates, provided it terminates successfully.
7. Your compilermustread the location of the Jasmin jar from an environment variable
calledJASMIN_JAR. If it does not, your submission will not be marked.
4950
code generation for the java virtual machine
§6.3
table 6.1:Marks allocation for the fnal submission.
6.2
description weight
Test cases: variables, expressions, control fow, input, output
Test cases: functions
Style 50%
25%
25%
M A R K I N G G U I D E L I N E S
Te fnal compiler will only be tested on valid ampl source code fles. Also, as part of your
last submission form, you have to mark which ampl constructs your compiler can handle
code generation, and only these will be marked. Terefore, caps will only apply if you claim
something is complete, but it breaks. Te allocation of marks is given in table 6.1.
Strictly speaking, checking for syntax or type errors will not be evaluated again. Tis
does not, however, mean that you can simply remove the symbol table from the equation.
Information associated with identifers must still be retrieved from the symbol table.
Finally, if your designated ampl constructs compile successfully, and in addition, there
are no memory leaks, your fnal marks (including style) for the fnal submission will be
multiplied by 1.1. If you have memory leaks, your marks remain unchanged.
6.3
P L A N O F A C T I O N
Taking table 6.1 into account, you should complete code generation in the following order:
1. Ensure that you set the target class fle name with the appropriate function, and see if
you can compile the trivial program; this will be the frst test case.
2. Handle output statements. A “Hello, world!” example will be the second test case. Also,
if output does not work, no other test cases can be run.
3. Handle mathematical expressions.
4. Handle boolean expressions.
5. Handle variable creation and assignment, which includes handling arrays.
6. Handle the “if”(–“elif”–“else”) statement.
7. Handle the “while” statement.
8. Handle input.
9. Handle functions and procedures.Te Jasmin assembler
§6.4
6.4
51
T H E J A S M I N A S S E M B L E R
Youmustonly use the following bytecodes, as defned injvm.h:
(1)aload, (2)areturn, (3)astore, (4)getstatic, (5)goto, (6)iadd,
(7)iaload, (8)iand, (9)iastore, (10)idiv, (11)ifeq, (12)if_icmpeq,
(13)if_icmpge, (14)if_icmpgt, (15)if_icmple, (16)if_icmplt,
(17)if_icmpne, (18)iload, (19)imul, (20)ineg, (21)invokestatic,
(22)invokevirtual, (23)ior, (24)istore, (25)isub, (26)irem,
(27)ireturn, (28)ixor, (29)ldc, (30)newarray, (31)return, and (32)swap.
Consult the JVM specifcation [13] for details, and ensure that you know which instructions
take their operands only from the stack, and which instructions specify operands as part of
the instruction stream. Also look at the Jasmin website [15] for assembler usage notes.
Remember, if you want to have any particular examples of code generation, just write a
small Java class, compile it withjavac, and decompile them withjavap. You should fnd
the-cand-verbosefags helpful for decompilation. When comparing your own generated
code to that produces for an equivalent Java class, do not worry about optimisation.
6.4.1 Methods
Ampl has functions and a main program, whereas the JVM supports static and instance
methods. Each ampl function must be compiled to a static method in the target class.
Te main ampl program – that is, the ampl code that appears afer the optional function
defnitions, between the “begin” and “end” keywords – must be compiled to the main
method of the class fle.
6.4.2 Variables
All ampl variablesmustbe stored as in a particular Jasmin method’s local variable array.
Besides, the code provided, youmust notuse static felds, which is to say, “global variables”.
For functions, the ofset into the local variable array starts at0, make sure that your symbol
table sets this correctly. If your function has m parameters and n local variables, indices
0, 1, . . . , m −1are used for the parameters – and the JVM will populate them appropriately
when the function is called – whereas indices m , m +1, . . . , m + n −1are used for local variables.
E X A M P L E 6 . 1
Suppose your are generating code for an ampl function that starts as
follows:
foo(int p1, int p2, bool p3) -> int:
int array v1;
bool v2;
In the stack frame’s local variable array, the parameters and local variables will be packed as:52
code generation for the java virtual machine
offset: 0
1
2
3
co nte nt: p1 p2 p3 v1 v2
§6.4
4
Note that each parameter or variable occupies exactly one cell. *
Te “ main ” ampl function must be handled slightly diferently: Te Java interpreter
expects amainmethod, which must have a Java string array for command-line arguments.
Terefore, the “main” ampl function, the ofset for local variablesmuststart at1. You may
do this either by modifying the symbol table, or by adding logic to the code generation unit.
6.4.3 Input and output
Te ampl “put” statement outputs its arguments via Java’s System.out.print method.
Tis to say, just like in C, youmust nothave an automatic newline at the end of any“put”
output. If an ampl programmer wants a newline, it must be written by appending “ \n ” to
the “put” arguments.
For input, ampl needs to distinguish between numeric and boolean literals. It does so
by borrowing Sedgewick and Wayne’s readBooleanandreadIntmethods [19]. Tey are
provided as boiler-plate Jasmin code in the code generation unit.
6.4.4 Jasmin assembler path
To read the path to the Jasmin assembler from an environment veriable, use the getenv
function fromstdlib.h. An environment variable is set (and exported to child processes)
by doing the following on the command line:
$ export JASMIN_JAR=/path/to/jasmin.jar
Remember, if you do not support this environment variable, your work cannot be marked.A P P E N D I X A
Parsing and Extended
Backus–Naur Form
Te idea of defning languages with mathematical precision goes back to Noam Chomsky.
Te language specifcation formalisms stemming from Chomsky’s work were found to be
insufcient in representing the complexity of spoken languages; they are, however, quite
useful in the specifcation of computer input and programming languages. Although we do
not study compiler design in detail in this course, we do need a way to specify input precisely
for the compiler project.
In 1960, Algol60 became the frst programming language to be defned formally and
precisely. Its syntax was defned with a formalism now known as Backus–Naur Form (BNF).
Niklaus Wirth in 1977 postulated some extensions to BNF to express repetitions in grammar
structure better. Tese extensions are known as Extended BNF (EBNF), and we will use
this standard to specify input formats. Although EBNF is now an ISO standard, we use the
simplifed version originally proposed by Wirth [28], and is also known as Wirth Syntax
Notation (WSN).
A.1
D E F I N I N G A L A N G U A G E W I T H E B N F
In a self-referential twist, EBNF is employed to defne itself in figure a.1. Note that EBNF *
gives only syntax, that is, how the input is formatted, and not semantics, that is, what the
input means. An EBNF for a set of grammar rules consists of a number of productions.
Each production looks like a mathematical equation: Te idea is that the identifers in EBNF
function like variables, and a production specifes how a right-hand side (of the equals sign)
many additional characters are allowed in the ⟨ punctuation ⟩ production. Also, the nonterminals have not been
delimited by angular brackets.
5354
parsing and extended backus–naur form
§A.1
syntax = { production }.
production = variable “=” expression “.”.
expression = term {“|” term }.
term = factor { factor }.
factor = variable | terminal | “(” expression “)” | “[” expression “]”
| “{” expression “}”.
variable = letter { letter | digit }.
terminal = “"” { letter | digit | punctuation } “"”.
letter = “a” | ... | “z” | “A” | ... | “Z”.
digit = “0” | ... | “9”.
punctuation = “=” | “.” | “|” | “(” | “)” | “[” | “]” | “{” | “}” | “"”.
figure A.1:EBNF for Extended Backus–Naur Form; for simplicity, nonterminals have not
been delimited by angular brackets.
may be substituted for the variable on the lef. A sequence of such substitutions until no
variables remain derives a sentence in the specifed language.
In the next list, let α , β , γ , and δ be any grammar symbol, which is to say, a terminal or
variable. Ten, to interpret EBNF, note the following:
1. An variable (identifer), also called a nonterminal, is a sequence of alphanumeric
characters that starts with a letter. Tey function as variables that will be substituted by
the right-hand side of a production.
2. A terminal (string) is a sequence of alphanumeric characters that are delimited by
double quotation marks. Tey are terminal values in that they cannot be substituted by
another symbol—as such, these terminals appear as literal strings in the input.
3. A choice is expressed by a vertical bar; it is interpreted as a logical (exclusive) or. For
example, α = β ∣ γ means that α may be replaced by either β or γ .
4. Optionality is expressed in two ways:
(a) Square brackets mean zero or one occurrences. Tus, a sequence of symbols
delimited by brackets may either be present in the input once, or absent entirely.
For example, α = β [ γ ] δ means that α may be replaced by either βγδ or βδ .
(b) Braces (curly brackets) mean zero or more occurrences. Tus, a sequence of
symbols delimited by braces may be present any number of times, or not at all.
For example, α = β { γ } δ may be replaced by either βδ , or βγδ , βγγδ , βγγγδ , and
so on.§A.2
Deriving a parser from an EBNF
55
5. Note that terms may be grouped by parentheses. Tis works like a normal mathematical
equation. Here the bar functions like and has the precedence of a plus sign. Tere is
no operator akin to the times sign—according to the⟨ term ⟩rule, other⟨ factors ⟩may
follow the frst factor arbitrarily, and this concatenation is like an implies times sign
or logical and. (If it helps, the algebraically inclined may imagine a concatenation
operator between factors.) For example, α = ( β ∣ γ ) δ may be replaced by either βδ or
γδ .
A.2
D E R I V I N G A P A R S E R F R O M A N E B N F
Te EBNF specifcation in this course may be parsed with a simple one-symbol lookahead
predictive parser. Tis is to say that we may use the method of recursive-descent, as is
described in the course slides. Following Wirth’s suggestions [27], we may derive our parsers
directly from the EBNF. We typically split the parsing action into two components:
1. thescanneror lexical analyser, which reads the input and builds tokens, i.e. the “words”
in the language; and
2. theparseror syntactic analyser, which takes the tokens requested from the scanner
and ensures that they form valid sentences for our input language.
Note that the tokens returned by the scanner correspond to the terminals in the language
specifcation. Tis makes intuitive sense, since for input languages, we expect to have well-
formed sentences, built from scanned literals, as input. It is also interesting to note in passing
that your garden variety scanner works over a regular language, and therefore, can be modelled
by a fnite automaton. However, parsers analysing Context-Free Grammars (CFGs), which
allow recursion in the productions, cannot be modelled by fnite automata; although we could
use a push-down automaton, the method presented here works directly over the parse tree
obtained from the input.
To construct a parser by hand, we associate with each grammar construct K a program
fragment Fr( K ) as in table a.1. Here, token is a global variable holding the token last
read from the input, while Next() and Error() are routines to read the next token into
tokenand report an error, respectively. Te setFirst( K ) contains all the tokens with which a
sentence derived from construct K may start. Note that First( K ) may be a singleton set.
If we study the last K in table a.1, an interesting question appears: What should happen
if bothtoken ∈ First( t i ) andtoken ∈ First( t j ) for some i ≠ j , where0 ⩽ i , j ⩽ n? If more
than one such choice derive a valid sentence, the grammar is ambiguous. In table a.1,
the program fragment associated with whichever of i or j is frst in theif statement will be
executed; but is this necessarily what we want?
Since computers are notoriously silly about intuiting what we as humans want from them,
ambiguity in which production to use can only complicate things—unless we give a way of
resolving the ambiguity, or better yet, disallow ambiguities entirely. For the sake of simplicity,
we opt for the latter. We do so by giving constraints to which certain constructs K must56
parsing and extended backus–naur form
§A.2
table A.1:Grammar constructs and their associated program fragments.
K
Fr( K )
“x” if token= “x”then Next() else Error() end if
( x ) Fr( x )
[ x ] if token∈ First( x ) thenFr( x ) end if
{ x } whiletoken∈ First( x ) doFr( x ) end while
f f 0 1 . . . f n
t 0 ∣ t 1 ∣ . . . ∣ t n
Fr( f 0 )Fr( f 1 ). . . Fr( f n )
if token∈ First( t 0 )thenFr( t 0 )
else if token∈ First( t 1 )thenFr( t 1 )
...
else if token∈ First( t n )thenFr( t n )
else Error()
end if
adhere; these constraints are given in table a.2. Te setFollow( K ) contains all the symbols
that may follow K , where ε denotes the empty string. Te EBNF specifcation for the compiler
project adheres to the constraints of table a.2.
Te fnal idea we need to construct a recursive-descent parser concerns how we construct
the program fragments for nonterminals according to the rules in table a.1. Wirth gives the
following rule [27]:
A parsing algorithm is derived for each nonterminal symbol, and it is formulated
as a procedure carrying the name of the symbol. Te occurrence of the symbol *
in the syntax is translated into a call of the corresponding procedure.
Note that this rule holds regardless of whether the procedure is recursive or not, and that we
exclude those productions that are handled by the scanner. Put another way, such a procedure
mimics the right side of a production according to the constructs in table a.1.
Essentially, what we are doing then, is traversing the parse tree of our input. Aparse tree
gives the structure of a particular program in a language. Every node is labelled by a grammar
symbol: Internal nodes are labelled by nonterminals, while leaves are labelled by terminals.
Ten any internal node and its children correspond to a production, where the internal node
itself corresponds to the lef-hand side of the production, and its children to the right-hand
side.
operate by causing side efects. Te languages for which Wirth is famous, such as Pascal and Oberon, all distinguish
between with functions (with return values) and procedures (without return values, likevoidfunctions in C). A
side efectis when a subroutine causes some observable change in program state outside the subroutine or when
an operator modifes its operand(s). For example, in C the expression x + 1 has no side efects (the value of x
remains unchanged), butx++has the side efect of adding one to the value stored inx.Deriving a parser from an EBNF
§A.2
57
table A.2:Ambiguity constraints over some grammar constructs.
K
t 0 ∣ t 1
f f 0 1
[ x ] or{ x }
Constr( K )
First( t 0 ) ∩First( t 1 ) = ∅
If ε ∈ f 0 , then First( f 0 ) ∩First( f 1 ) = ∅
First( x ) ∩Follow( K ) = ∅
⟨ expr ⟩= ⟨ term ⟩{(“+” | “-”)⟨ term ⟩}.
⟨ term ⟩= ⟨ factor ⟩{(“*” | “/”)⟨ factor ⟩}.
⟨ factor ⟩= ⟨ number ⟩|⟨ id ⟩| “(”⟨ expr ⟩“)”.
⟨ number ⟩= ⟨ digit ⟩{⟨ digit ⟩}.
⟨ id ⟩= ⟨ letter ⟩{⟨ letter ⟩|⟨ digit ⟩}.
⟨ digit ⟩= “0” | ... | “9”.
⟨ letter ⟩= “a” | ... | “z”.
figure A.2:EBNF for arithmetic expressions.
Suppose we wish to write a parser for arithmetic expressions over numbers
and identifers, specifed as an EBNF in figure a.2. Te question is now which productions
are handled by the scanner, and which by the parser. Does the scanner, for example, return a
unique token for every letter and digit, or does it treat⟨ number ⟩and⟨ id ⟩as token types?
E X A M P L E A . 1
Te answer follows from noting that (1) the scanner constructs tokens from their respec-
tivelexemes—the actual character sequences that constitute a token and are read from the
input—and (2) the productions for⟨ number ⟩and⟨ id ⟩areregular, that is, they do not derive
“sentences” containing themselves (which would have required a recursive parsing approach).
Since “(” , “)” , “+” , “-” , “*” , and “/” appear as terminals in the frst three productions, it is
clear that they are tokens, and that each of these tokens corresponds to one lexeme only. Next
note that digits and letters on their own have no syntactic meaning—we are, rather, interested
in the syntactic classes ⟨ number ⟩ and ⟨ id ⟩ , and wish them to be returned as tokens. Here,
then, we have tokens where each token corresponds to more than one lexeme. For example,
“var1”, “x”, and “length” are diferent lexemes for an⟨ id ⟩token.
Te pseudocode for the Next() routine is given in figure a.3. Assume that GetChar()
reads a single character from the input stream, and that IntVal(ch) returns the numeric
value of the character stored inch. Also assume thattoken,num, andidare global variables.
If the token is a⟨ number ⟩, then the numeric value of the token is given innum; if the token is an
⟨ id ⟩, then the token lexeme is given inid. Te variabletokenwill typically be implemented
as an enumerated type.
Finally, the parsing routines in figure a.4 are derived according to table a.1 and58
parsing and extended backus–naur form
§A.3
Wirth’s rule for a parsing algorithm. Consider the parse tree for 13 * (v1 + v2), given
in figure a.5. Convince yourself that this tree illustrates both how the parsing routines of
figure a.4 are called, as well as that every internal node and its children correspond to a
production in the EBNF of figure a.2. Also make sure that you understand why and where
the parsing of an invalid sentence such as13 * -(v1 + v2)will fail.
A.3
F I N A L R E M A R K S
A fnal word on parsers are now in order. When writing a large-scale, complex compiler, we
do not construct either the scanner or parser by hand. For scanners, we use tools such as
lex or its open-source incarnationflex. Tey use regular expressions to build scanning
routines—in essence, glorifed fnite automata—automatically. Similarly, we use tools such as
yacc(“Yet Another Compiler Compiler”), the open-sourcebison(you had to see this name
coming), orjavaccfor Java. Te frst two may interface withlexorflex, so that to handle
parsing, we write a token specifcation as regular expressions (like those employed by Vim)
and a CFG (passed toyaccorbison).
Note that the class of grammars we can parse with a typical compiler compiler is larger
than that which we have treated here. Te class ofLR( 1) grammars handled by these programs
is a proper superset of theLL( 1) grammar we use in this course. As a matter of fact, *
LR( 1)
parsers can be constructed for virtually all programming language constructs for which CGFs
can be written. Additionally, with anLR( 1) parser it is possible to detect syntactic errors as
soon as possible. Coupled with error-recovery markers so that we can fnd as many errors as
possible in a single pass,LR( 1) parsers are much more efcient than a simple recursive-descent
parser. It is, however, possible to structure the latter so that we do not stop afer the frst error
is encountered.
or rightmost derivations, while a number in parentheses indicates the number of lookahead input symbols that
are used.§A.3
Final remarks
1: procedure
59
Next()
▷ Before this procedure is called the frst time, ch ← GetChar() must be executed
once during initialisation.
2:
num←0
3:
clear string buferid
4:
whilechis a space characterdo
5:
ch← GetChar()
6:
end while
7:
if ch=“(”then
8:
token← lparen ;ch← GetChar()
9:
else if ch=“)”then
10:
token← rparen ;ch← GetChar()
11:
else if ch=“+”then
12:
token← plus ;ch← GetChar()
13:
else if ch=“-”then
14:
token← minus ;ch← GetChar()
15:
else if ch=“*”then
16:
token← times ;ch← GetChar()
17:
else if ch=“/”then
18:
token← div ;ch← GetChar()
19:
else if chis a digitthen
20:
repeat
21:
num←num×10+I n tV a l ( ch)
22:
ch← GetChar()
23:
untilchis not a digit
24:
token← number
25:
else if chis a letterthen
26:
repeat
27:
appendchto end of string buferid
28:
ch← GetChar()
29:
untilchis not an alphanumeric character
30:
token← id
31:
else
32:
report error: “invalid character”
33:
end if
34: end procedure
figure A.3:A scanner for the EBNF in figure a.2.60
parsing and extended backus–naur form
1: procedure
§A.3
Expr()
▷ Before this procedure is called the frst time, Next() must be called once to ensure
that the lookahead symbol is available.
2:
Term()
3:
whiletoken= plus ∨token= minus do
4:
Next()
5:
Term()
6:
end while
7: end procedure
1: procedure Term()
Factor()
3:
whiletoken= times ∨token= div do
4:
Next()
5:
Factor()
6:
end while
7: end procedure
2:
1: procedure
Factor()
if token= number then
3:
Next()
4:
else if token= id then
5:
Next()
6:
else if token= lparen then
7:
Next()
8:
Expr()
9:
if token= rparen then
10:
Next()
11:
else
12:
report error: “right parenthesis expected”
13:
end if
14:
else
15:
report error: “number, id, or lef parenthesis expected”
16:
end if
17: end procedure
2:
figure A.4:A parser for the EBNF in figure a.2.Final remarks
§A.3
61
⟨ expr ⟩
⟨ term ⟩
⟨ factor ⟩
⟨ expr ⟩
⟨ term ⟩ ⟨ term ⟩
⟨ factor ⟩ ⟨ factor ⟩ ⟨ factor ⟩
⟨ number ⟩ ⟨ id ⟩ ⟨ id ⟩
13
*
(
v1
+
v2
figure A.5:Parse tree for13 * (v1 + v2).
)A P P E N D I X B
Converting EBNF to BNF
For certain activities, such as writing a bottom-up parser, the older BNF grammar format
is better, or at least, better known. It is also easier to write attributed grammars, where we
add information on semantics, in BNF. In particular, we are interested in adding type and
evaluation rules to our grammar.
BNF only has the “operator” for choice, the vertical bar; there are no constructs for
expressing optionality, repetition, or grouping. BNF grammars, therefore, must show these by
the use of recursion in the productions. However, it is very easy to convert an EBNF grammar
to a BNF grammar. Simply do the following:
1. Replace each option[ α ] by a new nonterminal A , and add a new production
A = α ∣ ε ,
where ε denotes the empty string. Essentially, the choice between having α and not
having α is moved to the new production for A . Tis is equivalent to saying we convert
B = β [ γ ] δ to
B = βγδ ∣ βγ .
2. Replace each repetition{ α } by a new nonterminal A , and add a new production
A = Aα ∣ ε .
Here, recurring onAallows α to be repeated, and the ε allows the recursion to stop
(and α to be absent).
3. Replace each group( α ) by a new nonterminal A , and add a new production
A = α .
62Bibliography
[1] Alfred V. Aho, Monica A. Lam, Ravi Sethi, and Jefrey D. Ullman. Compilers: Principles, Tech-
niques, and Tools . Second. Reading, Mass.: Addison-Wesley, 2007.
[2] S. Bradner. Key words for use in RFCs to Indicate Requirement Levels . RFC 2119. RFC Editor,
1997-03. 3 pp. isrn: 2070-1721. url:https://www.ietf.org/rfc/rfc2119.txt.
[3] Scott Chacon. GitHub fow . 2011-08-31. url:https://scottchacon.com/2011/08/31/
github-flow.html.
[4] Scott Chacon and Ben Straub. Pro Git . Second. New York, N.Y.: apress, 2014. url: https:
//git-scm.com/book/en/v2.
[5]
Dein.vim . url:https://github.com/Shougo/dein.vim(visited on 2022-08-02).
[6] Vincent Driessen. A successful Git branching model . 2010-01-05. url:https://nvie.com/
posts/a-successful-git-branching-model/.
[7] GitHub. A collection of .gitignore templates . GitHub repository. url: https://github.
com/github/gitignore(visited on 2022-08-02).
[8] GitHub. Ignoring fles . GitHub repository. url: https://docs.github.com/en/get-
started/getting-started-with-git/ignoring-files(visited on 2022-08-02).
[9]
GNU Make: A Program for Directing Recompilation . Free Sofware Foundation, 2016. isbn:
1-882114-83-3. url:https://www.gnu.org/software/make/manual/(visited on 2022-
08-02).
[10] James Gosling, Bill Joy, Guy Steele, Gilad Bracha, and Alex Buckley. Te Java Language Speci-
fcation: Java SE 8 Edition . Redwood City, Calif.: Oracle, 2015. url:https://docs.oracle.
com/javase/specs/jls/se8/jls8.pdf.
[11] Brian W. Kernighan and Dennis M. Ritchie. Te C Programming Language . Second edition.
Englewood Clifs, New Jersey, USA. Prentice Hall, 1988.
[12]
Lazy.nvim . url:https://github.com/folke/lazy.nvim(visited on 2023-08-08).
[13] Tim Lindholm, Frank Yellin, Gilad Bracha, and Alex Buckley. Te Java Virtual Machine Speci-
fcation: Java SE 8 Edition . Redwood City, Calif.: Oracle, 2015. url:https://docs.oracle.
com/javase/specs/jvms/se8/jvms8.pdf.
[14] Steve Losh. Learn Vimscript the Hard Way . First edition. Leanpub, 2013-04-04. url:https:
//learnvimscriptthehardway.stevelosh.com/(visited on 2022-08-02).
[15] Jonathan Meyer, Daniel Reynaud, and Iouri Kharon. Jasmin. An assembler for the Java Virtual
Machine . url:http://jasmin.sourceforge.net/(visited on 2022-08-02).
6364
[16]
bibliography
NFS Documentation. D2: What is a “silly rename”? url : http://nfs.sourceforge.net/
#faq_d2(visited on 2022-08-02).
[17]
Packer.nvim . url:https://github.com/wbthomason/packer.nvim(visited on 2023-08-
08).
[18] Adam Ruka. GitFlow considered harmful . 2015-05-03. url:https://endoflineblog.com/
gitflow-considered-harmful.
[19] Robert Sedgewick and Kevin Wayne. Algorithms . Fourth. Upper Saddle River, N.J.: Pearson
Education, 2011. isbn: 9780132762564.
[20] Sytse Sijbrandij. GitLab Flow . 2014-09-29. url:https://about.gitlab.com/2014/09/
29/gitlab-flow/.
[21] Michael Sipser. Introduction to the Teory of Computation . Tird edition. Boston, Mass.: Cen-
gage Learning, 2012.
[22] Andrew S. Tanenbaum and Todd Austin. Structured Computer Organization . Sixth (interna-
tional) edition. Boston, MA: Pearson, 2013.
[23]
vim-plug . url:https://github.com/junegunn/vim-plug(visited on 2022-08-02).
[24] Wikipedia. Bit feld . Wikipedia, Te Free Encyclopedia. 2004. url:https://en.wikipedia.
org/wiki/Bit_field(visited on 2022-08-02).
[25] Wikipedia. Mask (computing) . Wikipedia, Te Free Encyclopedia. 2004. url: https://en.
wikipedia.org/wiki/Mask_(computing)(visited on 2022-08-02).
[26] Wikipedia. Shebang (Unix) . 2004. url: https://en.wikipedia.org/wiki/Shebang_
(Unix)(visited on 2022-08-02).
[27] Niklaus Wirth. Compiler Construction . Harlow, U.K.: Addison Wesley Longman, 1996.
[28] Niklaus Wirth. “What can we do about the unnecessary diversity of notation for syntactic
defnitions?” In: Communications of the ACM 20.11 (1977-11), pp. 822–823. issn: 0001-0782.
doi: 10.1145/359863.359883. url: https://doi.acm.org/10.1145/359863.
359883./**
* @file boolean.h
* @brief Boolean data type definitions.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-08-03
*/
#ifndef BOOLEAN_H
#define BOOLEAN_H
/**
* @brief A Boolean type definition.
*/
typedef enum boolean {
FALSE, /**< symbol for Boolean false */
TRUE /**< symbol for Boolean true */
} Boolean;
/** a macro to return a string value from a boolean; useful for debugging. */
#define BOOLEAN_VALUE(x) ((x) ? "TRUE" : "FALSE")
#endif /* BOOLEAN_H */
/**
* @file codegen.h
* @brief Code generation functions for AMPL-2023.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-08-05
*/
#ifndef CODEGEN_H
#define CODEGEN_H
#include "jvm.h"
#include "symboltable.h"
#include "token.h"
typedef unsigned int Label;
/**
* Assemble a Jasmin file. The file must first be written by calling
* <code>make_code_file</code>.
*
* @param[in] jasmin_path
*
the path to the Jasmin JAR file
*/
void assemble(const char *jasmin_path);/**
* Close the code generation for the current function or procedure.
*
* @param[in] varwidth
* the length of the local variable array, including space for parameters;
* should be read from the symbol table
*/
void close_subroutine_codegen(int varwidth);
/**
* Generate an instruction that does not have an operand.
*
* @param[in] opcode
*
the bytecode instruction
*/
void gen_1(Bytecode opcode);
/**
* Generate a label.
*
* @param[in] label
*
the label to generate
*/
void gen_label(Label label);
/**
* Generate an instruction with one operand.
*
* @param[in] opcode
*
the bytecode instruction
* @param[in] operand
*
the operand
*/
void gen_2(Bytecode opcode, int value);
/**
* Generate an instruction that takes a label.
*
* @param[in] opcode
*
the bytecode instruction
* @param[in] label
*
the label
*/
void gen_2_label(Bytecode opcode, Label label);
/**
* Generate a subroutine call.
*
* @param[in] fname
*
the name of the function or procedure* @param[in] idprop
*
the properties of the function or procedure identifier
*/
void gen_call(char *fname, IDPropt *idprop);
/**
* Generate an instruction sequence for handling the specified comparison,
* ensuring that either zero or one is pushed onto the stack.
*
* @param[in] opcode
*
the comparison instruction
*/
void gen_cmp(Bytecode opcode);
/**
* Generate an instruction that creates a new array of the specified type.
*
* @param[in] atype
*
the type of the array items
*/
void gen_newarray(JVMatype atype);
/**
* Generate the instructions for the displaying output on screen.
*
* @param[in] type
*
the type of the value to be printed
*/
void gen_print(ValType type);
/**
* Generate the instructions for displaying a string on screen.
*
* @param[in] string
*
the string to display
*/
void gen_print_string(char *string);
/**
* Generate the instructions for reading from standard input into a variable.
*
* @param[in] type
*
the type of the value to be read
*/
void gen_read(ValType type);
/**
* Return a new label.
*
* @return
*
the next label*/
Label get_label(void);
/**
* Get a string representation (mnemonic) of an opcode. It would
* probably not be wise to pack the strings in a const char * array -- since
* the original interpreter do not use enums, it would be stylistically
* out-of-sync to assume that the ordinal numbering will work out
* automatically. <b>Note:</b> This routine is included as an alternative to
* the approach taken in the token files.
*
* @param[in] opcode
*
the opcode for which to get the mnemonic
* @return
*
the mnemonic string of the opcode
*/
const char *get_opcode_string(Bytecode opcode);
/**
* Initialise the code generation unit.
*/
void init_code_generation(void);
/**
* Initialise the code array for a subroutine.
*
* @param[in] name
*
the name of the function or procedure
* @param[in] p
*
the properties of the function or procedure identifier
*/
void init_subroutine_codegen(const char *name, IDPropt *p);
/**
* Print the generated code to screen. This function is for debugging purposes
* only.
*/
void list_code(void);
/**
* Open the object file, and write the generated code to it.
*/
void make_code_file(void);
/**
* Set the name of the class file. This must be called after
* <code>init_code_generation</code>, but before any other code is emitted.
*
* @param[in] cname
*
*/
the name of the class filevoid set_class_name(char *cname);
/**
* Release the resources allocated or held by the code generation unit.
*/
void release_code_generation(void);
#endif /* CODEGEN_H */
/**
* @file errmsg.h
* @brief Error messages for the AMPL-2023 compiler.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-07-03
*/
#ifndef ERRMSG_H
#define ERRMSG_H
typedef enum
{
ERR_EXPECT,
ERR_EXPECTED_EXPRESSION_OR_ARRAY_ALLOCATION,
ERR_EXPECTED_EXPRESSION_OR_STRING,
ERR_EXPECTED_FACTOR,
ERR_EXPECTED_SCALAR,
ERR_EXPECTED_STATEMENT,
ERR_EXPECTED_TYPE_SPECIFIER,
ERR_ILLEGAL_ARRAY_OPERATION,
ERR_MISSING_RETURN_EXPRESSION,
ERR_MULTIPLE_DEFINITION,
ERR_NOT_AN_ARRAY,
ERR_NOT_A_FUNCTION,
ERR_NOT_A_PROCEDURE,
ERR_NOT_A_VARIABLE,
ERR_RETURN_EXPRESSION_NOT_ALLOWED,
ERR_TOO_FEW_ARGUMENTS,
ERR_TOO_MANY_ARGUMENTS,
ERR_UNKNOWN_IDENTIFIER,
ERR_UNREACHABLE
} Error;
#endif /* ERRMSG_H */
/**
* @file error.h
* @brief Error reporting, memory allocation, and string functions.*
* This file contains prototypes for error reporting (with the program name,
* source file position, and output tags), as well as wrapped versions of
* allocation and string duplication functions where any error will terminate
* the program. Warning functions prepend the string "warning" to the output,
* and do not terminate the program.
*
* Some of the functions were adapted from "The Practice of Programming" by
* Brian W. Kernighan and Rob Pike, and they are copyright (C) 1999 Lucent
* Technologies.
*
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-06-29
*/
#ifndef ERROR_H
#define ERROR_H
#include <stddef.h>
/** a place (position) in the source file */
typedef struct
{
int line; /**< the line number */
int col; /**< the column number */
} SourcePos;
extern SourcePos position;
/**
* Display an error message on the standard error stream and exit.
*
* @param[in] fmt
*
a printf format string
* @param[in] ...
*
the variable arguments to the format string
*/
void eprintf(const char *fmt, ...);
/**
* Display an error message on the standard error stream, with the current
* position prepended, and exit.
*
* @param[in] fmt
*
a printf format string
* @param[in] ...
*
the variable arguments to the format string
*/
void leprintf(const char *fmt, ...);
/*** Display an error message on the standard error stream, with a tag prepended,
* and exit.
*
* @param[in] tag
*
the tag to prepend to the error message
* @param[in] fmt
*
a printf format string
* @param[in] ...
*
the variable arguments to the format string
*/
void teprintf(const char *tag, const char *fmt, ...);
/**
* Display a warning message on the standard error stream.
*
* @param[in] fmt
*
a print format string
* @param[in] ...
*
the variable arguments to the format string
*/
void weprintf(const char *fmt, ...);
/**
* Duplicate a string, and terminate the program with a message on the standard
* error stream if the duplication fails.
*
* @param[in] s
*
the string to duplicate
* @return
*
a pointer to newly allocated memory that contains a copy of the string
*/
char *estrdup(const char *s);
/**
* Duplicate a string, and display a warning on the standard error stream if the
* duplication fails.
*
* @param[in] s
*
the string to duplicate
* @return
*
a pointer to newly allocated memory that contains a copy of the string
*/
char *westrdup(const char *s);
/**
* Allocate memory, and terminate the program with a message on the standard
* error stream if the allocation fails.
*
* @param[in] n
*
the number of bytes to allocate
* @return*
a pointer to the newly allocated memory
*/
void *emalloc(size_t n);
/**
* Allocate memory, and display a warning on the standard error stream if the
* allocation fails.
*
* @param[in] n
*
the number of bytes to allocate
* @return
*
a pointer to the newly allocated memory
*/
void *wemalloc(size_t n);
/**
* Reallocate memory, and terminate the program with a message on the standard
* error stream if the reallocation fails.
*
* @param[in] vp
*
a pointer to the existing memory of which to adjust the size
* @param[in] n
*
the new byte size
* @return
*
a pointer to the reallocated memory
* @see
*
system documentation for realloc
*/
void *erealloc(void *vp, size_t n);
/**
* Reallocate memory, and display a warning message on the standard error
* stream if the reallocation fails.
*
* @param[in] vp
*
a pointer to the existing memory of which to adjust the size
* @param[in] n
*
the new byte size
* @return
*
a pointer to the reallocated memory
* @see
*
system documentation for realloc
*/
void *werealloc(void *vp, size_t n);
/**
* Free the program name.
*/
void freeprogname(void);
/*** Free the source name.
*/
void freesrcname(void);
#ifndef __APPLE__
/**
* Return the stored program name.
*
* @return
*
the stored program name
*/
char *getprogname(void);
#endif
/**
* Return the stored source name.
*
* @return
*
the stored source name
*/
char *getsrcname(void);
#ifndef __APPLE__
/**
* Set the program name.
*
* @param[in] s
*
the program name
*/
void setprogname(char *s);
#endif
/**
* Set the source name.
*
* @param[in] s
*
the source name
*/
void setsrcname(char *s);
#endif /* ERROR_H */
/**
* @file hashtable.h
*
* A hash table implementation that can store arbitrary keys and
* values. This implementation does not support the insertion or retrieval of
* <code>NULL</code> keys or values.
*
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)* @date 2023-07-06
*/
#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "boolean.h"
/* --- error return codes --------------------------------------------------- */
#define HASH_TABLE_KEY_VALUE_PAIR_EXISTS -1
#define HASH_TABLE_NO_SPACE_FOR_NODE
-2
/** the container structure for a hash table */
typedef struct hashtab HashTab;
/* --- function prototypes -------------------------------------------------- */
/**
* Initialise a hash table. This function fails if the specialised load factor
* is less than or equal to zero, or memory could not be allocated.
*
* @param[in] loadfactor
* the maximum load factor, which, when reached, triggers a resize of the
* underlying table
* @param[in] hash
* a pointer to a hash function over the domain of the keys, taking a
* pointer to the key and the size of the underlying table as parameters
* @param[in] cmp
* a pointer to a function that compares two values from the domain of
* values, returning <code>-1</code>, <code>0</code>, or <code>1</code> if
* <code>val1</code> is less than, equal to, or greater than
* <code>val2</code>, respectively
* @return
* a pointer to the hash table container structure, or <code>NULL</code> if
* initialisation failed
*/
HashTab *ht_init(float loadfactor,
unsigned int (*hash)(void *key, unsigned int size),
int (*cmp)(void *val1, void *val2));
/**
* Associate the specified key with the specified value in the specified hash
* table. This function fails if any argument is <code>NULL</code>, or if
* insertion was not successful.
*
* @param[in] ht
*
a pointer to the hash table in which to associate the key with the value
* @param[in] key
*
a pointer to the key
* @param[in] value*
a pointer to the value
* @return
* <code>EXIT_SUCCESS</code> if the insertion was successful,
* <code>EXIT_FAILURE</code> if any argument value is <code>NULL</code>, or
* one of designated error codes if insertion failed
*/
int ht_insert(HashTab *ht, void *key, void *value);
/**
* Search the specified hash table for the value associated with the specified
* key. This function fails if any argument is <code>NULL</code>.
*
* @param[in] ht
*
a pointer to the hash table in which to search for the key
* @param[in] key
*
the key for which to find the associated value
* @return
* a pointer to the value, or <code>NULL</code> if the key was not found or
* any argument value is <code>NULL</code>
*/
void *ht_search(HashTab *ht, void *key);
/**
* Free the space associated with the specified hash table. This function fails
* if any argument is <code>NULL</code>.
*
* @param[in] hashtable
*
the hash table to free
* @param[in] freekey
*
a pointer to a function that releases the memory resources of a key
* @param[in] freeval
*
a pointer to a function that releases the memory resources of a value
* @return
* <code>EXIT_SUCCESS</code> if the memory resources of the specified hash
* table were released successfully, or <code>EXIT_FAILURE</code> otherwise
*/
int ht_free(HashTab *ht,
void (*freekey)(void *k),
void (*freeval)(void *v));
/**
* Display the specified hash table on standard output. This function fails
* silently if any argument is <code>NULL</code>.
*
* @param[in] ht
*
the hash table to display
* @param[in] keyval2str
* a pointer to a function that returns a string representation of the
* specified key and value in the specified buffer; the buffer is assumed to
* have been allocated by the caller
*/void ht_print(HashTab *ht, void (*keyval2str)(void *k, void*v, char *b));
#endif /* HASH_TABLE_H */
/**
* @file jvm.h
* @brief Definitions for Java Virtual Machine instructions and data types.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-08-05
*/
#ifndef JVM_H
#define JVM_H
/* Java array types */
typedef enum {
T_BOOLEAN = 4,
T_CHAR,
T_FLOAT,
T_DOUBLE,
T_BYTE,
T_SHORT,
T_INT,
T_LONG
} JVMatype;
/* JVM bytecodes */
typedef enum {
JVM_ALOAD,
JVM_ARETURN,
JVM_ASTORE,
JVM_GETSTATIC,
JVM_GOTO,
JVM_IADD,
JVM_IALOAD,
JVM_IAND,
JVM_IASTORE,
JVM_IDIV,
JVM_IFEQ,
JVM_IF_ICMPEQ,
JVM_IF_ICMPGE,
JVM_IF_ICMPGT,
JVM_IF_ICMPLE,
JVM_IF_ICMPLT,
JVM_IF_ICMPNE,
JVM_ILOAD,
JVM_IMUL,
JVM_INEG,
JVM_INVOKESTATIC,
JVM_INVOKEVIRTUAL,
JVM_IOR,JVM_ISTORE,
JVM_ISUB,
JVM_IREM,
JVM_IRETURN,
JVM_IXOR,
JVM_LDC,
JVM_NEWARRAY,
JVM_RETURN,
JVM_SWAP
} Bytecode;
#endif /* JVM_H */
/**
* @file scanner.h
* @brief Definitions for the exported variables and functions for the
*
lexical analyser (scanner) of AMPL-2023.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-06-29
*/
#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include "token.h"
/**
* Initialise the scanner.
*
* @param[in] in_file
*
the (already open) source file
*/
void init_scanner(FILE *in_file);
/**
* Get the next token from the input (source) file.
*
* @param[out] token
*
contains the token just scanned
*/
void get_token(Token *token);
#endif /* SCANNER_H */
/**
* @file symboltable.h
* @brief A symbol table for AMPL-2023.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-07-06
*/#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "boolean.h"
#include "token.h"
#include "valtypes.h"
typedef struct {
ValType type;
/*<< variable or subroutine return type */
union {
unsigned int offset;
/*<< local variable offset for codegen */
struct {
unsigned int nparams; /*<< number of parameters for subroutines */
ValType
*params; /*<< array of parameter types
};
};
} IDPropt;
/**
* Initialise the global symbol table.
*/
void init_symbol_table(void);
/**
* Open a new function or procedure (subroutine) context by (1) inserting the
* subroutine name and properties into the global symbol table, (2) preserving
* the global symbol table for later re-use, and (3) initialising a new local
* symbol table for the subroutine as current symbol table.
*
* @param[in] id
*
the identifier of the new function or procedure
* @param[in] prop
*
the identifier properties of the new function or procedure
* @return
* <code>TRUE</code> if the local subroutine context was set up
* successfully, or <code>FALSE</code> otherwise
*/
Boolean open_subroutine(char *id, IDPropt *prop);
/**
* Close the current subroutine context by (1) releasing memory resources
* associated with the current local symbol table, and (2) setting the preserved
* global symbol table as the current symbol table.
*/
void close_subroutine(void);
/**
* Insert the specified identifier with the specified properties into the
* current symbol table. This function "steals" the <code>id</code> and
* <code>prop</code> pointers, and assumes responsibility for their
*/* deallocation.
*
* @param[in] id
*
the identifier to insert
* @param[in] prop
*
the properties to be associated with the new identifier
* @return
* <code>FALSE</code> if the identifier is already in the current symbol
* table, or if there was not enough space for a new entry, or
* <code>TRUE</code> otherwise
*/
Boolean insert_name(char *id, IDPropt *prop);
/**
* Retrieve the properties associated with the specified identifier from the
* current symbol table.
*
* @param[in] id
*
the identifier to look up in the current symbol table
* @param[out] prop
* the pointer to the pointer to which the pointer to the properties
* structure, associated with the identifier, will be copied
* @return
* <code>TRUE</code> if the identifier exists in the current symbol table,
* or <code>FALSE</code> otherwise
*/
Boolean find_name(char *id, IDPropt **prop);
/**
* Return the number of the identifiers stored in the current symbol table.
*/
int get_variables_width(void);
/**
* Release the memory resources associated with the global symbol table.
*/
void release_symbol_table(void);
/**
* Print the current symbol table to the standard output stream.
*/
void print_symbol_table(void);
#endif /* SYMBOLTABLE_H */
/**
* @file token.h
* @brief Data type definitions for the lexical analyser (scanner) of
*
AMPL-2023.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-06-29*/
#ifndef TOKEN_H
#define TOKEN_H
/** the maximum length of an identifier */
#define MAX_ID_LEN 32
/** the types of tokens that the scanner recognises */
typedef enum {
TOK_EOF, /* end-of-file */
TOK_ID, /* identifier
*/
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
TOK_EQ, /* equal to
*/
TOK_GE, /* greater than or equal to */
TOK_GT, /* (strictly) greater than */
TOK_LE, /* less than or equal to */
TOK_LT, /* (strictly) less than
*/TOK_NE, /* not equal to
*/
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
typedef struct {
TokenType type;
/**< the type of the token
*/
union {
int value;
/**< numeric value (for integers) */
char lexeme[MAX_ID_LEN+1]; /**< lexeme for identifiers
char *string;
/**< string (for write)
};
} Token;
/**
* Return a string representation of the specified token type.
*
* @param[in] type
*
the token for which to get the string representation
* @return
*
a (constant) string representation of the specified token type
*/
const char *get_token_string(TokenType type);
#endif /* TOKEN_H */
/**
* @file valtypes.h
*/
*/* @brief Value types for AMPL-2023 type checking.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-07-06
*/
#ifndef VALTYPES_H
#define VALTYPES_H
typedef enum {
TYPE_NONE
= 0,
TYPE_ARRAY = 1,
TYPE_BOOLEAN = 2,
TYPE_INTEGER = 4,
TYPE_CALLABLE = 8
} ValType;
#define IS_ARRAY(type)
(IS_ARRAY_TYPE(type) && !IS_CALLABLE_TYPE(type))
#define IS_ARRAY_TYPE(type)
/* TODO */
#define IS_BOOLEAN_TYPE(type) /* TODO */
#define IS_CALLABLE_TYPE(type) /* TODO */
#define IS_FUNCTION(type)
(IS_CALLABLE_TYPE(type) && !IS_PROCEDURE(type))
#define IS_INTEGER_TYPE(type) /* TODO */
#define IS_PROCEDURE(type)
#define IS_VARIABLE(type)
#define SET_AS_ARRAY(type)
/* TODO */
/* TODO */
/* TODO */
#define SET_AS_CALLABLE(type) ((type) |= TYPE_CALLABLE)
#define SET_BASE_TYPE(type)
/* TODO */
#define SET_RETURN_TYPE(type) ((type) &= ~TYPE_CALLABLE)
/**
* Return a string representation of the specified value type.
*
* @param[in] type
*
the type for which to return a string representation
* @return
*
a string representation of the specified value type
*/
const char *get_valtype_string(ValType type);
#endif /* VALTYPES_H */
/**
* @file valtypes.c
* @brief Value types for AMPL-2023.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-07-06
*/
#include <assert.h>
#include "valtypes.h"static char *valtype_names[] = {
"none", "**error**", "boolean", "boolean array", "integer", "integer array",
"**error**", "**error**", "procedure", "**error**", "boolean function",
"boolean array function", "integer function", "integer array function"
};
#define NUM_TYPES (sizeof(valtype_names) / sizeof(valtype_names[0]))
const char *get_valtype_string(ValType type)
{
assert(type >= 0 && type <= NUM_TYPES);
return valtype_names[type];
}
/**
* @file token.c
* @brief Utility functions for AMPL-2023 tokens.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-06-29
*/
#include <assert.h>
#include "token.h"
/* the token strings */
static char *token_names[] = {
"end-of-file", "identifier", "numeric literal", "string literal", "'array'",
"'bool'", "'chillax'", "'elif'", "'else'", "'end'", "'if'", "'input'",
"'int'", "'let'", "'main'", "'output'", "'program'", "'return'", "'while'",
"'false'", "'true'", "'not'", "'='", "'>='", "'>'", "'<='", "'<'", "'/='",
"'-'", "'or'", "'+'", "'and'", "'/'", "'*'", "'rem'", "'->'", "':'", "','",
"'..'", "'['", "'('", "']'", "')'", "';'"
};
/* --- functions ------------------------------------------------------------ */
const char *get_token_string(TokenType type)
{
assert(type >= 0 && type < (sizeof(token_names) / sizeof(token_names[0])));
return token_names[type];
}
/**
* @file testsymboltable.c
* @brief A driver program to test the symbol table implementation.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-07-06
*/#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boolean.h"
#include "symboltable.h"
#define BUFFER_SIZE 1024
int main()
{
char buffer[BUFFER_SIZE], *id;
unsigned offset = 0;
Boolean main_is_active;
IDPropt *propts;
/* XXX: Note that the offset is here NOT used in the way it must be used for
* code generation...
*/
init_symbol_table();
main_is_active = TRUE;
printf("type \"search <Enter>\" to stop inserting and start searching.\n");
printf("Actions\n=======\n");
printf("insert <id> -- insert <id> into current table\n");
printf("find <id> -- find <id> in current table\n");
printf("open <id> -- open subroutine <id> table\n");
printf("close -- close current subroutine table\n");
printf("print -- print current symbol table\n");
printf("quit -- quit program\n");
buffer[0] = '\0';
while (TRUE) {
printf(">> ");
scanf("%s", buffer);
if (strcmp(buffer, "open") == 0) {
scanf("%s", buffer);
if (!main_is_active) {
printf("Already in subroutine ... not added.\n");
continue;
}
id = strdup(buffer);
propts = malloc(sizeof(IDPropt));
memset(propts, 0, sizeof(IDPropt));
propts->type = TYPE_CALLABLE | TYPE_INTEGER;if (open_subroutine(id, propts)) {
main_is_active = FALSE;
} else {
printf("Subroutine already exists ... not added.\n");
free(id);
free(propts);
}
} else if (strcmp(buffer, "close") == 0) {
if (main_is_active) {
printf("Cannot close main routine.\n");
continue;
}
close_subroutine();
main_is_active = TRUE;
} else if (strcmp(buffer, "print") == 0) {
print_symbol_table();
} else if (strcmp(buffer, "insert") == 0) {
scanf("%s", buffer);
id = strdup(buffer);
propts = malloc(sizeof(IDPropt));
propts->type = TYPE_INTEGER;
propts->offset = offset++;
if (!insert_name(id, propts)) {
printf("Identifier already exists ... not added.\n");
free(id);
free(propts);
}
} else if (strcmp(buffer, "find") == 0) {
scanf("%s", buffer);
if (find_name(buffer, &propts)) {
printf("\"%s\" found ", buffer);
if (IS_CALLABLE_TYPE(propts->type)) {
printf("as callable\n");
} else {
printf("at offset %i\n", propts->offset);
}
} else {
printf("Identifier not found.\n");
}
} else if (strcmp(buffer, "quit") == 0) {if (!main_is_active) {
close_subroutine();
printf("Closed subroutine.\n");
}
break;
} else {
printf("Unknown command.\n");
}
}
printf("Goodbye!\n");
release_symbol_table();
return EXIT_SUCCESS;
}
/**
* @file testhashtable.c
* @brief A driver program to test the hash table implementation.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-07-06
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "hashtable.h"
/* --- type definitions and constants --------------------------------------- */
typedef struct
{
char *id;
int num;
} Name;
#define BUFFER_SIZE 1024
/* --- function prototypes -------------------------------------------------- */
unsigned int hash(void *key, unsigned int size);
int scmp(void *v1, void *v2);
void val2str(void *key, void *value, char *buffer);
/* --- main routine --------------------------------------------------------- */
int main(){
char buffer[BUFFER_SIZE];
int i = 1, ret;
Name *np;
HashTab *ht;
ht = ht_init(0.75f, hash, scmp);
printf("Type \"search <Enter>\" to stop inserting and start searching.\n");
printf(">> ");
scanf("%s", buffer);
while (strcmp(buffer, "search") != 0)
{
np = emalloc(sizeof(Name));
np->id = estrdup(buffer);
np->num = i++;
if ((ret = ht_insert(ht, np->id, np)) == EXIT_SUCCESS)
{
printf("Insert %s with %d\n", np->id, np->num);
}
else
{
printf("Not inserted...! (%i)\n", ret);
free(np->id);
free(np);
}
printf(">> ");
scanf("%s", buffer);
}
ht_print(ht, val2str);
printf("Type \"quit <Enter>\" to exit.\n");
printf(">> ");
scanf("%s", buffer);
while (strcmp(buffer, "quit") != 0)
{
if ((np = ht_search(ht, buffer)))
{
printf("Found \"%s\" with %d.\n", np->id, np->num);
}
else
{
printf("Not found.\n");
}
printf(">> ");
scanf("%s", buffer);
}
printf("\n");
ht_free(ht, free, free);return EXIT_SUCCESS;
}
/* --- hash helper functions ------------------------------------------------ */
unsigned int hash(void *key, unsigned int size)
{
char *keystr = (char *)key;
unsigned int i, hash, length;
hash = 0;
length = strlen(keystr);
for (i = 0; i < length; i++)
{
hash += keystr[i];
}
return (hash % size);
}
int scmp(void *v1, void *v2)
{
return strcmp((char *)v1, (char *)v2);
}
void val2str(void *key, void *value, char *buffer)
{
sprintf(buffer, "%s:[%d]", (char *)key, ((Name *)value)->num);
}
/**
* @file symboltable.c
* @brief A symbol table for AMPL-2023.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-07-06
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boolean.h"
#include "error.h"
#include "hashtable.h"
#include "symboltable.h"
#include "token.h"
/* --- global static variables ---------------------------------------------- */
static HashTab *table, *saved_table;
/* TODO: Nothing here, but note that the next variable keeps a running count of* the number of variables in the current symbol table. It will be necessary
* during code generation to compute the size of the local variable array of a
* method frame in the Java virtual machine.
*/
static unsigned int curr_offset;
/* --- function prototypes -------------------------------------------------- */
static void valstr(void *key, void *p, char *str);
static void freeprop(void *p);
static unsigned int shift_hash(void *key, unsigned int size);
static int key_strcmp(void *val1, void *val2);
/* --- symbol table interface ----------------------------------------------- */
void init_symbol_table(void)
{
saved_table = NULL;
if ((table = ht_init(0.75f, shift_hash, key_strcmp)) == NULL) {
eprintf("Symbol table could not be initialised");
}
curr_offset = 1;
}
Boolean open_subroutine(char *id, IDPropt *prop)
{
/* TODO:
* - Insert the subroutine name into the global symbol table; return TRUE or
* FALSE, depending on whether or not the insertion succeeded.
* - Save the global symbol table to `saved_table`, initialise a new hash
* table for the subroutine, and reset the current offset.
*/
}
void close_subroutine(void)
{
/* TODO: Release the subroutine table, and reactivate the global table. */
}
Boolean insert_name(char *id, IDPropt *prop)
{
/* TODO: Insert the properties of the identifier into the hash table, and
* remember to increment the current offset pointer if the identifier is a
* variable.
*
* VERY IMPORTANT: Remember to read the documentation of this function in
* the header file.
*/
}
Boolean find_name(char *id, IDPropt **prop){
/* TODO: Nothing, unless you want to. */
*prop = ht_search(table, id);
if (!*prop && saved_table) {
*prop = ht_search(saved_table, id);
if (*prop && !IS_CALLABLE_TYPE((*prop)->type)) {
*prop = NULL;
}
}
return (*prop ? TRUE : FALSE);
}
int get_variables_width(void)
{
return curr_offset;
}
void release_symbol_table(void)
{
/* TODO: Free the underlying structures of the symbol table. */
}
void print_symbol_table(void)
{
ht_print(table, valstr);
}
/* --- utility functions ---------------------------------------------------- */
static void valstr(void *key, void *p, char *str)
{
char *keystr = (char *) key;
IDPropt *idpp = (IDPropt *) p;
/* TODO: Nothing, but this should give you an idea of how to look at the
* content of the symbol table.
*/
if (IS_CALLABLE_TYPE(idpp->type)) {
sprintf(str, "%s@_[%s]", keystr, get_valtype_string(idpp->type));
} else {
sprintf(str, "%s@%d[%s]", keystr, idpp->offset,
get_valtype_string(idpp->type));
}
}
/* TODO: Here you should add your own utility functions, in particular, for
* deallocation, hashing, and key comparison. For hashing, you MUST NOT use the
* simply strategy of summing the integer values of characters. I suggest you
* use some kind of cyclic bit shift hash.*/
/**
* @file scanner.c
* @brief The scanner for AMPL-2023.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* & Josef Emile Oosthuizen (26507404@sun.ac.za)
* @date 2023-06-29
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
static FILE *src_file; /* the source file pointer */
static int ch;
/* the next source character */
static int column_number; /* the current column number */
static struct
{
char *word;
/* the reserved word, i.e., the lexeme */
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
{"while", TOK_WHILE}};#define NUM_RESERVED_WORDS (sizeof(reserved) / sizeof(reserved[0]))
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
switch (ch){
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
next_char();break;
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
break;case '/':
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
}last_read = ch;
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
nstring *= 2;string = erealloc(string, nstring);
}
/* the position is set to the invalid character*/
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
/* the position is set to the illegal escape code*/
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
}lexeme[i++] = ch;
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
}/* EOF has been reached because the comment was not closed*/
position = start_pos;
leprintf("comment not closed");
}
/**
* @file hashtable.c
* @brief A generic hash table.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-07-06
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#define INITIAL_DELTA_INDEX 4
#define PRINT_BUFFER_SIZE 1024
/** an entry in the hash table */
typedef struct htentry HTentry;
struct htentry {
void *key; /*<< the key
void *value; /*<< the value
*/
*/
HTentry *next_ptr; /*<< the next entry in the bucket */
};
/** a hash table container */
struct hashtab {
/** a pointer to the underlying table
*/
HTentry **table;
/** the current size of the underlying table
*/
unsigned int size;
/** the current number of entries
*/
unsigned int num_entries;
/** the maximum load factor before the underlying table is resized */
float max_loadfactor;
/** the index into the delta array
*/
unsigned short idx;
/** a pointer to the hash function
*/
unsigned int (*hash)(void *, unsigned int);
/** a pointer to the comparison function
*/
int (*cmp)(void *, void *);
};
/* --- function prototypes -------------------------------------------------- */
/* TODO: For the following functions, refer to the TODO note at the end of the
* file.
*/static int getsize(HashTab *ht);
static HTentry **talloc(int tsize);
static void rehash(HashTab *ht);
/* TODO: For this implementation, we want to ensure we *always* have a hash
* table that is of prime size. To that end, the next array stores the
* different between a power of two and the largest prime less than that
* particular power-of-two. When you rehash, compute the new prime size using
* the following array.
*/
/**
* the array of differences between a power-of-two and the largest prime less
* than that power-of-two.
*/
unsigned short delta[] = { 0, 0, 1, 1, 3, 1, 3, 1, 5, 3, 3, 9, 3, 1, 3, 19, 15,
1, 5, 1, 3, 9, 3, 15, 3, 39, 5, 39, 57, 3, 35, 1 };
#define MAX_IDX (sizeof(delta) / sizeof(short))
/* --- hash table interface ------------------------------------------------- */
HashTab *ht_init(float loadfactor,
unsigned int (*hash)(void *, unsigned int),
int (*cmp)(void *, void *))
{
HashTab *ht;
unsigned int i;
/* TODO:
* - Initialise a hash table structure by calling an allocation function
* twice:
* (1) once to allocate space for a HashTab variable, and
* (2) once to allocate space for the table field of this new HashTab
*
variable.
* - If any allocation fails, free anything that has already been allocated
* successfully, and return NULL.
* - Also set up the other fields of the newly-allocated HashTab structure
* appropriately.
*/
return ht;
}
int ht_insert(HashTab *ht, void *key, void *value)
{
int k;
HTentry *p;
/* TODO: Insert a new key--value pair, rehashing if necessary. The best way* to go about rehashing is to put the necessary elements into a static
* function called rehash. Remember to free space (the "old" table) you not
* use any longer. Also, if something goes wrong, use the #define'd
* constants in hashtable.h for return values; remember, unless it runs out
* of memory, no operation on a hash table may terminate the program.
*/
return EXIT_SUCCESS;
}
void *ht_search(HashTab *ht, void *key)
{
int k;
HTentry *p;
/* TODO: Nothing! This function is complete, and should explain by example
* how the hash table looks and must be accessed.
*/
if (!(ht && key)) {
return NULL;
}
k = ht->hash(key, ht->size);
for (p = ht->table[k]; p; p = p->next_ptr) {
if (ht->cmp(key, p->key) == 0) {
return p->value;
}
}
return NULL;
}
int ht_free(HashTab *ht, void (*freekey)(void *k), void (*freeval)(void *v))
{
unsigned int i;
HTentry *p, *q;
if (!(ht && freekey && freeval)) {
return EXIT_FAILURE;
}
/* free the nodes in the buckets */
/* TODO */
/* free the table and container */
/* TODO */
return EXIT_SUCCESS;
}void ht_print(HashTab *ht, void (*keyval2str)(void *k, void *v, char *b))
{
unsigned int i;
HTentry *p;
char buffer[PRINT_BUFFER_SIZE];
/* TODO: This function is complete and useful for testing, but you have to
* write your own keyval2str function if you want to use it.
*/
if (ht && keyval2str) {
for (i = 0; i < ht->size; i++) {
printf("bucket[%2i]", i);
for (p = ht->table[i]; p != NULL; p = p->next_ptr) {
keyval2str(p->key, p->value, buffer);
printf(" --> %s", buffer);
}
printf(" --> NULL\n");
}
}
}
/* --- utility functions ---------------------------------------------------- */
/* TODO: I suggest completing the following helper functions for use in the
* global functions ("exported" as part of this unit's public API) given above.
* You may, however, elect not to use them, and then go about it in your own way
* entirely. The ball is in your court, so to speak, but remember: I have
* suggested using these functions for a reason -- they should make your life
* easier.
*/
static int getsize(HashTab *ht)
{
/* TODO: Compute the next prime size of the hash table. */
}
static HTentry **talloc(int tsize)
{
/* TODO: Allocate space for one hash table entry; this is useful if your
* source code lines become rather long, and you have to wrap. */
}
static void rehash(HashTab *ht)
{
/* TODO: Rehash the hash table by
* (1) allocating a new table that uses as size the next prime in the
*
"almost-double" array,
* (2) moving the entries in the existing table to appropriate positions in
*
the new table, and
* (3) freeing the old table.*/
}
/**
* @file error.c
* @brief Implementations for error reporting, memory allocation, and string
*
funtions.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-06-29
*/
/* Adapted extensively from "The Practice of Programming" */
/* by Brian W. Kernighan and Rob Pike
/* Copyright (C) 1999 Lucent Technologies
*/
*/
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "error.h"
/* --- ASCII colours -------------------------------------------------------- */
#define ESC "\033["
#define BOLD "1;"
#define ASCII_RESET ESC "m"
#define BLACK "30m"
#define RED "31m"
#define GREEN "32m"
#define YELLOW "33m"
#define BLUE "34m"
#define MAGENTA "35m"
#define CYAN "36m"
#define WHITE "37m"
#define HIGH_BLACK "90m"
#define HIGH_RED "91m"
#define HIGH_GREEN "92m"
#define HIGH_YELLOW "93m"
#define HIGH_BLUE "94m"
#define HIGH_MAGENTA "95m"
#define HIGH_CYAN "96m"
#define HIGH_WHITE "97m"
#define ASCII_BLACK ESC BLACK
#define ASCII_RED ESC RED
#define ASCII_GREEN ESC GREEN#define ASCII_YELLOW ESC YELLOW
#define ASCII_BLUE ESC BLUE
#define ASCII_MAGENTA ESC MAGENTA
#define ASCII_CYAN ESC CYAN
#define ASCII_WHITE ESC WHITE
#define ASCII_BOLD_BLACK ESC BOLD BLACK
#define ASCII_BOLD_RED ESC BOLD RED
#define ASCII_BOLD_GREEN ESC BOLD GREEN
#define ASCII_BOLD_YELLOW ESC BOLD YELLOW
#define ASCII_BOLD_BLUE ESC BOLD BLUE
#define ASCII_BOLD_MAGENTA ESC BOLD MAGENTA
#define ASCII_BOLD_CYAN ESC BOLD CYAN
#define ASCII_BOLD_WHITE ESC BOLD WHITE
#define ASCII_HIGH_BLACK ESC HIGH_BLACK
#define ASCII_HIGH_RED ESC HIGH_RED
#define ASCII_HIGH_GREEN ESC HIGH_GREEN
#define ASCII_HIGH_YELLOW ESC HIGH_YELLOW
#define ASCII_HIGH_BLUE ESC HIGH_BLUE
#define ASCII_HIGH_MAGENTA ESC HIGH_MAGENTA
#define ASCII_HIGH_CYAN ESC HIGH_CYAN
#define ASCII_HIGH_WHITE ESC HIGH_WHITE
#define ASCII_BOLD_HIGH_BLACK ESC BOLD HIGH_BLACK
#define ASCII_BOLD_HIGH_RED ESC BOLD HIGH_RED
#define ASCII_BOLD_HIGH_GREEN ESC BOLD HIGH_GREEN
#define ASCII_BOLD_HIGH_YELLOW ESC BOLD HIGH_YELLOW
#define ASCII_BOLD_HIGH_BLUE ESC BOLD HIGH_BLUE
#define ASCII_BOLD_HIGH_MAGENTA ESC BOLD HIGH_MAGENTA
#define ASCII_BOLD_HIGH_CYAN ESC BOLD HIGH_CYAN
#define ASCII_BOLD_HIGH_WHITE ESC BOLD HIGH_WHITE
/* --- error routines ------------------------------------------------------- */
SourcePos position;
#ifndef __APPLE__
static char *pname = NULL;
#endif
static char *sname = NULL;
static void _weprintf(const char *pre, const SourcePos *pos, const char *fmt,
va_list args)
{
int istty = isatty(2);
const char *ac_end = (istty ? ASCII_RESET : "");
const char *ac_src = (istty ? ASCII_BOLD_WHITE : "");
const char *ac_pos = (istty ? ASCII_BOLD_WHITE : "");
const char *progname = getprogname();
const char *srcname = getsrcname();fflush(stdout);
if (progname != NULL)
{
fprintf(stderr, "%s:", progname);
}
if (srcname != NULL)
{
fprintf(stderr, " %s%s:%s", ac_src, srcname, ac_end);
}
if (pos != NULL)
{
fprintf(stderr, "%s%d:%d%s:", ac_pos, pos->line, pos->col, ac_end);
}
if (pre != NULL)
{
fprintf(stderr, " %s ", pre);
}
else
{
fprintf(stderr, " ");
}
vfprintf(stderr, fmt, args);
if (fmt[0] != '\0' && fmt[strlen(fmt) - 1] == ':')
fprintf(stderr, " %s", strerror(errno));
fprintf(stderr, "\n");
}
void eprintf(const char *fmt, ...)
{
int istty = isatty(2);
va_list args;
const char *pre =
(istty ? ASCII_BOLD_RED "error:" ASCII_RESET : "error:");
va_start(args, fmt);
_weprintf(pre, NULL, fmt, args);
va_end(args);
exit(2);
}
void leprintf(const char *fmt, ...)
{
int istty = isatty(2);
va_list args;
const char *pre =
(istty ? ASCII_BOLD_RED "error:" ASCII_RESET : "error:");
va_start(args, fmt);
_weprintf(pre, &position, fmt, args);va_end(args);
exit(2);
}
void weprintf(const char *fmt, ...)
{
int istty = isatty(2);
va_list args;
const char *pre =
(istty ? ASCII_BOLD_YELLOW "warning:" ASCII_RESET : "warning:");
va_start(args, fmt);
_weprintf(pre, NULL, fmt, args);
va_end(args);
}
void teprintf(const char *tag, const char *fmt, ...)
{
va_list args;
va_start(args, fmt);
_weprintf(tag, &position, fmt, args);
va_end(args);
exit(3);
}
char *estrdup(const char *s)
{
char *t;
t = malloc((strlen(s) + 1) * sizeof(char));
if (t == NULL)
{
eprintf("estrdup(\"%.20s\") failed:", s);
}
strcpy(t, s);
return t;
}
char *westrdup(const char *s)
{
char *t;
t = malloc((strlen(s) + 1) * sizeof(char));
if (t == NULL)
{
weprintf("estrdup(\"%.20s\") failed:", s);
}
strcpy(t, s);
return t;
}
void *emalloc(size_t n){
void *p;
p = malloc(n);
if (p == NULL)
{
eprintf("malloc of %u bytes failed:", n);
}
return p;
}
void *wemalloc(size_t n)
{
void *p;
p = malloc(n);
if (p == NULL)
{
weprintf("malloc of %u bytes failed:", n);
}
return p;
}
void *erealloc(void *vp, size_t n)
{
void *p;
p = realloc(vp, n);
if (p == NULL)
{
eprintf("realloc of %u bytes failed:", n);
}
return p;
}
void *werealloc(void *vp, size_t n)
{
void *p;
p = realloc(vp, n);
if (p == NULL)
{
weprintf("realloc of %u bytes failed:", n);
}
return p;
}
#ifndef __APPLE__
void setprogname(char *s)
{
char *c;if ((c = strrchr(s, '/')) == NULL)
{
c = s;
}
else
{
c++;
}
pname = estrdup(c);
}
#endif
void setsrcname(char *s)
{
char *c;
if ((c = strrchr(s, '/')) == NULL)
{
c = s;
}
else
{
c++;
}
sname = estrdup(c);
}
#ifndef __APPLE__
char *getprogname(void)
{
return pname;
}
#endif
char *getsrcname(void)
{
return sname;
}
void freeprogname(void)
{
#ifndef __APPLE__
free(pname);
#endif
}
void freesrcname(void)
{
free(sname);
}/**
* @file codegen.c
* @brief The code generator for AMPL-2023.
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date 2023-08-03
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "boolean.h"
#include "codegen.h"
#include "error.h"
#include "valtypes.h"
/* --- type definitions and constants --------------------------------------- */
typedef enum {
CODE_LABEL
= 0x0001,
CODE_INSTRUCTION = 0x0002,
CODE_OPERAND
MASK_TYPE
= 0x0004,
= 0x000f,
CODE_INTEGER
= 0x0010,
CODE_ARRAY_TYPE = 0x0020,
CODE_STRING
= 0x0040,
CODE_REFERENCE = 0x0080,
MASK_DATA_TYPE = 0x00f0,
CODE_ALLOCATED = 0x0100,
MASK_ALLOCATION = 0x0f00
} CodeType;
typedef struct {
const char *instr;
short pop;
short push;
} BC;
typedef struct {
CodeType type;
union {
JVMatype atype;
Bytecode code;
Label
label;
int num;
char *string;};
} Code;
typedef struct body_s Body;
struct body_s {
char *name;
IDPropt *idprop;
Code *code;
int ip;
int max_stack_depth;
int variables_width;
Body *next;
Body *prev;
};
/* --- Jasmin output string literals ---------------------------------------- */
char class_preamble[] =
".class public %s\n"
".super java/lang/Object\n\n"
".field private static final charsetName Ljava/lang/String;\n"
".field private static final usLocale Ljava/util/Locale;\n"
".field private static final scanner Ljava/util/Scanner;\n\n"
".method static public <clinit>()V\n"
".limit stack 5\n"
".limit locals 1 \n"
"\tldc
\"UTF-8\"\n"
"\tputstatic %s/charsetName Ljava/lang/String;\n"
"\tnew java/util/Locale\n"
"\tdup\n"
"\tldc \"en\"\n"
"\tldc \"US\"\n"
"\tinvokespecial "
"java/util/Locale/<init>(Ljava/lang/String;Ljava/lang/String;)V\n"
"\tputstatic %s/usLocale Ljava/util/Locale;\n"
"\tnew java/util/Scanner\n"
"\tdup\n"
"\tnew java/io/BufferedInputStream\n"
"\tdup\n"
"\tgetstatic java/lang/System/in Ljava/io/InputStream;\n"
"\tinvokespecial"
" java/io/BufferedInputStream/<init>(Ljava/io/InputStream;)V\n"
"\tgetstatic %s/charsetName Ljava/lang/String;\n"
"\tinvokespecial "
"java/util/Scanner/<init>(Ljava/io/InputStream;Ljava/lang/String;)V\n"
"\tputstatic %s/scanner Ljava/util/Scanner;\n"
"\tgetstatic %s/scanner Ljava/util/Scanner;\n"
"\tgetstatic %s/usLocale Ljava/util/Locale;\n"
"\tinvokevirtual"
" java/util/Scanner/useLocale(Ljava/util/Locale;)Ljava/util/Scanner;\n"
"\tpop\n""\treturn\n"
".end method\n\n";
char method_init[] = /* TODO: "steal" via javap */
char method_readBoolean[] =
".method public static readBoolean()Z\n"
".limit stack 2\n"
".limit locals 1\n"
"\tgetstatic %s/scanner Ljava/util/Scanner;\n"
"\tinvokevirtual java/util/Scanner/next()Ljava/lang/String;\n"
"\tastore 0\n"
"\taload 0\n"
"\tldc
\"true\"\n"
"\tinvokevirtual java/lang/String/equalsIgnoreCase(Ljava/lang/String;)Z\n"
"\tifeq False\n"
"\ticonst_1\n"
"\tireturn\n"
"False:\n"
"\taload 0\n"
"\tldc
\"false\"\n"
"\tinvokevirtual java/lang/String/equalsIgnoreCase(Ljava/lang/String;)Z\n"
"\tifeq Exception\n"
"\ticonst_0\n"
"\tireturn\n"
"Exception:\n"
"\tnew java/util/InputMismatchException\n"
"\tdup\n"
"\tinvokespecial java/util/InputMismatchException/<init>()V\n"
"\tathrow\n"
".end method\n\n";
char method_readInt[] =
".method public static readInt()I\n"
".limit stack 1\n"
".limit locals 1\n"
"\tgetstatic %s/scanner Ljava/util/Scanner;\n"
"\tinvokevirtual java/util/Scanner/nextInt()I\n"
"\tireturn\n"
".end method\n\n";
char ref_print_boolean[] = "java/io/PrintStream/print(Z)V";
char ref_print_integer[] = "java/io/PrintStream/print(I)V";
char ref_print_stream[] = "java/lang/System/out Ljava/io/PrintStream;";
char ref_print_string[] = "java/io/PrintStream/print(Ljava/lang/String;)V";
char *ref_read_boolean; /* must be set in set_class_name */
char *ref_read_integer; /* must be set in set_class_name */
#define REF_READ_BOOLEAN "/readBoolean()Z"
#define REF_READ_INTEGER "/readInt()I"/* --- global static variables ---------------------------------------------- */
static BC instruction_set[] = {
{ "aload", 0, 1 },
{ "areturn", 1, 0 },
{ "astore", 1, 0 },
{ "getstatic", 0, 1 },
{ "goto", 0, 0 },
{ "iadd", 2, 1 },
{ "iaload", 2, 1 },
{ "iand", 2, 1 },
{ "iastore", 3, 0 },
{ "idiv", 2, 1 },
{ "ifeq", 1, 0 },
{ "if_icmpeq", 2, 0 },
{ "if_icmpge", 2, 0 },
{ "if_icmpgt", 2, 0 },
{ "if_icmple", 2, 0 },
{ "if_icmplt", 2, 0 },
{ "if_icmpne",
2, 0 },
{ "iload", 0, 1 },
{ "imul", 2, 1 },
{ "ineg", 1, 1 },
{ "invokestatic", 0, 1 },
{ "invokevirtual", 0, 0 },
{ "ior",
2, 1 },
{ "istore", 1, 0 },
{ "isub", 2, 1 },
{ "irem", 2, 1 },
{ "ireturn", 1, 0 },
{ "ixor", 2, 1 },
{ "ldc", 0, 1 },
{ "newarray",
1, 1 },
{ "return", 0, 0 },
{ "swap", 2, 2 }
};
static const char *java_types[] = {
"boolean", "char", "float", "double", "byte", "short", "int", "long"
};
#define NBYTECODES (sizeof(instruction_set) / sizeof(BC))
#define INITIAL_SIZE 1024
#define JASM_EXT
".jasmin"
static char *class_name; /**< the class name
*/
static char *function_name; /**< the name of current function
static char *jasm_name;
static int code_size;
static int ip;
/**< the jasmin file name
*/
/**< the current code array size
/**< the instruction pointer
static Body *bodies;
*/
/**< list of function bodies
*/
*/
*/static Code *code;
/**< the generated code
static IDPropt *idprop;
*/
/**< id properties of the current function
*/
int stack_depth, max_stack_depth;
/* --- function prototypes -------------------------------------------------- */
static void ensure_space(int num_instr);
static void adjust_stack(BC *instr);
/* --- code generation interface -------------------------------------------- */
void init_code_generation(void)
{
bodies = NULL;
}
void init_subroutine_codegen(const char *name, IDPropt *p)
{
max_stack_depth = stack_depth = 0;
ip = 0;
code = emalloc(sizeof(Code) * INITIAL_SIZE);
code_size = INITIAL_SIZE;
function_name = estrdup(name);
idprop = p;
}
void close_subroutine_codegen(int varwidth)
{
Body *body;
body = emalloc(sizeof(Body));
/* populate new body */
body->name = function_name;
body->idprop = idprop;
body->code = code;
body->ip = ip;
body->max_stack_depth = max_stack_depth;
body->variables_width = varwidth;
/* link into list */
/* TODO */
}
void set_class_name(char *cname)
{
size_t class_name_len;
class_name = estrdup(cname);
class_name_len = strlen(class_name);jasm_name = emalloc(class_name_len + sizeof(JASM_EXT));
strcpy(jasm_name, class_name);
strncat(jasm_name, JASM_EXT, sizeof(JASM_EXT));
#pragma GCC diagnostic ignored "-Wsizeof-pointer-memaccess"
ref_read_boolean = emalloc(class_name_len + sizeof(REF_READ_BOOLEAN));
strcpy(ref_read_boolean, class_name);
strncat(ref_read_boolean, REF_READ_BOOLEAN, sizeof(REF_READ_BOOLEAN));
ref_read_integer = emalloc(class_name_len + sizeof(REF_READ_INTEGER));
strcpy(ref_read_integer, class_name);
strncat(ref_read_integer, REF_READ_INTEGER, sizeof(REF_READ_INTEGER));
#pragma GCC diagnostic warning "-Wsizeof-pointer-memaccess"
}
void assemble(const char *jasmin_path)
{
int status;
pid_t pid;
if ((pid = fork()) < 0) {
eprintf("Could not fork a new process for assembler");
} else if (pid == 0) {
if (execlp("java", "java", "-jar", jasmin_path, jasm_name,
(char *) NULL) < 0) {
eprintf("Could not exec Jasmin");
}
}
if (waitpid(pid, &status, 0) < 0) {
eprintf("Error waiting for Jasmin");
} else {
if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS) {
eprintf("Jasmin reported failure");
} else if (WIFSIGNALED(status) || WIFSTOPPED(status)) {
eprintf("Jasmin stopped or terminated abnormally");
}
}
}
void gen_1(Bytecode opcode)
{
/* TODO */
}
void gen_2(Bytecode opcode, int operand)
{
ensure_space(2);
code[ip].type = CODE_INSTRUCTION;code[ip++].code = opcode;
code[ip].type = CODE_OPERAND | CODE_INTEGER;
code[ip++].num = operand;
adjust_stack(&instruction_set[opcode]);
}
void gen_call(char *fname, IDPropt *idprop)
{
char *fpath;
unsigned int i;
ensure_space(2);
code[ip].type = CODE_INSTRUCTION;
code[ip++].code = JVM_INVOKESTATIC;
/* 6 + 2 * idprop->nparams:
* -- 1 for '\0'
* -- 2 for '(' and ')' of parameter list
* -- 1 for '.' separating class from method name
* -- 2 for return type, including possibility of array type
* the multiplier of 2 includes the possibilities of array types
*/
fpath = emalloc(strlen(class_name) + strlen(fname) +
(6 + 2 * idprop->nparams) * sizeof(char));
strcpy(fpath, class_name);
strcat(fpath, ".");
strcat(fpath, fname);
strcat(fpath, "(");
for (i = 0; i < idprop->nparams; i++) {
if (IS_ARRAY_TYPE(idprop->params[i])) {
strcat(fpath, "[");
}
strcat(fpath, "I");
}
strcat(fpath, ")");
if (IS_ARRAY_TYPE(idprop->type)) {
strcat(fpath, "[");
}
if (idprop->type == TYPE_CALLABLE) {
strcat(fpath, "V");
} else {
strcat(fpath, "I");
}
code[ip].type = CODE_OPERAND | CODE_REFERENCE | CODE_ALLOCATED;
code[ip++].string = fpath;
adjust_stack(&instruction_set[JVM_INVOKESTATIC]);}
void gen_cmp(Bytecode opcode)
{
int l1, l2;
/* unnecessary to adjust stack depth or to ensure space, since both are
* handled in the other gen functions
*/
l1 = get_label();
l2 = get_label();
gen_2_label(opcode, l1);
gen_2(JVM_LDC, FALSE);
gen_2_label(JVM_GOTO, l2);
gen_label(l1);
gen_2(JVM_LDC, TRUE);
gen_label(l2);
}
void gen_label(Label label)
{
ensure_space(1);
code[ip].type = CODE_LABEL;
code[ip++].label = label;
}
void gen_2_label(Bytecode opcode, Label label)
{
/* TODO */
}
void gen_newarray(JVMatype atype)
{
ensure_space(2);
code[ip].type = CODE_INSTRUCTION;
code[ip++].code = JVM_NEWARRAY;
code[ip].type = CODE_OPERAND | CODE_ARRAY_TYPE;
code[ip++].atype = atype;
adjust_stack(&instruction_set[JVM_NEWARRAY]);
}
void gen_print(ValType type)
{
ensure_space(5);
code[ip].type = CODE_INSTRUCTION;
code[ip++].code = JVM_GETSTATIC;code[ip].type = CODE_OPERAND | CODE_REFERENCE;
code[ip++].string = ref_print_stream;
code[ip].type = CODE_INSTRUCTION;
code[ip++].code = JVM_SWAP;
code[ip].type = CODE_INSTRUCTION;
code[ip++].code = JVM_INVOKEVIRTUAL;
code[ip].type = CODE_OPERAND | CODE_REFERENCE;
if (IS_CALLABLE_TYPE(type)) {
SET_RETURN_TYPE(type);
}
if (type == TYPE_BOOLEAN) {
code[ip++].string = ref_print_boolean;
} else if (type == TYPE_INTEGER) {
code[ip++].string = ref_print_integer;
} else {
assert(FALSE);
}
adjust_stack(&instruction_set[JVM_GETSTATIC]);
adjust_stack(&instruction_set[JVM_SWAP]);
adjust_stack(&instruction_set[JVM_INVOKEVIRTUAL]);
}
void gen_print_string(char *string)
{
ensure_space(6);
code[ip].type = CODE_INSTRUCTION;
code[ip++].code = JVM_GETSTATIC;
code[ip].type = CODE_OPERAND | CODE_REFERENCE;
code[ip++].string = ref_print_stream;
code[ip].type = CODE_INSTRUCTION;
code[ip++].code = JVM_LDC;
code[ip].type = CODE_OPERAND | CODE_STRING | CODE_ALLOCATED;
code[ip++].string = string;
code[ip].type = CODE_INSTRUCTION;
code[ip++].code = JVM_INVOKEVIRTUAL;
code[ip].type = CODE_OPERAND | CODE_REFERENCE;
code[ip++].string = ref_print_string;
adjust_stack(&instruction_set[JVM_GETSTATIC]);
adjust_stack(&instruction_set[JVM_LDC]);adjust_stack(&instruction_set[JVM_INVOKEVIRTUAL]);
}
void gen_read(ValType type)
{
ensure_space(2);
code[ip].type = CODE_INSTRUCTION;
code[ip++].code = JVM_INVOKESTATIC;
code[ip].type = CODE_OPERAND | CODE_REFERENCE;
if (type == TYPE_BOOLEAN) {
code[ip++].string = ref_read_boolean;
} else if (type == TYPE_INTEGER) {
code[ip++].string = ref_read_integer;
} else {
assert(FALSE);
}
adjust_stack(&instruction_set[JVM_INVOKESTATIC]);
}
Label get_label(void)
{
static Label label = 1;
return label++;
}
const char *get_opcode_string(Bytecode opcode)
{
if ((unsigned long) opcode < NBYTECODES) {
return instruction_set[opcode].instr;
} else {
return "INVALID OPCODE";
}
}
/* --- code dumping --------------------------------------------------------- */
static void dump_code(FILE *file);
static void dump_method(FILE *file, Body *b);
static void dump_preamble(FILE *file, char *name);
void list_code(void)
{
dump_code(stdout);
}
void dump_code(FILE *obj_file)
{
Body *b;/* preamble */
dump_preamble(obj_file, class_name);
/* dump the methods */
for (b = bodies; b; b = b->next) {
dump_method(obj_file, b);
}
}
void make_code_file(void)
{
FILE *obj_file;
if ((obj_file = fopen(jasm_name, "w")) == NULL) {
eprintf("Could not open code file:");
}
dump_code(obj_file);
fclose(obj_file);
}
/* --- utility functions ---------------------------------------------------- */
static void ensure_space(int num_instr)
{
if (ip + num_instr > code_size) {
code = erealloc(code, code_size * 2 * sizeof(Code));
code_size *= 2;
}
}
/**
* Computes the net change in the stack depth caused by the instruction, and
* updates the maximum stack depth if necessary.
*
* @param[in] instr the instruction for which to factor in the stack effect.
*/
static void adjust_stack(BC *instr)
{
stack_depth += instr->push;
if (stack_depth > max_stack_depth) {
max_stack_depth = stack_depth;
}
stack_depth -= instr->pop;
}
/**
* Writes a method to the Jasmin output file.
** @param[in] file the output file.
* @param[in] b the body of the method
*/
static void dump_method(FILE *file, Body *b)
{
int i;
unsigned int k;
if (strcmp(b->name, "main") == 0) {
fprintf(file, ".method public static main([Ljava/lang/String;)V\n");
} else {
fprintf(file, ".method public static %s(", b->name);
for (k = 0; k < b->idprop->nparams; k++) {
if (IS_ARRAY(b->idprop->params[k])) {
fputs("[", file);
}
fputs("I", file);
}
fprintf(file, ")%s%s\n",
(IS_ARRAY_TYPE(b->idprop->type) ? "[" : ""),
(b->idprop->type == TYPE_CALLABLE ? "V" : "I"));
}
fprintf(file, ".limit stack %d\n", b->max_stack_depth);
fprintf(file, ".limit locals %d\n", b->variables_width);
for (i = 0; i < b->ip; i++) {
Code c = b->code[i];
switch (c.type & MASK_TYPE) {
case CODE_LABEL:
fprintf(file, "L%d:\n", c.label);
break;
case CODE_LABEL | CODE_OPERAND:
fprintf(file, " L%d\n", c.label);
break;
case CODE_INSTRUCTION:
fprintf(file, "\t%s", get_opcode_string(c.code));
switch (c.code) {
case JVM_ARETURN:
case JVM_IADD:
case JVM_IALOAD:
case JVM_IAND:
case JVM_IASTORE:
case JVM_IDIV:
case JVM_IMUL:
case JVM_INEG:case JVM_IOR:
case JVM_ISUB:
case JVM_IREM:
case JVM_IRETURN:
case JVM_IXOR:
case JVM_RETURN:
case JVM_SWAP:
/* emit linefeed */
fprintf(file, "\n");
break;
default:
/* no linefeed */
break;
}
break;
case CODE_OPERAND:
switch (c.type & MASK_DATA_TYPE) {
case CODE_ARRAY_TYPE:
fprintf(file, " %s\n",
java_types[c.atype - T_BOOLEAN]);
break;
case CODE_INTEGER:
fprintf(file, " %d\n", c.num);
break;
case CODE_REFERENCE:
fprintf(file, " %s\n", c.string);
break;
case CODE_STRING:
fprintf(file, " \"%s\"\n", c.string);
break;
default:
weprintf("Unknown data type for bytecode: %x\n",
(unsigned int) c.type);
}
break;
default:
weprintf("Unknown operator for bytecode: %x\n",
(unsigned int) c.type);
}
}
/* guard against a dangling label at the end of the code stream */
if ((b->code[b->ip-1].type & MASK_TYPE) == CODE_LABEL) {
fprintf(file, "\tnop\n");
}
fprintf(file, ".end method\n\n");
}
/*** Writes the preamble to the Jasmin output file. The preamble consists of (i)
* the class name and visibility specifier, (ii) the superclass, and (iii) the
* default initialiser (constructor).
*
* @param[in] file the output file.
* @param[in] name the name of the class.
*/
static void dump_preamble(FILE *file, char *name)
{
fprintf(file, class_preamble, name, name, name, name, name, name, name);
fputs(method_init, file);
fprintf(file, method_readInt, name);
fprintf(file, method_readBoolean, name);
}
void release_code_generation(void)
{
int i;
Body *b, *d;
/* remove Jasmin file */
#ifndef DEBUG_CODEGEN
unlink(jasm_name);
#endif
/* free bodies */
/* TODO */
/* free strings */
/* TODO */
}
/**
* @file amplc.c
*
* A recursive-descent compiler for the AMPL-2023 language.
*
* All scanning errors are handled in the scanner. Parser errors MUST be
* handled by the <code>abort_c</code> function. System and environment errors
* -- for example, running out of memory -- MUST be handled in the unit in which
* they occur. Transient errors -- for example, nonexistent files, MUST be
* reported where they occur. There are no warnings, which is to say, all
* errors are fatal and MUST cause compilation to terminate with an abnormal
* error code.
*
* @author W.H.K. Bester (whkbester@cs.sun.ac.za) & J.E Oosthuizen (26507404@sun.ac.za)
* @date 2023-07-04
*/
#include "symboltable.h"
#include "token.h"#include "error.h"
#include "scanner.h"
#include "errmsg.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
/* --- type definitions ----------------------------------------------------- */
#if 0
typedef struct variable_s Variable;
struct variable_s
{
char *id; /**< variable identifier */
ValType type; /**< variable type */
SourcePos pos; /**< position of the variable in the source
Variable *next; /**< pointer to the next variable in the list
*/
*/
};
#endif
/* --- debugging ------------------------------------------------------------ */
/* TODO: Your Makefile has a variable called DFLAGS. If it is set to contain
* -DDEBUG_PARSER, it will cause the following prototypes to be included, and
* the functions to which they refer (given at the end of this file) will be
* compiled. If, on the other hand, this flag is comment out, by setting
* DFLAGS to #-DDEBUG_PARSER, these functions will be excluded. These
* definitions should be used at the start and end of every parse function.
* For an example, see the provided parse_program function.
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
Token token; /**< the lookahead token type
*/
#if 0
ValType return_type; /**< the return type of the current subroutine
#endif
*//* TODO: Uncomment the previous definition for use during type checking. */
/* --- helper macros -------------------------------------------------------- */
#define STARTS_FACTOR(toktype)
\
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
void parse_type(void);
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
void parse_index(void);
void parse_expr(void);
void parse_simple(void);
void parse_term(void);
void parse_factor(void);
/* --- function prototypes: helper routines --------------------------------- *//* TODO: Uncomment the following commented-out prototypes for use during type
* checking.
*/
#if 0
void chktypes(ValType found, ValType expected, SourcePos *pos, ...);
#endif
void expect(TokenType type);
void expect_id(char **id);
/* --- function prototypes: constructors ------------------------------------ */
#if 0
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
setsrcname(argv[1]);/* open the source file, and report an error if it cannot be opened */
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
* also handle initialising and closing the "main" function. But from the
* perspective of simple parsing, this function is complete.
*/
expect(TOK_PROGRAM);
expect_id(&class_name);
expect(TOK_COLON);
while (token.type == TOK_ID)
{parse_subdef();
}
expect(TOK_MAIN);
expect(TOK_COLON);
parse_body();
free(class_name);
DBG_end("</program>");
}
/*
* subdef = ⟨id⟩ “(” ⟨type⟩ ⟨id⟩ {“,” ⟨type⟩ ⟨id⟩} “)” [“->” ⟨type⟩] “:” ⟨body⟩ .
*/
void parse_subdef(void)
{
char *id;
DBG_start("<subdef>");
expect_id(&id);
expect(TOK_LPAREN);
parse_type();
expect_id(&id);
while (token.type == TOK_COMMA)
{
expect(TOK_COMMA);
parse_type();
expect_id(&id);
}
expect(TOK_RPAREN);
if (token.type == TOK_ARROW)
{
expect(TOK_ARROW);
parse_type();
}
expect(TOK_COLON);
parse_body();
free(id);
DBG_end("</subdef>");
}
/** body = {⟨vardef⟩} ⟨statements⟩ .
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
* type = (“bool” ∣ “int”) [“array”] .
*/
void parse_type(void)
{
DBG_start("<type>");
if (token.type == TOK_INT || token.type == TOK_BOOL)
{
get_token(&token);
if (token.type == TOK_ARRAY)
{
get_token(&token);
}
}
else
{
abort_c(ERR_EXPECTED_TYPE_SPECIFIER, token.type);
}
DBG_end("</type>");
}
/*
* vardef = ⟨type⟩ ⟨id⟩ {“,” ⟨id⟩} “;” .
*/
void parse_vardef(void)
{
char *id;
DBG_start("<vardef>");
parse_type();
expect_id(&id);
while (token.type == TOK_COMMA)
{expect(TOK_COMMA);
expect_id(&id);
free(id);
}
expect(TOK_SEMICOLON);
DBG_end("</vardef>");
}
/*
* statements = “chillax” ∣ ⟨statement⟩ {“;” ⟨statement⟩} .
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
* statement = ⟨assign⟩ ∣ ⟨call⟩ ∣ ⟨if⟩ ∣ ⟨input⟩ ∣ ⟨output⟩ ∣ ⟨return⟩ ∣ ⟨while⟩ .
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
parse_if();break;
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
* assign = “let” ⟨id⟩ [⟨index⟩] “=” (⟨expr⟩ ∣ “array” ⟨simple⟩) .
*/
void parse_assign(void)
{
char *id;
DBG_start("<assign>");
expect(TOK_LET);
expect_id(&id);
if (token.type == TOK_LBRACK)
{
parse_index();
}
expect(TOK_EQ);
if (STARTS_EXPR(token.type))
{
parse_expr();
}
else if (token.type == TOK_ARRAY)
{
expect(TOK_ARRAY);
parse_simple();
}
else
{
abort_c(ERR_EXPECTED_EXPRESSION_OR_ARRAY_ALLOCATION, token.type);
}free(id);
DBG_end("</assign>");
}
/*
* call = ⟨id⟩ ⟨arglist⟩ .
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
* if = “if” ⟨expr⟩ “:” ⟨statements⟩ {“elif” ⟨expr⟩ “:” ⟨statements⟩} [“else” “:” ⟨statements⟩] “end” .
*/
void parse_if(void)
{
DBG_start("<if>");
expect(TOK_IF);
parse_expr();
expect(TOK_COLON);
parse_statements();
while (token.type == TOK_ELIF)
{
DBG_info("<elif>");
expect(TOK_ELIF);
parse_expr();
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
}expect(TOK_END);
DBG_end("</if>");
}
/*
* input = “input” “(” ⟨id⟩ [ ⟨index⟩ ] “)” .
*/
void parse_input(void)
{
char *id;
DBG_start("<input>");
expect(TOK_INPUT);
expect(TOK_LPAREN);
expect_id(&id);
if (token.type == TOK_LBRACK)
{
parse_index();
}
expect(TOK_RPAREN);
free(id);
DBG_end("</input>");
}
/*
* output = “output” “(” (⟨string⟩ ∣ ⟨expr⟩) {“..” (⟨string⟩ ∣ ⟨expr⟩)} “)” .
*/
void parse_output(void)
{
DBG_start("<output>");
expect(TOK_OUTPUT);
expect(TOK_LPAREN);
if (token.type == TOK_STR)
{
expect(TOK_STR);
}
else if (STARTS_EXPR(token.type))
{
parse_expr();
}
else
{
abort_c(ERR_EXPECTED_EXPRESSION_OR_STRING, token.type);
}
while (token.type == TOK_DOTDOT){
expect(TOK_DOTDOT);
if (token.type == TOK_STR)
{
get_token(&token);
}
else if (STARTS_EXPR(token.type))
{
parse_expr();
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
* return = “return” [⟨expr⟩] .
*/
void parse_return(void)
{
DBG_start("<return>");
expect(TOK_RETURN);
if (STARTS_EXPR(token.type))
{
parse_expr();
}
DBG_end("</return>");
}
/*
* while = “while” ⟨expr⟩ “:” ⟨statements⟩ “end” .
*/
void parse_while(void)
{
DBG_start("<while>");
expect(TOK_WHILE);
parse_expr();
expect(TOK_COLON);
parse_statements();
expect(TOK_END);DBG_end("</while>");
}
/*
* arglist = “(” ⟨expr⟩ {“,” ⟨expr⟩ }“)” .
*/
void parse_arglist(void)
{
DBG_start("<arglist>");
expect(TOK_LPAREN);
parse_expr();
while (token.type == TOK_COMMA)
{
expect(TOK_COMMA);
parse_expr();
}
expect(TOK_RPAREN);
DBG_end("</arglist>");
}
/*
* index = “[” ⟨simple⟩ “]” .
*/
void parse_index(void)
{
DBG_start("<index>");
expect(TOK_LBRACK);
parse_simple();
expect(TOK_RBRACK);
DBG_end("</index>");
}
/*
* expr = ⟨simple⟩ [⟨relop⟩ ⟨simple⟩] .
*/
void parse_expr(void)
{
DBG_start("<expr>");
parse_simple();
if (IS_RELOP(token.type))
{
get_token(&token);
parse_simple();}
DBG_end("</expr>");
}
/*
* simple = [ “-” ] ⟨term⟩ {⟨addop⟩ ⟨term⟩} .
*/
void parse_simple(void)
{
DBG_start("<simple>");
if (token.type == TOK_MINUS)
{
get_token(&token);
}
parse_term();
while (IS_ADDOP(token.type))
{
get_token(&token);
parse_term();
}
DBG_end("</simple>");
}
/*
* term = ⟨factor⟩ {⟨mulop⟩ ⟨factor⟩} .
*/
void parse_term(void)
{
DBG_start("<term>");
parse_factor();
while (IS_MULOP(token.type))
{
get_token(&token);
parse_factor();
}
DBG_end("</term>");
}
/*
* factor = ⟨id⟩ [ ⟨index⟩ ∣ ⟨arglist⟩ ] ∣ ⟨num⟩ ∣ “(” ⟨expr⟩ “)” ∣ “not” ⟨factor⟩ ∣ “true” ∣ “false” .
*/
void parse_factor(void)
{DBG_start("<factor>");
char *id;
if (token.type == TOK_ID)
{
expect_id(&id);
if (token.type == TOK_LBRACK)
{
parse_index();
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
parse_expr();
expect(TOK_RPAREN);
}
else if (token.type == TOK_NOT)
{
expect(TOK_NOT);
parse_factor();
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
#if 0void chktypes(ValType found, ValType expected, SourcePos *pos, ...)
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
abort_c(ERR_TYPE, buf);
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
/* TODO: Uncomment the following two functions for use during type checking. */
#if 0
IDPropt *idpropt(ValType type, unsigned int offset, unsigned int nparams,
ValType *params){
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
case ERR_TOO_FEW_ARGUMENTS:case ERR_TOO_MANY_ARGUMENTS:
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
break;case ERR_NOT_AN_ARRAY:
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
}void abort_cp(SourcePos *posp, Error err, ...)
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
indent -= 2;va_start(args, fmt);
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
/**
* @file
alanc.c
*
* A recursive-descent compiler for the ALAN-2022 language.
*
* All scanning errors are handled in the scanner. Parser errors MUST be
* handled by the <code>abort_compile</code> function. System and environment errors,
* for example, running out of memory, MUST be handled in the unit in which they
* occur. Transient errors, for example, non-existent files, MUST be reported
* where they occur. There are no warnings, which is to say, all errors are
* fatal and MUST cause compilation to terminate with an abnormal error code.
*
* @author W.H.K. Bester (whkbester@cs.sun.ac.za)
* @date
2022-08-03
*/
#include
#include
#include
#include
#include
#include
#include
#include
<stdio.h>
<stdlib.h>
<string.h>
"scanner.h"
"errmsg.h"
"error.h"
"symboltable.h"
"codegen.h"
/* --- type definitions ----------------------------------------------------- */
typedef struct variable_s Variable;
struct variable_s
{
char *id;
/**< variable identifier
*/
ValType type;
/**< variable type
*/
SourcePos pos; /**< variable position in the source
*/
Variable *next; /**< pointer to the next variable in the list */
};
/* --- debugging ------------------------------------------------------------ */
#ifdef DEBUG_PARSERvoid debug_start(const char *fmt, ...);
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
Token token;
/**< the lookahead token.type
*/
FILE *src_file;
/**< the source code file
*/
ValType return_type; /**< the return type of the current function */
/* --- function prototypes: parser routines --------------------------------- */
void
void
void
void
void
void
void
void
void
void
void
void
void
void
void
void
void
void
parse_source(void);
parse_funcdef(void);
parse_body(void);
parse_type(ValType *);
parse_vardef(void);
parse_statements(void);
parse_statement(void);
parse_assign(void);
parse_call(void);
parse_if(void);
parse_input(void);
parse_leave(void);
parse_output(void);
parse_while(void);
parse_expr(ValType *type);
parse_simple(ValType *type);
parse_term(ValType *type);
parse_factor(ValType *type);
/* --- helper macros -------------------------------------------------------- */
#define STARTS_FACTOR(toktype)
\
(toktype == TOKEN_ID || toktype == TOKEN_NUMBER ||
\
toktype == TOKEN_OPEN_PARENTHESIS || toktype == TOKEN_NOT || \
toktype == TOKEN_TRUE || toktype == TOKEN_FALSE)
#define STARTS_EXPR(toktype) \
(toktype == TOKEN_MINUS || STARTS_FACTOR(toktype))
#define IS_ADDOP(toktype)
\
(toktype >= TOKEN_MINUS && toktype <= TOKEN_PLUS || \
toktype == TOKEN_OR)
#define IS_MULOP(toktype)
\
(toktype == TOKEN_AND || toktype == TOKEN_DIVIDE || \
toktype == TOKEN_MULTIPLY || toktype == TOKEN_REMAINDER)
#define IS_ORDOP(toktype) \
(toktype == TOKEN_EQUAL || toktype == TOKEN_NOT_EQUAL)#define IS_RELOP(toktype)
\
(toktype == TOKEN_EQUAL || toktype == TOKEN_GREATER_EQUAL ||
\
toktype == TOKEN_GREATER_THAN || toktype == TOKEN_LESS_EQUAL || \
toktype == TOKEN_LESS_THAN || toktype == TOKEN_LESS_THAN ||
\
toktype == TOKEN_NOT_EQUAL)
#define IS_TYPE_TOKEN(toktype) \
(toktype == TOKEN_BOOLEAN || toktype == TOKEN_INTEGER)
/* --- function prototypes: helper routines --------------------------------- */
void check_types(ValType type1, ValType type2, SourcePos *pos, ...);
void expect(TokenType type);
void expect_id(char **id);
IDprop *idprop(ValType type, unsigned int offset, unsigned int nparams,
ValType *params);
Variable *variable(char *id, ValType type, SourcePos pos);
/* --- function prototypes: error reporting --------------------------------- */
void abort_compile(Error err, ...);
void abort_compile_pos(SourcePos *posp, Error err, ...);
/* --- main routine --------------------------------------------------------- */
int main(int argc, char *argv[])
{
char *jasmin_path;
setprogname(argv[0]);
if (argc != 2)
{
eprintf("usage: %s <filename>", getprogname());
}
if ((jasmin_path = getenv("JASMIN_JAR")) == NULL)
{
eprintf("JASMIN_JAR environment variable not set");
}
init_symbol_table();
init_code_generation();
if ((src_file = fopen(argv[1], "r")) == NULL)
{
eprintf("file '%s' could not be opened:", argv[1]);
}
setsrcname(argv[1]);
init_scanner(src_file);
get_token(&token);
parse_source();make_code_file();
assemble(jasmin_path);
release_symbol_table();
release_code_generation();
fclose(src_file);
freeprogname();
freesrcname();
#ifdef DEBUG_PARSER
printf("SUCCESS!\n");
#endif
return EXIT_SUCCESS;
}
/* --- parser routines ------------------------------------------------------ */
/* <source> = "source" <id> { <funcdef> } <body>. */
void parse_source(void)
{
DBG_start("<source>");
char *class_name;
IDprop *main;
expect(TOKEN_SOURCE);
expect_id(&class_name);
set_class_name(class_name);
free(class_name);
while (token.type == TOKEN_FUNCTION)
{
parse_funcdef();
}
main = idprop(TYPE_CALLABLE, 0, 0, NULL);
init_subroutine_codegen("main", main);
free(main);
parse_body();
gen_1(JVM_RETURN);
close_subroutine_codegen(get_variables_width());
DBG_end("</source>");
}
/* <funcdef> = "function" <id> "(" [ <type> <id> { "," <type> <id> } ] ")"
* ["to" <type> ] <body> */
void parse_funcdef(void)
{
DBG_start("<funcdef>");
char *t, *id;
ValType type;
SourcePos pos = position;Variable *param_list = NULL;
expect(TOKEN_FUNCTION);
expect_id(&id);
expect(TOKEN_OPEN_PARENTHESIS);
if (IS_TYPE_TOKEN(token.type))
{
parse_type(&type);
pos = position;
expect_id(&t);
param_list = variable(t, type, pos);
Variable *temp = param_list;
/* parse all parameters and link them to the param_list */
while (token.type == TOKEN_COMMA)
{
get_token(&token);
parse_type(&type);
expect_id(&t);
pos = position;
param_list->next = variable(t, type, pos);
param_list = param_list->next; /* move to the next variable */
}
param_list = temp; /* reset param_list to the start of the list */
}
// Count the number of parameters by traversing the linked list.
int nparams = 0;
Variable *p = param_list; // start from the second variable
while (p != NULL)
{ // traverse the list
nparams++;
p = p->next;
}
/* create an array of ValTypes for the params field of the IDprop
* structure and fill it with the types of the parameters. */
ValType *params = emalloc(sizeof(ValType) * nparams);
p = param_list;
for (int i = 0; i < nparams; i++)
{
params[i] = p->type;
p = p->next;
}
expect(TOKEN_CLOSE_PARENTHESIS);
if (token.type == TOKEN_TO)
{
get_token(&token);
parse_type(&type);
SET_AS_CALLABLE(type);
return_type = type;IDprop *idp = idprop(type, get_variables_width(), nparams, params);
init_subroutine_codegen(id, idp);
SET_RETURN_TYPE(return_type);
open_subroutine(id, idp);
}
else
{
return_type = TYPE_NONE;
IDprop *idp = idprop(TYPE_CALLABLE, get_variables_width(), nparams, params);
init_subroutine_codegen(id, idp);
open_subroutine(id, idp);
}
// insert function parameters and local variables into subroutine
p = param_list;
for (int i = 0; i < nparams; i++)
{
insert_name(p->id, idprop(p->type, get_variables_width(), 0, NULL));
p = p->next;
}
parse_body();
if (return_type == TYPE_NONE)
{
gen_1(JVM_RETURN);
}
else if (IS_ARRAY_TYPE(return_type))
{
gen_1(JVM_ARETURN);
}
else
{
gen_1(JVM_IRETURN);
}
close_subroutine_codegen(get_variables_width());
// go through the linked list and free each variable
while (param_list != NULL)
{
Variable *temp = param_list;
param_list = param_list->next;
free(temp);
}
close_subroutine();
DBG_end("</funcdef>");/* <body> = "begin" { <vardef> } <statements> "end" */
void parse_body(void)
{
DBG_start("<body>");
expect(TOKEN_BEGIN);
while (IS_TYPE_TOKEN(token.type))
{
parse_vardef();
}
parse_statements();
expect(TOKEN_END);
DBG_end("</body>");
}
/* <type> = ("boolean" | "integer") ["array"] */
void parse_type(ValType *type)
{
DBG_start("<type>");
if (token.type == TOKEN_BOOLEAN)
{
get_token(&token);
*type = TYPE_BOOLEAN;
}
else if (token.type == TOKEN_INTEGER)
{
get_token(&token);
*type = TYPE_INTEGER;
}
if (token.type == TOKEN_ARRAY)
{
get_token(&token);
SET_AS_ARRAY(*type);
}
DBG_end("</type>");
}
/* <vardef> = <type> <id> { "," <id> } ";" */
void parse_vardef(void)
{
DBG_start("<vardef>");
char *t;
ValType type;
parse_type(&type);
expect_id(&t);
insert_name(t, idprop(type, get_variables_width(), 0, NULL));while (token.type == TOKEN_COMMA)
{
get_token(&token);
expect_id(&t);
insert_name(t, idprop(type, get_variables_width(), 0, NULL));
}
expect(TOKEN_SEMICOLON);
DBG_end("</vardef>");
}
/* <statements> = "relax" | <statement> { ";" <statement> } */
void parse_statements(void)
{
DBG_start("<statements>");
if (token.type == TOKEN_RELAX)
{
get_token(&token);
}
else
{
parse_statement();
while (token.type == TOKEN_SEMICOLON)
{
get_token(&token);
parse_statement();
}
}
DBG_end("</statements>");
}
/* <statement> = <assign> | <call> | <leave> | <if> | <input> | <output> |
* <while> */
void parse_statement(void)
{
DBG_start("<statement>");
switch (token.type)
{
case TOKEN_ID:
parse_assign();
break;
case TOKEN_CALL:
parse_call();
break;
case TOKEN_IF:
parse_if();
break;
case TOKEN_GET:
parse_input();
break;case TOKEN_LEAVE:
parse_leave();
break;
case TOKEN_PUT:
parse_output();
break;
case TOKEN_WHILE:
parse_while();
break;
default:
abort_compile(ERR_STATEMENT_EXPECTED, token.type);
}
DBG_end("</statement>");
}
/* <assign> = <id> [ "[" <simple> "]" ] ":=" (<expr> | "array" <simple>) */
void parse_assign(void)
{
DBG_start("<assign>");
ValType temp;
char *t;
IDprop *prop;
expect_id(&t);
find_name(t, &prop);
if (token.type == TOKEN_OPEN_BRACKET)
{
get_token(&token);
gen_2(JVM_ALOAD, prop->offset);
parse_simple(&temp);
expect(TOKEN_CLOSE_BRACKET);
}
expect(TOKEN_GETS);
if (STARTS_EXPR(token.type))
{
parse_expr(&temp);
if (IS_ARRAY_TYPE(temp))
{
gen_2(JVM_ASTORE, prop->offset);
}
else
{
if (IS_ARRAY(prop->type))
{}
else
{
gen_2(JVM_ISTORE, prop->offset);
}
}
}
else if (token.type == TOKEN_ARRAY)
{
get_token(&token);
parse_simple(&temp);
gen_newarray(T_INT);
gen_2(JVM_ASTORE, prop->offset);
}
else
{
abort_compile(ERR_ARRAY_ALLOCATION_OR_EXPRESSION_EXPECTED, token.type);
}
free(t);
DBG_end("</assign>");
}
/* <call> = "call" <id> "(" [ <expr> { "," <expr> } ] ")" */
void parse_call(void)
{
DBG_start("<call>");
ValType temp;
char *t;
IDprop *prop;
expect(TOKEN_CALL);
expect_id(&t);
find_name(t, &prop);
expect(TOKEN_OPEN_PARENTHESIS);
if (STARTS_EXPR(token.type))
{
parse_expr(&temp);
while (token.type == TOKEN_COMMA)
{
get_token(&token);
parse_expr(&temp);
}
}gen_call(t, prop);
expect(TOKEN_CLOSE_PARENTHESIS);
free(t);
DBG_end("</call>");
}
/* <if> = "if" <expr> "then" <statements> { "elsif" <expr> "then"
* <statements> } [ "else" <statements> ] "end" */
void parse_if(void)
{
DBG_start("<if>");
ValType temp;
Label begin, end;
begin = get_label();
end = get_label();
expect(TOKEN_IF);
parse_expr(&temp);
gen_2_label(JVM_IFEQ, begin);
expect(TOKEN_THEN);
parse_statements();
gen_2_label(JVM_GOTO, end);
gen_label(begin);
while (token.type == TOKEN_ELSIF)
{
gen_label(begin);
begin = get_label();
get_token(&token);
parse_expr(&temp);
gen_2_label(JVM_IFEQ, begin);
expect(TOKEN_THEN);
parse_statements();
gen_2_label(JVM_GOTO, end);
gen_label(begin);
}
if (token.type == TOKEN_ELSE)
{
get_token(&token);
parse_statements();
}
gen_label(end);
expect(TOKEN_END);
DBG_end("</if>");}
/* <input> = "get" <id> [ "[" <simple> "]" ] */
void parse_input(void)
{
DBG_start("<input>");
ValType temp;
char *t;
IDprop *prop;
expect(TOKEN_GET);
expect_id(&t);
find_name(t, &prop);
if (token.type == TOKEN_OPEN_BRACKET)
{
gen_2(JVM_ALOAD, prop->offset);
get_token(&token);
parse_simple(&temp);
gen_read(temp);
gen_1(JVM_IASTORE);
expect(TOKEN_CLOSE_BRACKET);
}
else
{
gen_read(prop->type);
gen_2(JVM_ISTORE, prop->offset);
}
free(t);
DBG_end("</input>");
}
/* <leave> = "leave" [ <expr> ] */
void parse_leave(void)
{
DBG_start("<leave>");
ValType temp;
expect(TOKEN_LEAVE);
if (STARTS_EXPR(token.type))
{
parse_expr(&temp);
}
if (return_type == TYPE_NONE)
{
gen_1(JVM_RETURN);
}else if (IS_ARRAY_TYPE(return_type))
{
gen_1(JVM_ARETURN);
}
else
{
gen_1(JVM_IRETURN);
}
DBG_end("</leave>");
}
/* <output> = "put" (<string> | <expr>) {"." (<string> | <expr>)} */
void parse_output(void)
{
DBG_start("<output>");
ValType temp;
char *s;
expect(TOKEN_PUT);
if (token.type == TOKEN_STRING)
{
s = strdup(token.string);
gen_print_string(s);
free(token.string);
get_token(&token);
}
else if (STARTS_EXPR(token.type))
{
parse_expr(&temp);
gen_print(temp);
}
else
{
abort_compile(ERR_EXPRESSION_OR_STRING_EXPECTED, token.type);
}
while (token.type == TOKEN_CONCATENATE)
{
get_token(&token);
if (token.type == TOKEN_STRING)
{
s = strdup(token.string);
gen_print_string(s);
free(token.string);
get_token(&token);
}
else if (STARTS_EXPR(token.type))
{parse_expr(&temp);
gen_print(temp);
}
else
{
abort_compile(ERR_EXPRESSION_OR_STRING_EXPECTED,
token.type);
}
}
DBG_end("</output>");
}
/* <while> = "while" <expr> "do" <statements> "end" */
void parse_while(void)
{
DBG_start("<while>");
ValType temp;
Label begin, end;
begin = get_label();
end = get_label();
gen_label(begin);
expect(TOKEN_WHILE);
parse_expr(&temp);
gen_2_label(JVM_IFEQ, end);
expect(TOKEN_DO);
parse_statements();
gen_2_label(JVM_GOTO, begin);
expect(TOKEN_END);
gen_label(end);
DBG_end("</while>");
}
/* <expr> = <simple> [ <relop> <simple> ] */
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
case TOKEN_EQUAL:
get_token(&token);
parse_simple(&temp);gen_cmp(JVM_IF_ICMPEQ);
break;
case TOKEN_NOT_EQUAL:
get_token(&token);
parse_simple(&temp);
gen_cmp(JVM_IF_ICMPNE);
break;
case TOKEN_LESS_THAN:
get_token(&token);
parse_simple(&temp);
gen_cmp(JVM_IF_ICMPLT);
break;
case TOKEN_LESS_EQUAL:
get_token(&token);
parse_simple(&temp);
gen_cmp(JVM_IF_ICMPLE);
break;
case TOKEN_GREATER_THAN:
get_token(&token);
parse_simple(&temp);
gen_cmp(JVM_IF_ICMPGT);
break;
case TOKEN_GREATER_EQUAL:
get_token(&token);
parse_simple(&temp);
gen_cmp(JVM_IF_ICMPGE);
break;
default:
break;
}
}
DBG_end("</expr>");
}
/* <simple> = [ "-" ] <term> { <addop> <term> } */
void parse_simple(ValType *type)
{
DBG_start("<simple>");
if (token.type == TOKEN_MINUS)
{
get_token(&token);
parse_term(type);
gen_1(JVM_INEG);
}
else
{
parse_term(type);
}
while (IS_ADDOP(token.type))
{switch (token.type)
{
case TOKEN_PLUS:
get_token(&token);
parse_term(type);
gen_1(JVM_IADD);
break;
case TOKEN_MINUS:
get_token(&token);
parse_term(type);
gen_1(JVM_ISUB);
break;
case TOKEN_OR:
get_token(&token);
parse_term(type);
gen_1(JVM_IOR);
break;
default:
break;
}
}
DBG_end("</simple>");
}
/* <term> = <factor> { <mulop> <factor> } */
void parse_term(ValType *type)
{
DBG_start("<term>");
parse_factor(type);
while (IS_MULOP(token.type))
{
switch (token.type)
{
case TOKEN_MULTIPLY:
get_token(&token);
parse_factor(type);
gen_1(JVM_IMUL);
break;
case TOKEN_DIVIDE:
get_token(&token);
parse_factor(type);
gen_1(JVM_IDIV);
break;
case TOKEN_REMAINDER:
get_token(&token);
parse_factor(type);
gen_1(JVM_IREM);break;
case TOKEN_AND:
get_token(&token);
parse_factor(type);
gen_1(JVM_IAND);
break;
default:
break;
}
}
DBG_end("</term>");
}
/* <factor> = <id> [ "[" <simple> "]" | "(" [ <expr> { "," <expr> } ] ")" ] |
* <num> | "(" <expr> ")" | "not" <factor> | "true" | "false" */
void parse_factor(ValType *type)
{
DBG_start("<factor>");
if (token.type == TOKEN_ID)
{
char *factor_id;
expect_id(&factor_id);
IDprop *prop;
find_name(factor_id, &prop);
if (token.type == TOKEN_OPEN_BRACKET)
{
get_token(&token);
gen_2(JVM_ALOAD, prop->offset);
parse_simple(type);
*type = prop->type;
SET_BASE_TYPE(*type);
gen_1(JVM_IALOAD);
expect(TOKEN_CLOSE_BRACKET);
}
else if (token.type == TOKEN_OPEN_PARENTHESIS)
{
get_token(&token);
if (STARTS_EXPR(token.type))
{
parse_expr(type);
while (token.type == TOKEN_COMMA)
{
get_token(&token);parse_expr(type);
}
}
gen_call(factor_id, prop);
expect(TOKEN_CLOSE_PARENTHESIS);
}
else
{
if (IS_ARRAY_TYPE(prop->type))
{
*type = prop->type;
gen_2(JVM_ALOAD, prop->offset);
}
else
{
*type = prop->type;
gen_2(JVM_ILOAD, prop->offset);
}
}
free(factor_id);
}
else if (token.type == TOKEN_NUMBER)
{
gen_2(JVM_LDC, token.value);
*type = TYPE_INTEGER;
get_token(&token);
}
else if (token.type == TOKEN_OPEN_PARENTHESIS)
{
get_token(&token);
parse_expr(type);
expect(TOKEN_CLOSE_PARENTHESIS);
}
else if (token.type == TOKEN_NOT)
{
get_token(&token);
gen_2(JVM_LDC, 1);
parse_factor(type);
gen_1(JVM_IXOR);
}
else if (token.type == TOKEN_TRUE)
{
*type = TYPE_BOOLEAN;
gen_2(JVM_LDC, 1);
get_token(&token);
}
else if (token.type == TOKEN_FALSE)
{*type = TYPE_BOOLEAN;
gen_2(JVM_LDC, 0);
get_token(&token);
}
else
{
abort_compile(ERR_FACTOR_EXPECTED, token.type);
}
DBG_end("</factor>");
}
/* --- helper routines ------------------------------------------------------ */
#define MAX_MESSAGE_LENGTH 256
void check_types(ValType found, ValType expected, SourcePos *pos, ...)
{
char buf[MAX_MESSAGE_LENGTH], *s;
va_list ap;
if (found != expected)
{
buf[0] = '\0';
va_start(ap, pos);
s = va_arg(ap, char *);
vsnprintf(buf, MAX_MESSAGE_LENGTH, s, ap);
va_end(ap);
if (pos != NULL)
{
position = *pos;
}
leprintf("incompatible types (expected %s, found %s) %s",
get_valtype_string(expected), get_valtype_string(found), buf);
}
}
void expect(TokenType type)
{
if (token.type == type)
{
get_token(&token);
}
else
{
abort_compile(ERR_EXPECT, type);
}
}
void expect_id(char **id)
{
if (token.type == TOKEN_ID)
{
*id = strdup(token.lexeme);
get_token(&token);
}
else
{}
IDprop *idprop(ValType type, unsigned int offset, unsigned int nparams,
ValType *params)
{
IDprop *ip;
ip = emalloc(sizeof(IDprop));
ip->type = type;
ip->offset = offset;
ip->nparams = nparams;
ip->params = params;
return ip;
}
Variable *variable(char *id, ValType type, SourcePos pos)
{
Variable *vp;
vp = emalloc(sizeof(Variable));
vp->id = id;
vp->type = type;
vp->pos = pos;
vp->next = NULL;
return vp;
}
/* --- error handling routine ----------------------------------------------- */
void _abort_compile(SourcePos *posp, Error err, va_list args);
void abort_compile(Error err, ...)
{
va_list args;
va_start(args, err);
_abort_compile(NULL, err, args);
va_end(args);
}
void abort_compile_pos(SourcePos *posp, Error err, ...)
{
va_list args;
va_start(args, err);
_abort_compile(posp, err, args);
va_end(args);
}
void _abort_compile(SourcePos *posp, Error err, va_list args)
{
char expstr[MAX_MESSAGE_LENGTH], *s;
int t;
if (posp)
{position = *posp;
}
// snprintf(expstr, MAX_MESSAGE_LENGTH, "expected %%s, but found %s",
// get_token_string(token.type));
s = va_arg(args, char *);
switch (err)
{
case ERR_ILLEGAL_ARRAY_OPERATION:
leprintf("%s is an illegal array operation", s);
break;
case ERR_LEAVE_EXPRESSION_NOT_ALLOWED_FOR_PROCEDURE:
leprintf("a leave expression is not allowed for a procedure");
break;
case ERR_MISSING_LEAVE_EXPRESSION_FOR_FUNCTION:
leprintf("missing leave expression for a function");
break;
case ERR_MULTIPLE_DEFINITION:
leprintf("multiple definition of %s", s);
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
case ERR_NOT_AN_ARRAY:
leprintf("%s is not an array", s);
break;
case ERR_SCALAR_EXPECTED:
leprintf("expected scalar variable instead of %s", s);
break;
case ERR_TOO_FEW_ARGUMENTS:
leprintf("too few arguments for call to '%s'", s);
break;
case ERR_TOO_MANY_ARGUMENTS:
leprintf("too many arguments for call to '%s'", s);
break;
case ERR_UNKNOWN_IDENTIFIER:
leprintf("unknown identifier of '%s'", s);
break;default:
break;
}
switch (err)
{
case ERR_EXPECT:
t = va_arg(args, int);
// leprintf(expstr, get_token_string(t));
break;
case ERR_FACTOR_EXPECTED:
leprintf(expstr, "factor");
break;
case ERR_UNREACHABLE:
leprintf("unreachable: %s", s);
break;
case ERR_TYPE_EXPECTED:
leprintf(expstr, "type");
break;
case ERR_STATEMENT_EXPECTED:
leprintf(expstr, "statement");
break;
case ERR_ARRAY_ALLOCATION_OR_EXPRESSION_EXPECTED:
leprintf(expstr, "array allocation or expression");
break;
case ERR_EXPRESSION_OR_STRING_EXPECTED:
leprintf(expstr, "expression or string");
break;
default:
break;
}
}
/* --- debugging output routines -------------------------------------------- */
#ifdef DEBUG_PARSER
static int indent = 0;
void debug_start(const char *fmt, ...)
{
va_list ap;
va_start(ap, fmt);
debug_info(fmt, ap);
va_end(ap);
indent += 2;
}
void debug_end(const char *fmt, ...)
{va_list ap;
indent -= 2;
va_start(ap, fmt);
debug_info(fmt, ap);
va_end(ap);
}
void debug_info(const char *fmt, ...)
{
int i;
char buf[MAX_MESSAGE_LENGTH], *buf_ptr;
va_list ap;
buf_ptr = buf;
va_start(ap, fmt);
for (i = 0; i < indent; i++)
{
*buf_ptr++ = ' ';
}
vsprintf(buf_ptr, fmt, ap);
buf_ptr += strlen(buf_ptr);
snprintf(buf_ptr, MAX_MESSAGE_LENGTH, " in line %d.\n", position.line);
fflush(stdout);
fputs(buf, stdout);
fflush(NULL);
va_end(ap);
}
#endif /* DEBUG_PARSER */



xpr(T0) = simple(T1) T0 ← T1
| simple(T1) “=” simple(T2) T0 ← bool T1 = T2
| simple(T1) “/=” simple(T2) T0 ← bool T1 = T2
| simple(T1) “>=” simple(T2) T0 ← bool T1 = T2 = int
| simple(T1) “>” simple(T2) T0 ← bool T1 = T2 = int
| simple(T1) “<=” simple(T2) T0 ← bool T1 = T2 = int
| simple(T1) “<” simple(T2) T0 ← bool T1 = T2 = int
simple(T0) = term(T1) T0 ← T1
| “-” term(T1) T0 ← T1 T1 = int
| term(T1) “+” simple′(T2) T0 ← T1 T1 = T2 = int
| term(T1) “-” simple′(T2) T0 ← T1 T1 = T2 = int
| term(T1) “or” simple′(T2) T0 ← T1 T1 = T2 = bool
term(T0) = factor(T1) T0 ← T1
| factor(T1) “*” term′(T2) T0 ← T1 T1 = T2 = int
| factor(T1) “/” term′(T2) T0 ← T1 T1 = T2 = int
| factor(T1) “rem” term′(T2) T0 ← T1 T1 = T2 = int
| factor(T1) “and” term′(T2) T0 ← T1 T1 = T2 = bool
factor(T0) = num T0 ← int
| id idf(T1 , id) T0 ← T1
| “(” expr(T1) “)” T0 ← T1
| “not” factor(T1) T0 ← T1 T1 = bool
| “false” T0 ← bool
| “true” T0 ← bool
idf(T0 , id) = “[” simple(T1) “]” T0 ← base(T2) T1 = int
∧ T2 ← find(id)
∧ T2 ⊆ array
| “(” expr(T1) param(k1) “)” T0 ← T2 T1 = ptype(id, 0)
∧ T2 ← find(id)
∧ T2 ⊆ function
∧ k0 ← k1 + 1
∧ k0 = nparams(id)
| ε T0 ← T1 T1 ← find(id) ∧ T1 /⊆ function
param(k0 , id) = param′(k1 , id) “,” expr(T1) k0 ← k2 k2 ← k1 + 1
∧ n ← nparams(id)
∧ T1 = ptype(id, n − k2)
| ε k0 ← 0
