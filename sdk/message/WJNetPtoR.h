#ifndef _WJ_NetPtoR_H_
#define _WJ_NetPtoR_H_
//���ļ������½����������Ϸ������֮���ͨ��Э��
#include "message/WJNetPtoR_roominfo.h"
#include "message/WJNetPtoR_userinfo.h"
namespace WJ
{
	//////////////////////////////////////////////////////////////////////////
	//�������ϷͨѶָ��궨��

	enum E_PtoR_MAINID
	{
		e_pr_mess_roominfo = 500,//������Ϣ
		e_pr_mess_userinfo = 501,//�û���Ϣ
	};
}

#endif // _WJ_NetPtoR_H_