/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */


#ifndef APP_H_
#define APP_H_
//#include "ArchiveBuilder.h"
#include "AppBase.h"

#define APP_NAME "Idxit"


using namespace CommandLineProcessing;



/**
* @brief This is the main application class. This class is the first object main() will execute.
*/
class IdxitApp : public AppBase {
private:
	std::string m_HomePath;

public:
	/// Constructor
	IdxitApp(const char* appName);
	/// Destructor
	virtual ~IdxitApp();
	/// @brief This is the main application run function. All application processing starts here.
	
	bool about(const char* outputType, const char* filename);
protected:
	bool initaliseConfig();
	bool initaliseArgs(int argc, char **argv);
	bool doRun();

	bool setExitStatus(bool commandReturn);
};


#endif /* APP_H_ */
