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

extern bool touching(touchPosition touch, Structs::ButtonPos button);
static const std::vector<Structs::ButtonPos> markBox = {
	{ 10, 94, 52, 52 },
	{ 72, 94, 52, 52 },
	{ 134, 94, 52, 52 },
	{ 196, 94, 52, 52 },
	{ 258, 94, 52, 52 },

	{ 45, 215, 24, 24 }
};

/*
	Draw the Marking part.

	int marks: The active mark flags.
*/
void StoreUtils::DisplayMarkBox(int marks) {
	Gui::Draw_Rect(0, 0, 320, 240, DIM_COLOR); // Darken.

	Gui::Draw_Rect(markBox[0].x, markBox[0].y, markBox[0].w, markBox[0].h, ((marks & favoriteMarks::STAR) ?
		UIThemes->MarkSelected() : UIThemes->MarkUnselected()));

	Gui::Draw_Rect(markBox[1].x, markBox[1].y, markBox[1].w, markBox[1].h, ((marks & favoriteMarks::HEART) ?
		UIThemes->MarkSelected() : UIThemes->MarkUnselected()));

	Gui::Draw_Rect(markBox[2].x, markBox[2].y, markBox[2].w, markBox[2].h, ((marks & favoriteMarks::DIAMOND) ?
		UIThemes->MarkSelected() : UIThemes->MarkUnselected()));

	Gui::Draw_Rect(markBox[3].x, markBox[3].y, markBox[3].w, markBox[3].h, ((marks & favoriteMarks::CLUBS) ?
		UIThemes->MarkSelected() : UIThemes->MarkUnselected()));

	Gui::Draw_Rect(markBox[4].x, markBox[4].y, markBox[4].w, markBox[4].h, ((marks & favoriteMarks::SPADE) ?
		UIThemes->MarkSelected() : UIThemes->MarkUnselected()));

	Gui::DrawString(markBox[0].x + 15, markBox[0].y + 11, 0.9, UIThemes->TextColor(), "★", 0, 0, font);
	Gui::DrawString(markBox[1].x + 15, markBox[1].y + 11, 0.9, UIThemes->TextColor(), "♥", 0, 0, font);
	Gui::DrawString(markBox[2].x + 15, markBox[2].y + 11, 0.9, UIThemes->TextColor(), "♦", 0, 0, font);
	Gui::DrawString(markBox[3].x + 15, markBox[3].y + 11, 0.9, UIThemes->TextColor(), "♣", 0, 0, font);
	Gui::DrawString(markBox[4].x + 15, markBox[4].y + 11, 0.9, UIThemes->TextColor(), "♠", 0, 0, font);

	GFX::DrawBox(markBox[5].x, markBox[5].y, markBox[5].w, markBox[5].h, false);
	Gui::DrawString(markBox[5].x + 5, markBox[5].y + 2, 0.6f, UIThemes->TextColor(), "★", 0, 0, font);
}

/*
	Mark Menu handle.
	Here you can..

	- Mark the selected app.
	- Return to EntryInfo with `B`.

	std::unique_ptr<StoreEntry> &entry: Reference to the current StoreEntry.
	bool &showMark: Reference to showMark, so we know, if we should stay here or not.
*/
void StoreUtils::MarkHandle(std::unique_ptr<StoreEntry> &entry, bool &showMark) {
	hidScanInput();
	touchPosition t;
	hidTouchRead(&t);

	if (StoreUtils::meta && entry && StoreUtils::store) {
		if (hidKeysDown() & KEY_TOUCH) {
			/* Star. */
			if (touching(t, markBox[0])) {
				StoreUtils::meta->SetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle(),
					StoreUtils::meta->GetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle()) ^ favoriteMarks::STAR);
				entry->SetMark(StoreUtils::meta->GetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle()));

			/* Heart. */
			} else if (touching(t, markBox[1])) {
				StoreUtils::meta->SetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle(),
					StoreUtils::meta->GetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle()) ^ favoriteMarks::HEART);
				entry->SetMark(StoreUtils::meta->GetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle()));

			/* Diamond. */
			} else if (touching(t, markBox[2])) {
				StoreUtils::meta->SetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle(),
					StoreUtils::meta->GetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle()) ^ favoriteMarks::DIAMOND);
				entry->SetMark(StoreUtils::meta->GetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle()));

			/* Clubs. */
			} else if (touching(t, markBox[3])) {
				StoreUtils::meta->SetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle(),
					StoreUtils::meta->GetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle()) ^ favoriteMarks::CLUBS);
				entry->SetMark(StoreUtils::meta->GetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle()));

			/* Spade. */
			} else if (touching(t, markBox[4])) {
				StoreUtils::meta->SetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle(),
					StoreUtils::meta->GetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle()) ^ favoriteMarks::SPADE);

				entry->SetMark(StoreUtils::meta->GetMarks(StoreUtils::store->GetEshopTitle(), entry->GetTitle()));
			}
		}
	}


	if ((hidKeysDown() & KEY_B || hidKeysDown() & KEY_SELECT) || (hidKeysDown() & KEY_TOUCH && touching(t, markBox[5]))) showMark = false; // Return back to screen.
}