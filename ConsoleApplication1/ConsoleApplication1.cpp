﻿#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <string>
using namespace std;

template <class T>
struct AVLTreeNode {
	AVLTreeNode* left;
	AVLTreeNode* right;

	const T value;
	int count;  // how many nodes are there in this subtree
	int height;

	AVLTreeNode(T value);
	void updateValues();
	int balanceFactor();

	AVLTreeNode* left_rotate();
	AVLTreeNode* right_rotate();
};

template <class T>
AVLTreeNode<T>::AVLTreeNode(T value) : value(value) {
	count = 1;
	height = 1;

	left = nullptr;
	right = nullptr;
}

template <class T>
void AVLTreeNode<T>::updateValues() {
	count = (left != nullptr ? left->count : 0) + (right != nullptr ? right->count : 0) + 1;

	height = max(left != nullptr ? left->height : 0,
		right != nullptr ? right->height : 0) + 1;
}

template <class T>
int AVLTreeNode<T>::balanceFactor() {
	return (left != nullptr ? left->height : 0) - (right != nullptr ? right->height : 0);
}

template <class T>
AVLTreeNode<T>* AVLTreeNode<T>::left_rotate() {
	AVLTreeNode* R = right;
	right = right->left;
	R->left = this;

	this->updateValues();  // the order is important
	R->updateValues();

	return R;
}

template <class T>
AVLTreeNode<T>* AVLTreeNode<T>::right_rotate() {
	AVLTreeNode* L = left;
	left = left->right;
	L->right = this;

	this->updateValues();  // the order is important
	L->updateValues();

	return L;
}

template <class T>
class AVLTree {
	int _size;
	AVLTreeNode<T>* root;

	void balance(vector<AVLTreeNode<T>**> path);
	void display(AVLTreeNode<T>* cur, int depth = 0, int state = 0);

public:
	AVLTree();
	~AVLTree();

	void insert(T value);
	void erase(T value);

	void clear();
	bool empty() const;
	int size() const;

	int find(T value) const;
	int lower_bound(T value) const;
	int upper_bound(T value) const;
	const T& operator[](size_t idx) const;

	void display();
};

template <class T>
AVLTree<T>::AVLTree() {
	root = nullptr;
	_size = 0;
}

template <class T>
AVLTree<T>::~AVLTree() {
	clear();
}

template <class T>
void AVLTree<T>::clear() {
	vector<AVLTreeNode<T>*> stack;

	if (root != nullptr)
		stack.push_back(root);

	while (!stack.empty()) {
		AVLTreeNode<T>* node = stack.back();
		stack.pop_back();

		if (node->left != nullptr)
			stack.push_back(node->left);

		if (node->right != nullptr)
			stack.push_back(node->right);

		_size--;
		delete node;
	}

	root = nullptr;
}

template <class T>
void AVLTree<T>::insert(T value) {
	AVLTreeNode<T>** indirect = &root;  // to generalize insertion
	vector<AVLTreeNode<T>**> path;  // to update height values

	while (*indirect != nullptr) {
		path.push_back(indirect);

		if ((*indirect)->value > value)
			indirect = &((*indirect)->left);
		else
			indirect = &((*indirect)->right);
	}

	*indirect = new AVLTreeNode<T>(value);
	path.push_back(indirect);

	balance(path);
	_size++;
}

template <class T>
void AVLTree<T>::erase(T value) {
	AVLTreeNode<T>** indirect = &root;  // to generalize insertion
	vector<AVLTreeNode<T>**> path;  // to update height values

	while (*indirect != nullptr and (*indirect)->value != value) {
		path.push_back(indirect);

		if ((*indirect)->value > value)
			indirect = &((*indirect)->left);
		else
			indirect = &((*indirect)->right);
	}

	if (*indirect == nullptr)  // the value does not exist in tree
		return;  // may be raising an Exception is more elegant

	else
		path.push_back(indirect);

	size_t index = path.size();

	if ((*indirect)->left == nullptr and (*indirect)->right == nullptr) {  // the node is leaf
		delete* indirect;  // just delete node
		*indirect = nullptr;
		path.pop_back();  // pop the deleted node from path
	}

	else if ((*indirect)->right == nullptr) {  // only left child exists
		AVLTreeNode<T>* toRemove = *indirect;

		(*indirect) = (*indirect)->left;
		delete toRemove;

		path.pop_back();
	}

	else {  // right child exists
		AVLTreeNode<T>** successor = &((*indirect)->right);

		while ((*successor)->left != nullptr) {
			path.push_back(successor);
			successor = &((*successor)->left);
		}

		if (*successor == (*indirect)->right) {
			(*successor)->left = (*indirect)->left;

			AVLTreeNode<T>* toRemove = *indirect;
			*indirect = *successor;
			delete toRemove;
		}

		else {
			AVLTreeNode<T>* tmp = *path.back(), * suc = *successor;

			tmp->left = (*successor)->right;
			suc->left = (*indirect)->left;
			suc->right = (*indirect)->right;

			delete* indirect;
			*indirect = suc;
			path[index] = &(suc->right);
		}
	}

	balance(path);
	_size--;
}

template <class T>
void AVLTree<T>::balance(vector<AVLTreeNode<T>**> path) {  // starting from root
	reverse(path.begin(), path.end());

	for (auto indirect : path) {
		(*indirect)->updateValues();

		if ((*indirect)->balanceFactor() >= 2 and (*indirect)->left->balanceFactor() >= 1)   // left - left
			*indirect = (*indirect)->right_rotate();

		else if ((*indirect)->balanceFactor() >= 2) {  // left - right
			(*indirect)->left = (*indirect)->left->left_rotate();
			*indirect = (*indirect)->right_rotate();
		}

		else if ((*indirect)->balanceFactor() <= -2 and (*indirect)->right->balanceFactor() <= -1)  // right - right
			*indirect = (*indirect)->left_rotate();

		else if ((*indirect)->balanceFactor() <= -2) {  // right - left
			(*indirect)->right = ((*indirect)->right)->right_rotate();
			*indirect = (*indirect)->left_rotate();
		}
	}
}

template <class T>
bool AVLTree<T>::empty() const {
	return _size == 0;
}

template <class T>
int AVLTree<T>::size() const {
	return _size;
}

template <class T>
int AVLTree<T>::find(T value) const {
	AVLTreeNode<T>* direct = root;  // to generalize insertion
	int idx = 0;

	while (direct != nullptr and direct->value != value) {
		if (direct->value > value)
			direct = direct->left;
		else {
			idx += (direct->left ? direct->left->count : 0) + 1;
			direct = direct->right;
		}
	}

	if (direct == nullptr)
		return -1;

	else
		return idx + (direct->left ? direct->left->count : 0);
}

template <class T>
int AVLTree<T>::upper_bound(T value) const {
	AVLTreeNode<T>* direct = root;  // to generalize insertion
	int idx = 0;

	while (direct != nullptr) {
		if (direct->value > value)
			direct = direct->left;
		else {
			idx += (direct->left ? direct->left->count : 0) + 1;
			direct = direct->right;
		}
	}

	return idx;
}

template <class T>
int AVLTree<T>::lower_bound(T value) const {
	AVLTreeNode<T>* direct = root;  // to generalize insertion
	int idx = 0;

	while (direct != nullptr) {
		if (direct->value >= value)
			direct = direct->left;
		else {
			idx += (direct->left ? direct->left->count : 0) + 1;
			direct = direct->right;
		}
	}

	return idx;
}

template <class T>
const T& AVLTree<T>::operator[](size_t idx) const {
	AVLTreeNode<T>* cur = root;
	int left = cur->left != nullptr ? cur->left->count : 0;

	while (left != idx) {
		if (left < idx) {
			idx -= left + 1;

			cur = cur->right;
			left = cur->left != nullptr ? cur->left->count : 0;
		}

		else {
			cur = cur->left;
			left = cur->left != nullptr ? cur->left->count : 0;
		}
	}

	return cur->value;
}

template <class T>
void AVLTree<T>::display() {
	printf("\n");
	if (root != nullptr)
		display(root);
	else
		printf("Empty");
	printf("\n");
}

template <class T>
void AVLTree<T>::display(AVLTreeNode<T>* cur, int depth, int state) {  // state: 1 -> left, 2 -> right , 0 -> root
	if (cur->left)
		display(cur->left, depth + 1, 1);

	for (int i = 0; i < depth; i++)
		printf("     ");

	if (state == 1) // left
		printf("┌───");
	else if (state == 2)  // right
		printf("└───");

	cout << "[" << cur->value << "] - (" << cur->count << ", " << cur->height << ")" << endl;

	if (cur->right)
		display(cur->right, depth + 1, 2);
}

int main()
{
	string input;
	bool correct = false;
	do {
		cout << "First of all:\nInput 0, in you want to use default array.\nInput 1, if you want to input array yourself.\nYor decision: ";
		cin >> input;

		if (input == "0" && input == "1") {
			correct = true;
		}
		else {
			cout << "\nnot correct input!\n";
		}
	} while (!correct);

	int* arr{ new int[10] };
	int arrSize = 0;
	if (input == "0") {
		cout << "Input array of values (10-15 numbers). Input non-number value if you want to stop\n";

		// ввод массива
		do {
			cout << "Input value " << arrSize << ":";
			cin >> input;
			int number = stoi(input);

			if (input != "0" && number == 0) {
				break;
			}
		} while (arrSize < 15);
	}
	else {
		arr = new int[15]{ 0,1, 2, 3, 4,5,6,7,8,9,10,11,12,13,14 };
		arrSize = 15;
	}

	// вывод массива
	cout << "\narray:";
	for (int i = 0; i < arrSize; i++) {
		cout << arr[i];
	}

	node node = new node;
	// пихаем в дерево
	for (int i = 0; i < arrSize; i++) {
		cout << arr[i];
	}
	return 0;
}