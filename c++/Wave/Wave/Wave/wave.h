#pragma pack(1)
#pragma once
#include "def.h"

bool CompFStr(LPCSTR s, LPCSTR p)
{
	if (strlen(s) == 0 || strlen(p) == 0){ DEBUG::wrn("in Function CompFStr(): \n\tThe string is NULL !"); return true; }
	if (strlen(p) > strlen(s)){ DEBUG::wrn("in Function CompFStr(): \n\tThe PARA p is longer than the PARA s !"); return false; }
	for (int i = 0, j = 0; j < strlen(p); i++, j++)
	if (s[i] != p[j])
		return false;
	return true;
}

//RIFF WAVE Chunk 文件格式标记  
struct RIFF_HEADER
{
	char szRiff[4];       //'R','I','F','F'  
	DWORD dwRiffSize;
	char szRiffFormat[4]; //'W','A','V','E'  
};
RIFF_HEADER*Header = new RIFF_HEADER;
struct WAVE_FORMAT//PCM WAV格式说明  
{
	WORD  wFormatTag;          //编码方式-一般为PCM=0x0001,-------其他压缩编码如ADPCM 见后面说明  
	WORD  wChannels;           //声道数目，1--单声道，2--双声道  
	DWORD dwSamplesPerSec;     //采样频率  
	DWORD dwAvgBytesPerSec;    //每秒所需字节数  
	WORD  wBlockAlign;         // 数据块对其单位  
	WORD  wBitsPerSample;      //每个采样需要的bit数  
	//WORD  wAppend;           //附加信息（可选，通过Size来判断有无）PCM中忽略此值  
};
//Format Chunk   
struct FMT_BLOCK
{
	char  szFmtID[4];          //'f' 'm' 't' ' '   
	DWORD Size;                //数值为16或18，18则最后又附加信息  
	WAVE_FORMAT wavFormat;
};
FMT_BLOCK*Fmt = new FMT_BLOCK;
//FACT Chunk 可选字段，一般当wav文件由某些软件转化而成，则包含该Chunk  
struct FACT_BLOCK
{
	char szFactID[4];         //'f' 'a' 'c' 't'  
	DWORD dwFactSize;
};
FACT_BLOCK*Fact = new FACT_BLOCK;
//Data Chunk   
struct DATA_BLOCK
{
	char szDataID[4];         //'d' 'a' 't' 'a'  
	DWORD dwDataSize;
};
DATA_BLOCK*Data = new DATA_BLOCK;
struct SOUND_DATA
{
	int channel;
	int BitsPerSample;
	int BytesPerSample;
	int num;
	short *ldata, *rdata;
	long double TimeLast;
};
SOUND_DATA*Sound = new SOUND_DATA;

void ReadWavePackage(LPCSTR FileName)
{
	FILE*fIn = fopen(FileName,"rb");
	if (fIn==NULL)DEBUG::err("Open the wave file failed !");

	fread((char*)Header, sizeof(RIFF_HEADER),1,fIn);
	if (!CompFStr(Header->szRiff, "RIFF"))DEBUG::err("This is not a RIFF file !");
	if (!CompFStr(Header->szRiffFormat, "WAVE"))DEBUG::err("This is not a wave file !");
	fseek(fIn, 0, SEEK_END);
	long endpos = ftell(fIn);
	fseek(fIn,sizeof(RIFF_HEADER),SEEK_SET);
	if (endpos - 8 != Header->dwRiffSize){ cout << "True," << endpos - 8 << "  Found," << Header->dwRiffSize << " :"; DEBUG::wrn("dwRiffSize check failed in Block Header !"); }
	DEBUG::info("RIFF and WAVE checked !");

	
	fread((char*)Fmt, sizeof(FMT_BLOCK),1,fIn);
	if (!CompFStr(Fmt->szFmtID, "fmt"))DEBUG::err("The block FMT was crashed !");
	DEBUG::info("FMT Block checked !");


	if (Fmt->wavFormat.wFormatTag != 0x0001){ cout << Fmt->wavFormat.wFormatTag << ": "; DEBUG::err("This is not a PCM file !"); }
	if (Fmt->Size == 18){ fgetc(fIn); fgetc(fIn); Fmt->Size = 16; DEBUG::wrn("Skipped two bytes."); }//Skip the extra information
	long position = ftell(fIn);
	
	fread((char*)Fact, sizeof(FACT_BLOCK),1,fIn);
	if (!CompFStr(Fact->szFactID, "fact")){ DEBUG::info("This file have no FACT Chunk !"); Fact = NULL; fseek(fIn,position,SEEK_SET); }
againdata:
	
	fread((char*)Data, sizeof(DATA_BLOCK),1,fIn);
	if (!CompFStr(Data->szDataID, "data"))
	{ 
		cout << Data->szDataID << ": ";
		if(!CompFStr(Data->szDataID,"LIST"))DEBUG::err("The block DATA was crashed !");
		else
		{
			DEBUG::wrn("No found the block DATA but found the block LIST.\nTry to skip it.");
			long position = ftell(fIn);
			fseek(fIn,position + Data->dwDataSize,SEEK_SET);
			Header->dwRiffSize -= (Data->dwDataSize+4);
			goto againdata;
		}
	}
	long nowpos = ftell(fIn);
	if (endpos - nowpos != Data->dwDataSize){ cout << "True,"<<endpos - nowpos << "  Found," << Data->dwDataSize << " :"; DEBUG::wrn("dwDataSize check failed in Block Data !"); }
	DEBUG::info("DATA Block checked !");
	BYTE a[2] = { 0 };
	BYTE b = NULL;
	
	Sound->channel=Fmt->wavFormat.wChannels;
	Sound->BitsPerSample=Fmt->wavFormat.wBitsPerSample;

	Sound->BytesPerSample = Sound->BitsPerSample / 8;
	Sound->num = Data->dwDataSize / Sound->BytesPerSample;
	Sound->TimeLast = (long double)Data->dwDataSize /(long double)Fmt->wavFormat.dwAvgBytesPerSec;
	
	cout << "Loading sound datas ... ";
	if (Sound->channel == 1)
	{
		Sound->ldata = new short[Sound->num];
		if (Sound->BytesPerSample == 1)
		{
			for (int i = 0; i < Sound->num; i++)
			{
				b = fgetc(fIn);
				if (b>255 || b < 0){ cout << i << " ," << (long)b << " :"; DEBUG::wrn("Sound data crashes !"); }
				Sound->ldata[i] = b /*-128*/;
			}
		}
		else if (Sound->BytesPerSample == 2)
		{
			for (int i = 0; i < Sound->num; i++)
			{
				b = fgetc(fIn);
				Sound->ldata[i] = b;
				b = fgetc(fIn);
				Sound->ldata[i] = MakeByPart(b,Sound->ldata[i]);
				if (Sound->ldata[i]>32767 || Sound->ldata[i] < -32768){ cout << i << " ," << (long)b << " :"; DEBUG::wrn("Sound data crashes !"); }
			}
		}
		else{ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); cout << Sound->BitsPerSample << ": "; DEBUG::err("The bits is not support !"); }
	}
	else if (Sound->channel == 2)
	{
		Sound->ldata = new short[Sound->num/2];
		Sound->rdata = new short[Sound->num/2];
		if (Sound->BytesPerSample == 1)
		{
			for (int i = 0; i < Sound->num/2; i++)
			{
				b = fgetc(fIn);
				Sound->ldata[i] = b /*- 128*/;
				if (b>255 || b < 0){ cout << i << "L ,"<<(long)b<<" :"; DEBUG::wrn("Sound data crashes !"); }
				b = fgetc(fIn);
				Sound->rdata[i] = b /*- 128*/;
				if (b>255 || b < 0){ cout << i << "R ,"<<(long)b<<" :"; DEBUG::wrn("Sound data crashes !"); }
			}
		}
		else if (Sound->BytesPerSample == 2)
		{
			for (int i = 0; i < Sound->num/2; i++)
			{
				b = fgetc(fIn);
				Sound->ldata[i] = b;
				b = fgetc(fIn);
				Sound->ldata[i] = MakeByPart(b, Sound->ldata[i]);
				if (Sound->ldata[i]>32767 || Sound->ldata[i] < -32768){ cout << i << "L ," << (long)b << " :"; DEBUG::wrn("Sound data crashes !"); }
				b = fgetc(fIn);
				Sound->rdata[i] = b;
				b = fgetc(fIn);
				Sound->rdata[i] = MakeByPart(b, Sound->rdata[i]);
				if (Sound->rdata[i]>32767 || Sound->rdata[i] < -32768){ cout << i << "R ," << (long)b << " :"; DEBUG::wrn("Sound data crashes !"); }
			}
		}
		else{ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); cout << Sound->BitsPerSample << ": "; DEBUG::err("The bits is not support !"); }
	}
	else{ ChangeColor(COLOR_RED); cout << "Failed.\n"; ChangeColor(COLOR_WHITE); cout << Sound->channel << ": "; DEBUG::err("The number of channel is not support !"); }
	ChangeColor(COLOR_GREEN); cout << "OK.\n"; ChangeColor(COLOR_WHITE);

	DEBUG::info("Read sound datas successfully !");
	fclose(fIn);
}