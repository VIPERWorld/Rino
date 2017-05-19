#ifndef DATADROCESSING_H
#define DATADROCESSING_H
#include <QObject>
#include <QWebSocket>
#include "websocket\WSMsgHandler.h"

//�������c++����web�ͻ��˷�������Ϣ��������Ӧ�Ĵ���,������뱻Ͷ�ݵ�
class DataDrocessing :public WSMsgHandler
{
	Q_OBJECT

public:
	DataDrocessing();
	~DataDrocessing();
	static DataDrocessing* inobgect();
	
	void web_Processing_1001(QJsonObject TYPE);//���ܵ�web��½��Ϣ
	void web_Processing_1004(QJsonObject TYPE);//���ܵ�web���������Ϣ
	void web_Processing_1005(QJsonObject TYPE);//���ܵ�web����LOL������������Ϣ
	void web_Processing_1006(QJsonObject TYPE);//���ܵ�web������������������

	//��������ڵ��ȡ�����б���
	void web_Processing_1013(QJsonObject TYPE);
	//�뿪��������ҳ�潻��
	void web_Processing_1014(QJsonObject TYPE);
	//��������Ƿ�����Ϸ״̬��־
	void web_Processing_1031(QJsonObject TYPE);
	//΢�ŵ�½��ά��ɨ�贰�ڴ򿪽���
	void web_Processing_9001();
	//΢�ŵ�½��ά��ɨ�贰�ڹرս���
	void web_Processing_9002(QJsonObject TYPE);
	//΢�ŵ�½��ά��ɨ���õ��û���Ϣ����ת������
	void web_Processing_9003(QJsonObject TYPE);
	//΢�ŵ�½��ά��ɨ��֪ͨ����ʹ���˻���½����
	void web_Processing_9004();
	//΢�ŵ�½��ά��ɨ��֪ͨ����ʹ��key+userid��½ҵ�����������
	void web_Processing_9005(QJsonObject TYPE);
	//�������޸�����
	void web_Processing_9006(QJsonObject TYPE);
	//��֪��ǰ��ʲô��Ϸid
	void web_Processing_1027(QJsonObject TYPE);
	//H5��Ҫע����Ҫ���ƹ�id
	void web_Processing_7000();
	//��֪��ǰ�ͻ��˰汾
	void web_Processing_1028();

	//��ס��������ҳ��ť
	void OnContestButLock();
	//������������ҳ��ť
	void OnContestButUnlock();
	//֪ͨh5��·�����ԣ������ٴ�������Ϸ·��
	void HTTPMessageProcessing_1012();
	//�����Ϸ·������
	void HTTPMessageProcessing_1010(QString &data);
	//���·����ȷ��
	bool CheckRoute(QString &str_result, QString &str_name);
	//����·��
	void SetRoute(QString &str_route, QString &str_name);
	//ȡweb��������ַ
	void OnGetWebAddr();
	//lol solo ����������
	void web_Processing_3012(QJsonObject TYPE);
	//��ͨ��¼ת΢�ŵ�¼
	void web_Processing_9011();
	//��ͨ��¼���ڹرս���
	void web_Processing_9012();
	//����Ƿ�鿴����ͼ
	void web_Processing_9010(QJsonObject TYPE);
	//¯ʯ�������pickӢ��
	void web_Processing_9016(QJsonObject TYPE);
	//������������ҷ���������Ϣ
	void web_Processing_9017(QJsonObject TYPE);
	//¯ʯ�������banӢ��
	void web_Processing_9021(QJsonObject TYPE);
	//֪ͨ��ҿ�ʼ����С����Ϸ�Ƿ�ɹ�
	//UINT resid
	//0 �ɹ�
	//1 ·������
	//2 ��������
	void tellEventGroupStartRes(int resid);
private:

	virtual void onRecvMsg(const QString &_msg, Cweb_c_type _type)//�����յ���websocket��Ϣ
	{
		QWebSocket *pSocket = qobject_cast<QWebSocket *>(sender());
		qDebug() << QStringLiteral("�յ�websocket��Ϣ��")<< _msg;
		Processing(_msg,_type);//���ݴ���
	}

	virtual void callbackConn(QWebSocket *_socket, bool _isSrv);

	void Processing(const QString &_msg, Cweb_c_type _type);//���ݴ���
public:
	enum HS_HERO_NAME
	{
		hs_hero_name_Rogue = 1,  //����
		hs_hero_name_Druid,      //��³��
		hs_hero_name_Mage,	     //��ʦ
		hs_hero_name_Hunter, 	 //����
		hs_hero_name_pastor,	 //��ʦ
		hs_hero_name_Shaman,	 //������˾
		hs_hero_name_Paladin,	 //ʥ��ʿ
		hs_hero_name_Warlock,	 //��ʿ
		hs_hero_name_Warrior,	 //սʿ
	};
};

#endif // DATADROCESSING_H
