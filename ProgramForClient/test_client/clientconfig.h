#ifndef __WJ_clientconfig_H__
#define __WJ_clientconfig_H__

#include <QtCore>
#include "QJsonArray"
#include "QJsonDocument"
#include "QFile"
#include "qlogging.h"
#include "QJsonObject"
#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
class Ccconfig
{
	struct  Ccconfig_data
	{
		QString hall_server_address;//��½������ip
		QString web_server_address;//web������ip
		UINT hall_server_port;//��½�������˿�
		UINT gameid;//������Ϸ���е���Ϸid
		UINT ereaid;//������Ϸ���е�����id
		UINT contestid;//������Ϸ���еı���id
		UINT roomid;//��Ҫ����ķ���ķ���id
		Ccconfig_data()
		{
			hall_server_address = "127.0.0.1";
			web_server_address = "127.0.0.1";
			hall_server_port = PORT_LS;
			gameid = 0;
			ereaid = 0;
			contestid = 1;
			roomid = 737;
		}
	};

public:
	// ��ȡ����
	static Ccconfig& getInstance();

	//��ʼ�������ļ����������ļ������ھʹ����ļ�������Ĭ�ϲ���д���ļ�
	void initconfig();
	//��ȡ����
	const Ccconfig_data* GetData();
private:
	// ���캯����˽�е�
	Ccconfig(void);

	// ��������
	virtual ~Ccconfig(void);

private:
	//�࿽����˽�е�
	Ccconfig(const Ccconfig &);
	Ccconfig & operator = (const Ccconfig &);

private:
	Ccconfig_data m_data;//����
};

#define  clientconfig()	Ccconfig::getInstance()

#endif//__WJ_clientconfig_H__