#ifndef _H_BMP
#define _H_BMP

#include "debug.h"
#include <cstdio>
#include <cstdlib>

#define BMP_WIDTH 28
#define BMP_HEIGHT 28
#define BMP_LENGTH (BMP_WIDTH*BMP_HEIGHT)

class BMP
{
	public:
		unsigned char bDatas[BMP_LENGTH+1];
		unsigned char oDatas[BMP_HEIGHT+1][BMP_WIDTH*3+1];
		
		BMP();
		bool openbmp(char[]);
		bool convert();
		bool savebmp(char[]);
		
	private:
		BITMAPFILEHEADER bf;
		BITMAPINFOHEADER bi;
		char filename[MAX_PATH+1];
		
};


BMP::BMP()
{
	memset(&bf,0,sizeof(BITMAPFILEHEADER));
	memset(&bi,0,sizeof(BITMAPINFOHEADER));
	memset(bDatas,0,sizeof(bDatas));
	memset(oDatas,0,sizeof(oDatas));
}

bool BMP::openbmp(char filename[])
{
	DEBUG::add("bmp.h -> openbmp()");
	strncpy(this->filename,filename,MAX_PATH);
	
	FILE*fp=fopen(filename,"rb");
	if(fp==NULL)
	{
		DEBUG::add("fopen()");
		DEBUG::error("Couldn't load BMP file (\"%s\") with error=%d",filename,GetLastError());
		DEBUG::del(2);
		return false;
	}
	
	int ret=fread(&bf,sizeof(BITMAPFILEHEADER),1,fp);
	if(ret!=1)
	{
		DEBUG::add("fread()");
		DEBUG::error("Couldn't read struct BITMAPFILEHEADER with error=%d",GetLastError());
		DEBUG::del(2);
		return false; 
	}
	
	ret=fread(&bi,sizeof(BITMAPINFOHEADER),1,fp);
	if(ret!=1)
	{
		DEBUG::add("fread()");
		DEBUG::error("Couldn't read struct BITMAPINFOHEADER with error=%d",GetLastError());
		DEBUG::del(2);
		return false; 
	}
	
	if(bi.biWidth!=BMP_WIDTH || bi.biHeight!=BMP_HEIGHT)
	{
		DEBUG::error("We only support BMP file with %d*%d but %d*%d was checked.",BMP_HEIGHT,BMP_WIDTH,bi.biHeight,bi.biWidth);
		DEBUG::del();
		return false;
	}
	
	if(bi.biBitCount!=8)
	{
		for(int i=0;i<bi.biHeight;i++)
			for(int j=0;j<bi.biWidth*3;j++)
			{
				ret=fread(&oDatas[i][j],sizeof(unsigned char),1,fp);
				if(ret!=1)
				{
					DEBUG::add("fread()");
					DEBUG::error("Couldn't read hex at (%d,%d) with error=%d",i,j,GetLastError());
					DEBUG::del(2);
					return false;
				}
			}
	}else
	{
		RGBQUAD*pColorTable=new RGBQUAD[256];
		fread(pColorTable,sizeof(RGBQUAD),256,fp);
		
		for(int i=0;i<bi.biHeight;i++)
			for(int j=0;j<bi.biWidth;j++)
			{
				ret=fread(&oDatas[i][j],sizeof(unsigned char),1,fp);
				if(ret!=1)
				{
					DEBUG::add("fread()");
					DEBUG::error("Couldn't read hex at (%d,%d) with error=%d",i,j,GetLastError());
					DEBUG::del(2);
					return false;
				}
			}
	}
	if(!feof(fp))
		DEBUG::warning("There're some extra datas at the end of the file");
	fclose(fp);
	
	DEBUG::info("Load BMP file successfully !"); 
	
	DEBUG::del();
	return true;
}


bool BMP::convert()
{
	DEBUG::add("bmp.h -> convert()");
	
	if(bi.biBitCount!=8)
	{
		for(int i=0;i<BMP_HEIGHT;i++)
			for(int j=0;j<BMP_WIDTH;j++)
			{
				bDatas[i*BMP_HEIGHT+j]=oDatas[i][j*3]*0.114+oDatas[i][j*3+1]*0.587+oDatas[i][j*3+2]*0.299;
				if(bDatas[i*BMP_HEIGHT+j]!=0)
					bDatas[i*BMP_HEIGHT+j]=255;
			}
	}
	else
	{
		DEBUG::warning("It's already a 8-bit BMP.");
		for(int i=0;i<BMP_HEIGHT;i++)
			for(int j=0;j<BMP_WIDTH;j++)
			{
				bDatas[i*BMP_HEIGHT+j]=oDatas[i][j];
				if(bDatas[i*BMP_HEIGHT+j]!=0)
					bDatas[i*BMP_HEIGHT+j]=255;
			}
	}
	
	DEBUG::info("Convert BMP datas successfully !");
	
	DEBUG::del();
	return true;
}


bool BMP::savebmp(char filename[])
{
	DEBUG::add("bmp.h -> savebmp()");
	
	FILE*fp=fopen(filename,"wb");
	if(fp==NULL)
	{
		DEBUG::add("fopen()");
		DEBUG::error("Couldn't open BMP file for writing (\"%s\") with error=%d",filename,GetLastError());
		DEBUG::del(2);
		return false;
	}
	
	BITMAPFILEHEADER newbf;
	memcpy(&newbf,&bf,sizeof(BITMAPFILEHEADER));
	newbf.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	newbf.bfSize=bi.biWidth*bi.biHeight+newbf.bfOffBits;
	BITMAPINFOHEADER newbi;
	memcpy(&newbi,&bi,sizeof(BITMAPINFOHEADER));
	newbi.biBitCount=8;
	
	int ret=fwrite(&newbf,sizeof(BITMAPFILEHEADER),1,fp);
	if(ret!=1)
	{
		DEBUG::add("fwrite()");
		DEBUG::error("Couldn't write struct BITMAPFILEHEADER with error=%d",GetLastError());
		DEBUG::del(2);
		return false; 
	}
	
	ret=fwrite(&newbi,sizeof(BITMAPINFOHEADER),1,fp);
	if(ret!=1)
	{
		DEBUG::add("fwrite()");
		DEBUG::error("Couldn't write struct BITMAPINFOHEADER with error=%d",GetLastError());
		DEBUG::del(2);
		return false; 
	}
	
	RGBQUAD*pRGB=new RGBQUAD[256];
    for(int i=0;i!=256;i++)
    	pRGB[i].rgbRed=pRGB[i].rgbGreen=pRGB[i].rgbBlue=i;
    ret=fwrite(pRGB,sizeof(RGBQUAD),256,fp);
	if(ret!=256)
	{
		DEBUG::add("fwrite()");
		DEBUG::error("Couldn't write struct RGBQUAD with error=%d",GetLastError());
		DEBUG::del(2);
		return false; 
	}
	
	ret=fwrite(&bDatas[0],BMP_LENGTH*sizeof(unsigned char),1,fp);
	if(ret!=1)
	{
		DEBUG::add("fwrite()");
		DEBUG::error("Couldn't write hex datas with error=%d",GetLastError());
		DEBUG::del(2);
		return false; 
	}
	fclose(fp);
	
	DEBUG::info("Write BMP file successfully !");
	
	DEBUG::del();
	return true;
}


#endif
