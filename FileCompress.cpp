#include"FileCompress.h"
#include"HuffmanTree.hpp"
#include<iostream>
FileCompress::FileCompress()
{
	for (int i = 0; i < 256; i++)
	{
		fileByteInfo[i]._ch = i;
	}
}
bool FileCompress::CompressFile(const string& FilePath)
{
	//1��ͳ��Դ�ļ��е�ÿ���ַ�ȫ����ȡһ��
	FILE* pf = fopen(FilePath.c_str(), "rb");
	if (pf == nullptr)
	{
		cout << "�򿪴�ѹ���ļ�ʧ��\n";
		return false;
	}

	//�ļ��Ĵ�С��ʱδ֪���˴���Ҫѭ���ķ�ʽ����ȡ�ļ�������
	uch ReadBuff[1024];
	while (true)
	{
		//��Ҫһ���Զ�ȡ1024���ֽڣ�����ʵ�ʶ�ȡ�����ֽ�
		size_t rdsize = fread(ReadBuff, 1, 1024, pf);
		if(rdsize == 0)
		{
			//�Ѿ���ȡ�����ļ�ĩβ
			break;
		}
		//����ͳ��
		for(size_t i = 0; i < rdsize; i++)
		{
			//���õ���ֱ�Ӷ�ֵ�������ַ���ASCLL��ֵ�����п��ٵ�ͳ��
			fileByteInfo[ReadBuff[i]].appearCount++;
		}
	}
	HuffmanTree<ByteInfo> ht;
	ByteInfo invaild;

	//�ڴ�����������ʱ�������ִ���Ϊ0�ַ���û�д��������£���Ҫ�����ִ���Ϊ0���ַ��޳�����
	ht.CreateHuffmanTree(fileByteInfo, 256,invaild);
	//3����������������ȡÿ���ֽڵı���
	GenerateHuffmenCode(ht.GetRoot());

	//4�������ֽڱ����Դ�ļ����и�д
	//д��ѹ��Ҫ��ͷ����Ϣ
	FILE* fOut = fopen("2.hzp", "wb");
	WriteHead(fOut, FilePath);
	fseek(pf,0,SEEK_SET);
	uch ch = 0;
	uch bitCount = 0;
	while (true)
	{
		size_t rdsize = fread(ReadBuff, 1, 1024, pf);
		if (rdsize == 0)
			break;

		//�ñ����д�ֽڣ�����д��Ľ����Ҫ���õ�ѹ���ļ��б���
		for (size_t i = 0; i < rdsize; i++)
		{
			//�ñ����д��readbuff[i] -----> 'A' ----->"100"
			string& strCode = fileByteInfo[ReadBuff[i]].strCode;
			
			//ֻ��Ҫ���ַ�����ʽ�Ķ����Ʊ���洢���ֽ��о�����
			for (size_t j = 0; j < strCode.size(); j++)
			{
				ch <<= 1;  //��λ��������λ��0
				if ('1' == strCode[j])
					ch |= 1;

			 // ��ch�е�8������λ��������Ҫ�����ֽ�д���ļ�����
			 //����������Ascll��֮����������ʾ���±����128����һ��������������Ҫ���з��Ÿ�Ϊ�޷���
				bitCount++;
				if (bitCount == 8)
				{
					fputc(ch, fOut);
					bitCount = 0;
					ch = 0;
				}
			}
		}
	}
	if (bitCount > 0 && bitCount < 8)
	{
		ch <<= (8 - bitCount);
		fputc(ch, fOut);
	}
	fclose(pf);
	fclose(fOut);
	return true;
}
void FileCompress::WriteHead(FILE* fOut, const string& FilePath)
{
	//1����ȡԴ�ļ��ĺ�׺
	string postFix = FilePath.substr(FilePath.rfind('.'));
	postFix += "\n";
	fwrite(postFix.c_str(), 1, postFix.size(), fOut);

	//�����ַ�Ƶ����Ϣ��ͳ����Ч�ַ��ĸ����Ͷ�Ӧ����
	string chAppearCount;
	size_t lineCount = 0;
	for (size_t i = 0; i < 256; i++)
	{
		if (fileByteInfo[i].appearCount > 0)
		{
			chAppearCount += fileByteInfo[i]._ch;
			chAppearCount += ':';
			chAppearCount += to_string(fileByteInfo[i].appearCount);
			chAppearCount += "\n";
			lineCount++;
		}
	}
	//д��������Ƶ����Ϣ
	string totalLine = to_string(lineCount);
	totalLine += "\n";
	fwrite(totalLine.c_str(), 1, totalLine.size(), fOut);
	fwrite(chAppearCount.c_str(), 1, chAppearCount.size(), fOut);
}
void FileCompress::GetLine(FILE* fIn, string& strContent)
{
	uch ch;
	while(!feof(fIn))
	{
		ch = fgetc(fIn);
		if (ch == '\n')
			break;

		strContent += ch;
	}
}
// �ļ���ѹ��
bool FileCompress::UNCompressFile(const string& filePath)
{
	// 1.��ѹ���ļ��ж�ȡ��ѹ��ʱ��Ҫʹ�õ���Ϣ
	FILE* fIn = fopen(filePath.c_str(), "rb");
	if (fIn < 0)
	{
		cout << "��ѹ���ļ�ʧ��" << endl;
		return false;
	}
	//  ��ȡԴ�ļ���׺
	string postFix;
	GetLine(fIn, postFix);
	// ��ȡƵ����Ϣ������
	string strContent;
	GetLine(fIn, strContent);
	size_t lineCount = atoi(strContent.c_str());
	//ѭ����ȡlinecount�У���ȡ�ֽڵ�Ƶ����Ϣ
	strContent = "";
	for (size_t i = 0; i < lineCount; ++i)
	{
		GetLine(fIn, strContent);
		if ("" == strContent)
		{
			//˵���ոն�ȡ������һ������
			strContent += '\n';
			GetLine(fIn, strContent);
		}
		fileByteInfo[(unsigned char)strContent[0]].appearCount = atoi(strContent.c_str() + 2);
		strContent = "";
	}

	// 2.���õõ�����Ϣ�ָ�huffman��
	ByteInfo invaild;
	HuffmanTree<ByteInfo> ht;
	ht.CreateHuffmanTree(fileByteInfo, 256,invaild);
	// 3.��ȡѹ�����ݣ����Huffman�����н�ѹ��
	string filename("3");
	filename += postFix;
	FILE* fOut = fopen(filename.c_str(), "wb");
	filename += postFix;
	unsigned char readBuff[1024];
	unsigned char bitCount = 0;
	HuffmanTreeNode<ByteInfo>* cur = ht.GetRoot();
	const int fileSize = cur->weight.appearCount;
	int compressSize = 0;
	while (true)
	{
		size_t rdsize = fread(readBuff, 1, 1024, fIn);
		if (0 == rdsize)
			break;
		for (size_t i = 0; i < rdsize; ++i)
		{
			//���ֽڽ�ѹ��
			unsigned char ch = readBuff[i];
			bitCount = 0;
			while (bitCount < 8)
			{
				if (ch & 0x80)
					cur = cur->right;
				else
					cur = cur->left;
				bitCount++;
				if (nullptr == cur->left&&nullptr == cur->right)
				{
					fputc(cur->weight._ch, fOut);
					cur = ht.GetRoot();
					compressSize++;
					if (compressSize == fileSize)
						break;
				}
				ch <<= 1;
			}
		}
	}

	fclose(fIn);
	fclose(fOut);
	return true;
}


void FileCompress::GenerateHuffmenCode(HuffmanTreeNode<ByteInfo>* root)
{
	if (root == nullptr)
		return;

	//������Ч��ȨֵΪҶ�ӽڵ��Ϊֹ����������Ҷ�ӽ���λ��ʱ����Ȩֵ��Ӧ�ı�����õ���
	if (root->left == nullptr && root->right == nullptr)
	{
		HuffmanTreeNode<ByteInfo>* cur = root;
		HuffmanTreeNode<ByteInfo>* parent = cur->parent;
		//fileByteInfo[cur->weight._ch].strCode     
		//���ջ�ȡ���ı�����Ҫ���浽fileByteInfo��Ӧ��λ����
		//curΪ��ǰ�Ľڵ㣬
		//cur->weight->_ch��ʾ���Ǹ�λ���ϵ�Ԫ��
		//strCode�Ǳ���ı���
		string& strCode = fileByteInfo[cur->weight._ch].strCode; //�������
		while (parent) //��ȡ������һ������ı���
		{
			if (cur == parent->left)
				strCode += '0';
			else
				strCode += '1';
			cur = parent;
			parent = cur->parent;
		}
		//���ַ�������
		reverse(strCode.begin(), strCode.end());
	}

	GenerateHuffmenCode(root->left);
	GenerateHuffmenCode(root->right);
}