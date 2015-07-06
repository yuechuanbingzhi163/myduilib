#ifndef _UICRACK_H_
#define _UICRACK_H_

#ifdef _MSC_VER
#pragma once
#endif

namespace DuiLib {

/////////////////////////////////////////////////////////////////////////////////////
//
//
// constrol class name and interface name

// UIActiveX
UILIB_API  extern const TCHAR* const kUIActiveXClassName;// = _T("ActiveXUI");
UILIB_API  extern const TCHAR* const kUIActiveXInterfaceName;// = _T("ActiveX");

// ComboUI
UILIB_API  extern const TCHAR* const kComboUIClassName;// = _T("ComboUI");
UILIB_API  extern const TCHAR* const kComboUIInterfaceName;// = _T("Combo");

// LabelUI
UILIB_API  extern const TCHAR* const kLabelUIClassName;// = _T("LabelUI");
UILIB_API  extern const TCHAR* const kLabelUIInterfaceName;// = _T("Label");


// ButtonUI
UILIB_API  extern const TCHAR* const kButtonUIClassName;// = _T("ButtonUI");
UILIB_API  extern const TCHAR* const kButtonUIInterfaceName;// = _T("Button");

// OptionUI
UILIB_API  extern const TCHAR* const kOptionUIClassName;// = _T("OptionUI");
UILIB_API  extern const TCHAR* const kOptionUIInterfaceName;// = _T("Option");

// TextUI
UILIB_API  extern const TCHAR* const kTextUIClassName;// = _T("TextUI");
UILIB_API  extern const TCHAR* const kTextUIInterfaceName;// = _T("Text");

// ProgressUI
UILIB_API  extern const TCHAR* const kProgressUIClassName;// = _T("ProgressUI");
UILIB_API  extern const TCHAR* const kProgressUIInterfaceName;// = _T("Progress");

// SliderUI
UILIB_API  extern const TCHAR* const kSliderUIClassName;// = _T("SliderUI");
UILIB_API  extern const TCHAR* const kSliderUIInterfaceName;// = _T("Slider");

// FlashUI
UILIB_API  extern const TCHAR* const kFlashUIClassName;// = _T("FlashUI");
UILIB_API  extern const TCHAR* const kFlashUIInterfaceName;// = _T("Flash");

// EditUI
UILIB_API  extern const TCHAR* const kEditUIClassName;// = _T("EditUI");
UILIB_API  extern const TCHAR* const kEditUIInterfaceName;// = _T("Edit");

// IEditUI
UILIB_API  extern const TCHAR* const kIEditUIInterfaceName;// = _T("Edit");

// ScrollBarUI
UILIB_API  extern const TCHAR* const kScrollBarUIClassName;// = _T("ScrollBarUI");
UILIB_API  extern const TCHAR* const kScrollBarUIInterfaceName;// = _T("ScrollBar");

// ContainerUI
UILIB_API  extern const TCHAR* const kContainerUIClassName;// = _T("ContainerUI");
UILIB_API  extern const TCHAR* const kContainerUIInterfaceName;// = _T("Container");

// IContainerUI
UILIB_API  extern const TCHAR* const kIContainerUIInterfaceName;// = _T("IContainer");

// VerticalLayoutUI
UILIB_API  extern const TCHAR* const kVerticalLayoutUIClassName;// = _T("VerticalLayoutUI");
UILIB_API  extern const TCHAR* const kVerticalLayoutUIInterfaceName;// = _T("VerticalLayout");

// HorizontalLayoutUI
UILIB_API  extern const TCHAR* const kHorizontalLayoutUIClassName;// = _T("HorizontalLayoutUI");
UILIB_API  extern const TCHAR* const kHorizontalLayoutUIInterfaceName;// = _T("HorizontalLayout");

// TileLayoutUI
UILIB_API  extern const TCHAR* const kTileLayoutUIClassName;// = _T("TileLayoutUI");
UILIB_API  extern const TCHAR* const kTileLayoutUIInterfaceName;// = _T("TileLayout");

// DialogLayoutUI
UILIB_API  extern const TCHAR* const kDialogLayoutUIClassName;// = _T("DialogLayoutUI");
UILIB_API  extern const TCHAR* const kDialogLayoutUIInterfaceName;// = _T("DialogLayout");

// TabLayoutUI
UILIB_API  extern const TCHAR* const kTabLayoutUIClassName;// = _T("TabLayoutUI");
UILIB_API  extern const TCHAR* const kTabLayoutUIInterfaceName;// = _T("TabLayout");

// ControlUI
UILIB_API  extern const TCHAR* const kControlUIClassName;// = _T("ControlUI");
UILIB_API  extern const TCHAR* const kControlUIInterfaceName;// = _T("Control");

// ListUI
UILIB_API  extern const TCHAR* const kListUIClassName;// = _T("ListUI");
UILIB_API  extern const TCHAR* const kListUIInterfaceName;// = _T("List");

// IListUI
UILIB_API  extern const TCHAR* const kIListUIInterfaceName;// = _T("IList");

// IListOwnerUI
UILIB_API  extern const TCHAR* const kIListOwnerUIInterfaceName;// = _T("IListOwner");

// ListHeaderUI
UILIB_API  extern const TCHAR* const kListHeaderUIClassName;// = _T("ListHeaderUI");
UILIB_API  extern const TCHAR* const kListHeaderUIInterfaceName;// = _T("ListHeader");

// ListHeaderItemUI
UILIB_API  extern const TCHAR* const kListHeaderItemUIClassName;// = _T("ListHeaderItemUI");
UILIB_API  extern const TCHAR* const kListHeaderItemUIInterfaceName;// = _T("ListHeaderItem");

// ListElementUI
UILIB_API  extern const TCHAR* const kListElementUIClassName;// = _T("ListElementUI");
UILIB_API  extern const TCHAR* const kListElementUIInterfaceName;// = _T("ListElement");

// IListItemUI
UILIB_API  extern const TCHAR* const kIListItemUIInterfaceName;// = _T("ListItem");

// ListLabelElementUI
UILIB_API  extern const TCHAR* const kListLabelElementUIClassName;// = _T("ListLabelElementUI");
UILIB_API  extern const TCHAR* const kListLabelElementUIInterfaceName;// = _T("ListLabelElement");

// ListTextElementUI
UILIB_API  extern const TCHAR* const kListTextElementUIClassName;// = _T("ListTextElementUI");
UILIB_API  extern const TCHAR* const kListTextElementUIInterfaceName;// = _T("ListTextElement");

// ListExpandElementUI
//UILIB_API  extern const TCHAR* const kListExpandElementUIClassName;// = _T("ListExpandElementUI");
//UILIB_API  extern const TCHAR* const kListExpandElementUIInterfaceName;// = _T("ListExpandElement");

// ListContainerElementUI
UILIB_API  extern const TCHAR* const kListContainerElementUIClassName;// = _T("ListContainerElementUI");
UILIB_API  extern const TCHAR* const kListContainerElementUIInterfaceName;// = _T("ListContainerElement");

// RichEditUI
UILIB_API  extern const TCHAR* const kRichEditUIClassName;// = _T("RichEditUI");
UILIB_API  extern const TCHAR* const kRichEditUIInterfaceName;// = _T("RichEdit");

// WbkWebkitUI
UILIB_API  extern const TCHAR* const kWkeWebkitUIClassName; // = _T("WbkWebkitUI");
UILIB_API  extern const TCHAR* const kWkeWebkitUIInterfaceName; // = _T("WbkWebkit");
/////////////////////////////////////////////////////////////////////////////////////
//
//
// control related message
UILIB_API  extern const TCHAR* const kWindowInit;// = _T("windowinit");
UILIB_API  extern const TCHAR* const kClick;// = _T("click");
UILIB_API  extern const TCHAR* const kDBClick;// = _T("dbclick");
UILIB_API  extern const TCHAR* const kSelectChanged;// = _T("selectchanged");
UILIB_API  extern const TCHAR* const kItemSelect;// = _T("itemselect");
UILIB_API  extern const TCHAR* const kItemActivate;// = _T("itemactivate");
UILIB_API  extern const TCHAR* const kItemClick;// = _T("itemclick");
UILIB_API  extern const TCHAR* const kDropDown;// = _T("dropdown");
UILIB_API  extern const TCHAR* const kTimer;// = _T("timer");
UILIB_API  extern const TCHAR* const kMenu;// = _T("menu");
UILIB_API  extern const TCHAR* const kReturn;// = _T("return");
UILIB_API  extern const TCHAR* const kTextChanged;// = _T("textchanged");
UILIB_API  extern const TCHAR* const kKillFocus; // = _T("killfocus");
UILIB_API  extern const TCHAR* const kSetFocus; // = _T("setfocus");
UILIB_API  extern const TCHAR* const kValueChanged; // = _T("valuechanged");
UILIB_API  extern const TCHAR* const kMenuItem; // =_T("menuItem);
UILIB_API  extern const TCHAR* const kItemDBClick; // = _T("itemdbclick");
}; // namespace DuiLib

#endif // _UICRACK_H_