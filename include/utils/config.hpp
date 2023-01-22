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

#ifndef _GHOST_ESHOP_CONFIG_HPP
#define _GHOST_ESHOP_CONFIG_HPP

#include "json.hpp"

#include <3ds.h>
#include <string>

class Config {
public:
	Config();
	void save();
	void initialize();
	void sysLang();

	/* Language. */
	std::string language() const { return this->v_language; };
	void language(const std::string &v) { this->v_language = v; if (!this->changesMade) this->changesMade = true; };

	/* Last Store. */
	std::string lastStore() const { return this->v_lastStore; };
	void lastStore(const std::string &v) { this->v_lastStore = v; if (!this->changesMade) this->changesMade = true; };

	/* Using Top List. */
	bool list() const { return this->v_list; };
	void list(bool v) { this->v_list = v; if (!this->changesMade) this->changesMade = true; };

	/* Auto update on boot. */
	bool autoupdate() const { return this->v_autoUpdate; };
	void autoupdate(bool v) { this->v_autoUpdate = v; if (!this->changesMade) this->changesMade = true; };

	bool _3dsxInFolder() const { return this->v_3dsxInFolder; };
	void _3dsxInFolder(bool v) { this->v_3dsxInFolder = v; if (!this->changesMade) this->changesMade = true; };

	std::string _3dsxPath() const { return this->v_3dsxPath; };
	void _3dsxPath(const std::string &v) { this->v_3dsxPath = v; if (!this->changesMade) this->changesMade = true; };

	std::string ndsPath() const { return this->v_ndsPath; };
	void ndsPath(const std::string &v) { this->v_ndsPath = v; if (!this->changesMade) this->changesMade = true; };

	std::string archPath() const { return this->v_archivePath; };
	void archPath(const std::string &v) { this->v_archivePath = v; if (!this->changesMade) this->changesMade = true; };

	std::string firmPath() const { return this->v_firmPath; };
	void firmPath(const std::string &v) { this->v_firmPath = v; if (!this->changesMade) this->changesMade = true; };

	/* Fetching old metadata. */
	bool metadata() const { return this->v_metadata; };
	void metadata(bool v) { this->v_metadata = v; if (!this->changesMade) this->changesMade = true; };

	/* Ghost eShop Update check on startup. */
	bool updatecheck() const { return this->v_updateCheck; };
	void updatecheck(bool v) { this->v_updateCheck = v; if (!this->changesMade) this->changesMade = true; };

	/* Check for nightly/release on startup. */
	bool updatenightly() const { return this->v_updateNightly; };
	void updatenightly(bool v) { this->v_updateNightly = v; if (!this->changesMade) this->changesMade = true; };

	/* Ghost eShop Update check on startup. */
	bool usebg() const { return this->v_showBg; };
	void usebg(bool v) { this->v_showBg = v; if (!this->changesMade) this->changesMade = true; };

	/* If using custom Font. */
	bool customfont() const { return this->v_customFont; };
	void customfont(bool v) { this->v_customFont = v; if (!this->changesMade) this->changesMade = true; };

	/* The shortcut path. */
	std::string shortcut() const { return this->v_shortcutPath; };
	void shortcut(const std::string &v) { this->v_shortcutPath = v; if (!this->changesMade) this->changesMade = true; };

	/* If displaying changelog. */
	bool changelog() const { return this->v_changelog; };
	void changelog(bool v) { this->v_changelog = v; if (!this->changesMade) this->changesMade = true; };

	/* The active Theme. */
	std::string theme() const { return this->v_theme; };
	void theme(const std::string &v) { this->v_theme = v; if (!this->changesMade) this->changesMade = true; };

	/* If showing prompt if action failed / succeeded. */
	bool prompt() const { return this->v_prompt; };
	void prompt(bool v) { this->v_prompt = v; if (!this->changesMade) this->changesMade = true; };
private:
	/* Mainly helper. */
	bool getBool(const std::string &key);
	void setBool(const std::string &key, bool v);
	int getInt(const std::string &key);
	void setInt(const std::string &key, int v);
	std::string getString(const std::string &key);
	void setString(const std::string &key, const std::string &v);

	nlohmann::json json;
	bool changesMade = false;

	std::string v_language = "en", v_lastStore = "ghosteshop.unistore",
				v_3dsxPath = "sdmc:/3ds", v_ndsPath = "sdmc:/roms/nds", v_archivePath = "sdmc:",
				v_shortcutPath = "sdmc:/3ds/Universal-Updater/shortcuts", v_firmPath = "sdmc:/luma/payloads", v_theme = "Default";

	bool v_list = false, v_autoUpdate = true, v_metadata = true, v_updateCheck = true, v_updateNightly = false,
		v_showBg = false, v_customFont = false, v_changelog = true, v_prompt = true, v_3dsxInFolder = false;
};

#endif
