#pragma once
#include "ITimeAction.h"
#include "ITrader.h"
#include "ThostFtdcUserApiDataType.h"
#include "InstrumentData.h"
#include <list>
#include <float.h>
#include <limits.h>

using namespace std;

//ÿ�����ײ���ÿ����Լ��Ӧһ��������ִ�в���
class IExecutionStrategy:public IOrderStatusReceiver
{
public:
	IExecutionStrategy(IOrderStatusReceiver * tradingstrategy, ITrader & trader, CInstrumentData * instrumentdata, ILog & logger = DefaultLogger, int commmode = Signalling);

	virtual ~IExecutionStrategy(){};

	//������ʷ��λ
	virtual void LoadHistoryPosition(int position = 0, int ydposition = 0)
	{YdPosition = ydposition;Position = position;};

	//�޼�
	//������λ�������в�λΪ�ο��Ĳ�λ����ƫ�ƣ�+����ͷ����ƫ�ƣ�-����ͷ����ƫ�ơ���֮ǰ�Ķ����ڳ���֮ǰ�Ѿ������˵�����λʱ���򲻻ᷢ�κ��¶���������
	//������Чʱ�����ƣ�ʱ������ܳ���limittic����
	//������Ч�۸����ƣ����磺���ʱ���ɽ���Ѹ�ٵس����޼ۣ����ڴ˲������ͳ�������ʱ�˲���һ�����ڵ����޼ۡ�����ʱ������ơ���
	//����falseʱ���������µ�������С�ڽ���ʱ��˱��ܾ�
	virtual bool ReqOrderInsert(double limitprice,int adj_pos,long tick = LONG_MAX,double constrainprice = DBL_MAX) = 0;

	//ȡ��ִ�в��Ե����ж���������falseʱ���������µ�������С�ڽ���ʱ��˱��ܾ�
	virtual bool CancelAllOrders() = 0;

	inline int GetStatus() const {return Status;};
	inline bool IsRunning() const {return Status == RUNNING;};
	inline bool IsFrozen() const {return Status == WAITING;};

	//�ȴ�����״̬ = ������ֹ״̬
	static const int WAITING = 0;
	//��������״̬ = �µ����߳�����
	static const int RUNNING = 1;

	//��ȡ��ǰ��λ������Ϊ��֣�����Ϊ�ղ�
	inline int GetPosition() const {return Position;};
	inline int GetYdPosition() const {return YdPosition;};
	
	//������һ�ζ����ɽ��۸�
	inline double GetLastTradedPrice() const {return LastTradePrice;};
	//������һ�ζ����ɽ���
	inline unsigned int GetLastTradedVolume() const {return LastTradeVolume;};
	//��õ�ǰ���������и�ҹ�ֲ֣�����Ϊ���������̼۽��������ղ�λ���Խ����̼���ս��ղ�λ�����õ����󡣴�����û�м��������ѡ����Գɱ��ȡ������º����п۳����׳ɱ�
	inline double GetProfit() const {return Profit;};
	//���ƽ�����γɱ�
	inline double GetAverageHiddenCost() const {return TotalTradePosition?HiddenCost/TotalTradePosition:0;};
	//��ý��ձ�ִ�в��Գ�������
	inline unsigned int GetCancelNumber() const {return CancelNumber;};
	//��ý��տ���������
	inline unsigned int GetTotalOpenPosition() const {return TotalOpenPosition;};
	//��ý��ս���������
	inline unsigned int GetTotalTradePosition() const {return TotalTradePosition;};
	//��ý���ӯ���Ļغ���
	inline unsigned int GetWinningRound() const {return WinningRound;};	
	//��ý��ս��׻غ����������޲�λ���в�λ�ٵ��޲�λΪһ���غ�
	inline unsigned int GetRound() const {return Round;};

	inline void SetCancelNumberLimit(unsigned long limit){CancelNumberLimit = limit;};
	inline void SetCancelNumberPerTickLimit(unsigned long limit){CancelNumberPerTickLimit = limit;};

protected:
	
	//�ܲ�λ
	int Position;

	//���(YdPositionҪ������Ϊ��ֹ״̬��Ż���£���Ʋ���ʱֻ�����ܲ�λ)
	int YdPosition; 

	//���׽ӿ�
	ITrader & Trader;

	//��Լ��������
	CInstrumentData * InstrumentData;

	//���һ�ζ����ɽ��۸�Ͷ����ɽ���
	double LastTradePrice;
	unsigned int LastTradeVolume;

	//����(���轫����������̼۽��֣����������ĳֲ��Խ����̼�ƽ�ֵ�����ֵ��)
	double Profit;

	//���γɱ������������Գɱ�������ɱ��ȵȣ�����ʵ��ͳ�Ƴɱ���
	double HiddenCost;

	//��������
	unsigned int CancelNumber;
	//ÿ�ճ����������ƣ�ȱʡΪ500��
	unsigned int CancelNumberLimit;
	//Tickʱ���ڳ�������
	unsigned int CancelNumberPerTick;
	//ÿ��Tick�����������ƣ�ȱʡΪ3��
	unsigned int CancelNumberPerTickLimit;
	//����������
	unsigned int TotalOpenPosition;
	//����������
	unsigned int TotalTradePosition;
	//�����������гֲֵ��ֲ�Ϊ����Ϊһ��
	unsigned int Round;
	//ʤ������
	unsigned int WinningRound;

	//����״̬
	int Status;

	//�����ر�����ʵ��: ���ײ��ԣ����ײ���DoAction�������ȵ�����غ�Լִ�в��Ե�DoAction��
	IOrderStatusReceiver * TradingStrategy;
};

extern IExecutionStrategy * CreateSimpleExecutionStrategy(IOrderStatusReceiver * OrderStatusReceiver, ITrader & trader, CInstrumentData * instrumentdata, ILog & logger = DefaultLogger, int commmode = IOrderStatusReceiver::Signalling);