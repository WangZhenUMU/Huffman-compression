//���е��ļ������밴�ն������ļ��ĸ�ʽ���ж�д������
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
//�����ı��Ļ���ѹ���������롣��ֱ�۵��Ŵ���Ǽ��ѹ���ͽ�ѹ�����������Ƿ�һ��
//ֻ��Ҫ�Ƚ�ѹ���ͽ�ѹ��ʱ��Ƶ����Ϣ�Ƿ�һ��