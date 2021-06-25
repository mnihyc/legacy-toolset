#pragma warning(disable:4996)
#pragma once

#include "debug.h"
#include "wave.h"
#include "def.h"
#include "draw.h"



char*str_replace(char*source, char*sub, char*rep)
{
	char*result;
	/*pc1�Ǹ��Ƶ����result��ɨ��ָ��*/
	/*pc2��ɨ�� source �ĸ���ָ��*/
	/*pc3Ѱ���Ӵ�ʱ,Ϊ���仯�е�source�Ƿ����Ӵ����,��ָ��sub��ɨ��ָ�� */
	/*�ҵ�ƥ���,Ϊ�˸��Ƶ������,��ָ��rep��ɨ��ָ��*/
	char*pc1, *pc2, *pc3;
	int isource, isub, irep;
	isub = strlen(sub);	/*�Ա��ַ����ĳ���*/
	irep = strlen(rep);	/*�滻�ַ����ĳ���*/
	isource = strlen(source);/*Դ�ַ����ĳ���*/
	if (NULL == *sub)
		return strdup(source);
	/*����������Ҫ�Ŀռ�*/
	result = (char*)malloc(((irep > isub) ? float(strlen(source)) / isub* irep + 1 : isource) * sizeof(char));
	pc1 = result;/*Ϊpc1���θ��ƽ������ÿ���ֽ���׼��*/
	while (*source != NULL)
	{
		/*Ϊ���source��sub�Ƿ������׼��,Ϊpc2,pc3 ����ֵ*/
		pc2 = source;
		pc3 = sub;
		/* ��ѭ���ģ���һ�������ǣ�
		**pc2������ *pc3�����Ӵ�����ȣ�
		*pc2��Դ����β
		*pc3��Դ����β����ʱ,�����ȫ���Ӵ�,source����sub��ȣ�
		*****************************************************/
		while (*pc2 == *pc3&&*pc3 != NULL&&*pc2 != NULL)
			pc2++, pc3++;
		/* ����ҵ����Ӵ�,�������´�����*/
		if (NULL == *pc3)
		{
			pc3 = rep;
			/*�������׷�ӵ������*/
			while (*pc3 != NULL)
				*pc1++ = *pc3++;
			pc2--;
			source = pc2;
			/*��� source��sub��ȵ�ѭ��������
			* pc2 ��Ӧ��λ������ sub �д���������������Դ������һ��λ�á�
			* ��source ָ����ǰ��һ���ַ���
			***************************************************/
		}
		else /*���û�ҵ��Ӵ�,���渴��source��ָ���ֽڵ������*/
			*pc1++ = *source;
		source++; /* ��source�����һ���ַ�*/
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