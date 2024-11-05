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
#include "files.hpp"
#include "storeUtils.hpp"
#include "structs.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
static const Structs::ButtonPos btn = { 45, 215, 24, 24 };
static const Structs::ButtonPos sshot = { 75, 215, 24, 24 };
static const Structs::ButtonPos description = { 105, 215, 24, 24 };
extern bool checkWifiStatus();
extern bool exiting, QueueRuns;

/*
	Draw the Entry Info part.

	const std::unique_ptr<StoreEntry> &entry: Const Reference to the current StoreEntry.
*/
void StoreUtils::DrawEntryInfo(const std::unique_ptr<StoreEntry> &entry) {
	if (StoreUtils::store && entry) { // Ensure, store & entry is not a nullptr.
		Gui::Draw_Rect(40, 0, 280, 36, UIThemes->EntryBar());
		Gui::Draw_Rect(40, 36, 280, 1, UIThemes->EntryOutline());

		Gui::DrawStringCentered(17, 0, 0.6, UIThemes->TextColor(), entry->GetTitle(), 273, 0, font);
		Gui::DrawStringCentered(17, 20, 0.4, UIThemes->TextColor(), entry->GetAuthor(), 273, 0, font);
		Gui::DrawStringCentered(17, 50, 0.4, UIThemes->TextColor(), entry->GetDescription(), 248, 0, font, C2D_WordWrap);

		Gui::DrawString(53, 130, 0.45, UIThemes->TextColor(), Lang::get("VERSION") + ": " + entry->GetVersion(), 248, 0, font);
		Gui::DrawString(53, 145, 0.45, UIThemes->TextColor(), Lang::get("CATEGORY") + ": " + entry->GetCategory(), 248, 0, font);
		Gui::DrawString(53, 160, 0.45, UIThemes->TextColor(), Lang::get("CONSOLE") + ": " + entry->GetConsole(), 248, 0, font);
		Gui::DrawString(53, 190, 0.45, UIThemes->TextColor(), entry->GetAdditionalcontent(), 248, 0, font);

		GFX::DrawBox(btn.x, btn.y, btn.w, btn.h, false);
		if (!entry->GetScreenshots().empty()) GFX::DrawIcon(sprites_screenshot_idx, sshot.x, sshot.y, UIThemes->TextColor());
		if (entry->GetLongDescription() != "") GFX::DrawIcon(sprites_description_idx, description.x, description.y, UIThemes->TextColor());
		Gui::DrawString(btn.x + 5, btn.y + 2, 0.6f, UIThemes->TextColor(), "★", 0, 0, font);
	}
}

/*
	The EntryInfo handle.
	Here you can..

	- Go to the download list, by pressing `A`.
	- Show the MarkMenu with START.

	bool &showMark: Reference to showMark.. to show the mark menu.
	bool &fetch: Reference to fetch, so we know, if we need to fetch, when accessing download list.
	bool &sFetch: Reference to the screenshot fetch.
	int &mode: Reference to the store mode.
	const std::unique_ptr<StoreEntry> &entry: The store Entry.
*/
void StoreUtils::EntryHandle(bool &showMark, bool &fetch, bool &sFetch, int &mode, const std::unique_ptr<StoreEntry> &entry) {
	if (entry) {
		if ((hDown & KEY_SELECT) || (hDown & KEY_TOUCH && touching(touch, btn))) showMark = true;

		if ((hDown & KEY_Y) || (hDown & KEY_TOUCH && touching(touch, sshot))) {
			if (!entry->GetScreenshots().empty()) {
				if (checkWifiStatus()) {
					if (QueueRuns) {
						if (!Msg::promptMsg(Lang::get("FEATURE_SIDE_EFFECTS"))) return;
						sFetch = true;
						mode = 6;

					} else {
						sFetch = true;
						mode = 6;
					}
				}
			}
		}

		if ((hDown & KEY_X) || (hDown & KEY_TOUCH && touching(touch, description))) {
			if (entry->GetLongDescription() != "") {
				ProcessLongDescription(entry->GetLongDescription());
				mode = 7;
			}
		}
	}

	/* Quit UU. */
	if (hDown & KEY_START && !QueueRuns)
		exiting = true;
}