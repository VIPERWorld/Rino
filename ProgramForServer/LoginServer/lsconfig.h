#ifndef __WJ_clientconfig_H__
#define __WJ_clientconfig_H__

#include "json/json.h"
#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
class Clsconfig
{
	struct  Clsconfig_data
	{
		bool redis_on_off;//redis�����Ƿ���(true,false)
		std::string redis_address;//redis������ip
		int redis_port;//redis�˿�
		std::string redis_pass;//redis����
		int iMaxVirtualCount;//�����û��������
		int iMinVirtualCount;//�����û���С����
		Clsconfig_data()
		{
			redis_on_off = true;
			redis_address = "0.0.0.0";
			redis_port = 6380;
			redis_pass = "XnDj";
			iMaxVirtualCount = 800;
			iMinVirtualCount = 700;
		}
	};

public:
	// ��ȡ����
	static Clsconfig& getInstance();

	//��ʼ�������ļ����������ļ������ھʹ����ļ�������Ĭ�ϲ���д���ļ�
	void initconfig();
	//��ȡ����
	const Clsconfig_data* GetData();
private:
	// ���캯����˽�е�
	Clsconfig(void);

	// ��������
	virtual ~Clsconfig(void);

private:
	//�࿽����˽�е�
	Clsconfig(const Clsconfig &);
	Clsconfig & operator = (const Clsconfig &);

private:
	Clsconfig_data m_data;//����
};

#define  lsconfig()	Clsconfig::getInstance()

#endif//__WJ_clientconfig_H__
