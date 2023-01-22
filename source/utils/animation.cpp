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
#include "queueSystem.hpp"
#include "stringutils.hpp"
#include <curl/curl.h>

extern int filesExtracted, extractFilesCount;
extern std::string extractingFile;
char progressBarMsg[128] = "";
bool showProgressBar = false;
ProgressBar progressbarType = ProgressBar::Downloading;
int Animation::DisplayY = 240, Animation::DisplayDelay = 3 * 60;
bool Animation::MoveUp = true, Animation::DoDelay = false;

extern u32 extractSize, writeOffset;
extern u32 installSize, installOffset;
extern u32 copyOffset, copySize;

extern curl_off_t downloadTotal;
extern curl_off_t downloadNow;

/*
	Draw the progressbar.

	u64 currentProgress: The current progress.
	u64 totalProgress: The total progress.
*/
void Animation::DrawProgressBar(u64 currentProgress, u64 totalProgress) {
	Gui::Draw_Rect(30, 120, 342, 30, UIThemes->ProgressbarOut());
	Gui::Draw_Rect(31, 121, (int)(((float)currentProgress / (float)totalProgress) * 338.0f), 28, UIThemes->ProgressbarIn());
}

/*
	Display the progressbar.
*/
void Animation::displayProgressBar() {
	char str[256];

	while(showProgressBar) {
		switch(progressbarType) {
			case ProgressBar::Downloading:
				if (downloadTotal < 1.0f) downloadTotal = 1.0f;
				if (downloadTotal < downloadNow) downloadTotal = downloadNow;

				snprintf(str, sizeof(str), "%s / %s (%.2f%%)",
						StringUtils::formatBytes(downloadNow).c_str(),
						StringUtils::formatBytes(downloadTotal).c_str(),
						((float)downloadNow/(float)downloadTotal) * 100.0f);
				break;

			case ProgressBar::Extracting:
				snprintf(str, sizeof(str), "%s / %s (%.2f%%)",
						StringUtils::formatBytes(writeOffset).c_str(),
						StringUtils::formatBytes(extractSize).c_str(),
						((float)writeOffset/(float)extractSize) * 100.0f);
				break;

			case ProgressBar::Installing:
				snprintf(str, sizeof(str), "%s / %s (%.2f%%)",
						StringUtils::formatBytes(installOffset).c_str(),
						StringUtils::formatBytes(installSize).c_str(),
						((float)installOffset/(float)installSize) * 100.0f);
				break;

			case ProgressBar::Copying:
				snprintf(str, sizeof(str), "%s / %s (%.2f%%)",
						StringUtils::formatBytes(copyOffset).c_str(),
						StringUtils::formatBytes(copySize).c_str(),
						((float)copyOffset/(float)copySize) * 100.0f);
				break;
		}

		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, TRANSPARENT);
		C2D_TargetClear(Bottom, TRANSPARENT);
		GFX::DrawTop();
		Gui::DrawStringCentered(0, 1, 0.7f, UIThemes->TextColor(), progressBarMsg, 390, 0, font);

		switch(progressbarType) {
			case ProgressBar::Downloading:
				Gui::DrawStringCentered(0, 80, 0.6f, UIThemes->TextColor(), str, 390, 0, font);
				Animation::DrawProgressBar(downloadNow, downloadTotal);
				break;

			case ProgressBar::Extracting:
				Gui::DrawStringCentered(0, 180, 0.6f, UIThemes->TextColor(), str, 390, 0, font);
				Gui::DrawStringCentered(0, 100, 0.6f, UIThemes->TextColor(), std::to_string(filesExtracted) + " / " + std::to_string(extractFilesCount) + " " + (filesExtracted == 1 ? (Lang::get("FILE_EXTRACTED")).c_str() :(Lang::get("FILES_EXTRACTED"))), 390, 0, font);
				Gui::DrawStringCentered(0, 40, 0.6f, UIThemes->TextColor(), Lang::get("CURRENTLY_EXTRACTING"), 390, 0, font);
				Gui::DrawStringCentered(0, 70, 0.6f, UIThemes->TextColor(), extractingFile, 390, 0, font);
				Animation::DrawProgressBar(writeOffset, extractSize);
				break;

			case ProgressBar::Installing:
				Gui::DrawStringCentered(0, 80, 0.6f, UIThemes->TextColor(), str, 390, 0, font);
				Animation::DrawProgressBar(installOffset, installSize);
				break;

			case ProgressBar::Copying:
				Gui::DrawStringCentered(0, 80, 0.6f, UIThemes->TextColor(), str, 390, 0, font);
				Animation::DrawProgressBar(copyOffset, copySize);
				break;
		}

		GFX::DrawBottom();
		C3D_FrameEnd(0);
	}
}

static int frame = 0; // 0 - 7.
static int advanceFrame = 0; // Only animate every 4 frames.
extern bool QueueRuns;
extern std::deque<std::unique_ptr<Queue>> queueEntries;

void Animation::DrawQueue(int x, int y) {
	GFX::DrawIcon(sprites_queue0_idx + frame, x, y);
	Gui::DrawStringCentered(x + 20 - 160, y + 11, 0.6f, UIThemes->SideBarIconColor(), QueueSystem::Wait ? "!" : std::to_string(queueEntries.size()), 0, 0, font);
}
void Animation::QueueAnimHandle() {
	if (QueueRuns) {
		advanceFrame = (advanceFrame + 1) % 4;
		if (advanceFrame == 0) frame = (frame + 1) % 8;
	}
}

#define DISPLAYBOX_UP 206
#define DISPLAYBOX_DOWN 240

void Animation::QueueEntryDone() {
	if (QueueSystem::Popup) {
		Gui::Draw_Rect(0, DisplayY, 400, 34, UIThemes->DownListPrev());

		if (QueueSystem::EndMsg != "") {
			Gui::DrawStringCentered(0, DisplayY + 8, 0.6f, UIThemes->TextColor(), QueueSystem::EndMsg, 395, 0, font);
		}
	}
}
void Animation::HandleQueueEntryDone() {
	if (QueueSystem::Popup) {
		if (!Animation::DoDelay) {
			if (Animation::MoveUp) {
				if (Animation::DisplayY > DISPLAYBOX_UP) {
					Animation::DisplayY--;

					if (Animation::DisplayY <= DISPLAYBOX_UP) {
						Animation::DisplayDelay = 3 * 60;
						Animation::DoDelay = true;
					}
				}

			} else {
				if (Animation::DisplayY < DISPLAYBOX_DOWN) {
					Animation::DisplayY++;

					if (Animation::DisplayY >= DISPLAYBOX_DOWN) {
						QueueSystem::Popup = false;
						Animation::MoveUp = true;
					}
				}
			}

		} else {
			if (Animation::DisplayDelay > 0) {
				Animation::DisplayDelay--;

				if (Animation::DisplayDelay <= 0) {
					Animation::MoveUp = false;
					Animation::DoDelay = false;
				}
			}
		}
	}
}