#pragma once
#include "debug.h"
#include "bmp.h"
#include "network.h"
#include <iostream>
#include <algorithm>

UINT bit_reverse(UINT);//From MSB to LSB
//void RandomArray(std::vector< std::vector<double> >&,std::vector<int>&);
const long perSample=60000;
#define MAXIMUM_DIFFERENCE 0.1
#define MAXIMUM_ITERATOR 0xFFFFFFFF

int main(int argc,char**argv)
{
	DEBUG::add("main.cpp"); 
	
	if(argc==1 || argc>3)
	{
		DEBUG::info("Usage: neural-networks.exe [BMP file]\n\tTESTING mode for default.\n\tTraining mode with parameter [train_pic_data] [train_lab_data] (for mnist)\n");
		DEBUG::del();
		
		printf("Press any key to exit ...");
		getch();
		return 0;
	}
	if(argc==3 && strlen(argv[2])!=1) //training_mode
	{
		DeleteFile(SAVE_FILENAME);
		DEBUG::DEBUG_LEVEL=DEBUG_LEVEL_NECESSARY;
		
		char picdata[MAX_PATH+1]={0};
		char labdata[MAX_PATH+1]={0};
		strncpy(picdata,argv[1],MAX_PATH);
		strncpy(labdata,argv[2],MAX_PATH);
		FILE*fp=fopen(picdata,"rb");
		FILE*fl=fopen(labdata,"rb");
		if(fp==NULL || fl==NULL)
		{
			DEBUG::add("fopen()");
			DEBUG::fault("Error when opening training datas. (%d)",GetLastError());
		}
		long magic=0;
		fread(&magic,sizeof(long),1,fp);
		magic=bit_reverse(magic);
		if(magic!=0x803)
			DEBUG::fault("Bad file checked for train_pic_data ! (0x%x but 0x%x checked)",0x803,magic);
		magic=0;
		fread(&magic,sizeof(long),1,fl);
		magic=bit_reverse(magic);
		if(magic!=0x801)
			DEBUG::fault("Bad file checked for train_lab_data ! (0x%x but 0x%x checked)",0x801,magic);
		int num=0,t=0,t1=0;
		fread(&num,sizeof(int),1,fp);
		fread(&t,sizeof(int),1,fl);
		num=bit_reverse(num);t=bit_reverse(t);
		if(num!=t)
			DEBUG::fault("Bad item_number checked ! (%d and %d)",num,t);
		DEBUG::info("Total %d items are found.",num);
		t=0;
		fread(&t,sizeof(int),1,fp);
		fread(&t1,sizeof(int),1,fp);
		t=bit_reverse(t);t1=bit_reverse(t1);
		if(t!=BMP_WIDTH || t1!=BMP_HEIGHT)
			DEBUG::fault("%d*%d is required but %d*%d is checked !",BMP_WIDTH,BMP_HEIGHT,t,t1);
		
		
		NETWORK net;
		std::vector<double>a;
		a.clear();a.resize(BMP_LENGTH);
		
		bool ret=net.InitNetwork(BMP_LENGTH,4,16,a,0,perSample,0.9);
		if(!ret)
			DEBUG::fault("Error when initializing the network !");
		
		long tic=GetTickCount();
		
		for(int l=0;l<num/perSample;l++)
		{
			double cost=0xFFFF;
			std::vector< std::vector<double> >s_a;
			std::vector<int>s_l;
			s_a.clear();s_l.clear();
			for(int n=l*perSample;n<(l+1)*perSample;n++)
			{
				unsigned char lab=0;
				fread(&lab,sizeof(UCHAR),1,fl);
				fclose(fp);
				//Different BMP datas between loading file and files
				char st[10]={0};itoa(l*perSample+n+1,st,10);
				char path[MAX_PATH]={0};
				strcpy(path,"train\\");
				strcat(path,st);strcat(path,".bmp");
				BMP bmp;
				bool ret=bmp.openbmp(path);
				if(!ret)
					DEBUG::fault("Error when loading BMP file !");
				ret=bmp.convert();
				if(!ret)
					DEBUG::fault("Error when converting BMP datas !");
				for(int i=0;i<BMP_LENGTH;i++)
					a[i]=net.sigmoid(bmp.bDatas[i]);
				s_a.push_back(a);s_l.push_back((int)lab);
			}
			long iter=0;
			while(cost>MAXIMUM_DIFFERENCE && iter<=MAXIMUM_ITERATOR)
			{
				cost=0;
				for(int i=0;i<perSample;i++)
				{
					net.reset(s_a[i],s_l[i]);
					DEBUG::debuginfo("Training network with lab %d ... ",s_l[i]);
					net.backprop();
					cost+=net.cost;
				}
				
				net.update();
				
				net.clear_ok();
				//cost/=(double)perSample;
				iter++;
				if(iter%1==0)
					DEBUG::info("Training %d/%d with difference %.4lf from %.4lf with %d times",l+1,num/perSample,cost,net.cost,iter);
				if(iter%100==0)
				{
					DEBUG::info("Saving datas ...... ");
					net.save();
				}
			}
			DEBUG::info("Saving datas ...... ");
			net.save();
		}
		DEBUG::info("Train ended successfully ! (%ldsec)",(GetTickCount()-tic)/1000);
		DEBUG::info("Saving datas ...... ");
		net.save();
		/*NETWORK*np=&net;
		delete np;
		np=new NETWORK;
		net=*np;
		delete &np;
		ret=net.InitNetwork(BMP_LENGTH,4,16,a,0,perSample,0.9);
		if(!ret)
			DEBUG::fault("Error when initializing the network !");*/
		
		
		fclose(fp);fclose(fl);
		DEBUG::del();
		return 0;
	}
	
	
	char filename[MAX_PATH+1]={0};
	int realnum=atoi(argv[1]);
	strncpy(filename,argv[1],MAX_PATH);
	
	BMP bmp;
	bool ret=bmp.openbmp(filename);
	if(!ret)
		DEBUG::fault("Error when loading BMP file !");
	ret=bmp.convert();
	if(!ret)
		DEBUG::fault("Error when converting BMP datas !");
	
	
	NETWORK net;
	std::vector<double>a;
	for(int i=0;i<BMP_LENGTH;i++)
		a.push_back(net.sigmoid(bmp.bDatas[i]));
	delete &bmp;
	
	ret=net.InitNetwork(BMP_LENGTH,4,16,a,realnum);
	if(!ret)
		DEBUG::fault("Error when initializing the network !"); 
	
	net.forwardprop();
	//net.dump(1);
	
	net.result_dump();
	printf("The number is: %d\n",net.result());

	DEBUG::del();
	return 0;
}

unsigned int bit_reverse(unsigned int x)
{
	char*f=(char*)&x;
	std::reverse(f,f+sizeof(UINT));
	return x;
}

/*void RandomArray(std::vector< std::vector<double> >&o,std::vector<int>&l)
{
	srand(unsigned(time(NULL)));
	std::vector< std::vector<double> >no=o;no.clear();
	std::vector<int>nl;nl.clear();
    for(int i=o.size();i>0;i--)
	{
        int index=rand()%i;
        no.push_back(o[index]);
        nl.push_back(l[index]);
        o.erase(o.begin()+index);
        l.erase(l.begin()+index);
    }
    o=no;l=nl;
    return ;
}*/
