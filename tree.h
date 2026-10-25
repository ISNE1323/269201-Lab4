#include <cstddef>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

#ifndef Binary_Search_Tree
#define Binary_Search_Tree

template<class T> class Tree;

template<class T>
class Node {
public:
	Node() { left = right = NULL; }
	Node(const T& el, Node *l = 0, Node *r = 0) {
		key = el; left = l; right = r;
	}
	T key;
	Node *left, *right;
};

template<class T>
class Tree {
public:
	Tree() { root = 0; }
	~Tree() { clear(); }
	void clear() { clear(root); root = 0; }
	bool isEmpty() { return root == 0; }
	void inorder() { inorder(root); }
	void insert(const T& el);
	T* search(const T& el);          // returns pointer to the stored key, or 0 if not found
	void findAndDelete(const T& el);
	void deleteNode(Node<T> *& node);

	int height() { return height(root); }   // number of levels (empty tree = 0, one node = 1)
	int size() { return size(root); }       // number of nodes
	void balance();                         // rebuild the tree so it is as short as possible

protected:
	Node<T> *root;

	void clear(Node<T> *p);
	void inorder(Node<T> *p);
	int height(Node<T> *p);
	int size(Node<T> *p);
	void storeInorder(Node<T> *p, vector<T>& data);
	Node<T>* build(const vector<T>& data, int first, int last);

};

template<class T>
void Tree<T>::clear(Node<T> *p)
{
	if (p != 0) {
		clear(p->left);
		clear(p->right);
		delete p;
	}
}

template<class T>
void Tree<T>::inorder(Node<T> *p) {
	// Inorder traversal: left -> node -> right
	// In a BST this visits keys in sorted (here: alphabetical) order.
	if (p != 0) {
		inorder(p->left);
		cout << p->key << endl;
		inorder(p->right);
	}
}

// Height = number of levels from the root down to the deepest node.
// Empty tree -> 0, a tree with only a root -> 1.
template<class T>
int Tree<T>::height(Node<T> *p) {
	if (p == 0)
		return 0;
	int leftHeight = height(p->left);
	int rightHeight = height(p->right);
	return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// Number of nodes in the tree.
template<class T>
int Tree<T>::size(Node<T> *p) {
	if (p == 0)
		return 0;
	return 1 + size(p->left) + size(p->right);
}

// Copy all keys into a vector in sorted order (an inorder walk).
template<class T>
void Tree<T>::storeInorder(Node<T> *p, vector<T>& data) {
	if (p != 0) {
		storeInorder(p->left, data);
		data.push_back(p->key);
		storeInorder(p->right, data);
	}
}

// Build a perfectly balanced tree from the sorted keys data[first..last]:
// the middle key becomes the root, the left half becomes the left subtree
// and the right half becomes the right subtree (same idea, repeated).
template<class T>
Node<T>* Tree<T>::build(const vector<T>& data, int first, int last) {
	if (first > last)
		return 0;
	int middle = first + (last - first) / 2;
	Node<T> *leftChild = build(data, first, middle - 1);
	Node<T> *rightChild = build(data, middle + 1, last);
	return new Node<T>(data[middle], leftChild, rightChild);
}

// Balance the tree:
// 1) save all keys in sorted order  2) delete the old nodes  3) build a new balanced tree.
template<class T>
void Tree<T>::balance() {
	vector<T> data;
	storeInorder(root, data);
	clear();
	root = build(data, 0, (int)data.size() - 1);
}

template<class T>
T* Tree<T>::search(const T& el) {
	Node<T> *p = root;
	while (p != 0) {
		if (p->key == el)
			return &p->key;
		else if (p->key < el)
			p = p->right;
		else
			p = p->left;
	}
	return 0;
}

template<class T>
void Tree<T>::insert(const T &el) {
	Node<T> *p = root, *prev = 0;
	while (p != 0) {
		prev = p;
		if (p->key < el)
			p = p->right;
		else
			p = p->left;
	}
	if (root == 0)
		root = new Node<T>(el);
	else if (prev->key<el)
		prev->right = new Node<T>(el);
	else
		prev->left = new Node<T>(el);
}

template<class T>
void Tree<T>::findAndDelete(const T& el) {
	Node<T> *node = root, *prev = 0;
	while (node != 0) {
		if (node->key == el)
			break;
		prev = node;
		if (node->key < el)
			node = node->right;
		else node = node->left;
	}
	if (node != 0 && node->key == el)
		if (node == root)
			deleteNode(root);
		else if (prev->left == node)
			deleteNode(prev->left);
		else deleteNode(prev->right);
}


template<class T>
void Tree<T>::deleteNode(Node<T> *&node) {
	Node<T> *prev, *tmp = node;
	if (node->right == 0)
		node = node->left;
	else if (node->left == 0)
		node = node->right;
	else {
		tmp = node->left;
		prev = node;
		while (tmp->right != 0) {
			prev = tmp;
			tmp = tmp->right;
		}
		node->key = tmp->key;
		if (prev == node)
			prev->left = tmp->left;
		else prev->right = tmp->left;
	}
	delete tmp;
}

#endif // Binary_Search_Tree