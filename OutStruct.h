#ifndef OUTSTRUCT_H_
#define OUTSTRUCT_H_

#pragma pack(push,1) //请务必一字节对齐

typedef struct tag_NTime
{
	unsigned short year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
}NTime;

typedef struct tag_HISDAT
{
	NTime Time;			//时间
	float Open;			//单位开盘价
	float High;			//单位最高价
	float Low;			//单位最低价
	float Close;		//单位收盘价
	float Amount;
	DWORD Volume;       //单位成交量,指数无成交量
	union 
	{ 
		float YClose;
		struct
		{   
			WORD up;                // 上涨家数
			WORD down;              // 下跌家数
		}zd;						// 单位前收盘价(除权用)，期货的持仓量
	};
}HISDAT,*LPHISDAT;

typedef struct ExDayInfo
{
	DWORD dwDate;     //日期               
	short nStockNum;  //当天交易股票数     
	short nUpIdx;     //个股涨幅名次       
	DWORD dwInVol;    //买盘成交量         
	DWORD dwFirstVol; //集合竞价成交量     
	float fLast5ZDFD; //最后5分钟振动幅度  
	WORD  wNoCjMinute;//没有成交的分钟数   
	WORD  wCjbs;      //总成交笔数
	union
	{
		float Yjlx;	  //如果是国债,是当天利息
		float Lead;	  //如果是指数,是当天最后时刻的领先值
		float Other;  //对于AB股,暂未用
	};
	char  strNoUsed[4];//备用字段
}EXDAYINFO;

typedef struct tag_REPORTDAT
{
	WORD           ItemNum;              // 采样点数
	float          Close;                // 前收盘价
	float          Open;                 // 今开盘价
	float          Max;                  // 最高价
	float          Min;                  // 最低价
	float          Now;                  // 现价
	DWORD          Volume;               // 总手
	DWORD          NowVol;               // 现手(总手差)
	float          Amount;               // 总成交金额
	DWORD          Inside;               // 内盘
	DWORD          Outside;              // 外盘
	float          TickDiff;             // 笔涨跌(价位差)
	BYTE		   InOutFlag;			 // 内外盘标志 0:Buy 1:Sell 2:None
	union
	{
		struct	//个股
		{
			float          Buy;			// 最高叫买价
			float          Sell;        // 最低叫卖价
			float          Buyp[3];		// 三个叫买价
			DWORD          Buyv[3];		// 对应三个叫买价的三个买盘
			float          Sellp[3];	// 三个叫卖价
			DWORD          Sellv[3];	// 对应三个叫卖价的三个卖盘
		}Ggpv;
		struct	//指数
		{
			float			LxValue;	// 领先指标
			float			Yield;		// 不含加权的指数
			long			UpHome;		// 上涨家数
			long			DownHome;	// 下跌家数
			float			JJAmount;	// 基金成交金额
			float			ZQAmount;	// 债券成交金额
			float			GZAmount;	// 国债成交金额
			float			QZAmount;	// 权证成交金额
			float			QTAmount;	// 指数其它成交金额
		}Zspv;
	}Other;
}REPORTDAT,*LPREPORTDAT;	

typedef struct tag_STOCKINFO 
{
	char        Name[9];			// 证券名称

	short       Unit;               // 交易单位	
	long		VolBase;			// 量比的基量
	float       cdp;				// 昨日中价
	float       nh;					// 卖点
	float       nl;					// 买点
	float       ah;					// 高价突破点
	float       al;					// 低价突破点
 	short       dkflag;				// 适合作多或作空
	float       tbp;				// 多空平衡点
	float       stoplost;			// 止损点
	float       leave;				// 了结点

	float       ActiveCapital;		// 流通股本
    long        J_start;			//上市日期
	short       J_addr;				//所属省份
	short       J_hy;				//所属行业
    float       J_zgb;				//总股本
    float       J_gjg;				//国家股
    float       J_fqrfrg;			//发起人法人股
    float       J_frg;				//法人股
    float       J_bg;				//B股
    float       J_hg;				//H股
    float       J_zgg;				//职工股
    float       J_zzc;				//总资产(元)
    float       J_ldzc;				//流动资产
    float       J_gdzc;				//固定资产
    float       J_wxzc;				//无形资产
    float       J_cqtz;				//长期投资
    float       J_ldfz;				//流动负债
    float       J_cqfz;				//长期负债
    float       J_zbgjj;			//资本公积金
    float       J_jzc;				//股东权益(就是净资产)
    float       J_zysy;				//主营收入
    float       J_zyly;				//主营利润
    float       J_qtly;				//应收帐款
    float       J_yyly;				//营业利润
    float       J_tzsy;				//投资收益
    float       J_btsy;				//经营现金净流量
    float       J_yywsz;			//总现金净流量
    float       J_snsytz;			//存货
    float       J_lyze;				//利益总额
    float       J_shly;				//税后利益
    float       J_jly;				//净利益
    float       J_wfply;			//未分配利益
    float       J_tzmgjz;			//调整每股净资产 物理意义:  净资产/调整后的总股本
	//增加部分
	float		J_jyl;				//净益率%
	float		J_mgwfp;			//每股未分配
	float		J_mgsy;				//每股收益
	float		J_mggjj;			//每股公积金
	float		J_mgjzc;			//每股净资产
	float		J_gdqyb;			//股东权益比
}STOCKINFO,*LPSTOCKINFO;

typedef struct tag_TICKDAT
{ 
	NTime	Time;
	float   Now;            //现价(元)
	DWORD   NowVol;         //现手(手)
	float 	Amount;		    //成交金额(元)
	short   InOutFlag;      //买卖标志
}TICKDAT,*LPTICKDAT;

typedef struct tag_BSPDAT
{
	NTime	Time;
	float   Now;
	unsigned long NowAmount;	//单笔成交金额(百元)
	long	NowVol;
	float   BuyP[3];
	long    BuyV[3];
	float   SellP[3];
	long    SellV[3];
	char    InOutFlag;
	char    MaxFlag;//最高价标志 0：最高价在前面已出现或者Now即为最高价1:最高价存放于Max中
	char    MinFlag;
	float   Max;
	float   Min;
}BSPDAT,*LPBSPDAT;

typedef struct tag_GBInfo	//股本总股本信息
{
	float Zgb;
	float Ltgb;
}GBINFO,*LPGBINFO;

typedef struct tag_OtherInfo
{
	short RelativeMin;	//离开市的相对分钟数
}OTHERINFO,*LPOTHERINFO;

typedef struct tag_ALLGPInfo
{
	short StockNum;			//每种分类的股票数
	char  buf[1];			//按市场[1],代码[6],名称[8]顺序存放
}ALLGPINFO,*LPALLGPINFO;

typedef struct tag_FLInfo
{
	char  FLNum;			//最多100个分类
	char  FLName[100][20];	//分类名
	short FLStockNum[100];	//每种分类的股票数
	char  buf[1];			//按市场[1],代码[6],名称[8]顺序存放
}FLINFO,*LPFLINFO;

typedef struct tag_TPPrice
{
	float Close;
	float TPTop;
	float TPBottom;
}TPPRICE,*LPTPPRICE;

/////考虑兼容
#define PER_MIN5     0		//5分钟数据,需要下载5分钟数据
#define PER_MIN15    1		//15分钟数据,需要下载5分钟数据
#define PER_MIN30    2		//30分钟数据,需要下载5分钟数据
#define PER_HOUR     3		//1小时数据,需要下载5分钟数据
#define PER_DAY      4		//日线数据,需要下载日线数据
#define PER_WEEK     5		//周线数据,需要下载日线数据
#define PER_MONTH    6		//月线数据,需要下载日线数据
/////////////////////
#define PER_MIN1     7		//通达信券商集成版暂不支持
#define PER_MINN	 8		//通达信券商集成版暂不支持
#define PER_DAYN     9		//多天线数据,需要下载日线数据
#define PER_SEASON	 10		//季线,需要下载日线数据
#define PER_YEAR	 11		//年线,需要下载日线数据

#define PER_EXDAY	20		//扩展日线的周期,通达信券商集成版暂不支持

#define TICK_DAT    100		//分笔数据,通达信券商集成版暂不支持
#define	MINUTE_DAT  101		//分时数据,通达信券商集成版暂不支持
#define	BSP_DAT		102		//买卖盘,  通达信券商集成版暂不支持
#define GBINFO_DAT	103		//股本信息
#define	REPORT_DAT	104		//行情数据
#define	STKINFO_DAT	105		//股票相关数据

#define ALLGPINFO_DAT	106		//所有股票码表信息
#define FLGINFO_DAT		107		//分类股信息
#define DYGINFO_DAT		108		//地区板块信息
#define HYGINFO_DAT		109		//行业板块信息
#define GNGINFO_DAT		110		//概念板块信息
#define ZDGINFO_DAT		111		//自定义板块信息
#define ZHGINFO_DAT		112		//组合板块信息

#define REMOTEHQ_DAT	120		//远程行情数据
#define TPPRICE_DAT		121		//涨跌停数据
#define OTHER_DAT		130		//其它

/////////////
#define ASK_ALL			-1


//功能切换的类型
#define SWITCH_TOZST			11
#define SWITCH_TOVIP			12
#define SWITCH_ADDBLOCK			13
#define SWITCH_SAVENEWBLOCK		14
#define SWITCH_OPENURL			15
#define SWITCH_USERINFO			16
#define SWITCH_TOZSGPC			17
#define SWITCH_TOWARNWT			18

#define SWITCH_JYGETHQ			22
#define SWITCH_SETKEY			23
#define SWITCH_SHIFTCJZX		24
#define SWITCH_CLOSECJZX		25
#define SWITCH_CLOSEASSISWND	26
#define SWITCH_REFRESHTITLE		27
#define SWITCH_LOGINEMBWT		28
#define SWITCH_QUITEMBWT		29
#define SWITCH_MAXASSISWND		30
#define SWITCH_TQNOTIFY			32
#define SWITCH_OPENEMBWT		33
//////////////////////////////////////////////////////////////////////

typedef struct tag_OP_Scheme_Color
{ 
	COLORREF	BACKCOLOR;			//背景
	COLORREF	BACKCOLOR2;			//背景2
	COLORREF	TXTCOLOR;			//图形文字

	//分析图相关
	COLORREF	AXISCOLOR;			//坐标轴框
	COLORREF	TIMECOLOR;			//坐标文字(包括十字线、游标框)
	
	COLORREF	KPCOLOR;			//阳Ｋ线及上涨字
	COLORREF	KNCOLOR;			//阴Ｋ线
	COLORREF	DOWNCOLOR;			//下跌字
	COLORREF	LEVELCOLOR;			//平线

	COLORREF	VOLCOLOR;			//成交量颜色
	COLORREF	BIGTITLECOLOR;		//大标题
	COLORREF	FLOATCURSORCOLOR;	//高亮颜色

	//列表相关
	COLORREF	Grid_CodeName_Color;//代码名称颜色
	COLORREF	GridLineColor;		//列表的分隔线颜色
	COLORREF	GridHighLineColor;	//高亮选中线的颜色
	COLORREF	GridTitleTextColor;	//栏目串的颜色
	COLORREF	GridTitleArrowColor;//栏目排序的颜色
	COLORREF	Grid_Spec_Color;	//特别颜色，正常情况下为TXTCOLOR

	//资讯颜色
	COLORREF	ZxColor;
	COLORREF	ZxTitleColor;
	COLORREF	MineTitleTxtColor;
	COLORREF	MineTitleSelColor;
	COLORREF	MineTitleSpeColor;
	COLORREF	F10TitleColor;
	COLORREF	F10TitleSelColor;
			
	//网页相关
	COLORREF	LinkColor;
	COLORREF	ActiveLinkColor;
	COLORREF	VisitedLinkColor;
	COLORREF	WebSpecColor;

	COLORREF	OTHERCOLOR[11];		//其它颜色,用于指标或十大股东的饼图
}SCHEME_COLOR;

typedef struct tag_OP_Scheme_Font
{ 
	char		ListFontName[40];
	short		ListFontSize;
	char		GraphFontName[40];
	short		GraphFontSize;
	char		ZxFontName[40];
	short		ZxFontSize;
}SCHEME_FONT;

typedef struct tag_OP_Scheme_Menu
{ 
	short	nMenuMode; 		//0:传统风格 1:XP风格
	short	bExtendXPStyle; //是否是增强型的XP风格 
	long	Reserved;
}SCHEME_MENU;

//////////////////////////////////////////////////////////////////////


#pragma pack(pop)

#endif
