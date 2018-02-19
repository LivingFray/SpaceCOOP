#pragma once

//All claims that this is just pasted from stackoverflow are clearly false

#include <queue>
#include <mutex>
#include <condition_variable>

// A threadsafe-queue.
template <class T>
class TSQueue {
public:
	TSQueue(void)
		: q()
		, m()
		, c() {
	}

	~TSQueue(void) {
	}

	// Add an element to the queue.
	void push(T t) {
		std::lock_guard<std::mutex> lock(m);
		q.push(t);
		c.notify_one();
	}

	// Get the "front"-element.
	// If the queue is empty, wait till a element is avaiable.
	T poll(void) {
		std::unique_lock<std::mutex> lock(m);
		while (q.empty()) {
			// release lock as long as the wait and reaquire it afterwards.
			c.wait(lock);
		}
		T val = q.front();
		q.pop();
		return val;
	}

private:
	std::queue<T> q;
	mutable std::mutex m;
	std::condition_variable c;
};
