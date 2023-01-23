#include <3ds.h>
#include <queueSystem.h>

QueueSystem::QueueSystem() {
	this->head = 0;
	this->tail = 0;
	this->maxSize = 0;
	this->currentSize = 0;
	this->data = NULL;
}

QueueSystem::QueueSystem(int maxSize) {
	this->head = 0;
	this->tail = 0;
	this->maxSize = maxSize;
	this->currentSize = 0;
	this->data = new int[maxSize];
}

QueueSystem::~QueueSystem() {
	if (this->data != NULL) {
		delete [] this->data;
	}
}

bool QueueSystem::isEmpty() {
	return (this->head == this->tail);
}

bool QueueSystem::isFull() {
	return (this->currentSize == this->maxSize);
}

int QueueSystem::enqueue(int value) {
	if (this->isFull()) {
		return -1;
	}

	this->data[this->tail] = value;
	this->tail = (this->tail + 1) % this->maxSize;
	this->currentSize++;
	return 0;
}

int QueueSystem::dequeue() {
	int value;

	if (this->isEmpty()) {
		return -1;
	}

	value = this->data[this->head];
	this->head = (this->head + 1) % this->maxSize;
	this->currentSize--;
	return value;
}
