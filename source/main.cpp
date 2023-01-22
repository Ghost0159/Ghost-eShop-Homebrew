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

#include "argumentParser.hpp"
#include "common.hpp"
#include "init.hpp"
#include <dirent.h>
#include <string>

#define ARG_AMOUNT 4 // In case for more args, change this. It must be ARG amount + 1, because of 3DSX Path.
std::string _3dsxPath = "";

/* ARG Init. */
static void InitForARG() {
	gfxInitDefault();
	romfsInit();

	cfguInit();
	amInit();
	acInit();

	/* Create Directories, if missing. */
	mkdir("sdmc:/3ds", 0777);
	mkdir("sdmc:/3ds/Universal-Updater", 0777);
	mkdir("sdmc:/3ds/Universal-Updater/stores", 0777);
	mkdir("sdmc:/3ds/Universal-Updater/shortcuts", 0777);
	mkdir("sdmc:/3ds/Universal-Updater/GhosteShop", 0777);
	mkdir("sdmc:/3ds/Universal-Updater/GhosteShop/config", 0777);
	mkdir("sdmc:/3ds/Universal-Updater/GhosteShop/themes", 0777);
	mkdir("sdmc:/roms/", 0777);
	mkdir("sdmc:/roms/nds/", 0777);
	mkdir("sdmc:/_nds/", 0777);
	mkdir("sdmc:/_nds/TWiLightMenu/", 0777);
	mkdir("sdmc:/_nds/TWiLightMenu/boxart/", 0777);

	config = std::make_unique<Config>();
	UIThemes = std::make_unique<Theme>();
	UIThemes->LoadTheme(config->theme());

	CFG_Region region = CFG_REGION_USA;
	if(config->language() == "cn-SI") {
		region = CFG_REGION_CHN;
	} else if(config->language() == "cn-TR") {
		region = CFG_REGION_TWN;
	} else if(config->language() == "kr") {
		region = CFG_REGION_KOR;
	}
	Gui::init(region);

	Lang::load(config->language());
	Init::LoadFont();
	osSetSpeedupEnable(true); // Enable speed-up for New 3DS users.
}

/* ARG Exit. */
static Result ExitForARG() {
	Gui::exit();
	Init::UnloadFont();
	gfxExit();
	cfguExit();
	acExit();
	amExit();
	romfsExit();

	return 0;
}

int main(int argc, char *argv[]) {
	if (argc > 0) _3dsxPath = argv[0];

	/* 4 --> Argument mode. */
	if (argc == ARG_AMOUNT) {
		InitForARG();

		const std::string file = argv[1];
		const std::string entry = argv[2];
		int dlIndex = atoi(argv[3]);

		std::unique_ptr<ArgumentParser> arg = std::make_unique<ArgumentParser>(file, entry, dlIndex);

		if (arg->GetValid()) arg->Execute(); // Execute, if valid.
		else Msg::waitMsg(Lang::get("ARGUMENT_INVALID"));
		return ExitForARG();
	}

	return Init::MainLoop();
}