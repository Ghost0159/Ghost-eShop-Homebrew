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

#include "gfx.hpp"
#include "keyboard.hpp"
#include "screenCommon.hpp"

static std::vector<SwkbdDictWord> words;

/*
	Return a string of the keyboard.

	uint maxLength: The max length.
	const std::string &Text: Const Reference to the Text.
	const std::vector<std::unique_ptr<StoreEntry>> &entries: Const Reference of all entries for the words to suggest.
*/
std::string Input::setkbdString(uint maxLength, const std::string &Text, const std::vector<std::unique_ptr<StoreEntry>> &entries) {
	C3D_FrameEnd(0); // Needed, so the system will not freeze.

	SwkbdState state;
	swkbdInit(&state, SWKBD_TYPE_NORMAL, 2, maxLength);
	char temp[maxLength + 1] = { 0 };
	swkbdSetHintText(&state, Text.c_str());
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, SWKBD_FILTER_PROFANITY, 0);

	if (entries.size()) {
		words.clear();
		words.resize(entries.size());

		for (uint i = 0; i < entries.size(); i++) {
			/* Checking for not nullptr. */
			if (entries[i]) swkbdSetDictWord(&words[i], StringUtils::lower_case(entries[i]->GetTitle()).c_str(), entries[i]->GetTitle().c_str());
			else swkbdSetDictWord(&words[i], "", "");
		}

		if (words.size() > 0) {
			swkbdSetDictionary(&state, words.data(), entries.size());
			swkbdSetFeatures(&state, SWKBD_PREDICTIVE_INPUT);
		}
	}

	SwkbdButton ret = swkbdInputText(&state, temp, sizeof(temp));
	temp[maxLength] = '\0';

	return (ret == SWKBD_BUTTON_CONFIRM ? temp : "");
}