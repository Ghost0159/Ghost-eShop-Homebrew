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
extern bool checkWifiStatus();

/*
	Draw the Screenshot menu.

	const C2D_Image &img: The C2D_Image of the screenshot.
	const int sIndex: The Screenshot index.
	const bool sFetch: If fetching screenshots or not.
	const int screenshotSize: The screenshot amount.
	const std::string &name: The name of the screenshot.
	const int zoom: The zoom level, zoom out, 1x scale, or zoom in.
	const bool canDisplay: If can display, or not.
*/
void StoreUtils::DrawScreenshotMenu(const C2D_Image &img, const int sIndex, const bool sFetch, const int screenshotSize, const std::string &name, const int zoom, const bool canDisplay) {
	Gui::ScreenDraw(Top);
	Gui::Draw_Rect(0, 0, 400, 240, UIThemes->BGColor());

	if (sFetch) {
		Animation::QueueEntryDone();
		GFX::DrawBottom();
		Gui::DrawStringCentered(0, 2, 0.6f, UIThemes->TextColor(), Lang::get("LOADING_SCREENSHOT"), 310);
		return;
	}

	if (!canDisplay) {
		Animation::QueueEntryDone();

		GFX::DrawBottom();
		if (screenshotSize > 0) { // if texture is nullptr AND screenshot size is larger than 0.
			Gui::DrawStringCentered(0, 2, 0.6f, UIThemes->TextColor(), Lang::get("SCREENSHOT_COULD_NOT_LOAD"), 310);

		} else {
			Gui::DrawStringCentered(0, 2, 0.6f, UIThemes->TextColor(), Lang::get("NO_SCREENSHOTS_AVAILABLE"), 310);
		}

		return;
	}

	if (!sFetch) { // Only, if not fetch. This avoids a small flicker of the old screenshot on entries without screenshots.
		if (screenshotSize > 0) {
			float scale = 1.0f;

			if (zoom == 0) {
				scale = std::min(1.0f, std::min(400.0f / img.subtex->width, 240.0f / img.subtex->height));
				if (img.tex) C2D_DrawImageAt(img, (400 - img.subtex->width * scale) / 2, (240 - img.subtex->height * scale) / 2, 0.5f, nullptr, scale, scale);

			} else {
				// Create new C2D_Image with smaller subtex
				C2D_Image top = img;
				if (img.subtex->height > 240)
					top.subtex = new Tex3DS_SubTexture({img.subtex->width, (u16)(img.subtex->height / 2), img.subtex->left, img.subtex->top, img.subtex->right, 1.0f - (img.subtex->height / 2 / 512.0f)});

				// If zoom == 2, then zoom in to fit the screen
				if (zoom == 2)
					scale = std::min(400.0f / top.subtex->width, 240.0f / top.subtex->height);

				if (top.tex) C2D_DrawImageAt(top, (400 - top.subtex->width * scale) / 2, (240 - top.subtex->height * scale) / 2, 0.5f, nullptr, scale, scale);

				// Only delete if new
				if (top.subtex->height > 240)
					delete top.subtex;
			}

			Animation::QueueEntryDone();
			GFX::DrawBottom();

			/* Bottom. */
			if (zoom > 0 && img.subtex->height * scale > 240) {
				C2D_Image bottom = img;
				bottom.subtex = new Tex3DS_SubTexture({img.subtex->width, (u16)(img.subtex->height / 2), img.subtex->left, img.subtex->bottom + (img.subtex->height / 2 / 512.0f), img.subtex->right, img.subtex->bottom});
				if (bottom.tex) C2D_DrawImageAt(bottom, (320 - bottom.subtex->width * scale) / 2, (240 - bottom.subtex->height * scale) / 2, 0.5f, nullptr, scale, scale);
				delete bottom.subtex;

			} else {
				Gui::Draw_Rect(0, 215, 320, 25, UIThemes->BarColor());
				Gui::Draw_Rect(0, 214, 320, 1, UIThemes->BarOutline());
				Gui::DrawStringCentered(0, 220, 0.5f, UIThemes->TextColor(), Lang::get("SCREENSHOT_INSTRUCTIONS"), 310, 0, font);

				char screenshots[0x100];
				snprintf(screenshots, sizeof(screenshots), Lang::get("SCREENSHOT").c_str(), sIndex + 1, screenshotSize);
				Gui::DrawStringCentered(0, 2, 0.6f, UIThemes->TextColor(), screenshots, 310, 0, font);
				Gui::DrawStringCentered(0, 40, 0.6f, UIThemes->TextColor(), name, 310, 0, font);
			}

		} else {
			Animation::QueueEntryDone();
			GFX::DrawBottom();
			Gui::DrawStringCentered(0, 2, 0.6f, UIThemes->TextColor(), Lang::get("NO_SCREENSHOTS_AVAILABLE"), 310);
		}
	}
}

/*
	Screenshot Menu handle.

	C2D_Image &img: The C2D_Image of the screenshot.
	int &sIndex: The Screenshot index.
	bool &sFetch: If fetching screenshots or not.
	const int screenshotSize: The screenshot amount.
	int &zoom: The zoom level, zoom out, 1x scale, or zoom in.
	bool &canDisplay: If can display or not.
*/
void StoreUtils::ScreenshotMenu(C2D_Image &img, int &sIndex, bool &sFetch, int &storeMode, const int screenshotSize, int &zoom, bool &canDisplay) {
	if (hDown & KEY_B) {
		canDisplay = false;
		zoom = 0;
		sIndex = 0;
		storeMode = 0; // Go back to EntryInfo.
	}

	if ((hDown & KEY_RIGHT) || (hDown & KEY_R)) {
		if (checkWifiStatus()) {
			if (sIndex < screenshotSize - 1) {
				sIndex++;
				sFetch = true;
			}
		}
	}

	if (hDown & KEY_DOWN && zoom > 0) zoom--;

	if (hDown & KEY_UP && zoom < 2) zoom++;

	if ((hDown & KEY_LEFT) || (hDown & KEY_L)) {
		if (checkWifiStatus()) {
			if (sIndex > 0) {
				sIndex--;
				sFetch = true;
			}
		}
	}
}