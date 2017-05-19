#ifndef MODEL_H
#define MODEL_H

#define STR_LEN_64	64//�ַ�����
#define PLAYER_COUNT	10//��Ϸ����

#include <QMap>
#include <QList>
#include <vector>
#include <set>
using namespace std;


typedef struct UserData//�û�����
{
	QString szName;//��¼��
	QString	nickName;//�û��ǳ�
	QString	password;//����
	int dwUserID;//ID
	int dwPoint;//����
	long i64Money;//���
	QString	bUserState;//�û�״̬
	QString	landmassage = "";//��½��Ϣ
	QString icon;//ͷ��
	QString rewardTotal;//�û��ܵĽ���
}USERDATA, *PUSERDATA;
typedef std::vector<USERDATA> QMAPUSERDATA;

//Ѻע����
typedef struct _BetInfo {

	//��ʾλ��
	int				_sinIndex;
	//��ϷID
	int				_sinGameId;
	//���
	int				_sinMoney;
	//����
	float			_sfRate;

	bool operator < (const _BetInfo& Value) const {

		return Value._sinIndex < _sinIndex;
	}

	_BetInfo() {

		_sinIndex = 0;
		_sinGameId = 0;
		_sinMoney = 0;
		_sfRate = 0;
	}

} BetInfo, *pBetInfo;
typedef set<BetInfo> TVBETINFO;

typedef struct LOLautomation//lol�Զ���
{
	char roomname[128];		/*��������*/
	char password[50];		/*��������*/
	int mode;				/*��Ϸģʽ*/
	int map;				/*��ͼ*/
	int playernum;			/*��������*/
	int observe;			/*��սģʽ*/
	int gametype;			/*��Ϸ����*/
	bool bCreate;			//��������������
	int ServiceArea;		//����ID
	int side;				//�����Ӫ
	LOLautomation()
	{
		std::memset(this, 0, sizeof(LOLautomation));
	}
}LOLAUTOMATION, *PLOLAUTOMATION;



typedef struct GameInformation//��Ϸ��Ϣ
{
	int GameState;
}GAMEINFORMATION, *PGAMEINFORMATION;



typedef struct HallData//��������
{
	int id;//����ID
	int sort_no;//` int(10) DEFAULT NULL COMMENT '��������',
	QString name;//` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '��������',
	int manage_id;//` bigint(20) DEFAULT NULL COMMENT '��������ģ��',
	int game_id;//` bigint(20) DEFAULT NULL COMMENT '��ϷID',
	int server_id;//` bigint(20) DEFAULT NULL COMMENT '����ID',
	int type;//` tinyint(4) DEFAULT NULL COMMENT '������� 0 ��� 1 ���� 2 ��� ',
	int point_type;//` tinyint(4) DEFAULT NULL COMMENT '������ʽ \r\n����SOLO\r\n�����Ƶ��з�һ���ĸ��˻�ʤ\r\n�����ȴﵽ100���ĸ��˻�ʤ\r\n10�������ڻ�ý�Ǯ���ĸ��˻�ʤ\r\n\r\n���\r\n���Ȼ�õ�һ�������Ķ����ʤ\r\n�����Ƶ��Է����صĶ����ʤ\r\n�����Ƶ��з�4���������Ķ����ʤ\r\n�����л�ɱӢ���������ȴﵽ15���Ķ����ʤ',
	QString pointName;
	int theme;//` tinyint(4) DEFAULT NULL COMMENT '��������   1 ������ 2���׳� 3 �߼��� 4 ��ʦ��',
	QString icon;//` varchar(128) COLLATE utf8_unicode_ci DEFAULT NULL,
	int criteria;//` int(11) DEFAULT NULL COMMENT '�������� 0 ��� ����ֵ��Ӧ���Ļ��ֻ���ҽ��',
	int mode;//` tinyint(4) DEFAULT NULL COMMENT '����ģʽ 1 ����solo 2 �Ŷ�',
	int begin_time;//` datetime DEFAULT NULL COMMENT '������ʼʱ��',
	int end_time;//` datetime DEFAULT NULL COMMENT '��������ʱ��',
	QString specialty;//` varchar(256) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '�����ص� �磺soloһѪ',
	int max_number;//` int(11) DEFAULT NULL COMMENT '����������',
	int team_number;//` tinyint(4) DEFAULT NULL COMMENT '�Ŷ�����',
	QString prize;//` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '��߽���',
	QString comment;//` varchar(512) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '����˵��',
	QString prize_info;//` varchar(512) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '��������',
	QString mode_info;//` varchar(512) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '����˵��',
	int is_valid;//` tinyint(4) DEFAULT NULL,
	int exec_user;//` bigint(20) DEFAULT NULL COMMENT '����������',
	QString create_date;//` datetime DEFAULT NULL COMMENT '¼������',
	QString update_date;//` datetime DEFAULT NULL COMMENT '�����޸�����',
	QString open_status;//` char(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT ' ����״̬:0-�༭,1-������,2-�ѽ���',
	int profit;//` int(8) DEFAULT NULL COMMENT '��������',
	int profitType;
	//QString prize;
}HALLDATA, *PHALLDATA;
typedef QMap<int, HALLDATA> QMAHALLDATA;


#endif

