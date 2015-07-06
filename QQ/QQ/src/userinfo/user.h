#ifndef _user_h_
#define _user_h_

enum UserState
{	
	US_OFFLINE,//离线
	US_ONLINE,//在线
	US_HIDE,//隐身
	US_AWAY,//离开
	US_BUSY,//忙碌
	US_MUTEX//免打扰
};

#define MAX_BUFSIZE 256
//定义用户结构图
typedef struct UserInfo
{
	char _id[MAX_BUFSIZE];//用户Id号
	char _passwd[MAX_BUFSIZE];//用户密码
	char _nickname[MAX_BUFSIZE];//用户昵称
	char _logo[MAX_BUFSIZE];//用户logo
	char _signature[MAX_BUFSIZE*2];//个性签名
	UserState _state;//用户的状态
	long _lastlogin;//上次登录时间
	char _ip[MAX_BUFSIZE];//用户ip
	unsigned long _port;//端口
	UserInfo()
	{
		memset(_id,0,MAX_BUFSIZE);
		memset(_passwd,0,MAX_BUFSIZE);
		memset(_nickname,0,MAX_BUFSIZE);
		memset(_logo,0,MAX_BUFSIZE);
		memset(_signature,0,MAX_BUFSIZE*2);
		memset(_ip,0,MAX_BUFSIZE);
		_state = US_OFFLINE;
		_lastlogin = 0;
		_port = 0;
	}
	UserInfo& operator = (const UserInfo& other)
	{
		memset(_id,0,MAX_BUFSIZE);
		memset(_passwd,0,MAX_BUFSIZE);
		memset(_nickname,0,MAX_BUFSIZE);
		memset(_logo,0,MAX_BUFSIZE);
		memset(_signature,0,MAX_BUFSIZE*2);
		memset(_ip,0,MAX_BUFSIZE);

		memcpy(_id,other._id,strlen(other._id));
		memcpy(_passwd,other._passwd,strlen(other._passwd));
		memcpy(_nickname,other._nickname,strlen(other._nickname));
		memcpy(_logo,other._logo,strlen(other._logo));
		memcpy(_signature,other._signature,strlen(other._signature));
		memcpy(_ip,other._ip,strlen(other._ip));
		_state = other._state;
		_lastlogin = other._lastlogin;
		_port = other._port;
	
		return (*this);
	}
	void IP(const char* ip)
	{
		assert(ip);
		memset(_ip,0,MAX_BUFSIZE);
		memcpy(_ip,ip,strlen(ip));
	}
	void ID(const char* id)
	{
		assert(id);
		memset(_id,0,MAX_BUFSIZE);
		memcpy(_id,id,strlen(id));
	}
	void PSD(const char* psd)
	{
		assert(psd);
		memset(_passwd,0,MAX_BUFSIZE);
		memcpy(_passwd,psd,strlen(psd));
	}
	void NickName(const char* nickname)
	{
		assert(nickname);
		memset(_nickname,0,MAX_BUFSIZE);
		memcpy(_nickname,nickname,strlen(nickname));
	}
	void Logo(const char* logo)
	{
		assert(logo);
		memset(_logo,0,MAX_BUFSIZE);
		memcpy(_logo,logo,strlen(logo));
	}
	void Signature(const char* sig)
	{
		assert(sig);
		memset(_signature,0,MAX_BUFSIZE*2);
		memcpy(_signature,sig,strlen(sig));
	}
}USERINFO,*LPUSERINFO;





#endif//_user_h_