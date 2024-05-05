#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// структура для представления узлов дерева
struct node 
{
	int key;
	unsigned char height;
	node* left;
	node* right;
	node(int k) { key = k; left = right = 0; height = 1; }
};

unsigned char height(node* p)
{
	return p ? p->height : 0;
}

int bfactor(node* p)
{
	return height(p->right) - height(p->left);
}

void fixheight(node* p)
{
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

// правый поворот вокруг p
node* rotateright(node* p) 
{
	node* q = p->left;
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);
	return q;
}

// левый поворот вокруг q
node* rotateleft(node* q)
{
	node* p = q->right;
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
}

// балансировка узла p
node* balance(node* p) 
{
	fixheight(p);
	if (bfactor(p) == 2)
	{
		if (bfactor(p->right) < 0)
			p->right = rotateright(p->right);
		return rotateleft(p);
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0)
			p->left = rotateleft(p->left);
		return rotateright(p);
	}
	return p; // балансировка не нужна
}

// вставка ключа k в дерево с корнем p
node* insert(node* p, int k) 
{
	if (!p) return new node(k);
	if (k < p->key)
		p->left = insert(p->left, k);
	else
		p->right = insert(p->right, k);
	return balance(p);
}

// поиск узла с минимальным ключом в дереве p 
node* findmin(node* p) 
{
	return p->left ? findmin(p->left) : p;
}

int main()
{
	string input = "";
	cout << "Input array of values (10-15 numbers): ";
	cin >> input;

	// тут мы конвертим строку в массив чисел 
	stringstream ss(input);
	vector<int> vector;
	copy(istream_iterator<int>(ss), {}, back_inserter(vector));
	copy(begin(vector), end(vector), ostream_iterator<int>(cout, " "));


}