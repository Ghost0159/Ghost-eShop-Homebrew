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

#ifndef _GHOST_ESHOP_COMMON_HPP
#define _GHOST_ESHOP_COMMON_HPP

#include "config.hpp"
#include "gfx.hpp"
#include "lang.hpp"
#include "msg.hpp"
#include "screenCommon.hpp"
#include <3ds.h>
#include <vector>

#define _STORE_PATH "sdmc:/3ds/Universal-Updater/stores/"
#define _META_PATH "sdmc:/3ds/Universal-Updater/MetaData.json"
#define _THEME_AMOUNT 2
#define _ESHOP_VERSION 4

inline std::unique_ptr<Config> config;
inline uint32_t hRepeat, hDown, hHeld;
inline touchPosition touch;
inline C2D_Font font;

#endif