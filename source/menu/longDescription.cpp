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
#include "download.hpp"
#include "storeUtils.hpp"

std::vector<std::string> wrappedNotes;

size_t StoreUtils::FindSplitPoint(const std::string &str, const std::vector<std::string> splitters) {
	for (const std::string &splitter : splitters) {
		size_t pos = str.rfind(splitter);
		if (pos != std::string::npos) return pos;
	}

	return std::string::npos;
}

/* Process release notes into lines */
void StoreUtils::ProcessLongDescription(std::string longDescription) {
	wrappedNotes.clear();

	size_t splitPos = 0;
	do {
		splitPos = longDescription.find('\n');
		std::string substr = longDescription.substr(0, splitPos);

		Gui::clearTextBufs();
		float width = Gui::GetStringWidth(0.5f, substr, font);

		/* If too long to fit on screen, wrap at spaces, slashes, periods, etc. */
		size_t spacePos;
		while (width > 390.0f && (spacePos = FindSplitPoint(substr.substr(0, splitPos - 1), {" ", "/", ".", "-", "_", "。", "、", "，"})) != std::string::npos) {
			splitPos = spacePos;
			if (substr[splitPos] != ' ') splitPos++;

			/* Skip to next if UTF-8 multibyte char */
			while ((substr[splitPos] & 0xC0) == 0x80) splitPos++;

			substr = substr.substr(0, splitPos);
			Gui::clearTextBufs();
			width = Gui::GetStringWidth(0.5f, substr, font);
		}

		wrappedNotes.push_back(substr);

		if (splitPos != std::string::npos) {
			if (longDescription[splitPos] == ' ' || longDescription[splitPos] == '\n') splitPos++;
			longDescription = longDescription.substr(splitPos);
		}
	} while (splitPos != std::string::npos);
}

void StoreUtils::DrawLongDescription(const int &scrollIndex, const std::unique_ptr<StoreEntry> &entry) {
	if (entry && StoreUtils::store) {
		Gui::ScreenDraw(Top);
		Gui::Draw_Rect(0, 26, 400, 214, UIThemes->BGColor());
		
		float fontHeight = Gui::GetStringHeight(0.5f, "", font);
		for (size_t i = 0; (scrollIndex + i) < wrappedNotes.size() && i < (240.0f - 25.0f) / fontHeight; i++) {
			Gui::DrawString(5, 25 + i * fontHeight, 0.5f, UIThemes->TextColor(), wrappedNotes[scrollIndex + i], 390, 0, font);
		}

		Gui::Draw_Rect(0, 0, 400, 25, UIThemes->BarColor());
		Gui::Draw_Rect(0, 25, 400, 1, UIThemes->BarOutline());
		Gui::DrawStringCentered(0, 1, 0.7f, UIThemes->TextColor(), entry->GetTitle(), 390, 0, font);

	} else {
		Gui::ScreenDraw(Top);
		Gui::Draw_Rect(0, 0, 400, 25, UIThemes->BarColor());
		Gui::Draw_Rect(0, 25, 400, 1, UIThemes->BarOutline());
		Gui::Draw_Rect(0, 26, 400, 214, UIThemes->BGColor());
	}

	Animation::QueueEntryDone();
}

/*
	As the name says: Release notes logic.

	int &scrollIndex: The scroll index for the Release Notes text.
	int &storeMode: The store mode to properly return back.
*/
void StoreUtils::LongDescriptionLogic(int &scrollIndex, int &storeMode) {
	int linesPerScreen = ((240.0f - 25.0f) / Gui::GetStringHeight(0.5f, "", font));

	if (hRepeat & KEY_DOWN) scrollIndex++;
	if (hRepeat & KEY_UP) scrollIndex--;
	if (hRepeat & KEY_RIGHT) scrollIndex += linesPerScreen;
	if (hRepeat & KEY_LEFT) scrollIndex -= linesPerScreen;

	/* Ensure it doesn't scroll off screen. */
	if (scrollIndex < 0) scrollIndex = 0;
	if (scrollIndex > (int)wrappedNotes.size() - linesPerScreen)
		scrollIndex = std::max(0, (int)wrappedNotes.size() - linesPerScreen);

	if (hDown & KEY_B) {
		scrollIndex = 0;
		storeMode = 0;
	}
}


/*
	I place it temporarely here for now.

	Display Release changelog for Ghost eShop.
*/
void DisplayChangelog() {
	if (config->changelog()) {
		config->changelog(false);

		bool confirmed = false;
		const std::string notes = GetChangelog();
		if (notes == "") return;
		int scrollIndex = 0;

		while(!confirmed) {
			Gui::clearTextBufs();
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
			C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

			Gui::ScreenDraw(Top);
			Gui::Draw_Rect(0, 26, 400, 214, UIThemes->BGColor());
			Gui::DrawString(5, 25 - scrollIndex, 0.5f, UIThemes->TextColor(), notes, 390, 0, font, C2D_WordWrap);
			Gui::Draw_Rect(0, 0, 400, 25, UIThemes->BarColor());
			Gui::Draw_Rect(0, 25, 400, 1, UIThemes->BarOutline());
			Gui::DrawStringCentered(0, 1, 0.7f, UIThemes->TextColor(), "Ghost eShop", 390, 0, font);
			Gui::Draw_Rect(0, 215, 400, 25, UIThemes->BarColor());
			Gui::Draw_Rect(0, 214, 400, 1, UIThemes->BarOutline());
			Gui::DrawStringCentered(0, 217, 0.7f, UIThemes->TextColor(), C_V, 390, 0, font);

			GFX::DrawBottom();
			Gui::Draw_Rect(0, 0, 320, 25, UIThemes->BarColor());
			Gui::Draw_Rect(0, 25, 320, 1, UIThemes->BarOutline());
			C3D_FrameEnd(0);

			hidScanInput();
			touchPosition t;
			touchRead(&t);
			u32 repeat = hidKeysDownRepeat();
			u32 down = hidKeysDown();

			/* Scroll Logic. */
			if (repeat & KEY_DOWN) scrollIndex += Gui::GetStringHeight(0.5f, "", font);

			if (repeat & KEY_UP) {
				if (scrollIndex > 0) scrollIndex -= Gui::GetStringHeight(0.5f, "", font);
			}

			if ((down & KEY_A) || (down & KEY_B) || (down & KEY_START) || (down & KEY_TOUCH)) confirmed = true;
		}
	}
}