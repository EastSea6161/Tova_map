/**
 * @file PointMarkerSelectDlg.h
 * @brief 포인트 심볼 설정 다이얼로그
 * @author nomplus2@qbicware.com
 * @date 2011.08.28
 * @remark 
 */


#pragma once
#include "afxwin.h"

// KPointMarkerSelectDlg 대화 상자입니다.


/**
* @brief 포인트 심볼 설정
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.28
*/
class KPointMarkerSelectDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KPointMarkerSelectDlg)

public:
	KPointMarkerSelectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPointMarkerSelectDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3240_SelectSymbolDlg };

private:
    CFont   m_oQbicDefaultMarkerFont;
    int     m_selectedMarkerIntex;

private:
	const int m_nReportColCnt;

public:

    /**
    * @brief 포인트 Marker Index Get
    * @return int - 포인트 Marker Index
    */
    int GetSelectedMarkerIndex() 
    {
        return m_selectedMarkerIntex;
    }
	COLORREF GetSelectedMarkerColor()
	{
		return m_clrBack;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
    CEdit m_SelectedMarker;

	CXTPColorPicker   m_cpBackColor;
	COLORREF          m_clrBack;

    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
	afx_msg void OnPaint();    
    afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
    afx_msg void OnBnClickedOk();
	afx_msg void OnSelEndOkBackClr();
	void RedrawMarkerColor();
private:
	bool m_bColorEdit;
public:
	void SetInitialData(int a_nSelectedMarkerIndex);
	void SetInitialData(int a_nSelectedMarkerIndex, COLORREF a_colorMarker);
};
