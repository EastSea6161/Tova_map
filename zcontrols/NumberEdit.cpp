// NumberEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NumberEdit.h"


// ZNumberEdit

IMPLEMENT_DYNAMIC(ZNumberEdit, CEdit)

ZNumberEdit::ZNumberEdit()
	: m_bReal(false), m_bRange(false), m_dMinValue(-999999.0), m_dMaxValue(999999.0)
{

}

ZNumberEdit::~ZNumberEdit()
{
}


BEGIN_MESSAGE_MAP(ZNumberEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



void ZNumberEdit::EnableRealNumber(bool bReal)
{
	m_bReal = bReal;
}

void ZNumberEdit::EnableMinMax(double a_dMinValue, double a_dMaxValue)
{
    m_bRange    = true;
    m_dMinValue = a_dMinValue;
    m_dMaxValue = a_dMaxValue;
}


// ZNumberEdit 메시지 처리기입니다.
void ZNumberEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // -1 : range error 
    //  0 : error, 
    //  2 : OK

    UpdateData(FALSE);
    
    if(IsCharAlpha((TCHAR)nChar))
    {
        return;
    }

    // 숫자, (.), (-)
    if ( !((nChar >= _T('0')) && (nChar <= _T('9'))) )
    {
        if( !(nChar == _T('.') || nChar == _T('-')) )
        {
            CEdit::OnChar(nChar, nRepCnt, nFlags);
            return;
        }
    }
    
	int nCheck = 0;
	if(((nChar >= _T('0')) && (nChar <= _T('9'))) || (VK_BACK == nChar))
	{
		nCheck = 2;
	}
	else if(nChar == _T('.'))
	{
		if(true == m_bReal)
		{
			CString strNumber;
			GetWindowText(strNumber);
			if(strNumber.Find(_T('.')) == -1)
			{
				nCheck = 2;
			}
		}
	}
    else if(nChar == _T('-'))
    {        
		// check '-' exists
		CString strNumber;
		GetWindowText(strNumber);
		if(strNumber.Find(_T('-')) == -1)
		{
			int nStart, nEnd;
			GetSel(nStart, nEnd);
			if(nStart == 0)
			{
				nCheck = 2;
			}
		}
    }
    
    if ( nCheck == 2 && m_bRange)
    {
        CString strNumber;
        GetWindowText(strNumber);
        CString strTemp((TCHAR)nChar);

        int nStart, nEnd;
        GetSel(nStart, nEnd);

        strNumber.Insert(nEnd, strTemp);        
        double dValue = _ttof(strNumber);
                
        if ( m_dMinValue <= dValue && dValue <= m_dMaxValue )
        {
            nCheck = 2;
        }
        else
        {
            nCheck = -1;
        }
    }

    // 나중에 : IsCharAlphaNumeric : API 확인 해 볼것(nombara)    
	if( nCheck == 2 )
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
    else if ( nCheck == -1)
    {
        EDITBALLOONTIP ebt;
        ebt.cbStruct = sizeof(EDITBALLOONTIP);
        ebt.pszTitle = L"값의 범위를 초과했습니다";
        ebt.ttiIcon = TTI_ERROR;
        CStringW strMsg;
        strMsg.Format(L"%.2f ~ %.2f", m_dMinValue, m_dMaxValue);
        ebt.pszText = strMsg;

#ifdef _UNICODE
        ShowBalloonTip(&ebt);
#else
        SendMessage(EM_SHOWBALLOONTIP, (WPARAM)&ebt, NULL);
#endif
    }
	else 
	{
		EDITBALLOONTIP ebt;
		ebt.cbStruct = sizeof(EDITBALLOONTIP);
		ebt.pszTitle = L"허용되지 않는 문자";
		ebt.ttiIcon = TTI_ERROR;
		if(true == m_bReal)
		{
			ebt.pszText = L"여기에는 실수 와 정수만 입력할 수 있습니다.";
		}
		else
		{
			ebt.pszText = L"여기에는 숫자만 입력할 수 있습니다.";
		}

#ifdef _UNICODE
		ShowBalloonTip(&ebt);
#else
		SendMessage(EM_SHOWBALLOONTIP, (WPARAM)&ebt, NULL);
#endif
	}
}
