// OleDateTime.cpp: implementation of the COleDateTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OleDateTime.h"

#include <math.h>

#define MAX_TIME_BUFFER_SIZE    128         // matches that in timecore.cpp
#define MIN_DATE                (-657434L)  // about year 100
#define MAX_DATE                2958465L    // about year 9999

// Half a second, expressed in days
#define HALF_SECOND  (1.0/172800.0)

// One-based array of days in year at month start
int _afxMonthDays[13] =
{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

BOOL _AfxOleDateFromTm(WORD wYear, WORD wMonth, WORD wDay,
										 WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest)
{
	// Validate year and month (ignore day of week and milliseconds)
	if (wYear > 9999 || wMonth < 1 || wMonth > 12)
		return FALSE;
	
	//  Check for leap year and set the number of days in the month
	BOOL bLeapYear = ((wYear & 3) == 0) &&
		((wYear % 100) != 0 || (wYear % 400) == 0);
	
	int nDaysInMonth =
		_afxMonthDays[wMonth] - _afxMonthDays[wMonth-1] +
		((bLeapYear && wDay == 29 && wMonth == 2) ? 1 : 0);
	
	// Finish validating the date
	if (wDay < 1 || wDay > nDaysInMonth ||
		wHour > 23 || wMinute > 59 ||
		wSecond > 59)
	{
		return FALSE;
	}
	
	// Cache the date in days and time in fractional days
	long nDate;
	double dblTime;
	
	//It is a valid date; make Jan 1, 1AD be 1
	nDate = wYear*365L + wYear/4 - wYear/100 + wYear/400 +
		_afxMonthDays[wMonth-1] + wDay;
	
	//  If leap year and it's before March, subtract 1:
	if (wMonth <= 2 && bLeapYear)
		--nDate;
	
	//  Offset so that 12/30/1899 is 0
	nDate -= 693959L;
	
	dblTime = (((long)wHour * 3600L) +  // hrs in seconds
		((long)wMinute * 60L) +  // mins in seconds
		((long)wSecond)) / 86400.;
	
	dtDest = (double) nDate + ((nDate >= 0) ? dblTime : -dblTime);
	
	return TRUE;
}

BOOL _AfxTmFromOleDate(DATE dtSrc, struct tm& tmDest)
{
	// The legal range does not actually span year 0 to 9999.
	if (dtSrc > MAX_DATE || dtSrc < MIN_DATE) // about year 100 to about 9999
		return FALSE;

	long nDays;             // Number of days since Dec. 30, 1899
	long nDaysAbsolute;     // Number of days since 1/1/0
	long nSecsInDay;        // Time in seconds since midnight
	long nMinutesInDay;     // Minutes in day

	long n400Years;         // Number of 400 year increments since 1/1/0
	long n400Century;       // Century within 400 year block (0,1,2 or 3)
	long n4Years;           // Number of 4 year increments since 1/1/0
	long n4Day;             // Day within 4 year block
							//  (0 is 1/1/yr1, 1460 is 12/31/yr4)
	long n4Yr;              // Year within 4 year block (0,1,2 or 3)
	BOOL bLeap4 = TRUE;     // TRUE if 4 year block includes leap year

	double dblDate = dtSrc; // tempory serial date

	// If a valid date, then this conversion should not overflow
	nDays = (long)dblDate;

	// Round to the second
	dblDate += ((dtSrc > 0.0) ? HALF_SECOND : -HALF_SECOND);

	nDaysAbsolute = (long)dblDate + 693959L; // Add days from 1/1/0 to 12/30/1899

	dblDate = fabs(dblDate);
	nSecsInDay = (long)((dblDate - floor(dblDate)) * 86400.);

	// Calculate the day of week (sun=1, mon=2...)
	//   -1 because 1/1/0 is Sat.  +1 because we want 1-based
	tmDest.tm_wday = (int)((nDaysAbsolute - 1) % 7L) + 1;

	// Leap years every 4 yrs except centuries not multiples of 400.
	n400Years = (long)(nDaysAbsolute / 146097L);

	// Set nDaysAbsolute to day within 400-year block
	nDaysAbsolute %= 146097L;

	// -1 because first century has extra day
	n400Century = (long)((nDaysAbsolute - 1) / 36524L);

	// Non-leap century
	if (n400Century != 0)
	{
		// Set nDaysAbsolute to day within century
		nDaysAbsolute = (nDaysAbsolute - 1) % 36524L;

		// +1 because 1st 4 year increment has 1460 days
		n4Years = (long)((nDaysAbsolute + 1) / 1461L);

		if (n4Years != 0)
			n4Day = (long)((nDaysAbsolute + 1) % 1461L);
		else
		{
			bLeap4 = FALSE;
			n4Day = (long)nDaysAbsolute;
		}
	}
	else
	{
		// Leap century - not special case!
		n4Years = (long)(nDaysAbsolute / 1461L);
		n4Day = (long)(nDaysAbsolute % 1461L);
	}

	if (bLeap4)
	{
		// -1 because first year has 366 days
		n4Yr = (n4Day - 1) / 365;

		if (n4Yr != 0)
			n4Day = (n4Day - 1) % 365;
	}
	else
	{
		n4Yr = n4Day / 365;
		n4Day %= 365;
	}

	// n4Day is now 0-based day of year. Save 1-based day of year, year number
	tmDest.tm_yday = (int)n4Day + 1;
	tmDest.tm_year = n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr;

	// Handle leap year: before, on, and after Feb. 29.
	if (n4Yr == 0 && bLeap4)
	{
		// Leap Year
		if (n4Day == 59)
		{
			/* Feb. 29 */
			tmDest.tm_mon = 2;
			tmDest.tm_mday = 29;
			goto DoTime;
		}

		// Pretend it's not a leap year for month/day comp.
		if (n4Day >= 60)
			--n4Day;
	}

	// Make n4DaY a 1-based day of non-leap year and compute
	//  month/day for everything but Feb. 29.
	++n4Day;

	// Month number always >= n/32, so save some loop time */
	for (tmDest.tm_mon = (n4Day >> 5) + 1;
		n4Day > _afxMonthDays[tmDest.tm_mon]; tmDest.tm_mon++);

	tmDest.tm_mday = (int)(n4Day - _afxMonthDays[tmDest.tm_mon-1]);

DoTime:
	if (nSecsInDay == 0)
		tmDest.tm_hour = tmDest.tm_min = tmDest.tm_sec = 0;
	else
	{
		tmDest.tm_sec = (int)nSecsInDay % 60L;
		nMinutesInDay = nSecsInDay / 60L;
		tmDest.tm_min = (int)nMinutesInDay % 60;
		tmDest.tm_hour = (int)nMinutesInDay / 60;
	}

	return TRUE;
}

#define MAX_TIME_BUFFER_SIZE    128         // matches that in timecore.cpp

void _AfxTmConvertToStandardFormat(struct tm& tmSrc)
{
	// Convert afx internal tm to format expected by runtimes (_tcsftime, etc)
	tmSrc.tm_year -= 1900;  // year is based on 1900
	tmSrc.tm_mon -= 1;      // month of year is 0-based
	tmSrc.tm_wday -= 1;     // day of week is 0-based
	tmSrc.tm_yday -= 1;     // day of year is 0-based
}

#ifndef _ATL_MIN_CRT
CString COleDateTimeSpan::Format(LPCTSTR pFormat) const
{
	CString strSpan;
	struct tm tmTemp;
	
	// If null, return empty string
	if (GetStatus() == null)
		return strSpan;
	
	// If invalid, return DateTimeSpan resource string
	if (GetStatus() == invalid || !_AfxTmFromOleDate(m_span, tmTemp))
		return szInvalidDateTimeSpan;
	
	// Convert tm from afx internal format to standard format
	_AfxTmConvertToStandardFormat(tmTemp);
	
	// _tcsftime() doesn't handle %D, so do it here
	
	CString strPreParsed;
	LPCTSTR pstrSource = pFormat;
	int nTargetChar = 0;
	int nAccumulatedLength = lstrlen(pFormat);
	LPTSTR pstrTarget = strPreParsed.GetBuffer(nAccumulatedLength);
	
	while (*pstrSource)
	{
		if (*pstrSource == '%' && pstrSource[1] == 'D')
		{
			TCHAR szDay[12];
			_itot(GetDays(), szDay, 10);
			strPreParsed.ReleaseBuffer(nTargetChar);
			strPreParsed += szDay;
			int nTemp = lstrlen(szDay);
			nAccumulatedLength += nTemp;
			nTargetChar += nTemp;
			pstrTarget = strPreParsed.GetBuffer(nAccumulatedLength)
				+ nTargetChar;
			pstrSource = _tcsinc(pstrSource);
			pstrSource = _tcsinc(pstrSource);
		}
		*pstrTarget = *pstrSource;
		nTargetChar++;
		pstrSource = _tcsinc(pstrSource);
		pstrTarget = _tcsinc(pstrTarget);
	}
	strPreParsed.ReleaseBuffer(nTargetChar);
	
	// Fill in the buffer, disregard return value as it's not necessary
	LPTSTR lpszTemp = strSpan.GetBufferSetLength(MAX_TIME_BUFFER_SIZE);
	_tcsftime(lpszTemp, strSpan.GetLength(), (LPCTSTR) strPreParsed, &tmTemp);
	strSpan.ReleaseBuffer();
	
	return strSpan;
}

/////////////////////////////////////////////////////////////////////////////
// COleDateTimeSpan
/////////////////////////////////////////////////////////////////////////////

COleDateTimeSpan::COleDateTimeSpan() throw() : m_span(0), m_status(valid)
{
}

COleDateTimeSpan::COleDateTimeSpan(double dblSpanSrc) throw() : m_span(dblSpanSrc), m_status(valid)
{
	CheckRange();
}

COleDateTimeSpan::COleDateTimeSpan(LONG lDays, int nHours, int nMins, int nSecs) throw()
{
	SetDateTimeSpan(lDays, nHours, nMins, nSecs);
}

void COleDateTimeSpan::SetStatus(DateTimeSpanStatus status) throw()
{
	m_status = status;
}

COleDateTimeSpan::DateTimeSpanStatus COleDateTimeSpan::GetStatus() const throw()
{
	return m_status;
}

__declspec(selectany) const double
	COleDateTimeSpan::OLE_DATETIME_HALFSECOND =
	1.0 / (2.0 * (60.0 * 60.0 * 24.0));

double COleDateTimeSpan::GetTotalDays() const throw()
{
	ATLASSERT(GetStatus() == valid);

	return LONG(m_span + (m_span < 0 ?
		-OLE_DATETIME_HALFSECOND : OLE_DATETIME_HALFSECOND));
}

double COleDateTimeSpan::GetTotalHours() const throw()
{
	ATLASSERT(GetStatus() == valid);

	return LONG((m_span + (m_span < 0 ? 
		-OLE_DATETIME_HALFSECOND : OLE_DATETIME_HALFSECOND)) * 24);
}

double COleDateTimeSpan::GetTotalMinutes() const throw()
{
	ATLASSERT(GetStatus() == valid);

	return LONG((m_span + (m_span < 0 ?
		-OLE_DATETIME_HALFSECOND : OLE_DATETIME_HALFSECOND)) * (24 * 60));
}

double COleDateTimeSpan::GetTotalSeconds() const throw()
{
	ATLASSERT(GetStatus() == valid);

	return LONG((m_span + (m_span < 0 ?
		-OLE_DATETIME_HALFSECOND : OLE_DATETIME_HALFSECOND)) * (24 * 60 * 60));
}

LONG COleDateTimeSpan::GetDays() const throw()
{
	ATLASSERT(GetStatus() == valid);
	return LONG(m_span);
}

LONG COleDateTimeSpan::GetHours() const throw()
{
	return LONG(GetTotalHours()) % 24;
}

LONG COleDateTimeSpan::GetMinutes() const throw()
{
	return LONG(GetTotalMinutes()) % 60;
}

LONG COleDateTimeSpan::GetSeconds() const throw()
{
	return LONG(GetTotalSeconds()) % 60;
}

COleDateTimeSpan& COleDateTimeSpan::operator=(double dblSpanSrc) throw()
{
	m_span = dblSpanSrc;
	m_status = valid;
	CheckRange();
	return *this;
}

bool COleDateTimeSpan::operator==(const COleDateTimeSpan& dateSpan) const throw()
{
	if(GetStatus() == dateSpan.GetStatus())
	{
		if(GetStatus() == valid)
			return (m_span == dateSpan.m_span);			
		
		return (GetStatus() == null);
	}

	return false;
}

bool COleDateTimeSpan::operator!=(const COleDateTimeSpan& dateSpan) const throw()
{
	return !operator==(dateSpan);
}

bool COleDateTimeSpan::operator<(const COleDateTimeSpan& dateSpan) const throw()
{
	ATLASSERT(GetStatus() == valid);
	ATLASSERT(dateSpan.GetStatus() == valid);
	if( (GetStatus() == valid) && (GetStatus() == dateSpan.GetStatus()) )
		return m_span < dateSpan.m_span;

	return false;
}

bool COleDateTimeSpan::operator>(const COleDateTimeSpan& dateSpan) const throw()
{
	ATLASSERT(GetStatus() == valid);
	ATLASSERT(dateSpan.GetStatus() == valid);
	if( (GetStatus() == valid) && (GetStatus() == dateSpan.GetStatus()) )
		return m_span > dateSpan.m_span ;

	return false;
}

bool COleDateTimeSpan::operator<=(const COleDateTimeSpan& dateSpan) const throw()
{
	return operator<(dateSpan) || operator==(dateSpan);
}

bool COleDateTimeSpan::operator>=(const COleDateTimeSpan& dateSpan) const throw()
{
	return operator>(dateSpan) || operator==(dateSpan);
}

COleDateTimeSpan COleDateTimeSpan::operator+(const COleDateTimeSpan& dateSpan) const throw()
{
	COleDateTimeSpan dateSpanTemp;

	// If either operand Null, result Null
	if (GetStatus() == null || dateSpan.GetStatus() == null)
	{
		dateSpanTemp.SetStatus(null);
		return dateSpanTemp;
	}

	// If either operand Invalid, result Invalid
	if (GetStatus() == invalid || dateSpan.GetStatus() == invalid)
	{
		dateSpanTemp.SetStatus(invalid);
		return dateSpanTemp;
	}

	// Add spans and validate within legal range
	dateSpanTemp.m_span = m_span + dateSpan.m_span;
	dateSpanTemp.CheckRange();

	return dateSpanTemp;
}

COleDateTimeSpan COleDateTimeSpan::operator-(const COleDateTimeSpan& dateSpan) const throw()
{
	COleDateTimeSpan dateSpanTemp;

	// If either operand Null, result Null
	if (GetStatus() == null || dateSpan.GetStatus() == null)
	{
		dateSpanTemp.SetStatus(null);
		return dateSpanTemp;
	}

	// If either operand Invalid, result Invalid
	if (GetStatus() == invalid || dateSpan.GetStatus() == invalid)
	{
		dateSpanTemp.SetStatus(invalid);
		return dateSpanTemp;
	}

	// Subtract spans and validate within legal range
	dateSpanTemp.m_span = m_span - dateSpan.m_span;
	dateSpanTemp.CheckRange();

	return dateSpanTemp;
}

COleDateTimeSpan& COleDateTimeSpan::operator+=(const COleDateTimeSpan dateSpan) throw()
{
	ATLASSERT(GetStatus() == valid);
	ATLASSERT(dateSpan.GetStatus() == valid);
	*this = *this + dateSpan;
	CheckRange();
	return *this;
}

COleDateTimeSpan& COleDateTimeSpan::operator-=(const COleDateTimeSpan dateSpan) throw()
{
	ATLASSERT(GetStatus() == valid);
	ATLASSERT(dateSpan.GetStatus() == valid);
	*this = *this - dateSpan;
	CheckRange();
	return *this;
}

COleDateTimeSpan COleDateTimeSpan::operator-() const throw()
{
	return -this->m_span;
}

COleDateTimeSpan::operator double() const throw()
{
	return m_span;
}

void COleDateTimeSpan::SetDateTimeSpan(LONG lDays, int nHours, int nMins, int nSecs) throw()
{
	// Set date span by breaking into fractional days (all input ranges valid)
	m_span = lDays + ((double)nHours)/24 + ((double)nMins)/(24*60) +
		((double)nSecs)/(24*60*60);
	m_status = valid;
	CheckRange();
}

void COleDateTimeSpan::CheckRange()
{
	if(m_span < -maxDaysInSpan || m_span > maxDaysInSpan)
		m_status = invalid;
}

/////////////////////////////////////////////////////////////////////////////
// COleDateTime
/////////////////////////////////////////////////////////////////////////////

COleDateTime WINAPI COleDateTime::GetCurrentTime() throw()
{
	return COleDateTime(::time(NULL));
}

COleDateTime::COleDateTime() throw() :
	m_dt( 0 ), m_status(valid)
{
}

COleDateTime::COleDateTime( const VARIANT& varSrc ) throw() :
	m_dt( 0 ), m_status(valid)
{
	*this = varSrc;
}

COleDateTime::COleDateTime( DATE dtSrc ) throw() :
	m_dt( dtSrc ), m_status(valid)
{
}

COleDateTime::COleDateTime( time_t timeSrc) throw() :
	m_dt( 0 ), m_status(valid)
{
	*this = timeSrc;
}

COleDateTime::COleDateTime( const FILETIME& filetimeSrc ) throw() :
	m_dt( 0 ), m_status(valid)
{
	*this = filetimeSrc;
}

COleDateTime::COleDateTime( const SYSTEMTIME& systimeSrc ) throw() :
	m_dt( 0 ), m_status(valid)
{
	*this = systimeSrc;
}

COleDateTime::COleDateTime(int nYear, int nMonth, int nDay,
	int nHour, int nMin, int nSec) throw()
{
	SetDateTime(nYear, nMonth, nDay, nHour, nMin, nSec);
}

COleDateTime::COleDateTime(WORD wDosDate, WORD wDosTime) throw()
{
	m_status = ::DosDateTimeToVariantTime(wDosDate, wDosTime, &m_dt) ?
		valid : invalid;
}

void COleDateTime::SetStatus(DateTimeStatus status) throw()
{
	m_status = status;
}

COleDateTime::DateTimeStatus COleDateTime::GetStatus() const throw()
{
	return m_status;
}

bool COleDateTime::GetAsSystemTime(SYSTEMTIME& sysTime) const throw()
{
	return GetStatus() == valid && ::VariantTimeToSystemTime(m_dt, &sysTime);
}

bool COleDateTime::GetAsUDATE(UDATE &udate) const throw()
{
	return SUCCEEDED(::VarUdateFromDate(m_dt, 0, &udate));
}

int COleDateTime::GetYear() const throw()
{
	SYSTEMTIME st;
	return GetAsSystemTime(st) ? st.wYear : error;
}

int COleDateTime::GetMonth() const throw()
{
	SYSTEMTIME st;
	return GetAsSystemTime(st) ? st.wMonth : error;
}

int COleDateTime::GetDay() const throw()
{
	SYSTEMTIME st;
	return GetAsSystemTime(st) ? st.wDay : error;
}

int COleDateTime::GetHour() const throw()
{
	SYSTEMTIME st;
	return GetAsSystemTime(st) ? st.wHour : error;
}

int COleDateTime::GetMinute() const throw()
{
	SYSTEMTIME st;
	return GetAsSystemTime(st) ? st.wMinute : error;
}

int COleDateTime::GetSecond() const throw()
{ 
	SYSTEMTIME st;
	return GetAsSystemTime(st) ? st.wSecond : error;
}

int COleDateTime::GetDayOfWeek() const throw()
{
	SYSTEMTIME st;
	return GetAsSystemTime(st) ? st.wDayOfWeek + 1 : error;
}

int COleDateTime::GetDayOfYear() const throw()
{
	UDATE udate;
	return GetAsUDATE(udate) ? udate.wDayOfYear : error;
}

COleDateTime& COleDateTime::operator=(const VARIANT& varSrc) throw()
{
	if (varSrc.vt != VT_DATE)
	{
		VARIANT varDest;
		varDest.vt = VT_EMPTY;
		if(SUCCEEDED(::VariantChangeType(&varDest, const_cast<VARIANT *>(&varSrc), 0, VT_DATE)))
		{
			m_dt = varDest.date;
			m_status = valid;
		}
		else
			m_status = invalid;
	}
	else
	{
		m_dt = varSrc.date;
		m_status = valid;
	}

	return *this;
}

COleDateTime& COleDateTime::operator=(DATE dtSrc) throw()
{
	m_dt = dtSrc;
	m_status = valid;
	return *this;
}

COleDateTime& COleDateTime::operator=(const time_t& timeSrc) throw()
{
	// Convert time_t to struct tm
	tm *ptm = localtime(&timeSrc);
	
	if (ptm != NULL)
	{
		m_status = _AfxOleDateFromTm((WORD)(ptm->tm_year + 1900),
			(WORD)(ptm->tm_mon + 1), (WORD)ptm->tm_mday,
			(WORD)ptm->tm_hour, (WORD)ptm->tm_min,
			(WORD)ptm->tm_sec, m_dt) ? valid : invalid;
	}
	else
	{
		// Local time must have failed (timsSrc before 1/1/70 12am)
		SetStatus(invalid);
		ATLASSERT(FALSE);
	}
	
	return *this;
}

COleDateTime &COleDateTime::operator=(const SYSTEMTIME &systimeSrc) throw()
{
	m_status = ::SystemTimeToVariantTime(const_cast<SYSTEMTIME *>(&systimeSrc), &m_dt) ?
		valid : invalid;
	return *this;
}

COleDateTime &COleDateTime::operator=(const FILETIME &filetimeSrc) throw()
{
	SYSTEMTIME st;
	m_status = ::FileTimeToSystemTime(&filetimeSrc, &st) &&
				::SystemTimeToVariantTime(&st, &m_dt) ?
		valid : invalid;

	return *this;
}

COleDateTime &COleDateTime::operator=(const UDATE &udate) throw()
{
	m_status = (S_OK == VarDateFromUdate((UDATE*)&udate, 0, &m_dt)) ? valid : invalid;

	return *this;
}

bool COleDateTime::operator==( const COleDateTime& date ) const throw()
{
	if(GetStatus() == date.GetStatus())
	{
		if(GetStatus() == valid)
			return( m_dt == date.m_dt );

		return (GetStatus() == null);
	}
	return false;

}

bool COleDateTime::operator!=( const COleDateTime& date ) const throw()
{
	return !operator==(date);
}

bool COleDateTime::operator<( const COleDateTime& date ) const throw()
{
	ATLASSERT(GetStatus() == valid);
	ATLASSERT(date.GetStatus() == valid);
	if( (GetStatus() == valid) && (GetStatus() == date.GetStatus()) )
		return( DoubleFromDate( m_dt ) < DoubleFromDate( date.m_dt ) );

	return false;
}

bool COleDateTime::operator>( const COleDateTime& date ) const throw()
{
	ATLASSERT(GetStatus() == valid);
	ATLASSERT(date.GetStatus() == valid);
	if( (GetStatus() == valid) && (GetStatus() == date.GetStatus()) )
		return( DoubleFromDate( m_dt ) > DoubleFromDate( date.m_dt ) );

	return false;		
}

bool COleDateTime::operator<=( const COleDateTime& date ) const throw()
{
	return operator<(date) || operator==(date);
}

bool COleDateTime::operator>=( const COleDateTime& date ) const throw()
{
	return operator>(date) || operator==(date);
}

COleDateTime COleDateTime::operator+( COleDateTimeSpan dateSpan ) const throw()
{
	ATLASSERT(GetStatus() == valid);
	ATLASSERT(dateSpan.GetStatus() == valid);
	return( COleDateTime( DateFromDouble( DoubleFromDate( m_dt )+(double)dateSpan ) ) );
}

COleDateTime COleDateTime::operator-( COleDateTimeSpan dateSpan ) const throw()
{
	ATLASSERT(GetStatus() == valid);
	ATLASSERT(dateSpan.GetStatus() == valid);
	return( COleDateTime( DateFromDouble( DoubleFromDate( m_dt )-(double)dateSpan ) ) );
}

COleDateTime& COleDateTime::operator+=( COleDateTimeSpan dateSpan ) throw()
{
	ATLASSERT(GetStatus() == valid);
	ATLASSERT(dateSpan.GetStatus() == valid);
	m_dt = DateFromDouble( DoubleFromDate( m_dt )+(double)dateSpan );
	return( *this );
}

COleDateTime& COleDateTime::operator-=( COleDateTimeSpan dateSpan ) throw()
{
	ATLASSERT(GetStatus() == valid);
	ATLASSERT(dateSpan.GetStatus() == valid);
	m_dt = DateFromDouble( DoubleFromDate( m_dt )-(double)dateSpan );
	return( *this );
}

COleDateTimeSpan COleDateTime::operator-(const COleDateTime& date) const throw()
{
	ATLASSERT(GetStatus() == valid);
	ATLASSERT(date.GetStatus() == valid);
	return DoubleFromDate(m_dt) - DoubleFromDate(date.m_dt);
}

COleDateTime::operator DATE() const throw()
{
	ATLASSERT(GetStatus() == valid);
	return( m_dt );
}

int COleDateTime::SetDateTime(int nYear, int nMonth, int nDay,
	int nHour, int nMin, int nSec) throw()
{
	SYSTEMTIME st;
	::ZeroMemory(&st, sizeof(SYSTEMTIME));

	st.wYear = WORD(nYear);
	st.wMonth = WORD(nMonth);
	st.wDay = WORD(nDay);
	st.wHour = WORD(nHour);
	st.wMinute = WORD(nMin);
	st.wSecond = WORD(nSec);

	return m_status = ::SystemTimeToVariantTime(&st, &m_dt) ? valid : invalid;
}

int COleDateTime::SetDate(int nYear, int nMonth, int nDay) throw()
{
	return SetDateTime(nYear, nMonth, nDay, 0, 0, 0);
}

int COleDateTime::SetTime(int nHour, int nMin, int nSec) throw()
{
	// Set date to zero date - 12/30/1899
	return SetDateTime(1899, 12, 30, nHour, nMin, nSec);
}

double WINAPI COleDateTime::DoubleFromDate( DATE date ) throw()
{
	double fTemp;

	// No problem if positive
	if( date >= 0 )
	{
		return( date );
	}

	// If negative, must convert since negative dates not continuous
	// (examples: -1.25 to -.75, -1.50 to -.50, -1.75 to -.25)
	fTemp = ceil( date );

	return( fTemp-(date-fTemp) );
}

DATE WINAPI COleDateTime::DateFromDouble( double f ) throw()
{
	double fTemp;

	// No problem if positive
	if( f >= 0 )
	{
		return( f );
	}

	// If negative, must convert since negative dates not continuous
	// (examples: -.75 to -1.25, -.50 to -1.50, -.25 to -1.75)
	fTemp = floor( f ); // fTemp is now whole part

	return( fTemp+(fTemp-f) );
}

//////////////////////////////////////////////////////////////////////////////////////

bool COleDateTime::ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags, LCID lcid) throw()
{
	USES_CONVERSION;
	LPCTSTR pszDate = ( lpszDate == NULL ) ? _T("") : lpszDate;

	HRESULT hr;
	LPOLESTR p = T2OLE((LPTSTR)pszDate);
#ifndef _UNICODE
	if( p == NULL )
	{
		m_dt = 0;
		m_status = invalid;
		return false;
	}
#endif // _UNICODE

	if (FAILED(hr = VarDateFromStr( p, lcid, dwFlags, &m_dt )))
	{
		if (hr == DISP_E_TYPEMISMATCH)
		{
			// Can't convert string to date, set 0 and invalidate
			m_dt = 0;
			m_status = invalid;
			return false;
		}
		else if (hr == DISP_E_OVERFLOW)
		{
			// Can't convert string to date, set -1 and invalidate
			m_dt = -1;
			m_status = invalid;
			return false;
		}
		else
		{
			m_dt = -1;
			m_status = invalid;
			return false;
		}
	}

	m_status = valid;
	return true;
}

CString COleDateTimeSpan::Format(UINT nFormatID) const
{
	CString strFormat;
	strFormat.LoadString(nFormatID);
	return Format(strFormat);
}
#endif // !_ATL_MIN_CRT

#ifndef _ATL_MIN_CRT
CString COleDateTime::Format(DWORD dwFlags, LCID lcid) const
{
	// If null, return empty string
	if (GetStatus() == null)
		return _T("");

	// If invalid, return DateTime global string
	if (GetStatus() == invalid)
		return szInvalidDateTime;

	CComBSTR bstr;
	if (FAILED(::VarBstrFromDate(m_dt, lcid, dwFlags, &bstr)))
		return szInvalidDateTime;

	CString tmp = CString(bstr);
	return tmp;
}

CString COleDateTime::Format(LPCTSTR pFormat) const
{
	// If null, return empty string
	if(GetStatus() == null)
		return _T("");

	// If invalid, return DateTime global string
	if(GetStatus() == invalid)
		return szInvalidDateTime;

	UDATE ud;
	if (S_OK != VarUdateFromDate(m_dt, 0, &ud))
		return szInvalidDateTime;

	struct tm tmTemp;
	tmTemp.tm_sec	= ud.st.wSecond;
	tmTemp.tm_min	= ud.st.wMinute;
	tmTemp.tm_hour	= ud.st.wHour;
	tmTemp.tm_mday	= ud.st.wDay;
	tmTemp.tm_mon	= ud.st.wMonth - 1;
	tmTemp.tm_year	= ud.st.wYear - 1900;
	tmTemp.tm_wday	= ud.st.wDayOfWeek;
	tmTemp.tm_yday	= ud.wDayOfYear - 1;
	tmTemp.tm_isdst	= 0;

	CString strDate;
	LPTSTR lpszTemp = strDate.GetBufferSetLength(256);
	_tcsftime(lpszTemp, strDate.GetLength(), pFormat, &tmTemp);
	strDate.ReleaseBuffer();

	return strDate;
}

CString COleDateTime::Format(UINT nFormatID) const
{
	CString strFormat;
	strFormat.LoadString(nFormatID);
	return Format(strFormat);
}
#endif // !_ATL_MIN_CRT

#ifdef __oledb_h__
COleDateTime::COleDateTime(const DBTIMESTAMP& dbts)
{
	SYSTEMTIME st;
	::ZeroMemory(&st, sizeof(SYSTEMTIME));

	st.wYear = WORD(dbts.year);
	st.wMonth = WORD(dbts.month);
	st.wDay = WORD(dbts.day);
	st.wHour = WORD(dbts.hour);
	st.wMinute = WORD(dbts.minute);
	st.wSecond = WORD(dbts.second);

	m_status = ::SystemTimeToVariantTime(&st, &m_dt) ? valid : invalid;
}

bool COleDateTime::GetAsDBTIMESTAMP(DBTIMESTAMP& dbts) const
{
	UDATE ud;
	if (S_OK != VarUdateFromDate(m_dt, 0, &ud))
		return false;

	dbts.year = (SHORT) ud.st.wYear;
	dbts.month = (USHORT) ud.st.wMonth;
	dbts.day = (USHORT) ud.st.wDay;
	dbts.hour = (USHORT) ud.st.wHour;
	dbts.minute = (USHORT) ud.st.wMinute;
	dbts.second = (USHORT) ud.st.wSecond;
	dbts.fraction = 0;

	return true;
}

#endif