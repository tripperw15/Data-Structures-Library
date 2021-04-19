#include <iostream>
#include <new>
#include <stdlib.h>
using namespace std;

template <class keytype, class valuetype>
struct Node {
	keytype key; // holds the key
    valuetype value; // holds the value
	Node *parent; // pointer to the parent
	Node *left; // pointer to left child
	Node *right; // pointer to right child
	bool color; // 1 -> Red, 0 -> Black
    int s;
};


template <class keytype, class valuetype>
class RBTree {
private:
// keytype key[];
// valuetype value[];
int num;
Node<keytype, valuetype>* root;
Node<keytype, valuetype>* TNULL;


public:
RBTree();
RBTree(keytype key[], valuetype value[], int s);
~RBTree();
void destroyRecursive(Node<keytype, valuetype>* node);
RBTree(const RBTree& old_tree);
RBTree<keytype, valuetype>& operator=(const RBTree& other);
Node<keytype, valuetype>* copyTree(Node<keytype, valuetype>* otherRoot, Node<keytype, valuetype>* otherNull);
valuetype* search(keytype k);
Node<keytype, valuetype>* searchHelper(Node<keytype, valuetype>* node, keytype key);
void insert(keytype k, valuetype v);
void insertFix(Node<keytype, valuetype>* k);
void leftRotate(Node<keytype, valuetype>* x);
void rightRotate(Node<keytype, valuetype>* x);
int remove(keytype k);
int removeHelper(Node<keytype, valuetype>* node, keytype k);
void removeFix(Node<keytype, valuetype>* x);
void rbTransplant(Node<keytype, valuetype>* u, Node<keytype, valuetype>* v);
void recurseToRoot(Node<keytype, valuetype>* node);
int rank(keytype k);
int rankHelper(Node<keytype, valuetype>* x, Node<keytype, valuetype>* y);
keytype select(int pos);
Node<keytype, valuetype>* selectHelper(Node<keytype, valuetype>* node, int i);
keytype* successor(keytype k);
Node<keytype, valuetype>* successorHelper(Node<keytype, valuetype>* x);
Node<keytype, valuetype>* minimum(Node<keytype, valuetype>* node);
keytype* predecessor(keytype k);
Node<keytype, valuetype>* predecessorHelper(Node<keytype, valuetype>* x);
Node<keytype, valuetype>* maximum(Node<keytype, valuetype>* node);
int size();
void preorder();
void preOrderHelper(Node<keytype, valuetype>* node); 
void postorder();
void postOrderHelper(Node<keytype, valuetype>* node);
void inorder();
void inOrderHelper(Node<keytype, valuetype>* node);
void printk(int k);
void printHelper(Node<keytype, valuetype>* node, int k, int& count);

};

template <class keytype, class valuetype>
RBTree<keytype, valuetype>::RBTree() {
    TNULL = new Node<keytype, valuetype>;
    TNULL->color = 0;
    TNULL->left = NULL;
    TNULL->right = NULL;
	TNULL->parent = NULL;
	TNULL->s = 0;
    num = 0;
    root = TNULL;
};

template <class keytype, class valuetype>
RBTree<keytype, valuetype>::RBTree(keytype key[], valuetype value[], int s) {
    TNULL = new Node<keytype, valuetype>;
    TNULL->color = 0;
    TNULL->left = NULL;
    TNULL->right = NULL;
	TNULL->parent = NULL;
	TNULL->s = 0;
    root = TNULL;
    num = 0;
    for(int i=0; i<s; i++) insert(key[i],value[i]);
};

template <class keytype, class valuetype>
RBTree<keytype, valuetype>::~RBTree() {
    destroyRecursive(this->root);
};

template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::destroyRecursive(Node<keytype, valuetype>* node) {
    if (node == TNULL) {
		return;
    }
	destroyRecursive(node->left);
    destroyRecursive(node->right);
    delete node;
};

template <class keytype, class valuetype>
RBTree<keytype, valuetype>::RBTree(const RBTree<keytype, valuetype>& old_tree) {
    // TNULL = new Node<keytype, valuetype>;
	// TNULL->parent = NULL;
	// TNULL->left = NULL;
	// TNULL->right = NULL;
	// num = old_tree.num;
	// root = TNULL;
	// if (old_tree.root == old_tree.TNULL) {
	// 	return;
	// }
	root = copyTree(old_tree.root, old_tree.TNULL);
};

template <class keytype, class valuetype>
RBTree<keytype, valuetype>& RBTree<keytype, valuetype>::operator=(const RBTree<keytype, valuetype>& other) {
    if (this != &other) {
		removeHelper(root);
		TNULL = new Node<keytype, valuetype>;
		TNULL->parent = NULL;
		TNULL->left = NULL;
		TNULL->right = NULL;
		num = other.num;
		root = TNULL;
		if (other.root = other.TNULL) {
			return *this;
		}
		root = copyTree(other.root, other.TNULL);
	}
	return *this;
};

template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::copyTree(Node<keytype, valuetype>* otherRoot, Node<keytype, valuetype>* otherNULL) {
    Node<keytype, valuetype>* copiedRoot = new Node<keytype, valuetype>;
    if (otherRoot == otherNULL)
            return TNULL;
    copiedRoot->key = otherRoot->key;
    copiedRoot->value = otherRoot->value;
    copiedRoot->color = otherRoot->color;
	copiedRoot->parent = otherRoot->parent;
	copiedRoot->s - otherRoot->s;
    copiedRoot->left = copyTree(otherRoot->left, otherNULL);
    copiedRoot->right = copyTree(otherRoot->right, otherNULL);
    return copiedRoot;
};

template <class keytype, class valuetype>
valuetype* RBTree<keytype, valuetype>::search(keytype k) {
    Node<keytype, valuetype>* spot = new Node<keytype, valuetype>;
    spot = searchHelper(this->root, k);
    if (spot != TNULL) {
        return &spot->value;
    } else {
        return NULL;
    }
};

template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::searchHelper(Node<keytype, valuetype>* node, keytype key) {
    if (node == TNULL || key == node->key) {
			return node;
		}

		if (key < node->key) {
			return searchHelper(node->left, key);
		} 
		return searchHelper(node->right, key);
};

template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::insert(keytype k, valuetype v) {
		// Ordinary Binary Search Insertion
		Node<keytype, valuetype>* node = new Node<keytype, valuetype>;
		node->parent = nullptr;
		node->key = k;
        node->value = v;
		node->left = TNULL;
		node->right = TNULL;
		node->s = 1;
		node->color = 1; // new node must be red

		Node<keytype, valuetype>* y = nullptr;
		Node<keytype, valuetype>* x = this->root;

		while (x != TNULL) {
			y = x;
			y->s++;
			if (node->key < x->key) {
				x = x->left;
			} else {
				x = x->right;
			}
		}

		// y is parent of x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		} else if (node->key < y->key) {
			y->left = node;
		} else {
			y->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == nullptr){
			node->color = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == nullptr) {
			return;
		}

		// Fix the tree
		insertFix(node);
        num++;
};

template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::insertFix(Node<keytype, valuetype>* k) {
    Node<keytype, valuetype>* u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			} else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// mirror case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;	
				} else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
};

template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::leftRotate(Node<keytype, valuetype>* x) {
    Node<keytype, valuetype>* y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
        y->s = x->s;
        x->s = x->left->s + x->right->s + 1;
};

template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::rightRotate(Node<keytype, valuetype>* x) {
    Node<keytype, valuetype>* y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
        y->s = x->s;
        x->s = x->left->s + x->right->s + 1;
};

template <class keytype, class valuetype>
int RBTree<keytype, valuetype>::remove(keytype k) {
    return removeHelper(this->root, k);
};

template <class keytype, class valuetype>
int RBTree<keytype, valuetype>::removeHelper(Node<keytype, valuetype>* node, keytype k) {
    Node<keytype, valuetype>* z = TNULL;
		Node<keytype, valuetype>* x;
        Node<keytype, valuetype>* y;

		while (node != TNULL){
			if (node->key == k) {
				z = node;
			}

			if (node->key <= k) {
				node = node->right;
			} else {
				node = node->left;
			}
		}

		if (z == TNULL) {
			return 0;
		} 

		y = z;
		num--;

		bool y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		} else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		} else {
			y = maximum(z->left);
			y_original_color = y->color;
			x = y->left;
			if (y->parent == z) {
				x->parent = y;
			} else {
				rbTransplant(y, y->left);
				y->left = z->left;
				y->left->parent = y;
			}

			rbTransplant(z, y);
			y->right = z->right;
			y->right->parent = y;
			y->color = z->color;
		}

		delete z;
		if (y_original_color == 0){
			removeFix(x);
		}

        return 1;
};

template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::removeFix(Node<keytype, valuetype>* x) {
    
    Node<keytype, valuetype>* s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}
                if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->right->color == 0) {
						// case 3.3
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					} 

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			} else {
				s = x->parent->left;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->left->color == 0) {
						// case 3.3
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					} 

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			} 
		}
		x->color = 0;
};

template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::rbTransplant(Node<keytype, valuetype>* u, Node<keytype, valuetype>* v) {
    recurseToRoot(u);
	if (u->parent == nullptr) {
			root = v;
		} else if (u == u->parent->left){
			u->parent->left = v;
		} else {
			u->parent->right = v;
		}
		v->parent = u->parent;
};

template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::recurseToRoot(Node<keytype, valuetype>* node) {
    Node<keytype, valuetype>* temp = node;
	while (temp != root) {
        temp->s--;
        temp = temp->parent;
    }
	root->s--;
};

template <class keytype, class valuetype>
int RBTree<keytype, valuetype>::rank(keytype k) {
    Node<keytype, valuetype>* node = new Node<keytype, valuetype>;
    node = searchHelper(this->root, k);
    return rankHelper(node, this->root);
    delete node;
};

template <class keytype, class valuetype>
int RBTree<keytype, valuetype>::rankHelper(Node<keytype, valuetype>* x, Node<keytype, valuetype>* y) {
    if (x == y) {
        return y->left->s+1;
    } else if (x->key < y->key) {
        return rankHelper(x, y->left);
    } else {
        return rankHelper(x, y->right)+y->left->s+1;
    }
};

template<class keytype, class valuetype>
keytype RBTree<keytype, valuetype>::select(int pos) {
    Node<keytype, valuetype>* spot = new Node<keytype, valuetype>;
    spot = selectHelper(this->root, pos);
    return spot->key;
    delete spot;
};

template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::selectHelper(Node<keytype, valuetype>* node, int i) {
    int r = node->left->s+1;
    if (i == r) {
        return node;
    } else if (i < r) {
        return selectHelper(node->left, i);
    } else {
        return selectHelper(node->right, i - r);
    }
};

template <class keytype, class valuetype>
keytype* RBTree<keytype, valuetype>::successor(keytype k) {
    Node<keytype, valuetype>* spot = new Node<keytype, valuetype>;
	spot = searchHelper(this->root, k);
    return &successorHelper(spot)->key;
};

template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::successorHelper(Node<keytype, valuetype>* x) {
    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;
	if (x->right != TNULL) {
			return minimum(x->right);
		}
		y = x->parent;
		while (y != TNULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
}

template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::minimum(Node<keytype, valuetype>* node) {
    while (node->left != TNULL) {
			node = node->left;
		}
		return node;
};

template <class keytype, class valuetype>
keytype* RBTree<keytype, valuetype>::predecessor(keytype k) {
    Node<keytype, valuetype>* spot = new Node<keytype, valuetype>;
	spot = searchHelper(this->root, k);
    return &predecessorHelper(spot)->key;
};

template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::predecessorHelper(Node<keytype, valuetype>* x) {
    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;
	if (x->left != TNULL) {
			return maximum(x->left);
		}
		y = x->parent;
		while (y != TNULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
}

template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::maximum(Node<keytype, valuetype>* node) {
    while (node->right != TNULL) {
			node = node->right;
		}
		return node;
};

template<class keytype, class valuetype>
int RBTree<keytype, valuetype>::size() {
    return num;
};

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::preorder() {
    preOrderHelper(this->root);
    cout << endl;
};

template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::preOrderHelper(Node<keytype, valuetype>* node) {
    if (node != TNULL) {
			cout<<node->key<<" ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
};

template<class keytype, class valuetype> 
void RBTree<keytype, valuetype>::inorder() {
    inOrderHelper(this->root);
    cout << endl;
};

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::inOrderHelper(Node<keytype, valuetype>* node) {
    if (node != TNULL) {
			inOrderHelper(node->left);
			cout<<node->key<<" ";
			inOrderHelper(node->right);
		} 
};

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::postorder() {
    postOrderHelper(this->root);
    cout << endl;
};

template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::postOrderHelper(Node<keytype, valuetype>* node) {
    if (node != TNULL) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout<<node->key<<" ";
		} 
};

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::printk(int k) {
    int count = 0;
	if (root != TNULL && count < k) {
		printHelper(root->left, k, count);
		if (count < k) {
			cout << root->key << " ";
		}
		count++;
		printHelper(root->right, k, count);
	}
	// for (int i = 1; i <= k; i++) {
	// 	cout << select(i);
	// 	if (i < k - 1) {
	// 		cout << " ";
	// 	}
	// }
	//inOrderHelper(root);
	cout << endl;
};

template<class keytype, class valuetype>
void RBTree<keytype, valuetype>::printHelper(Node<keytype, valuetype>* node,int k, int& count) {
	if (node != TNULL && count < k) {
		printHelper(node->left, k, count);
		if (count < k) {
			cout << node->key << " ";
		}
		count++;
		printHelper(node->right, k, count);
	}
};

// int main() {
//     string K[10] = {"A","B","C","D","E","F","G","H","I","K"};
// 	int V[10] = {10,9,8,7,6,5,4,3,2,1};
	
// 	RBTree<string,int> T1, T2(K,V,10);
	
// 	for(int i=0; i<10; i++) T1.insert(K[i],V[i]);
//     T1.postorder();
//     T2.postorder();
//     cout << *(T2.search("C")) << endl;
//     cout <<*(T1.successor("C"))<<endl;
//     cout <<*(T1.predecessor("C"))<<endl;
//     cout << T1.rank("C") << endl;
//     cout << T1.select(3) << endl;
//     T1.print(3);

// }