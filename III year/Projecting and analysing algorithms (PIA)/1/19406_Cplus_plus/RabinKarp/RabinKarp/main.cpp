#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <limits>
using namespace std;
using namespace std::chrono;

void rabinKarp(const string& T, const string& P, int d, int q) 
{
    int n = T.length();
    int m = P.length();
    int h = 1;
    int p = 0;  
    int t = 0;  
    int brojac = 0;
    int i, j;

    for (i = 0; i < m - 1; i++)
    { 
        h = (h * d) % q;
    }

    for (i = 0; i < m; i++) 
    {
        p = (d * p + P[i]) % q;
        t = (d * t + T[i]) % q;
    }

    for (j = 0; j <= n - m; j++) 
    {
        if (p == t) 
        {
            bool poklapanje = true;
            for (i = 0; i < m; i++) 
            {
                if (T[j + i] != P[i]) 
                {
                    poklapanje = false;
                    break;
                }
            }
            if (poklapanje == true)
            {
                //cout << "Pattern se pojavljuje na indeksu " << j << endl;
                //ovaj deo je zakomentarisan zbog preglednosti screenshotova za izvestaj
                brojac++;
            }
        }

        if (j < n - m)
        {
            t = (d * (t - T[j] * h) + T[j + m]) % q;
            if (t < 0)
            {
                t = t + q;
            }
        }
    }

    cout << "Broj pojavljivanja patterna u fajlu je: " << brojac << endl;
}

int main() 
{
    string T,P; 
    int d = 256;   
    int q = 103;  
    string nazivFajla;

    while (true) 
    {
        cout << "Unesi naziv fajla za pretragu stringova:" << endl;
        cin >> nazivFajla;

        if (nazivFajla.length() < 4 || nazivFajla.substr(nazivFajla.length() - 4) != ".txt") 
        {
            nazivFajla += ".txt";
        }

        ifstream file(nazivFajla.c_str());
        if (file.good()) 
        {
            file.close();
            break; 
        }
        else 
        {
            cout << "Fajl '" << nazivFajla << "' ne postoji." << endl;
        }
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    //zelimo da se osiguramo ako postoji neocekivani unos u baferu
    cout << "Unesi string za poredjenje:" << endl;
    //cin >> P; prekida unos posle prvog blanko znaka, getline je bolja alternativa
    getline(cin, P);
    int duzinaStringa = P.length();
    cout << "Izabrani string '" << P << "' ima duzinu od " << duzinaStringa << " karaktera" << endl;

    ifstream fajlZaCitanje(nazivFajla);
    if (fajlZaCitanje.is_open() == true) 
    {
        string linija;
        while (getline(fajlZaCitanje, linija)) 
        {
            T += linija;
        }
        fajlZaCitanje.close();
    }
    else 
    {
        cout << "Doslo je do greske pri otvaranju fajla '"<< nazivFajla <<"'"<< endl;
        return 1;
    }

    auto pocetak = high_resolution_clock::now();

    rabinKarp(T, P, d, q);

    auto kraj = high_resolution_clock::now();

    duration<double> protekloVreme = kraj - pocetak;

    cout << "Vreme izvrsenja za fajl '"<< nazivFajla <<"' je "<< protekloVreme.count() << " s" << endl;

    return 0;
}
