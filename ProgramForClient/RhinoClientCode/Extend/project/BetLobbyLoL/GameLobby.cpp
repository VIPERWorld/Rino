#include "stdafx.h"
#include "GameLobby.h"


DWORD CGameLobby::m_JmpGameLobby = 0;
//����ѵ��ģʽ
const char* g_pchXrxlms = "createTutorialGame";
//�Զ���ģʽ
const char* g_pchZdymsJr = "listAllPracticeGames";
const char* g_pchZdymsCj = "createPracticeGame";
//�˻�ģʽ
const char* g_pchRjmsJr = "attachToQueue";
const char* g_pchRjmsTd = "createArrangedBotTeamLobby";
const char* g_pchRjmsEx = "MEDIUM";
//��Ҷ�ս
//ƥ��ģʽ
const char* g_pchWjdzppmsTd = "createArrangedTeamLobby";
//��λ��
const char* g_pchWjdzPws = "teambuilder-draft-indicateAfkReadinessV1";
const char* g_pchWjdzPwsSelect = "teambuilder-draft5signalChampionPickIntentV1";
//����ѡ��ɫ
const char* g_pchStartGame = "setClientReceivedGameMessage";
const char* g_pchSelectRole = "CHAMP_SELECT_CLIENT";
//�˳�ѡ��ɫ
const char* g_pchEndGame = "quitGame";
//�Ƿ���ʾѺע
bool g_bBetState = false;
//ģʽ(1:�Զ���,2:ƥ��,3:��λ)
int g_inZdyMode = 0;


CGameLobby::CGameLobby()
{
	m_HookAddress = 0;
	::ZeroMemory(m_HookOpCode, ARRAYSIZE(m_HookOpCode));
}

CGameLobby::~CGameLobby()
{

}

void CGameLobby::FilterGameLobby(char* pchBuffer, int inLen)
{
	__try {

		if ((NULL == pchBuffer) || (inLen <= 0)) {

			return;
		}

		for (int i = 0; i < inLen; i++, ++pchBuffer) {

			//����ѵ��ģʽ
			if (::memcmp(pchBuffer, g_pchXrxlms, ::strlen(g_pchXrxlms)) == 0) {

				g_bBetState = false;
				g_inZdyMode = 0;
			}
			//�Զ���ģʽ
			if (::memcmp(pchBuffer, g_pchZdymsJr, ::strlen(g_pchZdymsJr)) == 0) {

				g_bBetState = true;
				g_inZdyMode = 1;
			}

			if (::memcmp(pchBuffer, g_pchZdymsCj, ::strlen(g_pchZdymsCj)) == 0) {

				g_bBetState = true;
				g_inZdyMode = 1;
			}
			//�˻�ģʽ
			if (::memcmp(pchBuffer, g_pchRjmsJr, ::strlen(g_pchRjmsJr)) == 0) {

				if (FindMode(i, inLen, pchBuffer, g_pchRjmsEx)) {

					g_bBetState = false;
					g_inZdyMode = 0;
				}
				else {

					g_bBetState = true;
					g_inZdyMode = 2;
				}
			}

			if (::memcmp(pchBuffer, g_pchRjmsTd, ::strlen(g_pchRjmsTd)) == 0) {

				g_bBetState = false;
				g_inZdyMode = 0;
			}
			//��Ҷ�ս(ƥ��ģʽ)
			if (::memcmp(pchBuffer, g_pchWjdzppmsTd, ::strlen(g_pchWjdzppmsTd)) == 0) {

				g_bBetState = true;
				g_inZdyMode = 2;
			}

			//��Ҷ�ս(��λ��)
			if (::memcmp(pchBuffer, g_pchWjdzPws, ::strlen(g_pchWjdzPws)) == 0) {

				g_bBetState = true;
				g_inZdyMode = 3;
			}

			//////////////////////////////////////////////////////////////////////////
			//����ѡӢ��
			if (::memcmp(pchBuffer, g_pchStartGame, ::strlen(g_pchStartGame)) == 0) {

				if ((true == g_bBetState) && (FindMode(i, inLen, pchBuffer, g_pchSelectRole))) {

					SendGameData(inselect, g_inZdyMode);
				}

				break;
			}
			else if (::memcmp(pchBuffer, g_pchWjdzPwsSelect, ::strlen(g_pchWjdzPwsSelect)) == 0) {

				if (true == g_bBetState) {

					SendGameData(inselect, g_inZdyMode);
				}

				break;
			}

			//��ѡӢ���˳�
			if (::memcmp(pchBuffer, g_pchEndGame, ::strlen(g_pchEndGame)) == 0) {

				SendGameData(breakslect);
				break;
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

	}
}

__declspec(naked)void __stdcall FilterFlashSend()
{
	__asm {

		pushad
		pushfd

		push eax
		lea eax, dword ptr[esi + edi + 0x1C50]
		push eax
		call CGameLobby::FilterGameLobby

		popfd
		popad

		lea eax, dword ptr[esi + edi + 0x1C50]
		jmp dword ptr CGameLobby::m_JmpGameLobby
	}
}

void CGameLobby::HookGameLobby()
{
	__try {

		HMODULE hModule = NULL;

		while (NULL == hModule) {

			hModule = ::GetModuleHandle(TEXT("Adobe AIR.dll"));

			::Sleep(500);
		}

		byte* pBase = (byte*)hModule + 0x1000;
		m_HookAddress = 0;
		::ZeroMemory(m_HookOpCode, ARRAYSIZE(m_HookOpCode));

		for (int i = 0; i < 0x0FFFFFFF; i++) {

			if ((*(pBase - 0x09) == 0x8B) &&
				(*(pBase - 0x08) == 0x86) &&
				(*(pBase - 0x03) == 0x2B) &&
				(*(pBase - 0x02) == 0xC7) &&
				(*(pBase) == 0x8D) &&
				(*(pBase + 0x01) == 0x84) &&
				(*(pBase + 0x02) == 0x3E)) {

				m_HookAddress = (DWORD)pBase;
				break;
			}

			++pBase;
		}

		if (0 == m_HookAddress) {

			::OutputDebugString(TEXT("Error:HookGameLobby()->û���ҵ�HOOK��ַ"));
			return;
		}

		m_JmpGameLobby = m_HookAddress + 0x07;
		::memcpy(m_HookOpCode, (void*)m_HookAddress, ARRAYSIZE(m_HookOpCode));

		byte byCode[] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90 };
		DWORD dwJmpAddr = (DWORD)FilterFlashSend - m_HookAddress - 5;
		::memcpy(byCode + 1, &dwJmpAddr, sizeof(DWORD));

		DWORD dwOleFlags = 0;
		::VirtualProtect((LPVOID)m_HookAddress, ARRAYSIZE(m_HookOpCode), PAGE_EXECUTE_READWRITE, &dwOleFlags);
		::memcpy((void*)m_HookAddress, byCode, ARRAYSIZE(byCode));
		::VirtualProtect((LPVOID)m_HookAddress, ARRAYSIZE(m_HookOpCode), dwOleFlags, NULL);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

	}
}

void CGameLobby::UnHookGameLobby()
{
	__try {

		if (0 == m_HookAddress) {

			::OutputDebugString(TEXT("Error:UnHookGameLobby()->û��HOOK��ַ"));
			return;
		}

		DWORD dwOleFlags = 0;
		::VirtualProtect((LPVOID)m_HookAddress, ARRAYSIZE(m_HookOpCode), PAGE_EXECUTE_READWRITE, &dwOleFlags);
		::memcpy((void*)m_HookAddress, m_HookOpCode, ARRAYSIZE(m_HookOpCode));
		::VirtualProtect((LPVOID)m_HookAddress, ARRAYSIZE(m_HookOpCode), dwOleFlags, &dwOleFlags);

		m_HookAddress = 0;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

	}
}

bool CGameLobby::FindMode(int inStart, int inEnd, char* pchData, const char* pchTemp)
{
	bool bRet = false;

	__try {

		if ((0 == inStart) || (0 == inEnd) || (NULL == pchData) || (NULL == pchTemp)) {

			return bRet;
		}

		for (int i = inStart; i < inEnd; i++, ++pchData) {

			if (::memcmp(pchData, pchTemp, ::strlen(pchTemp)) == 0) {

				bRet = true;
				break;
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

		bRet = false;
	}

	return bRet;
}

void CGameLobby::SendGameData(State pState, int inMode)
{
	try {

		COPYDATASTRUCT CopyData = { 0 };

		if (inselect == pState) {

			CopyData.dwData = WM_USER + 1;
			CopyData.lpData = &inMode;
			CopyData.cbData = sizeof(inMode);

			::OutputDebugString(TEXT("����ѡ��ɫ!"));
		}
		else if (breakslect == pState) {

			CopyData.dwData = WM_USER + 2;

			::OutputDebugString(TEXT("��ѡ��ɫ�˳�!"));
		}

		HWND hWnd = ::FindWindow(NULL, TEXT("Ϭţ�羺"));

		if (NULL == hWnd) {

			::OutputDebugString(TEXT("Error:û���ҵ�ƽ̨!"));
			return;
		}

		::SendMessage(hWnd, WM_COPYDATA, NULL, (LPARAM)&CopyData);
	}
	catch (...) {

	}
}