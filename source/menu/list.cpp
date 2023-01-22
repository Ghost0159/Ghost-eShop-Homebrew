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
#include "storeUtils.hpp"
#include "structs.hpp"

static const std::vector<Structs::ButtonPos> StoreBoxesList = {
	{ 20, 45, 360, 50 },
	{ 20, 105, 360, 50 },
	{ 20, 165, 360, 50 }
};

/* Draw the top List. */
void StoreUtils::DrawList() {
	if (StoreUtils::store) { // Ensure, store is not a nullptr.

		if (config->usebg() && StoreUtils::store->customBG()) {
			C2D_DrawImageAt(StoreUtils::store->GetStoreImg(), 0, 26, 0.5f, nullptr);

		} else {
			Gui::Draw_Rect(0, 26, 400, 214, UIThemes->BGColor());
		}

		if (StoreUtils::entries.size() > 0) {
			for (int i = 0; i < 3 && i < (int)StoreUtils::entries.size(); i++) {

				if (i + StoreUtils::store->GetScreenIndx() == StoreUtils::store->GetEntry()) {
					GFX::DrawBox(StoreBoxesList[i].x, StoreBoxesList[i].y, StoreBoxesList[i].w, StoreBoxesList[i].h, false);
				}

				/* Ensure, entries is larger than the index. */
				if ((int)StoreUtils::entries.size() > i + StoreUtils::store->GetScreenIndx()) {
					if (StoreUtils::entries[i + StoreUtils::store->GetScreenIndx()]) { // Ensure, the Entry is not nullptr.
						const C2D_Image tempImg = StoreUtils::entries[i + StoreUtils::store->GetScreenIndx()]->GetIcon();
						const uint8_t offsetW = (48 - tempImg.subtex->width) / 2; // Center W.
						const uint8_t offsetH = (48 - tempImg.subtex->height) / 2; // Center H.

						C2D_DrawImageAt(tempImg, StoreBoxesList[i].x + 1 + offsetW, StoreBoxesList[i].y + 1 + offsetH, 0.5);
					}

					if (StoreUtils::entries[i + StoreUtils::store->GetScreenIndx()]->GetUpdateAvl()) GFX::DrawSprite(sprites_update_app_idx, StoreBoxesList[i].x + 32, StoreBoxesList[i].y + 32);
					Gui::DrawStringCentered(29, StoreBoxesList[i].y + 5, 0.6f, UIThemes->TextColor(), StoreUtils::entries[i + StoreUtils::store->GetScreenIndx()]->GetTitle(), 300, 0, font);
					Gui::DrawStringCentered(29, StoreBoxesList[i].y + 24, 0.6f, UIThemes->TextColor(), StoreUtils::entries[i + StoreUtils::store->GetScreenIndx()]->GetAuthor(), 300, 0, font);
				}
			}
		}
	}
}


/*
	Top List Logic Handle.
	Here you can..

	- Scroll through the Grid with the D-Pad Up / Down and skip 3 entries with Left / Right.

	int &currentMode: Const Reference to the current Mode.
	int &lastMode: Reference to the last mode.
	bool &fetch: Reference to fetch.
	int &smallDelay: Reference to the small delay.
*/
void StoreUtils::ListLogic(int &currentMode, int &lastMode, bool &fetch, int &smallDelay) {
	if (StoreUtils::store) { // Ensure, store is not a nullptr.
		if (hRepeat & KEY_DOWN) {
			if (StoreUtils::store->GetEntry() < (int)StoreUtils::entries.size() - 1) StoreUtils::store->SetEntry(StoreUtils::store->GetEntry() + 1);
			else StoreUtils::store->SetEntry(0);
		}

		if (hRepeat & KEY_RIGHT) {
			if (StoreUtils::store->GetEntry() < (int)StoreUtils::entries.size() - 3) StoreUtils::store->SetEntry(StoreUtils::store->GetEntry() + 3);
			else StoreUtils::store->SetEntry(StoreUtils::entries.size() - 1);
		}

		if (hRepeat & KEY_LEFT) {
			if (StoreUtils::store->GetEntry() - 2 > 0) StoreUtils::store->SetEntry(StoreUtils::store->GetEntry() - 3);
			else StoreUtils::store->SetEntry(0);
		}

		if (hRepeat & KEY_UP) {
			if (StoreUtils::store->GetEntry() > 0) StoreUtils::store->SetEntry(StoreUtils::store->GetEntry() - 1);
			else StoreUtils::store->SetEntry(StoreUtils::entries.size() - 1);
		}

		if (hDown & KEY_A) {
			fetch = true;
			smallDelay = 5;
			lastMode = currentMode;
			currentMode = 1;
		}

		/* Scroll Logic. */
		if (StoreUtils::store->GetEntry() < StoreUtils::store->GetScreenIndx()) StoreUtils::store->SetScreenIndx(StoreUtils::store->GetEntry());
		else if (StoreUtils::store->GetEntry() > StoreUtils::store->GetScreenIndx() + 3 - 1) StoreUtils::store->SetScreenIndx(StoreUtils::store->GetEntry() - 3 + 1);
	}
}