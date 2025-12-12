/**
 * @file ColoredDialog.h
 * @brief ZColoredDialog 선언
 * @author 
 * @date 2011.02.16
 * @remark 
 */

#pragma once


/**
 * @brief Dialog 배경색을 변경하기 위한 Custom dialog base class.
 *          WM_CTRLCOLOR 메시지 handler에서 지정된 색상의 Brush를 반환한다.
 * @remark 
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.02.16
 */
class AFX_EXT_CLASS ZColoredDialog : public CDialog
{
	DECLARE_DYNAMIC(ZColoredDialog)

public:
	ZColoredDialog(void);
	ZColoredDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	ZColoredDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);

	virtual ~ZColoredDialog();

public :
	/**
	 * @brief 다이얼로그의 배경색을 지정한다.
	 * @param[in] crBackground - 다이얼로그의 배경색
	 */
	void setBackgroundColor(COLORREF crBackground) { m_crBackground = crBackground; }

	/**
	 * @brief 다이얼로그의 배경색을 반환한다.
	 * @return COLORREF - 다이얼로그의 배경색
	 */
	COLORREF getBackgroundColor(void) { return m_crBackground; }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public :
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected :
	COLORREF m_crBackground;
	CBrush m_brBackground;
};
