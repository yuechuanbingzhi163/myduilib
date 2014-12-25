#ifndef UIReflection_h__
#define UIReflection_h__

#include<string>
#include<map>
using namespace std;

namespace DuiLib
{
	typedef void* (*DuiInstanceFun)();
	class UILIB_API DuiObjectMap
	{
	private:
		DuiObjectMap(){};
	public:
		
#ifdef _UNICODE
		static void RegistDuiClass(const wchar_t* _duiClassName, DuiInstanceFun _createDuiInstanceFun);
		static void* createInstance(const wchar_t* _duiClassName);
		static std::map<wstring, DuiInstanceFun>& GetDuiClassMaps();
#else
		static void RegistDuiClass(const char* _duiClassName, DuiInstanceFun _createDuiInstanceFun);
		static void* createInstance(const char* _duiClassName);
		static std::map<string, DuiInstanceFun>& GetDuiClassMaps();
#endif
	};

	template<typename T>
	class UILIB_API DelegatingObject 
	{
	public:
		DelegatingObject(const char* _duiClassName)
		{
#ifdef _UNICODE
			CDuiString sChange;
			sChange.AssignChar(_duiClassName);
			DuiObjectMap::RegistDuiClass(sChange.GetData(), (DuiInstanceFun)&(DelegatingObject::CreateClassInstance));
#else
			DuiObjectMap::RegistDuiClass(_duiClassName, (DuiInstanceFun)&(DelegatingObject::CreateClassInstance));
#endif // 

			
		}

		static T* CreateClassInstance()
		{
			return static_cast<T*>(new T);
		}
	};


#ifdef _UNICODE
	template<class TObj>
	TObj CreateDuiInstance(const wchar_t* _DuiClassName){
		return static_cast<TObj>(DuiObjectMap::createInstance(_DuiClassName));
	}
#else
	template<class TObj>
	TObj CreateDuiInstance(const char* _DuiClassName){
		return static_cast<TObj>(DuiObjectMap::createInstance(_DuiClassName));
	}
#endif // !_UNICODE

#ifndef REGIST_DUICLASS
#define REGIST_DUICLASS(DuiObject) DelegatingObject<DuiObject> __class_##DuiObject( #DuiObject );
#endif
}

#endif // UIReflection_h__