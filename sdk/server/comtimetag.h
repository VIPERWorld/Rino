#ifndef WJ_COMTIMETAG_H
#define WJ_COMTIMETAG_H

#define TIME_SPACE						50L								///��Ϸ�ڲ���ʱ����󲻵ó�����ֵ
#define TIME_START_ID					100L							///��Ϸ��ʱ����ʼ ID
///////��½������///////////////////////////////////////////////////////////////////////////////


enum E_P_TIMER_ID
{
	E_P_TIME_ID_TEAM_VS=1,//Ϊ�Ŷӽ���ƥ��
	E_P_TIME_ID_UPDATE_ONLINE_COUNT,//������������
	E_P_TIME_ID_HEART,//�������
	E_P_TIME_ID_UPDATE_USER_INFO,//ѯ��redisˢ���û�����
};

///////��Ϸ������///////////////////////////////////////////////////////////////////////////////

enum E_R_TIMER_ID
{
	E_R_TIME_ID_ReconnectLoginServe = 1,
	E_R_TIME_ID_LoginServerKeepalive,
	E_R_TIME_ID_RoomQueue, // �Ŷӻ���ʱ��id
	E_R_TIME_ID_HEART,//�������
};

/*
��Ϸ�������ϵļ�ʱ��ID�����ʽ����
��λ                         ��λ
|--roomid--|--deskid--|-��Ϸϵͳ��ʱ��id-|-��Ϸ��ʱ��id-|
���У�
  roomid			12λ
  deskid			12λ
  ��Ϸϵͳ��ʱ��id  4λ
  ��Ϸ��ʱ��id		4λ
*/

#define TIMER_ROOMID_MASK   0xfff00000         //����ID����
#define TIMER_DESKID_MASK   0x000fff00         //����ID����
#define TIMER_GAME_TIMERID_MASK_SYS     0x000000f0         //��Ϸϵͳ��ʱ��ID����
#define TIMER_GAME_TIMERID_MASK			0x0000000f         //��Ϸ��ʱ��ID����



#endif//WJ_COMTIMETAG_H