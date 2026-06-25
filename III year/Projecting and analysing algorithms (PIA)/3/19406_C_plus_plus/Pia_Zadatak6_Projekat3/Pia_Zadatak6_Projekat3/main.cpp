#include <iostream>
using namespace std;

struct Dan 
{
    int cena_akcije;
    int max_akcija;
};

void zameni(Dan& a, Dan& b) 
{
    Dan tmp = a;
    a = b;
    b = tmp;
}

int quickSortPom(Dan dan[], int levo, int desno) 
{
    int poslednji = dan[desno].cena_akcije; 
    int i = levo - 1; 

    for (int j = levo; j < desno; j++)
    {
        if (dan[j].cena_akcije <= poslednji) 
        {
            i++;
            zameni(dan[i], dan[j]);
        }
    }
    zameni(dan[i + 1], dan[desno]);
    return i + 1;
}

void quickSort(Dan dan[], int levo, int desno) 
{
    if (levo < desno) 
    {
        int posIndeks = quickSortPom(dan, levo, desno);
        quickSort(dan, levo, posIndeks - 1); 
        quickSort(dan, posIndeks + 1, desno); 
    }
}

int brojKupljenihAkcija(int cena[], int n, int k) 
{
    Dan* dani = new Dan[n];
    for (int i = 0; i < n; i++) 
    {
        dani[i].cena_akcije = cena[i];
        dani[i].max_akcija = i + 1; 
    }

    quickSort(dani, 0, n - 1);

    int brojAkcija = 0;

    for (int i = 0; i < n; i++) 
    {
        if (k < dani[i].cena_akcije) 
        {
            continue; 
        }
        int moguce_akcije = k / dani[i].cena_akcije;
        if (moguce_akcije > dani[i].max_akcija) 
        {
            moguce_akcije = dani[i].max_akcija;
        }
        brojAkcija += moguce_akcije;
        k -= moguce_akcije * dani[i].cena_akcije;
    }

    delete[] dani; 
    return brojAkcija;
}

int main() 
{
    int cena1[] = { 10, 7, 19 };
    int n1 = 3;
    int k1 = 45;
    cout << "Primer 1:\nBroj kupljenih akcija: " << brojKupljenihAkcija(cena1, n1, k1) << endl;

    int cena2[] = { 10, 7, 4 };
    int n2 = 3;
    int k2 = 100;
    cout << "Primer 2:\nBroj kupljenih akcija: " << brojKupljenihAkcija(cena2, n2, k2) << endl;

    return 0;
}
