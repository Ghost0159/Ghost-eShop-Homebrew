/*
*   This file is part of Universal-Updater
*   Copyright (C) 2019-2020 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "common.hpp"
#include "config.hpp"
#include "json.hpp"
#include <string>
#include <unistd.h>

/*
	Détecte la langue du système et est utilisé plus tard pour définir la langue de l’application à la langue du système.
*/
void Config::sysLang() {
	u8 language = 1;
	CFGU_GetSystemLanguage(&language);

	switch(language) {
		case 0:
			this->language("jp"); // Japonais (日本人), traduit.
			break;

		case 1:
			this->language("en"); // Anglais (English), traduit.
			break;

		case 2:
			this->language("fr"); // Français (Français), traduit.
			break;

		case 3:
			this->language("de"); // Allemand (Deutsche), traduit.
			break;

		case 4:
			this->language("it"); // Italien (Italiano), traduit.
			break;

		case 5:
			this->language("es"); // Espagnol (Español), traduit.
			break;

		case 6:
			this->language("cn-SI"); // Chinois simplifié (简体中文), traduit.
			break;

		case 7:
			this->language("kr"); // Coréen (한국어), traduit.
			break;

		case 8:
			this->language("nl"); // Néerlandais (Nederlands), pas totalement traduit.
			break;

		case 9:
			this->language("pt"); // Portugais (Português), pas totalement traduit.
			break;

		case 10:
			this->language("ru"); // Russe (русский), pas totalement traduit.
			break;

		case 11:
			this->language("cn-TR"); // chinois traditionnel (繁體中文), traduit.
			break;
	}
}

/*
	Au cas où ça n’existe pas.
*/
void Config::initialize() {
	FILE *temp = fopen("sdmc:/3ds/GhosteShop/Config.json", "w");
	char tmp[2] = { '{', '}' };
	fwrite(tmp, sizeof(tmp), 1, temp);
	fclose(temp);
}

/*
	Constructeur de la configuration.
*/
Config::Config() {
	if (access("sdmc:/3ds/GhosteShop/Config.json", F_OK) != 0) {
		this->initialize();
	}

	FILE *file = fopen("sdmc:/3ds/GhosteShop/Config.json", "r");
	this->json = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

	/* Créons-en une nouvelle. */
	if (!this->json.contains("Version")) this->initialize();

	if (!this->json.contains("Language")) this->sysLang();
	else this->language(this->getString("Language"));

	if (this->json.contains("LastStore")) this->lastStore(this->getString("LastStore"));
	if (this->json.contains("List")) this->list(this->getBool("List"));
	if (this->json.contains("AutoUpdate")) this->autoupdate(this->getBool("AutoUpdate"));
	if (this->json.contains("_3DSX_Path")) this->_3dsxPath(this->getString("_3DSX_Path"));
	if (this->json.contains("NDS_Path")) this->ndsPath(this->getString("NDS_Path"));
	if (this->json.contains("Archive_Path")) this->archPath(this->getString("Archive_Path"));
	if (this->json.contains("MetaData")) this->metadata(this->getBool("MetaData"));
	if (this->json.contains("UpdateCheck")) this->updatecheck(this->getBool("UpdateCheck"));
	if (this->json.contains("UseBG")) this->usebg(this->getBool("UseBG"));
	if (this->json.contains("CustomFont")) this->customfont(this->getBool("CustomFont"));
	if (this->json.contains("Shortcut_Path")) this->shortcut(this->getString("Shortcut_Path"));
	if (this->json.contains("Display_Changelog")) this->changelog(this->getBool("Display_Changelog"));

	this->changesMade = false; // Aucune modification n’a encore été apportée.
}

/* Ecrivez à config si changesMade. */
void Config::save() {
	if (this->changesMade) {
		FILE *file = fopen("sdmc:/3ds/GhosteShop/Config.json", "w");

		/* Valeurs Théoriques. */
		this->setString("Language", this->language());
		this->setInt("Version", 1);
		this->setString("LastStore", this->lastStore());
		this->setBool("List", this->list());
		this->setBool("AutoUpdate", this->autoupdate());
		this->setString("_3DSX_Path", this->_3dsxPath());
		this->setString("NDS_Path", this->ndsPath());
		this->setString("Archive_Path", this->archPath());
		this->setBool("MetaData", this->metadata());
		this->setBool("UpdateCheck", this->updatecheck());
		this->setBool("UseBG", this->usebg());
		this->setBool("CustomFont", this->customfont());
		this->setString("Shortcut_Path", this->shortcut());
		this->setBool("Display_Changelog", this->changelog());

		/* Écrire les modifications au fichier. */
		const std::string dump = this->json.dump(1, '\t');
		fwrite(dump.c_str(), 1, this->json.dump(1, '\t').size(), file);
		fclose(file);
	}
}

/* Fonctions d’aide. */
bool Config::getBool(const std::string &key) {
	if (!this->json.contains(key)) return false;

	return this->json.at(key).get_ref<const bool &>();
}
void Config::setBool(const std::string &key, bool v) { this->json[key] = v; };

int Config::getInt(const std::string &key) {
	if (!this->json.contains(key)) return 0;

	return this->json.at(key).get_ref<const int64_t &>();
}
void Config::setInt(const std::string &key, int v) { this->json[key] = v; };

std::string Config::getString(const std::string &key) {
	if (!this->json.contains(key)) return "";

	return this->json.at(key).get_ref<const std::string &>();
}
void Config::setString(const std::string &key, const std::string &v) { this->json[key] = v; };