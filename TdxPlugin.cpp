// choice1.cpp : Defines the entry point for the DLL application.
// 插件实例

#define PLUGIN_EXPORTS

#include "stdafx.h"
#include "plugin.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

PDATAIOFUNC	 m_pfn;

void RegisterDataInterface(PDATAIOFUNC pfn)
{
	m_pfn = pfn;
}

////////////////////////////////////////////////////////////////////////////////
//注册插件信息
void GetCopyRightInfo(LPPLUGIN info)
{
	//填写基本信息
	strcpy(info->Name,"选股");
	strcpy(info->Dy,"深圳");
	strcpy(info->Author,"田雨");
	strcpy(info->Period,"短线");
	strcpy(info->Descript,"自定义选股");
	strcpy(info->OtherInfo,"自定义X理论");
	//填写参数信息
	info->ParamNum = 4;
	strcpy(info->ParamInfo[0].acParaName,"参数1");
	info->ParamInfo[0].nMin=1;
	info->ParamInfo[0].nMax=100;
	info->ParamInfo[0].nDefault=1;
	strcpy(info->ParamInfo[1].acParaName,"参数2");
	info->ParamInfo[1].nMin=1;
	info->ParamInfo[1].nMax=1000;
	info->ParamInfo[1].nDefault=60;
	strcpy(info->ParamInfo[2].acParaName,"参数3");
	info->ParamInfo[2].nMin=0;
	info->ParamInfo[2].nMax=1000;
	info->ParamInfo[2].nDefault=60;
	strcpy(info->ParamInfo[3].acParaName,"参数4");
	info->ParamInfo[3].nMin=0;
	info->ParamInfo[3].nMax=1000;
	info->ParamInfo[3].nDefault=60;
}

////////////////////////////////////////////////////////////////////////////////
const	BYTE	g_nAvoidMask[]={0xF8,0xF8,0xF8,0xF8};	// 无效数据标志(系统定义)

WORD   AfxRightData(float*pData,WORD nMaxData)	//获取有效数据位置
{	
	WORD nIndex=0;
	for(;nIndex<nMaxData&&!memcmp(&pData[nIndex],g_nAvoidMask,4);nIndex++);
	return(nIndex);
}
////////////////////////////////////////////////////////////////////////////////
//自定义实现细节函数(可根据选股需要添加)

void   AfxCalcMa(float*pData,long nData,WORD nParam)
{	
	if(pData==NULL||nData==0||nParam==1) return;
	long i=nData-nParam+1,nMinEx=AfxRightData(pData,nData);
	if(nParam==0||nParam+nMinEx>nData) nMinEx=nData;
	else
	{	
		float	nDataEx=0,nDataSave=0;
		float	*MaPtr=pData+nData-1,*DataPtr=pData+nData-nParam;
		for(nMinEx+=nParam-1;i<nData;nDataEx+=pData[i++]);
		for(i=nData-1;i>=nMinEx;i--,MaPtr--,DataPtr--)
		{
			nDataEx+=(*DataPtr);
			nDataSave=(*MaPtr);
			*MaPtr=nDataEx/nParam;
			nDataEx-=nDataSave;
		}
	}
}

WORD   AfxCross(float*psData,float*plData,WORD nIndex,float&nCross)
{	
	if(psData==NULL||plData==NULL||nIndex==0) return(0);
	float  nDif=psData[nIndex-1]-plData[nIndex-1];
	float  nDifEx=plData[nIndex]-psData[nIndex];
	float  nRatio=(nDif+nDifEx)?nDif/(nDif+nDifEx):0;
	nCross=psData[nIndex-1]+(psData[nIndex]-psData[nIndex-1])*nRatio;
	if(nDif<0&&nDifEx<0)	return(1);
	if(nDif>0&&nDifEx>0)	return(2);
	return(0);
}

///////////////////////////////////////////////////////////////////////////////////
//
BOOL InputInfoThenCalc1(char * Code,		//股票代码
						short nSetCode,		//市场分类，0为深市 1为沪市
						int Value[4],		//设置参数
						short DataType,		//申请数据类型，缺省为日K线历史数据
						short nDataNum,		//申请数据个数
						BYTE nTQ,			//精确除权信息
						unsigned long unused) //按最近数据计算
{
	BOOL nRet = FALSE;
	NTime tmpTime={0};

	LPHISDAT pHisDat = new HISDAT[nDataNum];  //数据缓冲区
	long readnum = m_pfn(Code,nSetCode,DataType,pHisDat,nDataNum,tmpTime,tmpTime,nTQ,0);  //利用回调函数申请数据，返回得到的数据个数
	lalala
	if( readnum > max(Value[0],Value[1]) ) //只有数据个数大于Value[0]和Value[1]中的最大值才有意义
	{
		float *pMa1 = new float[readnum];
		float *pMa2 = new float[readnum];
		for(int i=0;i < readnum;i++)
		{
			pMa1[i] = pHisDat[i].Close;
			pMa2[i] = pHisDat[i].Close;
		}
		AfxCalcMa(pMa1,readnum,Value[0]);	//计算MA
		AfxCalcMa(pMa2,readnum,Value[1]);
		float nCross;
		if(AfxCross(pMa1,pMa2,readnum-1,nCross) == 1)	//判断是不是在readnum-1(最后一个数据)处交叉 1:上穿 2:下穿
			nRet = TRUE;  //返回为真，符合选股条件
		delete []pMa1;pMa1=NULL;
		delete []pMa2;pMa2=NULL;
	}

	delete []pHisDat;pHisDat=NULL;
	return nRet;
}

BOOL InputInfoThenCalc2(char * Code,		//股票代码
						short nSetCode,		//市场分类
						int Value[4],		//参数列表
						short DataType,		//申请数据类型，缺省为日K线历史数据
						NTime time1,		//开始时间
						NTime time2,		//结束时间
						BYTE nTQ,			//精确除权信息
						unsigned long unused)  //选取区段
{
	BOOL nRet = FALSE;
	NTime tmpTime={0};

	//窥视数据个数
	long datanum = m_pfn(Code,nSetCode,DataType,NULL,-1,time1,time2,nTQ,0);
	if( datanum < max(Value[0],Value[1]) ) 
		return FALSE;
	
	//读取数据
	LPHISDAT pHisDat = new HISDAT[datanum];
	long readnum = m_pfn(Code,nSetCode,DataType,pHisDat,datanum,time1,time2,nTQ,0);
	if( readnum > max(Value[0],Value[1]) ) //只有将数据个数大于Value[0]和Value[1]中的最大值才有意义
	{
		float *pMa1 = new float[readnum];
		float *pMa2 = new float[readnum];
		for(int i=0;i < readnum;i++)
		{
			pMa1[i] = pHisDat[i].Close;
			pMa2[i] = pHisDat[i].Close;
		}
		AfxCalcMa(pMa1,readnum,Value[0]);	//计算MA
		AfxCalcMa(pMa2,readnum,Value[1]);
		float nCross;
		if(AfxCross(pMa1,pMa2,readnum-1,nCross) == 1)	//判断是不是在readnum-1(最后一个数据)处交叉 1:上穿 2:下穿
			nRet = TRUE;
		delete []pMa1;pMa1=NULL;
		delete []pMa2;pMa2=NULL;
	}

	delete []pHisDat;pHisDat=NULL;
	return nRet;
}
