// OleDateTime.h: interface for the COleDateTime class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __OLEDATETIME_H_
#define __OLEDATETIME_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>

const int maxTimeBufferSize = 128;
const long maxDaysInSpan  =	3615897L;

extern __declspec(selectany) const TCHAR * const szInvalidDateTime = _T("无效的日期时间");
extern __declspec(selectany) const TCHAR * const szInvalidDateTimeSpan = _T("无效的日期时间范围");

typedef double DATE;

class COleDateTimeSpan
{
// Constructors
public:
	COleDateTimeSpan() throw();

	COleDateTimeSpan(double dblSpanSrc) throw();
	COleDateTimeSpan(LONG lDays, int nHours, int nMins, int nSecs) throw();

// Attributes
	enum DateTimeSpanStatus
	{
		valid = 0,
		invalid = 1,    // Invalid span (out of range, etc.)
		null = 2,       // Literally has no value
	};

	double m_span;
	DateTimeSpanStatus m_status;

	void SetStatus(DateTimeSpanStatus status) throw();
	DateTimeSpanStatus GetStatus() const throw();

	double GetTotalDays() const throw();    // span in days (about -3.65e6 to 3.65e6)
	double GetTotalHours() const throw();   // span in hours (about -8.77e7 to 8.77e6)
	double GetTotalMinutes() const throw(); // span in minutes (about -5.26e9 to 5.26e9)
	double GetTotalSeconds() const throw(); // span in seconds (about -3.16e11 to 3.16e11)

	LONG GetDays() const throw();       // component days in span
	LONG GetHours() const throw();      // component hours in span (-23 to 23)
	LONG GetMinutes() const throw();    // component minutes in span (-59 to 59)
	LONG GetSeconds() const throw();    // component seconds in span (-59 to 59)

// Operations
	COleDateTimeSpan& operator=(double dblSpanSrc) throw();

	bool operator==(const COleDateTimeSpan& dateSpan) const throw();
	bool operator!=(const COleDateTimeSpan& dateSpan) const throw();
	bool operator<(const COleDateTimeSpan& dateSpan) const throw();
	bool operator>(const COleDateTimeSpan& dateSpan) const throw();
	bool operator<=(const COleDateTimeSpan& dateSpan) const throw();
	bool operator>=(const COleDateTimeSpan& dateSpan) const throw();

	// DateTimeSpan math
	COleDateTimeSpan operator+(const COleDateTimeSpan& dateSpan) const throw();
	COleDateTimeSpan operator-(const COleDateTimeSpan& dateSpan) const throw();
	COleDateTimeSpan& operator+=(const COleDateTimeSpan dateSpan) throw();
	COleDateTimeSpan& operator-=(const COleDateTimeSpan dateSpan) throw();
	COleDateTimeSpan operator-() const throw();

	operator double() const throw();

	void SetDateTimeSpan(LONG lDays, int nHours, int nMins, int nSecs) throw();

#ifndef _ATL_MIN_CRT
	// formatting
	CString Format(LPCTSTR pFormat) const;
	CString Format(UINT nID) const;
#endif

// Implementation
	void CheckRange();

private:
	static const double OLE_DATETIME_HALFSECOND;
};

class COleDateTime
{
// Constructors
public:
	static COleDateTime WINAPI GetCurrentTime() throw();

	COleDateTime() throw();

	COleDateTime(const VARIANT& varSrc) throw();
	COleDateTime(DATE dtSrc) throw();

	COleDateTime(time_t timeSrc) throw();

	COleDateTime(const SYSTEMTIME& systimeSrc) throw();
	COleDateTime(const FILETIME& filetimeSrc) throw();

	COleDateTime(int nYear, int nMonth, int nDay,
		int nHour, int nMin, int nSec) throw();
	COleDateTime(WORD wDosDate, WORD wDosTime) throw();

#ifdef __oledb_h__
	COleDateTime( const DBTIMESTAMP& dbts) throw();
	bool GetAsDBTIMESTAMP( DBTIMESTAMP& dbts ) const throw();
#endif

// Attributes
	enum DateTimeStatus
	{
		error = -1,
		valid = 0,
		invalid = 1,    // Invalid date (out of range, etc.)
		null = 2,       // Literally has no value
	};

	DATE m_dt;
	DateTimeStatus m_status;

	void SetStatus(DateTimeStatus status) throw();
	DateTimeStatus GetStatus() const throw();

	bool GetAsSystemTime(SYSTEMTIME& sysTime) const throw();
	bool GetAsUDATE( UDATE& udate ) const throw();

	int GetYear() const throw();
	// Month of year (1 = January)
	int GetMonth() const throw();
	// Day of month (1-31)
	int GetDay() const throw();
	// Hour in day (0-23)
	int GetHour() const throw();
	// Minute in hour (0-59)
	int GetMinute() const throw();
	// Second in minute (0-59)
	int GetSecond() const throw();
	// Day of week (1 = Sunday, 2 = Monday, ..., 7 = Saturday)
	int GetDayOfWeek() const throw();
	// Days since start of year (1 = January 1)
	int GetDayOfYear() const throw();

// Operations
	COleDateTime& operator=(const VARIANT& varSrc) throw();
	COleDateTime& operator=(DATE dtSrc) throw();
	COleDateTime& operator=(const time_t& timeSrc) throw();
	COleDateTime& operator=(const SYSTEMTIME& systimeSrc) throw();
	COleDateTime& operator=(const FILETIME& filetimeSrc) throw();
	COleDateTime& operator=(const UDATE& udate) throw();

	bool operator==(const COleDateTime& date) const throw();
	bool operator!=(const COleDateTime& date) const throw();
	bool operator<(const COleDateTime& date) const throw();
	bool operator>(const COleDateTime& date) const throw();
	bool operator<=(const COleDateTime& date) const throw();
	bool operator>=(const COleDateTime& date) const throw();

	// DateTime math
	COleDateTime operator+(COleDateTimeSpan dateSpan) const throw();
	COleDateTime operator-(COleDateTimeSpan dateSpan) const throw();
	COleDateTime& operator+=(COleDateTimeSpan dateSpan) throw();
	COleDateTime& operator-=(COleDateTimeSpan dateSpan) throw();

	// DateTimeSpan math
	COleDateTimeSpan operator-(const COleDateTime& date) const throw();

	operator DATE() const throw();

	int SetDateTime(int nYear, int nMonth, int nDay,
		int nHour, int nMin, int nSec) throw();
	int SetDate(int nYear, int nMonth, int nDay) throw();
	int SetTime(int nHour, int nMin, int nSec) throw();
	bool ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags = 0,
		LCID lcid = LANG_USER_DEFAULT) throw();

#ifndef _ATL_MIN_CRT
	// formatting
	CString Format(DWORD dwFlags = 0, LCID lcid = LANG_USER_DEFAULT) const;
	CString Format(LPCTSTR lpszFormat) const;
	CString Format(UINT nFormatID) const;
#endif

protected:
	static double WINAPI DoubleFromDate( DATE date ) throw();
	static DATE WINAPI DateFromDouble( double f ) throw();

	void CheckRange();
};

#endif // __OLEDATETIME_H_
