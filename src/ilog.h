#pragma once
#include <boost/asio.hpp> 
#include <stdio.h>
using namespace boost::asio;

class ILog
{
public:
	ILog(void):ScreenOutput(false){};
	virtual ~ILog(void){};

	inline void EnableScreenOutput(){ScreenOutput = true;};
	inline void DisableScreenOutput(){ScreenOutput = false;};

	virtual int WriteLog(const char * format, ...);

protected:
	bool ScreenOutput;
};

class FileLog: public ILog
{
public:
	//filename = NULL --> stderr
	FileLog(const char * filename = NULL);
	FileLog(FILE * file):File(file){};
	virtual ~FileLog(void){fclose(File);};

	virtual int WriteLog(const char * format, ...);

protected:
	FILE * fh;
};

class SocketLog: public ILog
{
public:
	SocketLog(const char * url,int port);
	virtual ~SocketLog(void){};

	virtual int WriteLog(const char * format, ...);

protected:
	// ����asio�඼��Ҫio_service���� 
	io_service iosev; 
	// socket���� 
	ip::tcp::socket socket; 
	// ���Ӷ˵㣬����ʹ���˱������ӣ������޸�IP��ַ����Զ������ 
	ip::tcp::endpoint ep; 
	// ������� 
	boost::system::error_code ec; 	
};

class SocketAndFileLog: public FileLog, public SocketLog
{
public:
	SocketAndFileLog(const char * url,int port, const char * filename = NULL);
	virtual ~SocketAndFileLog(void){};

	virtual int WriteLog(const char * format, ...);
};

extern ILog DefaultLogger;
