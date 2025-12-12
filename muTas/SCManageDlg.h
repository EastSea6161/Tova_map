/**
 * @file KSCManageDlg.h
 * @brief Screen & Cordon Management 
 * @author jyk@qbicware.com
 * @date 2012.05.07
 * @remark 
 */

#pragma once
#include "afxwin.h"
//^#include "ResizeDialogEx.h"
#include "TDrawScreenCordonInfo.h"

class KTarget;
class KMapView;

enum KEMScreenCordonType
{
	TYPE_SCREEN = 1,
	TYPE_CORDON = 2
};


// KSCManageDlg 대화 상자입니다.

class KSCManageDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KSCManageDlg)

public:
	KSCManageDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSCManageDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5750_SCManageDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	void SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}

	void SetMapView(KMapView* a_pMapView)
	{
		m_pMapView = a_pMapView;
	}

	void UpdateDrawState();

private:
	/**
	* @brief Report Control Header 초기화
	*/
	void InitReportHeader();
	void InitScreenReportHeader();
	void InitCordonReportHeader();

	/**
	* @brief 정보 Display
	*/
	void InitReportData();
	void InitScreenReportData(bool a_bColorRefresh);
	void InitCordonReportData(bool a_bColorRefresh);


	/**
	* @brief component resize seting
	*/
	void SetResizeComponent();


	void InitialColorRampCtrl();


	void SetColorRampInfo(KEMScreenCordonType a_emScreenCordonType, int nSelectIndex);


	void CheckboxControlScreen(BOOL a_bCheck);


	void CheckboxControlCordon(BOOL a_bCheck);


	void ControlScreenLineDraw(CXTPReportRecord* a_pRecord, BOOL a_bAddNRemove, bool a_bReDraw = true);


	void ControlCordonLineDraw(CXTPReportRecord* a_pRecord, BOOL a_bAddNRemove, bool a_bReDraw = true);

	/**
	* @brief 업데이트 하여 저장된 Screen 정보를 지도 상에 바로 적용 한다. 
	*/
	void ApplyUpdatedScreenInfo();


	/**
	* @brief 업데이트 하여 저장된 Cordon 정보를 지도 상에 바로 적용 한다. 
	*/
	void ApplyUpdatedCordonInfo();

private:
	CImageList       m_colorRampImageList;
	COLORREF         m_clrStart;
	COLORREF         m_clrEnd;


	KTarget*			    m_pTarget;
	KMapView*               m_pMapView;

	std::set<Integer/*ID*/> m_setScreenKey;
	std::set<Integer/*ID*/> m_setCordonKey;

protected:

    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
	void         CloseWindowProc();

	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCboScreenColorramp();
	afx_msg void OnCbnSelchangeCboCordonColorramp();
	afx_msg void OnScreenSelectCancel();
	afx_msg void OnScreenSelectAll();
	afx_msg void OnCordonSelectCancel();
	afx_msg void OnCordonSelectAll();
	afx_msg void OnScreenReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCordonReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnScreenReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnCordonReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnScreenReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnCordonReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedButtonDelScreenLine();
	afx_msg void OnBnClickedButtonDelCordonLine();
	afx_msg void OnScreenSelChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnScreenValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnCordonSelChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnCordonValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnBnClickedButtonSaveScreen();
	afx_msg void OnBnClickedButtonSaveCordon();
	afx_msg void OnBnClickedButtonRefreshScreen();
	afx_msg void OnBnClickedButtonRefreshCordon();
	
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ScreenReportCtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_CordonReportCtrl;

	CComboBoxEx       m_cboScreenColorRamp;    
	CComboBoxEx       m_cboCordonColorRamp;    

public:
    void ScreenLineRefresh();
    void CordonLineRefresh();
public:
    void ScreenLineSelect(__int64 nxID);
    void CordonLineSelect(__int64 nxID);
};

typedef std::shared_ptr<KSCManageDlg> KSCManageDlgPtr;