#pragma once

#include "Common.h"

template <class Type>
class IHistoryData	//��ʷ���ݽӿ�
{
public:
	IHistoryData(){};
	virtual ~IHistoryData(){};

	//idxֻ֧�ָ���������ָ�����ʱ��ȡ��ʷ���ݡ�idx��Ϊ�ӽ��տ�ʼ��ǰ��Tick��ֵ��-1���������յ����̼�¼��
	virtual Type operator[](long idx) const = 0;
};

template <class Type>
class SimpleHistoryData: public IHistoryData<Type>
{
public:
	//sizeΪ���ݴ洢��С
	SimpleHistoryData(long size):Size(size){Data=new Type[size];WritingPoint=(char*)Data;};
	SimpleHistoryData():Size(0),Data(NULL){};
	virtual ~SimpleHistoryData(){if(Data) delete [] Data;};

	//idxֻ֧�ָ���������ָ�����ʱ��ȡ��ʷ���ݡ�idx��ΪTick��ֵ
	virtual Type operator[] (long idx) const{return Data[Size+idx];};

	//�˺��������ļ���˳�����ν��ļ������洢�Ķ������������μ�����Data��ַ������ʱ���ʱ������ļ�¼��ʼ���أ�
	inline void LoadData(char * filename){WritingPoint += LoadBinFileToMem(filename,WritingPoint);};

protected:
	Type * Data;
	long Size;

	char * WritingPoint;
};

template <class Type>
class SampleHistoryData: public SimpleHistoryData<Type>	//������ʷ����
{
public:
	//sizeΪ���ݴ洢��С��stepΪ������ٸ�Tick���еĲ���
	SampleHistoryData(long size,long step):SimpleHistoryData<Type>(size),Step(step){};
	virtual ~SampleHistoryData(){};

	//idxֻ֧�ָ���������ָ�����ʱ��ȡ��ʷ���ݡ�idx��ΪTick��ֵ
	virtual Type operator[] (long idx) const{return this->Data[this->Size + (++idx)/Step];};

protected:
	long Step;
};

template <class Type>
class PatchedHistoryData:public IHistoryData<Type>	//�Ե��յ�һ�����������ʷ����
{
public:
	//sizeΪ���ݴ洢��С��stepΪ������ٸ�Tick���еĲ���
	PatchedHistoryData(const Type * firstdata):FirstData(firstdata){};
	virtual ~PatchedHistoryData(){};

	//idxֻ֧�ָ���������ָ�����ʱ��ȡ��ʷ���ݡ�idx��ΪTick��ֵ
	virtual Type operator[] (long idx) const{return *FirstData;};

protected:
	const Type * FirstData;
};
