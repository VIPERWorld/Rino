#pragma once


#include <fstream>
#include <Windows.h>
#include <stdarg.h>

#define LOG_BUF_SIZE	1024*100

/******************************************************************
* @brief�� ϵͳ��־��
*/
class XLog
{
public:
	XLog();
	~XLog();


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
	/**
	* @brief�� �ͷ���Դ
	*/
	void release();


	/**
	* @brief�� ��ʼ���� ������־·����
	*/
	void initialize(const char* _logFile);


	/**
	* @brief�� ��ȡϵͳ��ǰʱ�� (�ַ�����ʽ yyyy-mm-dd hh:mm:ss.mms)
	*/
	char* getSysTimeBuffer();

	
	/**
	* @brief�� �����־
	*/
	void writeLog(const EnLogLevel& _level, const char* _format, ...);

	bool isOpen() { return mIsOpen; }

	void open() { mIsOpen = true; }

private:
	/**
	* @brief�� �ļ�������
	*/
	std::fstream mFSream;

	/**
	* @brief�� �ַ�������
	*/
	char mBuffer[LOG_BUF_SIZE];

	/**
	* @brief:	��־�ļ��Ƿ�򿪱�ʶ
	*/
	bool mIsOpen;

}; // $END$ class SystemLog