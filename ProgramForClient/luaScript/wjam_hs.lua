
function is_window_exist(title)
    if Inf_GetWindowHwnd(WINDOW_TITLE_HS_GAME) > 0 then
        return true;
    else
        return false;
    end
end


function mouse_click_wait(hWnd, x, y, seconds)
    print("--LUA--:  ������� (", x, ",", y, ")");
    Inf_MouseClickLButton1(hWnd, x, y, 1)
    Sleep(seconds);
end


KEY_CODE_ENTER = 13 

function startup_hs()
    local hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_HS_GAME);
    local count = 0;

    while
        (hWnd <= 0)
    do
        print("--LUA--:  ��Ϸ���ڲ�����", count);
        Sleep(1000)
        count = count + 1;
        hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_HS_GAME);
    end

    local tick = 3;
    print("--LUA--:  �����Ϸ���ھ��,", tick, "���ʼע��DLL ==>  ", WINDOW_TITLE_HS_GAME, EXTEND_DLL_HS);
    while
        tick > 0
    do
        print("--LUA--:  �ȴ�......", tick, "��");
        Sleep(1000);
        tick = tick - 1;
    end

    print("--LUA--:  ��ʼִ��ע��:BEGIN");
    Lua_InjectDll(WINDOW_TITLE_HS_GAME, EXTEND_DLL_HS);

    Inf_ActiveWindow(WINDOW_TITLE_HS_GAME);
    --mouse_click_wait(hWnd, 0.5, 0.2, 100);
    print("--LUA--:  ��ʼִ��ע��:END");
end


-- ������Ӻ�������
function reqAddFriend(hWnd, player)
    
    print("--LUA--:  ��Ӻ��ѣ�", player);

    -- ������Ϸ����
    Inf_ActiveWindow(WINDOW_TITLE_HS_GAME);
     Sleep(500);

    -- ������½ǰ�ť
    -- mouse_click_wait(hWnd, 0.03, 0.98, 100);

    -- �����Ӱ�ť
    local x, y, w, h = Inf_GetHS_FriendWindowRect();

   
    mouse_click_wait(hWnd, x + w * 0.24, y + h * 0.935, 100);

    -- �����û���

    Inf_Input(hWnd, player)

    -- ���س���
    Inf_PressKey(hWnd, KEY_CODE_ENTER)
    Sleep(100);

    -- ����հ׵ط���ť
    --mouse_click_wait(hWnd, 0.5, 0.95, 1000);
end

-- ���ܺ����������
function acceptFriendReq(hWnd)
    Inf_ActiveWindow(WINDOW_TITLE_HS_GAME);

    -- ������½ǰ�ť
    -- mouse_click_wait(hWnd, 0.03, 0.97, 100);

    local x, y, w, h = Inf_GetHS_FriendWindowRect();

    -- ��������ȷ�ϰ�ť
    mouse_click_wait(hWnd, x + w * 0.7, y + h * 0.23, 100);
	Sleep(1000);
	
	print("================");
    -- ����հ׵ط�
    mouse_click_wait(hWnd, 0.5, 0.95, 100);
	Sleep(1000);
end

function reqChallenge(hWnd, x, y)
    Inf_ActiveWindow(WINDOW_TITLE_HS_GAME);
    Sleep(1000);
   
    -- �����սģʽѡ��ť
	mouse_click_wait(hWnd, x, y, 1000);

    -- �����սģʽѡ��ť
	mouse_click_wait(hWnd, x + 120, y, 1000);

    -- ����հ׵ط�
    mouse_click_wait(hWnd, 0.5, 0.95, 1000);
end

function acceptChallenge(hWnd)
    Inf_ActiveWindow(WINDOW_TITLE_HS_GAME);
    Sleep(1000);
   
    -- ���������ս��ť
    mouse_click_wait(hWnd, 0.45, 0.62, 100);

    -- ����հ׵ط�
    mouse_click_wait(hWnd, 0.5, 0.95, 1000);
end
