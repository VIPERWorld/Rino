#include "lsconfig.h"
#include <io.h>
Clsconfig& Clsconfig::getInstance()
{
	static Clsconfig cconfig;
	return cconfig;
}

Clsconfig::Clsconfig()
{

}

Clsconfig::~Clsconfig()
{

}

//��ʼ�������ļ����������ļ������ھʹ����ļ�������Ĭ�ϲ���д���ļ�
void Clsconfig::initconfig()
{
	bool bExit = false;
	std::string strFile = "lsconfig.json";
	if ((_access(strFile.c_str(), 0)) != -1)
	{
		bExit = true;
	}
	if (bExit)
	{
		FILE*fp = fopen(strFile.c_str(), "rb");// localfile�ļ���       
		fseek(fp, 0L, SEEK_END); /* ��λ���ļ�ĩβ */
		int nLen = ftell(fp); /* �õ��ļ���С */
		char *pData = new char[nLen + 1]; /* �����ļ���С��̬�����ڴ�ռ� */
		if (NULL == pData)
		{
			fclose(fp);
			return;
		}
		memset(pData, 0, nLen + 1);

		fseek(fp, 0L, SEEK_SET); /* ��λ���ļ���ͷ */
		fread(pData, nLen, 1, fp); /* һ���Զ�ȡȫ���ļ����� */
		std::string strJsonString = pData;
		if (pData)
		{
			delete[]pData;
		}
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(strJsonString, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
		{

			Json::Value val = root["SysSet"];
			{//redis�����Ƿ���(true,false)
				m_data.redis_on_off = val["redis_on_off"].asBool();
			}
			{//redis������ip
				m_data.redis_address = val["redis_address"].asString();
			}
			{//redis�˿�
				m_data.redis_port = val["redis_port"].asInt();
			}
			{//redis����
				m_data.redis_pass = val["redis_pass"].asString();
			}
			{//�����û��������
				m_data.iMaxVirtualCount = val["MaxVirtualCount"].asInt();
			}
			{//�����û���С����
				m_data.iMinVirtualCount = val["MinVirtualCount"].asInt();
			}
		}
	}
	else
	{
		std::string str = "{\n"
			"\"SysSet\" :\n"
			"	{\n"
			"	\"redis_on_off\": true,\n"
			"	\"redis_address\": \"118.178.25.88\",\n"
			"	\"redis_port\": 6380,\n"
			"	\"redis_pass\": \"XnDj\",\n"
			"	\"MaxVirtualCount\": 800,\n"
			"	\"MinVirtualCount\": 700 \n"
			"	}\n"
			"}\n";
		FILE *fp = fopen(strFile.c_str(), "wb");
		if (fp)
		{
			fwrite(str.c_str(), str.length(), 1, fp);
			fclose(fp);
		}
	}
}

//��ȡ����
const Clsconfig::Clsconfig_data* Clsconfig::GetData()
{
	static bool bInit = false;
	if (!bInit)
	{
		initconfig();
		bInit = true;
	}
	return &m_data;
}
