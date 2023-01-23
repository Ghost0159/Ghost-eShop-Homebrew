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

#include "utils/queueSystem.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_net.h>

#include <cstdio>
#include <cstdint>

// Structure utilisée pour stocker des informations sur le téléchargement
struct QueueSystemDownloadInfo {
	TCPsocket socket;
	uint64_t start;
	uint64_t end;
	uint64_t size;
	uint64_t progress;
	uint64_t chunkSize;
	char * buffer;
	uint64_t bufferSize;
	uint64_t bufferOffset;
	bool error;
	bool complete;
	bool running;
};

// Fonction utilisée pour télécharger le fichier
int QueueSystem::downloadThread(void * data) {
	QueueSystemDownloadInfo * info = (QueueSystemDownloadInfo*)data;

	uint64_t totalBytesRead = 0;

	// Boucle de téléchargement
	while(info->progress < info->size && !info->error && !info->complete) {
		// On calcule la taille du chunk à télécharger
		uint64_t chunkSize = std::min(info->chunkSize, info->size - info->progress);

		// On lit le chunk
		int bytesRead = SDLNet_TCP_Recv(info->socket, info->buffer + info->bufferOffset, (int)chunkSize);

		// On vérifie si il y a eu une erreur
		if(bytesRead == -1) {
			info->error = true;
			break;
		}

		// On incrémente le nombre de bytes lus
		totalBytesRead += bytesRead;
		info->progress += bytesRead;

		// On vérifie si le buffer est plein
		if(info->bufferOffset + bytesRead == info->bufferSize) {
			// On appelle la fonction de callback
			info->bufferOffset = 0;
			if(!info->callback(info->buffer, info->bufferSize, info->progress, info->size)) {
				// La fonction de callback a retourné false, on arrête le téléchargement
				break;
			}
		} else {
			info->bufferOffset += bytesRead;
		}
	}

	info->running = false;
	info->complete = true;

	if(!info->error && info->progress == info->size) {
		// On appelle la fonction de callback avec le dernier buffer
		info->callback(info->buffer, info->bufferOffset, info->progress, info->size);
	}

	// On libère le buffer
	delete[] info->buffer;

	// On ferme la socket
	SDLNet_TCP_Close(info->socket);

	return 0;
}

QueueSystem::QueueSystem() : m_threads(nullptr), m_numThreads(0) {

}

QueueSystem::~QueueSystem() {
	// On arrête tous les threads
	for(int i = 0; i < m_numThreads; i++) {
		SDL_WaitThread(m_threads[i], nullptr);
	}

	// On libère le tableau de threads
	delete[] m_threads;
}

bool QueueSystem::init(int numThreads) {
	// On vérifie qu'il n'y a pas déjà des threads
	if(m_threads != nullptr) {
		return false;
	}

	// On crée le tableau de threads
	m_numThreads = numThreads;
	m_threads = new SDL_Thread*[m_numThreads];

	// On initialise les threads
	for(int i = 0; i < m_numThreads; i++) {
		m_threads[i] = nullptr;
	}

	return true;
}

bool QueueSystem::startDownload(TCPsocket socket, uint64_t start, uint64_t end, uint64_t size, void (*callback)(const char * buffer, uint64_t bufferSize, uint64_t progress, uint64_t total), uint64_t chunkSize) {
	// On vérifie que le socket est valide
	if(socket == nullptr) {
		return false;
	}

	// On vérifie qu'il y a encore des threads disponibles
	int threadIndex = -1;
	for(int i = 0; i < m_numThreads; i++) {
		if(m_threads[i] == nullptr) {
			threadIndex = i;
			break;
		}
	}

	if(threadIndex == -1) {
		// Il n'y a plus de threads disponibles
		return false;
	}

	// On crée le buffer
	char * buffer = new char[chunkSize];

	// On crée la structure contenant les informations sur le téléchargement
	QueueSystemDownloadInfo * info = new QueueSystemDownloadInfo;
	info->socket = socket;
	info->start = start;
	info->end = end;
	info->size = size;
	info->progress = 0;
	info->chunkSize = chunkSize;
	info->buffer = buffer;
	info->bufferSize = chunkSize;
	info->bufferOffset = 0;
	info->error = false;
	info->complete = false;
	info->running = true;
	info->callback = callback;

	// On lance le thread
	m_threads[threadIndex] = SDL_CreateThread(QueueSystem::downloadThread, "DownloadThread", info);

	return true;
}

bool QueueSystem::isComplete(int threadIndex) {
	if(threadIndex < 0 || threadIndex >= m_numThreads) {
		return false;
	}

	if(m_threads[threadIndex] == nullptr) {
		return true;
	}

	// On récupère la structure contenant les informations sur le téléchargement
	QueueSystemDownloadInfo * info = (QueueSystemDownloadInfo*)SDL_GetThreadID(m_threads[threadIndex]);

	// On vérifie si le téléchargement est terminé
	return info->complete;
}

bool QueueSystem::hasError(int threadIndex) {
	if(threadIndex < 0 || threadIndex >= m_numThreads) {
		return false;
	}

	if(m_threads[threadIndex] == nullptr) {
		return false;
	}

	// On récupère la structure contenant les informations sur le téléchargement
	QueueSystemDownloadInfo * info = (QueueSystemDownloadInfo*)SDL_GetThreadID(m_threads[threadIndex]);

	// On vérifie si le téléchargement a eu une erreur
	return info->error;
}

bool QueueSystem::isRunning(int threadIndex) {
	if(threadIndex < 0 || threadIndex >= m_numThreads) {
		return false;
	}

	if(m_threads[threadIndex] == nullptr) {
		return false;
	}

	// On récupère la structure contenant les informations sur le téléchargement
	QueueSystemDownloadInfo * info = (QueueSystemDownloadInfo*)SDL_GetThreadID(m_threads[threadIndex]);

	// On vérifie si le téléchargement est toujours en cours
	return info->running;
}

uint64_t QueueSystem::getProgress(int threadIndex) {
	if(threadIndex < 0 || threadIndex >= m_numThreads) {
		return 0;
	}

	if(m_threads[threadIndex] == nullptr) {
		return 0;
	}

	// On récupère la structure contenant les informations sur le téléchargement
	QueueSystemDownloadInfo * info = (QueueSystemDownloadInfo*)SDL_GetThreadID(m_threads[threadIndex]);

	// On renvoie la progression du téléchargement
	return info->progress;
}

uint64_t QueueSystem::getStart(int threadIndex) {
	if(threadIndex < 0 || threadIndex >= m_numThreads) {
		return 0;
	}

	if(m_threads[threadIndex] == nullptr) {
		return 0;
	}

	// On récupère la structure contenant les informations sur le téléchargement
	QueueSystemDownloadInfo * info = (QueueSystemDownloadInfo*)SDL_GetThreadID(m_threads[threadIndex]);

	// On renvoie le début du téléchargement
	return info->start;
}

uint64_t QueueSystem::getEnd(int threadIndex) {
	if(threadIndex < 0 || threadIndex >= m_numThreads) {
		return 0;
	}

	if(m_threads[threadIndex] == nullptr) {
		return 0;
	}

	// On récupère la structure contenant les informations sur le téléchargement
	QueueSystemDownloadInfo * info = (QueueSystemDownloadInfo*)SDL_GetThreadID(m_threads[threadIndex]);

	// On renvoie la fin du téléchargement
	return info->end;
}

uint64_t QueueSystem::getSize(int threadIndex) {
	if(threadIndex < 0 || threadIndex >= m_numThreads) {
		return 0;
	}

	if(m_threads[threadIndex] == nullptr) {
		return 0;
	}

	// On récupère la structure contenant les informations sur le téléchargement
	QueueSystemDownloadInfo * info = (QueueSystemDownloadInfo*)SDL_GetThreadID(m_threads[threadIndex]);

	// On renvoie la taille du téléchargement
	return info->size;
}

void QueueSystem::waitUntilComplete(int threadIndex) {
	if(threadIndex < 0 || threadIndex >= m_numThreads) {
		return;
	}

	if(m_threads[threadIndex] == nullptr) {
		return;
	}

	// On attend le thread
	int status;
	SDL_WaitThread(m_threads[threadIndex], &status);
	m_threads[threadIndex] = nullptr;
}
