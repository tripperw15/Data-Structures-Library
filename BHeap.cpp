#include <iostream>
using namespace std;
#include "CDA.cpp"

template <class keytype>
struct Node {
    keytype key;
    Node *leftChild;
    Node *rightSibling;
    Node *parent;
    bool marked;
    int size;
};

template <class keytype>
class BHeap {
    private:
    Node<keytype>* min;
    Node<keytype>* head;
    Node<keytype>* TNULL;
    void linkTrees(Node<keytype>* x, Node<keytype>* y);
    void combine(Node<keytype>* h1, Node<keytype>* h2);
    void printHelper(Node<keytype>* node);

    public:
    BHeap();
    BHeap(keytype k[], int s);
    ~BHeap();
    void destroyRecursive(Node<keytype>* node);
    BHeap(const BHeap& other);
    BHeap<keytype>& operator=(const BHeap& other);
    Node<keytype>* copyTree(Node<keytype>* otherHead);
    keytype peekKey();
    void minimum(Node<keytype>* head);
    keytype extractMin();
    void insert(keytype k);
    void merge(BHeap<keytype>& H2);
    void printKey();
};

template <class keytype>
BHeap<keytype>::BHeap() {
    head = nullptr;
};

template <class keytype>
BHeap<keytype>::BHeap(keytype k[], int s) {
    head = nullptr;
    for (int i=0; i < s; i++) insert(k[i]);
};

template <class keytype>
BHeap<keytype>::~BHeap() {
    destroyRecursive(this->head);
};

template <class keytype>
void BHeap<keytype>::destroyRecursive(Node<keytype>* node) {
    if (node == nullptr) {
		return;
    }
	destroyRecursive(node->leftChild);
    destroyRecursive(node->rightSibling);
    delete node;
    return;
};

template <class keytype>
BHeap<keytype>::BHeap(const BHeap<keytype>& old) {
    Node<keytype>* newNode = new Node<keytype>;
    newNode->size = old.head->size;
    newNode->key = old.head->key;
    newNode->parent = nullptr;
    newNode->rightSibling = copyTree(old.head->rightSibling);
    newNode->leftChild = copyTree(old.head->leftChild);

    head = newNode;
};

template <class keytype>
BHeap<keytype>& BHeap<keytype>::operator=(const BHeap<keytype>& other) {
    if (this != &other) {
        destroyRecursive(head);
        Node<keytype>* newNode = new Node<keytype>;
        newNode->size = other.head->size;
        newNode->key = other.head->key;
        newNode->parent = nullptr;
        newNode->rightSibling = copyTree(other.head->rightSibling);
        newNode->leftChild = copyTree(other.head->leftChild);

        head = newNode;
    }
    return *this;
};

template <class keytype>
Node<keytype>* BHeap<keytype>::copyTree(Node<keytype>* otherHead) {
    Node<keytype>* copiedHead = new Node<keytype>;
    if (otherHead == nullptr) {
        return nullptr;
    }
    copiedHead->key = otherHead->key;
    copiedHead->parent = otherHead->parent;
    copiedHead->marked = otherHead->marked;
    copiedHead->size = otherHead->size;
    copiedHead->leftChild = copyTree(otherHead->leftChild);
    copiedHead->rightSibling = copyTree(otherHead->rightSibling);
    return copiedHead;
};

template <class keytype>
keytype BHeap<keytype>::peekKey() {
    if (min != nullptr) {
        return min->key;
    } else {
        return head->key;
    }
    
};

template <class keytype>
void BHeap<keytype>::minimum(Node<keytype>* head) {
    Node<keytype>* y = nullptr;
    Node<keytype>* x = head;
    keytype low = head->key; //need to figure out what to store here
    while (x != nullptr) {
        if (x->key < low) {
            low = x->key;
            y = x;
        }
        x = x->rightSibling;
    }
    min = y;
};

template <class keytype>
void BHeap<keytype>::insert(keytype k) {
    BHeap<keytype> *h2 = new BHeap<keytype>;
    Node<keytype> *x = new Node<keytype>;
    x->parent = nullptr;
    x->leftChild = nullptr;
    x->rightSibling = nullptr;
    x->size = 0;
    x->key = k;
    h2->head = x;
    merge(*h2);
};

template <class keytype>
void BHeap<keytype>::linkTrees(Node<keytype>* x, Node<keytype>* y) {
    x->parent = y;
    x->rightSibling = y->leftChild;
    y->leftChild = x;
    y->size += 1;
};

template <class keytype>
void BHeap<keytype>::combine(Node<keytype>* h1, Node<keytype>* h2) {
        Node<keytype>* currOld = h1;
		Node<keytype>* currNew = h2;
		Node<keytype>* currH = nullptr;
		Node<keytype>* temp = nullptr;

		if ((currOld != nullptr) && currNew != nullptr && (currOld->size <= currNew->size)) {
			currH = currOld;
			currOld = currOld->rightSibling;
		} else if (currNew != nullptr){
			currH = currNew;
			currNew = currNew->rightSibling;
		}

		temp = currH;

		// merge two heaps without taking care of trees with same degree
		// the roots of the tree must be in accending order of degree from
		// left to right
		while(currOld != nullptr && currNew != nullptr) {
			if (currOld->size <= currNew->size) {
				currH->rightSibling = currOld;
				currOld = currOld->rightSibling;
			} else {
				currH->rightSibling = currNew;
				currNew = currNew->rightSibling;
			}

			currH = currH->rightSibling;
		}

		if (currOld != nullptr) {
			// copy all the remaining trees of heap1
			while(currOld != nullptr && currH != nullptr) {
				currH->rightSibling = currOld;
				currOld = currOld->rightSibling;
				currH = currH->rightSibling;
			}
		}

		if (currNew != nullptr) {
			// copy all the remaining trees of heap2
			while(currNew != nullptr && currH!= nullptr) {
				currH->rightSibling = currNew;
				currNew = currNew->rightSibling;
				currH = currH->rightSibling;
			}
		}
        head = temp;
};

template <class keytype>
void BHeap<keytype>::merge(BHeap<keytype>& H2) {
    if (H2.head == nullptr) {
        return;
    }
    combine(head, H2.head);
    H2.head = nullptr;
    if (head == nullptr) {
        return;
    }
    Node<keytype> *prevx, *x, *nextx;
    prevx = nullptr;
    x = head;
    nextx = x->rightSibling;

    while (nextx != nullptr) {
        if ((x->size != nextx->size) || ((nextx->rightSibling != nullptr) && (nextx->rightSibling->size == x->size))) {
            prevx = x;
            x = nextx;
        } else {
            if (x->key <= nextx->key) {
                x->rightSibling = nextx->rightSibling;
                linkTrees(nextx, x);
            } else {
                if (prevx == nullptr) {
                    head = nextx;
                } else {
                    prevx->rightSibling = nextx;
                }
                linkTrees(x, nextx);
                x = nextx;
            }
        }
        nextx = x->rightSibling;
    }
    minimum(head);
};

template <class keytype>
void BHeap<keytype>::printKey() {
    Node<keytype>* x = head;
    while (x != nullptr) {
        cout << "B" << x->size << endl;
        cout << x->key;
        printHelper(x->leftChild);

        cout << endl;
        x = x->rightSibling;
        if (x != nullptr) {
            cout << endl;
        }
    }
};

template <class keytype>
void BHeap<keytype>::printHelper(Node<keytype>* node) {
    if (node != nullptr) {
        cout << " " << node->key;
        printHelper(node->leftChild);
        printHelper(node->rightSibling);
    }
};

template <class keytype>
keytype BHeap<keytype>::extractMin() {
    Node<keytype>* curr = head;
    Node<keytype>* prevMin = nullptr;
    Node<keytype>* minPtr = nullptr;
    Node<keytype>* prevPtr = nullptr;
    keytype min = curr->key;

    while (curr != nullptr) {
        if (curr->key <= min) {
            min = curr->key;
            prevMin = prevPtr;
            minPtr = curr;
        }
        prevPtr = curr;
        curr = curr->rightSibling;
    }

    if (prevMin != nullptr) {
        prevMin->rightSibling = minPtr->rightSibling;
    } else {
        head = minPtr->rightSibling;
    }

    keytype returnValue = min;

    BHeap<keytype> *newHead = new BHeap<keytype>;
    Node<keytype>* prevChild = nullptr;
    Node<keytype>* curChild = minPtr->leftChild;
    Node<keytype>* nextChild = nullptr;

    while (curChild != nullptr) {
        curChild->parent = nullptr;
        nextChild = curChild->rightSibling;
        curChild->rightSibling = prevChild;
        prevChild = curChild;
        curChild = nextChild;
    }

    delete minPtr;
    
    newHead->head = prevChild;
    merge(*newHead);
    return returnValue;
};