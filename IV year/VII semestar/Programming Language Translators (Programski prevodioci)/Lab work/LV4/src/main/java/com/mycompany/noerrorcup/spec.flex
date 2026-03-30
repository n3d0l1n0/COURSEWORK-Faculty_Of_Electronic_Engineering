package com.mycompany.noerrorcup;

import java_cup.runtime.*;

%%

%class MPLexer
//%implements java_cup.runtime.Scanner
%unicode
%cup
%line
%column

%{
    private Symbol symbol(int type) {
        return new Symbol(type, yyline, yycolumn);
    }
    private Symbol symbol(int type, Object value) {
        return new Symbol(type, yyline, yycolumn, value);
    }
%}

digit        = [0-9]
digits       = {digit}+
ident_start  = [a-zA-Z$]
ident_part   = [a-zA-Z0-9$]
ident        = {ident_start}{ident_part}*

%%

[ \t\r\n]+       { /* ignorisi */ }

"program"        { return symbol(sym.PROGRAM); }
"begin"          { return symbol(sym.BEGIN); }
"end"            { return symbol(sym.END); }
"integer"        { return symbol(sym.INTEGER); }
"real"           { return symbol(sym.REAL); }
"char"           { return symbol(sym.CHAR); }
"boolean"        { return symbol(sym.BOOLEAN); }
"select"         { return symbol(sym.SELECT); }
"case"           { return symbol(sym.CASE); }
"or"             { return symbol(sym.OR); }
"and"            { return symbol(sym.AND); }

":="             { return symbol(sym.ASSIGN); }
"=>"             { return symbol(sym.ARROW); }
"<="             { return symbol(sym.LE); }
">="             { return symbol(sym.GE); }
"=="             { return symbol(sym.EQ); }
"<>"             { return symbol(sym.NE); }
"<"              { return symbol(sym.LT); }
">"              { return symbol(sym.GT); }

"("              { return symbol(sym.LEFTPAR); }
")"              { return symbol(sym.RIGHTPAR); }
";"              { return symbol(sym.SEMICOLON); }
":"              { return symbol(sym.COLON); }
","              { return symbol(sym.COMMA); }
"."              { return symbol(sym.DOT); }

"true"|"false"   { return symbol(sym.CONST, yytext()); }
{digits}"."{digits}([Ee][+-]?{digits})?  { return symbol(sym.CONST, yytext()); }
{digits}         { return symbol(sym.CONST, yytext()); }
'[^']'           { return symbol(sym.CONST, yytext()); }
{ident}          { return symbol(sym.ID, yytext()); }

.                { System.err.println("Nepoznat karakter: '"+yytext()+"' linija "+yyline); }