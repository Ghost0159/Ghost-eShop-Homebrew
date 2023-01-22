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

#ifndef _GHOST_ESHOP_QUEUE_SYSTEM_HPP
#define _GHOST_ESHOP_QUEUE_SYSTEM_HPP

#include "json.hpp"
#include <citro2d.h>
#include <deque>
#include <memory>

/* Extend this, if more statuses are neccessary. */
enum class QueueStatus {
	None,
	Copying,
	Deleting,
	Downloading,
	Extracting,
	Installing,
	Moving,
	Request, // For User needed Requests.
	Failed,
	Done
};

enum RequestType {
	PROMPT_RET = -3,
	NO_REQUEST = -1,
	RMDIR_REQUEST = 1, // remove dir prompt request.
	PROMPT_REQUEST = 2, // skip prompt request.
	PROMPT_ERROR = 3 // Error message prompt. Unused right now.
};

/* Of course also a namespace to that part, so we can do that in a Thread. */
namespace QueueSystem {
	extern int RequestNeeded, RequestAnswer;
	extern std::string RequestMsg, EndMsg;
	extern int LastElement;
	extern bool Wait, Popup, CancelCallback;

	void QueueHandle(); // Handles the Queue.
	void AddToQueue(nlohmann::json obj, const C2D_Image &icn, const std::string &name, const std::string &uName, const std::string &eName, const std::string &lUpdated); // Adds to Queue.
	void ClearQueue(); // Clears the Queue.
	void Resume();
};

class Queue {
public:
	Queue(nlohmann::json object, const C2D_Image &img, const std::string &name, const std::string &uName, const std::string &eName, const std::string &lUpdated) :
		obj(object), icn(img), total(object.size()), current(QueueSystem::LastElement), name(name), eshopName(uName), entryName(eName), lastUpdated(lUpdated) { };

	QueueStatus status = QueueStatus::None;
	nlohmann::json obj;
	C2D_Image icn;
	int total, current;
	std::string name = "", eshopName = "", entryName = "", lastUpdated = "";
};

#endif