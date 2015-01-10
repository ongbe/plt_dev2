#pragma once

#include "ITimeAction.h"
#include <pthread.h>

class IEventQueue
{
public:
	IEventQueue(size_t size);
	virtual ~IEventQueue(void);

	void SendEvent(long tick, const char * instrument, ITimeAction * action);

	void ReInit();

	void Resize(size_t size);

protected:
	pthread_mutex_t lock;
	pthread_cond_t cond;
	pthread_spinlock_t spinlock; //����Ϣ�����ã�����ͽ����߳̿���ͬʱ�����¼�

	size_t bufSize;
	long * TickBuf;
	const char ** InstrumentID;
	ITimeAction ** ActionBuf;

	int currentIndex;
	int executeIndex;
};
