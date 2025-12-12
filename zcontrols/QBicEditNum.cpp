#include "stdafx.h"
#include "QBicEditNum.h"

IMPLEMENT_DYNAMIC(QBicEditNum, CEdit)

QBicEditNum::QBicEditNum() : m_bReal(true), m_bRange(false), m_dMinValue(-999999.0), m_dMaxValue(999999.0)
{
    m_bUseMinus = true;
}

QBicEditNum::~QBicEditNum()
{
}

BEGIN_MESSAGE_MAP(QBicEditNum, CEdit)
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void QBicEditNum::ShowTip()
{
    CString strTip(_T(""));
    if (m_bReal == true) {
        strTip.Format(_T("Real Number"));
        if (m_bRange == true) {
            strTip.Append(_T(":"));
            strTip.AppendFormat(m_strFormat, m_dMinValue);
            strTip.Append(_T("~"));
            strTip.AppendFormat(m_strFormat, m_dMaxValue);
        }
    } else {
        strTip.Format(_T("Integer Number"));
        if (m_bRange == true) {
            strTip.Append(_T(":"));
            strTip.AppendFormat(m_strFormat, (int)m_dMinValue);
            strTip.Append(_T("~"));
            strTip.AppendFormat(m_strFormat, (int)m_dMaxValue);
        }
    }

    ShowBalloonTip(_T("Edit"), strTip, TTI_INFO);
}

void QBicEditNum::OnKillFocus(CWnd* pNewWnd) 
{
    if (m_bRange == true) {
        CString strTxt; {
            GetWindowText(strTxt);
        }

        if (m_bReal == false) {
            int nValue = _ttoi(strTxt);
            if (!(m_dMinValue <= nValue && nValue <= m_dMaxValue)) {
                CString strTemp(_T("")); {
                    if (nValue < m_dMinValue)
                        strTemp.Format(m_strFormat, (int)m_dMinValue);
                    else 
                        strTemp.Format(m_strFormat, (int)m_dMaxValue);

                    SetWindowText(strTemp);
                }
            }
        } else {
            double dValue = _ttof(strTxt);
            if (!(m_dMinValue <= dValue && dValue <= m_dMaxValue)) {
                CString strTemp(_T("")); {
                    if (dValue < m_dMinValue)
                        strTemp.Format(m_strFormat, m_dMinValue);
                    else 
                        strTemp.Format(m_strFormat, m_dMaxValue);

                    SetWindowText(strTemp);
                }
            }
        }        
    }

    CEdit::OnKillFocus(pNewWnd); 
}

void QBicEditNum::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (VK_DELETE == nChar) {        
        CString strTxt; {
            GetWindowText(strTxt);
        }

        int nStart(0), nEnd(0); {
            GetSel(nStart, nEnd);
        }

        CString strTemp(_T("")); {
            strTemp = strTxt.Left(nStart-1);
            strTemp = strTxt.Mid(nEnd);
            strTemp.Format(_T("%s%s"), strTxt.Left(nStart-1), strTxt.Mid(nEnd));
        }

        strTxt = strTemp;

        double dValue(0);
        if (m_bReal == true) {        
            if (m_bUseMinus && strTxt.Compare(_T("-")) == 0) {
                dValue = 0;
            } else {
                LPTSTR lpszInvalid;
                dValue = _tcstod(strTxt, &lpszInvalid);
                if (*lpszInvalid) {
                    ShowTip();
                    return;
                }
            }        
        } else { // 정수
            if (m_bUseMinus && strTxt.Compare(_T("-")) == 0 ) {
                dValue = 0;
            } else {
                LPTSTR lpszInvalid;
                long   nValue = _tcstol(strTxt, &lpszInvalid, 10);
                if (*lpszInvalid) {
                    ShowTip();
                    return;
                }

                dValue = nValue;
            }        
        }

        if (dValue < m_dMinValue) {
            ShowTip(); 
            // 값을 입력 받아야 한다.
        } else if (m_dMaxValue < dValue) {
            ShowTip(); 
            return;
        }
    }

    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void QBicEditNum::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if(IsCharAlpha((TCHAR)nChar) == TRUE) {
        ShowTip();
        return;
    }

    if (m_bUseMinus == false) {
        if (nChar == _T('-')) {
            ShowTip();
            return;
        }
    }

    CString strTxt; {
        GetWindowText(strTxt);
    }

    int nStart(0), nEnd(0); {
        GetSel(nStart, nEnd);
    }

    if (VK_BACK == nChar) {
        CString strTemp(_T("")); {
            strTemp = strTxt.Left(nStart-1);
            strTemp = strTxt.Mid(nEnd);
            strTemp.Format(_T("%s%s"), strTxt.Left(nStart-1), strTxt.Mid(nEnd));
        }

        strTxt = strTemp;
    } else if (nStart == nEnd) {
        strTxt.Insert(nEnd, (TCHAR)nChar); 
    } else {
        CString strTemp(_T("")); {
            strTemp = strTxt.Left(nStart);
            strTemp = strTxt.Mid(nEnd);
            strTemp.Format(_T("%s%c%s"), strTxt.Left(nStart), (TCHAR)nChar, strTxt.Mid(nEnd));
        }

        strTxt = strTemp;
    }

    double dValue(0);
    if (m_bReal == true) {        
        if (m_bUseMinus && strTxt.Compare(_T("-")) == 0) {
            dValue = 0;
        } else {
            LPTSTR lpszInvalid;
            dValue = _tcstod(strTxt, &lpszInvalid);
            if (*lpszInvalid) {
                ShowTip();
                return;
            }
        }        
    } else { // 정수
        if (m_bUseMinus && strTxt.Compare(_T("-")) == 0) {
            dValue = 0;
        } else {
            LPTSTR lpszInvalid;
            long   nValue = _tcstol(strTxt, &lpszInvalid, 10);
            if (*lpszInvalid) {
                ShowTip();
                return;
            }

            dValue = nValue;
        }        
    }

    if (dValue < m_dMinValue) {
        ShowTip(); 
        // 값을 입력 받아야 한다.
    } else if (m_dMaxValue < dValue) {
        ShowTip(); 
        return;
    }

    // 정상적인 경우 통과
    CEdit::OnChar(nChar, nRepCnt, nFlags);
}