//Copyright (c) 2012��2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved
#pragma once

#ifndef _WJ_WJException_H_
#define _WJ_WJException_H_
#include "../common/WJBaseType.h"



#include <WinBase.h>
#include <Winuser.h>
#include <stdlib.h>
namespace WJ
{
	///***********************************************************************************************///
	///ϵͳ����
#define EX_UNKNOWN_ERROR			0						///δ֪����
#define EX_INIT_ERR					1						///��ʼ��ʧ��
#define EX_HEAP_MEM_LOW				2						///���ڴ治��
#define EX_IOCP_ERR					3						///��ɶ˿ڴ���

	///���ݿ����
#define EX_DATA_CONNECT				100						///���ݿ����Ӵ���
#define EX_DATA_USER				101						///���ݿ����ִ���
#define EX_DATA_PASS				102						///���ݿ��������
#define EX_DATA_EXEC				103						///���ݿ�ִ�д���

	///�������
#define EX_SOCKET_CREATE_ERR		200						///���罨������
#define EX_SOCKET_BIND_ERR			201						///����󶨴���
#define EX_SOCKET_LISTEN_ERR		202						///�����������
#define EX_SOCKET_SEND_ERR			203						///���緢�ʹ���
#define EX_SOCKET_RECV_ERR			204						///������ܴ���
#define EX_SOCKET_PACKET_ERR		205						///�������ݰ��쳣

	///�������
#define EX_SERVICE_START			300						///��������
#define EX_SERVICE_PAUSE			301						///������ͣ
#define EX_SERVICE_STOP				302						///����ֹͣ
#define EX_SERVICE_BUSY				303						///����æ
#define EX_SERVICE_UNKNOW			304						///δ֪������Ϣ

	///�Զ������

	///***********************************************************************************************///

	///�쳣��
	class WJSERVERMANAGELIB_API CWJException
	{
		///��������
	protected:
		UINT					m_uErrorCode;					///������
		bool					m_bAutoDelete;					///�Ƿ��Զ�ɾ��
		char					m_szMessage[255];				///������Ϣ
		static bool				m_bShowError;					///�Ƿ���ʾ����

		///��������
	public:
		///���캯��
		inline CWJException(char * szErrorMessage, UINT uErrorCode = EX_UNKNOWN_ERROR, bool bAutoDelete = true);
		///��������
		virtual ~CWJException(void);

		///���ܺ���
	public:
		///��ȡ�������
		virtual UINT GetErrorCode() const;
		///��ȡ������Ϣ
		virtual char const * GetErrorMessage(char * szBuffer, int iBufLength) const;
		///ɾ������
		virtual bool Delete();
		///�����Ƿ���ʾ����
		static bool ShowErrorMessage(bool bShowError);
	};
}

#endif//_WJ_WJException_H_