#pragma once

#include <fstream>
#include <Windows.h>
#include <stdarg.h>
#include <string>
class QString;
//class CSystemLog;

//ϵͳ��־�࣬  ������Ϊ��ʵ���ࣩ
class CSystemLog
{
public:

	// ���캯��˽�л���ͨ��getInstance()����ȡʵ��ָ��
	CSystemLog();

public:
	~CSystemLog();

public:
	enum EnLogLevel
	{
		E_LOG_DEBUG,	// ������Ϣ
		E_LOG_NOTICE,	// ��ʾ��Ϣ
		E_LOG_WARNING,	// ����
		E_LOG_ERROR,	// ����
		E_LOG_FATAL,	// ��������
	};

public:

	static CSystemLog* getInstance();

	// �ͷ���Դ
	void release();	
	
	// ��ʼ���� �������򿪵������־�ļ�
	void initialize();
	
	// ��ȡϵͳ��ǰʱ�� (�ַ�����ʽ yyyy-mm-dd hh:mm:ss.mms)	
	char* getSysTimeBuffer();
		
	// ��ʽ�������־���ļ�(buf������)	
	void LogToFile(char* format, ...);

	// �Ǹ�ʽ�������־���ļ�(buf������)
	void LogToFile(const char* buf);


	// �Ǹ�ʽ�������־���ļ�,strҪʹ��ǰҪת��utf8����
	void LogToFile(std::string str);

#ifdef _UNUSE_QT

#else
	// �Ǹ�ʽ�������־���ļ�,strҪʹ��ǰҪת��utf8����
	void LogToFile(QString str);
#endif

	// ��ʽ�������־��������(buf������)
	void LogToDebug(char* lpPre, char* format, ...);

private:
	//Utf8ToGb2312
	char* U2G(const char* utf8);
	//GB2312��UTF-8��ת��
	char* G2U(const char* gb2312);
	int IsTextUTF8(const char* str, ULONGLONG length);
private:
	// �ļ�������	
	std::fstream mFSream;
	
	// �ַ�������	
#define BUFFERSIZE 1024 * 1000
	char mBuffer[BUFFERSIZE];

}; // $END$ class CSystemLog

#define SystemLog() CSystemLog::getInstance()

//DEBUG ������־
#define LOG_DEBUG(...)		\
if (SystemLog()->isOpen()) \
{ \
	SystemLog()->LogToFile(CSystemLog::E_LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);	\
}

//NITICE ��ʾ��־
#define LOG_NITICE(...)		\
if (SystemLog()->isOpen()) \
{ \
	SystemLog()->LogToFile(CSystemLog::E_LOG_NOTICE, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);	\
}

//	WARNING �澯��־
#define LOG_WARNING(...)		\
if (SystemLog()->isOpen()) \
{ \
	SystemLog()->LogToFile(CSystemLog::E_LOG_WARNING, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);	\
}

//	ERROR ������־
#define LOG_ERROR(...)		\
if (SystemLog()->isOpen()) \
{ \
	SystemLog()->LogToFile(CSystemLog::E_LOG_ERROR, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);	\
}

//	FATAL ����������־
#define LOG_FATAL(...)		\
if (SystemLog()->isOpen()) \
{ \
	SystemLog()->LogToFile(CSystemLog::E_LOG_FATAL, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);	\
}