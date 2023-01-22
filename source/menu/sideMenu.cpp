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

#include "animation.hpp"
#include "common.hpp"
#include "storeUtils.hpp"
#include "structs.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
static const std::vector<Structs::ButtonPos> sidePos = {
	{ 0, 0, 40, 40 },
	{ 0, 40, 40, 40 },
	{ 0, 80, 40, 40 },
	{ 0, 120, 40, 40 },
	{ 0, 160, 40, 40 },
	{ 0, 200, 40, 40 }
};

/*
	Draw the Side Menu part.

	int currentMenu: The current store Mode / Menu.
*/
void StoreUtils::DrawSideMenu(int currentMenu) {
	for (int i = 0; i < 6; i++) {
		if (i == currentMenu) {
			Gui::Draw_Rect(sidePos[i].x, sidePos[i].y, sidePos[i].w, sidePos[i].h, UIThemes->SideBarSelected());

		} else {
			Gui::Draw_Rect(sidePos[i].x, sidePos[i].y, sidePos[i].w, sidePos[i].h, UIThemes->SideBarUnselected());
		}
	}

	GFX::DrawIcon(sprites_info_idx, sidePos[0].x, sidePos[0].y);
	GFX::DrawIcon(sprites_download_idx, sidePos[1].x, sidePos[1].y);
	Animation::DrawQueue(sidePos[2].x, sidePos[2].y);
	GFX::DrawIcon(sprites_search_idx, sidePos[3].x, sidePos[3].y);
	GFX::DrawIcon(sprites_sort_idx, sidePos[4].x, sidePos[4].y);
	GFX::DrawIcon(sprites_settings_idx, sidePos[5].x, sidePos[5].y);

	Gui::Draw_Rect(40, 0, 1, 240, UIThemes->BarOutline());
}

/*
	Side Menu Handle.
	Here you can..

	- Switch between the Menus through the sidebar.

	int &currentMenu: Reference to the store Mode / Menu.
	bool &fetch: Reference of the download fetch variable.. so we know, if we need to fetch the download entries.
	int &lastMenu: Reference to the last menu.
*/
void StoreUtils::SideMenuHandle(int &currentMenu, bool &fetch, int &lastMenu) {
	Animation::QueueAnimHandle();

	if (hDown & KEY_TOUCH) {
		for (int i = 0; i < 6; i++) {
			if (touching(touch, sidePos[i])) {
				lastMenu = currentMenu;
				if (i == 1) fetch = true; // Fetch download list, if 1.
				currentMenu = i;
				break;
			}
		}
	}

	if (hRepeat & KEY_R) {
		if (currentMenu < 5) {
			lastMenu = currentMenu;
			if (currentMenu + 1 == 1) fetch = true; // Fetch download list, if 1.
			currentMenu++;
		}
	}

	if (hRepeat & KEY_L) {
		if (currentMenu > 0) {
			lastMenu = currentMenu;
			if (currentMenu - 1 == 1) fetch = true; // Fetch download list, if 1.
			currentMenu--;
		}
	}
}