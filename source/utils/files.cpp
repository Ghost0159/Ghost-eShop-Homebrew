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

#include "files.hpp"
#include <sys/stat.h>
#include <sys/statvfs.h>

FS_Path getPathInfo(const char *path, FS_ArchiveID *archive) {
	*archive = ARCHIVE_SDMC;
	FS_Path filePath = { PATH_INVALID, 0, nullptr };
	unsigned int prefixlen = 0;

	if (!strncmp(path, "sdmc:/", 6)) {
		prefixlen = 5;

	} else if (*path != '/') {
		/*
			if the path is local (doesnt start with a slash),
			it needs to be appended to the working dir to be valid.
		*/
		char *actualPath = NULL;
		asprintf(&actualPath, "%s%s", "/", path);
		filePath = fsMakePath(PATH_ASCII, actualPath);
		free(actualPath);
	}

	/* if the filePath wasnt set above, set it. */
	if (filePath.size == 0) filePath = fsMakePath(PATH_ASCII, path + prefixlen);

	return filePath;
}

Result makeDirs(const char *path) {
	Result ret = 0;
	FS_ArchiveID archiveID;
	FS_Path filePath = getPathInfo(path, &archiveID);
	FS_Archive archive;

	ret = FSUSER_OpenArchive(&archive, archiveID, fsMakePath(PATH_EMPTY, ""));

	for (char *slashpos = strchr(path + 1, '/'); slashpos != NULL; slashpos = strchr(slashpos + 1, '/')) {
		char bak = *(slashpos);
		*(slashpos) = '\0';
		Handle dirHandle;

		ret = FSUSER_OpenDirectory(&dirHandle, archive, filePath);
		if (R_SUCCEEDED(ret)) FSDIR_Close(dirHandle);
		else ret = FSUSER_CreateDirectory(archive, filePath, FS_ATTRIBUTE_DIRECTORY);

		*(slashpos) = bak;
	}

	FSUSER_CloseArchive(archive);

	return ret;
}

Result openFile(Handle *fileHandle, const char *path, bool write) {
	FS_ArchiveID archive;
	FS_Path filePath = getPathInfo(path, &archive);
	u32 flags = (write ? (FS_OPEN_CREATE | FS_OPEN_WRITE) : FS_OPEN_READ);

	Result ret = 0;
	ret = makeDirs(strdup(path));
	ret = FSUSER_OpenFileDirectly(fileHandle, archive, fsMakePath(PATH_EMPTY, ""), filePath, flags, 0);
	if (write)	ret = FSFILE_SetSize(*fileHandle, 0); // truncate the file to remove previous contents before writing.

	return ret;
}

Result deleteFile(const char *path) {
	FS_ArchiveID archiveID;
	FS_Path filePath = getPathInfo(path, &archiveID);

	FS_Archive archive;

	Result ret = FSUSER_OpenArchive(&archive, archiveID, fsMakePath(PATH_EMPTY, ""));
	if (R_FAILED(ret)) return ret;
	ret = FSUSER_DeleteFile(archive, filePath);
	FSUSER_CloseArchive(archive);

	return ret;
}

Result removeDir(const char *path) {
	FS_ArchiveID archiveID;
	FS_Path filePath = getPathInfo(path, &archiveID);
	FS_Archive archive;

	Result ret = FSUSER_OpenArchive(&archive, archiveID, fsMakePath(PATH_EMPTY, ""));
	if (R_FAILED(ret)) return ret;
	ret = FSUSER_DeleteDirectory(archive, filePath);
	FSUSER_CloseArchive(archive);

	return ret;
}

Result removeDirRecursive(const char *path) {
	FS_ArchiveID archiveID;
	FS_Path filePath = getPathInfo(path, &archiveID);
	FS_Archive archive;

	Result ret = FSUSER_OpenArchive(&archive, archiveID, fsMakePath(PATH_EMPTY, ""));
	if (R_FAILED(ret)) return ret;
	ret = FSUSER_DeleteDirectoryRecursively(archive, filePath);
	FSUSER_CloseArchive(archive);

	return ret;
}

/* Code borrowed from GodMode9i:
	https://github.com/DS-Homebrew/GodMode9i/blob/d68ac105e68b4a1fc2c706a08c7a394255c325c2/arm9/source/driveOperations.cpp#L166-L170
*/
u64 getAvailableSpace() {
	struct statvfs st;
	statvfs("sdmc:/", &st);
	return (u64)st.f_bsize * (u64)st.f_bavail;
}