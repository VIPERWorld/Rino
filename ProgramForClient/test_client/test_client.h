#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H

#include <mutex>
#include <vector>
#include <QtWidgets/QMainWindow>
#include "ui_test_client.h"
#include "WJPlatformLogic.h"

class test_client : public QMainWindow
{
	Q_OBJECT

public:
	test_client(QWidget *parent = 0);
	~test_client();

private:
	Ui::test_clientClass ui;
	std::vector<WJ::WJPlatformLogic*> m_vector_logic_struct;
	bool m_bMakeSocket;//�Ƿ������߼����
	bool m_runthread;//�Ƿ�ִ���߳�
	std::mutex m_dataMutex_test_client;			//����ͬ�����������������̶߳Ի���������

	QTimer *m_timer;//���м�ʱ��

private slots:
	//ִ�����Ӷ���
	void onsocketconnect();
	//ִ�з��Ͷ���
	void onsocketRun();
	//ִ�йرն���
	void onsocketClose();

	//������ ����
	//������ ����
	//��½�� ����
	//��½�� ����
	//��������
	//��Ϸ����
	//��������

	//���׽����߼��߳�
	int MakeSocketThread();
	//��������߼�����
	void clear_socketlogic();

	//�û�������ʱ��
	void onTimeRun();
};

#endif // TEST_CLIENT_H
