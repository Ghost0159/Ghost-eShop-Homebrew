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

#include "common.hpp"
#include "download.hpp"
#include "init.hpp"
#include "mainScreen.hpp"
#include "queueSystem.hpp"
#include "sound.hpp"

#include <dirent.h>
#include <unistd.h>

bool exiting = false, is3DSX = false, needUnloadFont = false;
C2D_SpriteSheet sprites;
int fadeAlpha = 0;
u32 old_time_limit;
std::unique_ptr<Theme> UIThemes = nullptr;
std::unique_ptr<Sound> Music = nullptr;
bool dspfirmFound = false;
std::vector<std::pair<std::string, std::string>> Themes = { };

/*
	Set, if 3DSX or CIA.
*/
static void getCurrentUsage(){
	u64 id;
	APT_GetProgramID(&id);
	is3DSX = (id != 0x0004000007392600);
}

/*
	Init Music.
*/
static void InitMusic() {
	if (access("sdmc:/3ds/dspfirm.cdc", F_OK) == 0) { // Ensure dspfirm dump exist.
		if (access("sdmc:/3ds/Universal-Updater/GhosteShop/music.wav", F_OK) == 0) { // Ensure music.wav exist.
			dspfirmFound = true;
			ndspInit();
			Music = std::make_unique<Sound>("sdmc:/3ds/Universal-Updater/GhosteShop/music.wav");

			Music->play();
		}
	}
}

/*
	Exit Music.
*/
static void ExitMusic() {
	if (dspfirmFound) {
		Music->stop();
		Music = nullptr;
		ndspExit();
	}
}

/*
	If button Position pressed -> Do something.

	touchPosition touch: The TouchPosition variable.
	Structs::ButtonPos button: The Button Struct.
*/
bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h)) return true;
	return false;
}

/*
	Load the custom font and use it instead of SysFont, if found.
*/
void Init::LoadFont() {
	if (config->customfont()) {
		if (!needUnloadFont) {
			if (access("sdmc:/3ds/Universal-Updater/font.bcfnt", F_OK) == 0) {
				Gui::loadFont(font, "sdmc:/3ds/Universal-Updater/font.bcfnt");
				needUnloadFont = true;
			}
		}
	}
}

/*
	Unload the custom font and switch back to SysFont.
*/
void Init::UnloadFont() {
	if (needUnloadFont) {
		Gui::unloadFont(font);
		font = nullptr;
		needUnloadFont = false;
	}
}

/*
	Initialize Ghost eShop.
*/
Result Init::Initialize() {
	gfxInitDefault();
	romfsInit();

	cfguInit();
	ptmuInit();
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
	Themes = UIThemes->ThemeNames();

	CFG_Region region = CFG_REGION_USA;
	if(config->language() == "cn-SI") {
		region = CFG_REGION_CHN;
	} else if(config->language() == "cn-TR") {
		region = CFG_REGION_TWN;
	} else if(config->language() == "kr") {
		region = CFG_REGION_KOR;
	}
	Gui::init(region);

	APT_GetAppCpuTimeLimit(&old_time_limit);
	APT_SetAppCpuTimeLimit(30); // Needed for QR Scanner to work.
	getCurrentUsage();
	aptSetSleepAllowed(false);
	hidSetRepeatParameters(20, 8);

	Lang::load(config->language());

	Gui::loadSheet("romfs:/gfx/sprites.t3x", sprites);
	LoadFont();

	osSetSpeedupEnable(true); // Enable speed-up for New 3DS users.

	/* Check here for updates. */
	if (config->updatecheck()) UpdateAction();

	if (exiting) return -1; // In case the update was successful.

	Gui::setScreen(std::make_unique<MainScreen>(), false, false);
	InitMusic();

	return 0;
}

/*
	MainLoop of Ghost eShop.
*/
Result Init::MainLoop() {
	bool fullExit = false;

	if (Initialize() == -1) fullExit = true;

	/* Loop as long as the status is not fullExit. */
	while (aptMainLoop() && !fullExit) {
		hidScanInput();
		hHeld = hidKeysHeld();
		hDown = hidKeysDown();
		hRepeat = hidKeysDownRepeat();
		hidTouchRead(&touch);

		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		Gui::DrawScreen(false);
		C3D_FrameEnd(0);

		if (!exiting) Gui::ScreenLogic(hDown, hHeld, touch, true, false);
		else {
			if (hidKeysDown() & KEY_START) fullExit = true; // Make it optionally faster.

			if (fadeAlpha < 255) {
				fadeAlpha += 4;
				if (fadeAlpha >= 255) fullExit = true;
			}
		}
	}

	/* Exit all services and exit the app. */
	Exit();
	return 0;
}

/*
	Exit Ghost eShop.
*/
Result Init::Exit() {
	Gui::exit();
	Gui::unloadSheet(sprites);
	UnloadFont();
	ExitMusic();
	gfxExit();
	cfguExit();
	config->save();
	ptmuExit();
	acExit();
	amExit();

    if (old_time_limit != UINT32_MAX) APT_SetAppCpuTimeLimit(old_time_limit); // Restore old limit.
	aptSetSleepAllowed(true);

	romfsExit();
	return 0;
}