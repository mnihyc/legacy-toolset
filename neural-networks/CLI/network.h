#ifndef _H_NETWORK
#define _H_NETWORK

#include "debug.h"
#include <assert.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>


#define SAVE_FILENAME "svf.bin" 
class NETWORK
{
	public:
		inline double sigmoid(double x)
		{
			return (1.0/(1.0+exp(-x)));
		}
		inline double dsigmoid(double x)
		{
			x=sigmoid(x);
			return (x*(1.0-x));
		}
		struct _NormalLayer{
			std::vector<double>a;
			std::vector<double>a1;					//Datas without sigmoid()
			std::vector< std::vector<double> >w;	//From this node to the next w.size() nodes, except the output layer, including the input layer
			std::vector<double>b;					//Bias in each node, except the input layer, including the output layer
			std::vector< std::vector<double> >wDelta;
			std::vector<double>bDelta;
		};
		struct _OutputLayer{
			std::vector<double>a;
		};
		
		void dump(bool);
		void result_dump();
		
		bool InitNetwork(int,int,int,std::vector<double>,int,int,double);
		void forwardprop();
		double cost;
		void backprop();
		inline void clear();
		inline void clear_ok();
		inline bool reset(std::vector<double>,int);
		inline void update();
		void save();
		void load();
		int result();
		
	private:
		int InputLayerNodeNum;
		int HiddenLayerNodeNum;
		int OutputLayerNodeNum;
		int TotalLayerNum;
		int perSample;
		double learningRate;
		
		std::vector<_NormalLayer>NormalLayer;		//Except the output layer, including the input layer
		//_OutputLayer OutputLayer;
		_OutputLayer ExpectedOutputLayer;
		
		
}; 

void NETWORK::dump(bool full=false)
{
	DEBUG::debuginfo("dump of the whole network");
	for(int i=0;i<TotalLayerNum;i++)
	{
		printf("%d\n",i);
		printf("%d %d %d %d\n",NormalLayer[i].a.size(),NormalLayer[i].b.size(),NormalLayer[i].w.size(),NormalLayer[i].w[0].size());
		if(!full)
		 	continue;
		printf("a\n");
		for(int j=0;j<NormalLayer[i].a.size();j++)
			printf("%.2lf ",NormalLayer[i].a[j]);
		printf("\na1\n");
		for(int j=0;j<NormalLayer[i].a1.size();j++)
			printf("%.2lf ",NormalLayer[i].a1[j]);
		printf("\nb\n");
		for(int j=0;j<NormalLayer[i].b.size();j++)
			printf("%.2lf ",NormalLayer[i].b[j]);
		printf("\nw\n");
		for(int j=0;j<NormalLayer[i].w.size();j++)
		{
			for(int k=0;k<NormalLayer[i].w[j].size();k++)
				printf("%.2lf ",NormalLayer[i].w[j][k]);
			printf("\n");
		}
		printf("\n");
		printf("\n\n");
	}
}
void NETWORK::result_dump()
{
	for(int i=0;i<OutputLayerNodeNum;i++)
		printf("  %d  ",i);
	printf("\n");
	for(int i=0;i<OutputLayerNodeNum;i++)
		printf("%.2lf ",NormalLayer[TotalLayerNum-1].a[i]);
	printf("\n");
}

bool NETWORK::InitNetwork(int inputs,int layers,int nodes,std::vector<double>a,int expected=0,int perSample=0,double learningRate=0)
{
	DEBUG::add("network.h -> InitNetwork()");
	
	int &n=TotalLayerNum;
	int &on=OutputLayerNodeNum;
	int &in=InputLayerNodeNum;
	int &hn=HiddenLayerNodeNum; 
	std::vector<_NormalLayer> &nl=NormalLayer;
	
	on=10;
	in=inputs;
	n=layers;
	hn=nodes;
	this->perSample=perSample;
	this->learningRate=learningRate;
	if(n<=2 || hn<=0 || a.size()!=in || expected<0 || expected>9)
	{
		DEBUG::error("Bad arguments !");
		DEBUG::del();
		return false;
	}
	ExpectedOutputLayer.a.resize(on);
	for(int i=0;i<ExpectedOutputLayer.a.size();i++)
		ExpectedOutputLayer.a[i]=0;
	ExpectedOutputLayer.a[expected]=1;
	
	//Construct network nodes
	
	_NormalLayer tn;
	tn.a=a;
	tn.a1=a;
	tn.b.resize(in);		//Useless
	tn.w.resize(in);
	tn.bDelta.resize(in);
	tn.wDelta.resize(in);
	for(int i=0;i<tn.w.size();i++)
	{
		tn.w[i].resize(hn);
		tn.wDelta[i].resize(hn);
	}
	nl.push_back(tn);					//The input layer
	tn.a.clear();
	tn.a.resize(hn);
	tn.a1.clear();
	tn.a1.resize(hn);
	for(int i=0;i<tn.a.size();i++)
		tn.a[i]=tn.a1[i]=0;
	tn.b.resize(hn);
	tn.bDelta.resize(hn);
	tn.w.resize(hn);
	for(int i=0;i<tn.w.size();i++)
	{
		tn.w[i].resize(hn);
		tn.wDelta[i].resize(hn);
	}
	for(int i=1;i<n;i++)			//The hidden layer
		nl.push_back(tn);
	nl[n-1].w.clear();		//The output layer
	nl[n-1].b.clear();
	nl[n-1].b.resize(on);
	nl[n-1].wDelta.clear();
	nl[n-1].bDelta.clear();
	nl[n-1].bDelta.resize(on);
	nl[n-1].a.clear();
	nl[n-1].a.resize(on);
	nl[n-1].a1.clear();
	nl[n-1].a1.resize(on);
	for(int i=0;i<on;i++)
		nl[n-1].a[i]=0;
	nl[n-2].w.clear();
	nl[n-2].w.resize(hn);
	nl[n-2].wDelta.clear();
	nl[n-2].wDelta.resize(hn);
	for(int i=0;i<tn.w.size();i++)
	{
		nl[n-2].w[i].resize(on);
		nl[n-2].wDelta[i].resize(on);
	}
	nl[n-2].b.clear();
	nl[n-2].b.resize(hn);
	nl[n-2].bDelta.clear();
	nl[n-2].bDelta.resize(hn);
	
	//Set values randomly
	srand((unsigned)time(NULL));
	for(int i=0;i<n;i++)
		for(int j=0;j<nl[i].w.size();j++)
		{
			for(int k=0;k<nl[i].w[j].size();k++)
			{
				nl[i].w[j][k]=(2.0*rand()/double(RAND_MAX)-1);
				nl[i].wDelta[j][k]=0;
			}
			nl[i].b[j]=(2.0*rand()/double(RAND_MAX)-1);
			nl[i].b[j]=0;
		}
	
	DEBUG::info("Initialize the network successfully !");
	
	load();
	
	DEBUG::del();
	return true;
}

void NETWORK::forwardprop()
{
	if(DEBUG::DebugLists[DEBUG::DebugListNum-1]=="network.h -> backprop()")
		DEBUG::add("forwardprop()");
	else DEBUG::add("network.h -> forwardprop()");
	
	for(int l=1;l<TotalLayerNum;l++)
		for(int i=0;i<NormalLayer[l].a.size();i++)
		{
			NormalLayer[l].a[i]=0;
			for(int j=0;j<NormalLayer[l-1].a.size();j++)
				NormalLayer[l].a[i]+=(NormalLayer[l-1].a[j]*NormalLayer[l-1].w[j][i]);
			NormalLayer[l].a[i]+=NormalLayer[l].b[i];
			NormalLayer[l].a1[i]=NormalLayer[l].a[i];
			NormalLayer[l].a[i]=sigmoid(NormalLayer[l].a[i]);
		}
	
	cost=0;
	for(int i=0;i<OutputLayerNodeNum;i++)
		cost+=pow((NormalLayer[TotalLayerNum-1].a[i]-ExpectedOutputLayer.a[i]),2);
	DEBUG::debuginfo("Forward propagation succeed with difference(%.2lf) !",cost);
	
	DEBUG::del(); 
	return;
}

void NETWORK::backprop()
{
	DEBUG::add("network.h -> backprop()");
	
	forwardprop();
	
	double delta[TotalLayerNum][std::max(InputLayerNodeNum,std::max(HiddenLayerNodeNum,OutputLayerNodeNum))];
	for(int i=0;i<OutputLayerNodeNum;i++)
		delta[TotalLayerNum-1][i]=((NormalLayer[TotalLayerNum-1].a[i]-ExpectedOutputLayer.a[i]) * dsigmoid(NormalLayer[TotalLayerNum-1].a1[i]));
	//Compute dCost
	
	for(int i=TotalLayerNum-1-1;i>=0;i--)
		for(int j=0;j<NormalLayer[i].a.size();j++)
		{
			double sum=0;
			for(int k=0;k<NormalLayer[i+1].a.size();k++)
				sum+=delta[i+1][k]*NormalLayer[i].w[j][k];
			delta[i][j]=sum*dsigmoid(NormalLayer[i].a1[j]);
		}
	//Compute delta
	
	for(int i=TotalLayerNum-1-1;i>=0;i--)
		for(int j=0;j<NormalLayer[i].a.size();j++)
		{
			NormalLayer[i].bDelta[j]+=delta[i][j];
			for(int k=0;k<NormalLayer[i+1].a.size();k++)
				NormalLayer[i].wDelta[j][k]+=delta[i+1][k]*NormalLayer[i].a[j];
		}
	for(int j=0;j<OutputLayerNodeNum;j++)
		NormalLayer[TotalLayerNum-1].bDelta[j]+=delta[TotalLayerNum-1][j];
	//Compute each delta
	
	DEBUG::debuginfo("Back propagation succeed !");
	
	DEBUG::del();
	return; 
} 

inline void NETWORK::clear()
{
	for(int i=0+1;i<TotalLayerNum;i++)
		for(int j=0;j<NormalLayer[i].a.size();j++)
			NormalLayer[i].a[j]=0;
	return;
}

inline void NETWORK::clear_ok()
{
	for(int i=0;i<TotalLayerNum;i++)
		for(int j=0;j<NormalLayer[i].a.size();j++)
		{
			for(int k=0;k<NormalLayer[i].w[j].size();k++)
				NormalLayer[i].wDelta[j][k]=0;
			NormalLayer[i].bDelta[j]=0;
		}
	return;
}

inline bool NETWORK::reset(std::vector<double>a,int expected)
{
	DEBUG::add("network.h -> reset()");
	
	this->clear();
	if(a.size()!=InputLayerNodeNum)
	{
		DEBUG::warning("The node number of the input layer is %d but %d is checked.",InputLayerNodeNum,a.size());
		return false;
	}
	NormalLayer[0].a=a;
	
	for(int i=0;i<ExpectedOutputLayer.a.size();i++)
		ExpectedOutputLayer.a[i]=0;
	if(expected<0 || expected>9)
	{
		DEBUG::warning("The node number of the input layer is %d but %d is checked.",InputLayerNodeNum,a.size());
		return false;
	}
	ExpectedOutputLayer.a[expected]=1;
	
	DEBUG::del();
	return true;
}

inline void NETWORK::update()
{
	for(int i=0;i<TotalLayerNum-1;i++)
		for(int j=0;j<NormalLayer[i].a.size();j++)
		{
			NormalLayer[i].b[j]-=learningRate*NormalLayer[i].bDelta[j]/perSample;
			for(int k=0;k<NormalLayer[i+1].a.size();k++)
				NormalLayer[i].w[j][k]-=learningRate*NormalLayer[i].wDelta[j][k]/perSample;
		}
	for(int j=0;j<OutputLayerNodeNum;j++)
		NormalLayer[TotalLayerNum-1].b[j]-=learningRate*NormalLayer[TotalLayerNum-1].bDelta[j]/perSample;
	return;
}

void NETWORK::load()
{
	DEBUG::add("load()");
	FILE*f=fopen(SAVE_FILENAME,"rb");
	if(f!=NULL)
	{
	try{
		for(int i=0;i<TotalLayerNum-1;i++)
			for(int j=0;j<NormalLayer[i].a.size();j++)
			{
				if(fread(&NormalLayer[i].b[j],sizeof(double),1,f)!=1)
					throw 1;
				for(int k=0;k<NormalLayer[i+1].a.size();k++)
					if(fread(&NormalLayer[i].w[j][k],sizeof(double),1,f)!=1)
						throw 1;
			}
		for(int j=0;j<OutputLayerNodeNum;j++)
			if(fread(&NormalLayer[TotalLayerNum-1].b[j],sizeof(double),1,f)!=1)
				throw 1;
	}catch(int e){
		DEBUG::warning("Error when loading datas");
	}
	}
	else
	{
		DEBUG::add("fopen()");
		DEBUG::warning("No saved datas were found ! Make sure that you're training the network, or the program 'll return a random value !");
		DEBUG::del();DEBUG::del();
		return;
	}
	if(!feof(f))
		DEBUG::warning("Broken datas with extra datas in the end !");
	fclose(f);
	DEBUG::info("Load datas successfully !");
	
	DEBUG::del();
	return;
}

void NETWORK::save()
{
	DEBUG::add("network.h -> save()");
	FILE*f=fopen(SAVE_FILENAME,"wb");
	if(f!=NULL)
	{
	try{
		for(int i=0;i<TotalLayerNum-1;i++)
			for(int j=0;j<NormalLayer[i].a.size();j++)
			{
				if(fwrite(&NormalLayer[i].b[j],sizeof(double),1,f)!=1)
					throw 1;
				for(int k=0;k<NormalLayer[i+1].a.size();k++)
					if(fwrite(&NormalLayer[i].w[j][k],sizeof(double),1,f)!=1)
						throw 1;
			}
		for(int j=0;j<OutputLayerNodeNum;j++)
			if(fwrite(&NormalLayer[TotalLayerNum-1].b[j],sizeof(double),1,f)!=1)
				throw 1;
	}catch(int e){
		DEBUG::warning("Error when loading datas");
	}
	}
	fclose(f);
	DEBUG::info("Save datas successfully !");
	
	DEBUG::del();
	return;
}

int NETWORK::result()
{
	int p=0;double m=0;
	for(int i=0;i<OutputLayerNodeNum;i++)
		if(NormalLayer[TotalLayerNum-1].a[i]>m)
		{
			m=NormalLayer[TotalLayerNum-1].a[i];
			p=i;
		}
	return p;
}




#endif
