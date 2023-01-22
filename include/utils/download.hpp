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

#ifndef _GHOST_ESHOP_DOWNLOAD_HPP
#define _GHOST_ESHOP_DOWNLOAD_HPP

#include "common.hpp"

#define APP_TITLE "Ghost eShop"
#define VERSION_STRING "13.5"

enum DownloadError {
	DL_ERROR_NONE = 0,
	DL_ERROR_WRITEFILE,
	DL_ERROR_ALLOC,
	DL_ERROR_STATUSCODE,
	DL_ERROR_GIT,
	DL_CANCEL, // No clue if that's needed tho.
};

struct StoreList {
	std::string Title;
	std::string Author;
	std::string URL;
	std::string Description;
};

struct GEUpdate {
	bool Available = false;
	std::string Notes = "";
	std::string Version = "";
};

Result downloadToFile(const std::string &url, const std::string &path);
Result downloadFromRelease(const std::string &url, const std::string &asset, const std::string &path, bool includePrereleases);

/*
	Check Wi-Fi status.
	@return True if Wi-Fi is connected; false if not.
*/
bool checkWifiStatus(void);

/*
	Display "Please connect to Wi-Fi" for 2s.
*/
void notConnectedMsg(void);

/*
	Display "Not Implemented Yet" for 2s.
*/
void notImplemented(void);

/*
	Display the done msg.
*/
void doneMsg(void);

bool IsUpdateAvailable(const std::string &URL, int revCurrent);
bool DownloadEshop(const std::string &URL, int currentRev, std::string &fl, bool isDownload = false, bool isEDB = false);
bool DownloadSpriteSheet(const std::string &URL, const std::string &file);
GEUpdate IsGEUpdateAvailable();
void UpdateAction();
std::vector<StoreList> FetchStores();
C2D_Image FetchScreenshot(const std::string &URL);
std::string GetChangelog();

#endif