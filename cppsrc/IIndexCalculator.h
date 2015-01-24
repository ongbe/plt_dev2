#pragma once

#include "ITimeAction.h"
#include "IHistoryData.h"
#include "Tick.h"

template <class RType>
class IIndexCalculator:
    public ITimeAction
{
public:
    IIndexCalculator(long winsize, long size);	//����ָ��Index�������С
    virtual ~IIndexCalculator();
	inline RType operator[] (long idx) const{return Index[idx];};
	inline long GetWinSize() const {return WinSize;};
	inline const RType * GetIndex() const{return Index;};
	
	
	//��Index������������ݴ���ָ���Ķ������ļ�����ָ���ڶ���������Ϊ�洢ĩβ��size�����ݡ�
	void SaveIndex2BinFile(char * filename,long size = 0) const;
	//���ı���ʽ�洢ָ������
	void SaveIndex2TxtFile(char * filename,CTickMapping & tmap) const;

protected:
    RType * Index;
    long WinSize;
    long BufSize;
};

//HType����Ϊ��ʷ���ݵ��������ͣ����磺int, long, float, double
//Const����˵��ָ����㲻��ı�ԭ������
//�����������ñ������Copy���캯��
//ͨ�����[]���������Ч�ʲ����ߡ�


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <class Type>
class CMAIndex :
    public IIndexCalculator<double>
{
public:
    CMAIndex(const Type * ori_data,const IHistoryData<Type> & old_ori_data, const IHistoryData<double> & old_ma_data, long winsize,long bufsize, long step = 1);
	CMAIndex(const Type * ori_data,long winsize,long bufsize, long step = 1);
    ~CMAIndex();

	inline long GetStep() const {return Step;};
    virtual void DoAction(long Tick, const char * Instrument = NULL);

private:
	const IHistoryData<Type> & OldOriginalData;
	const IHistoryData<double> & OldIndexData;

	const PatchedHistoryData<Type> DefaultOldOriginalData;
	const PatchedHistoryData<double> DefaultOldMAData;

    const Type * OriginalData;
    double factor;  //  =step/WindowsSize
	long Step;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <class Type>
class CEMAIndex :
    public IIndexCalculator<double>
{
public:
	CEMAIndex(const Type * ori_data,const IHistoryData<Type> & old_ori_data, const IHistoryData<double> & old_ema_data,long winsize,long bufsize,double alpha = 0, long step = 1);
    CEMAIndex(const Type * ori_data,long winsize,long bufsize,double alpha = 0, long step = 1);
    ~CEMAIndex();

	inline long GetStep() const {return Step;};
    virtual void DoAction(long Tick, const char * Instrument = NULL);

private:
	const IHistoryData<Type> & OldOriginalData;
	const IHistoryData<double> & OldIndexData;

	const PatchedHistoryData<Type> DefaultOldOriginalData;
	const PatchedHistoryData<double> DefaultOldEMAData;

    const Type * OriginalData;
    double Alpha;
    double Beta;	//1-Alpha
    long Step;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <class Type>
class CKLine :
    public IIndexCalculator<Type>
{
public:
	//winsize = K�ߵĴ���sizeΪTick������
    CKLine(const Type * ori_data,long winsize,long size);
    ~CKLine();

    virtual void DoAction(long Tick, const char * Instrument = NULL);

	//K�ߵ�OpenPrice��ClosePrice�ķ�����ֱ�ӷ���
	//const Type * GetOpenPrice() const{return OpenPrice;};
	//const Type * GetClosePrice() const{return ClosePrice;};
	inline const Type * GetMaxPriceIndex() const{return MaxPrice;};
	inline const Type * GetMinPriceIndex() const{return MinPrice;};

	Type operator[] (long Tick) const;	//����ClosePrice
	inline Type GetOpenPrice(long Tick) const{return OriginalData[(Tick/this->WinSize)*this->WinSize];};
	//����currentTick����K������֮ǰ��K�����ݷ�����һ��Tick������K�ߵ�ClosePrice
	Type GetClosePrice(long Tick) const;
	//����K���ڵ���ǰTick����������Сֵ
	inline Type GetMaxPrice(long Tick) const{return MaxPrice[Tick];};
	inline Type GetMinPrice(long Tick) const{return MinPrice[Tick];};

private:
    const Type * OriginalData;
    //Type * OpenPrice;
    //Type * ClosePrice;
    Type * MaxPrice;
    Type * MinPrice;
};

//Type���ͱ�����ָ�롢֧��[]�������ŵ�����
//�������� const int *������ &
//Const����˵��ָ����㲻��ı�ԭ������
//�����������ñ������Copy���캯��
//ͨ�����[]���������Ч�ʲ����ߡ�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <class Type>
class CRSIIndex :
	public IIndexCalculator<double>
{
public:
	CRSIIndex(const Type * ori_data,const IHistoryData<Type> & old_ori_data,long winsize, long bufsize,long step = 1);
	CRSIIndex(const Type * ori_data,long winsize, long bufsize,long step = 1);
	~CRSIIndex();

	inline long GetStep() const {return Step;};
	virtual void DoAction(long Tick, const char * Instrument = NULL);

private:
	const IHistoryData<Type> & OldOriginalData;
	const PatchedHistoryData<Type> DefaultOldOriginalData;

	const Type * OriginalData;
	long Step;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Type���ͱ�����ָ�롢֧��[]�������ŵ�����
//�������� const int *������ &
//Const����˵��ָ����㲻��ı�ԭ������
//�����������ñ������Copy���캯��
//ͨ�����[]���������Ч�ʲ����ߡ�


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <class Type>
class CSTDIndex :
	public IIndexCalculator<double>
{
public:
	CSTDIndex(const Type * ori_data,const IHistoryData<Type> & old_ori_data,long winsize,long bufsize, long step = 1);
	CSTDIndex(const Type * ori_data,long winsize,long bufsize, long step = 1);
	~CSTDIndex();

	inline long GetStep() const {return Step;};
	virtual void DoAction(long Tick, const char * Instrument = NULL);

private:
	const IHistoryData<Type> & OldOriginalData;

	const PatchedHistoryData<Type> DefaultOldOriginalData;

	const Type * OriginalData;
	double factor;  //  =step/WindowsSize
	long Step;

	double * AvgXSquare;
	double * AvgX;
};

template <class Type>
class CMaxIndex :
	public IIndexCalculator<Type>
{
public:
	CMaxIndex(const Type * ori_data,long winsize,long bufsize);
	~CMaxIndex();

	virtual void DoAction(long Tick, const char * Instrument = NULL);
	inline long  * GetMaxPosition() const{return MaxPosition;};
	inline long GetMaxPosition(long Tick) const{return MaxPosition[Tick];};

private:
	const Type * OriginalData;
	//���뵱ǰTick�ڴ��ڷ�Χ�����ֵ���ڵ����λ�� 
	long * MaxPosition;
};

template <class Type>
class CMinIndex :
	public IIndexCalculator<Type>
{
public:
	CMinIndex(const Type * ori_data,long winsize,long bufsize);
	~CMinIndex();

	virtual void DoAction(long Tick, const char * Instrument = NULL);
	inline long  * GetMinPosition() const{return MinPosition;};
	inline long GetMinPosition(long Tick) const{return MinPosition[Tick];};

private:
	const Type * OriginalData;
	//���뵱ǰTick�ڴ��ڷ�Χ�����ֵ���ڵ����λ�� 
	long * MinPosition;
};