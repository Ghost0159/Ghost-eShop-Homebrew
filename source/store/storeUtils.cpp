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

#include "storeUtils.hpp"

/*
	Comparez le titre.
	const std::unique_ptr<StoreEntry> &a : Const Référence à l’entrée A.
	const std::unique_ptr<StoreEntry> &b : Const Référence à l’entrée B.
*/
bool StoreUtils::compareTitleDescending(const std::unique_ptr<StoreEntry> &a, const std::unique_ptr<StoreEntry> &b) {
	if (a && b) return strcasecmp(StringUtils::lower_case(a->GetTitle()).c_str(), StringUtils::lower_case(b->GetTitle()).c_str()) > 0;

	return true;
}
bool StoreUtils::compareTitleAscending(const std::unique_ptr<StoreEntry> &a, const std::unique_ptr<StoreEntry> &b) {
	if (a && b) return strcasecmp(StringUtils::lower_case(b->GetTitle()).c_str(), StringUtils::lower_case(a->GetTitle()).c_str()) > 0;

	return true;
}

/*
	Comparer l’auteur.
	const std::unique_ptr<StoreEntry> &a : Const Référence à l’entrée A.
	const std::unique_ptr<StoreEntry> &b : Const Référence à l’entrée B.
*/
bool StoreUtils::compareAuthorDescending(const std::unique_ptr<StoreEntry> &a, const std::unique_ptr<StoreEntry> &b) {
	if (a && b) return strcasecmp(StringUtils::lower_case(a->GetAuthor()).c_str(), StringUtils::lower_case(b->GetAuthor()).c_str()) > 0;

	return true;
}
bool StoreUtils::compareAuthorAscending(const std::unique_ptr<StoreEntry> &a, const std::unique_ptr<StoreEntry> &b) {
	if (a && b) return strcasecmp(StringUtils::lower_case(b->GetAuthor()).c_str(), StringUtils::lower_case(a->GetAuthor()).c_str()) > 0;

	return true;
}

/*
	Comparer la dernière mise à jour.
	const std::unique_ptr<StoreEntry> &a : Const Référence à l’entrée A.
	const std::unique_ptr<StoreEntry> &b : Const Référence à l’entrée B.
*/
bool StoreUtils::compareUpdateDescending(const std::unique_ptr<StoreEntry> &a, const std::unique_ptr<StoreEntry> &b) {
	if (a && b) return strcasecmp(StringUtils::lower_case(a->GetLastUpdated()).c_str(), StringUtils::lower_case(b->GetLastUpdated()).c_str()) > 0;

	return true;
}
bool StoreUtils::compareUpdateAscending(const std::unique_ptr<StoreEntry> &a, const std::unique_ptr<StoreEntry> &b) {
	if (a && b) return strcasecmp(StringUtils::lower_case(b->GetLastUpdated()).c_str(), StringUtils::lower_case(a->GetLastUpdated()).c_str()) > 0;

	return true;
}

/*
	Triez les entrées.
	bool Ascending : Si Ascending.
	SortType sorttype : Le type de tri.
	std::vector<std::unique_ptr<StoreEntry>> &entries : référence aux entrées, qui doivent être triées.
*/
void StoreUtils::SortEntries(bool Ascending, SortType sorttype, std::vector<std::unique_ptr<StoreEntry>> &entries) {
	switch(sorttype) {
		case SortType::TITLE:
			Ascending ? std::sort(entries.begin(), entries.end(), StoreUtils::compareTitleAscending) : std::sort(entries.begin(), entries.end(), StoreUtils::compareTitleDescending);
			break;

		case SortType::AUTHOR:
			Ascending ? std::sort(entries.begin(), entries.end(), StoreUtils::compareAuthorAscending) : std::sort(entries.begin(), entries.end(), StoreUtils::compareAuthorDescending);
			break;

		case SortType::LAST_UPDATED:
			Ascending ? std::sort(entries.begin(), entries.end(), StoreUtils::compareUpdateAscending) : std::sort(entries.begin(), entries.end(), StoreUtils::compareUpdateDescending);
			break;
	}
}

/*
	Trouvez une requête à partir d’un vecteur.
	const std::vector<std::string> &items : Const Référence aux chaînes/éléments vectoriels.
	const std::string &query : Const Référence à la requête.
*/
static bool findInVector(const std::vector<std::string> &items, const std::string &query) {
	for(const std::string &item : items) {
		if (StringUtils::lower_case(item).find(query) != std::string::npos) return true;
	}

	return false;
}

/*
	Cherche des trucs du magasin.
	std::vectorstd::unique_ptrStoreEntry>> &entries : Référence aux entrées.
	const std::string &query : Const Référence à la requête.
	bool title : si les titres doivent être inclus.
	bool author : si les auteurs doivent être inclus.
	catégorie bool : si les catégories doivent être incluses.
	console bool : si les consoles doivent être incluses.
	int selectedMarks : Les options de marque sélectionnées.
	bool updateAvl : si les mises à jour disponibles doivent être un drapeau inclus
*/
void StoreUtils::search(std::vector<std::unique_ptr<StoreEntry>> &entries, const std::string &query, bool title, bool author, bool category, bool console, int selectedMarks, bool updateAvl) {
	for (auto it = entries.begin(); it != entries.end(); ++it) {
		if (!(((title && StringUtils::lower_case((*it)->GetTitle()).find(StringUtils::lower_case(query)) != std::string::npos)
		|| (author && StringUtils::lower_case((*it)->GetAuthor()).find(StringUtils::lower_case(query)) != std::string::npos)
		|| (category && findInVector((*it)->GetCategoryFull(), StringUtils::lower_case(query)))
		|| (console && findInVector((*it)->GetConsoleFull(), StringUtils::lower_case(query)))
		|| (!title && !author && !category && !console))
		&& ((selectedMarks == 0 && !updateAvl) || (*it)->GetMarks() & selectedMarks || (updateAvl && (*it)->GetUpdateAvl())))) {
			entries.erase(it);
			--it;
		}
	}
}

/*
	Filtrer les mises à jour disponibles.
	std::vectorstd::unique_ptrStoreEntry>> &entries : Référence aux entrées.
*/
void StoreUtils::FilterUpdateAvailable(std::vector<std::unique_ptr<StoreEntry>> &entries) {
	for (auto it = entries.begin(); it != entries.end(); ++it) {
		if (!((*it)->GetUpdateAvl())) {
			entries.erase(it);
			--it;
		}
	}
}

/*
	Réinitialiser tout le magasin et effacer + récupérer les entrées à nouveau.
	const std::unique_ptrStore> &store : Const Référence à la classe Store.
	const std::unique_ptrMeta> &meta : Const Référence à la classe Meta.
	std::vectorstd::unique_ptrStoreEntry>> &entries : Référence aux entrées.
*/
void StoreUtils::ResetAll(const std::unique_ptr<Store> &store, const std::unique_ptr<Meta> &meta, std::vector<std::unique_ptr<StoreEntry>> &entries) {
	if (store) {
		entries.clear();

		if (store->GetValid()) {
			for (int i = 0; i < store->GetStoreSize(); i++) {
				entries.push_back( std::make_unique<StoreEntry>(store, meta, i) );
			}

			store->SetBox(0);
			store->SetEntry(0);
			store->SetScreenIndx(0);
		}
	}
}