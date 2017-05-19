#ifndef _MONITOR_STATUS_HS_H_
#define _MONITOR_STATUS_HS_H_



enum EnGameStatusHS
{
	E_STATUS_HS_NOTSTART = 1,
	E_STATUS_HS_HOOKED,
	E_STATUS_HS_D3D_ERROR,
	//E_STATUS_HS_ADDFRIEND_REQ,		// �ѷ�����Ӻ�������
	//E_STATUS_HS_ADDFRIEND_ACCEPT,	// �ѽ�����Ӻ�������
	//E_STATUS_HS_CHALLENGE_REQ,		// �ѷ�����ս����
	//E_STATUS_HS_CHALLENGE_ACCEPT,	// �ѽ�����ս����

	//E_STATUS_HS_READY_TO_PLAY,		// ��Ϸ׼����

	E_STATUS_HS_CHOOSEING,			// ѡ����Ϸ��ɫ
	//E_STATUS_HS_CANCEL_PLAY,		// �û�ȡ��
	E_STATUS_HS_PLAYING,			// ���ڽ�����Ϸ

	E_STATUS_HS_GAME_VICTORY,		// ��Ϸ������ʤ��
	E_STATUS_HS_GAME_FAIL,			// ��Ϸ������ʧ��

	E_STATUS_HS_AM_SUCCESS,			// ִ���Զ����ɹ�
	E_STATUS_HS_AM_FAIL,			// ִ���Զ���ʧ��

	E_STATUS_HS_GAME_OFFLINE		// ע��DLL�쳣
};



#endif//_MONITOR_STATUS_HS_H_