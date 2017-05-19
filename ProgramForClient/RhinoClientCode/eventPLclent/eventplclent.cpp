#include "eventplclent.h"
#include "message\WJNetPM_gamelist.h"
#include <QLibrary>
eventPLclent::eventPLclent(QWidget *parent)
	:m_LoginInter(NULL),
	m_TeamChallengePage(NULL),
	m_SystemSettings(NULL),
	m_ClientLink(NULL),
	m_linkCtoQ(NULL),
	m_webPopup(NULL),
	QMainWindow(parent)
{
	ui.setupUi(this);
	Qt::WindowFlags flag = (Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	setWindowFlags(flag);
	setAttribute(Qt::WA_TranslucentBackground, true);

	CCommon::strlist.clear();

	connect(ui.pushButton_AIEcho, SIGNAL(clicked()), this, SLOT(slot_pushButton_AIEcho()));
	connect(ui.pushButton_juan, SIGNAL(clicked()), this, SLOT(slot_pushButton_juan()));
	connect(ui.pushButton_Noxus, SIGNAL(clicked()), this, SLOT(slot_pushButton_Noxus()));
	connect(ui.pushButton_Bandle, SIGNAL(clicked()), this, SLOT(slot_pushButton_Bandle()));
	connect(ui.pushButton_APyrgi, SIGNAL(clicked()), this, SLOT(slot_pushButton_APyrgi()));
	connect(ui.pushButton_WarCollege, SIGNAL(clicked()), this, SLOT(slot_pushButton_WarCollege()));
	connect(ui.pushButton_GiantGodPeak, SIGNAL(clicked()), this, SLOT(slot_pushButton_GiantGodPeak()));
	connect(ui.pushButton_ReetheGarrison, SIGNAL(clicked()), this, SLOT(slot_pushButton_ReetheGarrison()));
	connect(ui.pushButton_LandOfJudgment, SIGNAL(clicked()), this, SLOT(slot_pushButton_LandOfJudgment()));
	connect(ui.pushButton_BlackRose, SIGNAL(clicked()), this, SLOT(slot_pushButton_BlackRose()));
	connect(ui.pushButton_ShadowLsles, SIGNAL(clicked()), this, SLOT(slot_pushButton_ShadowLsles()));
	connect(ui.pushButton_SteelHeat, SIGNAL(clicked()), this, SLOT(slot_pushButton_SteelHeat()));
	connect(ui.pushButton_BalancedDenomination, SIGNAL(clicked()), this, SLOT(slot_pushButton_BalancedDenomination()));
	connect(ui.pushButton_CrystalSScar, SIGNAL(clicked()), this, SLOT(slot_pushButton_CrystalSScar()));
	connect(ui.pushButton_ShadowFlow, SIGNAL(clicked()), this, SLOT(slot_pushButton_ShadowFlow()));
	connect(ui.pushButton_WatchOfTheSea, SIGNAL(clicked()), this, SLOT(slot_pushButton_WatchOfTheSea()));
	connect(ui.pushButton_ConquestOfTheSea, SIGNAL(clicked()), this, SLOT(slot_pushButton_ConquestOfTheSea()));
	connect(ui.pushButton_Kalamunda, SIGNAL(clicked()), this, SLOT(slot_pushButton_Kalamunda()));
	connect(ui.pushButton_CityGuard, SIGNAL(clicked()), this, SLOT(slot_pushButton_CityGuard()));
	connect(ui.pushButton_BaoshBirgi, SIGNAL(clicked()), this, SLOT(slot_pushButton_BaoshBirgi()));
	connect(ui.pushButton_Demacia, SIGNAL(clicked()), this, SLOT(slot_pushButton_Demacia()));
	connect(ui.pushButton_FerrerJuud, SIGNAL(clicked()), this, SLOT(slot_pushButton_FerrerJuud()));
	connect(ui.pushButton_FearlessVanguard, SIGNAL(clicked()), this, SLOT(slot_pushButton_FearlessVanguard()));
	connect(ui.pushButton_TwistedTreeline, SIGNAL(clicked()), this, SLOT(slot_pushButton_TwistedTreeline()));
	connect(ui.pushButton_TheWrathofRama, SIGNAL(clicked()), this, SLOT(slot_pushButton_TheWrathofRama()));
	connect(ui.pushButton_Dragon, SIGNAL(clicked()), this, SLOT(slot_pushButton_Dragon()));
	connect(ui.pushButton_Education, SIGNAL(clicked()), this, SLOT(slot_pushButton_Education()));



	m_linkCtoQ = new linkCtoQ(this);
	connect(m_linkCtoQ, SIGNAL(signa_regame()), this, SLOT(slot_regame()));
	connect(m_linkCtoQ, SIGNAL(signa_Alreadyregistered()), this, SLOT(slot_Alreadyregistered()));
	connect(m_linkCtoQ, SIGNAL(signa_LoadUserinformation()), this, SLOT(slot_LoadUserinformation()));
	
	m_ClientLink = new ClientLink();//����
	connect(m_linkCtoQ, SIGNAL(signa_linkland(bool)), m_ClientLink, SLOT(slot_landlink(bool)));
	m_ClientLink->exec();
	//if (!m_ClientLink->isllink)
	//{
	//	iscolseclent = false;
	//	this->close();
	//	return;
	//}

	m_LoginInter = new LoginInter(NULL);//��½
	connect(m_LoginInter, SIGNAL(signa_logshowclient()), m_ClientLink, SLOT(slot_showclient()));
	connect(m_linkCtoQ, SIGNAL(signa_Landgame()), m_LoginInter, SLOT(slot_Landgame()));
	m_LoginInter->exec();
	if (!m_LoginInter->iscolseclent)
	{
		iscolseclent = false;
		this->close();
		return;
	}
	
	initUI();

	//m_webPopup = new webPopup(this);
	//ui.gridLayout_9->addWidget(m_webPopup);

	//engine->load(QUrl("C:/web/web.html"));//����

	//m_webView.setParent(this);
	////m_webView.move(0, 0);
	//////m_webView.resize(1024, 768);
	//channel = new QWebChannel(this);
	//m_webView.page()->setWebChannel(channel);
	//channel = new QWebChannel(this);
	//channel->registerObject("bridge", (QObject*)bridge::instance());
	//m_webView.page()->setWebChannel(channel);
	//bool u = connect((QObject*)bridge::instance(), SIGNAL(slgnal_webejectpag(QString)), this, SLOT(slots_newpag(QString)));

	//channel = new QWebChannel(this);
	//channel->registerObject("bridge", (QObject*)bridge::instance());
	//m_webView.page()->setWebChannel(channel);
	//channel->registeredObjects();
	//bool l = connect((QObject*)bridge::instance(), SIGNAL(slgnal_webeject(QString)), this, SLOT(webeject(QString)));

	//m_webView.show();

	//m_pChannel->registerObject(QStringLiteral("qtWebExtend"), m_pWebchannelimpl);

	
	m_TeamChallengePage = new TeamChallengePage(this);//���´�������
	ui.gridLayout_5->addWidget(m_TeamChallengePage);
	connect(m_TeamChallengePage, SIGNAL(signal_close()), this, SLOT(slot_break_hall()));
	//m_TeamChallengePage->setWindowModality(Qt::ApplicationModal);
	//m_TeamChallengePage->show();

	m_SystemSettings = new SystemSettings(this);
	ui.gridLayout_6->addWidget(m_SystemSettings);
	connect(m_SystemSettings, SIGNAL(signal_close()), this, SLOT(slot_break_hall()));


	ui.centestid_six->hide();
	ui.centestid_five->hide();
	ui.centestid_four->hide();
	ui.centestid_three->hide();
	ui.centestid_two->hide();
	ui.centestid_one->hide();

	connect(this, SIGNAL(aaaa(int)), this, SLOT(slots_Id(int)));

	connect(this, SIGNAL(singnal_setthedeable()), this, SLOT(slots_HeroUnion())); 
	iscolseclent = true;


}
void eventPLclent::slots_newpag(QString str)
{
	webpopuppag *m_webpopuppag = new webpopuppag();
	m_webpopuppag->setwebPopup(str);
	m_webpopuppag->setModal(true);
	m_webpopuppag->show();
}
void eventPLclent::send_ServiceArea()
{
	QMASERVICAAREA::const_iterator iter;
	WJ::struct_data_hs_ass_gamelist_contest sololist;
	for (iter = CCommon::ServiceArea.begin(); iter != CCommon::ServiceArea.end(); iter++)
	{
		if (iter.value().ereaname == CCommon::Gamequfu)
		{
			sololist.ereaid = iter.value().ereaid;
			WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_gamelist, (UINT)WJ::e_hs_ass_gamelist_contest, &sololist, sizeof(sololist));
		}
	}
}
void eventPLclent::slot_Alreadyregistered()//��½�ɹ��ѱ���
{
		m_TeamChallengePage->links();
}
void eventPLclent::slot_LoadUserinformation()//�����û���Ϣ
{
	ui.label_accountname->setText(CCommon::userinfo.nickName);
	ui.pushButton_EntertainmentCurrency->setText(QString("%1").arg(CCommon::userinfo.i64Money) + "  ");
	ui.pushButton_integral->setText(QString("%1").arg(CCommon::userinfo.dwPoint) + "  ");
	
	WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_login, (UINT)WJ::e_hs_ass_login_c_finish, NULL, 0);
	CCommon::GameRoom.clear();
}
void eventPLclent::slot_regame()//����������Ϣ����
{ 
	QMAGAMECONT::const_iterator itergamecont;
	for (itergamecont = CCommon::gamecont.begin(); itergamecont != CCommon::gamecont.end(); itergamecont++)
	{
		if (itergamecont.value().uContestRule == QStringLiteral("������ս��"))
		{
			CCommon::presongamelist << itergamecont.value().centestid;
		}
		else 	if (itergamecont.value().uContestRule == QStringLiteral("�Ŷ���ս��"))
		{
			CCommon::teamgamelist << itergamecont.value().centestid;
		}
	}
	setuiIsempty();
	setuiInterfacecontent();
	m_ClientLink->hide();
}
void eventPLclent::initUI()//��UI�����ʼ��
{
	ui.pag_two->hide();
	ui.pag_three->hide();
	ui.pag_ten->hide();
	ui.pag_six->hide();
	ui.pag_seven->hide();
	ui.pag_nine->hide();
	ui.pag_four->hide();
	ui.pag_five->hide();
	ui.pag_eight->hide();

	ui.pushButton_AIEcho->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));//��ʼ��������ѡ��
	CCommon::Gamequfu = QStringLiteral("��ŷ����");

	ui.widget_PersonalChallenge->installEventFilter(this);
	ui.label_PersonalChallenge_2->hide();
	ui.label_PersonalChallenge->hide();
	ui.widget_TeamChallenge->installEventFilter(this);
	ui.label_TeamChallenge_2->hide();
	ui.label_TeamChallenge->hide();

	ui.widget_Entertainment->installEventFilter(this);
	ui.label_Entertainment_Sele->hide();
	ui.widget_integral->installEventFilter(this);
	ui.label_integral_sele->hide();
}
bool eventPLclent::eventFilter(QObject *obj, QEvent *e)
{
	if (obj == ui.widget_Entertainment)
	{
		if (e->type() == QEvent::Enter)
		{
			ui.label_Entertainment_Sele->show();
			ui.label_Entertainment->hide();
		}
		else if (e->type() == QEvent::Leave)
		{
			ui.label_Entertainment_Sele->hide();
			ui.label_Entertainment->show();
		}
	}
	if (obj == ui.widget_integral)
	{
		if (e->type() == QEvent::Enter)
		{
			ui.label_integral_sele->show();
			ui.label_integral->hide();
		}
		else if (e->type() == QEvent::Leave)
		{
			ui.label_integral_sele->hide();
			ui.label_integral->show();
		}
	}

	if (obj == ui.widget_PersonalChallenge)
	{
		if (e->type() == QEvent::Enter)
		{
			ui.label_PersonalChallenge_2->show();
		}
		else if (e->type() == QEvent::Leave)
		{
			ui.label_PersonalChallenge_2->hide();
		}
	}
	if (obj == ui.widget_TeamChallenge)
	{
		if (e->type() == QEvent::Enter)
		{
			ui.label_TeamChallenge_2->show();
		}
		else if (e->type() == QEvent::Leave)
		{
			ui.label_TeamChallenge_2->hide();
		}
	}
	//if (obj == ui.widget_two)
	//{
	//	if (e->type() == QEvent::Enter)
	//	{

	//		if (ui.centestid_two->text() == QStringLiteral("��ѳ�"))
	//		{
	//			ui.label_4->setStyleSheet(QStringLiteral("background-image: url(:/eventq/png/ȫ͸��.png);border-image: url(:/eventq/png/���¿�Ƭ/��ѳ�-��ͣ.png);"));
	//		}
	//		else if (ui.centestid_two->text() == QStringLiteral("���ֳ�"))
	//		{
	//			ui.label_4->setStyleSheet(QStringLiteral("background-image: url(:/eventq/png/ȫ͸��.png);border-image: url(:/eventq/png/���¿�Ƭ/���ֳ�-��ͣ.png);"));
	//		}
	//		else if (ui.centestid_two->text() == QStringLiteral("��ҳ�"))
	//		{
	//			ui.label_4->setStyleSheet(QStringLiteral("background-image: url(:/eventq/png/ȫ͸��.png);border-image: url(:/eventq/png/���¿�Ƭ/��ҳ�-��ͣ.png);"));
	//		}
	//	}
	//	else if (e->type() == QEvent::Leave)
	//	{
	//		if (ui.centestid_two->text() == QStringLiteral("��ѳ�"))
	//		{
	//			ui.label_4->setStyleSheet(QStringLiteral("background-image: url(:/eventq/png/ȫ͸��.png);border-image: url(:/eventq/png/���¿�Ƭ/��ѳ�-��ֹ.png);"));
	//		}
	//		else if (ui.centestid_two->text() == QStringLiteral("���ֳ�"))
	//		{
	//			ui.label_4->setStyleSheet(QStringLiteral("background-image: url(:/eventq/png/ȫ͸��.png);border-image: url(:/eventq/png/���¿�Ƭ/���ֳ�-��ֹ.png);"));
	//		}
	//		else if (ui.centestid_two->text() == QStringLiteral("��ҳ�"))
	//		{
	//			ui.label_4->setStyleSheet(QStringLiteral("background-image: url(:/eventq/png/ȫ͸��.png);border-image: url(:/eventq/png/���¿�Ƭ/��ҳ�-��ֹ.png);"));
	//		}
	//	}
	//}
	//if (obj == ui.lineEdit_setCode)
	//{
		//ui.lineEdit_setCode->setStyleSheet(QStringLiteral("background-image:url(:/eventq/png/�ŶӰ���-��.png)"));
		//if (ui.lineEdit_setCode->text() == QStringLiteral("�������ŶӰ���"))
		//{
		//	ui.lineEdit_setCode->setText(QStringLiteral(""));
		//}
	//}
	return true;
}
void eventPLclent::slots_pag()//��һҳ
{
	QPropertyAnimation *m_QPropertyAnimation = new QPropertyAnimation(ui.widget_4, "geometry");
	m_QPropertyAnimation->setDuration(90);//�ٶ�,ԽСԽ��
	m_QPropertyAnimation->setStartValue(QRect(851, 60, 661, 601));
	m_QPropertyAnimation->setEndValue(QRect(140, 60, 661, 601));
	m_QPropertyAnimation->start();
	
	//m_QPropertyAnimation->setEasingCurve(QEasingCurve::OutBounce);//outbounce��һ�����������
	//m_QPropertyAnimation->setEasingCurve(QEasingCurve::OutBack);//outback��������
	// this->animation->setEasingCurve(QEasingCurve::OutCirc);//outcirc��outback���
	//m_QPropertyAnimation->setEasingCurve(QEasingCurve::OutBounce);
}
void eventPLclent::slots_home()//��һҳ
{
	QPropertyAnimation *m_QPropertyAnimation = new QPropertyAnimation(ui.widget_4, "geometry");
	m_QPropertyAnimation->setDuration(90);//�ٶ�,ԽСԽ��
	m_QPropertyAnimation->setStartValue(QRect(-900, 60, 661, 601));
	m_QPropertyAnimation->setEndValue(QRect(140, 60, 661, 601));
	m_QPropertyAnimation->start();
	
	//m_QPropertyAnimation->setEasingCurve(QEasingCurve::OutBounce);//outbounce��һ�����������
	//m_QPropertyAnimation->setEasingCurve(QEasingCurve::OutBack);//outback��������
	// this->animation->setEasingCurve(QEasingCurve::OutCirc);//outcirc��outback���
	//m_QPropertyAnimation->setEasingCurve(QEasingCurve::OutBounce);
	

}
void eventPLclent::slots_Hearthstone()//¯ʯ��˵
{	
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
		
	m_webView.resize(0, 0);
	ui.stackedWidget->setCurrentIndex(0);
	//m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	//m_ClientLink->show();
	//CCommon::GameContname = QStringLiteral("������ս��");
	//CCommon::presongamelist.clear();
	//CCommon::teamgamelist.clear();

	//GAMETYPE game;
	//QMAPPGAMETYPE::const_iterator iter;
	//WJ::struct_data_hs_ass_gamelist_contest sololist;
	//for (iter = CCommon::mapGameType.begin(); iter != CCommon::mapGameType.end(); iter++)
	//{
	//	if (iter.value().gamename == QStringLiteral("Ӣ������"))
	//	{
	//		sololist.nameid = iter.value().gamenameid;
	//		WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_gamelist, (UINT)WJ::e_hs_ass_gamelist_contest, &sololist, sizeof(sololist));
	//	}
	//}
}
void eventPLclent::slots_HeroUnion()//Ӣ������
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
		
	m_webView.resize(0, 0);
	ui.stackedWidget->setCurrentIndex(0);
	m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	m_ClientLink->show();
	CCommon::GameContname = QStringLiteral("������ս��");
	CCommon::presongamelist.clear();
	CCommon::teamgamelist.clear();

	ui.label_PersonalChallenge->show();
	ui.label_TeamChallenge->hide();


	GAMETYPE game;
	QMASERVICAAREA::const_iterator iter;
	WJ::struct_data_hs_ass_gamelist_contest sololist;
	for (iter = CCommon::ServiceArea.begin(); iter != CCommon::ServiceArea.end(); iter++)
	{
		if (iter.value().ereaname == CCommon::Gamequfu)
		{
			sololist.ereaid = iter.value().ereaid;
			WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_gamelist, (UINT)WJ::e_hs_ass_gamelist_contest, &sololist, sizeof(sololist));
		}
	}
}
void eventPLclent::slots_personalsolo()//����solo��
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return; 
	}

	m_webView.resize(0, 0);

	CCommon::GameContname = QStringLiteral("������ս��");

	CCommon::presongamelist.clear();
	CCommon::teamgamelist.clear();

	QMASERVICAAREA::const_iterator iter;
	WJ::struct_data_hs_ass_gamelist_contest sololist;
	for (iter = CCommon::ServiceArea.begin(); iter != CCommon::ServiceArea.end(); iter++)
	{
		if (iter.value().ereaname == QStringLiteral("��ŷ����"))
		{
			sololist.ereaid = iter.value().ereaid;
			WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_gamelist, (UINT)WJ::e_hs_ass_gamelist_contest, &sololist, sizeof(sololist));
		}
	}

	int istack = ui.stackedWidget->currentIndex();
	if (istack != 0)
	{
		ui.stackedWidget->setCurrentIndex(0);
	}
	ui.label_PersonalChallenge->show();
	ui.label_TeamChallenge->hide();

	ui.stackedWidget->setCurrentIndex(0);
	m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	m_ClientLink->show();

}
void eventPLclent::slots_teamchange()//�Ŷ���ս��
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}

	m_webView.resize(0, 0);
	ui.stackedWidget->setCurrentIndex(0);
	m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	m_ClientLink->show();

	CCommon::GameContname = QStringLiteral("�Ŷ���ս��");
	CCommon::presongamelist.clear();
	CCommon::teamgamelist.clear();

	QMASERVICAAREA::const_iterator iter;
	WJ::struct_data_hs_ass_gamelist_contest sololist;
	for (iter = CCommon::ServiceArea.begin(); iter != CCommon::ServiceArea.end(); iter++)
	{
		if (iter.value().ereaname == QStringLiteral("��ŷ����"))
		{
			sololist.ereaid = iter.value().ereaid;
			WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_gamelist, (UINT)WJ::e_hs_ass_gamelist_contest, &sololist, sizeof(sololist));
		}
	}
	ui.stackedWidget->setCurrentIndex(0);
	ui.label_PersonalChallenge->hide();
	ui.label_TeamChallenge->show();

}
void eventPLclent::setuiInterfacecontent()
{
	GAMECONT gamecont;
	QMAGAMECONT::const_iterator iter;
	for (iter = CCommon::gamecont.begin(); iter != CCommon::gamecont.end(); iter++)
	{
		if (CCommon::gamecont.size() > 0 && CCommon::GameContname == QStringLiteral("������ս��"))
			if (iter.value().centestid == CCommon::presongamelist.at(0))
			{
				ui.centestid_one->setText(QString("%1").arg(iter.value().centestid));
				ui.label_one1->setText(iter.value().centestname);//��������
				ui.label_one->setText(iter.value().uContestKind);//����ģʽ
				ui.label_one2->setText(iter.value().cContestDescribe);//��������
				ui.label_one3_2->setText(QString("%1").arg(iter.value()._i64award));//��������
				ui.pushButton_one->setText(QString("%1").arg(iter.value()._i64cost));//֧����ʽ
			}
		if (CCommon::gamecont.size() > 1 && CCommon::GameContname == QStringLiteral("������ս��"))
			if (iter.value().centestid == CCommon::presongamelist.at(1))
			{
				ui.centestid_two->setText(QString("%1").arg(iter.value().centestid));
				ui.label_two_2->setText(iter.value().centestname);//��������
				ui.label_two->setText(iter.value().uContestKind);//����ģʽ
				ui.label_two_3->setText(iter.value().cContestDescribe);//��������
				ui.label_one3_5->setText(QString("%1").arg(iter.value()._i64award));//��������
				ui.pushButton_two->setText(QString("%1").arg(iter.value()._i64cost));//֧����ʽ
			}
		if (CCommon::gamecont.size() > 2 && CCommon::GameContname == QStringLiteral("������ս��"))
			if (iter.value().centestid == CCommon::presongamelist.at(2))
			{
				ui.centestid_three->setText(QString("%1").arg(iter.value().centestid));
				ui.label_two_5->setText(iter.value().centestname);//��������
				ui.label_two_6->setText(iter.value().uContestKind);//����ģʽ
				ui.label_two_4->setText(iter.value().cContestDescribe);//��������
				ui.label_one3_11->setText(QString("%1").arg(iter.value()._i64award));//��������
				ui.pushButton_three->setText(QString("%1").arg(iter.value()._i64cost));//֧����ʽ
			}
		if (CCommon::gamecont.size() > 3 && CCommon::GameContname == QStringLiteral("������ս��"))
			if (iter.value().centestid == CCommon::presongamelist.at(3))
			{
				ui.centestid_four->setText(QString("%1").arg(iter.value().centestid));
				ui.label_two_8->setText(iter.value().centestname);//��������
				ui.label_two_9->setText(iter.value().uContestKind);//����ģʽ
				ui.label_two_7->setText(iter.value().cContestDescribe);//��������
				ui.label_one3_14->setText(QString("%1").arg(iter.value()._i64award));//��������
				ui.pushButton_four->setText(QString("%1").arg(iter.value()._i64cost));//֧����ʽ
			}
		if (CCommon::gamecont.size() > 4 && CCommon::GameContname == QStringLiteral("������ս��"))
			if (iter.value().centestid == CCommon::presongamelist.at(4))
			{
				ui.centestid_five->setText(QString("%1").arg(iter.value().centestid));
				ui.label_two_11->setText(iter.value().centestname);//��������
				ui.label_two_12->setText(iter.value().uContestKind);//����ģʽ
				ui.label_two_10->setText(iter.value().cContestDescribe);//��������
				ui.label_one3_17->setText(QString("%1").arg(iter.value()._i64award));//��������
				ui.pushButton_five->setText(QString("%1").arg(iter.value()._i64cost));//֧����ʽ
			}
		if (CCommon::gamecont.size() > 5 && CCommon::GameContname == QStringLiteral("������ս��"))
			if (iter.value().centestid == CCommon::presongamelist.at(5))
			{
				ui.centestid_six->setText(QString("%1").arg(iter.value().centestid));
				ui.label_two_14->setText(iter.value().centestname);//��������
				ui.label_two_15->setText(iter.value().uContestKind);//����ģʽ
				ui.label_two_13->setText(iter.value().cContestDescribe);//��������
				ui.label_one3_20->setText(QString("%1").arg(iter.value()._i64award));//��������
				ui.pushButton_six->setText(QString("%1").arg(iter.value()._i64cost));//֧����ʽ
			}
		if (CCommon::teamgamelist.size() > 0 && CCommon::GameContname == QStringLiteral("�Ŷ���ս��"))
			if (iter.value().centestid == CCommon::teamgamelist.at(0))
			{
				ui.centestid_one->setText(QString("%1").arg(iter.value().centestid));
				ui.label_one1->setText(iter.value().centestname);//��������
				ui.label_one->setText(iter.value().uContestKind);//����ģʽ
				ui.label_one2->setText(iter.value().cContestDescribe);//��������
				ui.label_one3_2->setText(QString("%1").arg(iter.value()._i64award));//��������
				ui.pushButton_one->setText(QString("%1").arg(iter.value()._i64cost));//֧����ʽ
			}
		if (CCommon::teamgamelist.size() > 1 && CCommon::GameContname == QStringLiteral("�Ŷ���ս��"))
			if (iter.value().centestid == CCommon::teamgamelist.at(1))
			{
				ui.centestid_two->setText(QString("%1").arg(iter.value().centestid));
				ui.label_two_2->setText(iter.value().centestname);//��������
				ui.label_two->setText(iter.value().uContestKind);//����ģʽ
				ui.label_two_3->setText(iter.value().cContestDescribe);//��������
				ui.label_one3_5->setText(QString("%1").arg(iter.value()._i64award));//��������
				ui.pushButton_two->setText(QString("%1").arg(iter.value()._i64cost));//֧����ʽ
			}
		if (CCommon::teamgamelist.size() > 2 && CCommon::GameContname == QStringLiteral("�Ŷ���ս��"))
			if (iter.value().centestid == CCommon::teamgamelist.at(2))
			{
				ui.centestid_three->setText(QString("%1").arg(iter.value().centestid));
				ui.label_two_5->setText(iter.value().centestname);//��������
				ui.label_two_6->setText(iter.value().uContestKind);//����ģʽ
				ui.label_two_4->setText(iter.value().cContestDescribe);//��������
				ui.label_one3_11->setText(QString("%1").arg(iter.value()._i64award));//��������
				ui.pushButton_three->setText(QString("%1").arg(iter.value()._i64cost));//֧����ʽ
			}
		if (CCommon::teamgamelist.size() > 3 && CCommon::GameContname == QStringLiteral("�Ŷ���ս��"))
			if (iter.value().centestid == CCommon::teamgamelist.at(3))
			{
				ui.centestid_four->setText(QString("%1").arg(iter.value().centestid));
				ui.label_two_8->setText(iter.value().centestname);//��������
				ui.label_two_9->setText(iter.value().uContestKind);//����ģʽ
				ui.label_two_7->setText(iter.value().cContestDescribe);//��������
				ui.label_one3_14->setText(QString("%1").arg(iter.value()._i64award));//��������
				ui.pushButton_four->setText(QString("%1").arg(iter.value()._i64cost));//֧����ʽ
			}
		if (CCommon::teamgamelist.size() > 4 && CCommon::GameContname == QStringLiteral("�Ŷ���ս��"))
			if (iter.value().centestid == CCommon::teamgamelist.at(4))
			{
				ui.centestid_five->setText(QString("%1").arg(iter.value().centestid));
				ui.label_two_11->setText(iter.value().centestname);//��������
				ui.label_two_12->setText(iter.value().uContestKind);//����ģʽ
				ui.label_two_10->setText(iter.value().cContestDescribe);//��������
				ui.label_one3_17->setText(QString("%1").arg(iter.value()._i64award));//��������
				ui.pushButton_five->setText(QString("%1").arg(iter.value()._i64cost));//֧����ʽ
			}
		if (CCommon::teamgamelist.size() > 5 && CCommon::GameContname == QStringLiteral("�Ŷ���ս��"))
			if (iter.value().centestid == CCommon::teamgamelist.at(5))
			{
				ui.centestid_six->setText(QString("%1").arg(iter.value().centestid));
				ui.label_two_14->setText(iter.value().centestname);//��������
				ui.label_two_15->setText(iter.value().uContestKind);//����ģʽ
				ui.label_two_13->setText(iter.value().cContestDescribe);//��������
				ui.label_one3_20->setText(QString("%1").arg(iter.value()._i64award));//��������
				ui.pushButton_six->setText(QString("%1").arg(iter.value()._i64cost));//֧����ʽ
			}
	}
}
void eventPLclent::setuiIsempty()
{
	ui.label_one1->setText(QStringLiteral(""));//��������//////   1
	ui.label_one->setText(QStringLiteral(""));//����ģʽ
	ui.label_one2->setText(QStringLiteral(""));//��������
	ui.label_one3_2->setText(QStringLiteral(""));//��������
	ui.pushButton_one->setText(QStringLiteral(""));//֧����ʽ

	ui.label_two_2->setText(QStringLiteral(""));//��������//////////    2
	ui.label_two->setText(QStringLiteral(""));//����ģʽ
	ui.label_two_3->setText(QStringLiteral(""));//��������
	ui.label_one3_5->setText(QStringLiteral(""));//��������
	ui.pushButton_two->setText(QStringLiteral(""));//֧����ʽ

	ui.label_two_5->setText(QStringLiteral(""));//��������///////////   3
	ui.label_two_6->setText(QStringLiteral(""));//����ģʽ
	ui.label_two_4->setText(QStringLiteral(""));//��������
	ui.label_one3_11->setText(QStringLiteral(""));//��������
	ui.pushButton_three->setText(QStringLiteral(""));//֧����ʽ

	ui.label_two_8->setText(QStringLiteral(""));//��������////////////      4
	ui.label_two_9->setText(QStringLiteral(""));//����ģʽ
	ui.label_two_7->setText(QStringLiteral(""));//��������
	ui.label_one3_14->setText(QStringLiteral(""));//��������
	ui.pushButton_four->setText(QStringLiteral(""));//֧����ʽ

	ui.label_two_11->setText(QStringLiteral(""));//��������          5
	ui.label_two_12->setText(QStringLiteral(""));//����ģʽ
	ui.label_two_10->setText(QStringLiteral(""));//��������
	ui.label_one3_17->setText(QStringLiteral(""));//��������
	ui.pushButton_five->setText(QStringLiteral(""));//֧����ʽ


	ui.label_two_14->setText(QStringLiteral(""));//��������			6
	ui.label_two_15->setText(QStringLiteral(""));//����ģʽ
	ui.label_two_13->setText(QStringLiteral(""));//��������
	ui.label_one3_20->setText(QStringLiteral(""));//��������
	ui.pushButton_six->setText(QStringLiteral(""));//֧����ʽ

}
void eventPLclent::slots_hall()//����
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}

	m_webView.resize(0, 0);
	ui.stackedWidget->setCurrentIndex(0);
}
void eventPLclent::slots_Recharge()//��ֵ
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	QDesktopServices::openUrl(QUrl(WEB_PAGE + QStringLiteral("recharge/page")));
}
void eventPLclent::slots_Platformmall()//ƽ̨�̳�
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	m_ClientLink->startnewTimer();
	m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	m_ClientLink->show();

	ui.stackedWidget->setCurrentIndex(1);
	m_webView.setParent(this);
	m_webView.move(163, 70);
	m_webView.resize(10000, 10000);

	m_webView.load(QUrl("http://192.168.1.109:8080/mall/home?page=1&sourceType=1&userId=17"));
	//m_webView.load(QUrl("http://121.199.28.123:8082/mall/home?page=1&sourceType=1&userId=" + QString("%1").arg(CCommon::userinfo.dwUserID)));

	m_webView.page()->setBackgroundColor(QColor(0, 0, 0, 0));
	m_webView.show();

	
}
void eventPLclent::slots_Entryrecord()//������¼
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	m_ClientLink->startnewTimer();
	m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	m_ClientLink->show();
	webPopup *m_webPopup = new webPopup();
	m_webPopup->setwebPopup(WEB_INTERFACE + QStringLiteral("fight/record?userId=") + QString("%1").arg(CCommon::userinfo.dwUserID));
	m_webPopup->setAttribute(Qt::WA_DeleteOnClose);
	m_webPopup->setModal(true);
	m_webPopup->exec();
}
void eventPLclent::slots_Officialnews()//�ٷ���Ϣ
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	m_ClientLink->startnewTimer();
	m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	m_ClientLink->show();
	webPopup* m_webPopup = new webPopup();
	m_webPopup->setwebPopup(WEB_INTERFACE + QStringLiteral("offical/msg?userId=") + QString("%1").arg(CCommon::userinfo.dwUserID));
	m_webPopup->setAttribute(Qt::WA_DeleteOnClose);
	m_webPopup->setModal(true);
	m_webPopup->exec();
}
void eventPLclent::slots_Invitefriends()//�������
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	m_ClientLink->startnewTimer();
	m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	m_ClientLink->show();
	ui.stackedWidget->setCurrentIndex(1);
	m_webView.setParent(this);
	m_webView.move(163, 70);
	m_webView.resize(10000, 10000);
	m_webView.page()->setBackgroundColor(QColor(0, 0, 0, 0));
	QString str = WEB_INTERFACE + QStringLiteral("invit/page?userId=") + QString("%1").arg(CCommon::userinfo.dwUserID);
	m_webView.load(QUrl(str));
	m_webView.show();

}
void eventPLclent::slots_Youmentionmechange()//�����Ҹ�
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	m_ClientLink->startnewTimer();
	m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	m_ClientLink->show();
	webPopup* m_webPopup = new webPopup();
	m_webPopup->setwebPopup(WEB_INTERFACE + QStringLiteral("advise/page?userId=") + QString("%1").arg(CCommon::userinfo.dwUserID));
	m_webPopup->setAttribute(Qt::WA_DeleteOnClose);
	m_webPopup->setModal(true);
	m_webPopup->exec();

}
void eventPLclent::slots_EntertainmentCurrency()//���
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	m_ClientLink->startnewTimer();
	m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	m_ClientLink->show();
	webPopup* m_webPopup = new webPopup();
	m_webPopup->setwebPopup(WEB_INTERFACE + QStringLiteral("detail/chip?userId=") + QString("%1").arg(CCommon::userinfo.dwUserID));
	m_webPopup->setAttribute(Qt::WA_DeleteOnClose);
	m_webPopup->setModal(true);
	m_webPopup->exec();
}
void eventPLclent::slots_integral()//����
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	webPopup* m_webPopup = new webPopup();
	m_webPopup->setwebPopup(WEB_INTERFACE + QStringLiteral("detail/coin?userId=") + QString("%1").arg(CCommon::userinfo.dwUserID));
	m_webPopup->setAttribute(Qt::WA_DeleteOnClose);
	m_webPopup->setModal(true);
		
	m_ClientLink->startnewTimer();
	m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	m_ClientLink->show();

	m_webPopup->exec();
}
void eventPLclent::slots_Ladderranking()//��������
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	m_ClientLink->startnewTimer();
	m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	m_ClientLink->show();
	ui.stackedWidget->setCurrentIndex(1);
	m_webView.setParent(this);
	m_webView.move(163, 70);
	m_webView.resize(10000, 10000);
	m_webView.page()->setBackgroundColor(QColor(0, 0, 0, 0));
	m_webView.load(QUrl(WEB_INTERFACE + QStringLiteral("seniority")));
	m_webView.show();
}
void eventPLclent::slots_Setup()//����
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}

	m_webView.resize(0, 0);
	ui.stackedWidget->setCurrentIndex(3);
}
void eventPLclent::slots_Helpcenter()//��������
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	m_webView.resize(0, 0);
	ui.stackedWidget->setCurrentIndex(1);
	m_webView.setParent(this);
	m_webView.move(163, 70);
	m_webView.resize(10000, 10000);
	m_webView.page()->setBackgroundColor(QColor(0, 0, 0, 0));
	m_webView.load(QUrl(WEB_INTERFACE + QStringLiteral("help/helpCenter")));
	m_ClientLink->startnewTimer();
	m_ClientLink->settextname(QStringLiteral("���ڼ������Ժ󡣡�"));
	m_ClientLink->show();
	m_webView.show();
}
//���°�ť
void eventPLclent::slots_pushButton_one()//��һ������
{
	if (ui.pushButton_one->text() == QStringLiteral(""))
		return;	
	ui.stackedWidget->setCurrentIndex(2);
	m_TeamChallengePage->setui(ui.centestid_one->text());
}
void eventPLclent::slots_pushButton_two()//�ڶ�������
{
	if (ui.pushButton_two->text() == QStringLiteral(""))
		return;
	ui.stackedWidget->setCurrentIndex(2);
	m_TeamChallengePage->setui(ui.centestid_two->text());
}
void eventPLclent::slots_pushButton_three()//����������
{
	if (ui.pushButton_three->text() == QStringLiteral(""))
		return;
	ui.stackedWidget->setCurrentIndex(2);
	m_TeamChallengePage->setui(ui.centestid_three->text());
}
void eventPLclent::slots_pushButton_four()//���ĳ�����
{
	if (ui.pushButton_four->text() == QStringLiteral(""))
		return;
	ui.stackedWidget->setCurrentIndex(2);
	m_TeamChallengePage->setui(ui.centestid_four->text());
}
void eventPLclent::slots_pushButton_five()//���峡����
{
	if (ui.pushButton_five->text() == QStringLiteral(""))
		return;
	ui.stackedWidget->setCurrentIndex(2);
	m_TeamChallengePage->setui(ui.centestid_five->text());
}
void eventPLclent::slots_pushButton_six()//����������
{
	if (ui.pushButton_six->text() == QStringLiteral(""))
		return;
	ui.stackedWidget->setCurrentIndex(2);
	m_TeamChallengePage->setui(ui.centestid_six->text());
}
void eventPLclent::slot_break_hall()//�������鷵�ش���
{
	if (ui.pushButton_one->text() == QStringLiteral(""))
		return;
		
	ui.stackedWidget->setCurrentIndex(0);
}
void eventPLclent::slots_close()//�ر�
{
	this->close();
	WJ::PlatformLogic().close();
}
void eventPLclent::setputBUttonqufu()
{
	send_ServiceArea();
	ui.pushButton_AIEcho->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_juan->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_Noxus->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_Bandle->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_APyrgi->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_WarCollege->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_GiantGodPeak->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_ReetheGarrison->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_LandOfJudgment->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_BlackRose->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_ShadowLsles->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_SteelHeat->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_BalancedDenomination->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_CrystalSScar->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_ShadowFlow->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_WatchOfTheSea->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_ConquestOfTheSea->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_Kalamunda->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_CityGuard->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_BaoshBirgi->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_Demacia->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_FerrerJuud->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_FearlessVanguard->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_SteelHeat->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_TwistedTreeline->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_TheWrathofRama->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_Dragon->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
	ui.pushButton_Education->setStyleSheet(QStringLiteral("color: rgb(150, 150, 150);"));
}
void eventPLclent::slot_pushButton_AIEcho()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("��ŷ����");
	setputBUttonqufu();
	ui.pushButton_AIEcho->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_juan()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("�氲");
	setputBUttonqufu();
	ui.pushButton_juan->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_Noxus()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("ŵ����˹");
	setputBUttonqufu();
	ui.pushButton_Noxus->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_Bandle()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("��¶���");
	setputBUttonqufu();
	ui.pushButton_Bandle->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_APyrgi()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("Ƥ��������");
	setputBUttonqufu();
	ui.pushButton_APyrgi->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_WarCollege()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("ս��ѧԺ");
	setputBUttonqufu();
	ui.pushButton_WarCollege->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_GiantGodPeak()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("�����");
	setputBUttonqufu();
	ui.pushButton_GiantGodPeak->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_ReetheGarrison()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("��ɪ�ر�");
	setputBUttonqufu();
	ui.pushButton_ReetheGarrison->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_LandOfJudgment()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("�þ�֮��");
	setputBUttonqufu();
	ui.pushButton_LandOfJudgment->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_BlackRose()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("��ɫõ��");
	setputBUttonqufu();
	ui.pushButton_BlackRose->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}


void eventPLclent::slot_pushButton_ShadowLsles()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("��Ӱ��");
	setputBUttonqufu();
	ui.pushButton_ShadowLsles->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_SteelHeat()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("��������");
	setputBUttonqufu();
	ui.pushButton_SteelHeat->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_BalancedDenomination()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("�������");
	setputBUttonqufu();
	ui.pushButton_BalancedDenomination->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_CrystalSScar()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("ˮ��֮��");
	setputBUttonqufu();
	ui.pushButton_CrystalSScar->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_ShadowFlow()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("Ӱ��");
	setputBUttonqufu();
	ui.pushButton_ShadowFlow->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_WatchOfTheSea()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("����֮��");
	setputBUttonqufu();
	ui.pushButton_WatchOfTheSea->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_ConquestOfTheSea()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("����֮��");
	setputBUttonqufu();
	ui.pushButton_ConquestOfTheSea->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_Kalamunda()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("��������");
	setputBUttonqufu();
	ui.pushButton_Kalamunda->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_CityGuard()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("Ƥ�Ǿ���");
	setputBUttonqufu();
	ui.pushButton_CityGuard->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_BaoshBirgi()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("�ȶ�������");
	setputBUttonqufu();
	ui.pushButton_BaoshBirgi->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_Demacia()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("��������");
	setputBUttonqufu();
	ui.pushButton_Demacia->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_FerrerJuud()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("���׶�׿��");
	setputBUttonqufu();
	ui.pushButton_FerrerJuud->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_FearlessVanguard()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("��η�ȷ�");
	setputBUttonqufu();
	ui.pushButton_FearlessVanguard->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_TwistedTreeline()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("Ť������");
	setputBUttonqufu();
	ui.pushButton_TwistedTreeline->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_TheWrathofRama()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("ŭ����");
	setputBUttonqufu();
	ui.pushButton_TheWrathofRama->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_Dragon()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("����֮��");
	setputBUttonqufu();
	ui.pushButton_Dragon->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
void eventPLclent::slot_pushButton_Education()
{
	if (!WJ::PlatformLogic().isConnect())
	{
		m_ClientLink->exec();
		return;
	}
	CCommon::Gamequfu = QStringLiteral("������ר��");
	setputBUttonqufu();
	ui.pushButton_Education->setStyleSheet(QStringLiteral("color: rgb(94, 158, 255);"));
}
eventPLclent::~eventPLclent()
{
	if (m_linkCtoQ)
	{
		delete m_linkCtoQ;
	}
	if (m_ClientLink)
	{
		delete m_ClientLink;
	}
	if (m_LoginInter)
	{
		delete m_LoginInter;
	}
	if (m_TeamChallengePage)
	{
		delete m_TeamChallengePage;
	}
	if (m_SystemSettings)
	{
		delete m_SystemSettings;
	}
	this->close();
	WJ::PlatformLogic().close();
}
