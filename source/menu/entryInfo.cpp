/*
*   This file is part of Universal-Updater
*   Copyright (C) 2019-2020 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "storeUtils.hpp"
#include "structs.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
static const Structs::ButtonPos btn = { 45, 215, 24, 24 };
static const Structs::ButtonPos sshot = { 75, 215, 24, 24 };
static const Structs::ButtonPos notes = { 105, 215, 24, 24 };
extern bool checkWifiStatus();

/*
	Dessinez la partie Entry Info.
	const std::unique_ptr<Store> &store : Const Référence à la classe Store.
	const std::unique_ptr<StoreEntry> &entry : Const Référence au StoreEntry actuel.
*/
void StoreUtils::DrawEntryInfo(const std::unique_ptr<Store> &store, const std::unique_ptr<StoreEntry> &entry) {
	if (store && entry) { // Assurez-vous que l’enregistrement et la saisie ne sont pas un nullptr.
		Gui::Draw_Rect(40, 0, 280, 36, ENTRY_BAR_COLOR);
		Gui::Draw_Rect(40, 36, 280, 1, ENTRY_BAR_OUTL_COLOR);

		Gui::DrawStringCentered(17, 0, 0.6, TEXT_COLOR, entry->GetTitle(), 273, 0, font);
		Gui::DrawStringCentered(17, 20, 0.4, TEXT_COLOR, entry->GetAuthor(), 273, 0, font);
		Gui::DrawStringCentered(17, 50, 0.4, TEXT_COLOR, entry->GetDescription(), 248, 0, font, C2D_WordWrap);

		Gui::DrawString(61, 130, 0.45, TEXT_COLOR, Lang::get("VERSION") + ": " + entry->GetVersion(), 248, 0, font);
		Gui::DrawString(61, 145, 0.45, TEXT_COLOR, Lang::get("CATEGORY") + ": " + entry->GetCategory(), 248, 0, font);
		Gui::DrawString(61, 160, 0.45, TEXT_COLOR, Lang::get("CONSOLE") + ": " + entry->GetConsole(), 248, 0, font);
		Gui::DrawString(61, 175, 0.45, TEXT_COLOR, Lang::get("SIZE") + ": " + entry->GetSize(), 248, 0, font);
		Gui::DrawString(61, 195, 0.45, TEXT_COLOR, entry->GetAdditionalcontent(), 248, 0, font);

		GFX::DrawBox(btn.x, btn.y, btn.w, btn.h, false);
		if (!entry->GetScreenshots().empty()) GFX::DrawSprite(sprites_screenshot_idx, sshot.x, sshot.y);
		if (entry->GetReleaseNotes() != "") GFX::DrawSprite(sprites_notes_idx, notes.x, notes.y);
		Gui::DrawString(btn.x + 5, btn.y + 2, 0.6f, TEXT_COLOR, "★", 0, 0, font);
	}
}

/*
	La poignée EntryInfo.
	Ici vous pouvez..
	- Accédez à la liste de téléchargement en appuyant sur « A ».
	- Montrez le MarkMenu avec START.
	bool &showMark : Référence à showMark.. pour afficher le menu mark.
	bool &fetch : Référence à fetch, pour que nous sachions, si nous avons besoin de fetch,
	quand nous accédons à la liste de téléchargement.
*/
void StoreUtils::EntryHandle(bool &showMark, bool &fetch, bool &sFetch, int &mode, const std::unique_ptr<StoreEntry> &entry) {
	if (entry) {
		if ((hDown & KEY_START) || (hDown & KEY_TOUCH && touching(touch, btn))) showMark = true;

		if ((hDown & KEY_Y) || (hDown & KEY_TOUCH && touching(touch, sshot))) {
			if (!entry->GetScreenshots().empty()) {
				if (checkWifiStatus()) {
					sFetch = true;
					mode = 6;
				}
			}
		}

		if ((hDown & KEY_X) || (hDown & KEY_TOUCH && touching(touch, notes))) {
			if (entry->GetReleaseNotes() != "") mode = 7;
		}
	}
}