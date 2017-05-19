#pragma once

#ifndef DLGBETVIEW_H
#define DLGBETVIEW_H

#include <QObject>
#include <QtWebEngineWidgets/QWebEngineView>

#include <QtCore/QAbstractNativeEventFilter>


class CDlgBetView : public QWebEngineView {

	Q_OBJECT
private:
protected:
public:
	CDlgBetView(QWidget *parent = 0);
	~CDlgBetView();

	void setHwnd_Operate(HWND hwnd_operate){ m_hwnd_operate = hwnd_operate; }
	void setHwnd_Tell(HWND hwnd_tell){ m_hwnd_tell = hwnd_tell; }
	void setWind_Init_Station(long lwind_init_station){ m_wind_init_station = lwind_init_station; }
	void StartTime();
	void KillTime();

protected:
	virtual void timerEvent(QTimerEvent *);

public:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

	//���ô�����괩͸(1 ��͸ 0 ����͸)
	void setWindMousePenetrate(bool bPenetrate);
	int m_TimeId;
private:
	HWND m_hwnd_operate;//�������Ĵ��ھ��
	long m_wind_init_station;//�������Ĵ��ڴ������ڵ�ʱ���ʼ״̬����겻�ɴ�͸��
	HWND m_hwnd_tell;//��֪ͨȥ������Ϣ��H5�Ĵ��ھ��
};

#endif