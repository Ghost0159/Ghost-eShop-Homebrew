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
std::unique_ptr<Sound> Music = nullptr;
bool dspfirmFound = false;

/*
	Set, si 3DSX ou CIA.
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
		if (access("romfs:/song/music.wav", F_OK) == 0) { // Ensure music.wav exist.
			dspfirmFound = true;
			ndspInit();
			Music = std::make_unique<Sound>("romfs:/song/music.wav");

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
	Chargez la police personnalisée et utilisez-la à la place de SysFont, si elle est trouvée.
*/
void Init::LoadFont() {
	if (config->customfont()) {
		if (!needUnloadFont) {
			if (access("sdmc:/3ds/GhosteShop/font.bcfnt", F_OK) == 0) {
				Gui::loadFont(font, "sdmc:/3ds/GhosteShop/font.bcfnt");
				needUnloadFont = true;
			}
		}
	}
}

/*
	Déchargez la police personnalisée et revenez à SysFont.
*/
void Init::UnloadFont() {
	if (needUnloadFont) {
		Gui::unloadFont(font);
		font = nullptr;
		needUnloadFont = false;
	}
}

/*
	Initialiser Ghost eShop.
*/
Result Init::Initialize() {
	gfxInitDefault();
	romfsInit();
	Gui::init();

	cfguInit();
	amInit();
	acInit();

	APT_GetAppCpuTimeLimit(&old_time_limit);
	APT_SetAppCpuTimeLimit(30); // Nécessaire pour que le scanner QR fonctionne.
	getCurrentUsage();
	aptSetSleepAllowed(false);
	hidSetRepeatParameters(20, 8);

	/* Créer des répertoires, s’il manque. */
	mkdir("sdmc:/3ds", 0777);
	mkdir("sdmc:/3ds/GhosteShop", 0777);
	mkdir("sdmc:/3ds/GhosteShop/stores", 0777);
	mkdir("sdmc:/3ds/GhosteShop/shortcuts", 0777);

	config = std::make_unique<Config>();
	Lang::load(config->language());

	Gui::loadSheet("romfs:/gfx/sprites.t3x", sprites);
	LoadFont();

	osSetSpeedupEnable(true); // Activer l’accélération pour les utilisateurs de new3DS.

	/* Check here for updates. */
	if (config->updatecheck()) UpdateAction();

	if (exiting) return -1; // En cas de succès de la mise à jour.

	Gui::setScreen(std::make_unique<MainScreen>(), false, false);
	InitMusic();

	/* Initialize Queue System LightLock. */
	LightLock_Init(&QueueSystem::lock);

	return 0;
}

/*
	MainLoop de Ghost eShop.
*/
Result Init::MainLoop() {
	bool fullExit = false;

	if (Initialize() == -1) fullExit = true;

	/* Boucle tant que le statut n’est pas fullExit. */
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
		if (!exiting) Gui::ScreenLogic(hDown, hHeld, touch, true, false);
		C3D_FrameEnd(0);

		if (exiting) {
			if (hDown & KEY_START) fullExit = true; // En option, plus vite.

			if (fadeAlpha < 255) {
				fadeAlpha += 4;
				if (fadeAlpha >= 255) fullExit = true;
			}
		}
	}

	/* Quitter tous les services et quitter l’application. */
	Exit();
	return 0;
}

/*
	Fermer Ghost eShop.
*/
Result Init::Exit() {
	Gui::exit();
	Gui::unloadSheet(sprites);
	UnloadFont();
	ExitMusic();
	gfxExit();
	cfguExit();
	config->save();
	acExit();
	amExit();

    if (old_time_limit != UINT32_MAX) APT_SetAppCpuTimeLimit(old_time_limit); // Rétablir l’ancienne limite.
	aptSetSleepAllowed(true);

	romfsExit();
	return 0;
}