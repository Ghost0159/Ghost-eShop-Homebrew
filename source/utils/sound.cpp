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

#include "sound.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

/*
	Reference: http://yannesposito.com/Scratch/en/blog/2010-10-14-Fun-with-wav/
*/
typedef struct _WavHeader {
	char magic[4]; // "RIFF"
	u32 totallength; // Total file length, minus 8.
	char wavefmt[8]; // Should be "WAVEfmt ".
	u32 format; // 16 for PCM format.
	u16 pcm; // 1 for PCM format.
	u16 channels; // Channels.
	u32 frequency; // Sampling frequency.
	u32 bytes_per_second;
	u16 bytes_by_capture;
	u16 bits_per_sample;
	char data[4]; // "data".
	u32 bytes_in_data;
} WavHeader;
static_assert(sizeof(WavHeader) == 44, "WavHeader size is not 44 bytes.");

#define _MAX_SIZE 10485760 // 10 MiB.

/*
	Initialize the sound.

	const std::string &path: Path to the file to play.
	const int channel: The channel to use. 1 by default.
	const bool toloop: If should loop or not. True by default.
*/
Sound::Sound(const std::string &path, const int channel, const bool toloop) {
	ndspSetOutputMode(NDSP_OUTPUT_MONO);
	ndspSetOutputCount(2); // Amount of buffers.

	/* Reading wav file. */
	FILE *file = fopen(path.c_str(), "rb");

	if (!file) {
		printf("Could not open the WAV file: %s.\n", path.c_str());
		this->good = false;
		return;
	}

	WavHeader wavHeader;
	size_t read = fread(&wavHeader, 1, sizeof(wavHeader), file);
	if (read != sizeof(wavHeader)) {
		/* Short read. */
		printf("WAV file header is too short: %s.\n", path.c_str());
		fclose(file);
		this->good = false;
		return;
	}

	/* Verify the header. */
	static const char RIFF_magic[4] = { 'R','I','F','F' };
	if (memcmp(wavHeader.magic, RIFF_magic, sizeof(wavHeader.magic)) != 0) {
		/* Incorrect magic number. */
		printf("Wrong file format.\n");
		fclose(file);
		this->good = false;
		return;
	}

	if (wavHeader.totallength == 0 ||
	   (wavHeader.channels != 1 && wavHeader.channels != 2) ||
	   (wavHeader.bits_per_sample != 8 && wavHeader.bits_per_sample != 16)) {
		/* Unsupported WAV file. */
		printf("Corrupted wav file.\n");
		fclose(file);
		this->good = false;
		return;
	}

	/* Get the file size. */
	fseek(file, 0, SEEK_END);
	this->dataSize = ftell(file) - sizeof(wavHeader);

	if (this->dataSize > _MAX_SIZE) {
		fclose(file);
		this->good = false;
		return;
	}

	/* Allocating and reading samples. */
	this->data = reinterpret_cast<u8 *>(linearAlloc(this->dataSize));
	fseek(file, 44, SEEK_SET);
	fread(this->data, 1, this->dataSize, file);
	fclose(file);

	//if (wavHeader.bits_per_sample == 16) this->dataSize /= 2; // Not sure.. if that is actually needed at all.

	this->chnl = channel;
	ndspChnReset(this->chnl);
	ndspChnSetInterp(this->chnl, NDSP_INTERP_NONE);
	ndspChnSetRate(this->chnl, float(wavHeader.frequency));
	ndspChnSetFormat(this->chnl, ((wavHeader.bits_per_sample == 8) ? NDSP_FORMAT_MONO_PCM8 : NDSP_FORMAT_MONO_PCM16));

	/* Create and play a wav buffer. */
	memset(&this->waveBuf, 0, sizeof(this->waveBuf));

	this->waveBuf.data_vaddr = reinterpret_cast<u32 *>(this->data);
	this->waveBuf.nsamples = this->dataSize / (wavHeader.bits_per_sample >> 3);
	this->waveBuf.looping = toloop;
	this->waveBuf.status = NDSP_WBUF_FREE;
}

Sound::~Sound() {
	if (this->good) {
		this->waveBuf.data_vaddr = 0;
		this->waveBuf.nsamples = 0;
		this->waveBuf.looping = false;
		this->waveBuf.status = 0;
		ndspChnWaveBufClear(this->chnl);

		if (this->data) linearFree(this->data);
	}
}

/*
	Plays the sound.
*/
void Sound::play() {
	if (!this->data || !this->good) return;

	DSP_FlushDataCache(this->data, this->dataSize);
	ndspChnWaveBufAdd(this->chnl, &this->waveBuf);
}

/*
	Stops the sound.
*/
void Sound::stop() {
	if (!this->data || !this->good) return;

	ndspChnWaveBufClear(this->chnl);
}