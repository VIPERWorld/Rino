#pragma once

#ifndef CANCEL_H
#define CANCEL_H

class CCancel : public WindowImplBase {

private:
protected:
public:
	CCancel();
	~CCancel();

	//Ƥ��
	CDuiString																				GetSkinFile();
	//����
	LPCTSTR																					GetWindowClassName() const;
	//��ʼ��
	void																					InitWindow();
	//��Ӧ
	void																					Notify(TNotifyUI& msg);
};

#endif