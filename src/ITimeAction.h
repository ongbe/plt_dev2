#pragma once
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include <pthread.h>
#include "ThostFtdcUserApiDataType.h"
#include "Tick.h"

using namespace std;


class ITickValidator
{
public:
	//�ж�Tick�Ƿ����趨�ĸý��м����ʱ��
	virtual bool IsTickValid(long Tick,long currentTick) const {return Tick>currentTick;};

	virtual long GetNextValidTick(long Tick) const {return ++Tick;};
};

extern ITickValidator AlwaysTrueTickValidator;

class CRecidualTickValidator: public ITickValidator
{
public:
	virtual bool IsTickValid(long Tick,long currentTick) const {return Tick >= GetNextValidTick(currentTick);};
	//���øý��м����Tickģ�壬Ŀǰ��ģValidTickStep��ValidTickOffsetΪ���м����Tickʱ���
	virtual long GetNextValidTick(long Tick) const {return (Tick+ValidTickStep-ValidTickOffset)/ValidTickStep*ValidTickStep+ValidTickOffset;};

	inline void SetValidTick(long step = 1, long offset = 0){ValidTickStep=step;ValidTickOffset=offset;};
protected:
	long ValidTickStep;
	long ValidTickOffset;
};

class ITimeAction
{
public:
    ITimeAction(void);
    virtual ~ITimeAction(void);

	//���³�ʼ�� (����currentTick = -1)
    virtual void ReInit();

	inline long GetCurrentTick() const{return currentTick;};

	//�ж�Tick�Ƿ����趨�ĸý��м����ʱ��
	inline bool IsTickValid(long Tick) const {return TickValidator.IsTickValid(Tick,currentTick);};
	
    //ִ�еĲ�����Tick ��ʱ�����������Tick��Instrument ��Լ��
	//����currentTick = ���µ�Tick
    virtual void DoAction(long Tick, const char * Instrument = NULL) = 0;
    
	//ֻ����һ����Լ��TimeActionʹ�ã�������Tick��Ϣ��������ʵ˳��
	void Run(long MaxTick);
	
	//ֻ����һ����Լ��TimeActionʹ�ã�����Tick���ݵ���˳��ִ�м��㡣
	void Run(const long * TickQueue,long TickQueueMaxIndex, const CTickMapping & TradeTime);
	
	//���������Լ��TimeActionʹ�ã�����Tick���ݵ���˳��ִ�м��㡣vector���������˳���InstrumentIDs��˳����ĸ��Сд˳��һ�£�������ȡ�
	void Run(const vector<const long *> & VectorTickQueue, const vector<long>  & VectorTickQueueMaxIndex, const CTickMapping & TradeTime);
	void Run(const vector<const long *> & VectorTickQueue, const vector<long>  & VectorTickQueueMaxIndex, const vector<const CTickMapping *> & VectorTradeTime);

	//ע����صĺ�Լ���������ɶ��ע�᲻ͬ�ĺ�Լ������������Լ���ԡ�
	void RegisterRelatedInstrument(const char * InstrumentID);
	//��ȡ��غ�Լ������DS���TimeAction���պ�Լ����ʹ��
	inline const vector<string> & GetRelatedInstrument() const{return InstrumentIDs;};

	inline void DoActionWithLock(long Tick, const char * Instrument = NULL)
	{
		pthread_spin_lock(&lock);
		DoAction(Tick,Instrument);
		pthread_spin_unlock(&lock);		
	};

	void SetTickValidator(ITickValidator & tickvalidator){TickValidator = tickvalidator;};

protected:
    //�Ѿ���currentTick�����ݽ����˴���
    long currentTick;
	vector<string> InstrumentIDs;

	ITickValidator & TickValidator;

	pthread_spinlock_t lock;
};
