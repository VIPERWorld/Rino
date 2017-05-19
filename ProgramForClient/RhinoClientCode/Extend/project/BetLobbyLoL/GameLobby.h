#pragma once

#ifndef GAMELOBBY_H
#define GAMELOBBY_H

//��Ϸ״̬
typedef enum _State {

	//δ֪
	unknown,
	//����ѡ��ɫ
	inselect,
	//��ѡ��ɫ����
	breakslect

} State;

class CGameLobby {

private:
	//HOOK��ַ
	DWORD																												m_HookAddress;
	//HOOK��ԭʼ������
	byte																												m_HookOpCode[7];
protected:
	//�������ݵ�ƽ̨
	static void																											SendGameData(State pState, int inMode = 0);
	//�����Ƿ����˻�ģʽ
	static bool																											FindMode(int inStart, int inEnd, char* pchData, const char* pchTemp);
public:
	//hook��ת��ַ
	static DWORD																										m_JmpGameLobby;
public:
	CGameLobby();
	~CGameLobby();

	//������ϷFlash����
	static void	__stdcall																								FilterGameLobby(char* pchBuffer, int inLen);
	//HOOK Flash����
	void																												HookGameLobby();
	//UnHook Flash����
	void																												UnHookGameLobby();
};

#endif