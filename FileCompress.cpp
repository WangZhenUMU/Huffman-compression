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
	//1、统计源文件中的每个字符全部读取一边
	FILE* pf = fopen(FilePath.c_str(), "rb");
	if (pf == nullptr)
	{
		cout << "打开带压缩文件失败\n";
		return false;
	}

	//文件的大小暂时未知，此处需要循环的方式来获取文件的内容
	uch ReadBuff[1024];
	while (true)
	{
		//想要一次性读取1024个字节，但是实际读取到的字节
		size_t rdsize = fread(ReadBuff, 1, 1024, pf);
		if(rdsize == 0)
		{
			//已经读取到了文件末尾
			break;
		}
		//进行统计
		for(size_t i = 0; i < rdsize; i++)
		{
			//利用到了直接定值法，以字符的ASCLL码值来进行快速的统计
			fileByteInfo[ReadBuff[i]].appearCount++;
		}
	}
	HuffmanTree<ByteInfo> ht;
	ByteInfo invaild;

	//在创建哈夫曼数时，将出现次数为0字符再没有处理的情况下，需要将出现次数为0的字符剔除掉。
	ht.CreateHuffmanTree(fileByteInfo, 256,invaild);
	//3、借助哈夫曼树获取每个字节的编码
	GenerateHuffmenCode(ht.GetRoot());

	//4、借助字节编码对源文件进行改写
	//写解压需要的头部信息
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

		//用编码改写字节，将改写后的结果需要放置到压缩文件中保存
		for (size_t i = 0; i < rdsize; i++)
		{
			//用编码改写，readbuff[i] -----> 'A' ----->"100"
			string& strCode = fileByteInfo[ReadBuff[i]].strCode;
			
			//只需要将字符串格式的二进制编码存储在字节中就行了
			for (size_t j = 0; j < strCode.size(); j++)
			{
				ch <<= 1;  //高位丢弃，低位补0
				if ('1' == strCode[j])
					ch |= 1;

			 // 当ch中的8个比特位充满后，需要将该字节写入文件当中
			 //汉字排列在Ascll码之后，所以它表示的下标大于128，是一个负数，所以需要将有符号改为无符号
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
	//1、获取源文件的后缀
	string postFix = FilePath.substr(FilePath.rfind('.'));
	postFix += "\n";
	fwrite(postFix.c_str(), 1, postFix.size(), fOut);

	//构造字符频次信息，统计有效字符的个数和对应编码
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
	//写总行数和频次信息
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
// 文件解压缩
bool FileCompress::UNCompressFile(const string& filePath)
{
	// 1.从压缩文件中读取解压缩时需要使用的信息
	FILE* fIn = fopen(filePath.c_str(), "rb");
	if (fIn < 0)
	{
		cout << "打开压缩文件失败" << endl;
		return false;
	}
	//  读取源文件后缀
	string postFix;
	GetLine(fIn, postFix);
	// 读取频次信息总行数
	string strContent;
	GetLine(fIn, strContent);
	size_t lineCount = atoi(strContent.c_str());
	//循环读取linecount行；获取字节的频次信息
	strContent = "";
	for (size_t i = 0; i < lineCount; ++i)
	{
		GetLine(fIn, strContent);
		if ("" == strContent)
		{
			//说明刚刚读取到的是一个换行
			strContent += '\n';
			GetLine(fIn, strContent);
		}
		fileByteInfo[(unsigned char)strContent[0]].appearCount = atoi(strContent.c_str() + 2);
		strContent = "";
	}

	// 2.利用得到的信息恢复huffman树
	ByteInfo invaild;
	HuffmanTree<ByteInfo> ht;
	ht.CreateHuffmanTree(fileByteInfo, 256,invaild);
	// 3.读取压缩数据，结合Huffman树进行解压缩
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
			//逐字节解压缩
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

	//所有有效的权值为叶子节点的为止，当遍历到叶子结点的位置时，该权值对应的编码就拿到了
	if (root->left == nullptr && root->right == nullptr)
	{
		HuffmanTreeNode<ByteInfo>* cur = root;
		HuffmanTreeNode<ByteInfo>* parent = cur->parent;
		//fileByteInfo[cur->weight._ch].strCode     
		//最终获取到的编码需要保存到fileByteInfo对应的位置上
		//cur为当前的节点，
		//cur->weight->_ch表示的是该位置上的元素
		//strCode是编码的别名
		string& strCode = fileByteInfo[cur->weight._ch].strCode; //保存编码
		while (parent) //获取到的是一个反向的编码
		{
			if (cur == parent->left)
				strCode += '0';
			else
				strCode += '1';
			cur = parent;
			parent = cur->parent;
		}
		//将字符串逆置
		reverse(strCode.begin(), strCode.end());
	}

	GenerateHuffmenCode(root->left);
	GenerateHuffmenCode(root->right);
}