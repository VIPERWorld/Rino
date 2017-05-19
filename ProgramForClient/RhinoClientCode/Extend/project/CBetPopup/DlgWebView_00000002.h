#pragma once

#include <QObject>
#include <QtWebEngineWidgets/QWebEngineView>

#include <QtCore/QAbstractNativeEventFilter>

class DlgWebView_00000002 : public QWebEngineView/*, public QAbstractNativeEventFilter*/
{
	Q_OBJECT
public:
	DlgWebView_00000002(QWidget *parent = 0);
	~DlgWebView_00000002();

public:
	/**
	* \brief:	���ô��ڵĿ�Ⱥ͸߶ȣ���λ����
	*/
	void setWindowSize(int _w, int _h);


	void setHSHWin(HWND _hWnd);

	///**
	// * \brief paintEvent ������Ⱦ�̳��࣬ͨ������ʵ�ֱ�������
	// */
	//virtual void paintEvent(QPaintEvent *);

	///**
	// * \brief timerEventFunction ��ʱ��
	// */
	//void timerEventFunction();


public:

	/* �����봰������ */
	virtual void enterEvent(QEvent *);

	/* ����뿪�������� */
	virtual void leaveEvent(QEvent *);

	//	private slots:
	//	/**
	//	 * @brief onLoadStarted ������ɿ�ʼʱ���е���
	//	 */
	//	void onLoadStarted();
	//	/**
	//	 * @brief onLoadProgress ���ع����н��е���
	//	 * @param progress
	//	 */
	//	void onLoadProgress(int progress);
	//	/**
	//	 * @brief onLoadFinished ���ؽ���ʱ���е���
	//	 */
	//	void onLoadFinished(bool);





	//	//Q_SIGNALS:
	//	//    virtual void loadStarted();
	//	//    virtual void loadProgress(int progress);
	//	//    virtual void loadFinished(bool);
	//	//    void titleChanged(const QString& title);
	//	//    void selectionChanged();
	//	//    void urlChanged(const QUrl&);
	//	//    void iconUrlChanged(const QUrl&);
	//	//    void renderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationStatus,
	//	//                             int exitCode);
	//
	//
public:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	HWND m_hWinHS;
};

