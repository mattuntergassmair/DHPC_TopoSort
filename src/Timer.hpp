#ifndef TIMER_HPP
#define TIMER_HPP

using namespace std;

class Timer {

	public:

		void start() {
			startTime = clock();
		}
		
		void stop() {
			stopTime = clock();
		}
		
		double getElapsed() {
			return double(startTime - stopTime) / CLOCKS_PER_SEC;
		}
		
		void printElapsed() {
			cout << "Time elapsed: " << getElapsed() << endl;
		}

	private:
		clock_t startTime;
		clock_t stopTime;

};

#endif // TIMER_HPP
