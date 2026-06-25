#include <iostream>
#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;

void CountingSort(int A[], int B[], int n, int k)
{
    int* C = new int[k + 1];

    for (int i = 0; i <= k; i++)
    {
        C[i] = 0;
    }

    for (int j = 0; j < n; j++)
    {
        C[A[j]] += 1;
    }

    for (int i = 1; i <= k; i++)
    {
        C[i] += C[i - 1];
    }

    for (int j = n - 1; j >= 0; j--)
    {
        B[C[A[j]] - 1] = A[j];
        C[A[j]] -= 1;
    }

    delete[] C;
}

int heap_size;

void Swap(int A[], int i, int j) 
{
    int tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

int Left(int i) 
{
    return 2 * i + 1; 
}

int Right(int i) 
{
    return 2 * i + 2; 
}

void Heapify(int A[], int i) 
{
    int l = Left(i);
    int r = Right(i);
    int largest = i;

    if (l < heap_size && A[l] > A[i]) 
    {
        largest = l;
    }

    if (r < heap_size && A[r] > A[largest]) 
    {
        largest = r;
    }

    if (largest != i) 
    {
        Swap(A, i, largest);
        Heapify(A, largest);
    }
}

void BuildHeap(int A[], int l) 
{
    heap_size = l;
    for (int i = (l / 2) - 1; i >= 0; i--) 
    {
        Heapify(A, i);
    }
}

void HeapSort(int A[], int l) 
{
    BuildHeap(A, l);
    for (int i = l - 1; i >= 1; i--) 
    {
        Swap(A, 0, i);
        heap_size--;
        Heapify(A, 0);
    }
}

void SelectionSort(int A[], int l)
{
    for (int i = 0; i < l - 1; i++)
    {
        int min = i; 

        for (int j = i + 1; j < l; j++)
        {
            if (A[j] < A[min])
            {
                min = j;
            }
        }

        if (min != i)
        {
            Swap(A, i, min);
        }
    }
}

void GenerisiNasumicneBrojeve(int* A, int l)
{
    static random_device uredjaj; 
    static mt19937 generator(uredjaj()); 
    static uniform_int_distribution<int> distribucija(0, 10000); 

    for (int i = 0; i < l; ++i) 
    {
        A[i] = distribucija(generator);
    }
}

int NadjiNajmanjuRazliku(int Fabrika[], int N, int M)
{
    if (M > N)
    {
        return -1; 
    }

    int min_razlika = Fabrika[M - 1] - Fabrika[0];

    for (int i = 1; i + M - 1 < N; i++)
    {
        int tr_razlika = Fabrika[i + M - 1] - Fabrika[i];
        if (tr_razlika < min_razlika)
        {
            min_razlika = tr_razlika;
        }
    }

    return min_razlika;
}

int main()
{
    int N; 
    for (int p = 0; p < 6; p++) //unos za 100, 1000, 10k, 100k, 1M, 10M
    {
        cout << "Unesite broj paketa: ";
        cin >> N;
        cout << "Broj paketa je:"<< N << endl;

        if (N <= 0)
        {
            cout << "Broj paketa mora biti pozitivan." << endl;
            return 0;
        }

        int* Fabrika = new int[N];

        GenerisiNasumicneBrojeve(Fabrika, N);
        
        int M = (3 * N + 9) / 10;
        if (M == 0)
        {
            M = 1;
        }

        if (M > N)
        {
            cout << "Broj studenata ne moze biti veci od broja paketa" << endl;
            delete[] Fabrika;
            return 0;
        }
        cout << "---------------------------SelectionSort-------------------------" << endl;
        auto pocetak = high_resolution_clock::now();

        SelectionSort(Fabrika, N);
        /*
        for (int i = 0; i < N; ++i)
        {
            cout << Fabrika[i] << " ";
        }
        cout << endl;
        */
       
        auto kraj = high_resolution_clock::now();

        duration<double> protekloVreme = kraj - pocetak;

        cout << "Vreme izvrsenja SelectionSort-a je " << protekloVreme.count() << " s" << endl;

        int razlika = NadjiNajmanjuRazliku(Fabrika, N, M);

        if (razlika == -1)
        {
            cout << "Greska" << endl;
        }
        else
        {
            cout << "Minimalna razlika je: " << razlika << endl;
        }
        cout << "---------------------------CountingSort-------------------------" << endl;
        int* PomocniNiz = new int[N];

        GenerisiNasumicneBrojeve(Fabrika, N);
        
        auto pocetak1 = high_resolution_clock::now();

        CountingSort(Fabrika, PomocniNiz, N, 10000);
        /*
        for (int i = 0; i < N; ++i)
        {
            cout << PomocniNiz[i] << " ";
        }
        cout << endl;
        */
        
        auto kraj1 = high_resolution_clock::now();

        duration<double> protekloVreme1 = kraj1 - pocetak1;

        cout << "Vreme izvrsenja CountingSort-a je " << protekloVreme1.count() << " s" << endl;

        int razlika1 = NadjiNajmanjuRazliku(PomocniNiz, N, M);

        if (razlika1 == -1)
        {
            cout << "Greska" << endl;
        }
        else
        {
            cout << "Minimalna razlika je: " << razlika1 << endl;
        }

        delete[] PomocniNiz;

        cout << "-----------------------------HeapSort---------------------------" << endl;
        
        GenerisiNasumicneBrojeve(Fabrika, N);

        auto pocetak2 = high_resolution_clock::now();

        HeapSort(Fabrika, N);
        /*
        for (int i = 0; i < N; ++i)
        {
            cout << Fabrika[i] << " ";
        }
        cout << endl;
        */
        
        auto kraj2 = high_resolution_clock::now();

        duration<double> protekloVreme2 = kraj2 - pocetak2;

        cout << "Vreme izvrsenja HeapSort-a je " << protekloVreme2.count() << " s" << endl;

        int razlika2 = NadjiNajmanjuRazliku(Fabrika, N, M);

        if (razlika2 == -1)
        {
            cout << "Greska" << endl;
        }
        else
        {
            cout << "Minimalna razlika je: " << razlika2 << endl;
        }

        delete[] Fabrika; 
    }

	return 0;
}
