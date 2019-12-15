#include "pch.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
using namespace std;

const int threadsAmount = 4;
const int arraySize = 1000000;


class MutexArray {
private:
	
	int currentIndex = 0;
	thread threads[threadsAmount];
	mutex indexLock;

public:

	int* initArray() {
		int* array = new int[arraySize];
		for (int i = 0; i < arraySize; i++) {
			array[i] = 0;
		}
		return array;
	}

	void incrementArray(int id, int* array) {
		int i = currentIndex;
		while (true) {
			indexLock.lock();
			if (currentIndex >= arraySize)
				break;
			i = currentIndex;
			currentIndex++;
			indexLock.unlock();
			array[i]++;
		}
	}

	void run() {
		
		int* array = initArray();

		for (int i = 0; i < threadsAmount; i++) {
			threads[i] = thread(&MutexArray::incrementArray, this, i, array);
		}

		for (int i = 0; i < threadsAmount; i++) {
			threads[i].join();
		}

		bool normal = true;
		for (int i = 0; i < arraySize; i++) {
			if (array[i] != 1)
				normal = false;
		}

		if (normal) cout << "ok";
	}
};

class AtomicArray {
private:
	atomic<int> index = 0;
	thread threads[threadsAmount];

public:

	int* initArray() {
		int* array = new int[arraySize];
		for (int i = 0; i < arraySize; i++) {
			array[i] = 0;
		}
		return array;
	}

	void incrementArray(int id, int* array) {
		int i = index.load();
		while (i < arraySize) {
			 i = index.fetch_add(1);
			 if (i < arraySize) {
				 array[i]++;
			 }
		}
	}

	void run() {

		int* array = initArray();

		for (int i = 0; i < threadsAmount; i++) {
			threads[i] = thread(&AtomicArray::incrementArray, this, i, array);
		}

		for (int i = 0; i < threadsAmount; i++) {
			threads[i].join();
		}

		this_thread::sleep_for(chrono::milliseconds(1));
		bool normal = true;
		for (int i = 0; i < arraySize; i++) {
			//cout << array[i];
			if (array[i] != 1)
				normal = false;
		}

		if (normal) cout << "ok";
	}
};


int main()
{
	AtomicArray mut;
	mut.run();
}