#ifndef QMBOX_H
#define QMBOX_H

#include <QDialog>
#include "ui_QMbox.h"

class QMbox : public QDialog
{
	Q_OBJECT

public:
	QMbox(QWidget *parent = 0);
	~QMbox();
	// ������ʾ����
	void setcontent(QString content);
	// ����OK��ť����
	void SetTextOKBtn(QString OKbutton);
	// ����NO��ť����
	void SetTextNOBtn(QString NObutton);
	// ����NO��ť
	void hideBtuno();
signals:
	void signal_NOOK(QString);
private slots:
	void slots_OK();
	void slots_NO();

private:
	Ui::QMbox ui;
	bool isToConfirmOrCancel;
};

#endif // QMBOX_H
