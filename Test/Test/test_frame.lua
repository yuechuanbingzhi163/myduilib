
module(..., package.seeall)

function OnCreate(win,msgid,wparam,lparam)
	win:ModifyStyle(DuiLib.CAPTION,Bit.Bor(DuiLib.CLIPSIBLINGS,DuiLib.CLIPCHILDREN))
	if not win:RegisterSkin("skin.xml") then
		DuiLib.MsgBox(nil,"Error")
		return false
	end
	win:AddPreMessageFilter()
	win:AddMessageFilter()
	win:AddNotifier()
	return true
end
local index = 0
function OnSearch(win)
	local pList = win:FindControl("domainlist")
	pList:RemoveAll()
	win:SetListCallback(pList)
	-- for i=1,100 do
	-- 	local pControl = ListTextElementUI.New()
	-- 	pControl:SetTag(i)
	-- 	win:PostMessage(1724,0,pControl)
	-- end
	index = 0
	local id = win:SetTimer(100,1)
end

function OnNotify(win,msg)
	local msgType = msg:getType()
	local sender = msg:getSender()
	if sender:isnil() then return end
	print("OnNotify",win,sender:GetClass(),msg:getType())
	if msgType == "click" then
		if sender:IsName("closebtn") then
			win:MsgBox("确定退出吗？","警告",0x00000030,function(ret)
				if ret == 1 then
					win:PostMessage(0x0012,0,0)
				end
			end)
		elseif sender:IsName("maxbtn") then
			win:SendMessage(MsgArgs.WM_SYSCOMMAND, DuiLib.SC_MAXIMIZE, 0)
			return;
		elseif sender:IsName("minbtn") then
			win:SendMessage(MsgArgs.WM_SYSCOMMAND, DuiLib.SC_MINIMIZE, 0)
			print("ddddddddddddddddd")
			return;
		elseif sender:IsName("btn") then
			OnSearch(win)
		end
	elseif msgType == "windowinit" then
		local btn = win:FindControl("btn")
		win:ListenUIEvent(btn)
		print("-----------",btn:IsClass("ButtonUI"))
	elseif msgType == "itemactivate" then
		local iIndex = sender:GetTag()
		local sMessage = "Click: "
		DuiLib.MsgBox(win,sMessage .. iIndex,"提示(by ldf)",0)
	end
end

function ProcessWindowMessage(win,msgid,wparam,lparam)
	--print("ProcessWindowMessage",win,msgid,wparam,lparam)
	--print(DuiLib.TraceMsg(msgid))
	if msgid == MsgArgs.WM_SYSCOMMAND then
		print(wparam,lparam)
	end
	if msgid == 1724 then
		OnAddListItem(win,msgid,wparam,lparam)
	elseif msgid == MsgArgs.WM_TIMER then
		OnTimer(win,msgid,wparam,lparam);
	end
end

function OnAddListItem(win,msgid,wParam,lParam)
	local pListElement = win:ControlFromPtr(lParam)
	--print(pListElement,lParam,win)
	local pList = win:FindControl("domainlist")
	pList:Add(pListElement)
	pList:EndDown()
	return 0
end

function GetItemText(win,pControl,iIndex,iSubItem)
	local szBuf = ""
    if iSubItem == 0 then
    	szBuf = "" .. iIndex
    elseif iSubItem ==  1 then
    	szBuf = "" .. "www.baidu.com"
    else
    	szBuf = "" .. "China"
    end
    pControl:SetUserData(szBuf);
end


function OnTimer(win,msgid,wParam,lParam)
	if wParam == 100 then
		if index <100 then
			local pControl = ListTextElementUI.New()
			pControl:SetTag(index)
			win:ListenUIDestroy(pControl)
			win:PostMessage(1724,0,pControl)
		end
		index = index + 1
	end
end

function OnUIEvent(win,event)
	--print("OnUIEvent",event)
	return true
end

function OnUIDestroy(win,event)
	print("OnUIDestroy",event:IsClass("ListTextElementUI"))
end
