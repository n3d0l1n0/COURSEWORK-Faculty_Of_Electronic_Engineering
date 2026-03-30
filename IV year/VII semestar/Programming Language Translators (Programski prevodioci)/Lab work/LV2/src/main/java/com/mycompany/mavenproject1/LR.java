/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.mavenproject1;

/**
 *
 * @author nstoj
 */
import java.io.IOException;
import java.util.*;
class Smena {
    int levaStrana;
    int duzina;
    public Smena(int l, int d) { //za R
    	levaStrana = l; 
    	duzina = d; 
    }
}

public class LR {
	HashMap<Integer, Integer> mapaKolone = new HashMap<>();
	String[][] tabela = new String[19][14];
    Smena[] smene = new Smena[8];

    public LR() {
        
        mapaKolone.put(sym.SELECT, 1);
        mapaKolone.put(sym.BEGIN, 2);
        mapaKolone.put(sym.END, 3);
        mapaKolone.put(sym.CASE, 4);
        mapaKolone.put(sym.ID, 5);
        mapaKolone.put(sym.IMPLICATION, 6); // =>
        mapaKolone.put(sym.ASSIGN, 7);      // :=
        mapaKolone.put(sym.SEMICOLON, 0);   // ;
        mapaKolone.put(sym.CONST, 8);
        mapaKolone.put(sym.EOF, 9);
        mapaKolone.put(100, 10); // S
        mapaKolone.put(101, 11); // CL
        mapaKolone.put(102, 12); // C
        mapaKolone.put(103, 13); // St

        //pravila
        smene[1] = new Smena(100, 4);
        smene[2] = new Smena(101, 2);   
        smene[3] = new Smena(101, 1);
        smene[4] = new Smena(102, 4);   
        smene[5] = new Smena(103, 1);   
        smene[6] = new Smena(103, 4);
        smene[7] = new Smena(103, 4);

        inicijalizujTabelu();
    }

    public int SA_LR(MPLexer lexer) {
    	Stack<Object> stek = new Stack<>();
    	stek.clear();
        stek.push(0);
        int prepoznat = 0;
        int greska = 0;
        
        Yytoken next;
        try {
            next = lexer.next_token();
        } catch (IOException e) { return 0; }

        do {
            int currentState = (Integer) stek.peek();
            int token = mapirajToken(next);
            int kolona = mapaKolone.get(token);
            String action = tabela[currentState][kolona];

            char tip = action.charAt(0);
            if (Character.isDigit(tip)) tip = 'g'; // g kao goto

            switch (tip) {
                case 's': // sk
                    int k_shift = Integer.parseInt(action.substring(1));
                    stek.push(token);
                    stek.push(k_shift);
                    try { next = lexer.next_token(); } catch (IOException e) { greska = 1; }
                    break;

                case 'r': // rk
                    int k_rule = Integer.parseInt(action.substring(1));
                    int popCount = 2 * smene[k_rule].duzina;
                    for (int i = 0; i < popCount; i++) stek.pop();
                    
                    int stateAfterPop = (Integer) stek.peek();
                    int leftSide = smene[k_rule].levaStrana;
                    
                    stek.push(leftSide);
                    int sledeceStanje = Integer.parseInt(tabela[stateAfterPop][mapaKolone.get(leftSide)]);
                    stek.push(sledeceStanje);
                    break;

                case 'a': // acc
                    prepoznat = 1;
                    break;

                case 'e': // err
                default:
                    greska = 1;
                    break;
            }
        } while (!(prepoznat == 1 || greska == 1));

        return prepoznat;
    }
    private void inicijalizujTabelu() {
        for (int i = 0; i < 19; i++) 
        	Arrays.fill(tabela[i], " ");

        tabela[0][1] = "s2";
        tabela[0][10] = "1";
        tabela[1][9] = "acc";
        tabela[2][2] = "s3";
        tabela[3][4] = "s6";   
        tabela[3][11] = "4";   
        tabela[3][12] = "5";
        tabela[4][3] = "s7";   
        tabela[4][4] = "s6";   
        tabela[4][12] = "8";
        tabela[5][3] = "r3";   
        tabela[5][4] = "r3";
        tabela[6][5] = "s9";
        tabela[7][3] = "r1";   
        tabela[7][4] = "r1";   
        tabela[7][9] = "r1";
        tabela[8][3] = "r2";   
        tabela[8][4] = "r2";
        tabela[9][6] = "s10";
        tabela[10][1] = "s2";  
        tabela[10][5] = "s13"; 
        tabela[10][10] = "12"; 
        tabela[10][13] = "11";
        tabela[11][3] = "r4";  
        tabela[11][4] = "r4";
        tabela[12][3] = "r5";  
        tabela[12][4] = "r5";
        tabela[13][7] = "s14";
        tabela[14][5] = "s15"; 
        tabela[14][8] = "s16";
        tabela[15][0] = "s17";
        tabela[16][0] = "s18";
        tabela[17][3] = "r6";  
        tabela[17][4] = "r6";
        tabela[18][3] = "r7";  
        tabela[18][4] = "r7";
    }

    private int mapirajToken(Yytoken t) {
        if (t == null) {
            return sym.EOF; 
        }
        return t.m_index;
    }
    public static void main(String[] args) {
        try {
            java.io.FileReader fajl = new java.io.FileReader(args[0]);
            MPLexer lexer = new MPLexer(fajl);
            
            LR sa = new LR();
            int rez = sa.SA_LR(lexer);
            
            if (rez == 1) System.out.println("Ulazni niz je prepoznat");
            else System.out.println("Postoji sintaksna greska u ulaznom nizu");
            
        } catch (Exception e) {
            System.out.println("Greska " + e.getMessage());
        }
    }
}
