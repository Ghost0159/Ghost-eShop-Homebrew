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

#ifndef _GHOST_ESHOP_STORE_ENTRY_HPP
#define _GHOST_ESHOP_STORE_ENTRY_HPP

#include "meta.hpp"
#include "store.hpp"
#include "stringutils.hpp"

class StoreEntry {
public:
	StoreEntry(const std::unique_ptr<Store> &store, const std::unique_ptr<Meta> &meta, int index);

	std::string GetTitle() const { return this->Title; };
	std::string GetAuthor() const { return this->Author; };
	std::string GetDescription() const { return this->Description; };
	std::string GetCategory() const { return this->Category; };
	std::string GetVersion() const { return this->Version; };
	std::string GetConsole() const { return this->Console; };
	std::string GetLastUpdated() const { return this->LastUpdated; };
	std::string GetAdditionalcontent() const { return this->Additional; };
	int GetMarks() const { return this->Marks; };

	C2D_Image GetIcon() const { return this->Icon; };

	int GetSheetIndex() const { return this->SheetIndex; };
	int GetEntryIndex() const { return this->EntryIndex; };

	std::vector<std::string> GetCategoryFull() const { return this->FullCategory; };
	std::vector<std::string> GetConsoleFull() const { return this->FullConsole; };
	std::vector<std::string> GetSizes() const { return this->Sizes; };
	std::vector<std::string> GetTypes() const { return this->Types; };
	std::vector<std::string> GetScreenshots() const { return this->Screenshots; };
	std::vector<std::string> GetScreenshotNames() const { return this->ScreenshotNames; };
	std::string GetReleaseNotes() const { return this->ReleaseNotes; };

	bool GetUpdateAvl() const { return this->UpdateAvailable; };
	void SetUpdateAvl(bool v) { this->UpdateAvailable = v; };

	void SetMark(int marks) {
		this->Marks = marks;
		this->MarkString = StringUtils::GetMarkString(this->Marks);
	};

private:
	std::string Title, Author, Description, Category, Version, Console, LastUpdated, MarkString, Additional, ReleaseNotes;
	C2D_Image Icon;
	int SheetIndex, EntryIndex, Marks;
	std::vector<std::string> FullCategory, FullConsole, Sizes, Types, Screenshots, ScreenshotNames;
	bool UpdateAvailable;
};

#endif