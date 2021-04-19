#include <iostream>
using namespace std;
#include "CDA.cpp"

template <class keytype>
class Heap {
    private:
    CDA<keytype>* heap;
    keytype nil;
    int parent(int i);
    int left(int i);
    int right(int i);
    void heapify(int i, int s);
    void buildHeap();

    public:
    Heap();
    Heap(keytype k[], int s);
    ~Heap();
    Heap(const Heap& other);
    Heap<keytype>& operator=(const Heap& other);
    keytype peekKey();
    keytype extractMin();
    void insert(keytype k);
    void decreaseKey(int i, keytype k);
    void printKey();
};

template <class keytype>
Heap<keytype>::Heap() {
    heap = new CDA<keytype>();

};

template <class keytype>
Heap<keytype>::Heap(keytype k[], int s) {
    heap = new CDA<keytype>();
    for (int i = 0; i < s; i++) {
        heap->AddEnd(k[i]);
    }
    buildHeap();
};

template <class keytype>
Heap<keytype>::~Heap() {
    delete heap;
};

template <class keytype>
Heap<keytype>::Heap(const Heap& other) {
    heap = new CDA<keytype>();
    for (int i = 0; i <other.heap->size; i++) {
        heap->AddEnd(other.heap->arr[i]);
    }
};

template <class keytype>
Heap<keytype>& Heap<keytype>::operator=(const Heap& other) {
    if (this != &other) {
        delete heap;
        heap = new CDA<keytype>();
        for (int i = 0; i <other.heap->size; i++) {
            heap->AddEnd(other.heap->arr[i]);
        }
    }
    return *this;
};

template <class keytype>
int Heap<keytype>::parent(int i) {
    return (i+1)/2 - 1;
};

template <class keytype>
int Heap<keytype>::left(int i) {
    return (i+1)*2 -1;
};

template <class keytype>
int Heap<keytype>::right(int i) {
    return (i+1)*2;
};

template <class keytype>
void Heap<keytype>::heapify(int i, int s) {
    int l = left(i);
    int r = right(i);
    int small = -1;
    if (l <= heap->size-1 && heap->arr[l] < heap->arr[i]) {
        small = l;
    } else {
        small = i;
    }
    if (r <= heap->size-1 && heap->arr[r] < heap->arr[small]) {
        small = r;
    }
    if (small != i) {
        keytype temp = heap->arr[small];
        heap->arr[small] = heap->arr[i];
        heap->arr[i] = temp;
        heapify(small, heap->size);
    }
};

template <class keytype>
void Heap<keytype>::buildHeap() {
    for (int i = (heap->size-1) / 2; i >= 0; i--) {
        heapify(i, heap->size);
    }
};

template <class keytype>
keytype Heap<keytype>::peekKey() {
    return heap->arr[0];
};

template <class keytype>
keytype Heap<keytype>::extractMin() {
    keytype min = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size-1];
    heap->size--;
    heapify(0, heap->size);
    return min;
};

template <class keytype>
void Heap<keytype>::insert(keytype k) {
    heap->AddEnd(nil);
    decreaseKey(heap->size-1, k);
};

template <class keytype>
void Heap<keytype>::decreaseKey(int i, keytype k) {
    heap->arr[i] = k;
    while (i > 0 && heap->arr[parent(i)] > heap->arr[i]) {
        keytype temp = heap->arr[parent(i)];
        heap->arr[parent(i)] = heap->arr[i];
        heap->arr[i] = temp;
        i = parent(i); 
    }
};

template <class keytype>
void Heap<keytype>::printKey() {
    for (int i = 0; i < heap->size; i++) {
        cout << heap->arr[i] << " ";
    }
    cout << endl;
};