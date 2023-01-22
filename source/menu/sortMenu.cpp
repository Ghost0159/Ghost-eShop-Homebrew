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

#include "common.hpp"
#include "keyboard.hpp"
#include "storeUtils.hpp"
#include "structs.hpp"

extern bool exiting, QueueRuns;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

static const std::vector<Structs::ButtonPos> buttons = {
	{ 71, 60, 104, 16 },
	{ 71, 80, 104, 16 },
	{ 71, 100, 104, 16 },

	{ 201, 60, 104, 16 },
	{ 201, 80, 104, 16 },

	{ 71, 170, 104, 16 },
	{ 71, 190, 104, 16 }
};

static void DrawCheck(int pos, bool v) {
	GFX::DrawIcon((v ? sprites_sort_checked_idx : sprites_sort_unchecked_idx), buttons[pos].x + 1, buttons[pos].y, UIThemes->TextColor());
}

/*
	Return SortType as an uint8_t.

	SortType st: The SortType variable.
*/
static const uint8_t GetType(SortType st) {
	switch(st) {
		case SortType::TITLE:
			return 0;

		case SortType::AUTHOR:
			return 1;

		case SortType::LAST_UPDATED:
			return 2;
	}

	return 1;
}

/*
	Draw the Sort Menu.

	bool asc: The Ascending variable.
	SortType st: The SortType variable.
*/
void StoreUtils::DrawSorting(bool asc, SortType st) {
	Gui::Draw_Rect(40, 0, 280, 25, UIThemes->EntryBar());
	Gui::Draw_Rect(40, 25, 280, 1, UIThemes->EntryOutline());
	Gui::DrawStringCentered(17, 2, 0.6, UIThemes->TextColor(), Lang::get("SORTING"), 273, 0, font);

	/* Sort By. */
	Gui::DrawString(buttons[0].x + 1, buttons[0].y - 20, 0.6f, UIThemes->TextColor(), Lang::get("SORT_BY"), 90, 0, font);
	for (int i = 0; i < 3; i++) {
		DrawCheck(i, i == GetType(st));
	}

	Gui::DrawString(buttons[0].x + 21, buttons[0].y + 2, 0.4f, UIThemes->TextColor(), Lang::get("TITLE"), 80, 0, font);
	Gui::DrawString(buttons[1].x + 21, buttons[1].y + 2, 0.4f, UIThemes->TextColor(), Lang::get("AUTHOR"), 80, 0, font);
	Gui::DrawString(buttons[2].x + 21, buttons[2].y + 2, 0.4f, UIThemes->TextColor(), Lang::get("LAST_UPDATED"), 80, 0, font);

	/* Direction. */
	Gui::DrawString(buttons[3].x + 1, buttons[3].y - 20, 0.6f, UIThemes->TextColor(), Lang::get("DIRECTION"), 80, 0, font);
	DrawCheck(3, asc);
	DrawCheck(4, !asc);
	Gui::DrawString(buttons[3].x + 21, buttons[3].y + 2, 0.4f, UIThemes->TextColor(), Lang::get("ASCENDING"), 80, 0, font);
	Gui::DrawString(buttons[4].x + 21, buttons[4].y + 2, 0.4f, UIThemes->TextColor(), Lang::get("DESCENDING"), 80, 0, font);

	/* Top Style. */
	Gui::DrawString(buttons[5].x + 1, buttons[5].y - 20, 0.6f, UIThemes->TextColor(), Lang::get("TOP_STYLE"), 90, 0, font);
	DrawCheck(5, config->list());
	DrawCheck(6, !config->list());
	Gui::DrawString(buttons[5].x + 21, buttons[5].y + 2, 0.4f, UIThemes->TextColor(), Lang::get("LIST"), 90, 0, font);
	Gui::DrawString(buttons[6].x + 21, buttons[6].y + 2, 0.4f, UIThemes->TextColor(), Lang::get("GRID"), 90, 0, font);
}

/*
	Sort Handle.
	Here you can..

	- Sort your entries to..
		- Title (Ascending / Descending).
		- Author (Ascending / Descending).
		- Last Updated Date (Ascending / Descending).

	- Change the Top Style.

	bool &asc: Reference to the Ascending variable.
	SortType &st: Reference to the SortType.
*/
void StoreUtils::SortHandle(bool &asc, SortType &st) {
	if (StoreUtils::store && StoreUtils::store->GetValid() && StoreUtils::entries.size() > 0) { // Ensure, this is valid and more than 0 StoreUtils::entries exist.
		if (hDown & KEY_TOUCH) {
			/* SortType Part. */
			if (touching(touch, buttons[0])) {
				st = SortType::TITLE;
				StoreUtils::SortEntries(asc, st);

			} else if (touching(touch, buttons[1])) {
				st = SortType::AUTHOR;
				StoreUtils::SortEntries(asc, st);

			} else if (touching(touch, buttons[2])) {
				st = SortType::LAST_UPDATED;
				StoreUtils::SortEntries(asc, st);

			/* Ascending | Descending Part. */
			} else if (touching(touch, buttons[3])) {
				asc = true;
				StoreUtils::SortEntries(asc, st);

			} else if (touching(touch, buttons[4])) {
				asc = false;
				StoreUtils::SortEntries(asc, st);

			} else if (touching(touch, buttons[5])) {
				if (config->list()) return;
				config->list(true);
				StoreUtils::store->SetEntry(0);
				StoreUtils::store->SetScreenIndx(0);
				StoreUtils::store->SetBox(0);

			} else if (touching(touch, buttons[6])) {
				if (!config->list()) return;
				config->list(false);
				StoreUtils::store->SetEntry(0);
				StoreUtils::store->SetScreenIndx(0);
				StoreUtils::store->SetBox(0);
			}
		}
	}

	/* Quit GE. */
	if (hDown & KEY_START && !QueueRuns)
		exiting = true;
}