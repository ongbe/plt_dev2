#ifndef TICK_H
#define TICK_H

#include <string>
#include <vector>
#include "ThostFtdcUserApiDataType.h"

using namespace std;

struct CPeriod
{
    TThostFtdcTimeType tStartTime, tEndTime;
};

struct CTickPeriod
{
    long lStartTick, lEndTick;	//��00��00��00Ϊ��׼��һ���е�Tick��
    long lStartIndex;		//lStartTick��Ӧ�ڹ滮ʱ��ε�Tick��
};

class CTickMapping
{
public:
    CTickMapping(const CPeriod *, const long);
    CTickMapping(const vector<CPeriod> &);
    CTickMapping(const CTickMapping &);
    ~CTickMapping();

    //����ʱ��ֵ���ؾ��Ե�Tickʱ�䣨��00��00��00��ʼ��500msһ��Tick��
	static long TimeToTick(const TThostFtdcTimeType _Time);
	
	//���ݾ��Ե�Tickʱ�䷵�ص���ʱ���ַ���
    static void TickToTime(long Tick, char * _Time);

	//�޶�ʱ��ε�Tick��
    long nTotalTick;

	static const long TICK_BEFORE_START = -1;
	static const long TICK_BETWEEN_PERIODS = -2;
	static const long TICK_AFTER_END = -3;

	//��CTP��ȡ��ʱ��ӳ�䵽ʱ��ε�Tick��ֵ���������޷�����ֵ��-1��ʾ�ں�Լÿ�ս��׿�ʼǰ��-2��ʾ�ں�Լÿ������ʱ��-3��ʾ�ں�Լÿ�ս��׽�����
    long TickMapping(const TThostFtdcTimeType, const TThostFtdcMillisecType) const;

	//�Ӿ��Ե�TiCKʱ��ӳ�䵽ʱ��ε�Tick��ֵ���������޷�����ֵ��-1��ʾ�ں�Լÿ�ս��׿�ʼǰ��-2��ʾ�ں�Լÿ������ʱ��-3��ʾ�ں�Լÿ�ս��׽�����
	long TickMapping(long globaltick) const;

	//���޶���ʱ��ε�Tick��ֵӳ�䵽���Ե�TiCKʱ��
    long TickUnmapping(long) const;

	//�ж�ʱ����Ƿ�ΪС�ڽ���ʱ���
	bool IsPeriodEndingTick(long Tick) const;

private:
    CTickPeriod * _TickPeriod;
    long size;
};

#endif
