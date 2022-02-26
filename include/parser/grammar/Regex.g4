grammar Regex;

/**
 * SQL Regex Syntax.
 * http://www.firebirdsql.org/file/documentation/html/en/refdocs/fblangref40/firebird-40-language-reference.html#fblangref40-commons-predsimilarto
 */

/**
 Lexer Rules.
 */
WS: [ \t\n\r]+ -> skip;
INT: Digit;

Wildcard: '%'; // equivalent to => .*
CharacterClassStart: '[';
CharacterClassEnd: ']';
Caret: '^';
Hyphen: '-';
Underscore: '_'; // equivalent to => .
Pipe: '|';
OpenParen: '(';
CloseParen: ')';
Plus: '+';
Asterisk: '*';
Qmark: '?';
OpenBrace: '{';
CloseBrace: '}';
Comma: ',';
EscapeChar: '\\';
LETTER: UnicodeLetter;

/**
 Parser Rules.
 */
regex: alternation EOF;

// union expression a | b
alternation: expression ('|' expression)*;

// concat expression
expression: element*;
element: atom quantifier?;

/*
 a+ a? a* a{1} a{1,} a{1,10}
 */
quantifier:
	Asterisk
	| Plus
	| Qmark
	| '{' min = number (Comma (max = number)?)? '}';
number: INT+;

atom:
	character
	| characterClass
	| Wildcard
	| '(' capture = alternation ')';
character: regularCharacter | EscapeChar specialChar;

characterClass:
	Underscore																# anyChar
	| CharacterClassStart positive = classMember+ CharacterClassEnd			# ccPositive
	| CharacterClassStart Caret negative = classMember+ CharacterClassEnd	# ccNegative;
// | CharacterClassStart positive = classMember+ Caret negative = classMember+ CharacterClassEnd #
// ccMixed;

classMember: character | range | predefinedClass;
range: min = character Hyphen max = character;

predefinedClass: '[:' predefinedClassName ':]';
predefinedClassName:
	value = 'ALPHA'
	| value = 'UPPER'
	| value = 'LOWER'
	| value = 'DIGIT'
	| value = 'ALNUM'
	| value = 'SPACE'
	| value = 'WHITESPACE';

// regularCharacter: value = LETTER | value = INT;
regularCharacter: value = LETTER | value=INT;
specialChar:
	value = Asterisk
	| value = Plus
	| value = Qmark
	| value = CharacterClassStart
	| value = CharacterClassEnd
	| value = OpenParen
	| value = CloseParen
	| value = Caret
	| value = Hyphen
	| value = Underscore
	| value = Pipe
	| value = OpenBrace
	| value = CloseBrace
	| value = Wildcard
	| value = EscapeChar;

fragment Digit: [0-9];
// fragment UnicodeLetter: [\p{Alnum}];
//fragment UnicodeLetter: [\p{L}\p{M}*];
//fragment UnicodeLetter: [\p{Emoji}];
fragment UnicodeLetter: [\u{00000}-\u{1FFFF}];
fragment LetterOrDigit: Letter | [0-9];
fragment Letter: [a-zA-Z];
fragment ASCII: [\u0000-\u007F];