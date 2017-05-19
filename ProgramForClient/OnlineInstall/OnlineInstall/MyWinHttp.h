
#ifndef WJ_HttpClient_h
#define WJ_HttpClient_h

#define HTTP_WEB_ADDRESS_SERVER	L"118.178.25.88"
#define HTTP_WEB_ADDRESS_SERVER2	L"api.xiniugame.com"/*L"118.178.25.88"*/

class CMainWnd;

class CMyWinHttp
{
public:
	CMyWinHttp(void);
	~CMyWinHttp(void);

	//֪ͨ���������ذ�װ�ɹ�
	void tellHttpServerDownFinish(int& iSource);
	//֪ͨ���������ذ�װ�ɹ�_����
	void tellHttpServerDownFinish_test(char* sHDSerial);
	//��ȡ��ǰ���°汾���ص�ַ
	void GetHttpDownAddress(std::string &str_res);

	void SetpCMainWnd(CMainWnd* pCMainWnd){ m_pCMainWnd = pCMainWnd; }
private:
	void GetHttpData(std::wstring str_web_addr, int iport, std::wstring str_web_content, std::string &str_res);

	CMainWnd* m_pCMainWnd;
};

#endif//WJ_HttpClient_h