//所有的文件都必须按照二进制文件的格式进行读写操作。
#include<iostream>
#include"FileCompress.h"
#include"HuffmanTree.hpp"
int main()
{
	FileCompress fc;
	fc.CompressFile("1.txt");
	fc.UNCompressFile("2.hzp");
	return 0;
}
//多行文本的话解压出现了乱码。最直观的排错就是检查压缩和解压缩哈夫曼树是否一致
//只需要比较压缩和解压缩时的频次信息是否一样