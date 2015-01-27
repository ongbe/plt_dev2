#pragma once

//�˻�����Ķ�������ϵͳ���ǲ��Լ���
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ILog.h"
#include "ITimeAction.h"
#include "IEvent.h"
#include <string>

using namespace std;

//���ڽ��ײ��������ִ�в�����Ķ���״̬���¹���
class IOrderStatusReceiver:public ITimeAction
{
public:
	IOrderStatusReceiver(ILog & logger = DefaultLogger, int commmode = Signalling):Logger(logger),CommunicationMode(commmode){};
	virtual ~IOrderStatusReceiver(){};

	virtual void DoAction(long Tick, const char * Instrument = NULL) = 0;

	inline void SetCommunicationMode(int mode){CommunicationMode = mode;};
	inline int GetCommunicationMode() const {return CommunicationMode;};
	inline void SetEventQueue(IEventQueue * eventqueue){EventQueue = eventqueue;};
	inline void SendOrderStatusUpdateEvent(const char* instrument){EventQueue->SendEvent(-1,instrument,this);};

	static const int Shared_Memory = 0; //��DataSniffer�е�SHARED_MEMģʽ��Ӧ
	static const int Signalling = 1; //��DataSniffer�е�SIGNALLINGģʽ��Ӧ
	static const int Mutex_Handling = 2; //��DataSniffer�е�SIGNALLINGģʽ��Ӧ

	ILog & Logger;

protected:
	int CommunicationMode;
	IEventQueue * EventQueue; 

};

///δ֪
#define ORDER_STATUS_Unknown 0
///������
#define ORDER_STATUS_Queueing 1
///δ�ڶ�����
#define ORDER_STATUS_NotQueueing 2
///����
#define ORDER_STATUS_Canceled 3
///ȫ���ɽ�
#define ORDER_STATUS_AllTraded 4
///����
#define ORDER_STATUS_Error 5
///����������ȫ�ĳɽ��ر�
#define ORDER_STATUS_CanceledwFullRtnTrade 6
///ȫ���ɽ�������ȫ�ĳɽ��ر�
#define ORDER_STATUS_AllTradedwFullRtnTrade 7



class CExeOrderStatus
{
public:
	CExeOrderStatus();

	//��ȡ�������򣬶�ͷ����false����ͷ����true
	inline bool IsShort() const
	{return OrderRtn.Direction!=THOST_FTDC_D_Buy;};

	inline bool IsLong() const
	{return OrderRtn.Direction==THOST_FTDC_D_Buy;};

	inline bool IsOpen() const
	{return OrderRtn.CombOffsetFlag[0] == THOST_FTDC_OF_Open;};

	inline bool IsClose() const
	{return OrderRtn.CombOffsetFlag[0] != THOST_FTDC_OF_Open;};

	CThostFtdcOrderField OrderRtn;
	
	//�ɽ��ر�������ʱ�����ڶ����ر���
	//�Ѿ�����ȷ�ɽ��ر��ĳɽ��ܼ�
	TThostFtdcPriceType TradeTotalPrice;
	//�Ѿ�����ȷ�ɽ��ر��ĳɽ�����
	TThostFtdcPriceType TradeAvgPrice;
	//���һ�γɽ��ر��ļ۸�
	TThostFtdcPriceType TradeLastPrice;
	//���һ�γɽ��ر�����
	TThostFtdcVolumeType TradeLastVolume;
	//�Ѿ�����ȷ�ɽ��ر��ĳɽ�����
	TThostFtdcVolumeType TradeVolume;

	int Status;
	int ErrID;

	//�������µĶ������������óɽ��ײ��ԣ�����״̬��ģʽ������ִ�в��ԣ���ģʽ��
	IOrderStatusReceiver * OrderUpdateAction;
};

class ITrader: public ITimeAction	//�����Զ��������������óɽ����������ģʽ�����Լ������ʽ����
{
public:
	ITrader(int category);	//����Trader����
	virtual ~ITrader(){};
	
	//ִ�н���ʵ��
	virtual void Execute(){};

	//�ȴ������߳��˳�
	virtual void Join(){};

	//�ͷŽ���ʵ��
	virtual void Release(){};

	//Ŀǰ���ڻز⣬����Ϊ�µ���ֱ�Ӿͻ�ɽ�������ʵ�ʽ������������׻ر�������OrderStatus
	virtual int ReqOrderInsert(const TThostFtdcInstrumentIDType INSTRUMENT_ID, TThostFtdcPriceType LIMIT_PRICE,
		TThostFtdcDirectionType DIRECTION, char OPENCLOSE, TThostFtdcVolumeType VOLUME, CExeOrderStatus * & pOrderStatus, IOrderStatusReceiver * TS,
		TThostFtdcLongTimeType Time=NULL, const string & comments="") = 0;

	virtual int ReqOrderCancel(CExeOrderStatus * pOrder) = 0;

	TThostFtdcMoneyType AvailableMoney;

	inline void DisableOrderLog(){OrderLog = false;};
	inline void EnableOrderLog(){OrderLog = true;};

	//Trader����
	const int Category;
	static const int FILETRADER = 0;
	static const int REALTRADER = 1;

protected:
	bool OrderLog;
};

extern ITrader * CreateBackTestTraderInstance(const char * filename);

extern ITrader * CreateRealTraderInstance(const TThostFtdcInvestorIDType INVESTOR_ID,const TThostFtdcPasswordType PASSWORD, bool TestServer, ILog & Logger = DefaultLogger);
extern ITrader * CreateRealTraderInstance(const string & Param, bool TestServer, ILog & Logger = DefaultLogger);