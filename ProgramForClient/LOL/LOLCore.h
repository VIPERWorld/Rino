#pragma once
//����
#define AiOuNiYa			0x101		//��ŷ����
#define ZhuAng				0x201		//�氲
#define NuoKeShaShi			0x301		//ŵ����˹
#define BanDeErCheng		0x401		//��¶���
#define PiErTeWoFu			0x501		//Ƥ�����ַ�
#define ZhangZhengXueYuan	0x601		//ս��ѧԺ
#define JuShengFeng			0x701		//�����
#define LeiSheShouBei		0x801		//��ɪ�ر�
#define GangTieLieYang		0xC01		//��������
#define ChaiJueZhiDi		0x901		//�þ�֮��
#define HeiSheMeiZhui		0xA01		//��ɫõ��
#define AngYingDao			0xB01		//��Ӱ��
#define JunHengJiaoPai		0xE01		//�������
#define ShuiJinZhiHeng		0xD01		//ˮ��֮��
#define YinLiu				0xF01		//Ӱ��
#define ShouWanZhiHai		0x1001		//����֮��
#define ZhengFuZhiHai		0x1101		//����֮��
#define KaLaMangDa			0x1201		//��������
#define PiChengJinBei		0x1301		//Ƥ�Ǿ���
#define Nanjuelingyu		0x105		//�о�����
//��ͨ
#define BiErJiWoTe			0x102		//�ȶ�������
#define DeMaXiYa			0x202		//��������
#define FeiLeiErZhuoDe		0x302		//���׶�׿��
#define WuWeiXianFeng		0x402		//��η�ȷ�
#define ShuRuiMa			0x502		//ˡ����
#define NiuQuChongLin		0x602		//Ť������
#define JuLongZhiChao		0x702		//����֮��
//����
#define JiaoYuWangZhuanQu	0x10003		//������ר��


#define PROCESSNAME L"CrossProxy.exe"
#include <windows.h>

#define MSG_LOLINFO_HALL					(0x1001)	//����
#define MSG_LOLINFO_CLIENT					(0x1002)	//League of Legends.exe

struct TGameInfo
{
	DWORD dwUin;			//QQ��
	wchar_t area[100];		//����	
	wchar_t	userName[100];	//�û���
	TGameInfo()
	{
		memset(this, 0, sizeof(TGameInfo));
	}
};