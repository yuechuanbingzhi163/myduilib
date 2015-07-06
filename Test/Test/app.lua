local Lplus = require "Lplus"

local MainFrame = require "main_frame"

local App = Lplus.Class("App")
do
	local def = App.define 
	def.field(MainFrame).mainFrame = nil
	def.final("=>",App).Instance = function()
		return theApp
	end

	def.method("string").ParseCmdLine = function(self,szCmdLine)
		print("解析命令行：", szCmdLine)
	end

	def.method("userdata","string").Run = function(self,hInst,szCmdLine)
		print("Run App")
		self:ParseCmdLine(szCmdLine)
		local PaintManagerUI = DuiLib.PaintManagerUI
		PaintManagerUI.SetInstance(hInst)
		PaintManagerUI.SetResourcePath(PaintManagerUI.GetInstancePath() .. "skin")
    		PaintManagerUI.SetResourceZip2("ListRes.zip")

		local mainFrame = MainFrame.new(nil)
		mainFrame:CreateWindow("TestDemo",nil)
		assert(mainFrame:IsValid())
		self.mainFrame = mainFrame
		PaintManagerUI.MessageLoop()
	end
	def.method().Exit = function(self)
		self.mainFrame:OnQuitApp()
		self.mainFrame = nil
	end
end
App.Commit()

theApp = App()
return App