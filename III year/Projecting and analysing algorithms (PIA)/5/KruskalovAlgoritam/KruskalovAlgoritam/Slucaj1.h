#pragma once
using namespace std; 
#include "Poteg.h"

class Slucaj1
{
private:
    int N;
    int brojPotega;
    int max;
    Poteg* poteg;

    void prosiri();
    bool daLiPotegPostoji(int u, int v);
    bool dodajPoteg(int u, int v, int tezina);

public:
    Slucaj1(int n, int k);
    void stampajGraf();
    ~Slucaj1();
    int getBrojPotega() const { return brojPotega; }
    Poteg* getPoteg() const { return poteg; }
    void oznaciObavezne();
};

