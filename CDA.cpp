#include <iostream> 
#include <new>
#include <stdlib.h>

#ifndef CDA_H
#define CDA_H

using namespace std;

template <class elmtype>
class CDA {
private:
    bool ordered;
    bool reversed;
    int back;
    elmtype safe;
 
public:
    int capacity;
    int size;
    elmtype *arr;
    int front;
    CDA();
    CDA(int s);
    ~CDA();
    CDA(const CDA& old_cda);
    elmtype& operator[](int i);
    CDA<elmtype>& operator=(const CDA& other);
    void AddEnd(elmtype v);
    void AddFront(elmtype v);
    void DelEnd();
    void DelFront();
    int Length();
    int Capacity();
    void Clear();
    void Reverse();
    elmtype Select(int k);
    elmtype partition(CDA A, int k);
    void Sort();
    void quickSort(elmtype arr[], int left, int right);
    int Search(elmtype e);
    int BinSearch(elmtype e);
};


template <class elmtype> 
CDA<elmtype>::CDA() {
        arr = new elmtype[1];
        size = 0;
        capacity = 1;
        ordered = false;
        reversed = false;
        front = 0;
        back = 0;

    };
template <class elmtype>
CDA<elmtype>::CDA(int s) {
        arr = new elmtype[s];
        capacity = s;
        size = s;
        ordered = false;
        reversed = false;
        front=0;
        back = size -1;
    };

template <class elmtype>
CDA<elmtype>::~CDA() {
        delete [] arr;
    };

template <class elmtype>
CDA<elmtype>::CDA(const CDA& old_cda) {
        size = old_cda.size;
        capacity = old_cda.capacity;
        ordered = old_cda.ordered;
        arr = new elmtype[size];
        for (int i = 0; i < size; i++) {
            arr[i] = old_cda.arr[i];
        }
        front = old_cda.front;
        back = old_cda.back;
        reversed = old_cda.reversed;
    }; 

template <class elmtype>
elmtype &CDA<elmtype>::operator[](int i) {
        if (i < 0 || i >= size) {
            cout << "Error: Out of bounds" << endl;
            return safe;
        }
        else if (reversed == false) {
            int search = (front + i) % capacity;
            return arr[search];
        } else {
            if (front - i >= 0) {
                return arr[front-i];
            } else if (front - i < 0){
                return arr[capacity + (front - i)];
            }
        }
    };

template <class elmtype>
CDA<elmtype>& CDA<elmtype>::operator=(const CDA& other) {
        if (this != &other) {
            delete [] arr;
            size = other.size;
            capacity = other.capacity;
            ordered = other.ordered;
            reversed = other.reversed;

            arr = new elmtype[size-1];

            for (int i = 0; i < size; i++) {
                arr[(front+i) % capacity] = other.arr[(front+i) % capacity];
            }
            front = other.front;
            back = other.back;
        }
        

        return *this;
    };

template <class elmtype>
void CDA<elmtype>::AddEnd(elmtype v) {
    
    if (size == capacity) {
        elmtype *newarr = new elmtype[capacity * 2];
        if (reversed == false) {
            for (int i = 0; i < size; i++) {
                newarr[i] = arr[(front+i) % capacity];
            }
        }
        else if (reversed == true) {
            for (int i = 0; i < size; i++) {
                if ((front-i+1) >= 0) {
                    newarr[i] = arr[front-i+1];
                } else if (front-i+1 < 0) {
                    newarr[i] = arr[capacity + front - i + 1];
                }
            }
        }
        
        delete [] arr;  
        arr = newarr;
        front = 0;
        back = size;
        capacity = capacity * 2;
    }
        if (reversed == false) {
            arr[(front+size) % capacity] = v;
            size++;
            back++;
        } else if (reversed == true) {
            if (back == 0) {
                arr[capacity-1] = v;
                back = capacity - 1;
                size++;
            }
            else {
                arr[front-size] = v;
                back--;
                size++;
            }
        }
    
    
};

template <class elmtype>
void CDA<elmtype>::AddFront(elmtype v) {
   
   if (size == capacity) {
        elmtype *newarr = new elmtype[capacity * 2];
        if (reversed == false) {
            newarr[0] = v;
            for (int i = 1; i < size+1; i++) {
                newarr[i] = arr[(front+i - 1) % capacity];
            }
        } else if (reversed == true) {
            newarr[0] = v;
            for (int i = 1; i < size+1; i++) {
                if ((front-i+1) >= 0) {
                    newarr[i] = arr[front-i+1];
                } else if (front-i+1 < 0) {
                    newarr[i] = arr[capacity + front - i + 1];
                }
            }
        }
        
        delete [] arr;  
        arr = newarr;
        front = 0;
        back = size;
        capacity = capacity * 2;
        size++;
    }
    else {
        if (reversed == false) {
            if (front == 0) {
                arr[capacity-1] = v;
                front = capacity - 1;
                size++;
            }
            else {
                arr[front-1] = v;
                front--;
                size++;
            }
        } else if (reversed == true) {
            front = (front + 1) % capacity;
            arr[front] = v;
            size++;
        }
    }
    
};

template <class elmtype>
void CDA<elmtype>::DelEnd() {
    size--;

    
    if (size == capacity / 4) {
        capacity = capacity / 2;
        elmtype *newarr = new elmtype[capacity];
        for (int i = 0; i  < size; i++) {
            newarr[i] = arr[(front + i) % capacity];
        }
        delete [] arr;
        arr = newarr;
        front = 0;
        back = size - 1;
    }
};

template <class elmtype>
void CDA<elmtype>::DelFront() {
    
    if (reversed == false) {
        front++;
    } else if (reversed == true) {
        front--;
    }
    size--;

    if (size == capacity / 4) {
        capacity = capacity / 2;
        elmtype *newarr = new elmtype[capacity];
        for (int i = 0; i  < size; i++) {
            newarr[i] = arr[(front + i) % capacity];
        }
        delete [] arr;
        arr = newarr;
        front = 0;
        back = size - 1;
    }
};

template <class elmtype>
int CDA<elmtype>::Length() {
        return size;
    };

template <class elmtype>
int CDA<elmtype>::Capacity() {
        return capacity;
    };

template <class elmtype>
void CDA<elmtype>::Clear() {
        delete [] arr;
        capacity = 4;
        arr = new elmtype[capacity];
        size = 0;
        front = 0;
        ordered = false;
        reversed = false;
    };

template <class elmtype>
void CDA<elmtype>::Reverse() {
    
    if (reversed == false) {
        int temp = front;
        front = (front + size -1) % capacity;
        back = temp;
        reversed = true;
    } else if (reversed == true) {
        int temp = back;
        back = (back + size -1) % capacity;
        front = temp;
        reversed = false;
    }
};

template <class elmtype>
elmtype CDA<elmtype>::Select(int k) {
    // int num = rand() % size;
    // int pivot = (front + num) % capacity;
    // elmtype comp = arr[pivot];

    // CDA L;
    // CDA E;
    // CDA G;

    // for (int i = 0; i < size; i++) {
    //     if(arr[(front + i) % capacity] < pivot) {
    //         E.AddEnd(arr[(front + i) % capacity]);
    //     } else if(arr[(front + i) % capacity] == pivot) {
    //         L.AddEnd(arr[(front + i) % capacity]);
    //     } else if(arr[(front + i) % capacity] > pivot) {
    //         L.AddEnd(arr[(front + i) % capacity]);
    //     } 
    // }

    // if (k <= L.Length()) {
    //     partition(L, k);
    // } else if (k <= L.Length() + E.Length()) {
    //     return pivot;
    // } else {
    //     return partition(G, k-L.Length()-E.Length());
    // }
};

template <class elmtype>
elmtype CDA<elmtype>::partition(CDA A, int k) {
    // int num = rand() % size;
    // int pivot = (A.front + num) % A.capacity;
    // elmtype comp = A[pivot];

    // CDA L;
    // CDA E;
    // CDA G;

    // for (int i = 0; i < size; i++) {
    //     if(A[(A.front + i) % A.capacity] < pivot) {
    //         E.AddEnd(A[(A.front + i) % A.capacity]);
    //     } else if(A[(A.front + i) % A.capacity] == pivot) {
    //         L.AddEnd(A[(A.front + i) % A.capacity]);
    //     } else if(A[(A.front + i) % A.capacity] > pivot) {
    //         L.AddEnd(A[(A.front + i) % A.capacity]);
    //     } 
    // }

    // if (k <= L.size) {
    //     partition(L, k);
    // } else if (k <= L.size + E.size) {
    //     return pivot;
    // } else {
    //     return partition(G, k-L.Length()-E.Length());
    // }
};


template <class elmtype>
void CDA<elmtype>::Sort() {
    if (reversed == true) {
        Reverse();
    }
        quickSort(arr, front, front+size-1);
};

template <class elmtype>
void CDA<elmtype>::quickSort(elmtype arr[], int left, int right) {
    int i = left, j = right; 
    elmtype tmp;
    elmtype pivot = arr[((left + right) / 2) % capacity];

    while (i <= j) {
        while (arr[i % capacity] < pivot) i++;
        while (arr[j % capacity] > pivot) j--;
        if (i <= j) {
            tmp = arr[i % capacity];
            arr[i % capacity] = arr[j % capacity];
            arr[j % capacity] = tmp;
            i++; j--;
        }
    }

    if (left < j) {
        quickSort(arr, left, j);
    }
    if (i < right) {
        quickSort(arr, i, right);
    }
};

template <class elmtype>
int CDA<elmtype>::Search(elmtype e) {
        if (reversed == false) {
            for (int i = 0; i < size; i++) {
                if (arr[(front+i) % capacity] == e) {
                    return i;
                }
            }
        } else if (reversed == true) {
            for (int i = 0; i < size; i++) {
                if (front - i >= 0) {
                    if(arr[(front-i) % capacity] == e) {
                        return i;
                    };
            } else if (front - i < 0){
                    if(arr[capacity + (front - i)] == e) {
                        return i;
                    }
                }
            }
        }
        
        
        return -1;
    };

template <class elmtype>
int CDA<elmtype>::BinSearch(elmtype e) {
        
    int low = 0;
    int high = size - 1;
    int mid;
    int ind;

    
    while (low <= high) {
        mid = (low + high) / 2;

        if (arr[mid % capacity] == e) {
            return mid % capacity;
        }
        if (arr[mid % capacity] < e) {
            low = mid + 1;
        }
        if (arr[mid % capacity] > e) {
            high = mid - 1;
        }
        ind = mid;
    }

    if(arr[(front + size - 1) % capacity] < e) {
        return ~size;
    } else {
        if (arr[ind % capacity] < e) {
            return ~(((ind - front) % capacity) + 1);
        }
        else {
            return ~((ind - front) % capacity);
        }
    }

    return ~mid; 
    };

#endif