#ifndef MUTEX_HPP
#define MUTEX_HPP

namespace Diggler {

class Mutex {
private:
	void *data;

public:
	Mutex();
	~Mutex();
	Mutex& operator=(Mutex &m) = delete;
	
	void lock();
	bool tryLock();
	void unlock();
};

}

#endif