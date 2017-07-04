/*
 * Formula.g4
 *
 * @author Phillip Tse
 * @date 22.6.2017
 *
 * A grammar for AnTLR to generate a parser for boolean expressions, equations and formulas.
 */

grammar Formula;

////// Parser Rules

//Add possiblity of boolexpr v boolexpr ...

add					: PLUS term ;
mult 				: (NUMBER | VARIABLE) (TIMES (NUMBER | VARIABLE))*?; 
term				: mult add*?;

equation 			: VARIABLE EQUALS term;
boolexpr			: term BOOLRELATION NUMBER; 
intervalexpr		: term IN INTERVAL;  
formula 			: equation | boolexpr | intervalexpr ;

////// Lexer Rules

//TODO: Add 'in'-keyword for intervals 
//TODO: Add either 'v' keyword for OR for multiple boolean expressions 

//Always remember: Keywords first!
IN 					: 'in' ;

EQUALS				: '=' ;
BOOLRELATION		: '<=' | '>=' | '<' | '>' | '=' ;
//BINOPERATOR		: '+' | '*' ;
PLUS				: '+' ;
TIMES				: '*' ;

fragment UPPERCASE	: [A-Z] ;
fragment LOWERCASE	: [a-z] ;
fragment DIGIT		: [0-9] ;
fragment SPECIALCHAR: '+' | '-' | '\'' ;
VARIABLE			: (UPPERCASE | LOWERCASE)(UPPERCASE | LOWERCASE | DIGIT | SPECIALCHAR)* ;
NUMBER				: '-'? DIGIT+ ('.' DIGIT+)? ;
INTERVAL 			: '[' NUMBER ',' NUMBER ']' ;

WS					: (' ' | '\t' | '\n' | '\r' )+ -> skip ;


