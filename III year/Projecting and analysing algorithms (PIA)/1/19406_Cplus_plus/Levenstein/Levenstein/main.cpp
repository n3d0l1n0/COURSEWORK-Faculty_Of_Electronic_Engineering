#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <limits>

using namespace std;
using namespace std::chrono;

int minimum(int a, int b, int c)
{
    return min(a, min(b, c));
}

int levenstein(const string& s, const string& t) 
{
    int m = s.length();
    int n = t.length();

    int** d = new int* [m + 1];
    for (int i = 0; i <= m; ++i)
    {
        d[i] = new int[n + 1];
    }

    for (int i = 0; i <= m; ++i)
    {
        d[i][0] = i;
    }
    for (int j = 0; j <= n; ++j)
    {
        d[0][j] = j;
    }

    for (int j = 1; j <= n; ++j)
    {
        for (int i = 1; i <= m; ++i)
        {
            int cena;
            if (s[i - 1] == t[j - 1])
            {
                cena = 0;
            }
            else
            {
                cena = 1;
            }
            d[i][j] = minimum(d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + cena);
        }
    }

    int udaljenost = d[m][n];

    for (int i = 0; i <= m; ++i)
    {
        delete[] d[i];
    }
    delete[] d;

    return udaljenost;
}

void primeniLevensteina(const string& tekst, const string& pattern)
{
    double duzinaPatterna = pattern.length();
    double min = duzinaPatterna * 0.8; 
    double max = duzinaPatterna * 1.2; 

    if (min == 0) 
    {
        min = 1;
    }

    int maxUdaljenost = (int)(duzinaPatterna * 0.2);

    for (double i = 0; i <= tekst.length() - min; ++i)
    {
        for (double j = min; j <= max; ++j)
        {
            if (i + j > tekst.length())
            {
                break;
            }

            string podstring = tekst.substr(i, j);

            int udaljenost = levenstein(pattern, podstring);

            if (udaljenost > 0 && udaljenost < maxUdaljenost)
            {
                cout << "Pronadjen string: '" << podstring << "' sa udaljenoscu " << udaljenost << endl;
            }

            //cout << "Udaljenost je:" << udaljenost << " ,ali ona nije u zeljenom opsegu" << endl;
        }
    }
}


int main()
{

    string nazivFajla;
 
    cout << "Unesi naziv fajla za pretragu stringova:" << endl;
    cin >> nazivFajla;
    if (nazivFajla.length() < 4 || nazivFajla.substr(nazivFajla.length() - 4) != ".txt")
    {
        nazivFajla += ".txt";
    }

    ifstream fajlZaCitanje(nazivFajla.c_str());
    if (!fajlZaCitanje.good())
    {
        cout << "Fajl '" << nazivFajla << "' ne postoji." << endl;
        return 1;
    }
    fajlZaCitanje.close();

    string tekst;
    ifstream fajl(nazivFajla);
    if (fajl.is_open())
    {
        string linija;
        while (getline(fajl, linija))
        {
            tekst += linija;
        }
        fajl.close();
    }
    else
    {
        cout << "Doslo je do greske pri otvaranju fajla '" << nazivFajla << "'" << endl;
        return 1;
    }

    for (int p=0; p<3; p++) //ponavljamo unos 3 puta za 3 razlicite duzine podstringa
    {
        string pattern;
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Unesi string za poredjenje  "<< endl;
        getline(cin, pattern);
        cout << "Izabrani string '" << pattern << "' ima duzinu od " << pattern.length() << " karaktera" << endl;
      
        auto pocetak = high_resolution_clock::now();

        primeniLevensteina(tekst, pattern);

        auto kraj = high_resolution_clock::now();

        duration<double> protekloVreme = kraj - pocetak;

        cout << "Vreme izvrsenja za fajl '" << nazivFajla <<" je " << protekloVreme.count() << " s" << endl;
        
    }

    return 0;
}
