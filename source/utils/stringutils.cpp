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

#include "common.hpp"
#include "stringutils.hpp"
#include <stdarg.h>

/*
	To lowercase conversion.

	const std::string &str: The string which should be converted.
*/
std::string StringUtils::lower_case(const std::string &str) {
	std::string lower;
	transform(str.begin(), str.end(), std::back_inserter(lower), tolower); // Transform the string to lowercase.

	return lower;
}

/*
	Fetch strings from a vector and return it as a single string.

	std::vector<std::string> fetch: The vector.
*/
std::string StringUtils::FetchStringsFromVector(const std::vector<std::string> &fetch) {
	std::string temp;

	if (fetch.size() < 1) return ""; // Smaller than 1 --> Return empty.

	for (int i = 0; i < (int)fetch.size(); i++) {
		if (i != (int)fetch.size() - 1) {
			temp += fetch[i] + ", ";

		} else {
			temp += fetch[i];
		}
	}

	return temp;
}

/*
	adapted from GM9i's byte parsing.
*/
std::string StringUtils::formatBytes(u64 bytes) {
	char out[32];

	if (bytes == 1)					snprintf(out, sizeof(out), "%lld Byte", bytes);
	else if (bytes < 1ull << 10)	snprintf(out, sizeof(out), "%lld Bytes", bytes);
	else if (bytes < 1ull << 20)	snprintf(out, sizeof(out), "%.1f KiB", (float)bytes / 1024);
	else if (bytes < 1ull << 30)	snprintf(out, sizeof(out), "%.1f MiB", (float)bytes / 1024 / 1024);
	else if (bytes < 1ull << 40)	snprintf(out, sizeof(out), "%.1f GiB", (float)bytes / 1024 / 1024 / 1024);
	else							snprintf(out, sizeof(out), "%.1f TiB", (float)bytes / 1024 / 1024 / 1024 / 1024);

	return out;
}

/*
	Return a vector of all marks.
*/
std::vector<std::string> StringUtils::GetMarks(int marks) {
	std::vector<std::string> out;

	if (marks & favoriteMarks::STAR)	out.push_back( "★" );
	if (marks & favoriteMarks::HEART)	out.push_back( "♥" );
	if (marks & favoriteMarks::DIAMOND) out.push_back( "♦" );
	if (marks & favoriteMarks::CLUBS)	out.push_back( "♣" );
	if (marks & favoriteMarks::SPADE)	out.push_back( "♠" );

	return out;
}

/*
	Return a string of all marks.
*/
std::string StringUtils::GetMarkString(int marks) {
	std::string out;

	if (marks & favoriteMarks::STAR)	out += "★";
	if (marks & favoriteMarks::HEART)	out += "♥";
	if (marks & favoriteMarks::DIAMOND) out += "♦";
	if (marks & favoriteMarks::CLUBS)	out += "♣";
	if (marks & favoriteMarks::SPADE)	out += "♠";

	return out;
}

std::string StringUtils::format(const char *fmt_str, ...) {
	va_list ap;
	char *fp = nullptr;
	va_start(ap, fmt_str);
	vasprintf(&fp, fmt_str, ap);
	va_end(ap);

	std::unique_ptr<char, decltype(free) *> formatted(fp, free);
	return std::string(formatted.get());
}