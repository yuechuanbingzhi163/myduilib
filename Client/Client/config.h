#pragma once

class Config
{
public:
    static CString ReadIP(LPCTSTR sFile);
	static void WriteIP(LPCTSTR sIP,LPCTSTR sFile);

	static CString ReadPort(LPCTSTR sFile);
	static void WritePort(LPCTSTR sPort,LPCTSTR sFile);

private:

};
