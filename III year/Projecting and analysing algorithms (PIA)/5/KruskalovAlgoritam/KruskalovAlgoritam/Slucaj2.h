#pragma once
using namespace std;
#include "Poteg.h"

class Slucaj2
{
private:
    int N;
    int brojPotega;
    int max;
    Poteg* poteg2;

    void prosiri();
    bool daLiPotegPostoji(int u, int v);
    bool dodajPoteg(int u, int v, int tezina);

public:
    Slucaj2(int n, int k);
    void stampajGraf();
    ~Slucaj2();
    int getBrojPotega() const { return brojPotega; }
    Poteg* getPoteg() const { return poteg2; }
    void oznaciObavezne();
};


