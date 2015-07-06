local Lplus = require "Lplus"

local IBaseWindow = Lplus.Class("IBaseWindow")

local def = IBaseWindow.define

def.field(IBaseWindow).m_parent = nil
def.field("userdata").m_hWin = nil
def.field("table").m_msgTable = nil

def.virtual("=>","string").WindowClass = function(self)
	return "IBaseWindow"
end
def.virtual("=>","string").WindowName = function(self)
	return "IBaseWindow"
end
def.virtual("=>","number").WindowStyle = function(self)
	return DuiLib.WNDSTYLE_FRAME
end
def.virtual("=>","number").WindowStyleEx = function(self)
	return DuiLib.Bit.Bor(DuiLib.EX_WINDOWEDGE,DuiLib.EX_OVERLAPPEDWINDOW)
end
def.virtual("=>","table").WindowInitPos = function(self)
	return nil
end
def.virtual("=>","string").SkinFile = function(self)
	return ""
end
def.method("=>",IBaseWindow).GetParent = function(self)
	return self.m_parent
end

def.method("=>","boolean").IsValid = function(self)
	return self.m_hWin and self.m_hWin:IsValid() and not self.m_hWin:isnil() or false
end
def.virtual("dynamic","dynamic","=>","boolean").OnCreate = function(self,wParam,lParam)
	local win = self.m_hWin
	win:ModifyStyle(DuiLib.CAPTION,DuiLib.Bit.Bor(DuiLib.CLIPSIBLINGS,DuiLib.CLIPCHILDREN))
	if not win:RegisterSkin(self:SkinFile()) then
		DuiLib.MsgBox(nil,"Error")
		return false
	end
	win:AddPreMessageFilter()
	win:AddMessageFilter()
	win:AddNotifier()
	return true
end
def.virtual().OnFinalMessage = function(self)
	print("OnFinalMessage")
	self:Release()
end
def.virtual("dynamic","dynamic").OnDestroy = function(self,wParam,lParam)
	print("================","OnDestroy")
	DuiLib.QuitApp()
end
def.virtual().OnQuitApp = function(self)
	print("================","OnQuitApp")
	self:Release()
	DuiLib.QuitApp()
end
def.method().Release = function(self)
	print("valid check",self:IsValid())
	if self.m_hWin and not self.m_hWin:isnil() then
		self.m_hWin:Delete()
	end
	self.m_hWin = nil
end
def.method().TouchMsgTable = function(self)
	local msgt = 
	{
		OnCreate = self:tryget("OnCreate"),
		OnDestroy = self:tryget("OnDestroy"),
		OnFinalMessage = self:tryget("OnFinalMessage"),
		MessageHandler = self:tryget("MessageHandler"),
		ProcessWindowMessage = self:tryget("ProcessWindowMessage"),
		OnNotify = self:tryget("OnNotify"),
		OnUIEvent = self:tryget("OnUIEvent"),
	}
	self.m_msgTable = msgt
	self:MsgTableExtend()
	self.m_hWin:TouchMsgTable(self,self.m_msgTable)
end
def.virtual().MsgTableExtend = function(self)	
end
def.method("string","userdata","=>","boolean").CreateWindow = function(self,name,parent)
	print("CreateWindow")
	local win = DuiLib.LuaWindow.New(self:WindowClass())
	self.m_hWin = win
	self:TouchMsgTable()
	win:Create(parent,self:WindowName(),self:WindowStyle(),self:WindowStyleEx(),0,0,600,320)
	assert(win:IsValid())
	if win:IsValid() then
		win:ShowWindow()
		return true
	else
		win:MsgBox("Create Error","Error")
		return false
	end
end
def.virtual("number","dynamic","dynamic").MessageHandler = function(self,uMsgId,wParam,lParam)
end


def.virtual("userdata").OnUIDestroy = function(self,event)
	print("OnUIDestroy",event:GetClass() ,event:IsClass("ListTextElementUI"))
end

def.virtual("userdata","=>","boolean").OnUIEvent = function(self,event)
	--print("OnUIEvent",event)
	return true
end

def.virtual("userdata").OnNotify = function(self,msg)
end

IBaseWindow.Commit()


return IBaseWindow