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

#include "theme.hpp"
#include <regex>
#include <unistd.h>

/**
 * @brief Creates a 8 byte RGBA color
 * @param r red component of the color
 * @param g green component of the color
 * @param b blue component of the color
 * @param a alpha component of the color
 */
#define RGBA8(r, g, b, a) ((((r) & 0xFF) << 0) | (((g) & 0xFF) << 8) | (((b) & 0xFF) << 16) | (((a) & 0xFF) << 24))


Theme::Theme(const std::string &ThemeJSON) {
	FILE *file = fopen(ThemeJSON.c_str(), "rt");
	if (file) {
		this->json = nlohmann::json::parse(file, nullptr, false);
		fclose(file);
	}

	if (!file || this->json.is_discarded()) this->json = this->InitWithDefaultColors();
	this->Loaded = true;
}

nlohmann::json Theme::InitWithDefaultColors(const std::string &ThemePath) {
	nlohmann::json JS = {
		{ "Default", {
			{ "BarColor", "#4D4D4D" },
			{ "BGColor", "#333333" },
			{ "BarOutline", "#5F5F5F" },
			{ "TextColor", "#FFFFFF" },
			{ "EntryBar", "#DF6500" },
			{ "EntryOutline", "#BF4500" },
			{ "BoxInside", "#6E6E6E" },
			{ "BoxSelected", "#FF5200" },
			{ "BoxUnselected", "#000000" },
			{ "ProgressbarOut", "#FF7800" },
			{ "ProgressbarIn", "#FFD200" },
			{ "SearchBar", "#FFA000" },
			{ "SearchBarOutline", "#191E35" },
			{ "SideBarSelected", "#FF829B" },
			{ "SideBarUnselected", "#4D4D4D" },
			{ "MarkSelected", "#D09500" },
			{ "MarkUnselected", "#9A5100" },
			{ "DownListPrev", "#9C6000" },
			{ "SideBarIconColor", "#FFFFFF" },
			{ "Description", "Ghost eShop's default Theme.\n\nBy: Ghost eShop Team" }
			}},
			{ "Black/White", {
				{ "BarColor", "#E5E5E5" },
                { "BGColor", "#E5E5E5" },
                { "BarOutline", "#333333" },
                { "TextColor", "#000000" },
                { "EntryBar", "#E5E5E5" },
                { "EntryOutline", "#333333" },
                { "BoxInside", "#E5E5E5" },
                { "BoxSelected", "#000000" },
                { "BoxUnselected", "#E5E5E5" },
                { "ProgressbarOut", "#333333" },
                { "ProgressbarIn", "#E5E5E5" },
                { "SearchBar", "#E5E5E5" },
                { "SearchBarOutline", "#333333" },
                { "SideBarSelected", "#333333" },
                { "SideBarUnselected", "#E5E5E5" },
                { "MarkSelected", "#333333" },
                { "MarkUnselected", "#E5E5E5" },
                { "DownListPrev", "#333333" },
                { "SideBarIconColor", "#000000" },
                { "Description", "Black & White Theme with a touch of gray.\n\nBy : Ghost eShop Team" }
				}},
				{ "Red/Black", {
					{ "BarColor", "#8B0000" },
                	{ "BGColor", "#000000" },
                	{ "BarOutline", "#8B0000" },
                	{ "TextColor", "#FFFFFF" },
                	{ "EntryBar", "#8B0000" },
                	{ "EntryOutline", "#FF4500" },
                	{ "BoxInside", "#8B0000" },
                	{ "BoxSelected", "#FF4500" },
                	{ "BoxUnselected", "#8B0000" },
                	{ "ProgressbarOut", "#FF4500" },
                	{ "ProgressbarIn", "#FFD700" },
                	{ "SearchBar", "#8B0000" },
                	{ "SearchBarOutline", "#FF4500" },
                	{ "SideBarSelected", "#FF4500" },
                	{ "SideBarUnselected", "#8B0000" },
                	{ "MarkSelected", "#8B0000" },
                	{ "MarkUnselected", "#FF4500" },
                	{ "DownListPrev", "#8B0000" },
                	{ "SideBarIconColor", "#FFFFFF" },
                	{ "Description", "Red & Black theme with improved contrast.\n\nBy: Ghost eShop Team" }
					}},
					{ "Purple", {
                		{ "BarColor", "#4B0082" },
                		{ "BGColor", "#6A5ACD" },
                		{ "BarOutline", "#8B008B" },
                		{ "TextColor", "#FFFFFF" },
                		{ "EntryBar", "#7B68EE" },
                		{ "EntryOutline", "#9400D3" },
                		{ "BoxInside", "#9370DB" },
                		{ "BoxSelected", "#FF00FF" },
                		{ "BoxUnselected", "#6A5ACD" },
                		{ "ProgressbarOut", "#8B008B" },
                		{ "ProgressbarIn", "#7B68EE" },
                		{ "SearchBar", "#7B68EE" },
                		{ "SearchBarOutline", "#8B008B" },
                		{ "SideBarSelected", "#FF00FF" },
                		{ "SideBarUnselected", "#6A5ACD" },
                		{ "MarkSelected", "#8B008B" },
                		{ "MarkUnselected", "#9370DB" },
                		{ "DownListPrev", "#6A5ACD" },
                		{ "SideBarIconColor", "#FFFFFF" },
                		{ "Description", "Purple Theme.\n\nBy: Ghost eShop Team" }
						}},
						{ "Blue/Yellow", {
							{ "BarColor", "#1E3C72" },
							{ "BGColor", "#F5F5DC" },
							{ "BarOutline", "#1E3C72" },
							{ "TextColor", "#000000" },
							{ "EntryBar", "#1E3C72" },
							{ "EntryOutline", "#F5F5DC" },
							{ "BoxInside", "#F5F5DC" },
							{ "BoxSelected", "#FDB813" },
							{ "BoxUnselected", "#1E3C72" },
							{ "ProgressbarOut", "#1E3C72" },
							{ "ProgressbarIn", "#FDB813" },
							{ "SearchBar", "#F5F5DC" },
							{ "SearchBarOutline", "#1E3C72" },
							{ "SideBarSelected", "#FDB813" },
							{ "SideBarUnselected", "#1E3C72" },
							{ "MarkSelected", "#FDB813" },
							{ "MarkUnselected", "#1E3C72" },
							{ "DownListPrev", "#F5F5DC" },
							{ "SideBarIconColor", "#FFFFFF" },
                			{ "Description", "Blue & Yellow Theme\n\nBy: Ghost eShop Team" }
							}},
							{ "Nintendo eShop DSi", {
                				{ "BarColor", "#00C9FF" },
                				{ "BGColor", "#4B4B4B" },
                				{ "BarOutline", "#FFFFFF" },
                				{ "TextColor", "#FFFFFF" },
                				{ "EntryBar", "#00C9FF" },
                				{ "EntryOutline", "#FFFFFF" },
                				{ "BoxInside", "#4B4B4B" },
                				{ "BoxSelected", "#00C9FF" },
                				{ "BoxUnselected", "#4B4B4B" },
                				{ "ProgressbarOut", "#FFFFFF" },
                				{ "ProgressbarIn", "#00C9FF" },
                				{ "SearchBar", "#00C9FF" },
                				{ "SearchBarOutline", "#FFFFFF" },
                				{ "SideBarSelected", "#00C9FF" },
                				{ "SideBarUnselected", "#4B4B4B" },
                				{ "MarkSelected", "#00C9FF" },
                				{ "MarkUnselected", "#4B4B4B" },
                				{ "DownListPrev", "#4B4B4B" },
                				{ "SideBarIconColor", "#FFFFFF" },
                				{ "Description", "Nintendo eShop's DSi Theme\n\nBy: Ghost eShop Team" }
								}},
								{ "Future Theme", {
									{ "BarColor", "#3D5AFE" },
									{ "BGColor", "#121212" },
									{ "BarOutline", "#6200EA" },
									{ "TextColor", "#FFFFFF" },
									{ "EntryBar", "#6200EA" },
									{ "EntryOutline", "#FFFFFF" },
									{ "BoxInside", "#121212" },
									{ "BoxSelected", "#3D5AFE" },
									{ "BoxUnselected", "#121212" },
									{ "ProgressbarOut", "#6200EA" },
									{ "ProgressbarIn", "#3D5AFE" },
									{ "SearchBar", "#6200EA" },
									{ "SearchBarOutline", "#FFFFFF" },
									{ "SideBarSelected", "#6200EA" },
									{ "SideBarUnselected", "#121212" },
									{ "MarkSelected", "#3D5AFE" },
									{ "MarkUnselected", "#121212" },
									{ "DownListPrev", "#121212" },
									{ "SideBarIconColor", "#FFFFFF" },
									{ "Description", "A Futuristic Theme\n\nBy : Ghost eShop Team" },
									}},
									{ "Nintendo eShop 3DS", {
										{ "BarColor", "#F5A623" },
										{ "BGColor", "#F5A623" },
										{ "BarOutline", "#000000" },
										{ "TextColor", "#000000" },
										{ "EntryBar", "#F5A623" },
										{ "EntryOutline", "#000000" },
										{ "BoxInside", "#F5A623" },
										{ "BoxSelected", "#FEDC00" },
										{ "BoxUnselected", "#9B9B9B" },
										{ "ProgressbarOut", "#000000" },
										{ "ProgressbarIn", "#F5A623" },
										{ "SearchBar", "#F5A623" },
										{ "SearchBarOutline", "#000000" },
										{ "SideBarSelected", "#F5A623" },
										{ "SideBarUnselected", "#9B9B9B" },
										{ "MarkSelected", "#F5A623" },
										{ "MarkUnselected", "#9B9B9B" },
										{ "DownListPrev", "#F5A623" },
										{ "SideBarIconColor", "#000000" },
										{ "Description", "Nintendo eShop's 3DS Theme\n\nBy: Ghost eShop Team" },
										}},
										{ "Deluxe", {
											{ "BarColor", "#1A1A1A" },
											{ "BGColor", "#0F0F0F" },
											{ "BarOutline", "#2A2A2A" },
											{ "TextColor", "#FFFFFF" },
											{ "EntryBar", "#9B59B6" },
											{ "EntryOutline", "#8E44AD" },
											{ "BoxInside", "#16A085" },
											{ "BoxSelected", "#2980B9" },
											{ "BoxUnselected", "#000000" },
											{ "ProgressbarOut", "#27AE60" },
											{ "ProgressbarIn", "#2ECC71" },
											{ "SearchBar", "#F1C40F" },
											{ "SearchBarOutline", "#34495E" },
											{ "SideBarSelected", "#E67E22" },
											{ "SideBarUnselected", "#1A1A1A" },
											{ "MarkSelected", "#E74C3C" },
											{ "MarkUnselected", "#C0392B" },
											{ "DownListPrev", "#9B59B6" },
											{ "SideBarIconColor", "#FFFFFF" },
											{ "Description", "A Deluxe Theme\n\nBy : Ghost eShop Team" },
											}},
											{ "Blue/Orange", {
												{ "BarColor", "#F9A922" },
												{ "BGColor", "#003768" },
												{ "BarOutline", "#F9A922" },
												{ "TextColor", "#FFFFFF" },
												{ "EntryBar", "#F9A922" },
												{ "EntryOutline", "#F9A922" },
												{ "BoxInside", "#003768" },
												{ "BoxSelected", "#003768" },
												{ "BoxUnselected", "#FAE042" },
												{ "ProgressbarOut", "#F9A922" },
												{ "ProgressbarIn", "#F9A922" },
												{ "SearchBar", "#F9A922" },
												{ "SearchBarOutline", "#FFFFFF" },
												{ "SideBarSelected", "#F9A922" },
												{ "SideBarUnselected", "#003768" },
												{ "MarkSelected", "#F9A922" },
												{ "MarkUnselected", "#F9A922" },
												{ "DownListPrev", "#F9A922" },
												{ "SideBarIconColor", "#FFFFFF" },
												{ "Description", "Blue & orange Theme.\n\nBy: Ghost eShop Team" },
												}}
	};

	FILE *out = fopen(ThemePath.c_str(), "w");
	const std::string dump = JS.dump(1, '\t');
	fwrite(dump.c_str(), 1, JS.dump(1, '\t').size(), out);
	fclose(out);

	return JS;
}


void Theme::LoadTheme(const std::string &ThemeName) {
	this->vBarColor	         = this->GetThemeColor(ThemeName, "BarColor", C2D_Color32(77, 77, 77, 255));
	this->vBGColor	         = this->GetThemeColor(ThemeName, "BGColor", C2D_Color32(51, 51, 51, 255));
	this->vBarOutline        = this->GetThemeColor(ThemeName, "BarOutline", C2D_Color32(95, 95, 95, 255));
	this->vTextColor         = this->GetThemeColor(ThemeName, "TextColor", C2D_Color32(255, 255, 255, 255));
	this->vEntryBar	         = this->GetThemeColor(ThemeName, "EntryBar", C2D_Color32(223, 101, 0, 255));
	this->vEntryOutline      = this->GetThemeColor(ThemeName, "EntryOutline", C2D_Color32(191, 69, 0, 255));
	this->vBoxInside	     = this->GetThemeColor(ThemeName, "BoxInside", C2D_Color32(110, 110, 110, 255));
	this->vBoxSelected	     = this->GetThemeColor(ThemeName, "BoxSelected", C2D_Color32(255, 82, 0, 255));
	this->vBoxUnselected     = this->GetThemeColor(ThemeName, "BoxUnselected", C2D_Color32(0, 0, 0, 255));
	this->vProgressbarOut    = this->GetThemeColor(ThemeName, "ProgressbarOut", C2D_Color32(255, 120, 0, 255));
	this->vProgressbarIn     = this->GetThemeColor(ThemeName, "ProgressbarIn", C2D_Color32(255, 210, 0, 255));
	this->vSearchBar	     = this->GetThemeColor(ThemeName, "SearchBar", C2D_Color32(255, 160, 0, 255));
	this->vSearchBarOutline  = this->GetThemeColor(ThemeName, "SearchBarOutline", C2D_Color32(25, 30, 53, 255));
	this->vSideBarSelected   = this->GetThemeColor(ThemeName, "SideBarSelected", C2D_Color32(255, 130, 155, 255));
	this->vSideBarUnselected = this->GetThemeColor(ThemeName, "SideBarUnselected", C2D_Color32(154, 81, 0, 255));
	this->vMarkSelected		 = this->GetThemeColor(ThemeName, "MarkSelected", C2D_Color32(208, 149, 0, 255));
	this->vMarkUnselected	 = this->GetThemeColor(ThemeName, "MarkUnselected", C2D_Color32(110, 110, 110, 255));
	this->vDownListPrev		 = this->GetThemeColor(ThemeName, "DownListPrev", C2D_Color32(110, 110, 110, 255));
	this->vSideBarIconColor  = this->GetThemeColor(ThemeName, "SideBarIconColor", C2D_Color32(255, 255 ,255, 255));
}

std::vector<std::pair<std::string, std::string>> Theme::ThemeNames() {
	std::vector<std::pair<std::string, std::string>> Temp = { };

	if (this->Loaded) {
		for(auto it = this->json.begin(); it != this->json.end(); ++it) {
			if (this->json[it.key()].contains("Description") && this->json[it.key()]["Description"].is_string()) {
				Temp.push_back(std::make_pair(it.key(), this->json[it.key()]["Description"]));

			} else {
				Temp.push_back(std::make_pair(it.key(), it.key()));
			}
		}
	}

	return Temp;
}


uint32_t Theme::GetThemeColor(const std::string &ThemeName, const std::string &Key, const uint32_t DefaultColor) {
	if(this->json.contains(ThemeName) && this->json[ThemeName].is_object() && this->json[ThemeName].contains(Key) && this->json[ThemeName][Key].is_string()) {
		const std::string &colorString = this->json[ThemeName][Key].get_ref<const std::string &>();
		if (colorString.length() < 7 || std::regex_search(colorString.substr(1), std::regex("[^0-9A-Fa-f]"))) { // invalid color.
			return DefaultColor;
		}

		int r = std::stoi(colorString.substr(1, 2), nullptr, 16);
		int g = std::stoi(colorString.substr(3, 2), nullptr, 16);
		int b = std::stoi(colorString.substr(5, 2), nullptr, 16);
		return RGBA8(r, g, b, 0xFF);

	}

	return DefaultColor;
}