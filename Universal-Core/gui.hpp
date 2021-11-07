/*
*   This file is part of Universal-Core
*   Copyright (C) 2020-2021 Universal-Team
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

#ifndef _UNIVERSAL_CORE_GUI_HPP
#define _UNIVERSAL_CORE_GUI_HPP

#include "screen.hpp"

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>

namespace Gui {
	/*
		Clear the Text Buffer.
	*/
	void clearTextBufs(void);

	/*
		Draw a sprite from a SpriteSheet.
		sheet: The SpriteSheet which should be used.
		imgIndex: The index of the sprite from the sheet which should be drawn.
		x: The X Position where the sprite should be drawn.
		y: The Y Position where the sprite should be drawn.
		ScaleX: The X-Scale for the sprite. (Optional!)
		ScaleY: The Y-Scale for the sprite. (Optional!)
	*/
	void DrawSprite(C2D_SpriteSheet sheet, size_t imgindex, int x, int y, float ScaleX = 1, float ScaleY = 1);

	/*
		Initialize the GUI with Citro2D & Citro3D and initialize the Textbuffer.
		call this when initializing.

		fontRegion: The region to use for the system font.
	*/
	Result init(CFG_Region fontRegion = CFG_REGION_USA);

	/*
		Load a system font.

		fontRegion: The region to use for the system font.
	*/
	void loadSystemFont(CFG_Region fontRegion = CFG_REGION_USA);

	/*
		Load a Font. (BCFNT)

		fnt: The C2D_Font variable which should be initialized.
		Path: Path to the BCFNT file.
		if you're unsure, just call 'Gui::init();' and it will load the system font.
	*/
	Result loadFont(C2D_Font &fnt, const char *Path = "");

	/*
		Unload a Font. (BCFNT)

		fnt: The C2D_Font variable which should be unloaded.
	*/
	Result unloadFont(C2D_Font &fnt);

	/*
		Load a spritesheet.

		Path: Path to the SpriteSheet file. (T3X)
		sheet: Reference to the C2D_SpriteSheet declaration.
	*/
	Result loadSheet(const char *Path, C2D_SpriteSheet &sheet);

	/*
		Unload a spritesheet.

		sheet: Reference to the C2D_SpriteSheet which should be free'd.
	*/
	Result unloadSheet(C2D_SpriteSheet &sheet);

	/*
		Exit the GUI.
		Call this at exit.
	*/
	void exit(void);

	/*
		Reinit the GUI.

		fontRegion: The region to use for the system font.
	*/
	Result reinit(CFG_Region fontRegion = CFG_REGION_USA);

	/*
		Draws a centered String.
		x: The X Offset from center. (Center: 200 px on top, 160 px on Bottom.)
		y: The Y Position of the Text.
		size: The size of the Text.
		color: The Color of the Text.
		Text: The Text which should be displayed.
		maxWidth: The maxWidth for the Text. (Optional!)
		maxHeight: The maxHeight of the Text. (Optional!)
		fnt: The Font which should be used. Uses SystemFont by default. (Optional!)
		int flags: C2D text flags to use. (Optional!)
	*/
	void DrawStringCentered(float x, float y, float size, u32 color, const std::string &Text, int maxWidth = 0, int maxHeight = 0, C2D_Font fnt = nullptr, int flags = 0);

	/*
		Draws a String.

		x: The X Position where the Text should be drawn.
		y: The Y Position where the Text should be drawn.
		size: The size of the Text.
		color: The Color of the Text.
		Text: The Text which should be displayed.
		maxWidth: The maxWidth for the Text. (Optional!)
		maxHeight: The maxHeight of the Text. (Optional!)
		fnt: The Font which should be used. Uses SystemFont by default. (Optional!)
		flags: C2D text flags to use.
	*/
	void DrawString(float x, float y, float size, u32 color, const std::string &Text, int maxWidth = 0, int maxHeight = 0, C2D_Font fnt = nullptr, int flags = 0);

	/*
		Get the width of a String.

		size: The size of the Text.
		Text: The Text where the width should be getted from.
		fnt: The Font which should be used. Uses SystemFont by default. (Optional!)
	*/
	float GetStringWidth(float size, const std::string &Text, C2D_Font fnt = nullptr);

	/*
		Get the size of a String.

		size: The size of the Text.
		width: The width of the Text.
		height: The height of the Text.
		Text: The Text where the size should be getted from.
		fnt: The Font which should be used. Uses SystemFont by default. (Optional!)
	*/
	void GetStringSize(float size, float *width, float *height, const std::string &Text, C2D_Font fnt = nullptr);

	/*
		Get the height of a String.

		size: The size of the Text.
		Text: The Text where the height should be getted from.
		fnt: The Font which should be used. Uses SystemFont by default. (Optional!)
	*/
	float GetStringHeight(float size, const std::string &Text, C2D_Font fnt = nullptr);

	/*
		Draw a Rectangle.

		x: X Position of the Rectangle.
		y: Y Position of the Rectangle.
		w: The width of the rectangle.
		h: The height of the rectangle.
		color: The color of the rectangle.
	*/
	bool Draw_Rect(float x, float y, float w, float h, u32 color);

	/*
		Used for the current Screen's Draw. (Optional!)
		stack: Is it the stack variant?
	*/
	void DrawScreen(bool stack = false);

	/*
		Used for the current Screen's Logic. (Optional!)

		hDown: the hidKeysDown() variable.
		hHeld: the HidKeysHeld() variable.
		touch: The TouchPosition variable.
		waitFade: Wheter to wait until the fade ends.
		stack: Is it the stack variant?
	*/
	void ScreenLogic(u32 hDown, u32 hHeld, touchPosition touch, bool waitFade = true, bool stack = false);

	/*
		Transfer the Temp Screen to the used one. (Optional!)

		stack: Is it the stack variant?
		It will check, if the tempScreen variable is not nullptr, so don't worry.
	*/
	void transferScreen(bool stack = false);

	/*
		Set a specific Screen with switch function. (Optional!)

		screen: unique_ptr of the screen. (Optional by using the screen class.)
		screenSwitch: Wheter to switch to the current screen.
		stack: Is it the stack variant?
	*/
	void setScreen(std::unique_ptr<Screen> screen, bool fade = false, bool stack = false);

	/*
		Fades into screens and calls the constructor after it. (Optional!)
		fadeoutFrames: Amount of frames for fadeout.
		fadeinFrames: Amount of frames for fadein.
		stack: Is it the stack variant?
	*/
	void fadeEffects(int fadeoutFrames = 6, int fadeinFrames = 6, bool stack = false);

	void screenBack(bool fade = false); // Goes a screen back. (Set!) (Stack only!)
	void screenBack2(); // Goes a screen back.(Action!) (Stack only!)

	/*
		Set on which screen to draw.

		screen: The render target. (Targets are inside the screenCommon.hpp file.)
	*/
	void ScreenDraw(C3D_RenderTarget * screen);

	/*
		Draws a grid.
		xPos: X Position of the grid.
		yPos: Y Position of the grid.
		Width: Width of the grid.
		Height: Height of the grid.
		color: Color of the grid.
		bgColor: The BG Color from the grid. (Optional! It's transparent by default.)
	*/
	void drawGrid(float xPos, float yPos, float Width, float Height, u32 color, u32 bgColor = C2D_Color32(0, 0, 0, 0));

	/*
		Draws an animated selector.
		xPos: X Position of the selector.
		yPos: Y Position of the Selector.
		Width: Width of the Selector.
		Height: Height of the Selector.
		speed: The speed of the animation. (Use .030f or something by default.)
		SelectorColor: The Color of the Selector outline.
		bgColor: The BG Color from the selector. (Optional! It's transparent by default.)
	*/
	void drawAnimatedSelector(float xPos, float yPos, float Width, float Height, float speed, u32 SelectorColor, u32 bgColor = C2D_Color32(0, 0, 0, 0));
};

#endif