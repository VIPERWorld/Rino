#ifndef __WJ_clientconfig_H__
#define __WJ_clientconfig_H__

#include <string>
#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"

class Cgsconfig
{
#define CONFIG_CONTEST_MAXNUM 100
	struct  Cgsconfig_data
	{
		std::string loginserver_address;//��½������ip
		std::string ip_for_client; // ���ͻ���ֱ����ip
        int port_for_client;
        int max_client_count;
		std::string redis_address;//redis������ip
		int redis_port;//redis�˿�
		std::string redis_pass;//redis����
		UINT contestid[CONFIG_CONTEST_MAXNUM];//���ط�����еı���id
		Cgsconfig_data()
		{
			loginserver_address = "127.0.0.1";
            port_for_client = 9000;
            max_client_count = 10000;
			redis_address = "118.178.25.88";
			redis_port = 6380;
			redis_pass = "XnDj";
			std::memset(contestid, 0, sizeof(contestid));
		}
	};

public:
	// ��ȡ����
	static Cgsconfig& getInstance();

	//��ʼ�������ļ����������ļ������ھʹ����ļ�������Ĭ�ϲ���д���ļ�
	void initconfig();
	//��ȡ����
	const Cgsconfig_data* GetData();
private:
	// ���캯����˽�е�
	Cgsconfig(void);

	// ��������
	virtual ~Cgsconfig(void);

private:
	//�࿽����˽�е�
	Cgsconfig(const Cgsconfig &);
	Cgsconfig & operator = (const Cgsconfig &);

private:
	Cgsconfig_data m_data;//����
};

#define  gsconfig()	Cgsconfig::getInstance().GetData()

#endif//__WJ_clientconfig_H__