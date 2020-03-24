#pragma once

class MenuLibrary
{
public:
	static UInt32 GetState(HMENU hMenu, UInt32 Item, bool ByPosition);
	static bool SetState(HMENU hMenu, UInt32 Item, bool ByPosition, UInt32 State);

	static bool IsEnabled(HMENU hMenu, UInt32 Item, bool ByPosition);
	static bool Enable(HMENU hMenu, UInt32 Item, bool ByPosition, bool Enable);

	static bool IsChecked(HMENU hMenu, UInt32 Item, bool ByPosition);
	static bool Check(HMENU hMenu, UInt32 Item, bool ByPosition, bool Check);

private:
	MenuLibrary() = delete;
	~MenuLibrary() = delete;
	MenuLibrary(const MenuLibrary&) = delete;
	MenuLibrary& operator=(const MenuLibrary&) = delete;
};
