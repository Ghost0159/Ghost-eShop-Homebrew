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

#ifndef _GHOST_ESHOP_GFX_HPP
#define _GHOST_ESHOP_GFX_HPP

#include <citro2d.h>
#include <memory>
#include "common.hpp"
#include "sprites.h"
#include "theme.hpp"

//Extern pointer to the current theme
extern std::unique_ptr<Theme> UIThemes;

//Standard colors
#define WHITE C2D_Color32(255, 255, 255, 255)
#define BLACK C2D_Color32(0, 0, 0, 255)
#define TRANSPARENT C2D_Color32(0, 0, 0, 0)
#define DIM_COLOR C2D_Color32(0, 0, 0, 190)

// Namespace for all graphics related functions
namespace GFX {
    // Draw the top screen
    void DrawTop();
    // Draw the bottom screen
    void DrawBottom();
    // Draw a sprite from the sprites array
    void DrawSprite(int img, int x, int y, float ScaleX = 1.0f, float ScaleY = 1.0f);
    // Draw a box with a specific color and selection state
    void DrawBox(float xPos, float yPos, float width = 50.0f, float height = 50.0f, bool selected = false, uint32_t clr = UIThemes->BoxInside());
    // Draw a checkbox with a specific selection state
    void DrawCheckbox(float xPos, float yPos, bool selected);
	// Draw a toggle button with a specific toggle state
    void DrawToggle(float xPos, float yPos, bool toggled);
    // Draw the current time
    void DrawTime();
    // Draw the battery status
    void DrawBattery();
    // Handle changes to the battery status
    void HandleBattery();
    // Draw the wifi status
    void DrawWifi();
    // Draw an icon with specific color, blend power and scale
    void DrawIcon(const int Idx, int X, int Y, uint32_t Color = UIThemes->SideBarIconColor(), float BlendPower = 1.0f, float ScaleX = 1.0f, float ScaleY = 1.0f);
};

#endif
