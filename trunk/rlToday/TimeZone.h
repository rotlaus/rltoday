/*******************************************************************************
*   rlToday - multi purpose today plugin.                                      *
*                                                                              *
*   Copyright (C) 2005-2009 Andre Adrian aka Rotlaus                           *
*	(rotlaus@gmail.com)                                                        *
*                                                                              *
*   This program is free software; you can redistribute it and/or              *
*   modify it under the terms of the GNU General Public License version 2      *
*   as published by the Free Software Foundation.                              *
*                                                                              *
*   This program is distributed in the hope that it will be useful,            *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of             *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          *
*   General Public License version 2 for more details.                         *
*                                                                              *
*   You should have received a copy of the GNU General Public License          *
*   version 2 along with this library; if not, write to the Free Software      *
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  *
*******************************************************************************/

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

