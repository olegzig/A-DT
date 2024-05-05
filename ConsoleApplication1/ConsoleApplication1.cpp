#include <iostream>
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

public:
	AVLTree();

	void insert(T value);

	int find(T value) const;
	const T& operator[](size_t idx) const;
};

template <class T>
AVLTree<T>::AVLTree() {
	root = nullptr;
	_size = 0;
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
void AVLTree<T>::balance(vector<AVLTreeNode<T>**> path) {  // starting from root
	reverse(path.begin(), path.end());

	for (auto indirect : path) {
		(*indirect)->updateValues();

		if ((*indirect)->balanceFactor() >= 2 and (*indirect)->left->balanceFactor() >= 0)   // left - left
			*indirect = (*indirect)->right_rotate();

		else if ((*indirect)->balanceFactor() >= 2) {  // left - right
			(*indirect)->left = (*indirect)->left->left_rotate();
			*indirect = (*indirect)->right_rotate();
		}

		else if ((*indirect)->balanceFactor() <= -2 and (*indirect)->right->balanceFactor() <= 0)  // right - right
			*indirect = (*indirect)->left_rotate();

		else if ((*indirect)->balanceFactor() <= -2) {  // right - left
			(*indirect)->right = ((*indirect)->right)->right_rotate();
			*indirect = (*indirect)->left_rotate();
		}
	}
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

int main()
{
	string input;
	bool correct = false;
	do {
		cout << "First of all:\nInput 0, in you want to use default array.\nInput 1, if you want to input array yourself.\nYor decision: ";
		cin >> input;

		if (input == "0" || input == "1") {
			correct = true;
		}
		else {
			cout << "\nnot correct input!\n";
		}
	} while (!correct);

	int* arr = new int[15];
	int arrSize = 0;
	if (input == "1") {
		cout << "Input array of values (up to numbers). Input non-number value if you want to stop\n";

		// ввод массива
		try {
			do {
				cout << "Input value " << arrSize << ":";
				cin >> input;
				int number = stoi(input);
				arr[arrSize] = number;
				arrSize++;
			} while (arrSize < 15);
		}
		catch (const invalid_argument& ex) {
			// ничего не делаем...
		}
		cout << "Input completed!";
	}
	else {
		arr = new int[15]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
		arrSize = 15;
	}

	// вывод массива
	cout << "\narray:";
	for (int i = 0; i < arrSize; i++) {
		cout << arr[i] << " ";
	}

	// приступаем к работе с деревьями
	AVLTree<long long> tree;

	// пихаем в дерево
	for (int i = 0; i < arrSize; i++) {
		int value = arr[i];
		tree.insert(value);
	}


	cout << "\ninput number to search:";
	cin >> input;
	int result = tree.find(stoi(input));
	cout << "index of element is: " << result;
	return 0;
}