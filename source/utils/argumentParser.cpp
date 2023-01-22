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

#include "argumentParser.hpp"
#include "common.hpp"
#include "scriptUtils.hpp"
#include <unistd.h>

/*
	The constructor of the Argument Parser.

	const std::string &file: Const Reference to the file.
	const std::string &entry: Const Reference to the Entry Title name.
	int dlIndex: The Download index.
*/
ArgumentParser::ArgumentParser(const std::string &file, const std::string &entry, int dlIndex) {
	if (dlIndex != -1 && file != "") {
		this->file = file;
		this->entry = entry;
		this->dlIndex = dlIndex;

		this->Load();
	}
}

/*
	Prepare UniStore and get valid state.
*/
void ArgumentParser::Load() {
	if (access((std::string(_STORE_PATH) + this->file).c_str(), F_OK) != 0) return;

	this->store = std::make_unique<Store>(_STORE_PATH + this->file, this->file, true);
	if (!this->store->GetValid()) return;

	for (int i = 0; i < this->store->GetStoreSize(); i++) {
		if (this->store->GetTitleEntry(i) == this->entry) {
			this->entryIndex = i;
			const std::vector<std::string> dlList = this->store->GetDownloadList(this->entryIndex);

			if (dlList.empty()) return;

			if ((int)dlList.size() >= this->dlIndex) {
				this->executeEntry = dlList[this->dlIndex];
				this->isValid = true;
				return;
			}
		}
	}
}

/*
	Execute the Argument's entry, if valid.
*/
void ArgumentParser::Execute() {
	if (this->isValid) {
		if (Msg::promptMsg(Lang::get("EXECUTE_ENTRY") + "\n\n" + this->executeEntry)) {
			ScriptUtils::runFunctions(this->store->GetJson(), this->entryIndex, this->executeEntry);
		}
	}
}