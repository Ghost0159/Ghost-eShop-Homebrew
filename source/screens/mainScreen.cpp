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
#include "download.hpp"
#include "fileBrowse.hpp"
#include "mainScreen.hpp"
#include "queueSystem.hpp"
#include "screenshot.hpp"
#include "storeUtils.hpp"
#include <unistd.h>

extern int fadeAlpha;

extern EshopInfo GetInfo(const std::string &file, const std::string &fileName);
extern void notConnectedMsg();
extern void DisplayChangelog();

/*
	MainScreen Constructor.

	Initialized meta, store and StoreEntry class and:

	- Downloads ghosteshop.unistore.. in case nothing exist.
*/
MainScreen::MainScreen() {
	StoreUtils::meta = std::make_unique<Meta>();

	/* Check if lastStore is accessible. */
	if (config->lastStore() != "ghosteshop.unistore" && config->lastStore() != "") {
		if (access((_STORE_PATH + config->lastStore()).c_str(), F_OK) != 0) {
			config->lastStore("ghosteshop.unistore");

		} else {
			/* check version and file here. */
			const EshopInfo info = GetInfo((_STORE_PATH + config->lastStore()), config->lastStore());

			if (info.Version != 3 && info.Version != _ESHOP_VERSION) {
				config->lastStore("ghosteshop.unistore");
			}

			if (info.File != "") { // Ensure to check for this.
				if ((info.File.find("/") != std::string::npos)) {
					config->lastStore("ghosteshop.unistore"); // It does contain a '/' which is invalid.
				}
			}
		}
	}

	/* If ghosteshop.eshop --> Get! */
	if (config->lastStore() == "ghosteshop.unistore" || config->lastStore() == "") {
		if (access("sdmc:/3ds/Universal-Updater/stores/ghosteshop.unistore", F_OK) != 0) {
			if (checkWifiStatus()) {
				std::string tmp = ""; // Just a temp.
				DownloadEshop("https://cdn.ghosteshop.com/script/ghosteshop.unistore", -1, tmp, true, true);
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon.t3x", "icon.t3x");
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon_1.t3x", "icon_1.t3x");
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon_2.t3x", "icon_2.t3x");
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon_3.t3x", "icon_3.t3x");
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon_4.t3x", "icon_4.t3x");
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon_5.t3x", "icon_5.t3x");
				DownloadEshop("https://cdn.ghosteshop.com/script/ghosteshop-ds.unistore", -1, tmp, true, true);
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon-ds.t3x", "icon-ds.t3x");
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon-ds_1.t3x", "icon-ds_1.t3x");

			} else {
				notConnectedMsg();
			}

		} else {
			const EshopInfo info = GetInfo("sdmc:/3ds/Universal-Updater/stores/ghosteshop.unistore", "ghosteshop.unistore");

			if (info.Version != 3 && info.Version != _ESHOP_VERSION) {
				if (checkWifiStatus()) {
					std::string tmp = ""; // Just a temp.
					DownloadEshop("https://cdn.ghosteshop.com/script/ghosteshop.unistore", -1, tmp, true, true);
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon.t3x", "icon.t3x");
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon_1.t3x", "icon_1.t3x");
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon_2.t3x", "icon_2.t3x");
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon_3.t3x", "icon_3.t3x");
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon_4.t3x", "icon_4.t3x");
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon_5.t3x", "icon_5.t3x");
				DownloadEshop("https://cdn.ghosteshop.com/script/ghosteshop-ds.unistore", -1, tmp, true, true);
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon-ds.t3x", "icon-ds.t3x");
				DownloadSpriteSheet("https://cdn.ghosteshop.com/script/icon-ds_1.t3x", "icon-ds_1.t3x");

				} else {
					notConnectedMsg();
				}
			}
		}
	}

	StoreUtils::store = std::make_unique<Store>(_STORE_PATH + config->lastStore(), config->lastStore());
	StoreUtils::ResetAll();
	StoreUtils::SortEntries(false, SortType::LAST_UPDATED);
	DisplayChangelog();
};

/*
	MainScreen Main Draw.
*/
void MainScreen::Draw(void) const {
	if (this->storeMode == 6) {
		/* Screenshot Menu. */
		StoreUtils::DrawScreenshotMenu(this->Screenshot, this->screenshotIndex, this->screenshotFetch, this->sSize, this->screenshotName, this->zoom, this->canDisplay);
		return;
	}

	if (this->storeMode == 7) {
		/* Release Notes. */
		StoreUtils::DrawReleaseNotes(this->scrollIndex, StoreUtils::entries[StoreUtils::store->GetEntry()]);
		GFX::DrawBottom();
		return;
	}

	Gui::ScreenDraw(Top);
	Gui::Draw_Rect(0, 0, 400, 25, UIThemes->BarColor());
	Gui::Draw_Rect(0, 25, 400, 1, UIThemes->BarOutline());

	if (StoreUtils::store && StoreUtils::store->GetValid()) Gui::DrawStringCentered(0, 1, 0.7f, UIThemes->TextColor(), StoreUtils::store->GetEshopTitle(), 360, 0, font);
	else Gui::DrawStringCentered(0, 1, 0.7f, UIThemes->TextColor(), Lang::get("INVALID_ESHOP"), 370, 0, font);
	config->list() ? StoreUtils::DrawList() : StoreUtils::DrawGrid();
	GFX::DrawTime();
	GFX::DrawBattery();
	GFX::DrawWifi();
	Animation::QueueEntryDone();

	/* Download-ception. */
	if (this->storeMode == 1) {
		StoreUtils::DrawDownList(this->dwnldList, this->fetchDown, StoreUtils::entries[StoreUtils::store->GetEntry()], this->dwnldSizes, this->installs);

	} else {
		if (fadeAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, fadeAlpha));
		GFX::DrawBottom();

		switch(this->storeMode) {
			case 0:
				/* Entry Info. */
				if (StoreUtils::store && StoreUtils::store->GetValid() && StoreUtils::entries.size() > 0) StoreUtils::DrawEntryInfo(StoreUtils::entries[StoreUtils::store->GetEntry()]);
				break;

			case 2:
				/* Queue Menu. */
				StoreUtils::DrawQueueMenu(this->queueIndex);
				break;

			case 3:
				/* Search + Favorites. */
				StoreUtils::DrawSearchMenu(this->searchIncludes, this->searchResult, this->marks, this->updateFilter, this->isAND);
				break;

			case 4:
				/* Sorting. */
				StoreUtils::DrawSorting(this->ascending, this->sorttype);
				break;

			case 5:
				/* Settings. */
				StoreUtils::DrawSettings(this->sPage, this->sSelection, this->sPos);
				break;
		}
	}

	StoreUtils::DrawSideMenu(this->storeMode);
	if (this->showMarks && StoreUtils::store && StoreUtils::store->GetValid()) StoreUtils::DisplayMarkBox(StoreUtils::entries[StoreUtils::store->GetEntry()]->GetMarks());
	if (fadeAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, fadeAlpha));
}

/*
	MainScreen Logic.
*/
void MainScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	Animation::HandleQueueEntryDone();
	GFX::HandleBattery();

	/* Screenshots Menu. */
	if (this->storeMode == 6) {
		if (this->screenshotFetch) {
			/* Delete Texture first. */
			if (this->Screenshot.tex) {
				C3D_TexDelete(this->Screenshot.tex);
				this->Screenshot.tex = nullptr;
				this->Screenshot.subtex = nullptr;
			}

			this->screenshotName = "";

			if (this->screenshotIndex < (int)StoreUtils::entries[StoreUtils::store->GetEntry()]->GetScreenshotNames().size()) {
				this->screenshotName = StoreUtils::entries[StoreUtils::store->GetEntry()]->GetScreenshotNames()[this->screenshotIndex];
			}

			this->sSize = 0;
			this->sSize = StoreUtils::entries[StoreUtils::store->GetEntry()]->GetScreenshots().size();

			if (this->screenshotIndex < this->sSize) {
				if (this->sSize > 0) {
					this->Screenshot = FetchScreenshot(StoreUtils::entries[StoreUtils::store->GetEntry()]->GetScreenshots()[this->screenshotIndex]);
					if (this->Screenshot.tex) this->canDisplay = true;
					else this->canDisplay = false;
				}
			}

			this->screenshotFetch = false;
		}

		StoreUtils::ScreenshotMenu(this->Screenshot, this->screenshotIndex, this->screenshotFetch, this->storeMode, this->sSize, this->zoom, this->canDisplay);
		return;
	}

	/* Release Notes. */
	if (this->storeMode == 7) {
		StoreUtils::ReleaseNotesLogic(this->scrollIndex, this->storeMode);
		return;
	}

	/* Mark Menu. */
	if (this->showMarks) StoreUtils::MarkHandle(StoreUtils::entries[StoreUtils::store->GetEntry()], this->showMarks);

	if (!this->showMarks) {
		if (storeMode == 0 || storeMode == 3 || storeMode == 4) {
			config->list() ? StoreUtils::ListLogic(storeMode, this->lastMode, this->fetchDown, this->smallDelay) : StoreUtils::GridLogic(storeMode, this->lastMode, this->fetchDown, this->smallDelay);
		}

		StoreUtils::SideMenuHandle(storeMode, this->fetchDown, this->lastMode);

		/* Fetch Download list. */
		if (this->fetchDown) {
			this->installs.clear();
			this->dwnldList.clear();
			this->dwnldSizes.clear();
			this->dwnldTypes.clear();

			if (StoreUtils::store && StoreUtils::store->GetValid()) {
				const std::vector<std::string> installedNames = StoreUtils::meta->GetInstalled(StoreUtils::store->GetEshopTitle(), StoreUtils::entries[StoreUtils::store->GetEntry()]->GetTitle());
				StoreUtils::store->SetDownloadIndex(0); // Reset to 0.
				StoreUtils::store->SetDownloadSIndex(0);

				if ((int)StoreUtils::entries.size() > StoreUtils::store->GetEntry()) {
					this->dwnldList = StoreUtils::store->GetDownloadList(StoreUtils::entries[StoreUtils::store->GetEntry()]->GetEntryIndex());
					this->dwnldSizes = StoreUtils::entries[StoreUtils::store->GetEntry()]->GetSizes();
					this->dwnldTypes = StoreUtils::entries[StoreUtils::store->GetEntry()]->GetTypes();

					for (int i = 0; i < (int)this->dwnldList.size(); i++) {
						bool good = false;

						for (int i2 = 0; i2 < (int)installedNames.size(); i2++) {
							if (installedNames[i2] == this->dwnldList[i]) {
								this->installs.push_back( true );
								good = true;
							}
						}

						if (!good) this->installs.push_back( false );
					}
				}
			}

			this->fetchDown = false;
		}

		switch(storeMode) {
			case 0:
				if (StoreUtils::store && StoreUtils::store->GetValid() && StoreUtils::entries.size() > 0) StoreUtils::EntryHandle(this->showMarks, this->fetchDown, this->screenshotFetch, storeMode, StoreUtils::entries[StoreUtils::store->GetEntry()]);
				break;

			case 1:
				if (StoreUtils::store && StoreUtils::store->GetValid() && StoreUtils::entries.size() > 0) StoreUtils::DownloadHandle(StoreUtils::entries[StoreUtils::store->GetEntry()], this->dwnldList, storeMode, this->lastMode, this->smallDelay, this->installs, this->dwnldTypes);
				break;

			case 2:
				StoreUtils::QueueMenuHandle(this->queueIndex, this->storeMode);
				break;

			case 3:
				StoreUtils::SearchHandle(this->searchIncludes, this->searchResult, this->marks, this->updateFilter, this->ascending, this->sorttype, this->isAND);
				break;

			case 4:
				StoreUtils::SortHandle(this->ascending, this->sorttype);
				break;

			case 5:
				StoreUtils::SettingsHandle(this->sPage, this->showSettings, storeMode, this->sSelection, this->sPos);
				break;
		}
	}
}