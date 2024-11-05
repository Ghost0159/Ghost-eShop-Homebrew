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
#include "overlay.hpp"
#include "version.hpp"

/* Show the Credits. */
void Overlays::ShowCredits() {
	bool doOut = false;

	while(!doOut) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, TRANSPARENT);
		C2D_TargetClear(Bottom, TRANSPARENT);

		GFX::DrawTop();
		GFX::DrawSprite(sprites_GhosteShop_idx, 240, 30);
		Gui::DrawStringCentered(0, 1, 0.7f, UIThemes->TextColor(), "Ghost eShop - " + Lang::get("CREDITS"), 395, 0, font);

		Gui::DrawString(10, 30, 0.5f, UIThemes->TextColor(), "- Ghost eShop Team", 0, 0, font);
		Gui::DrawString(10, 50, 0.5f, UIThemes->TextColor(), "- " + Lang::get("YOUTUBERS"), 0, 0, font);
		Gui::DrawString(10, 70, 0.5f, UIThemes->TextColor(), "- " + Lang::get("CITRA_BIGN_ICON"), 0, 0, font);
		Gui::DrawString(10, 90, 0.5f, UIThemes->TextColor(), "- " + Lang::get("3D_BANNER"), 0, 0, font);
		Gui::DrawString(10, 110, 0.5f, UIThemes->TextColor(), "- icons8", 0, 0, font);
		Gui::DrawString(10, 130, 0.5f, UIThemes->TextColor(), "- hShop", 0, 0, font);
		/* geTeamMember */
		Gui::DrawString(10, 150, 0.5f, UIThemes->TextColor(), "- BoomTchaGong for the charisma", 0, 0, font);
		/* geTeamMember */
		Gui::DrawString(10, 170, 0.5f, UIThemes->TextColor(), Lang::get("CONTRIBUTOR_TRANSLATORS"), 210, 0, font);
		Gui::DrawString(10, 197, 0.5f, UIThemes->TextColor(), Lang::get("WEBSITE"), 390, 0, font);

		Gui::Draw_Rect(0, 215, 400, 25, UIThemes->BarColor());
		Gui::Draw_Rect(0, 214, 400, 1, UIThemes->BarOutline());
		Gui::DrawStringCentered(0, 218, 0.6f, UIThemes->TextColor(), Lang::get("CURRENT_VERSION") + std::string(VER_NUMBER), 390, 0, font);

		Animation::QueueEntryDone();
		GFX::DrawBottom();
		GFX::DrawSprite(sprites_GhosteShop_core_idx, 0, 0);
		C3D_FrameEnd(0);

		hidScanInput();
		Animation::HandleQueueEntryDone();
		if ((hidKeysDown() & KEY_START) || (hidKeysDown() & KEY_B) || (hidKeysDown() & KEY_A)) doOut = true;
	}
}