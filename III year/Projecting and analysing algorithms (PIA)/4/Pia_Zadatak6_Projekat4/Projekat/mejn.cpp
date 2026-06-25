#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono; 

struct FibNode 
{
    int kljuc;
    int deg;
    FibNode* roditelj;
    FibNode* dete;
    FibNode* levo;
    FibNode* desno;
    bool oznaka;

    FibNode(int vr) 
    {
        kljuc = vr;
        deg = 0;
        roditelj = nullptr;
        dete = nullptr;
        levo = this;
        desno = this;
        oznaka = false;
    }
};

void bubbleSort(int* niz, int n)
{
    for (int i = 0; i < n - 1; i++) 
{
        for (int j = 0; j < n - i - 1; j++)
        {
            if (niz[j] < niz[j + 1])
            {
                int tmp = niz[j];
                niz[j] = niz[j + 1];
                niz[j + 1] = tmp;
            }
        }
    }
}

struct FibonacciHeap 
{
    FibNode* min;
    int n;

    FibonacciHeap() 
    {
        min = nullptr;
        n = 0;
    }

    void concatenate(FibNode*& a, FibNode*& b) 
    {
        if (!a && b) 
        {
            a = b;
            return;
        }
        if (!b) return;

        FibNode* tmp = a->desno;
        a->desno = b->desno;
        b->desno->levo = a;
        b->desno = tmp;
        tmp->levo = b;
    }

    void insert(int value) 
    {
        FibNode* f = new FibNode(value);
        if (!min) 
        {
            min = f;
        }
        else 
        {
            concatenate(min, f);
            if (f->kljuc < min->kljuc) 
            {
                min = f;
            }
        }
        n++;
    }

    int extractMin() 
    {
        if (!min)
        {
            return -1;
        }
           
        FibNode* node = min;
        if (node) 
        {
            if (node->dete) 
            {
                FibNode* child = node->dete;
                do 
                {
                    child->roditelj = nullptr;
                    child = child->desno;
                } while (child != node->dete);
                concatenate(min, node->dete);
            }

            if (node == node->desno) 
            {
                min = nullptr;
            }
            else 
            {
                node->levo->desno = node->desno;
                node->desno->levo = node->levo;
                min = node->desno;
            }
            n--;
        }
        int minKljuc = node->kljuc;
        delete node;
        return minKljuc;
    }

    void vratiElemente(FibNode* node, FibNode* stop, int* elementi, int& indeks) 
    {
        if (!node) return;
        FibNode* trenutni = node;
        do {
            elementi[indeks++] = trenutni->kljuc;
            if (trenutni->dete) 
            {
                vratiElemente(trenutni->dete, trenutni->dete, elementi, indeks);
            }
            trenutni = trenutni->desno;
        } while (trenutni != stop);
    }

    void vratiElementeHeapa(int* elementi, int& indeks) 
    {
        if (min) vratiElemente(min, min, elementi, indeks);
    }
};

void generisiHeap(FibonacciHeap& h, int a, int b, int N, int k, int* rezultat, int& indeks) 
{
    int brojac = 0;
    while (h.n < N) 
    {
        int nasumicniBr = a + rand() % (b - a + 1);
        h.insert(nasumicniBr);
        brojac++;
        if (brojac % k == 0) 
        {
            h.extractMin();
        }
    }
    h.vratiElementeHeapa(rezultat, indeks);
}

void test(int N, int k) 
{
    srand(time(0));
    int a = 1, b = 100;

    FibonacciHeap h1, h2, h3, h4;
    const int velicina = 4 * N;
    int* rez = new int[velicina];
    int indeks = 0;

    auto pocetak = high_resolution_clock::now();

    generisiHeap(h1, a, b, N, k, rez, indeks);
    generisiHeap(h2, a, b, N, k, rez, indeks);
    generisiHeap(h3, a, b, N, k, rez, indeks);
    generisiHeap(h4, a, b, N, k, rez, indeks);

    bubbleSort(rez, velicina);

    for (int i = 0; i < velicina; i++) 
    {
        cout << rez[i] << " ";
    }
    cout << endl;

    auto kraj = high_resolution_clock::now();
    duration<double> vreme = kraj - pocetak;

    cout << "Za N: " << N << ", i k: " << k << "Vreme izvrsavanja: " << vreme.count() << " s" << endl;

    delete[] rez;
}

int main() 
{
    for (int N = 1000; N <= 10000000; N *= 10) 
    {
        for (int k = 10; k <= 100; k += 30) 
        { 
            test(N, k);
        }
    }

    return 0;
}
