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

#include "fileBrowse.hpp"
#include "json.hpp"
#include "structs.hpp"
#include <3ds.h>
#include <cstring>
#include <functional>
#include <unistd.h>

bool nameEndsWith(const std::string &name, const std::vector<std::string> &extensionList) {
	if (name.substr(0, 2) == "._") return false;

	if (name.size() == 0) return false;

	if (extensionList.size() == 0) return true;

	for(int i = 0; i < (int)extensionList.size(); i++) {
		const std::string ext = extensionList.at(i);
		if (strcasecmp(name.c_str() + name.size() - ext.size(), ext.c_str()) == 0) return true;
	}

	return false;
}

bool dirEntryPredicate(const DirEntry &lhs, const DirEntry &rhs) {
	if (!lhs.isDirectory && rhs.isDirectory) return false;
	if (lhs.isDirectory && !rhs.isDirectory) return true;

	return strcasecmp(lhs.name.c_str(), rhs.name.c_str()) < 0;
}

void getDirectoryContents(std::vector<DirEntry> &dirContents, const std::vector<std::string> &extensionList) {
	struct stat st;

	dirContents.clear();

	DIR *pdir = opendir(".");

	if (pdir != nullptr) {
		while(true) {
			DirEntry dirEntry;

			struct dirent *pent = readdir(pdir);
			if (pent == NULL) break;

			stat(pent->d_name, &st);
			dirEntry.name = pent->d_name;
			dirEntry.isDirectory = (st.st_mode & S_IFDIR) ? true : false;

			if (dirEntry.name.compare(".") != 0 && (dirEntry.isDirectory || nameEndsWith(dirEntry.name, extensionList))) {
				dirContents.push_back(dirEntry);
			}
		}

		closedir(pdir);
	}

	sort(dirContents.begin(), dirContents.end(), dirEntryPredicate);
}

void getDirectoryContents(std::vector<DirEntry> &dirContents) {
	getDirectoryContents(dirContents, {});
}

std::vector<std::string> getContents(const std::string &name, const std::vector<std::string> &extensionList) {
	std::vector<std::string> dirContents;
	DIR *pdir = opendir(name.c_str());
	struct dirent *pent;

	while ((pent = readdir(pdir)) != NULL) {
		if (nameEndsWith(pent->d_name, extensionList)) dirContents.push_back(pent->d_name);
	}

	closedir(pdir);
	return dirContents;
}

/*
	Retourner les informations eShop.
	const std::string &file : Const Référence au chemin du fichier.
	const std::string &fieName : Const Référence au nom de fichier, sans chemin.
*/
EshopInfo GetInfo(const std::string &file, const std::string &fileName) {
	EshopInfo Temp = { "", "", "", "", fileName, "", -1, -1, -1 }; // Titre, Auteur, URL, Fichier (pour vérifier s’il n’existe pas de barre oblique), FileName, Desc, Version, Révision, Entrées.
	nlohmann::json JSON = nullptr;

	FILE *temp = fopen(file.c_str(), "r");
	JSON = nlohmann::json::parse(temp, nullptr, false);
	fclose(temp);

	if (!JSON.contains("storeInfo")) return Temp; // storeInfo n'existe pas.

	if (JSON["storeInfo"].contains("title") && JSON["storeInfo"]["title"].is_string()) {
		Temp.Title = JSON["storeInfo"]["title"];
	}

	if (JSON["storeInfo"].contains("file") && JSON["storeInfo"]["file"].is_string()) {
		Temp.File = JSON["storeInfo"]["file"];
	}

	if (JSON["storeInfo"].contains("author") && JSON["storeInfo"]["author"].is_string()) {
		Temp.Author = JSON["storeInfo"]["author"];
	}

	if (JSON["storeInfo"].contains("url") && JSON["storeInfo"]["url"].is_string()) {
		Temp.URL = JSON["storeInfo"]["url"];
	}

	if (JSON["storeInfo"].contains("description") && JSON["storeInfo"]["description"].is_string()) {
		Temp.Description = JSON["storeInfo"]["description"];
	}

	if (JSON["storeInfo"].contains("version") && JSON["storeInfo"]["version"].is_number()) {
		Temp.Version = JSON["storeInfo"]["version"];
	}

	if (JSON["storeInfo"].contains("revision") && JSON["storeInfo"]["revision"].is_number()) {
		Temp.Revision = JSON["storeInfo"]["revision"];
	}

	if (JSON.contains("storeContent")) Temp.StoreSize = JSON["storeContent"].size();

	return Temp;
}

/*
	Renvoie le vecteur d’informations eShop.
	const std::string &path : Const Référence au chemin, où vérifier.
*/
std::vector<EshopInfo> GetEshopInfo(const std::string &path) {
	std::vector<EshopInfo> info;
	std::vector<DirEntry> dirContents;

	if (access(path.c_str(), F_OK) != 0) return {}; // Le dossier n’existe pas.

	chdir(path.c_str());
	getDirectoryContents(dirContents, { "eshop" });

	for(uint i = 0; i < dirContents.size(); i++) {
		/* Assurez-vous de SEULEMENT pousser .eshop, et pas de dossiers. Évite les plantages dans ce cas aussi. */
		if ((path + dirContents[i].name).find(".eshop") != std::string::npos) {
			info.push_back( GetInfo(path + dirContents[i].name, dirContents[i].name) );
		}
	}

	return info;
}

#define copyBufSize 0x8000
u32 copyBuf[copyBufSize];

/*
	Copiez un répertoire.
	DirEntry *entry : pointeur vers un DirEntry.
	const char *destinationPath : pointeur vers le chemin de destination.
	const char *sourcePath : pointeur vers le chemin source.
*/
void dirCopy(DirEntry *entry, const char *destinationPath, const char *sourcePath) {
	std::vector<DirEntry> dirContents;
	dirContents.clear();
	if (entry->isDirectory)	chdir((sourcePath + ("/" + entry->name)).c_str());
	getDirectoryContents(dirContents);
	if (((int)dirContents.size()) == 1)	mkdir((destinationPath + ("/" + entry->name)).c_str(), 0777);
	if (((int)dirContents.size()) != 1)	fcopy((sourcePath + ("/" + entry->name)).c_str(), (destinationPath + ("/" + entry->name)).c_str());
}

u32 copyOffset = 0, copySize = 0;
/*
	L’opération de copie.
	const char *destinationPath : pointeur vers le chemin de destination.
	const char *sourcePath : pointeur vers le chemin source.
*/
int fcopy(const char *sourcePath, const char *destinationPath) {
	copyOffset = 0, copySize = 0;

	DIR *isDir = opendir(sourcePath);

	if (isDir != NULL) {
		closedir(isDir);

		/* Le chemin source est un répertoire. */
		chdir(sourcePath);
		std::vector<DirEntry> dirContents;
		getDirectoryContents(dirContents);
		DirEntry *entry = &dirContents.at(1);
		mkdir(destinationPath, 0777);

		for(int i = 1; i < ((int)dirContents.size()); i++) {
			chdir(sourcePath);
			entry = &dirContents.at(i);
			dirCopy(entry, destinationPath, sourcePath);
		}

		chdir(destinationPath);
		chdir("..");
		return 1;

	} else {
		closedir(isDir);

		/* Le chemin source est un fichier. */
		FILE *sourceFile = fopen(sourcePath, "rb");
		copySize = 0, copyOffset = 0;

		if (sourceFile) {
			fseek(sourceFile, 0, SEEK_END);
			copySize = ftell(sourceFile); // Obtenir la taille du fichier source.
			fseek(sourceFile, 0, SEEK_SET);

		} else {
			fclose(sourceFile);
			return -1;
		}

		FILE *destinationFile = fopen(destinationPath, "wb");
		//if (destinationFile) {
			fseek(destinationFile, 0, SEEK_SET);
		/*} else {
			fclose(sourceFile);
			fclose(destinationFile);
			return -1;
		}*/

		int numr;
		while(1) {
			scanKeys();
			if (keysHeld() & KEY_B) {
				/* Annuler la copie. */
				fclose(sourceFile);
				fclose(destinationFile);
				return -1;
				break;
			}

			printf("\x1b[16;0H");
			printf("Progress:\n");
			printf("%i/%i Bytes					   ", (int)copyOffset, (int)copySize);

			/* Copier le fichier vers le chemin de destination. */
			numr = fread(copyBuf, 2, copyBufSize, sourceFile);
			fwrite(copyBuf, 2, numr, destinationFile);
			copyOffset += copyBufSize;

			if (copyOffset > copySize) {
				fclose(sourceFile);
				fclose(destinationFile);

				printf("\x1b[17;0H");
				printf("%i/%i Bytes					   ", (int)copyOffset, (int)copySize);
				for(int i = 0; i < 30; i++) gspWaitForVBlank();

				return 1;
				break;
			}
		}

		return -1;
	}
}