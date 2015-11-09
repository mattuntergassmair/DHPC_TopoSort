#include "Timer.hpp"

void Timer::start() {
	startTime = clock();
}

void Timer::stop() {
	stopTime = clock();
}

double Timer::getElapsed() {
	return double(stopTime - startTime) / CLOCKS_PER_SEC;
}

void Timer::printElapsed() {
	cout << "Time elapsed: " << getElapsed() << endl;
}
