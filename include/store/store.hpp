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

#ifndef _GHOST_ESHOP_STORE_HPP
#define _GHOST_ESHOP_STORE_HPP

#include "json.hpp"
#include <citro2d.h>
#include <string>

class Store {
public:
	Store(const std::string &file, const std::string &file2, bool ARGMode = false);
	~Store();
	void LoadFromFile(const std::string &file);
	void loadSheets();
	void unloadSheets();
	void update(const std::string &file);

	/* Get Information of the eShop itself. */
	std::string GetEshopTitle() const;
	std::string GetEshopAuthor() const;

	/* Get Information of the eShop entries. */
	std::string GetTitleEntry(int index) const;
	std::string GetAuthorEntry(int index) const;
	std::string GetDescriptionEntry(int index) const;
	std::vector<std::string> GetCategoryIndex(int index) const;
	std::string GetVersionEntry(int index) const;
	std::vector<std::string> GetConsoleEntry(int index) const;
	std::string GetLastUpdatedEntry(int index) const;
	std::string GetAdditionalcontentEntry(int index) const;
	C2D_Image GetIconEntry(int index) const;
	std::string GetFileSizes(int index, const std::string &entry) const;
	std::string GetFileTypes(int index, const std::string &entry) const;
	std::vector<std::string> GetScreenshotList(int index) const;
	std::vector<std::string> GetScreenshotNames(int index) const;
	std::string GetReleaseNotes(int index) const;

	std::vector<std::string> GetDownloadList(int index) const;

	int GetStoreSize() const { return (int)this->storeJson["storeContent"].size(); };

	int GetScreenIndx() const { return this->screenIndex; };
	void SetScreenIndx(int v) { this->screenIndex = v; };

	int GetEntry() const { return this->entry; };
	void SetEntry(int v) { this->entry = v; };

	int GetBox() const { return this->box; };
	void SetBox(int v) { this->box = v; };

	int GetDownloadSIndex() const { return this->downIndex; };
	void SetDownloadSIndex(int v) { this->downIndex = v; };

	int GetDownloadIndex() const { return this->downEntry; };
	void SetDownloadIndex(int v) { this->downEntry = v; };

	nlohmann::json &GetJson() { return this->storeJson; };
	bool GetValid() const { return this->valid; };

	/* Both of these things are used for custom BG support. */
	C2D_Image GetStoreImg() const { return this->storeBG; };
	bool customBG() const { return this->hasCustomBG; };

	/* Return filename of the UniStore. */
	std::string GetFileName() const { return this->fileName; };
private:
	void SetC2DBGImage();
	nlohmann::json storeJson = nullptr;
	std::vector<C2D_SpriteSheet> sheets;
	C2D_Image storeBG = { nullptr };
	bool valid = false, hasSheet = false, hasCustomBG = false;
	int screenIndex = 0, entry = 0, box = 0, downEntry = 0, downIndex = 0;
	std::string fileName = "";
};

#endif