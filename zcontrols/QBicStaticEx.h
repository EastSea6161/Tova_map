#pragma once

#define TRANS_BACK -1

class AFX_EXT_CLASS QBicStaticEx : public CStatic
{
    DECLARE_DYNAMIC(QBicStaticEx)

public:
    QBicStaticEx();
    virtual ~QBicStaticEx();

protected:
    DECLARE_MESSAGE_MAP()
protected:
    //afx_msg void OnPaint();
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
    virtual void PreSubclassWindow( );

private:
    COLORREF m_TextColor;
    COLORREF m_BackColor;
    CBrush   m_Brush;

public:
    void SetTextColor(COLORREF col) { 
        m_TextColor = col;
        UpdateCtrl();      
    }
    void SetBackColor(COLORREF col) { 
        m_BackColor = col;
        UpdateCtrl();      
    }

private:
    void UpdateCtrl();
};
