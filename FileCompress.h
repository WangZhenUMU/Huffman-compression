#pragma once
#include"HuffmanTree.hpp"

#include<string>
using namespace std;

typedef unsigned char uch;

struct ByteInfo
{
	uch _ch;
	int appearCount; //字符出现的次数
	string strCode; //字节对应的编码信息

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

	bool operator!=(const ByteInfo& b)const //在比较的时候需要重写这个方法
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
	//文件最终在磁盘上以字节的形式存储的，
	//字节：总共具有256中状态。
	//只需要给定一个256个ByteInfo类型的数组来保存字节出现的信息
	ByteInfo fileByteInfo[256];
};