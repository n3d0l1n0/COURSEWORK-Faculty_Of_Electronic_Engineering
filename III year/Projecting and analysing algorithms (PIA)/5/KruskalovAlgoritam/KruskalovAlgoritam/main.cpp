#include <iostream>
#include "Slucaj1.h"
#include "Slucaj2.h"
#include "Poteg.h"
using namespace std;

void MakeSet(int* roditeljCvora, int* visinaStabla, int n) 
{
    for (int i = 0; i < n; ++i) 
    {
        roditeljCvora[i] = i;
        visinaStabla[i] = 0;
    }
}

int FindSet(int* roditeljCvora, int x) 
{
    if (roditeljCvora[x] != x)
        roditeljCvora[x] = FindSet(roditeljCvora, roditeljCvora[x]);
    return roditeljCvora[x];
}

void Union(int* roditeljCvora, int* visinaStabla, int x, int y) 
{
    x = FindSet(roditeljCvora, x);
    y = FindSet(roditeljCvora, y);
    if (x != y) 
    {
        if (visinaStabla[x] < visinaStabla[y])
            roditeljCvora[x] = y;
        else 
        {
            roditeljCvora[y] = x;
            if (visinaStabla[x] == visinaStabla[y])
                visinaStabla[x]++;
        }
    }
}

void sortiraj(Poteg* p, int broj) 
{
    for (int i = 0; i < broj - 1; ++i) 
    {
        for (int j = 0; j < broj - i - 1; ++j) 
        {
            if (p[j].tezina > p[j + 1].tezina) 
            {
                Poteg temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

Poteg* KurskalovAlgoritamModifikovano(int n, Poteg* p, int brojPotega, int& brojRezultata) 
{
    int* roditeljCvora = new int[n];
    int* visinaStabla = new int[n];
    MakeSet(roditeljCvora, visinaStabla, n);

    Poteg* T = new Poteg[n - 1];  
    brojRezultata = 0;

    for (int i = 0; i < brojPotega; ++i) 
    {
        if (p[i].obavezno) 
        {
            int uSet = FindSet(roditeljCvora, p[i].u);
            int vSet = FindSet(roditeljCvora, p[i].v);
            if (uSet != vSet) 
            {
                Union(roditeljCvora, visinaStabla, uSet, vSet);
                T[brojRezultata++] = p[i];
            }
            else 
            {
                
                delete[] T;
                delete[] roditeljCvora;
                delete[] visinaStabla;
                brojRezultata = 0;
                return nullptr;
            }
        }
    }

    sortiraj(p, brojPotega);

    for (int i = 0; i < brojPotega; ++i) 
    {
        if (!p[i].obavezno) 
        {
            int uSet = FindSet(roditeljCvora, p[i].u);
            int vSet = FindSet(roditeljCvora, p[i].v);
            if (uSet != vSet) 
            {
                Union(roditeljCvora, visinaStabla, uSet, vSet);
                T[brojRezultata++] = p[i];
            }
        }
    }

    delete[] roditeljCvora;
    delete[] visinaStabla;

    if (brojRezultata != n - 1) 
    {
        cout << "Nije povezan graf ili nije moguce formirati minimalno sprezno stablo." << endl;
        delete[] T;
        brojRezultata = 0;
        return nullptr;
    }

    return T;
}

int main() 
{
    /*N = 100, 1000, 10k, 100k 
    k = 2*N, 5*N, 10*N, 20*N, 33*N, 50*N */

    //u slucaju kada je k=50*N, a N=100 program ulazi u beskonacnu petlju jer pokusava da doda 50*N dodatnih grana
    //u grafu sa N (100) cvorova vec postoji N-1 (99) potega, kada program pokusa da doda ovih 50*N to ne uspeva da 
    //uradi jer je max broj grana potpunog grafa u ovom slucaju n*(n-1)/2 = 4950, a to je manje od 50*N = 5000 grana 
    //iz ovog razloga je dodata obavezna provera u kojoj k mora da bude manje ili jednako od n*(n-1)/2
    //kako bi se program izvrsio u svim slucajevima u kojima je to moguce 

    int N;
    int k;
    int br[6] = {2, 5, 10, 20, 33, 50};
    for (N = 100; N <= 100000; N *= 10)
    {
        for (int i = 0; i < 6; i++)
        {
            k = br[i] * N;
            if (k <= (N * (N - 1) / 2))
            {
                cout << "Broj cvorova: " << N << endl;
                cout << "Broj dodatih potega: " << k << endl;
                cout << "------------------------------------Slucaj1--------------------------------------------" << endl;

                Slucaj1 graf(N, k);
                graf.stampajGraf();

                int brojPotega = graf.getBrojPotega();
                Poteg* potegovi = graf.getPoteg();

                int brojRezultata = 0;
                Poteg* mst = KurskalovAlgoritamModifikovano(N, potegovi, brojPotega, brojRezultata);

                if (mst != nullptr)
                {
                    cout << "\nMinimalno sprezno stablo:\n";
                    for (int i = 0; i < brojRezultata; ++i)
                    {
                        cout << mst[i].u << " - " << mst[i].v << " [" << mst[i].tezina << "]";
                        if (mst[i].obavezno) cout << " *obavezno*";
                        cout << endl;
                    }
                    delete[] mst;
                }
                else
                {
                    cout << "Nije moguce naci minimalno sprezno stablo." << endl;
                }

                cout << "------------------------------------Slucaj2--------------------------------------------" << endl;
                Slucaj2 graf2(N, k);
                graf2.stampajGraf();

                int brojPotega2 = graf2.getBrojPotega();
                Poteg* potegovi2 = graf2.getPoteg();

                int brojRezultata2 = 0;
                Poteg* mst2 = KurskalovAlgoritamModifikovano(N, potegovi2, brojPotega2, brojRezultata2);

                if (mst2 != nullptr)
                {
                    cout << "\nMinimalno sprezno stablo:\n";
                    for (int i = 0; i < brojRezultata2; ++i)
                    {
                        cout << mst2[i].u << " - " << mst2[i].v << " [" << mst2[i].tezina << "]";
                        if (mst2[i].obavezno) cout << " *obavezno*";
                        cout << endl;
                    }
                    delete[] mst2;
                }
                else
                {
                    cout << "Nije moguce naci minimalno sprezno stablo." << endl;
                }
            }
            else
            {
                cout << "Nije moguce dodati " << k << " potega jer je maksmimalan broj potega za " << N << " cvorova " << (N * (N - 1) / 2) << endl;
            }
            
        }
    }
   
    return 0;
}
