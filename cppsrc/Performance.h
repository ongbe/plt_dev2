#pragma once

#include "ThostFtdcUserApiDataType.h"
//#include <string.h>
#include <string>
#include <pthread.h>

using namespace std;

class CPerformance
{
public:
	CPerformance(long bufsize);
	virtual ~CPerformance(void);

	void AddPerformanceItem(const TThostFtdcDateType tradingday,double profit);

	void Sort();

	//�����Kelly�ܸ�ϵ���������ָ�꣬���ȵ���Sort
	virtual void Evaluate();
	
	//����Kelly�ܸ�ϵ�������ȵ���Evaluate
	void GetKellyLeverage(double MaxValue);

	//�����ڡ���������ļ�
	virtual void SavePerformance(const char * filename) const;

	string * TradingDay;
	double * DailyProfit;

	//MAR���ʣ��껯����������ز�
	double MAR;
	//ƽ��������
	double AverageProfit;
	//�껯���� = �վ����� * 244
	double AnnualProfit;
	//ӯ������ռ��
	double WinningDayRatio;
	//���س�
	double MaxDrawdown;
	//���س�����
	double MaxDrawdownDuration;
	//Sharpe����
	double SharpeRatio;
	//�������ӯ��
	double MaxContinuousWinningProfit;
	double MaxContinuousLosingProfit;
	//�������ӯ��
	double MaxDailyWinningProfit;
	double MaxDailyLosingProfit;
	//�������ӯ������
	unsigned int MaxContinuousWinningDay;
	unsigned int MaxContinuousLosingDay;
	//Kelly�ܸ�ϵ��
	double KellyLeverage;

	static const unsigned int TradingDaysPerYear = 244;

protected:
	long Size;
	long Counter;

	//����Ԫ��i��Ԫ��j�����в���
	virtual void Swap(long i,long j);

	pthread_spinlock_t spinlock;
};

class CSingleInstrumentStrategyPerformance:public CPerformance
{
public:
	CSingleInstrumentStrategyPerformance(long bufsize);
	virtual ~CSingleInstrumentStrategyPerformance(void);

	void AddPerformanceItem(const TThostFtdcDateType tradingday,double profit,unsigned int totaltradeposition,
		unsigned int totalopenposition, unsigned int round,unsigned int winninground);

	//�����ܿ��������ļ�ЧУ׼: Profit -= totalopenposition * factor
	void Calibrate(double factor);

	//�����Kelly�ܸ�ϵ���������ָ��
	virtual void Evaluate();

	//�����ڡ����桢�ܽ��״������ܿ��ִ������غ�����ʤ���غ��������ļ�
	virtual void SavePerformance(const char * filename) const;

	unsigned int * DailyTotalTradePosition;
	unsigned int * DailyTotalOpenPosition;
	unsigned int * DailyRound;
	unsigned int * DailyWinningRound;

	//ÿ��ƽ�����׻غ�����ÿ��ƽ�����״���
	double RoundPerDay, TradePositionPerDay;
	//ÿ������״�����ÿ����С���״���
	unsigned int MaxTotalTradePositionPerDay, MinTotalTradePositionPerDay;
	//ӯ���غ�ռ��
	double WinningRoundRatio;

protected:
	virtual void Swap(long i,long j);
};