
function is_window_exist(title)
    if Inf_GetWindowHwnd(WINDOW_TITLE_HS_GAME) > 0 then
        return true;
    else
        return false;
    end
end

function startup_hs()
    print("����¯ʯ��˵");
    -- �ж���Ϸ�Ƿ����������û�У��ȴ��û���¼ս������¯ʯ��˵
    if is_window_exist(WINDOW_TITLE_HS_GAME) then
        -- �ѽ�����Ϸ
        return WJAM_RES_OK;
    else
        -- δ������Ϸ
        if not is_window_exist(WINDOW_TITLE_HS_BATTLENET) then
            -- ս��δ�������ȴ��û�����ս��
            if not is_window_exist(WINDOW_TITLE_HS_BATTLENET) then
                -- Inf_StartUp()
            end
        else
            -- ս��������

        end
    end
end

function start_game()

    local isEnd = false;
    local step = 0;

    while
        not isEnd
    do
        if (0 == step) then
            -- ��Ӻ���
        elseif (1 == step) then
        elseif (2 == step) then
        elseif (3 == step) then
        end
    end
end


function startup_hs()
    print("����¯ʯ��˵")
end



function mouse_click_wait(hWnd, x, y, seconds)
    print("������� (", x, ",", y, ")");
    mouseClick(hWnd, x, y, 0);
    Sleep(seconds);
end


KEY_CODE_ENTER = 13 

-- ������Ӻ�������
function reqAddFriend(hWnd, player)
    -- ������½ǰ�ť
    mouse_click_wait(hWnd, 0.03, 0.97, 100);
    -- �����Ӱ�ť
    mouse_click_wait(hWnd, 0.05, 0.90, 100);
    -- �����û���
    print("[¯ʯ] ��ʼ��Ϸ, �����ǣ�", player);
    Inf_Input(hWnd, player)
    -- ���س���
    Inf_PressKey(hWnd, KEY_CODE_ENTER)

    -- ������½ǰ�ť
    mouse_click_wait(hWnd, 0.03, 0.97, 100);
end

-- ���ܺ����������
function acceptFriendReq(hWnd)
    -- ������½ǰ�ť
    mouse_click_wait(hWnd, 0.03, 0.97, 100);
    
    -- ��������ȷ�ϰ�ť
    mouse_click_wait(hWnd, 0.157, 0.4454, 100);

end

function reqChallenge(hWnd)
    -- ������ѹ���ť
    mouse_click_wait(hWnd, 0.03, 0.97, 100);

    -- ���������ս��ť
    mouse_click_wait(hWnd, 0.18, 0.537, 100);

    -- ѡ����սģʽ
    mouse_click_wait(hWnd, 0.2835, 0.54, 100);
end

function acceptChallenge(hWnd)
    -- ���������ս��ť
    mouse_click_wait(hWnd, 0.55, 0.668, 100);
end




function play_hs(player)
    print("[¯ʯ] ��ʼ��Ϸ, �����ǣ�", player);

    --    if nil == player then
    --        print("Hello World");
    --        return;

    local hWnd = Inf_GetWindowHwnd(WINDOW_TITLE_HS_GAME)
    Inf_ActiveWindow(WINDOW_TITLE_HS_GAME)

    if hWnd > 0 then
        --reqAddFriend(hWnd, "Ҷ��ļ���#51207");
        --acceptFriendReq(hWnd);
        reqChallenge(hWnd);
        --acceptChallenge(hWnd);
        --mouse_click_wait(hWnd, 0.5, 0.36, 100);
        -- mouse_click_wait(hWnd, 0.03, 0.93, 100);
        -- mouse_click_wait(hWnd, 0.20, 0.50, 100);
        -- mouse_click_wait(hWnd, 0.30, 0.50, 100);
        -- mouse_click_wait(hWnd, 0.5, 0.295, 1000);	--��սģʽ
        -- --mouse_click_wait(hWnd, 0.5, 0.375, 1000);  --ð��ģʽ
        -- mouse_click_wait(hWnd, 0.78, 0.78, 1000);
        -- mouse_click_wait(hWnd, 0.78, 0.78, 1000);
        -- mouse_click_wait(hWnd, 0.74, 0.79, 1000);
        -- mouse_click_wait(hWnd, 0.74, 0.79, 1000);
        -- mouse_click_wait(hWnd, 0.8, 0.16, 1000);
        -- mouse_click_wait(hWnd, 0.92, 0.87, 1000);
        -- mouse_click_wait(hWnd, 0.92, 0.87, 1000);
    end
end