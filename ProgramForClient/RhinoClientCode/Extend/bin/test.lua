print("test.lua -begin\n");
EDataType ={
	BYTE_TYPE = 0,
	UBYTE_TYPE = 1,
	SHORT_TYPE = 2,
	USHORT_TYPE = 3,
	INT_TYPE = 4,
	UINT_TYPE = 5,
	FLOAT_TYPE = 6,
	DOUBLE_TYPE = 7,
	INT64_TYPE = 8,
	UINT64_TYPE = 9,
	STRING_TYPE = 10,			--string���ͣ���С���ó���256���ַ�
	ADDRESS_TYPE = 11			--���ص�ַ����
};

-- ϵͳ��׼��ַ����Ҫƫ����Ϊ����
-- ϵͳͨ��lua���г�ʼ�����㣬������Ϻ�֪ͨ��̨����������Ϣ��ʼ����
-- �������¼�֪ͨԶ�̳����ȡ����ָ���ȡ��Ϣ

LOL_GAME_OFFSET_ADD={
--��Ϸ��Ż�����ַ����Ϸ���Ϊlong���ݣ� �ڴ���Ϣ OFFSET_��Ϸ���  [0x154CCB4]+0x00000060  
	GAME_CODE_BASE_OFFSET = 0x154CCB4,
	GAME_CODE_OFFSET_1 = 0x00000060,
-- �û���ɫ���Ƶ�ַƫ��
	ROLE_OFFSET = 0x154CB20,
--��Ϣ��ָ��
	CENTERHUD_OFFSET = 0x154B9A4,
}

-- �������֮��ĵ�ַ��ϵͳ����ֱ�ӽ��е���
LOL_GAME_ADD ={
-- ��Ϸ���
	GAME_CODE_ADD = 0,
-- �û���
	USER_NAME = LOL_GAME_OFFSET_ADD.ROLE_OFFSET,
-- Ӣ������
	HERO_NAME = 0,
--Ӣ�۵ȼ�
	HERO_LEVEL = 0,
	 
}


--��Ϸ���ȡ��ַ

function LOL_Address_init()
	 
end
-- result = lua_readptr(EDataType.UBYTE_TYPE,0x0049e040);
-- lua_readptr(EDataType.INT_TYPE,result);

print("test.lua -end\n");