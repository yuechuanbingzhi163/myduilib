#include "stdafx.h"

#include "./src/YApp.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	theApp->Run(hInstance,
				hPrevInstance,
				lpCmdLine,
				nCmdShow);

	return 0;
}