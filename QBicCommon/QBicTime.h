#pragma once
/**
*@brief 평균 시간
*@details 평균 날짜, 평균 시간
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicTime : public CTime
{
public:
    QBicTime();
    QBicTime(int nYear, int nMonth, int nDay, int nHour=0, int nMinute=0, int nSec=0);
    QBicTime(CTime& oTime);
    virtual ~QBicTime();

public:
	/**
	*@brief 일간 간격
	*@param const QBicTime& right	
	*@return 일간 차이 반환
	*/
    __int64 DayInterval (const QBicTime& right) const;
	/**
	*@brief 시간 간격
	*@param const QBicTime& right	
	*@return 총 시간 차이 반환
	*/
    __int64 SecInterval (const QBicTime& right) const;

public:
	/**
	*@brief 년 월 일
	*@return 년-월-일 반환
	*/
	CString ToYmdString()
	{
		CString strTemp(_T(""));
		strTemp.Format(_T("%04d-%02d-%02d"), GetYear(), GetMonth(), GetDay());
		return strTemp;
	}
	/**
	*@brief 년 월 일 시 분 초 
	*@return 년-월-일-시-분-초 반환
	*/
	CString ToString()
	{
		CString strTemp(_T(""));
		strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond());
		return strTemp;
	}
	/**
	*@brief 년 월 일 시 분 초
	*@return 년월일시분초 반환
	*/
	CString ToString16Digit()
	{
		CString strTemp(_T(""));
		strTemp.Format(_T("%04d%02d%02d%02d%02d%02d"), GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond());
		return strTemp;
	}
};

class AFX_EXT_CLASS QBicStrTime
{
public:
    QBicStrTime() {
        m_nYear = 1900;
        m_nMonth = 1;
        m_nDay = 1;
        m_nHour = 0;
        m_nMin = 0;
        m_nSec = 0;
    }
    QBicStrTime(__int64 nxTime) {
        CString strTime(_T(""));
        strTime.Format(_T("%I64d"), nxTime);
        ParseDataTime(strTime);
    }
	QBicStrTime(CString strTime) {
        ParseDataTime(strTime);
	}
    virtual ~QBicStrTime() {};

private:
    void ParseDataTime(CString strTime)
    {
        m_nYear = 1900;
        m_nMonth = 1;
        m_nDay = 1;
        m_nHour = 0;
        m_nMin = 0;
        m_nSec = 0;

        m_strTime = strTime.Trim();
        m_nLength = m_strTime.GetLength();

        if (m_nLength >= 4) {
            m_nYear = _ttoi(m_strTime.Mid(0, 4));
        }
        else { m_nYear = 0; }

        if (m_nLength >= 6) {
            m_nMonth = _ttoi(m_strTime.Mid(4, 2));
        }
        else { m_nMonth = 0; }

        if (m_nLength >= 8) {
            m_nDay = _ttoi(m_strTime.Mid(6, 2));
        }
        else { m_nDay = 0; }

        if (m_nLength >= 10) {
            m_nHour = _ttoi(m_strTime.Mid(8, 2));
        }
        else { m_nHour = 0; }

        if (m_nLength >= 12) {
            m_nMin = _ttoi(m_strTime.Mid(10, 2));
        }
        else { m_nMin = 0; }

        if (m_nLength >= 14) {
            m_nSec = _ttoi(m_strTime.Mid(12, 2));
        }
        else { m_nSec = 0; }
    }

private:
	int     m_nLength;
	CString m_strTime;
public:
	int m_nYear;
	int m_nMonth;
	int m_nDay;
	int m_nHour;
	int m_nMin;
	int m_nSec;
public:
	//   ∨ ∨
	//0123 45 67 89 01 23
	//2015 05 26 14 24 18

	bool IsCheckDataType()
	{
		if (m_nMonth < 1 || 12 < m_nMonth)
		{
			return false;
		}

		if (m_nDay < 1 || 31 < m_nDay)
		{
			return false;
		}

		if (m_nHour < 0 || 24 < m_nHour)
		{
			return false;
		}

		if (m_nMin < 0 || 59 < m_nMin)
		{
			return false;
		}

		if (m_nSec < 0 || 59 < m_nSec)
		{
			return false;
		}

		return true;
	}

public:
	CTime GetTime() {
		return CTime(m_nYear, m_nMonth, m_nDay, m_nHour, m_nMin, m_nSec);
	}
};