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
#include "gfx.hpp"

/*
	Déssinez la base de l'écran supèrieur.
*/
void GFX::DrawTop(void) {
	Gui::ScreenDraw(Top);
	Gui::Draw_Rect(0, 0, 400, 25, BAR_COLOR);
	Gui::Draw_Rect(0, 26, 400, 214, BG_COLOR);
	Gui::Draw_Rect(0, 25, 400, 1, BAR_OUTL_COLOR);
}

/*
	Déssinez la base de l'écran infèrieur.
*/
void GFX::DrawBottom() {
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, BG_COLOR);
}

/*
	Dessinez la boîte.

	float xPos: La position X où dessiner la boîte.
	float yPos: La position Y où dessiner la boîte.
	float width: La largeur du bouton.
	float height: La hauteur du bouton.
	bool selected: Si sélectionné ou non.
	uint32_t clr: (Facultatif) La couleur de l’intérieur de la boîte.
*/
void GFX::DrawBox(float xPos, float yPos, float width, float height, bool selected, uint32_t clr) {
	Gui::Draw_Rect(xPos, yPos, width, height, BOX_INSIDE_COLOR); // Dessinez le BG du milieu.

	if (selected) {
		static constexpr int depth = 2;

		Gui::Draw_Rect(xPos - depth, yPos - depth, width + depth * 2, depth, BOX_SELECTED_COLOR); // Haut.
		Gui::Draw_Rect(xPos - depth, yPos - depth, depth, height + depth * 2, BOX_SELECTED_COLOR); // Gauche.
		Gui::Draw_Rect(xPos + width, yPos - depth, depth, height + depth * 2, BOX_SELECTED_COLOR); // Droite.
		Gui::Draw_Rect(xPos - depth, yPos + height, width + depth * 2, depth, BOX_SELECTED_COLOR); // Bas.
	}
}

extern C2D_SpriteSheet sprites;

/*
	Dessinez un Sprite des sprites SpriteSheet.
	
	int img: L'index d'image.
	int x: la position X où dessiner.
	int y: la position Y où dessiner.
	float ScaleX: (Facultatif) L'échelle X du Sprite. (1 par défaut)
	float ScaleY: (Facultatif) L'échelle Y du Sprite. (1 par défaut)
*/
void GFX::DrawSprite(int img, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(sprites, img, x, y, ScaleX, ScaleY);
}

/*
	Dessinez la case à cocher.
	
	float xPos: La position X où dessiner la boîte.
	float yPos: La position Y où dessiner la boîte.
	booléen sélectionné: si coché ou non.
*/
void GFX::DrawCheckbox(float xPos, float yPos, bool selected) {
	GFX::DrawSprite((selected ? sprites_checked_idx : sprites_unchecked_idx), xPos, yPos);
}

/*
	Dessinez la boîte à bascule.
	
	float xPos: La position X où dessiner la bascule.
	float yPos: La position Y où dessiner la bascule.
	bool toggled: Si basculé ou non.
*/
void GFX::DrawToggle(float xPos, float yPos, bool toggled) {
	GFX::DrawSprite((toggled ? sprites_toggle_on_idx : sprites_toggle_off_idx), xPos, yPos);
}