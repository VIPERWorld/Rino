#ifndef _WJ_DataLineHead_H_
#define _WJ_DataLineHead_H_

#include <mutex>
#include <list>
#include "common/WJBaseType.h"

namespace WJ
{
	//�����궨��
#define LD_DEF_STEP				4096								//���ݶ��л��岽��
#define LD_MAX_PART				8192								//���ݰ���󳤶�//3096 -> 1024*5 �ټ��� modify by wlr 20090716
#define LD_MAX_DATA				650000								//���ݶ�����󳤶�

	
#ifndef STRUCT_DATALINEHEAD
#define STRUCT_DATALINEHEAD

	enum DataLine_UserDataTag
	{//�ײ�������������첽���������ϲ�ֻ������һ�����е��̲߳�����������Ҫ��һ�������νӣ�
		//��һ����CDataLine�Ѿ�ʵ���ˣ�����һ�������������ܴ��ڶ��ʵ����ͬһ��ײ㴦��
		//�������ŵ�������ȥ��ʱ�����û�б�ǽ����������޷����ֵġ�����ÿһ�����нڵ㶼Ӧ�ô���
		//ʹ���߱�ǣ��Ա��ϲ�ҵ������Դ���

		e_UserDataTag_def = 0,//��ҵ��,Ĭ�ϴ���
		e_UserDataTag_LGsocketclient = 1,//��Ϸ��������,��Ϸ���������½������ͨ��
		e_UserDataTag_LGsocketserver = 2,//��½��������,��Ϸ���������½������ͨ��
	};
	
	//���ݶ�����Ϣͷ
	struct DataLineHead
	{
		UINT						uSize;								//���ݴ�С
		UINT						uDataKind;							//��������
		DataLine_UserDataTag		tag;								//�û����ݱ��

		DataLineHead()
		{
			std::memset(this, 0, sizeof(DataLineHead));
		}
	};
#endif //STRUCT_DATALINEHEAD
	struct RedisDataBaseLine
	{
		DataLineHead					LineHead;					///����ͷ
		UINT							uRediscommandid;			///redis����id
		UINT							uIndex;						///��������
		RedisDataBaseLine()
		{
			memset(this, 0, sizeof(RedisDataBaseLine));
		}
	};

	///redis�����Ϣ�ṹ����
	struct RedisResultLine
	{
		DataLineHead						LineHead;					///����ͷ
		UINT								uHandleRusult;				///������
		UINT								uRediscommandid;			///redis����id
		UINT								uIndex;						///��������
	};

	/*
	Struct		:ListItemData
	Memo		:���������ݽṹ
	Author		:Fred Huang
	Add Data	:2008-3-4
	Modify Data	:none
	Parameter	:
	stDataHead	:���ݰ�ͷ
	pData		:ÿ�����������ݵ�ָ�룬ʹ����new��ʽ������ڴ棬ע�⣬�ڳ�����ʱ��Ҫ��ʽ��delete ���ڴ�
	*/
	struct ListItemData
	{
		DataLineHead				stDataHead;
		BYTE						* pData;
	};
	//���ݶ�����
	class WJSERVERMANAGELIB_API CDataLine
	{
		//��������
	public:
		std::recursive_mutex m_dataMutex_CDataLine;						//�������������̶߳Ի���������
	private:
		std::list<ListItemData*> m_DataList;
		//��������
	public:
		//���캯��
		CDataLine();
		//��������
		virtual ~CDataLine(void);

		//���ܺ���
	public:
		//������������
		bool CleanLineData();
		//������Ϣ����
		virtual UINT AddData(DataLineHead * pDataInfo, UINT uAddSize, UINT uDataKind, void * pAppendData = nullptr, UINT uAppendAddSize = 0);
		//��ȡ��Ϣ����
		virtual UINT GetData(DataLineHead * pDataBuffer, UINT uBufferSize);
	public:
		int GetDataCount(void);
	};
}



#endif //_WJ_DataLineHead_H_