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

#include "storeEntry.hpp"

/*
	Fetch informations on constructor.

	const std::unique_ptr<Store> &store: Const Reference to the store class.
	const std::unique_ptr<Meta> &meta: Const Reference to the meta class.
	int index: Index of the entry.
*/
StoreEntry::StoreEntry(const std::unique_ptr<Store> &store, const std::unique_ptr<Meta> &meta, int index) {
	this->Title = store->GetTitleEntry(index);
	this->Author = store->GetAuthorEntry(index);

	this->Description = store->GetDescriptionEntry(index);

	this->Category = StringUtils::FetchStringsFromVector(store->GetCategoryIndex(index));
	this->Version = store->GetVersionEntry(index);
	this->Console = StringUtils::FetchStringsFromVector(store->GetConsoleEntry(index));
	this->LastUpdated = store->GetLastUpdatedEntry(index);
	this->Additional = store->GetAdditionalcontentEntry(index);
	this->MarkString = StringUtils::GetMarkString(meta->GetMarks(store->GetEshopTitle(), this->Title));

	this->Icon = store->GetIconEntry(index);
	this->SheetIndex = 0;
	this->EntryIndex = index;

	this->FullCategory = store->GetCategoryIndex(index);
	this->FullConsole = store->GetConsoleEntry(index);

	this->UpdateAvailable = meta->UpdateAvailable(store->GetEshopTitle(), this->Title, store->GetLastUpdatedEntry(index));
	this->Marks = meta->GetMarks(store->GetEshopTitle(), this->Title);

	const std::vector<std::string> entries = store->GetDownloadList(index);

	if (!entries.empty()) {
		for (int i = 0; i < (int)entries.size(); i++) {
			this->Sizes.push_back( store->GetFileSizes(index, entries[i]) );
			this->Types.push_back( store->GetFileTypes(index, entries[i]) );
		}
	}

	this->Screenshots = store->GetScreenshotList(index);
	this->ScreenshotNames = store->GetScreenshotNames(index);
	this->ReleaseNotes = store->GetReleaseNotes(index);
}