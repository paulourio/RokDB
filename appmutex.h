/*
 * appmutex.h
 *
 *  Created on: 26/11/2011
 */

#ifndef APPMUTEX_H_
#define APPMUTEX_H_

namespace rokdb {

/**
 * Application Mutex
 * Allow only one instance of the program.
 */
class AppMutex {
private:
	std::string lock_file;
	int lock_id;
	void RemoveLock();
	void ShowBlockMessage();
public:
	AppMutex();
	~AppMutex();
	bool CreateMutex(std::string);
	bool is_locked() const;
};

}


#endif /* APPMUTEX_H_ */
