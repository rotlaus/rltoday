#ifndef __TIMEZONE_H__
#define __TIMEZONE_H__
#include "helper.h"
class CTimeZone{
	public:
		CTimeZone();
		virtual ~CTimeZone();
		void SetName(const TSTRING name){m_sName = name;};
		const TSTRING GetName(){return m_sName;};
		const SYSTEMTIME GetTime();
		const int GetTimeZone(){return m_nTimeZone;};
		void SetTimeZone(const int timezone){m_nTimeZone = timezone;};
	protected:
		//The Name of the Timezone;
		TSTRING	m_sName;
		//Time Offset in seconds
		int m_nTimeZone;
};
#endif //__TIMEZONE_H__

