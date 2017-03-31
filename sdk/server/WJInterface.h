//Copyright (c) 2012��2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved
#pragma once

#ifndef WJ_INTERFACE_FILE
#define WJ_INTERFACE_FILE

#include "../common/WJBaseType.h"
#include "../message/WJNetMessageHead.h"
/********************************************************************************************/

///����ӿڶ���

/********************************************************************************************/
namespace WJ
{
	class CTCPClientSocket;				///<SOCKET ��

	///�������������ӿ� 
	class IServerSocketService
	{
		///�ӿں���
	public:
		//���ڿͻ������ӳɹ�
		virtual bool OnSocketConnectEvent_s(ULONG uAccessIP, UINT uIndex, UINT tag=0) = 0;
		///����رմ���
		virtual bool OnSocketCloseEvent_s(ULONG uAccessIP, UINT uIndex, UINT tag = 0) = 0;
		///������Ϣ����
		virtual bool OnSocketReadEvent_s(ULONG uAccessIP, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uIndex, UINT tag = 0) = 0;
	};

	///�ͻ����������ӿ�
	class IClientSocketService
	{
		///�ӿں��� 
	public:
		///�����ȡ��Ϣ
		virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket) = 0;
		///����������Ϣ
		virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket) = 0;
		///����ر���Ϣ
		virtual bool OnSocketCloseEvent() = 0;
	};

	/////���ݿ�������ӿ�
	//class IDataBaseResultService
	//{
	//	///�ӿں���
	//public:
	//	///���ݿ�������
	//	virtual bool OnDataBaseResultEvent(DataBaseResultLine * pResultData, UINT uHandleKind, UINT uHandleResult, UINT uResultSize, UINT uIndex)=0;
	//};
	//
	/////���ݿ⴦�����ӿ�
	//class IDataBaseHandleService
	//{
	//	///�ӿں���
	//public:
	//	///���ò���
	//	virtual bool SetParameter(IDataBaseResultService * pRusultService, CAFCDataBaseManage * pDataBaseManage, ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData)=0;
	//	///���ݿ⴦��ӿ�
	//	virtual UINT HandleDataBase(DataBaseLineHead * pSourceData)=0;
	//};
	//
}

#endif//WJ_INTERFACE_FILE