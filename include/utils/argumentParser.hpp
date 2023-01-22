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

#ifndef _GHOST_ESHOP_ARGUMENT_PARSER_HPP
#define _GHOST_ESHOP_ARGUMENT_PARSER_HPP

#include "json.hpp"
#include "store.hpp"
#include <string>

class ArgumentParser {
public:
	ArgumentParser(const std::string &file, const std::string &entry, int dlIndex);
	void Load();
	void Execute();
	bool GetValid() const { return this->isValid; };
private:
	std::unique_ptr<Store> store = nullptr;
	bool isValid = false;
	std::string file = "", executeEntry = "", entry = "";
	int dlIndex = -1, entryIndex = -1;
};

#endif