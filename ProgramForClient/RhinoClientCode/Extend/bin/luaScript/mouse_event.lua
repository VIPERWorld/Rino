
-- �����
function mouseClick(hWnd, x, y, isAbs)
    Inf_MouseClickLButton(hWnd,x, y, isAbs)
    Sleep(100)
end

-- ���˫��
function mouseDoubleClick(hWnd, x, y, isAbs)
    Inf_MouseClickLButton(hWnd,x, y, isAbs)
    Inf_MouseClickLButton(hWnd,x, y, isAbs)
    Sleep(100)
end


        
-- �������
function mouseThreeClick(hWnd, x, y, isAbs)
    Inf_MouseClickLButton(hWnd, x, y, isAbs)
    Inf_MouseClickLButton(hWnd, x, y, isAbs)
    Inf_MouseClickLButton(hWnd, x, y, isAbs)
    Sleep(100)
end


--function startup_hs()
--    print("����¯ʯ��˵");
--end
