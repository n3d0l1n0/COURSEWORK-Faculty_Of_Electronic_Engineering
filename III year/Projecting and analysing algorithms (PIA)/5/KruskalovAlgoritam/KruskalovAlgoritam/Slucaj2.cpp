#include "Slucaj2.h"
#include "Poteg.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

Slucaj2::Slucaj2(int n, int k)
    : N(n), brojPotega(0), max(n + k + 10)
{
    srand(time(0));
    poteg2 = new Poteg[max];
    
    for (int i = 0; i < N - 1; ++i)
    {
        int tezina = rand() % 100 + 1;
        dodajPoteg(i, i + 1, tezina);
    }

    int brojac = 0;
    while (brojac < k)
    {
        int u = rand() % N;
        int v = rand() % N;
        int tezina = rand() % 100 + 1;
        cout << "Slucajno generisani cvorovi (kandidati za dodatni poteg): " << u << " i " << v << endl;
        if (dodajPoteg(u, v, tezina))
            ++brojac;
    }
    oznaciObavezne();
}

Slucaj2::~Slucaj2()
{
    delete[] poteg2;
}

void Slucaj2::oznaciObavezne()
{
    int maxObaveznih = brojPotega / 100;
    if (maxObaveznih == 0 && brojPotega > 0)
        maxObaveznih = 1;

    srand(time(0));
    for (int i = 0; i < maxObaveznih; ++i)
    {
        int ind = rand() % brojPotega;
        poteg2[ind].obavezno = true;
        cout << "Obavezni poteg: (" << poteg2[ind].u << " - " << poteg2[ind].v << ") tezina " << poteg2[ind].tezina << endl;
    }
}

void Slucaj2::prosiri()
{
    max *= 2;
    Poteg* poteg3 = new Poteg[max];
    for (int i = 0; i < brojPotega; ++i)
        poteg2[i] = poteg2[i];
    delete[] poteg2;
    poteg2 = poteg3;
}

bool Slucaj2::daLiPotegPostoji(int u, int v)
{
    if (u > v)
        swap(u, v);
    for (int i = 0; i < brojPotega; ++i)
    {
        int a = poteg2[i].u;
        int b = poteg2[i].v;
        if (a > b)
            swap(a, b);
        if (a == u && b == v)
            return true;
    }
    return false;
}

bool Slucaj2::dodajPoteg(int u, int v, int tezina)
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
    if ((u == 0 && v == N - 1) || (u == N - 1 && v == 0)) 
    {
        cout << "poteg nije dodat, prvi i poslednji cvor ne smeju biti povezani" << endl;
        return false;
    }
    if (brojPotega >= max)
        prosiri();

    poteg2[brojPotega++] = { u, v, tezina };
    cout << "poteg je dodat" << endl;
    return true;
}

void Slucaj2::stampajGraf()
{
    cout << "Prikaz: cvor1 - tezina - cvor2" << endl;
    for (int i = 0; i < brojPotega; ++i)
    {
        cout << "(" << poteg2[i].u << ") --[" << poteg2[i].tezina << "]-- (" << poteg2[i].v << ")" << endl;
    }
}
