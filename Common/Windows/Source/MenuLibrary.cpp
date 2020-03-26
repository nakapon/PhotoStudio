#include <Platform.h>

#include <MenuLibrary.h>

UInt32 MenuLibrary::GetState(HMENU hMenu, UInt32 Item, bool ByPosition)
{
	MENUITEMINFO MenuItemInfo = { 0 };

	MenuItemInfo.cbSize = sizeof(MENUITEMINFO);
	MenuItemInfo.fMask = MIIM_STATE;

	if(!::GetMenuItemInfo(hMenu, Item, ByPosition ? TRUE : FALSE, &MenuItemInfo))
		return 0;

	return MenuItemInfo.fState;
}

bool MenuLibrary::SetState(HMENU hMenu, UInt32 Item, bool ByPosition, UInt32 State)
{
	MENUITEMINFO MenuItemInfo = { 0 };

	MenuItemInfo.cbSize = sizeof(MENUITEMINFO);
	MenuItemInfo.fMask = MIIM_STATE;
	MenuItemInfo.fState = State;

	if(!::SetMenuItemInfo(hMenu, Item, ByPosition ? TRUE : FALSE, &MenuItemInfo))
		return false;

	return true;
}

bool MenuLibrary::IsEnabled(HMENU hMenu, UInt32 Item, bool ByPosition)
{
	return ((MFS_DISABLED & MenuLibrary::GetState(hMenu, Item, ByPosition)) == 0) ? true : false;
}

bool MenuLibrary::Enable(HMENU hMenu, UInt32 Item, bool ByPosition, bool Enable)
{
	UInt32 State = 0;

	State = MenuLibrary::GetState(hMenu, Item, ByPosition);

	if(Enable)
	{
		State &= ~MFS_DISABLED;
	}
	else
	{
		State |= MFS_DISABLED;
	}

	return MenuLibrary::SetState(hMenu, Item, ByPosition, State);
}

bool MenuLibrary::IsChecked(HMENU hMenu, UInt32 Item, bool ByPosition)
{
	return ((MFS_CHECKED & MenuLibrary::GetState(hMenu, Item, ByPosition)) != 0) ? true : false;
}

bool MenuLibrary::Check(HMENU hMenu, UInt32 Item, bool ByPosition, bool Check)
{
	UInt32 State = 0;

	State = MenuLibrary::GetState(hMenu, Item, ByPosition);

	if(Check)
	{
		State |= MFS_CHECKED;
	}
	else
	{
		State &= ~MFS_CHECKED;
	}

	return MenuLibrary::SetState(hMenu, Item, ByPosition, State);
}

bool MenuLibrary::GetString(HMENU hMenu, UInt32 Item, bool ByPosition, LPTSTR pszString, UInt32 MaxLength)
{
	MENUITEMINFO MenuItemInfo = { 0 };

	if(hMenu == nullptr || pszString == nullptr || MaxLength == 0)
		return false;

	MenuItemInfo.cbSize = sizeof(MENUITEMINFO);
	MenuItemInfo.fMask = MIIM_TYPE;
	MenuItemInfo.dwTypeData = pszString;
	MenuItemInfo.cch = MaxLength;

	if(!::GetMenuItemInfo(hMenu, Item, ByPosition, &MenuItemInfo))
		return false;

	return true;
}

bool MenuLibrary::SetString(HMENU hMenu, UInt32 Item, bool ByPosition, LPCTSTR pszString)
{
	MENUITEMINFO MenuItemInfo = { 0 };

	if(hMenu == nullptr || pszString == nullptr)
		return false;

	MenuItemInfo.cbSize = sizeof(MENUITEMINFO);
	MenuItemInfo.fMask = MIIM_STRING;
	MenuItemInfo.dwTypeData = (LPTSTR)pszString;

	if(!::SetMenuItemInfo(hMenu, Item, ByPosition, &MenuItemInfo))
		return false;

	return true;
}
