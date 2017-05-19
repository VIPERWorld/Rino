#include "GameMonitorHS.h"

#include "caffe/ResourceHandler.hpp"
#include "CaffeManager.h"

#include "hook/HookEventParam.h"
extern int g_gameWndOffsetX;
extern int g_gameWndOffsetY;

#include "hook/HookD3D9.h"
#include "hook/CaptureFromShareTexDx9.h"

#include "automation/WJAutomation.h"
#include "automation/AMUtil.h"

#include "identification/pre_text_img.h"
#include "hook/WSMessageId.h"

#include "ocrimpl.h"

#include <Windows.h>
#include <winuser.h>

#include "common/LuaEngine.h"
#include "identification/hs_identification.h"

#define CHARACTER_DATABASE	"chi"	/* ����ʶ��� */


CaptureFromShareTexDx9 g_captureFromShareTex;
CaffeManager g_caffeMgr;

//using namespace cv;

extern HMODULE g_hModule;

#include "XLog.h"
extern XLog g_log;
DWORD ModuleAddress;	//��ģ���ַ

#define WJ_SHARE_MEMORY_NAME_HOOK_HS	TEXT("WJ_SHARE_MEMORY_NAME_HOOK_HS")

//dll ·��--
string g_modulePath;


extern IDirect3DDevice9Ex *deviceEx;
//extern HANDLE g_subThreadEvent;

extern AMCallback* g_pAmCallback;

#define WH_SHARE_MEMORY_HS	TEXT("WH_SHARE_MEMORY_HS")

#define AM_SHARE_MEM_HS_SERVER		TEXT("AM_SHARE_MEM_HS_SERVER")	
#define AM_SHARE_MEM_HS_CLIENT		TEXT("AM_SHARE_MEM_HS_CLIENT")	

std::string Utf8ToStr(const std::string& _utf8)
{
	static char sBuffer[1024];
	static wchar_t sBufferTemp[1024];

	ZeroMemory(sBufferTemp, 1024);
	ZeroMemory(sBuffer, 1024);

	int len = MultiByteToWideChar(CP_UTF8, 0, _utf8.c_str(), -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, _utf8.c_str(), _utf8.length(), sBufferTemp, len);

	int length = WideCharToMultiByte(CP_ACP, 0, sBufferTemp, -1, NULL, NULL, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, sBufferTemp, len, sBuffer, length, NULL, NULL);

	return std::string(sBuffer);
}



void AMResourceThreadProc(void *_p)
{
	GameMonitorHS * monitorHS = (GameMonitorHS*)_p;
	if (monitorHS)
	{
		monitorHS->initResource();
		do
		{
			double tick = GetTickCount();
			if (FindWindow(NULL, TEXT("¯ʯ��˵")))
			{
				try
				{
					monitorHS->update();
				}
				catch (...)
				{
					//g_log.writeLog(XLog::E_LOG_DEBUG, "AMResourceThreadProc------err update() ");
				}
			}
			//else
			//{
			//	if (monitorHS->getGameStatus() != E_STATUS_HS_NOTSTART)
			//	{
			//		monitorHS->setGameStatus(E_STATUS_HS_NOTSTART);
			//	}
			//}
			//Sleep(500);
			double t = 200 - (GetTickCount() - tick);
			if (t > 0)
			{
				Sleep(t);
			}
		} while (true);
	}
}


#include <atlimage.h>
void CImageToMat(CImage& cimage, Mat& mat)
{
	if (true == cimage.IsNull())
	{
		return;
	}


	int nChannels = cimage.GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = cimage.GetWidth();
	int nHeight = cimage.GetHeight();


	//�ؽ�mat
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}


	//��������


	uchar* pucRow;									//ָ������������ָ��
	uchar* pucImage = (uchar*)cimage.GetBits();		//ָ����������ָ��
	int nStep = cimage.GetPitch();					//ÿ�е��ֽ���,ע���������ֵ�����и�


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}
			}
		}
	}
}

bool GameMonitorHS::getWindowMatHS(HWND hWnd, Mat& _mat)
{
	RECT rt;
	::GetWindowRect(hWnd, &rt);

	HDC hdc = ::GetWindowDC(hWnd);
	if (!hdc)
		return false;

	HDC hdcMem = ::CreateCompatibleDC(hdc);
	if (!hdcMem)
		return false;

	HBITMAP hbitmap = ::CreateCompatibleBitmap(hdc, rt.right - rt.left, rt.bottom - rt.top);
	if (!hbitmap)
		return false;

	::SelectObject(hdcMem, hbitmap);
	::PrintWindow(hWnd, hdcMem, 0);

	BITMAP bmp;
	GetObject(hbitmap, sizeof(BITMAP), &bmp);
	int nChannels = (bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8);
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;

	Mat matTemp;
	matTemp.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));
	GetBitmapBits(hbitmap, bmp.bmHeight*bmp.bmWidth*nChannels, matTemp.data);

	m_width = rt.right - rt.left;
	m_height = rt.bottom - rt.top;
	if (0 == rt.left && 0 == rt.top && GetSystemMetrics(SM_CXSCREEN) == m_width && GetSystemMetrics(SM_CYSCREEN) == m_height)
	{
		int offset = 0;
		int r, g, b;
		for (int j = 0; j < matTemp.cols / 4; ++j)
		{
			// [row, col]���صĵ� 1 ͨ����ַ�� * ����(blueͨ��)
			b = int(*(matTemp.data + matTemp.step[0] * 0 + matTemp.step[1] * j));
			//[row, col]���صĵ� 2 ͨ����ַ�� * ����(greenͨ��), ����elemSize1�����ĸ���������� Fn1 �����еĲ�������
			g = int(*(matTemp.data + matTemp.step[0] * 0 + matTemp.step[1] * j + matTemp.elemSize1()));
			//[row, col]���صĵ� 3 ͨ����ַ�� * ����(redͨ��)
			r = int(*(matTemp.data + matTemp.step[0] * 0 + matTemp.step[1] * j + matTemp.elemSize1() * 2));
			if (0 == b + g + r)
			{
				++offset;
			}
			else
			{
				break;
			}
		}
		//int width = int((rt.bottom - rt.top) * float(m_width) / float(m_height));
		//g_gameWndOffsetX = int(0.5 *((rt.right - rt.left) - width));
		g_gameWndOffsetX = offset;
		g_gameWndOffsetY = 0;

		m_width -= g_gameWndOffsetX * 2;
		m_height -= g_gameWndOffsetY;
		try
		{
			_mat = matTemp(Rect(g_gameWndOffsetX, 0, m_width, m_height));
		}
		catch (...)
		{
			return false;
		}

		//imwrite("./HS_TEMP/temp123.jpg", _mat);
	}
	else
	{
		g_gameWndOffsetX = 8;
		g_gameWndOffsetY = 30;

		/* ���ڷֱ��� */
		m_width -= g_gameWndOffsetX * 2;
		m_height -= (g_gameWndOffsetY + 8);
		try
		{
			_mat = matTemp(Rect(g_gameWndOffsetX, g_gameWndOffsetY, m_width, m_height));
		}
		catch (...)
		{
			return false;
		}
		//imwrite("./HS_TEMP/temp124.jpg", _mat);
	}

	/* �����б��� */
	m_rectFriendWindow.x = 4;
	m_rectFriendWindow.height = int(m_height*0.675);
	m_rectFriendWindow.y = int(m_height - m_height*0.052 - m_rectFriendWindow.height);
	m_rectFriendWindow.width = int(m_rectFriendWindow.height*0.56);

	::DeleteObject(hbitmap);
	::DeleteObject(hdcMem);
	::ReleaseDC(hWnd, hdc);
	return true;
}

CRITICAL_SECTION g_cs;

GameMonitorHS::GameMonitorHS()
	: m_isVaild(false)
	, m_shareMem(nullptr)
	, m_gameStatus(E_STATUS_HS_NOTSTART)
	, m_initThreadHandle(0)
	, m_width(0)
	, m_height(0)
	, m_eventId(-1)
	, m_binit_KnowHeroLS(false)
	, m_pCallback_KnowHeroLS(nullptr)
{
	InitializeCriticalSection(&g_cs);

	m_vecImg.push_back("HS_TASK");
	m_vecImg.push_back("HS_MENU");
	m_vecImg.push_back("HS_CHOOSE");
	m_vecImg.push_back("HS_VS");
	//m_vecImg.push_back("HS_WIN");
	//m_vecImg.push_back("HS_FAIL");
	//m_vecImg.push_back("HS_SETTING");
	//m_vecImg.push_back("HS_ACCEPT_CHALLENGE");
	//m_vecImg.push_back("HS_FRIEND");
	m_initThreadHandle = (HANDLE)_beginthread(AMResourceThreadProc, 0, this);
}


GameMonitorHS::~GameMonitorHS()
{
	if (m_initThreadHandle)
	{
		::TerminateThread(m_initThreadHandle, 1);
		m_initThreadHandle = NULL;
	}
}

bool GameMonitorHS::initResource()
{
	g_log.initialize("WJAutomation");

	/* ����Ѿ���ʼ������ִ�� */
	if (m_isVaild)
		return true;

	/* ��ȡ����ִ�еĵ�ǰ·�� */
	char buf[256] = { 0 };
	GetCurrentDirectoryA(256 - 1, buf);
	m_appPath = buf;

	int index = -1;
	while ((index = m_appPath.find("\\")) > 0)
	{
		m_appPath = m_appPath.replace(index, 1, "/");
	}
	CreateDirectoryA((m_appPath + "/HS_TEMP").c_str(), 0);

	/* ��ʼ������ʶ��� */
	if (!ocrinit(NULL, CHARACTER_DATABASE))
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "��ʼ������ʶ���[%s]ʧ��!", CHARACTER_DATABASE);
	}
	//m_webSocketSrv = new WebSocketServer(g_shareParamHS.wsPort_Srv, this, NULL);

	/* ��ʼ��ͼ��ʶ����Դ */
	std::string path = m_appPath + "\\luaScript\\hs_image.out";
	if (!m_luaEngine.loadLuaFile(path.c_str()))
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "LuaEngine�����ļ�[%s]ʧ��!", path.c_str());
	}


	LuaDataMap* pLuaData = m_luaEngine.getTable(HSLUA_IDENTIFICATION_KEY);
	if (!pLuaData)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "����lua�ļ���ȡ�ؼ�key[%s]����!", HSLUA_IDENTIFICATION_KEY);
	}
	else
	{
		IndentificationMap indentifimapTemp;

		//��ȡ���ñ�
		for (auto data = pLuaData->begin(); data != pLuaData->end(); data++)
		{
			LuaDataMap* subData = data->second->value.value_map;
			if (!subData)
			{
				g_log.writeLog(XLog::E_LOG_DEBUG, "LUA ���ݽ�������");
			}
			else
			{
				string* name = ((*subData)["name"])->value.value_str;
				string* imageFile = ((*subData)["file"])->value.value_str;
				int threshold = ((*subData)["threshold"])->value.value_int;

				m_mapImgThreshold[name->c_str()] = threshold;

				ImageFileVector imageFileVector;
				imageFileVector.push_back(imageFile->c_str());
				indentifimapTemp.insert(IndentificationMap::value_type(name->c_str(), imageFileVector));
			}
		}

		if (!m_imageIdentifucation.initImage(indentifimapTemp, false))
		{
			g_log.writeLog(XLog::E_LOG_ERROR, "��ʼ��ͼ��ʶ����Դʧ��.");
		}
	}

	return true;
}

bool GameMonitorHS::initEmbedWinHS(int _wsPort, const char* _urlMatch, const char* _urlCover)
{
	/* ��ʼ�������ڴ� */
	//ShareParamHS param;
	//param.wsPort_Srv = 12351;
	//param.wsPort_Hook = _wsPort;
	//strcpy_s(param.urlMatch, (m_appPath + _urlMatch/*"/H5/HS_stake.html"*/).c_str());
	//strcpy_s(param.urlCover, (m_appPath + _urlCover/*"/H5/HS_stake.html"*/).c_str());

	//if (!m_shareMem)
	//{
	//	m_shareMem = new CShareMemory;
	//}
	//m_shareMem->OpenShareFile(WH_SHARE_MEMORY_HS);
	//m_shareMem->WriteShareMemory(sizeof(ShareParamHS), &param);



	m_isVaild = true;
	return true;
}

std::string GameMonitorHS::getGameImage(Mat& mat, int& _size)
{
	std::string id;

	//printf("ͼ��ƥ�䣺 ");
	//for (auto iter = m_mapImgThreshold.begin(); iter != m_mapImgThreshold.end(); ++iter)
	for (auto iter = m_vecImg.begin(); iter != m_vecImg.end(); ++iter)
	{
		double tick = GetTickCount();
		const std::string& curId = (*iter);
		int sizeTemp = imageContrast(mat, curId.c_str());
		//printf("  [%s:%d] <%lf>", curId.c_str(), sizeTemp, GetTickCount() - tick);
		if (sizeTemp > _size)
		{
			_size = sizeTemp;
			id = curId;
		}
	}
	//printf("\n");
	return id;
}



void GameMonitorHS::saveTempImg(const char* _name, int _val, Mat& _mat)
{
	char file[1024] = { 0 };
	sprintf_s(file, "/HS_TEMP/%s.png", _name);
	imwrite(m_appPath + file, _mat);
}



EnGameStatusHS& GameMonitorHS::update()
{
	HWND hWnd = ::FindWindow(NULL, TEXT("¯ʯ��˵"));
	if (!hWnd)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "¯ʯ��˵������");
		return m_gameStatus;
	}
	if (IsIconic(hWnd))
	{
		return m_gameStatus;
	}

	Mat mat;
	getWindowMatHS(hWnd, mat);

	int size = 0;		/* ͼ���ƥ��� */
	std::string img;	/* ƥ�䵽��ͼ������ */

	switch (m_gameStatus)
	{
	case E_STATUS_HS_D3D_ERROR:
		break;

	case E_STATUS_HS_NOTSTART:
		break;
	case E_STATUS_HS_HOOKED:
		if ((size = imageContrast(mat, "HS_CHOOSE")) > getImgThreshold("HS_CHOOSE"))
		{
			saveTempImg("HS_CHOOSE", size, mat);
			g_log.writeLog(XLog::E_LOG_DEBUG, "ƥ�䵽 HS_CHOOSE (%d)����ǰ״̬��HOOKED/CANCEL_PLAY --> ����ΪCHOOSEING ", size);
			setGameStatus(E_STATUS_HS_CHOOSEING);
			setEvent(GAME_EVENT_AM_END);
		}
		else
		{
			Rect rt;
			rt.height = int(m_height*0.5);
			rt.width = int(rt.height*1.28);
			rt.x = int(0.5*(m_width - rt.width));
			rt.y = int(0.5*(m_height - rt.height));
			size = imageContrastRect(mat, "HS_VS", rt);
			if (size > getImgThreshold("HS_VS")) // ƥ�������ϷPK����
			{
				saveTempImg("HS_VS", size, mat);
				g_log.writeLog(XLog::E_LOG_DEBUG, "ƥ�䵽 HS_VS (%d)����ǰ״̬��CHOOSEING --> ����ΪPLAYING ", size);
				setGameStatus(E_STATUS_HS_PLAYING);
				setEvent(GAME_EVENT_GAME_BEGIN);
				Sleep(5000);
			}
		}
		break;

	case E_STATUS_HS_CHOOSEING:
	{
		Rect rt;
		rt.height = int(m_height*0.5);
		rt.width = int(rt.height*1.28);
		rt.x = int(0.5*(m_width - rt.width));
		rt.y = int(0.5*(m_height - rt.height));
		size = imageContrastRect(mat, "HS_VS", rt);
		if (size > getImgThreshold("HS_VS")) // ƥ�������ϷPK����
		{
			saveTempImg("HS_VS", size, mat);
			g_log.writeLog(XLog::E_LOG_DEBUG, "ƥ�䵽 HS_VS (%d)����ǰ״̬��CHOOSEING --> ����ΪPLAYING ", size);
			setGameStatus(E_STATUS_HS_PLAYING);
			setEvent(GAME_EVENT_GAME_BEGIN);
			Sleep(5000);
		}
	}
	break;

	case E_STATUS_HS_PLAYING:
	{
		bool isEnd = false;

		int size1 = imageContrastRect(mat, "HS_END-001", Rect(int(m_width*0.375), int(m_height*0.60), int(m_width*0.25), int(m_height*0.26)), true);
		int size2 = imageContrastRect(mat, "HS_END-002", Rect(int(m_width*0.375), int(m_height*0.60), int(m_width*0.25), int(m_height*0.26)), true);
		if (size1 > getImgThreshold("HS_END-001") || size2 > getImgThreshold("HS_END-002"))
		{
			g_log.writeLog(XLog::E_LOG_DEBUG, "ƥ�䵽������ը��Ч��HS-END-001(%d), HS-END-002(%d), ��Ϸ�������ܱ�", size1, size2);
			saveTempImg("HS_FAIL", size, mat);
			setGameStatus(E_STATUS_HS_GAME_FAIL);
			this->setEvent(GAME_EVENT_GAME_END);
		}
		else
		{
			int size3 = imageContrastRect(mat, "HS_END-003", Rect(int(m_width*0.375), int(m_height*0.075), int(m_width*0.25), int(m_height*0.26)), true);
			int size4 = imageContrastRect(mat, "HS_END-004", Rect(int(m_width*0.375), int(m_height*0.075), int(m_width*0.25), int(m_height*0.26)), true);
			if (size3 > getImgThreshold("HS_END-003") || size4 > getImgThreshold("HS_END-004"))
			{
				g_log.writeLog(XLog::E_LOG_DEBUG, "ƥ�䵽���ֱ�ը��Ч��HS-END-003(%d), HS-END-004(%d), ��Ϸ������ʤ��", size3, size4);
				saveTempImg("HS_WIN", size, mat);
				setGameStatus(E_STATUS_HS_GAME_VICTORY);
				this->setEvent(GAME_EVENT_GAME_END);
			}
		}


		if (hWnd && (E_STATUS_HS_GAME_VICTORY == m_gameStatus || E_STATUS_HS_GAME_FAIL == m_gameStatus))
		{
			Sleep(5000);
			clickWindow(hWnd, float(m_width*0.5), float(m_height*0.57), 1000);
			clickWindow(hWnd, float(m_width*0.5), float(m_height*0.57), 1000);
			clickWindow(hWnd, float(m_width*0.5 + m_height*0.58), float(m_height*0.915), 0);

			double tick = GetTickCount();
			while (true)
			{
				Mat matTemp;
				getWindowMatHS(hWnd, matTemp);
				//saveTempImg("D:/temp.jpg", 0, matTemp);
				if (imageContrast(matTemp, "HS_MENU") > getImgThreshold("HS_MENU") && GetTickCount() - tick < 6000)
				{
					g_log.writeLog(XLog::E_LOG_DEBUG, "����������");
					clickWindow(hWnd, float(m_width*0.5), int(g_gameWndOffsetY + m_height*0.555));
					break;
				}
				else
				{
					clickWindow(hWnd, float(m_width*0.5 + m_height*0.58), float(m_height*0.915), 0);
				}
				Sleep(1000);
			}
		}
	}
	break;

	case E_STATUS_HS_GAME_VICTORY:
	case E_STATUS_HS_GAME_FAIL:
		setGameStatus(E_STATUS_HS_HOOKED);
		break;

	case E_STATUS_HS_AM_SUCCESS:
	case E_STATUS_HS_AM_FAIL:
	{
		setGameStatus(E_STATUS_HS_CHOOSEING);
		this->setEvent(GAME_EVENT_AM_END);
	}
	break;
	break;

	case E_STATUS_HS_GAME_OFFLINE:
		break;
	default:
		break;
	}


	if (m_binit_KnowHeroLS)
	{//�Ƿ��Ѿ���ʼ���˹���ʶ��Ӣ�۵�����
		do 
		{
			int w = mat.cols;
			int h = mat.rows;

			Rect rt(w*0.5 + h*0.26, h*0.26, h * 0.88 / 3, h / 3);
			Mat imgCharacter = mat(rt);

			PredictionRes res1;
			g_caffeMgr.getRoleType(imgCharacter, res1);

			if (m_pCallback_KnowHeroLS)
			{
				m_pCallback_KnowHeroLS->onDoForKnowHeroLS(res1.name);
			}
			cv::rectangle(mat, rt, Scalar(0, 0, 255), 2);
		} while (0);
	}


	return m_gameStatus;
}


//
//Mat* GameMonitorHS::getRanderData()
//{
//	static Mat mat;
//	GetWindowMatHS(mat);
//	return &mat;
//	//if (m_matTemp.data == nullptr)
//	//{
//	//	m_matTemp.data = new uchar(1920 * 1080 * 4);
//	//}
//
//	//IDirect3DSurface9* pOffSurface9 = g_captureFromShareTex.getRenderData();
//	//if (!pOffSurface9)
//	//	return NULL;
//
//	//D3DSURFACE_DESC desc;
//	//pOffSurface9->GetDesc(&desc);
//
//	//D3DLOCKED_RECT DeviceDrawRect;
//	//HRESULT hr;
//	//if (!SUCCEEDED(hr = pOffSurface9->LockRect(&DeviceDrawRect, NULL, D3DLOCK_READONLY)))
//	//	return NULL;
//
//
//	////Mat matImage(desc.Height, desc.Width, CV_8UC4, DeviceDrawRect.pBits);
//	////_matImg.create(desc.Height, desc.Width, CV_8UC4);
//	////mat.rows = desc.Height;
//	////mat.cols = desc.Width;
//
//	//if (m_matTemp.rows != desc.Height || m_matTemp.cols != desc.Width)
//	//{
//	//	m_matTemp = Mat(desc.Height, desc.Width, CV_8UC4);
//	//}
//
//	//memcpy(m_matTemp.data, DeviceDrawRect.pBits, desc.Height*DeviceDrawRect.Pitch);
//	//pOffSurface9->UnlockRect();
//	//return &m_matTemp;
//}


std::string GameMonitorHS::textRecognition(const char* _name, Mat& _mat)
{
	Mat _matOut;
	cvtColor(_mat, _matOut, CV_BGRA2GRAY);
	//threshold(matOut, matOut, 70, 255, CV_THRESH_BINARY_INV);

	std::string path = m_appPath;
	path += "/HS_TEMP/";
	path += _name;
	path += ".png";
	imwrite(path.c_str(), _matOut);

	char* text = ocrText(_matOut.data, _matOut.cols, _matOut.rows, 1);
	if (!text)
		return "";

	return Utf8ToStr(text);
}

void GameMonitorHS::showFriendWindow(HWND _hWnd)
{
	//if (this->imageContrast("HS_TASK"))
	//{
	//	RECT rt;
	//	::GetWindowRect(_hWnd, &rt);
	//	AMUtil::mouseClickLButton1(_hWnd, rt.left + g_gameWndOffsetX + (rt.right - rt.left) / 2, rt.top + g_gameWndOffsetY + (rt.bottom - rt.top) / 2);
	//	Sleep(200);
	//}

	int count = 0;
	RECT rt;
	::GetWindowRect(_hWnd, &rt);
	BringWindowToTop(_hWnd);

	for (int i = 0; i < 3; ++i)
	{
		AMUtil::mouseClickLButton1(_hWnd, rt.left + g_gameWndOffsetX + (rt.right - rt.left) / 2, rt.bottom - 20);
	}
	Sleep(50);
	AMUtil::mouseClickLButton1(_hWnd, rt.left + 30 + g_gameWndOffsetX, rt.bottom - 20 - 8);
}


bool GameMonitorHS::getPlayerName(HWND _hWnd, std::string& _name)
{
	//SwitchToThisWindow(_hWnd, TRUE);
	//Sleep(500);
	//
	//showFriendWindow(_hWnd);
	//Sleep(500);

	//Mat* magImg = getRanderData();
	//if (!magImg)
	//{
	//	return false;
	//}

	Mat mat;
	getWindowMatHS(_hWnd, mat);
	Mat matFrientBox = mat(m_rectFriendWindow);
	imwrite(m_appPath + "/HS_TEMP/hs_�����б��.png", matFrientBox);

	//m_imageIdentifucation.shibieLocation("HS_FRIEND", &matFrientBox);

	/* ������� */
	Rect rtName;
	rtName.x = int(m_rectFriendWindow.width*0.25);
	rtName.y = int(m_rectFriendWindow.height*0.022);
	rtName.width = int(m_rectFriendWindow.width*0.7);
	rtName.height = int(m_rectFriendWindow.height*0.09);
	_name = this->textRecognition("�������", matFrientBox(rtName));
	g_log.writeLog(XLog::E_LOG_DEBUG, "������ƣ� %s", _name);

	return true;
}

bool checkName(std::string& _src, std::string& _name)
{
	int i = _src.find_last_of('#');
	if (i < 0)
		return false;

	std::string id = _src.substr(i);
	i = id.find_last_not_of('\n');
	if (i < 0)
		return false;
	id = id.substr(0, i + 1);

	do
	{
		i = id.find(" ");
		if (i < 0)
		{
			break;
		}
		else
		{
			id.replace(i, 1, "");
		}
	} while (true);

	i = _name.find_last_of('#');
	if (i < 0)
		return false;

	std::string nameId = _name.substr(i);
	int res = id.find(nameId);
	if (id.length() > 0 && nameId.length() > 0 && res >= 0)
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "ƥ�䵽�û�����%s : %s", id.c_str(), nameId.c_str());
		return true;
	}
	return false;
}

bool GameMonitorHS::getFriendRect(const char* _name, vector<Rect>& _vecRt)
{

	/* ¯ʯ���ڲ�����ʱ����false */
	HWND hWnd = ::FindWindowA(NULL, "¯ʯ��˵");
	if (!hWnd)
		return false;

	Mat mat;
	getWindowMatHS(hWnd, mat);


	/* ��ʾ�����б��� */
	showFriendWindow(hWnd);
	Sleep(200);

	/* LOCK ȡͼ�� */
	/*Mat* magImg = getRanderData();
	if (!magImg)
	return false;*/

	CreateDirectoryA((m_appPath + "/HS_TEMP").c_str(), 0);
	imwrite(m_appPath + "/HS_TEMP/hs_temp.png", mat);


	/* ��ȡ���Ѵ���ͼ�� */
	Mat matFrientWin = mat(m_rectFriendWindow);
	imwrite(m_appPath + "/HS_TEMP/HS_���Ѵ���.png", matFrientWin);

	/* ��������б������С */
	Rect rtFriendBox;
	rtFriendBox.x = 0;
	rtFriendBox.y = int(m_rectFriendWindow.height*0.130);
	rtFriendBox.width = m_rectFriendWindow.width;
	rtFriendBox.height = int(m_rectFriendWindow.height*0.718);

	/* ��ȡ�����б�����ͼ�� */
	Mat matFriendBox = matFrientWin(rtFriendBox);
	imwrite(m_appPath + "/HS_TEMP/HS_�����б��.png", matFriendBox);

	/* ȡ�Ҷ�ͼ */
	Mat matImgColor;
	heartStone_friendList_color(matFriendBox, matImgColor);

	do
	{
		_vecRt.clear();

		vector<Rect> vecFriengList = get_heartStone_friendName_rect(matImgColor);
		for (size_t i = 0; i < vecFriengList.size(); i++)
		{
			Rect &rt = vecFriengList[i];

			char buf[128] = { 0 };
			sprintf_s(buf, "����_%d", i);

			if (rt.x + rt.width < matFriendBox.cols && rt.y + rt.height < matFriendBox.rows)
			{
				rt.height = int(rt.height*0.5);
				rt.width = int(m_rectFriendWindow.width*0.74 - rt.x);
				Mat matImgText = matFriendBox(rt);
				std::string text = this->textRecognition(buf, matFriendBox(rt));
				g_log.writeLog(XLog::E_LOG_DEBUG, "%s: %s", buf, text.c_str());
				if (checkName(text, std::string(_name)))
				{
					Rect rtOut;
					rtOut.x = rt.x + rtFriendBox.x + m_rectFriendWindow.x;
					rtOut.y = rt.y + rtFriendBox.y + m_rectFriendWindow.y;
					rtOut.width = rt.width;
					rtOut.height = rt.height;

					_vecRt.push_back(rtOut);
				}
			}
		}
		break;
	} while (0);

	return (int(_vecRt.size()) > 0);
}


int GameMonitorHS::imageContrast(Mat& _mat, const char* _image, bool _saveImg/*=false*/)
{
	try
	{
		//Mat* magImg = getRanderData();
		//if (!magImg)
		//	return false;
		double scale = 0.7;
		Size dsize = Size(_mat.cols*scale, _mat.rows*scale);
		Mat matT = Mat(dsize, _mat.type());
		resize(_mat, matT, dsize);

		if (_saveImg)
		{
			char buf[128] = { 0 };
			sprintf_s(buf, "%s/HS_TEMP/%s.png", m_appPath.c_str(), _image);
			imwrite(buf, matT);
		}
		//return m_imageIdentifucation.shibie(magImg, _image);
		return m_imageIdentifucation.shibie_flann(&matT, _image);
	}
	catch (...)
	{
		return 0;
	}
}

int GameMonitorHS::imageContrastRect(Mat& mat, const char* _image, Rect& _rt, bool _saveImg/*=false*/)
{
	try
	{
		/*	Mat* magImg = getRanderData();
			if (!magImg)
			return false;*/

		Mat matImgRect = mat(_rt);
		if (_saveImg)
		{
			char buf[128] = { 0 };
			sprintf_s(buf, "%s/HS_TEMP/%s.png", m_appPath.c_str(), _image);
			imwrite(buf, matImgRect);
		}
		//return (m_imageIdentifucation.shibie(&matImgRect, _image));
		return (m_imageIdentifucation.shibie_flann(&matImgRect, _image));
	}
	catch (...)
	{
		return 0;
	}
}

//bool GameMonitorHS::imageContrastRect(const char* _image, float _x, float _y, float _w, float _h)
//{
//	;
//	Mat* matImg = getRanderData();
//	if (!matImg)
//		return false;
//
//	Rect rt(matImg->cols*_x, matImg->rows*_y, matImg->cols*_w, matImg->rows*_h);
//	Mat matImgRect = (*matImg)(rt);
//	imwrite("D:/HS_TEMP/ͼ��ʶ��_TEMP_1.png", matImgRect);
//
//	// ʶ��ͼƬ
//	bool res = (m_imageIdentifucation.shibie(&matImgRect, _image) != nullptr);
//	return res;
//}
//
//
//char* GameMonitorHS::imageRecognition()
//{
//	Mat* magImg = getRanderData();
//	if (!magImg)
//		return false;
//
//	/* ʶ��ͼ�񣬻�ȡͼ��ID */
//	char* res = m_imageIdentifucation.shibie(magImg, NULL, 45);
//	return res;
//}

bool IsFullScreen(HWND _hWnd)
{
	RECT rt;
	::GetWindowRect(_hWnd, &rt);

	if (0 == rt.left && 0 == rt.top
		&& rt.right - rt.left == GetSystemMetrics(SM_CXSCREEN)
		&& rt.bottom - rt.top == GetSystemMetrics(SM_CYSCREEN))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GameMonitorHS::setGameWindwoSize(int _w, int _h)
{


}

bool GameMonitorHS::initShareTexture(CaptureInfo& _info)
{
	bool res = g_captureFromShareTex.init(_info);
	if (!res)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "��ʼ����������ʧ��.HANDLE=%d", _info.mSharedHandle);
		this->setGameStatus(E_STATUS_HS_D3D_ERROR);
	}
	else
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "��ʼ����������ɹ�.HANDLE=%d", _info.mSharedHandle);
		this->setGameWindwoSize(_info.mWidth, _info.mHeight);

		if (E_STATUS_HS_NOTSTART == m_gameStatus || E_STATUS_HS_D3D_ERROR == m_gameStatus)
		{
			this->setGameStatus(E_STATUS_HS_HOOKED);
		}
	}
	return res;
}


void GameMonitorHS::DoData(void* pData, size_t size)
{
	static HANDLE s_shareHandle = 0;

	if (pData)
	{
		EventParamHS param = *(EventParamHS*)pData;
		//hook�ɹ�,֪ͨ�ͻ���

		CaptureInfo captInfo;
		captInfo.mWidth = param.data.captInfo.width;
		captInfo.mHeight = param.data.captInfo.height;
		captInfo.mSharedHandle = param.data.captInfo.sharedHandle;
		captInfo.mFormat = param.data.captInfo.format;

		if (0 == captInfo.mSharedHandle)
		{
			return;
		}

		switch (param.id)
		{
		case E_EVENT_ID_HS_HOOK_SUCCESS:
		{
			this->initShareTexture(captInfo);
			s_shareHandle = captInfo.mSharedHandle;
			this->setGameWindwoSize(captInfo.mWidth, captInfo.mHeight);
		}
		break;

		case E_EVENT_ID_HS_HOOK_HEARTBEAT:
			if (!g_captureFromShareTex.m_pD3DOffScreenSurface9 || E_STATUS_HS_NOTSTART == m_gameStatus)
			{
				this->initShareTexture(captInfo);
			}
			else if (s_shareHandle != captInfo.mSharedHandle)
			{
				this->initShareTexture(captInfo);
			}
			s_shareHandle = captInfo.mSharedHandle;
			this->setGameWindwoSize(captInfo.mWidth, captInfo.mHeight);
			break;

		default:
			break;
		}
	}
}

void GameMonitorHS::SendData(void* pData, size_t maxSize)
{
}


void* GameMonitorHS::_callback(void* _data)
{
	if (_data)
	{
		IDirect3DSurface9* pOffSurface9 = (IDirect3DSurface9*)_data;
		D3DSURFACE_DESC desc;
		pOffSurface9->GetDesc(&desc);
		D3DLOCKED_RECT DeviceDrawRect;
		HRESULT hr;
		if (SUCCEEDED(hr = pOffSurface9->LockRect(&DeviceDrawRect, NULL, D3DLOCK_READONLY)))
		{
			Mat imageMatTest(desc.Height, desc.Width, CV_8UC4, DeviceDrawRect.pBits);
			imageMatTest.data = (uchar*)DeviceDrawRect.pBits;
			imwrite(m_appPath + "/imageTemp.png", imageMatTest);
			//m_imageIdentifucation.shibie(&imageMatTest);

			pOffSurface9->UnlockRect();
		}
	}
	return nullptr;
}



void GameMonitorHS::setGameStatus(const EnGameStatusHS& _status)
{
	if ((_status != m_gameStatus) && g_pAmCallback)
		g_pAmCallback->callbackStatusChandedHS(_status);

	m_gameStatus = _status;
	g_log.writeLog(XLog::E_LOG_ERROR, "����¯ʯ��Ϸ״̬: %d", m_gameStatus);
};

EnGameStatusHS& GameMonitorHS::getGameStatus()
{
	return m_gameStatus;
};


void GameMonitorHS::clickWindow(HWND _hWnd, float _x, float _y, int _sleep)
{
	/* ����ǰ����ʾ */
	BringWindowToTop(_hWnd);
	Sleep(200);

	/* ����Ļ�ϵ��һ�Σ��رտ�ʼ����ҳ���Ż��������� ���ͼ��ʶ���׼ȷ�ʣ�ͨ��ͼƬʶ���ж�״̬����е�������� */
	RECT rt;
	::GetWindowRect(_hWnd, &rt);

	if (_x < 1 && _y < 1)
	{
		AMUtil::mouseClickLButton1(_hWnd, int(rt.left + g_gameWndOffsetX + (rt.right - rt.left) * _x), int(rt.top + g_gameWndOffsetY + (rt.bottom - rt.top) * _y));
	}
	else
	{
		AMUtil::mouseClickLButton1(_hWnd, int(rt.left + g_gameWndOffsetX + _x), int(rt.top + g_gameWndOffsetY + _y));
	}
	Sleep(_sleep);
}

//��ʼ��¯ʯʶ��Ӣ��
int GameMonitorHS::InitKnowHeroLS(std::string &modelFile, std::string &deployFile, std::string &meanFile, std::string &labelFile)
{
	if (m_binit_KnowHeroLS)
	{
		return 0;
	}
	ModelInfo model;
	//std::string samplePath = "caffemodel/¯ʯ����/";
	//model.modelFile = samplePath + "_iter_1000.caffemodel";
	//model.deployFile = samplePath + "deploy.prototxt";
	//model.meanFile = samplePath + "mean.binaryproto";
	//model.labelFile = samplePath + "label.txt";
	
	model.modelFile = modelFile;
	model.deployFile = deployFile;
	model.meanFile = meanFile;
	model.labelFile = labelFile;
	g_caffeMgr.init(model);//��ʱ���ã��������ʼ��
	g_caffeMgr.initRoleRecognizer(model);
	m_binit_KnowHeroLS = true;

	return 1;
}

//����¯ʯʶ��Ӣ�۵Ļص���
void GameMonitorHS::SetKnowHeroLSCallBack(Callback_KnowHeroLS* pCallback)
{
	m_pCallback_KnowHeroLS = pCallback;
}
