#ifndef _LuaControlInclude_h
#define _LuaControlInclude_h
#pragma once

#include "LuaCControlUI.h"
#include "LuaCContainerUI.h"
#include "LuaCHorizontalLayoutUI.h"
#include "LuaCVerticalLayoutUI.h"
#include "LuaCTabLayoutUI.h"
#include "LuaCTileLayoutUI.h"
#include "LuaCChildLayoutUI.h"
#include "LuaCAnimationTabLayoutUI.h"
#include "LuaCActiveXUI.h"
#include "LuaCComboUI.h"
#include "LuaCComboBoxUI.h"
#include "LuaCLabelUI.h"
#include "LuaCTextUI.h"
#include "LuaCDateTimeUI.h"
#include "LuaCProgressUI.h"
#include "LuaCButtonUI.h"
#include "LuaCOptionUI.h"
#include "LuaCRadioUI.h"
#include "LuaCCheckBoxUI.h"
#include "LuaCFadeButtonUI.h"
#include "LuaCFadeOptionUI.h"
#include "LuaCFadeCheckBoxUI.h"
#include "LuaCAlbumButtonUI.h"
#include "LuaCAnimControlUI.h"
#include "LuaCEditUI.h"
#include "LuaCFlashUI.h"
#include "LuaCGifAnimUI.h"
#include "LuaCHotKeyUI.h"
#include "LuaCSeqButtonUI.h"
#include "LuaCSliderUI.h"
#include "LuaCScrollBarUI.h"
#include "LuaCListUI.h"
#include "LuaCTreeViewUI.h"
#include "LuaCWebBrowserUI.h"
#include "LuaCWkeWebkitUI.h"

namespace DuiLib
{
	class RegDuiControls2Lua
	{
	public:
		static void RegModule(lua_State* l)
		{
			REG_MODULE(LuaCControlUI)
			REG_MODULE(LuaCContainerUI)
			REG_MODULE(LuaCHorizontalLayoutUI)
			REG_MODULE(LuaCVerticalLayoutUI)
			REG_MODULE(LuaCTileLayoutUI)
			REG_MODULE(LuaCChildLayoutUI)
			REG_MODULE(LuaCTabLayoutUI)
			REG_MODULE(LuaCAnimationTabLayoutUI)
			REG_MODULE(LuaCActiveXUI)
			REG_MODULE(LuaCComboUI)
			REG_MODULE(LuaCComboBoxUI)
			REG_MODULE(LuaCLabelUI)
			REG_MODULE(LuaCTextUI)
			REG_MODULE(LuaCDateTimeUI)
			REG_MODULE(LuaCProgressUI)
			REG_MODULE(LuaCButtonUI)
			REG_MODULE(LuaCOptionUI)
			REG_MODULE(LuaCRadioUI)
			REG_MODULE(LuaCCheckBoxUI)
			REG_MODULE(LuaCFadeButtonUI)
			REG_MODULE(LuaCFadeOptionUI)
			REG_MODULE(LuaCFadeCheckBoxUI)
			REG_MODULE(LuaCAlbumButtonUI)
			REG_MODULE(LuaCAnimControlUI)
			REG_MODULE(LuaCEditUI)
			REG_MODULE(LuaCFlashUI)
			REG_MODULE(LuaCGifAnimUI)
			REG_MODULE(LuaCHotKeyUI)
			REG_MODULE(LuaCSeqButtonUI)
			REG_MODULE(LuaCSliderUI)
			REG_MODULE(LuaCScrollBarUI)
			REG_MODULE(LuaCListUI)
			REG_MODULE(LuaCListBodyUI)
			REG_MODULE(LuaCListHeaderUI)
			REG_MODULE(LuaCListHeaderItemUI)
			REG_MODULE(LuaCListElementUI)
			REG_MODULE(LuaCListLabelElementUI)
			REG_MODULE(LuaCListTextElementUI)
			REG_MODULE(LuaCListContainerElementUI)
			REG_MODULE(LuaCTreeNodeUI)
			REG_MODULE(LuaCTreeViewUI)
			REG_MODULE(LuaCWebBrowserUI)
			REG_MODULE(LuaCWkeWebkitUI)
		}
	};
}


#endif//_LuaControlInclude_h