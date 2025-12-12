#pragma once

enum EnumEditNum {
    EnumInt,
    EnumDbl
};

#pragma region Info
// CEdit -> 정수형, 실수형 입력
// - 범위를 정의할 수 있다.
// - 
#pragma endregion Info

class AFX_EXT_CLASS QBicEditNum : public CEdit
{
    DECLARE_DYNAMIC(QBicEditNum)

public:
    QBicEditNum();
    virtual ~QBicEditNum();

protected:
    DECLARE_MESSAGE_MAP()

public :
    void SetEditType(EnumEditNum enEditNum) {
        if (enEditNum == EnumInt) {
            m_bReal = false;
        } else {
            m_bReal = true;
        }
    }

    void ClearRange() {
        m_bRange = false;
    }

    void SetRange(int nMin, int nMax, CString strFormat=_T("%d")) {
        m_bRange = true;
        m_bReal  = false;

        if (nMin <= nMax) {
            m_dMinValue = nMin;
            m_dMaxValue = nMax;
        } else {
            m_dMinValue = nMax;
            m_dMaxValue = nMin;
        }

        m_strFormat = strFormat;
    }

    void SetRange(double dMin, double dMax, CString strFormat=_T("%f")) {
        m_bRange = true;
        m_bReal  = true;
        if (dMin <= dMax) {
            m_dMinValue = dMin;
            m_dMaxValue = dMax;
        } else {
            m_dMinValue = dMax;
            m_dMaxValue = dMin;
        }

        m_strFormat = strFormat;
    }

protected :
    bool m_bReal;
    bool m_bRange;
protected:
    bool m_bUseMinus;
protected:
    double m_dMinValue;
    double m_dMaxValue;
protected:
    CString m_strFormat;

private:
    void ShowTip();
protected:
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};