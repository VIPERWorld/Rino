
base_internal_path = {}

-- ������ַ
base_internal_path["GameCodeBase"] = "0x5E3C60"	--��Ϸ���
base_internal_path["KillTowerBase1"] = "0x1A1CBF0"	--����������ַ
base_internal_path["KillTowerBase2"] = "0x1A2309C"	--�Ƿ���������ַ
base_internal_path["GameResultBase"] = "0x1A226A4" --��Ϸ���
base_internal_path["TimeStartBase"] = "0x36A3AA0" --ʱ�����
base_internal_path["PlayBase"] = "0x1A1D794" --��ҽ�ɫ����
base_internal_path["CenterHudBase"] = "0x1A22EC8" --��ɫ��Ϣ
base_internal_path["g_ExitGameCall"] = "0xA9E140" --�˳���Ϸ



--��1����Ϸ���
function FunGetValue_1()
	local res = GetGameKey(base_internal_path["GameCodeBase"], 0x70)
return res
end

--��2����������������
function FunGetValue_2()
	local res = base_internal_path["KillTowerBase1"]
return res
end

--��3�����������Ƿ�����
function FunGetValue_3()
	local res = base_internal_path["KillTowerBase2"]
return res
end

--��4����Ϸ���
function FunGetValue_4()
	local res = base_internal_path["GameResultBase"]
return res
end

--��5����ʼʱ��
function FunGetValue_5()
	local res = fun_offset(base_internal_path["TimeStartBase"], 0x18)
return res
end

--��6����ҽ�ɫ����
function FunGetValue_6()
	local res = base_internal_path["PlayBase"]
return res
end

--��7����ɱ��
function FunGetValue_7()
	local res = fun_offset(base_internal_path["CenterHudBase"], 0x5c)
	res = fun_offset(res, 4)
	res = fun_offset(res, 0)
	res = fun_offset(res, 0)
	res = fun_offset(res, 0x24)
return res
end

--��8��������
function FunGetValue_8()
	local res = fun_offset(base_internal_path["CenterHudBase"], 0x5c)
	res = fun_offset(res, 4)
	res = fun_offset(res, 0)
	res = fun_offset(res, 0)
	res = fun_offset(res, 0x28)
return res
end

--��9��������
function FunGetValue_9()
	local res = fun_offset(base_internal_path["CenterHudBase"], 0x5c)
	res = fun_offset(res, 4)
	res = fun_offset(res, 0)
	res = fun_offset(res, 0)
	res = fun_offset(res, 0x2c)
return res
end

--��10��������
function FunGetValue_10()
	local res = fun_offset(base_internal_path["CenterHudBase"], 0x5c)
	res = fun_offset(res, 4)
	res = fun_offset(res, 0)
	res = fun_offset(res, 4)
	res = fun_offset(res, 0x24)
return res
end

--��11���˳���Ϸ
function FunGetValue_11()
	return base_internal_path["g_ExitGameCall"];
end