#pragma warning(disable:4996)
#pragma once

#include "debug.h"
#include "wave.h"
#include "def.h"
#include "draw.h"



char*str_replace(char*source, char*sub, char*rep)
{
	char*result;
	/*pc1是复制到结果result的扫描指针*/
	/*pc2是扫描 source 的辅助指针*/
	/*pc3寻找子串时,为检查变化中的source是否与子串相等,是指向sub的扫描指针 */
	/*找到匹配后,为了复制到结果串,是指向rep的扫描指针*/
	char*pc1, *pc2, *pc3;
	int isource, isub, irep;
	isub = strlen(sub);	/*对比字符串的长度*/
	irep = strlen(rep);	/*替换字符串的长度*/
	isource = strlen(source);/*源字符串的长度*/
	if (NULL == *sub)
		return strdup(source);
	/*申请结果串需要的空间*/
	result = (char*)malloc(((irep > isub) ? float(strlen(source)) / isub* irep + 1 : isource) * sizeof(char));
	pc1 = result;/*为pc1依次复制结果串的每个字节作准备*/
	while (*source != NULL)
	{
		/*为检查source与sub是否相等作准备,为pc2,pc3 赋初值*/
		pc2 = source;
		pc3 = sub;
		/* 出循环的（任一）条件是：
		**pc2不等于 *pc3（与子串不相等）
		*pc2到源串结尾
		*pc3到源串结尾（此时,检查了全部子串,source处与sub相等）
		*****************************************************/
		while (*pc2 == *pc3&&*pc3 != NULL&&*pc2 != NULL)
			pc2++, pc3++;
		/* 如果找到了子串,进行以下处理工作*/
		if (NULL == *pc3)
		{
			pc3 = rep;
			/*将替代串追加到结果串*/
			while (*pc3 != NULL)
				*pc1++ = *pc3++;
			pc2--;
			source = pc2;
			/*检查 source与sub相等的循环结束后，
			* pc2 对应的位置是在 sub 中串结束符处。该是源串中下一个位置。
			* 将source 指向其前面一个字符。
			***************************************************/
		}
		else /*如果没找到子串,下面复制source所指的字节到结果串*/
			*pc1++ = *source;
		source++; /* 将source向后移一个字符*/
	}
	*pc1 = NULL;
	return result;
}
void ChangeFileExt(LPSTR *File,LPCSTR Ext)
{
	char*tmp = NULL;
	*File = str_replace(*File, "/", "\\");
	tmp = strrchr(*File,'\\');
	if (tmp != NULL)
	{
		tmp++;
	}
	DEBUG::info("Apply that the output file is '" + (string)tmp + "'");
	tmp = NULL;
	tmp = strrchr(*File,'.');
	if (tmp != NULL)*(++tmp) = '\0';
	strcat(*File, Ext);
}
bool CompStr(LPCSTR s, LPCSTR p)
{
	if (strlen(s) == 0 || strlen(p) == 0){ DEBUG::wrn("in Function CompStr(): \n\tThe string is NULL !"); return true; }
	if (strlen(p) > strlen(s)){ DEBUG::wrn("in Function CompStr(): \n\tThe PARA p is longer than the PARA s !"); return false; }
	for (int i = strlen(s) - 1, j = strlen(p) - 1; j >= 0;i--,j--)
		if (s[i] != p[j])
			return false;
	return true;
}
bool FileExists(LPCSTR s)
{
	FILE*fp = fopen(s,"r");
	if (fp == NULL){ fclose(fp); return false; }
	fclose(fp);
	return true;
}
void usage()
{
	printf("Wave process program version '%s'\n",VERSION);
	printf("Made by MH\n");
	printf("Usage: wave.exe (FileName) (Command) [Option]\n");
	printf("\tFileName: Necessary, the wave file that you want to command.(ENGLISH !)\n");
	printf("\tCommand: Necessary(or Optional:Decide by File Extention), the command that you want to do in the wave file.\n");
	printf("\t\tPack: Change the wave file into a custom file\n\t\tUnpack: Change the file '*.%s' back into '*.wav'\n\t\tDraw: Draw a picture of the wave\n\t\tPlay: Play a wave sound.\n\t\tto8bit: Convert a 16bit wave file to 8bit\n\t\tAddVol: Add the volume of this wave sound(-vol [+ or - number])\n",PACK_FILEEXT);
	printf("\tOption: Optional, use following options---\n");
	printf("\t\t-o [OutputFile]\n\t\t-noload: Don't load the wave file(when play)\n\t\t-repeat [number>0 or number=-1]: The times to repeat(when play), -1 means INFINITE\n\t\t-vol [+ or - number(0<N<255 or -32768<N<32767)]: When command is AddVoln\t\t");
	cout << endl;
	printf("For example: wave.exe 1.wav AddVol -o 2.wav -vol 1.7\n");
	cout << endl;
}