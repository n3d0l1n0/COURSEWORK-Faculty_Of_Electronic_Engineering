package com.mycompany.noerrorcup;

// import sekcija
import java_cup.runtime.*;

%%
// sekcija opcija i deklaracija
%class MPLexer
%cup
//%implements java_cup.runtime.Scanner
//%type java_cup.runtime.Symbol 
//%function next_token
%line
%column

%eofval{
    return new Symbol(sym.EOF);
%eofval}

%{
    public int getLine()
   {
      return yyline;
   }
%}

//stanja
%xstate COMMENT

//makroi
line_terminator = \r|\n|\r\n
whitespace = {line_terminator} | [ \t]

// identifikatori - niz slova, cifara i $, prvi znak ne moze biti cifra
ident_start = [a-zA-Z$]
ident_part = [a-zA-Z0-9$]
ident = {ident_start}{ident_part}*

// konstante
digit = [0-9]
digits = {digit}+
heksa_digit = [0-9a-fA-F]
oktal_digit = [0-7]
decimal_const = {digits}
octal_const = 0{oktal_digit}+
hex_const = 0[xX]{heksa_digit}+
real_const = {digits}\.{digits}?([Ee][\+\-]?{digits})? | \.{digits}([Ee][\+\-]?{digits})?
char_const = \'[^\']\'
boolean_const = true | false

%%

//pravila
<YYINITIAL> {
    {whitespace}+       { /* ignorisi */ }
    "|*"                { yybegin(COMMENT); }

    // separatori
    "("                 { return new Symbol(sym.LEFTPAR, yyline, yycolumn, yytext()); }
    ")"                 { return new Symbol(sym.RIGHTPAR, yyline, yycolumn, yytext()); }
    ";"                 { return new Symbol(sym.SEMICOLON, yyline, yycolumn, yytext()); }
    ":"                 { return new Symbol(sym.COLON, yyline, yycolumn, yytext()); }
    ","                 { return new Symbol(sym.COMMA, yyline, yycolumn, yytext()); }
    "."                 { return new Symbol(sym.DOT, yyline, yycolumn, yytext()); }

    // operator
    ":="                { return new Symbol(sym.ASSIGN, yyline, yycolumn, yytext()); }
    "=>"                { return new Symbol(sym.ARROW, yyline, yycolumn, yytext()); }
    "<="                { return new Symbol(sym.LE, yyline, yycolumn, yytext()); }
    ">="                { return new Symbol(sym.GE, yyline, yycolumn, yytext()); }
    "=="                { return new Symbol(sym.EQ, yyline, yycolumn, yytext()); }
    "<>"                { return new Symbol(sym.NE, yyline, yycolumn, yytext()); }
    "<"                 { return new Symbol(sym.LT, yyline, yycolumn, yytext()); }
    ">"                 { return new Symbol(sym.GT, yyline, yycolumn, yytext()); }

    //konstante
    {real_const}        { return new Symbol(sym.CONST, yyline, yycolumn, yytext()); }
    {hex_const}         { return new Symbol(sym.CONST, yyline, yycolumn, yytext()); }
    {octal_const}       { return new Symbol(sym.CONST, yyline, yycolumn, yytext()); }
    {decimal_const}     { return new Symbol(sym.CONST, yyline, yycolumn, yytext()); }
    {char_const}        { return new Symbol(sym.CONST, yyline, yycolumn, yytext()); }
    {boolean_const}     { return new Symbol(sym.CONST, yyline, yycolumn, yytext()); }

    //kljucne reci
   "program"		{ return new Symbol( sym.PROGRAM );	}	
   "begin"			{ return new Symbol( sym.BEGIN ); }
   "end"			{ return new Symbol( sym.END );	}
   "select"			{ return new Symbol( sym.SELECT );	}
   "case"			{ return new Symbol( sym.CASE );	}
   "or"			{ return new Symbol( sym.OR );	}
   "and"			{ return new Symbol( sym.AND );	}
   "real"			{ return new Symbol( sym.REAL );	}
   "boolean"			{ return new Symbol( sym.BOOLEAN );	}
   "integer"		{ return new Symbol( sym.INTEGER );	}
   "char"			{ return new Symbol( sym.CHAR );	}

   //identifikatori
  {ident}    { return new Symbol(sym.ID, yytext()); }
   
   // obrada gresaka
    .                   {
        System.out.println(
            "nepoznat karakter na: '" + yytext() +
            "' na liniji " + (yyline+1) +
            ", koloni " + (yycolumn+1)
        );
    }
}

// comment
<COMMENT> {
    "*|"                { yybegin(YYINITIAL); }
    [^*]                { /* Ignorisi */ }
    "*"                 { /* Ignorisi */ }
    <<EOF>>             {
        System.out.println("greska");
        return new Symbol(sym.EOF);
    }
}
