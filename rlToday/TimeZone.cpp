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

#include "TimeZone.h"

CTimeZone::CTimeZone(){
}

CTimeZone::~CTimeZone(){
}

const SYSTEMTIME CTimeZone::GetTime(){
	const __int64 nano100SecInDay=(__int64)10000000*60*60*24; 
	const __int64 nano100SecInHour=(__int64)10000000*60*60; 
	double hours;
	SYSTEMTIME st;
	FILETIME ft;
	__int64 *pi;
	switch(m_nTimeZone){
		case 0:	hours = -12; break;
		case 1:	hours = -11; break;
		case 2:	hours = -10; break;
		case 3:	hours = -9; break;
		case 4:	hours = -8; break;
		case 5: hours = -7; break;
		case 6: hours = -7; break;
		case 7: hours = -6; break;
		case 8: hours = -6; break;
		case 9: hours = -6; break;
		case 10: hours = -6; break;
		case 11: hours = -5; break;
		case 12: hours = -5; break;
		case 13: hours = -5; break;
		case 14: hours = -4; break;
		case 15: hours = -4; break;
		case 16: hours = -4; break;
		case 17: hours = -3.5; break;
		case 18: hours = -3; break;
		case 19: hours = -3; break;
		case 20: hours = -3; break;
		case 21: hours = -2; break;
		case 22: hours = -1; break;
		case 23: hours = -1; break;
		case 24: hours = 0; break;
		case 25: hours = 0; break;
		case 26: hours = 1; break;
		case 27: hours = 1; break;
		case 28: hours = 1; break;
		case 29: hours = 1; break;
		case 30: hours = 1; break;
		case 31: hours = 2; break;
		case 32: hours = 2; break;
		case 33: hours = 2; break;
		case 34: hours = 2; break;
		case 35: hours = 2; break;
		case 36: hours = 2; break;
		case 37: hours = 3; break;
		case 38: hours = 3; break;
		case 39: hours = 3; break;
		case 40: hours = 3; break;
		case 41: hours = 3.5; break;
		case 42: hours = 4; break;
		case 43: hours = 4; break;
		case 44: hours = 4.5; break;
		case 45: hours = 5; break;
		case 46: hours = 5; break;
		case 47: hours = 5.5; break;
		case 48: hours = 5.75; break;
		case 49: hours = 6; break;
		case 50: hours = 6; break;
		case 51: hours = 6; break;
		case 52: hours = 6.5; break;
		case 53: hours = 7; break;
		case 54: hours = 7; break;
		case 55: hours = 8; break;
		case 56: hours = 8; break;
		case 57: hours = 8; break;
		case 58: hours = 8; break;
		case 59: hours = 8; break;
		case 60: hours = 9; break;
		case 61: hours = 9; break;
		case 62: hours = 9; break;
		case 63: hours = 9.5; break;
		case 64: hours = 9.5; break;
		case 65: hours = 10; break;
		case 66: hours = 10; break;
		case 67: hours = 10; break;
		case 68: hours = 10; break;
		case 69: hours = 10; break;
		case 70: hours = 11; break;
		case 71: hours = 12; break;
		case 72: hours = 12; break;
		case 73: hours = 13; break;
		default: hours = 0; break;
	}
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	pi = (__int64*) &ft;
	(*pi) += (__int64) ((double)hours * (double)nano100SecInHour); 
	FileTimeToSystemTime(&ft, &st);
	return st;
}


