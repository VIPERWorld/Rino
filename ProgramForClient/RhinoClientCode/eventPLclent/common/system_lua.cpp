
#include "system_lua.h"

  
int lua_import(lua_State *L)
{
	/* �õ��������� */
	int n = lua_gettop(L);
	double sum = 0;
	int i;

	/* ѭ�������֮��  ���ڼ���lua table �����ļ��жϣ���ֹ��ε��룡������������������������ */
	
	for (i = 1; i <= n; i++)
	{
		/* ��� */
		const char* luaFile = lua_tostring(L, i);
		luaL_dofile(L,luaFile);
		 
	}
	/* ���ط���ֵ�ĸ��� */
	return 0;
}
