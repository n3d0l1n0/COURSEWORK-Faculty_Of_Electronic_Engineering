package com.mycompany.mavenproject1;

// import sekcija
%%
// sekcija opcija i deklaracija
%class MPLexer
%standalone
%function next_token
%line
%column
//%debug
%type Yytoken

%eofval{
    return new Yytoken(sym.EOF, "EOF", yyline, yycolumn);
%eofval}

%{
    private KWTable kwTable = new KWTable();
    private Yytoken getKW() {
        return new Yytoken(kwTable.find(yytext()), yytext(), yyline, yycolumn);
    }
%}

//stanja
%xstate COMMENT

//makroi
line_terminator = \r|\n|\r\n
whitespace = {line_terminator} | [ \t\f]

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
    "|*"                { yybegin(COMMENT); }

    {whitespace}        { /* Ignorisi */ }

    // separatori
    "("                 { return new Yytoken(sym.LEFTPAR, yytext(), yyline, yycolumn); }
    ")"                 { return new Yytoken(sym.RIGHTPAR, yytext(), yyline, yycolumn); }
    ";"                 { return new Yytoken(sym.SEMICOLON, yytext(), yyline, yycolumn); }
    ":"                 { return new Yytoken(sym.COLON, yytext(), yyline, yycolumn); }
    ","                 { return new Yytoken(sym.COMMA, yytext(), yyline, yycolumn); }
    "."                 { return new Yytoken(sym.DOT, yytext(), yyline, yycolumn); }

    // operator
    ":="                { return new Yytoken(sym.ASSIGN, yytext(), yyline, yycolumn); }
    "=>"                { return new Yytoken(sym.ARROW, yytext(), yyline, yycolumn); }
    "<="                { return new Yytoken(sym.LE, yytext(), yyline, yycolumn); }
    ">="                { return new Yytoken(sym.GE, yytext(), yyline, yycolumn); }
    "=="                { return new Yytoken(sym.EQ, yytext(), yyline, yycolumn); }
    "<>"                { return new Yytoken(sym.NE, yytext(), yyline, yycolumn); }
    "<"                 { return new Yytoken(sym.LT, yytext(), yyline, yycolumn); }
    ">"                 { return new Yytoken(sym.GT, yytext(), yyline, yycolumn); }

    //konstante
    {real_const}        { return new Yytoken(sym.CONST, yytext(), yyline, yycolumn); }
    {hex_const}         { return new Yytoken(sym.CONST, yytext(), yyline, yycolumn); }
    {octal_const}       { return new Yytoken(sym.CONST, yytext(), yyline, yycolumn); }
    {decimal_const}     { return new Yytoken(sym.CONST, yytext(), yyline, yycolumn); }
    {char_const}        { return new Yytoken(sym.CONST, yytext(), yyline, yycolumn); }
    {boolean_const}     { return new Yytoken(sym.CONST, yytext(), yyline, yycolumn); }

    // identifikatori i kljucne reci
    {ident}             { return getKW(); }

    // obrada gresaka
    .                   { System.out.println("nepoznat karakter na: '" + yytext() + "' na liniji " + (yyline+1) + ", koloni " + (yycolumn+1)); }
}

// comment
<COMMENT> {
    "*|"                { yybegin(YYINITIAL); }
    [^*]                { /* Ignorisi */ }
    "*"                 { /* Ignorisi */ }
    <<EOF>>             { System.out.println("greska"); return new Yytoken(sym.EOF, "EOF", yyline, yycolumn); }
}