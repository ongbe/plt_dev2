#pragma once

#include "Tick.h"
#include "ITimeAction.h"
#include "ThostFtdcUserApiStruct.h"
#include "InstrumentData.h"
#include "ITrader.h"
#include "IIndexCalculator.h"
#include "ITradingStrategy.h"

#include <vector>
#include <pthread.h>

using namespace std;

class ITrader;
class ITradingStrategy;

typedef vector<ITimeAction *> TimeActionVector;
typedef vector<ITrader *> TraderVector;
typedef vector<ITimeAction *> IndexVector;
typedef vector<ITradingStrategy *> StrategyVector;

class CDataSniffer: public IEventQueue
{
public:
	CDataSniffer(IMarketDataReader & md, int commmode = Signalling);
	virtual ~CDataSniffer(void);

	//���ջ��߼����������֪ͨ�Լ���������֪ͨ��ִ�����ݴ������������ָ�����Ͳ��ԡ�
    virtual void Execute() = 0;
    
	//��Execute�������߳��У������ݶ�ȡ�߳�ͬ��
	inline int ExecuteInThread(){return pthread_create(&tid, NULL, Execute, this);};
	inline int JoinInThread(){return pthread_join(tid,NULL);};
	inline int StopThread(){return pthread_cancel(tid);};

	inline void SetOver(){IsOver = true;};

    //���ָ����㶯����ָ����㶯��������迼��������ϵ
    virtual void AddIndexCalculator(ITimeAction *) = 0;
	//��Ӳ��Զ��������Զ���Ӧ���������ָ������������Ϻ�
	virtual void AddStrategy(ITradingStrategy *) = 0;
	//����ʽ���/�ɽ����¶��������ڻز�����Ӷ�������ʵ�������У�������ɽ����º��ʽ������£�������ʵ�µ���ӵ����У����������ʽ������¡�
	virtual void AddTrader(ITrader *) = 0;

	inline int GetCommunicationMode() const {return CommunicationMode;};
	static const int Shared_Memory = 0;
	static const int Signalling = 1;

protected:
	IMarketDataReader & MarketDataReader;
	
	bool IsOver;
	//��pthreadʹ�õ��߳̽ӿ�
    pthread_t tid;
	
	const int CommunicationMode;

    static void* Execute(void* param);	
};

extern CDataSniffer * CreateSingleInstrumentSniffer(const string & instrumentid, IMarketDataReader & md, int Mode = CDataSniffer::Signalling);
extern CDataSniffer * CreateMultipleInstrumentSniffer(IMarketDataReader & md, int Mode = CDataSniffer::Signalling);