// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� ORCIMPL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// ORCIMPL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
//#ifdef ORCIMPL_EXPORTS
//#define ORCIMPL_API __declspec(dllexport)
//#else
//#define ORCIMPL_API __declspec(dllimport)
//#endif

// �����Ǵ� orcimpl.dll ������
class Corcimpl {
public:
	Corcimpl(void);
	// TODO:  �ڴ�������ķ�����
};

extern int norcimpl;

int fnorcimpl(void);
bool ocrinit(char* _path, char* _name);
char* ocrText(void* data,int width,int height,int pixSize);
char* ocrText(char* fileName);