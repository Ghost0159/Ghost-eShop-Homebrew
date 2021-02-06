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
#include "download.hpp"
#include "gui.hpp"
#include "scriptUtils.hpp"
#include "store.hpp"
#include <unistd.h>

extern C2D_SpriteSheet sprites;
extern bool checkWifiStatus();
static bool firstStart = true;

/*
	Initialiser un magasin.
	const std::string &file : Le fichier eShop.
	const std::string &file2 : Le fichier eShop.. sans chemin complet.
	bool ARGMode : mode If Argument.
*/
Store::Store(const std::string &file, const std::string &file2, bool ARGMode) {
	this->fileName = file2;

	if (!ARGMode) {
		this->update(file);
		this->SetC2DBGImage();

	} else {
		this->LoadFromFile(file);
	}
};

/*
	Mettre à jour un eShop,, y compris SpriteSheet, si la révision a augmenté.
	const std::string &file : Const Référence au nomfichier.
*/
void Store::update(const std::string &file) {
	bool doSheet = false;
	this->LoadFromFile(file);

	int rev = -1;

	/* Ne faites cela que si elle est valide. */
	if (this->valid) {
		if (this->storeJson["storeInfo"].contains("revision") && this->storeJson["storeInfo"]["revision"].is_number()) {
			rev = this->storeJson["storeInfo"]["revision"];
		}

		/* Premières exceptions de démarrage. */
		if (firstStart) {
			firstStart = false;

			if (!config->autoupdate()) {
				this->loadSheets();
				return;
			}
		}

		if (this->storeJson.contains("storeInfo")) {
			/* Vérification... */
			if (checkWifiStatus()) { // Seulement si le WiFi est disponible.
				if (this->storeJson["storeInfo"].contains("url") && this->storeJson["storeInfo"]["url"].is_string()) {
					if (this->storeJson["storeInfo"].contains("file") && this->storeJson["storeInfo"]["file"].is_string()) {

						const std::string fl = this->storeJson["storeInfo"]["file"];
						if (!(fl.find("/") != std::string::npos)) {
							const std::string URL = this->storeJson["storeInfo"]["url"];

							if (URL != "") {
								std::string tmp = "";
								doSheet = DownloadEshop(URL, rev, tmp);
							}

						} else {
							Msg::waitMsg(Lang::get("FILE_SLASH"));
						}
					}
				}

				if (doSheet) {
					/* Tableau SpriteSheet. */
					if (this->storeJson["storeInfo"].contains("sheetURL") && this->storeJson["storeInfo"]["sheetURL"].is_array()) {
						if (this->storeJson["storeInfo"].contains("sheet") && this->storeJson["storeInfo"]["sheet"].is_array()) {
							const std::vector<std::string> locs = this->storeJson["storeInfo"]["sheetURL"].get<std::vector<std::string>>();
							const std::vector<std::string> sht = this->storeJson["storeInfo"]["sheet"].get<std::vector<std::string>>();

							if (locs.size() == sht.size()) {
								for (int i = 0; i < (int)sht.size(); i++) {
									if (!(sht[i].find("/") != std::string::npos)) {
										char msg[150];
										snprintf(msg, sizeof(msg), Lang::get("UPDATING_SPRITE_SHEET2").c_str(), i + 1, sht.size());
										Msg::DisplayMsg(msg);
										DownloadSpriteSheet(locs[i], sht[i]);

									} else {
										Msg::waitMsg(Lang::get("SHEET_SLASH"));
										i++;
									}
								}
							}
						}

						/* Feuille SpriteSheet unique (pas de tableau). */
					} else if (this->storeJson["storeInfo"].contains("sheetURL") && this->storeJson["storeInfo"]["sheetURL"].is_string()) {
						if (this->storeJson["storeInfo"].contains("sheet") && this->storeJson["storeInfo"]["sheet"].is_string()) {
							const std::string fl = this->storeJson["storeInfo"]["sheetURL"];
							const std::string fl2 = this->storeJson["storeInfo"]["sheet"];

							if (!(fl2.find("/") != std::string::npos)) {
								Msg::DisplayMsg(Lang::get("UPDATING_SPRITE_SHEET"));
								DownloadSpriteSheet(fl, fl2);

							} else {
								Msg::waitMsg(Lang::get("SHEET_SLASH"));
							}
						}
					}
				}
			}

			this->LoadFromFile(file);
			this->loadSheets();
		}
	}
}

/*
	Déchargez toutes les SpriteSheets sur Destructor.
*/
Store::~Store() { this->unloadSheets(); };

/*
	Déchargez toutes les feuilles SpriteSheets.
*/
void Store::unloadSheets() {
	if (this->valid) {
		if (this->sheets.size() > 0) {
			for (int i = 0; i < (int)this->sheets.size(); i++) {
				if (this->sheets[i]) C2D_SpriteSheetFree(this->sheets[i]);
			}
		}

		this->sheets.clear();
	}
}

/*
	Charger toutes les feuilles SpriteSheets.
*/
void Store::loadSheets() {
	if (this->valid) {
		if (this->storeJson["storeInfo"].contains("sheet")) {
			this->unloadSheets();

			std::vector<std::string> sheetLocs = { "" };

			if (this->storeJson["storeInfo"]["sheet"].is_array()) {
				sheetLocs = this->storeJson["storeInfo"]["sheet"].get<std::vector<std::string>>();

			} else if (this->storeJson["storeInfo"]["sheet"].is_string()) {
				sheetLocs[0] = this->storeJson["storeInfo"]["sheet"];

			} else {
				return;
			}

			for (int i = 0; i < (int)sheetLocs.size(); i++) {
				this->sheets.push_back({ });

				if (sheetLocs[i] != "") {
					if (sheetLocs[i].find("/") == std::string::npos) {
						if (access((std::string(_STORE_PATH) + sheetLocs[i]).c_str(), F_OK) == 0) {

							char msg[150];
							snprintf(msg, sizeof(msg), Lang::get("LOADING_SPRITESHEET").c_str(), i + 1, sheetLocs.size());
							Msg::DisplayMsg(msg);

							this->sheets[i] = C2D_SpriteSheetLoad((std::string(_STORE_PATH) + sheetLocs[i]).c_str());
						}
					}
				}
			}
		}
	}
}


/*
	Charger un eShop à partir d’un fichier.
	const std::string &file : Le fichier de eShop.
*/
void Store::LoadFromFile(const std::string &file) {
	FILE *in = fopen(file.c_str(), "rt");
	if (!in) {
		this->valid = false;
		return;
	}

	this->storeJson = nlohmann::json::parse(in, nullptr, false);
	fclose(in);

	/* Check, if valid. */
	if (this->storeJson.contains("storeInfo") && this->storeJson.contains("storeContent")) {
		if (this->storeJson["storeInfo"].contains("version") && this->storeJson["storeInfo"]["version"].is_number()) {
			if (this->storeJson["storeInfo"]["version"] < 3) Msg::waitMsg(Lang::get("ESHOP_TOO_OLD"));
			else if (this->storeJson["storeInfo"]["version"] > _ESHOP_VERSION) Msg::waitMsg(Lang::get("ESHOP_TOO_NEW"));
			else if (this->storeJson["storeInfo"]["version"] == 3 || this->storeJson["storeInfo"]["version"] == _ESHOP_VERSION) {
				this->valid = this->storeJson["storeInfo"]["version"] = true;
			}
		}

	} else {
		Msg::waitMsg(Lang::get("ESHOP_INVALID_ERROR"));
	}
}

/*
	Retourne le titre de l’eShop.
*/
std::string Store::GetEshopTitle() const {
	if (this->valid) {
		if (this->storeJson["storeInfo"].contains("title")) return this->storeJson["storeInfo"]["title"];
	}

	return "";
}

/*
	Renvoie le titre d’un index.
	int index : L’index.
*/
std::string Store::GetTitleEntry(int index) const {
	if (!this->valid) return "";
	if (index > (int)this->storeJson["storeContent"].size() - 1) return ""; // Empty.

	if (this->storeJson["storeContent"][index]["info"].contains("title") && this->storeJson["storeContent"][index]["info"]["title"].is_string()) {
		return this->storeJson["storeContent"][index]["info"]["title"];
	}

	return "";
}

/*
	Return the Author name of an index.

	int index: The index.
*/
std::string Store::GetAuthorEntry(int index) const {
	if (!this->valid) return "";
	if (index > (int)this->storeJson["storeContent"].size() - 1) return ""; // Vide.

	if (this->storeJson["storeContent"][index]["info"].contains("author") && this->storeJson["storeContent"][index]["info"]["author"].is_string()) {
		return this->storeJson["storeContent"][index]["info"]["author"];
	}

	return "";
}

/*
	Renvoie la description d’un index.
	int index : L’index.
*/
std::string Store::GetDescriptionEntry(int index) const {
	if (!this->valid) return "";
	if (index > (int)this->storeJson["storeContent"].size() - 1) return ""; // Vide.

	if (this->storeJson["storeContent"][index]["info"].contains("description") && this->storeJson["storeContent"][index]["info"]["description"].is_string()) {
		return this->storeJson["storeContent"][index]["info"]["description"];
	}

	return "";
}

/*
	Renvoie la catégorie d’un index.
	int index : L’index.
*/
std::vector<std::string> Store::GetCategoryIndex(int index) const {
	if (!this->valid) return { "" };
	if (index > (int)this->storeJson["storeContent"].size() - 1) return { "" }; // Vide.

	if (this->storeJson["storeContent"][index]["info"].contains("category")) {
		if (this->storeJson["storeContent"][index]["info"]["category"].is_array()) {
			return this->storeJson["storeContent"][index]["info"]["category"].get<std::vector<std::string>>();

		} else if (this->storeJson["storeContent"][index]["info"]["category"].is_string()) {
			std::vector<std::string> temp;
			temp.push_back( this->storeJson["storeContent"][index]["info"]["category"] );
			return temp;
		}
	}

	return { "" };
}

/*
	Retourne la version d’un index.
	int index : L’index.
*/
std::string Store::GetVersionEntry(int index) const {
	if (!this->valid) return "";
	if (index > (int)this->storeJson["storeContent"].size() - 1) return ""; // Vide.

	if (this->storeJson["storeContent"][index]["info"].contains("version") && this->storeJson["storeContent"][index]["info"]["version"].is_string()) {
		return this->storeJson["storeContent"][index]["info"]["version"];
	}

	return "";
}

/*
	Retourne la console d’un index.
	int index : L’index.
*/
std::vector<std::string> Store::GetConsoleEntry(int index) const {
	if (!this->valid) return { "" };
	if (index > (int)this->storeJson["storeContent"].size() - 1) return { "" }; // Vide.

	if (this->storeJson["storeContent"][index]["info"].contains("console")) {
		if (this->storeJson["storeContent"][index]["info"]["console"].is_array()) {
			return this->storeJson["storeContent"][index]["info"]["console"].get<std::vector<std::string>>();

		} else if (this->storeJson["storeContent"][index]["info"]["console"].is_string()) {
			std::vector<std::string> temp;
			temp.push_back( this->storeJson["storeContent"][index]["info"]["console"] );
			return temp;
		}
	}

	return { "" };
}

/*
	Renvoie la date de la dernière mise à jour d’un index.
	int index : L’index.
*/
std::string Store::GetLastUpdatedEntry(int index) const {
	if (!this->valid) return "";
	if (index > (int)this->storeJson["storeContent"].size() - 1) return ""; // Vide.

	if (this->storeJson["storeContent"][index]["info"].contains("last_updated") && this->storeJson["storeContent"][index]["info"]["last_updated"].is_string()) {
		return this->storeJson["storeContent"][index]["info"]["last_updated"];
	}

	return "";
}

/*
	Renvoie la taille d’un index.
	int index : L’index.
*/
std::string Store::GetSizeEntry(int index) const {
	if (!this->valid) return Lang::get("NO_SIZE");
	if (index > (int)this->storeJson["storeContent"].size() - 1) return Lang::get("NO_SIZE"); // Vide.

	if (this->storeJson["storeContent"][index]["info"].contains("size") && this->storeJson["storeContent"][index]["info"]["size"].is_string()) {
		if (this->storeJson["storeContent"][index]["info"]["size"] == "") return Lang::get("NO_SIZE");

		return this->storeJson["storeContent"][index]["info"]["size"];
	}

	return Lang::get("NO_SIZE");
}

/*
	Renvoie la contenu additionel d’un index.
	int index : L’index.
*/
std::string Store::GetAdditionalcontentEntry(int index) const {
	if (!this->valid) return Lang::get("NO_ADDITIONAL_CONTENT_AVAILABLE");
	if (index > (int)this->storeJson["storeContent"].size() - 1) return Lang::get("NO_ADDITIONAL_CONTENT_AVAILABLE"); // Vide.

	if (this->storeJson["storeContent"][index]["info"].contains("additional") && this->storeJson["storeContent"][index]["info"]["additional"].is_string()) {
		if (this->storeJson["storeContent"][index]["info"]["additional"] == "") return Lang::get("NO_ADDITIONAL_CONTENT_AVAILABLE");

		return this->storeJson["storeContent"][index]["info"]["additional"];
	}

	return Lang::get("NO_ADDITIONAL_CONTENT_AVAILABLE");
}

/*
	Retourne une image C2D_Image d’un index.
	int index : L’index.
*/
C2D_Image Store::GetIconEntry(int index) const {
	if (!this->valid) return C2D_SpriteSheetGetImage(sprites, sprites_noIcon_idx);
	if (this->sheets.empty()) return C2D_SpriteSheetGetImage(sprites, sprites_noIcon_idx);
	int iconIndex = -1, sheetIndex = 0;

	if (index > (int)this->storeJson["storeContent"].size() - 1) return C2D_SpriteSheetGetImage(sprites, sprites_noIcon_idx);

	if (this->storeJson["storeContent"][index]["info"].contains("icon_index") && this->storeJson["storeContent"][index]["info"]["icon_index"].is_number()) {
		iconIndex = this->storeJson["storeContent"][index]["info"]["icon_index"];
	}

	if (this->storeJson["storeContent"][index]["info"].contains("sheet_index") && this->storeJson["storeContent"][index]["info"]["sheet_index"].is_number()) {
		sheetIndex = this->storeJson["storeContent"][index]["info"]["sheet_index"];
	}

	if (iconIndex == -1) return C2D_SpriteSheetGetImage(sprites, sprites_noIcon_idx);

	if (sheetIndex > (int)this->sheets.size()) return C2D_SpriteSheetGetImage(sprites, sprites_noIcon_idx);
	if (!this->sheets[sheetIndex]) return C2D_SpriteSheetGetImage(sprites, sprites_noIcon_idx);

	if (iconIndex > (int)C2D_SpriteSheetCount(this->sheets[sheetIndex])-1) return C2D_SpriteSheetGetImage(sprites, sprites_noIcon_idx);

	C2D_Image temp = C2D_SpriteSheetGetImage(this->sheets[sheetIndex], iconIndex);
	if (temp.subtex->width < 49 && temp.subtex->height < 49) return temp; // jusqu’à 48x48 est valide.

	return C2D_SpriteSheetGetImage(sprites, sprites_noIcon_idx);
}

/*
	Définit le BG personnalisé sur la variable storeBG.
*/
void Store::SetC2DBGImage() {
	if (!this->valid) return;
	if (this->sheets.empty()) return;
	int index = -1, sheetIndex = -1;

	if (this->storeJson["storeInfo"].contains("bg_index") && this->storeJson["storeInfo"]["bg_index"].is_number()) {
		index = this->storeJson["storeInfo"]["bg_index"];
	}

	if (this->storeJson["storeInfo"].contains("bg_sheet") && this->storeJson["storeInfo"]["bg_sheet"].is_number()) {
		sheetIndex = this->storeJson["storeInfo"]["bg_sheet"];
	}

	if (index == -1 || sheetIndex == -1) return;

	if (sheetIndex > (int)this->sheets.size()) return;
	if (!this->sheets[sheetIndex]) return;

	if (index > (int)C2D_SpriteSheetCount(this->sheets[sheetIndex])-1) return;

	C2D_Image temp = C2D_SpriteSheetGetImage(this->sheets[sheetIndex], index);

	if (temp.subtex->width == 400 && temp.subtex->height == 214) {
		this->hasCustomBG = true;
		this->storeBG = temp; // Doit être 400x214.
	}
}

/*
	Retourner la liste de téléchargement d’une entrée.
	int index : L’index.
*/
std::vector<std::string> Store::GetDownloadList(int index) const {
	if (!this->valid) return { "" };
	std::vector<std::string> temp;

	if (index > (int)this->storeJson["storeContent"].size() - 1) return temp;

	for(auto it = this->storeJson.at("storeContent").at(index).begin(); it != this->storeJson.at("storeContent").at(index).end(); it++) {
		if (it.key() != "info") temp.push_back(it.key());
	}

	return temp;
}

/*
	Get filesizes for each download entry.

	int index: The index.
	const std::string &entry: The entry name.
*/
std::string Store::GetFileSizes(int index, const std::string &entry) const {
	if (!this->valid) return "";

	if (index > (int)this->storeJson["storeContent"].size() - 1) return "";

	if (this->storeJson["storeContent"][index].contains(entry) && this->storeJson["storeContent"][index][entry].type() == nlohmann::json::value_t::object) {
		if (this->storeJson["storeContent"][index][entry].contains("size") && this->storeJson["storeContent"][index][entry]["size"].is_string()) {
			return this->storeJson["storeContent"][index][entry]["size"];
		}
	}

	return "";
}

/*
	Get Screenshot URL list.

	int index: The Entry Index.
*/
std::vector<std::string> Store::GetScreenshotList(int index) const {
	if (!this->valid) return { };

	if (index > (int)this->storeJson["storeContent"].size() - 1) return { };

	std::vector<std::string> screenshots;

	if (this->storeJson["storeContent"][index]["info"].contains("screenshots")) {
		if (this->storeJson["storeContent"][index]["info"]["screenshots"].is_array()) {
			for(auto &item : this->storeJson["storeContent"][index]["info"]["screenshots"]) {
				if (item.is_object() && item.contains("url")) screenshots.push_back(item["url"]);
				else screenshots.push_back("");
			}
		}
	}

	return screenshots;
}

/*
	Get Screenshot names.

	int index: The Entry Index.
*/
std::vector<std::string> Store::GetScreenshotNames(int index) const {
	if (!this->valid) return { };

	if (index > (int)this->storeJson["storeContent"].size() - 1) return { };

	std::vector<std::string> screenshotNames;

	if (this->storeJson["storeContent"][index]["info"].contains("screenshots")) {
		if (this->storeJson["storeContent"][index]["info"]["screenshots"].is_array()) {
			for(auto &item : this->storeJson["storeContent"][index]["info"]["screenshots"]) {
				if (item.is_object() && item.contains("description")) screenshotNames.push_back(item["description"]);
				else screenshotNames.push_back("");
			}
		}
	}

	return screenshotNames;
}

/*
	Get the update notes of an entry.

	int index: The Entry Index.
*/
std::string Store::GetReleaseNotes(int index) const {
	if (!this->valid) return "";
	if (index > (int)this->storeJson["storeContent"].size() - 1) return ""; // Empty.

	if (this->storeJson["storeContent"][index]["info"].contains("releasenotes") && this->storeJson["storeContent"][index]["info"]["releasenotes"].is_string()) {
		return this->storeJson["storeContent"][index]["info"]["releasenotes"];
	}

	return "";
}