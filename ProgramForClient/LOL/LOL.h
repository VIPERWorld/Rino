// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LOL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LOL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef LOL_EXPORTS
#define LOL_API __declspec(dllexport)
#else
#define LOL_API __declspec(dllimport)
#endif

// �����Ǵ� LOL.dll ������
class LOL_API CLOL {
public:
	CLOL(void);
	// TODO:  �ڴ�������ķ�����
};

extern LOL_API int nLOL;

LOL_API int fnLOL(void);
