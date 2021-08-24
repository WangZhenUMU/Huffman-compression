#pragma once
#include"HuffmanTree.hpp"

#include<string>
using namespace std;

typedef unsigned char uch;

struct ByteInfo
{
	uch _ch;
	int appearCount; //�ַ����ֵĴ���
	string strCode; //�ֽڶ�Ӧ�ı�����Ϣ

	ByteInfo(int count = 0)
		:appearCount(count)
	{}

	ByteInfo operator+(const ByteInfo& b) const
	{
		ByteInfo temp;
		temp.appearCount = appearCount + b.appearCount;
		return temp;
	}
	bool operator>(const ByteInfo& b) const
	{
		return appearCount > b.appearCount;
	}

	bool operator!=(const ByteInfo& b)const //�ڱȽϵ�ʱ����Ҫ��д�������
	{
		return appearCount != b.appearCount;
	}
	bool operator-=(const ByteInfo& b)const
	{
		return appearCount == b.appearCount;
	}
};

class FileCompress
{
public:
	FileCompress();
	bool CompressFile(const string& FilePath);
	bool UNCompressFile(const string& FilePath);
private:
	void GetLine(FILE* fIn, string& strContent);
	void WriteHead(FILE* fOut, const string& FilePath);
	void GenerateHuffmenCode(HuffmanTreeNode<ByteInfo>* root);
	//�ļ������ڴ��������ֽڵ���ʽ�洢�ģ�
	//�ֽڣ��ܹ�����256��״̬��
	//ֻ��Ҫ����һ��256��ByteInfo���͵������������ֽڳ��ֵ���Ϣ
	ByteInfo fileByteInfo[256];
};