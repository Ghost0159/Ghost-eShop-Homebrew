/*
*   This file is part of Ghost eShop Project
*   Copyright (C) 2020-2023 Ghost eShop Team
*   
*   This program is a free open-source software that allows users
*	to browse and download digital products.  It is based on the
*	code of the Universal-Updater project from Universal-Team.
*   
*   It's distributed under the terms of the GNU General Public
*	License and it's completely free to use and modify.
*   
*   This program comes with no warranty, but we are constantly
*	working on improving its functionality and user experience.
*   
*   If you have any suggestions or find any bugs, please let us know!
*   
*   Ghost eShop Team reserves the right to update the license terms
*	at any time without prior notice.
*   Any changes to the code must be clearly marked as such to avoid confusion.
*/

#ifndef _GHOST_ESHOP_THEME_HPP
#define _GHOST_ESHOP_THEME_HPP

#include "json.hpp"
#include <citro2d.h>
#include <string>

class Theme {
public:
	Theme(const std::string &ThemeJSON = "sdmc:/3ds/Universal-Updater/GhosteShop/themes/Themes.json");
	nlohmann::json InitWithDefaultColors(const std::string &ThemePath = "sdmc:/3ds/Universal-Updater/GhosteShop/themes/Themes.json");
	void LoadTheme(const std::string &ThemeName);
	std::vector<std::pair<std::string, std::string>> ThemeNames();
	uint32_t GetThemeColor(const std::string &ThemeName, const std::string &Key, const uint32_t DefaultColor);

	uint32_t BarColor() const { return this->vBarColor; };
	uint32_t BGColor() const { return this->vBGColor; };
	uint32_t BarOutline() const { return this->vBarOutline; };
	uint32_t TextColor() const { return this->vTextColor; };
	uint32_t EntryBar() const { return this->vEntryBar; };
	uint32_t EntryOutline() const { return this->vEntryOutline; };
	uint32_t BoxInside() const { return this->vBoxInside; };
	uint32_t BoxSelected() const { return this->vBoxSelected; };
	uint32_t BoxUnselected() const { return this->vBoxUnselected; };
	uint32_t ProgressbarOut() const { return this->vProgressbarOut; };
	uint32_t ProgressbarIn() const { return this->vProgressbarIn; };
	uint32_t SearchBar() const { return this->vSearchBar; };
	uint32_t SearchBarOutline() const { return this->vSearchBarOutline; };
	uint32_t SideBarSelected() const { return this->vSideBarSelected; };
	uint32_t SideBarUnselected() const { return this->vSideBarUnselected; };
	uint32_t MarkSelected() const { return this->vMarkSelected; };
	uint32_t MarkUnselected() const { return this->vMarkUnselected; };
	uint32_t DownListPrev() const { return this->vDownListPrev; };
	uint32_t SideBarIconColor() const { return this->vSideBarIconColor; };
private:
	uint32_t vBarColor = 0, vBGColor = 0, vBarOutline = 0, vTextColor = 0, vEntryBar = 0, vEntryOutline = 0,
			 vBoxInside = 0, vBoxSelected = 0, vBoxUnselected = 0, vProgressbarOut = 0, vProgressbarIn = 0,
			 vSearchBar = 0, vSearchBarOutline = 0, vSideBarSelected = 0, vSideBarUnselected = 0,
			 vMarkSelected = 0, vMarkUnselected = 0, vDownListPrev = 0, vSideBarIconColor = 0;

	int SelectedTheme = 0;
	bool Loaded = false;
	nlohmann::json json = nullptr;
};

#endif