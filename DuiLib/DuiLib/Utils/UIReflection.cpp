#include "StdAfx.h"
#include "UIReflection.h"

namespace DuiLib
{

	//************************************
	// 函数名称: RegistDuiClass
	// 返回类型: void
	// 参数信息: string _duiClassName					类名
	// 参数信息: DuiInstanceFun _createDuiInstanceFun	注册的函数指针
	// 函数说明：类反射注册，若已经注册过，则直接返回
	//************************************
#ifdef _UNICODE
	void DuiObjectMap::RegistDuiClass(const wchar_t* _duiClassName, DuiInstanceFun _createDuiInstanceFun)
	{
		if (GetDuiClassMaps().size()>0 && GetDuiClassMaps().find(_duiClassName) != GetDuiClassMaps().end())
			return;

		GetDuiClassMaps()[_duiClassName] = _createDuiInstanceFun;
	}

	//************************************
	// 函数名称: createInstance
	// 返回类型: void*
	// 参数信息: string _duiClassName
	// 函数说明：通过类名构造实例
	//************************************
	void* DuiObjectMap::createInstance(const wchar_t* _duiClassName)
	{
		if (GetDuiClassMaps().size()>0 && GetDuiClassMaps().find(_duiClassName) == GetDuiClassMaps().end())
		{
			std::map<wstring, DuiInstanceFun>& mDuiClassMaps = GetDuiClassMaps();
			DUITRACE(_T("没有找到类对象：%s ,该对象类不是由UI库注册的,请在外部调用CreateControl\r\n"), _duiClassName);
			return NULL;
		}

		return (GetDuiClassMaps()[_duiClassName])();
	}

	//************************************
	// 函数名称: GetDuiClassMaps
	// 返回类型: std::map<string, DuiInstanceFun>&
	// 函数说明: 
	//************************************
	std::map<wstring, DuiInstanceFun>& DuiObjectMap::GetDuiClassMaps()
	{
		static std::map<wstring, DuiInstanceFun> m_duiClassMaps;
		return m_duiClassMaps;
	}
#else
	void DuiObjectMap::RegistDuiClass(const char* _duiClassName, DuiInstanceFun _createDuiInstanceFun)
	{
		if (GetDuiClassMaps().size() > 0 && GetDuiClassMaps().find(_duiClassName) != GetDuiClassMaps().end())
			return;

		GetDuiClassMaps()[_duiClassName] = _createDuiInstanceFun;
	}
	void* DuiObjectMap::createInstance(const char* _duiClassName)
	{
		if (GetDuiClassMaps().size() > 0 && GetDuiClassMaps().find(_duiClassName) == GetDuiClassMaps().end())
		{
			std::map<string, DuiInstanceFun>& mDuiClassMaps = GetDuiClassMaps();
			DUITRACE(_T("没有找到类对象：%s ,该对象类不是由UI库注册的,请在外部调用CreateControl\r\n"), _duiClassName);
			return NULL;
		}

		return (GetDuiClassMaps()[_duiClassName])();
	}
	std::map<string, DuiInstanceFun>& DuiObjectMap::GetDuiClassMaps()
	{
		static std::map<string, DuiInstanceFun> m_duiClassMaps;
		return m_duiClassMaps;
	}
#endif


}

