#ifndef WJ_ENCRYPT
#define WJ_ENCRYPT

#define ENCRYPT_KEY 729520  // ��������
#define FILE_NAME_MAX_SIZE 1000  // �ļ������� 

// �任����������Դ���ܣ���֪�ļ���С
static int genFun(int size, int key, int i)
{
	return size + key + i - size / key;
}

// �任����������־���ܣ���ȷ���ļ���С
static int genFunEx(int key)
{
	return key;
}

#endif//WJ_ENCRYPT