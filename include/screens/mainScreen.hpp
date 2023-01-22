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

#ifndef _GHOST_ESHOP_MAIN_SCREEN_HPP
#define _GHOST_ESHOP_MAIN_SCREEN_HPP

#include "common.hpp"
#include "store.hpp"
#include "storeEntry.hpp"
#include "storeUtils.hpp"

/*
	Modes:

	0: Entry Info.
	1: Download List.
	2: Queue.
	3: Search + Favorites.
	4: Sorting.
	5: Settings / Credits(?).
	6: Screenshot Menu.
	7: Release Notes.
*/

class MainScreen : public Screen {
public:
	MainScreen();
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	std::vector<std::string> dwnldList, dwnldSizes, dwnldTypes;

	bool initialized = false, fetchDown = false, showMarks = false, showSettings = false,
		 ascending = false, updateFilter = false, screenshotFetch = false, canDisplay = false, isAND = true;

	int storeMode = 0, marks = 0, markIndex = 0, sPage = 0, lMode = 0, sSelection = 0,
		lastMode = 0, smallDelay = 0, sPos = 0, screenshotIndex = 0, sSize = 0, zoom = 0, scrollIndex = 0, queueIndex = 0;

	SortType sorttype = SortType::LAST_UPDATED;

	/* Title, Author, Category, Console. */
	std::vector<bool> searchIncludes = { true, false, false, false }, installs = { };
	std::string searchResult = "", screenshotName = "";

	C2D_Image Screenshot = { nullptr, nullptr };
};

#endif