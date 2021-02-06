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
#include "fileBrowse.hpp"
#include "meta.hpp"
#include <unistd.h>

/*
	Le Constructeur de la Meta.
	Inclut la création de fichier MetaData, si elle n’existe pas.
*/
Meta::Meta() {
	if (access(_META_PATH, F_OK) != 0) {
		FILE *temp = fopen(_META_PATH, "w");
		char tmp[2] = { '{', '}' };
		fwrite(tmp, sizeof(tmp), 1, temp);
		fclose(temp);
	}

	FILE *temp = fopen(_META_PATH, "rt");
	this->metadataJson = nlohmann::json::parse(temp, nullptr, false);
	fclose(temp);

	if (config->metadata()) this->ImportMetadata();
}

/*
	Importez les anciennes métadonnées du fichier 'updates.json'.
*/
void Meta::ImportMetadata() {
	if (access("sdmc:/3ds/GhosteShop/updates.json", F_OK) != 0) {
		config->metadata(false);
		return; // Introuvables.
	}

	Msg::DisplayMsg(Lang::get("FETCHING_METADATA"));
	FILE *old = fopen("sdmc:/3ds/GhosteShop/updates.json", "r");
	nlohmann::json oldJson = nlohmann::json::parse(old, nullptr, false);
	fclose(old);

	std::vector<EshopInfo> info = GetEshopInfo(_STORE_PATH); // Va chercher eShop.

	for (int i = 0; i < (int)info.size(); i++) {
		if (info[i].Title != "" && oldJson.contains(info[i].FileName)) {
			for(auto it = oldJson[info[i].FileName].begin(); it != oldJson[info[i].FileName].end(); ++it) {
				this->SetUpdated(info[i].Title, it.key().c_str(), it.value().get<std::string>());
			}
		}
	}

	config->metadata(false);
}

/*
	Obtenir la dernière mise à jour.
	const std::string &EshopName : Le nom eShop.
	const std::string &entry : Le nom de l’entrée.
*/
std::string Meta::GetUpdated(const std::string &EshopName, const std::string &entry) const {
	if (!this->metadataJson.contains(EshopName)) return ""; // Le nom de l'eShop n’existe pas.

	if (!this->metadataJson[EshopName].contains(entry)) return ""; // L'entrée n'existe pas.

	if (!this->metadataJson[EshopName][entry].contains("updated")) return ""; // Les mises à jour n'existe pas.

	if (this->metadataJson[EshopName][entry]["updated"].is_string()) return this->metadataJson[EshopName][entry]["updated"];
	return "";
}

/*
	Prends les marques.
	const std::string &EshopName : Le nom eShop.
	const std::string &entry : Le nom de l’entrée.
*/
int Meta::GetMarks(const std::string &EshopName, const std::string &entry) const {
	int temp = 0;

	if (!this->metadataJson.contains(EshopName)) return temp; // Le nom de l'eShop n’existe pas.

	if (!this->metadataJson[EshopName].contains(entry)) return temp; // L'entrée n'existe pas.

	if (!this->metadataJson[EshopName][entry].contains("marks")) return temp; // Les marques n'existe pas.

	if (this->metadataJson[EshopName][entry]["marks"].is_number()) return this->metadataJson[EshopName][entry]["marks"];
	return temp;
}

/*
	Retourner, si la mise à jour est disponible.
	const std::string &EshopName : Le nom eShop.
	const std::string &entry : Le nom de l’entrée.
	const std::string &updated : Compare pour la mise à jour.
*/
bool Meta::UpdateAvailable(const std::string &EshopName, const std::string &entry, const std::string &updated) const {
	if (this->GetUpdated(EshopName, entry) != "" && updated != "") {
		return strcasecmp(updated.c_str(), this->GetUpdated(EshopName, entry).c_str()) > 0;
	}

	return false;
}

/*
	L’appel de sauvegarde.
	Ecrire au fichier.. appelé sur destructor.
*/
void Meta::SaveCall() {
	FILE *file = fopen(_META_PATH, "wb");
	const std::string dump = this->metadataJson.dump(1, '\t');
	fwrite(dump.c_str(), 1, dump.size(), file);
	fclose(file);
}