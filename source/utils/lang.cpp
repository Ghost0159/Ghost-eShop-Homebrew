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

#include "lang.hpp"
#include <stdio.h>
#include <unistd.h>

static nlohmann::json appJson;

std::string Lang::get(const std::string &key) {
	if (!appJson.contains(key)) return key;

	return appJson.at(key).get_ref<const std::string&>();
}

void Lang::load(const std::string &lang) {
	FILE *values;

	/* Check if exist. */
	if (access(("romfs:/lang/" + lang + "/app.json").c_str(), F_OK) == 0) {
		values = fopen(("romfs:/lang/" + lang + "/app.json").c_str(), "rt");
		if (values) {
			appJson = nlohmann::json::parse(values, nullptr, false);
			fclose(values);
		}
		if (appJson.is_discarded())
			appJson = { };
		return;

	} else {
		values = fopen("romfs:/lang/en/app.json", "rt");
		if (values) {
			appJson = nlohmann::json::parse(values, nullptr, false);
			fclose(values);
		}
		if (appJson.is_discarded())
			appJson = { };
		return;
	}
}