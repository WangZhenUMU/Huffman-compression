#pragma once

#include<queue>
#include<vector>
using namespace std;

template<class W>
struct HuffmanTreeNode
{
public:
	HuffmanTreeNode(const W& w = W())
		:left(nullptr)
		, right(nullptr)
		, parent(nullptr)
		, weight(w)
	{}
	HuffmanTreeNode<W>* left;
	HuffmanTreeNode<W>* right;
	HuffmanTreeNode<W>* parent;
	W weight;//是一个复合类型的数据，不能直接比较，需要重载。

};

template<class T>
struct Com
{
	typedef HuffmanTreeNode<T> Node;

	bool operator()(const Node* left, const Node* right)
	{
		//2、整形系列或者内置类型的话不用重载，但是自定义类型的话就需要重载
		return left->weight > right->weight;
	}
};

template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;
public:
	HuffmanTree()
		:root(nullptr)
	{}
	~HuffmanTree()
	{
		Destroy(root);
	}
	void CreateHuffmanTree(const W array[], size_t size,const W& invaild)
	{
		//用优先级队列保存节点，创建小堆,在采用greate进行创建大堆时对地址进行了建堆，本来需要对其权值建堆的。没有达到预期的结果。
		//自己写一个函数进行比较，构建大堆。通过仿函数来进行比较实现大于的方式
		std::priority_queue<Node*,vector<Node*>, Com<W>> q;
		//1、先使用所给的权值创建只有根节点的二叉树的森林
		for (size_t i = 0; i < size; i++)
		{
			if (array[i] != invaild) //过滤掉出现次数为0的字符时，需要将 != 重载，因为是两个对象再比较
				q.push(new Node(array[i]));
		}

		//2、循环进行一下步骤，直到二叉树森林中只剩下一颗二叉树为止
		while (q.size() > 1)
		{
			//从二叉树森林中先取出权值最小的一颗二叉树
			Node* left = q.top();
			q.pop();

			Node* right = q.top();
			q.pop();

			//将Left和right作为某个新节点的左右子树，构建一颗新的二叉树，
			//新的二叉树根节点的权值是其左右孩子之和
			//权值相加出错，需要修改，问题1，内置类型正确的，但是自定义类型报错了，需要将加号重写,在byteinfo中重载加号
			Node* parent = new Node(right->weight + left->weight);

			parent->left = left;
			parent->right = right;
			left->parent = parent;
			right->parent = parent;

			//将新的二叉树插入到二叉树森林中去
			q.push(parent);
		}
		root = q.top();
	}
	Node* GetRoot()
	{
		return root;
	}
	void Destroy(Node*& proot)
	{
		if (proot)
		{
			Destroy(proot->left);
			Destroy(proot->right);
			delete proot;
			proot = nullptr;
		}
	}
private:
	Node* root;
};
