/*
*   This file is part of Universal-Core
*   Copyright (C) 2020 Universal-Team
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

#include "gui.hpp"
#include "screenCommon.hpp"

#include <3ds.h>
#include <stack>
#include <unistd.h>
#include <vector>

C3D_RenderTarget *Top, *TopRight, *Bottom;

C2D_TextBuf TextBuf;
C2D_Font Font;
std::unique_ptr<Screen> usedScreen, tempScreen; // tempScreen used for "fade" effects.
std::stack<std::unique_ptr<Screen>> screens;
bool currentScreen = false;
bool fadeout = false, fadein = false, fadeout2 = false, fadein2 = false;
int fadealpha = 0;
int fadecolor = 0;

/*
	Clear the Text Buffer.
*/
void Gui::clearTextBufs(void) { C2D_TextBufClear(TextBuf); };

/*
	Draw a sprite from the sheet.

	C2D_SpriteSheet sheet: The SpriteSheet.
	size_t imgindex: The image index.
	int x: The X-Position where to draw the sprite.
	int y: The Y-Position where to draw the sprite.
	float ScaleX: The X-Scale of the sprite.
	float ScaleY: The Y-Scale of the sprite.

	If the spritesheet is nullptr or image index goes out of scope, this doesn't do anything.
*/
void Gui::DrawSprite(C2D_SpriteSheet sheet, size_t imgindex, int x, int y, float ScaleX, float ScaleY) {
	if (sheet) {
		if (C2D_SpriteSheetCount(sheet) >= imgindex) {
			C2D_DrawImageAt(C2D_SpriteSheetGetImage(sheet, imgindex), x, y, 0.5f, nullptr, ScaleX, ScaleY);
		}
	}
}

/*
	Initialize the GUI.

	Contains initializing Citro2D, Citro3D and the screen targets.
	Call this when initing the app.
*/
Result Gui::init(void) {
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	/* Create Screen Targets. */
	Top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	TopRight = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);
	Bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	/* Load Textbuffer. */
	TextBuf = C2D_TextBufNew(4096);
	Font = C2D_FontLoadSystem(CFG_REGION_USA); // Load System font.
	return 0;
}

/*
	Load a bcfnt font.

	C2D_Font &fnt: The reference to the C2D_Font variable.
	const char *Path: The path to the file.
*/
Result Gui::loadFont(C2D_Font &fnt, const char *Path) {
	if (access(Path, F_OK) == 0) fnt = C2D_FontLoad(Path); // Only load if found.

	return 0;
}

/*
	Unload a Font.

	C2D_Font &fnt: The reference to the C2D_Font variable.
*/
Result Gui::unloadFont(C2D_Font &fnt) {
	if (fnt) C2D_FontFree(fnt); // Make sure to only unload if not nullptr.

	return 0;
}

/*
	Load a t3x SpriteSheet.

	const char *Path: The path to the file.
	C2D_SpriteSheet &sheet: The reference to the C2D_SpriteSheet variable.
*/
Result Gui::loadSheet(const char *Path, C2D_SpriteSheet &sheet) {
	if (access(Path, F_OK) == 0) sheet = C2D_SpriteSheetLoad(Path); // Only load if found.

	return 0;
}

/*
	Unload a SpriteSheet.

	C2D_SpriteSheet &sheet: The reference to the C2D_SpriteSheet variable.
*/
Result Gui::unloadSheet(C2D_SpriteSheet &sheet) {
	if (sheet) C2D_SpriteSheetFree(sheet); // Make sure to only unload if not nullptr.

	return 0;
}

/*
	Exit the GUI.

	Contains deinitializing Citro2D, Citro3D and clearing the textbuffer.
	Call this when exiting the app.
*/
void Gui::exit(void) {
	C2D_TextBufDelete(TextBuf);
	C2D_Fini();
	C3D_Fini();
	if (usedScreen) usedScreen = nullptr;
}

/*
	Reinitialize the GUI.
*/
Result Gui::reinit(void) {
	C2D_TextBufDelete(TextBuf);
	C2D_Fini();
	C3D_Fini();

	return Gui::init();
}

/*
	Draw a Centered String.

	float x: The X-Addition offset for the position from 200 (top) or 160 (bottom).
	float y: The Y-Position where to draw.
	float size: The size for the Font.
	u32 color: The Text Color.
	std::string Text: The Text which should be drawn.
	int maxWidth: (Optional) The max width of the Text.
	int maxHeight: (Optional) The max height of the Text.
	C2D_Font fnt: (Optional) The wanted C2D_Font. Is nullptr by default.
	int flags: (Optional) C2D text flags to use.
*/
void Gui::DrawStringCentered(float x, float y, float size, u32 color, std::string Text, int maxWidth, int maxHeight, C2D_Font fnt, int flags) {
	Gui::DrawString(x +(currentScreen ? 200 : 160), y, size, color, Text, maxWidth, maxHeight, fnt, flags | C2D_AlignCenter);
}

/*
	Draw a String.

	float x: The X-Position where to draw.
	float y: The Y-Position where to draw.
	float size: The size for the Font.
	u32 color: The Text Color.
	std::string Text: The Text which should be drawn.
	int maxWidth: (Optional) The max width of the Text.
	int maxHeight: (Optional) The max height of the Text.
	C2D_Font fnt: (Optional) The wanted C2D_Font. Is nullptr by default.
	int flags: (Optional) C2D text flags to use.
*/
void Gui::DrawString(float x, float y, float size, u32 color, std::string Text, int maxWidth, int maxHeight, C2D_Font fnt, int flags) {
	C2D_Text c2d_text;

	if (fnt) C2D_TextFontParse(&c2d_text, fnt, TextBuf, Text.c_str());
	else C2D_TextFontParse(&c2d_text, Font, TextBuf, Text.c_str());

	C2D_TextOptimize(&c2d_text);

	float heightScale;

	if (maxHeight == 0) {
		heightScale = size;

	} else {
		if (fnt) heightScale = std::min(size, size*(maxHeight/Gui::GetStringHeight(size, Text, fnt)));
		else heightScale = std::min(size, size*(maxHeight/Gui::GetStringHeight(size, Text)));
	}

	if (maxWidth == 0) {
		C2D_DrawText(&c2d_text, C2D_WithColor | flags, x, y, 0.5f, size, heightScale, color);
	} else if (flags & C2D_WordWrap) {
		C2D_DrawText(&c2d_text, C2D_WithColor | flags, x, y, 0.5f, size, heightScale, color, (float)maxWidth);
	} else {
		if (fnt) C2D_DrawText(&c2d_text, C2D_WithColor | flags, x, y, 0.5f, std::min(size, size*(maxWidth/Gui::GetStringWidth(size, Text, fnt))), heightScale, color);
		else C2D_DrawText(&c2d_text, C2D_WithColor | flags, x, y, 0.5f, std::min(size, size*(maxWidth/Gui::GetStringWidth(size, Text))), heightScale, color);
	}
}

/*
	Get String or Text Width.

	float size: The Textsize.
	std::string Text: The Text.
	C2D_Font fnt: (Optional) The wanted C2D_Font. Is nullptr by default.
*/
float Gui::GetStringWidth(float size, std::string Text, C2D_Font fnt) {
	float width = 0;

	if (fnt) GetStringSize(size, &width, NULL, Text, fnt);
	else GetStringSize(size, &width, NULL, Text);

	return width;
}

/*
	Get String or Text Size.

	float size: The Textsize.
	float *width: Pointer where to store the width.
	float *height: Pointer where to store the height.
	std::string Text: The Text.
	C2D_Font fnt: (Optional) The wanted C2D_Font. Is nullptr by default.
*/
void Gui::GetStringSize(float size, float *width, float *height, std::string Text, C2D_Font fnt) {
	C2D_Text c2d_text;

	if (fnt) C2D_TextFontParse(&c2d_text, fnt, TextBuf, Text.c_str());
	else C2D_TextFontParse(&c2d_text, Font, TextBuf, Text.c_str());

	C2D_TextGetDimensions(&c2d_text, size, size, width, height);
}


/*
	Get String or Text Height.

	float size: The Textsize.
	std::string Text: The Text.
	C2D_Font fnt: (Optional) The wanted C2D_Font. Is nullptr by default.
*/
float Gui::GetStringHeight(float size, std::string Text, C2D_Font fnt) {
	float height = 0;

	if (fnt) GetStringSize(size, NULL, &height, Text.c_str(), fnt);
	else GetStringSize(size, NULL, &height, Text.c_str());

	return height;
}

/*
	Draw a Rectangle.

	float x: The X-Position where to draw.
	float y: The Y-Position where to draw.
	float w: The width of the rectangle.
	float h: The height of the rectangle.
	u32 color: The color.
*/
bool Gui::Draw_Rect(float x, float y, float w, float h, u32 color) {
	return C2D_DrawRectSolid(x, y, 0.5f, w, h, color);
}

/*
	Draw's the current screen's draw.

	bool stack: If using the stack-screens or not.
*/
void Gui::DrawScreen(bool stack) {
	if (!stack) {
		if (usedScreen) usedScreen->Draw();

	} else {
		if (!screens.empty()) screens.top()->Draw();
	}
}

/*
	Do the current screen's logic.

	u32 hDown: The hidKeysDown() variable.
	u32 hHeld: The hidKeysHeld() variable.
	touchPosition touch: The touchPosition variable.
	bool waitFade: If waiting for the fade until control of the screen or not.
	bool stack: If using the stack-screens.
*/
void Gui::ScreenLogic(u32 hDown, u32 hHeld, touchPosition touch, bool waitFade, bool stack) {
	if (waitFade) {
		if (!fadein && !fadeout && !fadein2 && !fadeout2) {
			if (!stack) if (usedScreen) usedScreen->Logic(hDown, hHeld, touch);

		} else {
			if (!screens.empty()) screens.top()->Logic(hDown, hHeld, touch);
		}

	} else {
		if (!stack) {
			if (usedScreen) usedScreen->Logic(hDown, hHeld, touch);

		} else {
			if (!screens.empty()) screens.top()->Logic(hDown, hHeld, touch);
		}
	}
}

/*
	Move's the tempScreen to the used one.

	bool stack: If using the stack-screens or not.
*/
void Gui::transferScreen(bool stack) {
	if (!stack) {
		if (tempScreen) usedScreen = std::move(tempScreen);

	} else {
		if (tempScreen) screens.push(std::move(tempScreen));
	}
}

/*
	Set the current Screen.

	std::unique_ptr<Screen> screen: The screen class.
	bool fade: If doing a fade effect or not.
	bool stack: If using the stack-screens or not.
*/
void Gui::setScreen(std::unique_ptr<Screen> screen, bool fade, bool stack) {
	tempScreen = std::move(screen);

	/* Switch screen without fade. */
	if (!fade) {
		Gui::transferScreen(stack);

	} else {
		/* Fade, then switch. */
		fadeout = true;
	}
}

/*
	Fade's the screen in and out and transfer the screen.
	Credits goes to RocketRobz & SavvyManager.

	int fadeoutFrames: The fadeout frames.
	int fadeinFrames: The fadein frames.
	bool stack: If using the stack-screens or not. (Used to properly transfer screens).
*/
void Gui::fadeEffects(int fadeoutFrames, int fadeinFrames, bool stack) {
	if (fadein) {
		fadealpha -= fadeinFrames;

		if (fadealpha < 0) {
			fadealpha = 0;
			fadecolor = 255;
			fadein = false;
		}
	}

	if (stack) {
		if (fadein2) {
			fadealpha -= fadeinFrames;

			if (fadealpha < 0) {
				fadealpha = 0;
				fadecolor = 255;
				fadein2 = false;
			}
		}
	}

	if (fadeout) {
		fadealpha += fadeoutFrames;

		if (fadealpha > 255) {
			fadealpha = 255;
			Gui::transferScreen(stack); // Transfer Temp screen to the stack / used one.
			fadein = true;
			fadeout = false;
		}
	}

	if (stack) {
		if (fadeout2) {
			fadealpha += fadeoutFrames;

			if (fadealpha > 255) {
				fadealpha = 255;
				Gui::screenBack2(); // Go screen back.
				fadein2 = true;
				fadeout2 = false;
			}
		}
	}
}

/*
	Go a screen back. (Stack only!)

	bool fade: If doing a fade or not.
*/
void Gui::screenBack(bool fade) {
	if (!fade) {
		if (screens.size() > 0) screens.pop();

	} else {
		if (screens.size() > 0) fadeout2 = true;
	}
}
void Gui::screenBack2() { if (screens.size() > 0) screens.pop(); };

/*
	Select, on which Screen should be drawn.

	C3D_RenderTarget *screen: The render target.
*/
void Gui::ScreenDraw(C3D_RenderTarget *screen) {
	C2D_SceneBegin(screen);
	currentScreen = (screen == Top || screen == TopRight) ? 1 : 0;
}

/*
	Draw a grid.

	float xPos: The X-Position of the grid.
	float yPos: The Y-Position of the grid.
	float Width: The width of the grid.
	float Height: The height of the grid.
	u32 color: The color of the grid.
	u32 bgColor: The BG Color of the grid.
*/
void Gui::drawGrid(float xPos, float yPos, float Width, float Height, u32 color, u32 bgColor) {
	static constexpr int w	= 1;

	/* BG Color for the Grid. (Transparent by default.) */
	Gui::Draw_Rect(xPos, yPos, Width, Height, bgColor);

	/* Grid part, Top. */
	Gui::Draw_Rect(xPos, yPos, Width, w, color);
	/* Left. */
	Gui::Draw_Rect(xPos, yPos + w, w, Height - 2 * w, color);
	/* Right. */
	Gui::Draw_Rect(xPos + Width - w, yPos + w, w, Height - 2 * w, color);
	/* Bottom. */
	Gui::Draw_Rect(xPos, yPos + Height - w, Width, w, color);
}

/*
	Draw an animated selector.

	float xPos: The X-Position of the selector.
	float yPos: The Y-Position of the selector.
	float Width: The width of the selector.
	float Height: The height of the selector.
	float speed: The selector animation speed. Use .030f or so.
	u32 SelectorColor: The selector fade color.
	u32 bgColor: The selector BG color.
*/
void Gui::drawAnimatedSelector(float xPos, float yPos, float Width, float Height, float speed, u32 SelectorColor, u32 bgColor) {
	static constexpr int w		= 2;
	static float timer			= 0.0f;
	float highlight_multiplier  = fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
	u8 r						= SelectorColor & 0xFF;
	u8 g						= (SelectorColor >> 8) & 0xFF;
	u8 b						= (SelectorColor >> 16) & 0xFF;
	u32 color 					= C2D_Color32(r + (255 - r) * highlight_multiplier, g + (255 - g) * highlight_multiplier, b + (255 - b) * highlight_multiplier, 255);

	/* BG Color for the Selector. */
	Gui::Draw_Rect(xPos, yPos, Width, Height, bgColor);

	/* Selector part, Top. */
	Gui::Draw_Rect(xPos, yPos, Width, w, color);
	/* Left. */
	Gui::Draw_Rect(xPos, yPos + w, w, Height - 2 * w, color);
	/* Right. */
	Gui::Draw_Rect(xPos + Width - w, yPos + w, w, Height - 2 * w, color);
	/* Bottom. */
	Gui::Draw_Rect(xPos, yPos + Height - w, Width, w, color);

	timer += speed;
}