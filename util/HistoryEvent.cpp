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

#include <string>
#include "HistoryEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif



HistoryEvent::HistoryEvent() {
	m_event = Event::ERROR;
}

HistoryEvent::~HistoryEvent() {
	// TODO Auto-generated destructor stub
}

const char* HistoryEvent::getString() const {
	return getString(m_event);
}

const char *HistoryEvent::getString(HistoryEvent::Event evt) {
	switch (evt) {
	case Event::ADDED:
		return "added";
	case Event::CHECKOUT:
		return "chked-out";
	case Event::CHECKIN:
		return "chked-in";
	case Event::EXPORT:
		return "exported";
	case Event::UNCHECKOUT:
		return "unchked-out";
	case Event::ERROR:
		return "error";
	}
	return "error";
}

void HistoryEvent::set(const char *he) {
	std::string eventstr = he;
	if (eventstr.compare("added") == 0) {
		m_event = Event::ADDED;
	} else if(eventstr.compare("chked-out") == 0) {
		m_event = Event::CHECKOUT;
	} else if(eventstr.compare("chked-in") == 0) {
		m_event = Event::CHECKIN;
	} else if(eventstr.compare("exported") == 0) {
		m_event = Event::EXPORT;
	}
	m_event = Event::ERROR;

}

const char HistoryEvent::getChar(HistoryEvent::Event evt) {
	switch (evt) {
	case Event::ADDED:
		return 'A';
	case Event::CHECKOUT:
		return 'O';
	case Event::CHECKIN:
		return 'I';
	case Event::EXPORT:
		return 'X';
	case Event::UNCHECKOUT:
		return 'U';
	case Event::ERROR:
		return 'E';
	}
	return 'E';
}

