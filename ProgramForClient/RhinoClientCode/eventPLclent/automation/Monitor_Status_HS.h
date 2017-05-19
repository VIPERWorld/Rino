#ifndef _MONITOR_STATUS_HS_H_
#define _MONITOR_STATUS_HS_H_



enum EnGameStatusHS
{
	E_STATUS_HS_NOTSTART = 1,
	E_STATUS_HS_HOOKED,
	E_STATUS_HS_D3D_ERROR,
	//E_STATUS_HS_ADDFRIEND_REQ,		// 已发送添加好友请求
	//E_STATUS_HS_ADDFRIEND_ACCEPT,	// 已接受添加好友请求
	//E_STATUS_HS_CHALLENGE_REQ,		// 已发送挑战请求
	//E_STATUS_HS_CHALLENGE_ACCEPT,	// 已接受挑战请求

	//E_STATUS_HS_READY_TO_PLAY,		// 游戏准备中

	E_STATUS_HS_CHOOSEING,			// 选择游戏角色
	//E_STATUS_HS_CANCEL_PLAY,		// 用户取消
	E_STATUS_HS_PLAYING,			// 正在进行游戏

	E_STATUS_HS_GAME_VICTORY,		// 游戏结束：胜利
	E_STATUS_HS_GAME_FAIL,			// 游戏结束：失败

	E_STATUS_HS_AM_SUCCESS,			// 执行自动化成功
	E_STATUS_HS_AM_FAIL,			// 执行自动化失败

	E_STATUS_HS_GAME_OFFLINE		// 注入DLL异常
};



#endif//_MONITOR_STATUS_HS_H_