#include "Slucaj1.h"
#include "Poteg.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

Slucaj1::Slucaj1(int n, int k)
    : N(n), brojPotega(0), max(n + k + 10)
{
    srand(time(0));
    poteg = new Poteg[max];

    int centar = rand() % N;
    cout << "Nasumicno generisani centralni cvor je: " << centar << endl;

    for (int i = 0; i < N; ++i) 
    {
        if (i == centar) 
            continue;
        int tezina = rand() % 100 + 1;
        dodajPoteg(centar, i, tezina);
    }

    int brojac = 0;
    while (brojac < k) 
    {
        int u = rand() % N;
        int v = rand() % N;
        int tezina = rand() % 100 + 1;
        cout << "Slucajno generisani cvorovi (kandidati za dodavanje novog potega): " << u << " i " << v << endl;
        if (dodajPoteg(u, v, tezina))
            ++brojac;
    }
    oznaciObavezne();  

}

Slucaj1::~Slucaj1()
{
    delete[] poteg;
}

void Slucaj1::prosiri()
{
    max *= 2;
    Poteg* poteg2 = new Poteg[max];
    for (int i = 0; i < brojPotega; ++i)
        poteg2[i] = poteg[i];
    delete[] poteg;
    poteg = poteg2;
}

bool Slucaj1::daLiPotegPostoji(int u, int v)
{
    if (u > v) 
        swap(u, v);
    for (int i = 0; i < brojPotega; ++i) 
    {
        int a = poteg[i].u;
        int b = poteg[i].v;
        if (a > b) 
            swap(a, b);
        if (a == u && b == v)
            return true;
    }
    return false;
}

bool Slucaj1::dodajPoteg(int u, int v, int tezina)
{
    if (u == v) 
    { 
        cout << "poteg nije dodat, cvor ne moze da doda poteg sam sebi" << endl;
        return false; 
    }
    if (daLiPotegPostoji(u, v)) 
    { 
        cout << "poteg nije dodat, vec postoji" << endl;
        return false; 
    }

    if (brojPotega >= max)
        prosiri();

    poteg[brojPotega++] = { u, v, tezina };
    cout << "poteg je dodat" << endl;
    return true;
}

void Slucaj1::stampajGraf()
{
    cout << "Prikaz: cvor1 - tezina - cvor2" << endl;
    for (int i = 0; i < brojPotega; ++i) 
    {
        cout << "(" << poteg[i].u << ") --[" << poteg[i].tezina << "]-- (" << poteg[i].v << ")"<<endl;
    }
}

void Slucaj1::oznaciObavezne()
{
    int maxObaveznih = brojPotega / 100;
    if (maxObaveznih == 0 && brojPotega > 0)
        maxObaveznih = 1;  

    srand(time(0));
    for (int i = 0; i < maxObaveznih; ++i) 
    {
        int ind = rand() % brojPotega;
        poteg[ind].obavezno = true;
        cout << "Obavezni poteg: (" << poteg[ind].u << " - " << poteg[ind].v << ") tezina " << poteg[ind].tezina << endl;
    }
}
