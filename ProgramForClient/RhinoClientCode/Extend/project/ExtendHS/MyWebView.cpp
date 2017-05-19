#include "MyWebView.h"

MyWebView::MyWebView(QWidget *parent)
	: QWebEngineView(parent)
	, m_hWinHS(NULL)
{
	//QObject::connect(this, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));
	//QObject::connect(this, SIGNAL(loadProgress(int)), this, SLOT(onLoadProgress(int)));
	//QObject::connect(this, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));

	//setWindowOpacity(1);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::Dialog);
	setAttribute(Qt::WA_TranslucentBackground, true);
	//this->setAutoFillBackground(true);
	this->page()->setBackgroundColor(QColor(0, 0, 0, 0));
}

MyWebView::~MyWebView()
{

}

void MyWebView::setHSHwnd(HWND _hWnd)
{
	m_hWinHS = _hWnd;
	//::SetParent((HWND)winId(), m_hWinHS);
}

void MyWebView::paintEvent(QPaintEvent *)
{
	//QPainter painter(this);
	//painter.drawPixmap(0, 0, m_backgroundPix);//����ͼ��
	//painter.fillRect(this->rect(), QColor(255,255,0,50));
}

void MyWebView::timerEventFunction()
{
	//qDebug("timer event");
}

#include <QDebug>

void MyWebView::onLoadStarted()
{
	//this->hide();
	//this->setWindowOpacity(0);
}
void MyWebView::onLoadProgress(int progress)
{
	//if (progress == 100)
	//{
	//	this->setWindowOpacity(1);
	//	this->show();
	//}
}
void MyWebView::onLoadFinished(bool result)
{
	//printf("ҳ��������: %s\n", result ? "�ɹ�" : "ʧ��");
}


void MyWebView::enterEvent(QEvent *)
{
	//::MoveWindow((HWND)winId(), m_pos.x(), m_pos.y(), m_size.width(), m_size.height(), true);
	//static QPropertyAnimation* anima = new QPropertyAnimation(this, "geometry");
	//anima->setDuration(1000);
	//anima->setStartValue(QRect(m_x, m_y, m_width, 43));
	//anima->setEndValue(QRect(m_x, m_y, m_width, m_height));
	//anima->start();
	::SwitchToThisWindow(m_hWinHS, true);
}

void MyWebView::leaveEvent(QEvent *)
{
	//::MoveWindow((HWND)winId(), m_pos.x(), m_pos.y(), m_size.width(), 40, true);
	//resize(m_size.width(), 43);

	//static QPropertyAnimation* anima = new QPropertyAnimation(this, "geometry");
	//anima->setDuration(1000);
	//anima->setStartValue(QRect(m_x, m_y, m_width, m_height));
	//anima->setEndValue(QRect(m_x, m_y, m_width, 43));
	//anima->start();
	//
	//if (m_hWndParent)
	//{
	//	SwitchToThisWindow(m_hWndParent, true);
	//}
	::SwitchToThisWindow(m_hWinHS, true);
}


void MyWebView::setWindowSize(int _x, int _y, int _w, int _h)
{
	m_x = _x;
	m_y = _y;
	m_width = _w;
	m_height = _h;

	RECT rt;
	::GetWindowRect(m_hWinHS, &rt);

	//move(rt.left + _x, rt.top + _y);
	move(_x, _y);
	resize(_w, _h);
}
