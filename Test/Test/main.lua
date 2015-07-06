
package.path = './?.lua;' .. package.path

_G.theApp = nil

require "app"

function main(hInstance,hPrevInstance,lpCmdLine,nCmdShow)
	print("main",hInstance)
	theApp:Run(hInstance,lpCmdLine)
end

function exit()
	print("exit app")
	theApp:Exit()
	theApp = nil
end
--main()
