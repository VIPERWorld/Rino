// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� WJSERVERMANAGELIB_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// WJSERVERMANAGELIB_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#ifndef _WJ_MAINMANAGE_H_
#define _WJ_MAINMANAGE_H_

#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
#include "server/comstruct.h"
#include "server/DataLine.h"
#include "server/WJSSocketManage.h"

namespace WJ
{
	/*
	�������ײ�ֱ������ϲ�ӿڵĲ���ʵ������һ�����̶߳��У�
	���е��׽��ֶ�ȡ���׽��ֹرա��׽������ӡ����ݿ⴦��������ʱ��ִ���¼���������
	*/

	//�������Ͷ���
#define HD_SOCKET_READ					1							//SOCKET ��ȡ�¼�����	
#define HD_SOCKET_CLOSE					2							//SOCKET �ر��¼�����
#define HD_SOCKET_CONNECT				3							//SOCKET �����¼�����
#define HD_DATA_BASE_RESULT				4							//���ݿ�������
#define HD_TIMER_MESSAGE				5							//��ʱ����Ϣ����

	//SOCKET �пͻ������ӳɹ�֪ͨ�ṹ����
	struct SocketConnectLine
	{
		DataLineHead						LineHead;					//����ͷ
		UINT								uIndex;						//SOCKT ����
		ULONG								uAccessIP;					//SOCKET IP
		
	};

	//SOCKET �ر�֪ͨ�ṹ����
	struct SocketCloseLine
	{
		DataLineHead						LineHead;					//����ͷ
		UINT								uIndex;						//SOCKT ����
		ULONG								uAccessIP;					//SOCKET IP
	};

	//SOCKET ��ȡ֪ͨ�ṹ����
	struct SocketReadLine
	{
		DataLineHead						LineHead;					//����ͷ
		NetMessageHead						NetMessageHead;				//���ݰ�ͷ
		UINT								uHandleSize;				//���ݰ������С
		UINT								uIndex;						//SOCKET ����
		ULONG								uAccessIP;					//SOCKET IP
		SocketReadLine()
		{
			memset(this, 0, sizeof(SocketReadLine));
		}
	};

	///���ݿ�����Ϣ�ṹ����
	struct DataBaseResultLine
	{
		DataLineHead						LineHead;					///����ͷ
		UINT								uHandleRusult;				///������
		UINT								uHandleKind;				///��������
		UINT								uIndex;						///��������

		DataBaseResultLine()
		{
			memset(this, 0, sizeof(DataBaseResultLine));
		}
	};

	//��ʱ����Ϣ�ṹ����
	struct TimerLine
	{
		DataLineHead						LineHead;					//����ͷ
		UINT								uTimerID;					//��ʱ�� ID

		TimerLine()
		{
			memset(this, 0, sizeof(TimerLine));
		}
	};
	/*****************************************************************************************************************/
}

#endif //_WJ_MAINMANAGE_H_