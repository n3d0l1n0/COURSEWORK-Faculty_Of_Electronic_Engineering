#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath> // za log2 i floor
using namespace std;

#define INF 2147483647

struct Node {
    int key;
    int degree;
    Node* parent;
    Node* child;
    Node* left;
    Node* right;
    bool mark;

    Node(int value) : key(value), degree(0), parent(nullptr), child(nullptr), left(this), right(this), mark(false) {}
};

struct FibonacciHeap {
    Node* min;
    int n;

    FibonacciHeap() : min(nullptr), n(0) {}

    void insert(int value) {
        Node* newNode = new Node(value);
        if (!min) {
            min = newNode;
        }
        else {
            mergeNodes(min, newNode);
            if (value < min->key) {
                min = newNode;
            }
        }
        n++;
    }

    int extractMin() {
        if (!min) return INF;
        Node* z = min;
        if (z->child) {
            Node* startChild = z->child;
            Node* child = z->child;
            do {
                Node* next = child->right;
                // Uklanjamo dete iz liste dece i dodajemo u korenski nivo
                mergeNodes(min, child);
                child->parent = nullptr;
                child = next;
            } while (child != startChild);
        }

        removeNode(z);
        int minValue = z->key;
        if (z == z->right) {
            min = nullptr;
        }
        else {
            min = z->right;
            consolidate();
        }
        n--;
        delete z;
        return minValue;
    }

    void merge(FibonacciHeap* other) 
    {
        if (!other->min) return;
        if (!min) 
        {
            min = other->min;
        }
        else {
            mergeNodes(min, other->min);
            if (other->min->key < min->key) 
            {
                min = other->min;
            }
        }
        n += other->n;
    }

    void mergeNodes(Node* a, Node* b) 
    {
        a->left->right = b->right;
        b->right->left = a->left;
        a->left = b;
        b->right = a;
    }

    void removeNode(Node* node) {
        node->left->right = node->right;
        node->right->left = node->left;
    }

    void consolidate() {
        if (!min || n == 0) return;
        int maxDegree = (int)floor(log2((double)n)) + 50;
        Node** A = new Node * [maxDegree];
        for (int i = 0; i < maxDegree; i++)
            A[i] = nullptr;

        Node* start = min;
        Node* current = min;
        do {
            Node* x = current;
            current = current->right;
            int d = x->degree;
            while (A[d]) {
                Node* y = A[d];
                // rucni swap ako treba
                if (x->key > y->key) {
                    Node* temp = x;
                    x = y;
                    y = temp;
                }
                link(y, x);
                A[d] = nullptr;
                d++;
                if (d >= maxDegree) {
                    // U slučaju da d pređe opseg, realno ne bi trebalo da se desi, ali zaštita
                    break;
                }
            }
            A[d] = x;
        } while (current != start);

        min = nullptr;
        for (int i = 0; i < maxDegree; ++i) {
            if (A[i]) {
                if (!min) {
                    min = A[i];
                }
                else {
                    mergeNodes(min, A[i]);
                    if (A[i]->key < min->key) {
                        min = A[i];
                    }
                }
            }
        }
        delete[] A;
    }

    void link(Node* y, Node* x) {
        removeNode(y);
        y->left = y->right = y;
        if (!x->child) {
            x->child = y;
        }
        else {
            mergeNodes(x->child, y);
        }
        y->parent = x;
        x->degree++;
        y->mark = false;
    }
};

int main() {
    srand((unsigned)time(0));
    int N = 1000; // prilagodljivo do 10.000.000
    int k = 10;   // k izmedju 10 i 100
    int a = 1, b = 1000;

    FibonacciHeap heaps[4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < N; j++) {
            int num = rand() % (b - a + 1) + a;
            heaps[i].insert(num);
            if ((j + 1) % k == 0) {
                heaps[i].extractMin();
            }
        }
    }

    // Spajanje svih skupova u jedan
    for (int i = 1; i < 4; i++) {
        heaps[0].merge(&heaps[i]);
    }

    // Ekstrakcija u opadajućem redosledu
    int totalSize = heaps[0].n;
    if (totalSize < 0) totalSize = 0;
    int* sorted = new int[totalSize];

    int index = 0;
    while (heaps[0].n > 0) {
        int val = heaps[0].extractMin();
        if (val == INF) break; // ako je prazan heap
        sorted[index++] = val;
    }

    cout << "Poslednjih 10 brojeva u opadajućem redosledu:" << endl;
    int startIndex = totalSize - 10;
    if (startIndex < 0) startIndex = 0;

    for (int i = totalSize - 1; i >= startIndex; i--) {
        cout << sorted[i] << " ";
    }
    cout << endl;

    delete[] sorted;
    return 0;
}

