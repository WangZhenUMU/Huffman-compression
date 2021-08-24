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
	W weight;//��һ���������͵����ݣ�����ֱ�ӱȽϣ���Ҫ���ء�

};

template<class T>
struct Com
{
	typedef HuffmanTreeNode<T> Node;

	bool operator()(const Node* left, const Node* right)
	{
		//2������ϵ�л����������͵Ļ��������أ������Զ������͵Ļ�����Ҫ����
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
		//�����ȼ����б���ڵ㣬����С��,�ڲ���greate���д������ʱ�Ե�ַ�����˽��ѣ�������Ҫ����Ȩֵ���ѵġ�û�дﵽԤ�ڵĽ����
		//�Լ�дһ���������бȽϣ�������ѡ�ͨ���º��������бȽ�ʵ�ִ��ڵķ�ʽ
		std::priority_queue<Node*,vector<Node*>, Com<W>> q;
		//1����ʹ��������Ȩֵ����ֻ�и��ڵ�Ķ�������ɭ��
		for (size_t i = 0; i < size; i++)
		{
			if (array[i] != invaild) //���˵����ִ���Ϊ0���ַ�ʱ����Ҫ�� != ���أ���Ϊ�����������ٱȽ�
				q.push(new Node(array[i]));
		}

		//2��ѭ������һ�²��裬ֱ��������ɭ����ֻʣ��һ�Ŷ�����Ϊֹ
		while (q.size() > 1)
		{
			//�Ӷ�����ɭ������ȡ��Ȩֵ��С��һ�Ŷ�����
			Node* left = q.top();
			q.pop();

			Node* right = q.top();
			q.pop();

			//��Left��right��Ϊĳ���½ڵ����������������һ���µĶ�������
			//�µĶ��������ڵ��Ȩֵ�������Һ���֮��
			//Ȩֵ��ӳ�����Ҫ�޸ģ�����1������������ȷ�ģ������Զ������ͱ����ˣ���Ҫ���Ӻ���д,��byteinfo�����ؼӺ�
			Node* parent = new Node(right->weight + left->weight);

			parent->left = left;
			parent->right = right;
			left->parent = parent;
			right->parent = parent;

			//���µĶ��������뵽������ɭ����ȥ
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
