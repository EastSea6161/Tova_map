#pragma once

//^#include "ResizeDialogEx.h"
#include "afxwin.h"
#include "TChartTransit.h"
#include "TTransitPassenger.h"
#include "ImChampMultiIDFind.h"
#include "SeatCapaInfoDlg.h"
#include "TDrawLinkInfo.h"
#include "BulkDBaseLink.h"

class KTarget;
class KMapView;
class KIOTable;
class KIOColumn;

// KTransitLineThemesDlg 대화 상자입니다.

class KTransitLineThemesDlg : public KDialogEx, public ImChampModelessFeedback
{
	DECLARE_DYNAMIC(KTransitLineThemesDlg)

public:
	KTransitLineThemesDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTransitLineThemesDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4002_TRANSITLINE_THEMES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetViewTransit(Integer a_nxTransitID );
	void ActivatedFrameWndNotifyProcess();
	void DeActivatedFrameWndNotifyProcess();

private:
	void LoadAssignModelInfo(std::vector<TMultiModalInfo> &a_vecMultiModalInfo);
	void InitComboAssignModelSelect(std::vector<TMultiModalInfo> &a_vecMultiModalInfo);
	bool GetSelectedMinMaxValue(std::map<int, TTransitPassengerMinMax> &a_mapPassengerMinMax);
	void InitComboBoardAlight();
	void InsertComboBoardAlightData(CString a_strAddString, int a_nKeyIndex);
	void InitPassengerField();
	bool UserInputCheck();
	void ClearMapDraw(bool bRefresh);
	void UpdateTotalMinMaxData();
	void UpdateBoardAlightMinMaxData();
	void SetToolTip(std::vector<Integer> &a_vecTransitID, std::map<Integer, CString> &a_mapLineData);
	static bool		 PathWidthDescending( TDrawLinkInfo left, TDrawLinkInfo right );
protected:
	KTarget*     m_pTarget;
	KMapView*    m_pMapView;
	KIOTable*    m_pTableLine;
	
	std::vector<Integer>         m_vecTransitID;
	std::set<Integer>            m_setTransitID;
	TTransitPassengerMinMax      m_oTTransitPassengerMinMax;
	std::vector<TMultiModalInfo> m_vecMultiModalInfo;

	std::map<int, KIOColumn*>    m_mapPassengerField; 
	std::map<int, TTransitPassengerMinMax> m_mapPassengerMinMax;

	std::vector<KIDCaption>    m_vecLineIDCaption;
	std::map<Integer, CString> m_mapLineData;
protected:
	virtual BOOL OnInitDialog();

protected:
	virtual void ModelessOKFeedback(UINT nDialogID);
	virtual void ModelessCancleFeedback(UINT nDialogID);

protected:
	KImChampMultiIDFindPtr m_spImTasMultiIDFind;
	KSeatCapaInfoDlgPtr	   m_spSeatCapaInfo;

	CComboBox           m_cboMultiModal;
	CComboBox           m_cboBoardAlight;

	CXTPColorPicker     m_cpDefaultPColor;
	CXTPColorPicker     m_cpOutRangePColor;
	COLORREF            m_clrPDefault;
	COLORREF            m_clrPOutRange;

	CXTPColorPicker     m_cpDefaultSCColor;
	COLORREF            m_clrSCDefault;

	CXTPColorPicker     m_cpDefaultBaColor;
	CXTPColorPicker     m_cpOutRangeBaColor;
	COLORREF            m_clrBaDefault;
	COLORREF            m_clrBaOutRange;

protected:
	afx_msg void OnBnClickedCancel();
			void CloseWindowProc();
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnCbnSelchangeComboMultiModal();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedCheckLabel();
	afx_msg void OnBnClickedCheckLabel2();
	afx_msg void OnBnClickedRadioType( UINT nID );
	afx_msg void OnBnClickedButtonSCInfo();

	afx_msg void OnBnClickedButtonLineSearch();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickedEditLine();
	void SelectLineList();

private:
    std::map<KODKey, __int64> m_mapFTNodeLinkID;
};

typedef std::shared_ptr<KTransitLineThemesDlg> KTransitLineThemesDlgPtr;