#pragma once

/**
 * @file MarkupStatic.h
 * @brief Markup(XAML) Static Class
 * @author nomplus2@qbicware.com
 * @date 2011.08.26
 * @remark 모형 UI 적용 클래스
 */

class CMarkupStatic : public CStatic, public CXTPMarkupContext
{
    // Construction
public:
    CMarkupStatic();

    // Attributes
public:

    // Operations
public:
    /**
	  * @brief XAML 설정
	  * @param[in] lpszMarkup - XAML Text
	  */
    void SetMarkupText(LPCTSTR lpszMarkup);

    // Implementation
public:
    virtual ~CMarkupStatic();

    // Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()

    //{{AFX_VIRTUAL(CMarkupStatic)
    BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CMarkupStatic)
    afx_msg void OnPaint();
    //}}AFX_MSG

    CXTPMarkupUIElement* m_pUIElement;
};

