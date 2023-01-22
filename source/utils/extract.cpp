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

#include "extract.hpp"
#include "files.hpp"
#include "queueSystem.hpp"
#include "scriptUtils.hpp"
#include <archive.hpp>
#include <archive_entry.hpp>
#include <regex>

int filesExtracted = 0, extractFilesCount = 0;
std::string extractingFile = "";

/* That are our Extract Progressbar variable. */
u32 extractSize = 0, writeOffset = 0;

Result getExtractedSize(const std::string &archivePath, const std::string &wantedFile) {
	extractSize = 0, writeOffset = 0, filesExtracted = 0, extractFilesCount = 0;

	archive *a = archive_read_new();
	archive_entry *entry;

	archive_read_support_format_all(a);

	if (archive_read_open_filename(a, archivePath.c_str(), 0x4000) != ARCHIVE_OK) return EXTRACT_ERROR_OPENFILE;

	while(archive_read_next_header(a, &entry) == ARCHIVE_OK) {
		int size = archive_entry_size(entry);
		std::smatch match;
		std::string entryName(archive_entry_pathname(entry));
		if (std::regex_search(entryName, match, std::regex(wantedFile))) {
			extractSize += size;
			extractFilesCount++;
		}
	}

	archive_read_close(a);
	archive_read_free(a);
	return EXTRACT_ERROR_NONE;
}

Result extractArchive(const std::string &archivePath, const std::string &wantedFile, const std::string &outputPath) {
	if (getAvailableSpace() < extractSize) return -1; // Out of space.

	archive *a = archive_read_new();
	archive_entry *entry;

	archive_read_support_format_all(a);

	if (archive_read_open_filename(a, archivePath.c_str(), 0x4000) != ARCHIVE_OK) {
		archive_read_close(a);
		archive_read_free(a);
		return EXTRACT_ERROR_OPENFILE;
	}

	while(archive_read_next_header(a, &entry) == ARCHIVE_OK) {
		std::smatch match;
		std::string entryName(archive_entry_pathname(entry));
		if (std::regex_search(entryName, match, std::regex(wantedFile))) {
			extractingFile = outputPath + match.suffix().str();
			filesExtracted++;

			/* Make directories. */
			for (char *slashpos = strchr(extractingFile.c_str() + 1, '/'); slashpos != NULL; slashpos = strchr(slashpos + 1, '/')) {
				char bak = *(slashpos);
				*(slashpos) = '\0';

				mkdir(extractingFile.c_str(), 0777);

				*(slashpos) = bak;
			}

			/* If directory then mkdir it and skip extraction. */
			if (S_ISDIR(archive_entry_mode(entry))) {
				mkdir(extractingFile.c_str(), 0777);
				continue;
			}

			uint sizeLeft = archive_entry_size(entry);

			FILE *file = fopen(extractingFile.c_str(), "wb");
			if (!file) {
				archive_read_close(a);
				archive_read_free(a);
				return EXTRACT_ERROR_WRITEFILE;
			}

			u8 *buf = new u8[0x30000];
			if (!buf) {
				fclose(file);
				archive_read_close(a);
				archive_read_free(a);
				return EXTRACT_ERROR_ALLOC;
			}

			while(sizeLeft > 0) {
				u64 toRead = std::min(0x30000u, sizeLeft);
				ssize_t size = archive_read_data(a, buf, toRead);

				/* Archive error, stop extracting. */
				if (size < 0) {
					fclose(file);
					delete[] buf;
					archive_read_close(a);
					archive_read_free(a);
					return EXTRACT_ERROR_ARCHIVE;
				}

				ssize_t written = fwrite(buf, 1, size, file);

				/* Failed to write, likely out of space. */
				if (written != size) {
					fclose(file);
					delete[] buf;
					archive_read_close(a);
					archive_read_free(a);
					return EXTRACT_ERROR_WRITEFILE;
				}

				sizeLeft -= size;
				writeOffset += size;
			}

			fclose(file);
			delete[] buf;

			if (QueueSystem::CancelCallback) goto exit; // Cancel Extraction.
		}
	}

	exit:
		archive_read_close(a);
		archive_read_free(a);
		return EXTRACT_ERROR_NONE;
}