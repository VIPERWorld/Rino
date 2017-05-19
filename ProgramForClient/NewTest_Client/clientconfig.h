#ifndef __WJ_clientconfig_H__
#define __WJ_clientconfig_H__

#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
class Ccconfig
{
	struct  Ccconfig_data
	{
		std::string hall_server_address;//��½������ip
		std::string web_server_address;//web������ip
		UINT hall_server_port;//��½�������˿�
		UINT AInum;//��������Ŀ
		UINT AIFirstID;//��������ʼid
		UINT contestid;//������Ϸ���еı���id
		UINT roomid;//��Ҫ����ķ���ķ���id
		Ccconfig_data()
		{
			hall_server_address = "127.0.0.1";
			web_server_address = "127.0.0.1";
			hall_server_port = PORT_LS;
			AInum = 200;
			contestid = 27;
			roomid = 756;
		}
	};

public:
	// ��ȡ����
	static Ccconfig& getInstance();

	//��ʼ�������ļ����������ļ������ھʹ����ļ�������Ĭ�ϲ���д���ļ�
	void initconfig(){}
	//��ȡ����
	Ccconfig_data* GetData();
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