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

#ifndef _GHOST_ESHOP_META_HPP
#define _GHOST_ESHOP_META_HPP

#include "json.hpp"
#include <string>
#include <vector>

enum favoriteMarks {
	STAR = 1 << 0,
	HEART = 1 << 1,
	DIAMOND = 1 << 2,
	CLUBS = 1 << 3,
	SPADE = 1 << 4
};

class Meta {
public:
	Meta();
	~Meta() { this->SaveCall(); };

	std::string GetUpdated(const std::string &eshopName, const std::string &entry) const;
	int GetMarks(const std::string &eshopName, const std::string &entry) const;
	bool UpdateAvailable(const std::string &eshopName, const std::string &entry, const std::string &updated) const;
	std::vector<std::string> GetInstalled(const std::string &eshopName, const std::string &entry) const;

	void SetUpdated(const std::string &eshopName, const std::string &entry, const std::string &updated) {
		this->metadataJson[eshopName][entry]["updated"] = updated;
	};

	void SetMarks(const std::string &eshopName, const std::string &entry, int marks) {
		this->metadataJson[eshopName][entry]["marks"] = marks;
	};

	/* TODO: Handle this better. */
	void SetInstalled(const std::string &eshopName, const std::string &entry, const std::string &name) {
		const std::vector<std::string> installs = this->GetInstalled(eshopName, entry);
		bool write = true;

		if (!installs.empty()) {
			write = !installs.empty();

			for (int i = 0; i < (int)installs.size(); i++) {
				if (installs[i] == name) {
					write = false;
					break;
				}
			}
		}

		if (write) this->metadataJson[eshopName][entry]["installed"] += name;
	}

	/* Remove installed state from a download list entry. */
	void RemoveInstalled(const std::string &eshopName, const std::string &entry, const std::string &name) {
		const std::vector<std::string> installs = this->GetInstalled(eshopName, entry);
		if (installs.empty()) return;

		for (int i = 0; i < (int)installs.size(); i++) {
			if (installs[i] == name) {
				this->metadataJson[eshopName][entry]["installed"].erase(i);
				break;
			}
		}

		if (this->metadataJson[eshopName][entry]["installed"].empty() && this->metadataJson[eshopName][entry].contains("updated")) this->metadataJson[eshopName][entry].erase("updated");
	}

	void ImportMetadata();
	void SaveCall();
private:
	nlohmann::json metadataJson = nullptr;
};

#endif