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

#ifndef _GHOST_ESHOP_SCRIPT_UTILS_HPP
#define _GHOST_ESHOP_SCRIPT_UTILS_HPP

#include "json.hpp"
#include <3ds.h>
#include <string>

enum ScriptState {
	NONE = 0,
	FAILED_DOWNLOAD,
	SCRIPT_CANCELED,
	SYNTAX_ERROR,
	COPY_ERROR,
	MOVE_ERROR,
	DELETE_ERROR,
	EXTRACT_ERROR
};

namespace ScriptUtils {
	bool matchPattern(const std::string &pattern, const std::string &tested);

	Result removeFile(const std::string &file, bool isARG = false);
	void bootTitle(const std::string &TitleID, bool isNAND, bool isARG = false);
	Result prompt(const std::string &message);
	Result copyFile(const std::string &source, const std::string &destination, const std::string &message, bool isARG = false);
	Result renameFile(const std::string &oldName, const std::string &newName, bool isARG = false);
	Result downloadRelease(const std::string &repo, const std::string &file, const std::string &output, bool includePrereleases, const std::string &message, bool isARG = false);
	Result downloadFile(const std::string &file, const std::string &output, const std::string &message, bool isARG = false);
	void installFile(const std::string &file, bool updatingSelf, const std::string &message, bool isARG = false);
	Result extractFile(const std::string &file, const std::string &input, const std::string &output, const std::string &message, bool isARG = false);

	Result runFunctions(nlohmann::json storeJson, int selection, const std::string &entry);
};

#endif