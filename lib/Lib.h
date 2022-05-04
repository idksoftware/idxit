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

#ifndef SIALIB_H_
#define SIALIB_H_

#include <string>
#include <memory>

#include "ExifDate.h"
//#include "ResultsPresentation.h"
/*
*/
namespace simplearchive {


	class ArchiveBuilder;
	class ArchiveObject;
	
	class CompletedSummary {
		std::string m_summary;
		std::string m_result;
	public:
		CompletedSummary() = default;
		virtual ~CompletedSummary() = default;
		void setSummary(const char *s) { m_summary = s; };
		const char *getSummary() { return m_summary.c_str(); };
		void setResult(const char *s) { m_result = s; };
		const char *getResult() { return m_result.c_str(); };
	};

	class IDXLib
	{
	public:

		

	private:
		std::string m_HomePath;
		std::shared_ptr<ArchiveBuilder> m_ArchiveBuilder;
		
		bool m_winsockRequired;
		bool m_socklibStarted;
		bool m_enableEvents;
		int m_udpPortNum;
		int m_tcpPortNum;
		bool m_enableServer;
		std::string m_udpAddress;
	public:
		IDXLib();
		~IDXLib();
		void enableEvents(const char *address, int port);
		void enableServer(int port);
		int initalise(const char* appName);
		void start();
		int complete();
		//int complete();
		

		
		static const int getLastCode();
		
		static const char *getLastMessage();
		
		
	
	};

}
#endif /* SIALIB_H_ */
