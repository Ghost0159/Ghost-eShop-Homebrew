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

#ifndef _GHOST_ESHOP_SOUND_HPP
#define _GHOST_ESHOP_SOUND_HPP

#include <3ds.h>
#include <string>

class Sound {
public:
	Sound(const std::string &path, const int channel = 1, const bool toloop = true);
	~Sound();
	void play();
	void stop();
private:
	u32 dataSize;
	bool good = true;
	ndspWaveBuf waveBuf;
	u8 *data = nullptr;
	int chnl;
};

#endif