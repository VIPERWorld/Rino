#pragma once

#ifndef COMPLETE_H
#define COMPLETE_H

class CComplete : public WindowImplBase {

private:
protected:
public:
	CComplete();
	~CComplete();

	//Ƥ���ļ�
	CDuiString																				GetSkinFile();
	//����������
	LPCTSTR																					GetWindowClassName() const;
	//��ʼ��
	void																					InitWindow();
	//����¼�
	void																					Notify(TNotifyUI& msg);

	//�����ͻ���
	void																					StartClient();
};

#endif