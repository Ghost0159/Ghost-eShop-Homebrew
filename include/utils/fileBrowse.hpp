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

#ifndef _GHOST_ESHOP_FILE_BROWSE_HPP
#define _GHOST_ESHOP_FILE_BROWSE_HPP

#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <vector>

struct DirEntry {
	std::string name;
	std::string path;
	bool isDirectory;
};

/*
	eShop Info struct.
*/
struct EshopInfo {
	std::string Title;
	std::string Author;
	std::string URL;
	std::string File; // Used to check, if File does NOT contain a slash or so.
	std::string FileName;
	std::string Description;
	int Version;
	int Revision;
	int StoreSize;
};

bool nameEndsWith(const std::string &name, const std::vector<std::string> &extensionList);
void getDirectoryContents(std::vector<DirEntry> &dirContents, const std::vector<std::string> &extensionList);
void getDirectoryContents(std::vector<DirEntry> &dirContents);

std::vector<EshopInfo> GetEshopInfo(const std::string &path);

void dirCopy(DirEntry *entry, const char *destinationPath, const char *sourcePath);
int fcopy(const char *sourcePath, const char *destinationPath);

#endif