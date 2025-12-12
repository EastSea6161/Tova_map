/**
 * @file IntersectionBasicPage.h
 * @brief KIntersectionBasicPage 선언 파일
 * @author jyk@qbicware.com
 * @date 2012.06.07
 * @remark
 */
#pragma once
#include "intersection_common.h"
#include "afxwin.h"
#include "afxbutton.h"
//^#include "ResizeDialogEx.h"

// KIntersectionBasicPage 대화 상자입니다.

/**
 * @brief Intersection Basic 설정 page class
 * @remark
 * @warning
 * @version 1.0
 * @author jyk@qbicware.com
 * @date 2012.06.07
 */
class KIntersectionBasicPage : public KDialogEx
{
	DECLARE_DYNAMIC(KIntersectionBasicPage)

public:
	KIntersectionBasicPage(KDBaseConPtr spDBaseConnection, TIntersectionBase &a_oTSelectIntersection, std::map<Integer, TBoundInfo> &a_mapBoundInfo, CWnd* pParent = NULL);
	virtual ~KIntersectionBasicPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5920_IntersectionBasicPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetParentSheet(CXTPPropertySheet* a_pParentSheet)
	{
		m_pParentSheet = a_pParentSheet;
	}


	void SetIntersection(TIntersectionBase &a_oTSelectIntersection)
	{
		m_oTSelectIntersection = a_oTSelectIntersection;
	}


	void SetLaneBoundInfo(std::map<Integer, TBoundInfo> &a_mapBoundInfo)
	{
		m_mapBoundInfo = a_mapBoundInfo;
	}

protected:
	/**
	* @brief Report Control Header 초기화
	*/
	void InitReportHeader();

	void InitReportControl();

	void SetDirectionSeq(int a_nSignalLampType, bool a_bRedraw = true);

	void SetDirectionSeq(int a_nSignalLampType, int a_nIndex, bool a_bRedraw = true);

	void RegisterDirectionImgID();

	int  GetSignalIndication();

	void SetGreenLampTime(int a_nSeq);

	void SetYellowLampTime(int a_nSeq);

	bool UpsertIntersectionSignalIndication();

	bool UpsertIntersectionInfo(std::map<int, TIntersectionInfo> &a_mapIntersectionInfo);

	int  GetIntersectionInfo();

	double GetValue( KEMBoundDirectionCode emDirectionCode, size_t a_nVariable );

	void   SetValue( std::map<int, TIntersectionInfo> &a_mapIntersectionInfo, KEMBoundDirectionCode emDirectionCode,  int a_nRowIndex, int a_nValue );
	
public:
	static void DefaultModuleLaneBoundDataSet(TIntersectionBase &a_oTSelectIntersection, std::map<Integer, TBoundInfo> &a_mapBoundInfo, std::map<int, TIntersectionInfo> &a_mapIntersectionInfo);
protected:
	static int  GetBoundLaneCnt(std::map<Integer, TBoundInfo> &a_mapBoundInfo, Integer a_nxBoundNodeID);
public:
	static void DefaultModuleDataSet(std::vector<TSignalIndication> &a_vecSignalSeq, std::map<int, TIntersectionInfo> &a_mapIntersectionInfo);

public:
	BOOL SaveBasicFour();

protected :	
	KDBaseConPtr m_spDBaseConnection;

	CXTPPropertySheet*               m_pParentSheet;
	TIntersectionBase                m_oTSelectIntersection;
	std::map<int, TIntersectionInfo> m_mapIntersectionInfo;
	std::map<Integer, TBoundInfo>    m_mapBoundInfo;
	CXTPReportControl                m_wndReportCtrl;
	CBrush                           m_backBrush;
	CBrush                           m_backBrush2;

protected:
	/**
	* @brief 다이얼로그 초기화
	*/
	virtual BOOL OnInitDialog();
	//virtual BOOL OnApply();

	afx_msg HBRUSH OnCtlColor  (CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL   OnEraseBkgnd(CDC* pDC);

protected:
	
	int                            m_nRadioSignal;
	std::vector<TSignalIndication> m_vecSignalSeq;

	CMFCButton               m_btnSignalSeq1;
	CMFCButton               m_btnSignalSeq2;
	CMFCButton               m_btnSignalSeq3;
	CMFCButton               m_btnSignalSeq4;
	CMFCButton               m_btnSignalSeq5;
	CMFCButton               m_btnSignalSeq6;

	std::vector<CMFCButton*>       m_vecSignalBtnCtrl;
	std::map<int, TDirectionImgID> m_mapDirectionImgID;

	std::vector<int>         m_vecLampGreenCtrlID;
	std::vector<int>         m_vecLampYellowCtrlID;

protected:
	afx_msg void OnBnClickedBtnArrow01();
	afx_msg void OnBnClickedBtnArrow02();
	afx_msg void OnBnClickedBtnArrow03();
	afx_msg void OnBnClickedBtnArrow04();
	afx_msg void OnBnClickedBtnArrow05();
	afx_msg void OnBnClickedBtnArrow06();
	afx_msg void OnBnClickedBtnArrow07();
	afx_msg void OnBnClickedBtnArrow08();
	afx_msg void OnBnClickedBtnArrow09();
	afx_msg void OnBnClickedBtnArrow10();
	afx_msg void OnBnClickedBtnArrow11();
	afx_msg void OnBnClickedBtnArrow12();
	afx_msg void OnBnClickedBtnArrow13();
	afx_msg void OnBnClickedBtnArrow14();
	afx_msg void OnBnClickedBtnArrow15();
	afx_msg void OnBnClickedBtnArrow16();
	afx_msg void OnBnClickedBtnArrow17();
	afx_msg void OnBnClickedBtnArrow18();

	afx_msg void OnBnClickedBtnSetOne();
	afx_msg void OnBnClickedBtnSetTwo();
	afx_msg void OnBnClickedBtnAllRed();
	
	afx_msg void OnBnClickedRadioSignalSeqOne();
	afx_msg void OnBnClickedRadioSignalSeqTwo();
	afx_msg void OnBnClickedRadioSignalSeqThree();
	afx_msg void OnBnClickedRadioSignalSeqFour();
	afx_msg void OnBnClickedRadioSignalSeqFive();
	afx_msg void OnBnClickedRadioSignalSeqSix();

	afx_msg void OnBnClickedMfcbuttonSigseqOne();
	afx_msg void OnBnClickedMfcbuttonSigseqTwo();
	afx_msg void OnBnClickedMfcbuttonSigseqThree();
	afx_msg void OnBnClickedMfcbuttonSigseqFour();
	afx_msg void OnBnClickedMfcbuttonSigseqFive();
	afx_msg void OnBnClickedMfcbuttonSigseqSix();
	
	afx_msg void OnBnDoubleclickedMfcbuttonSigseqOne();
	afx_msg void OnBnDoubleclickedMfcbuttonSigseqTwo();
	afx_msg void OnBnDoubleclickedMfcbuttonSigseqThree();
	afx_msg void OnBnDoubleclickedMfcbuttonSigseqFour();
	afx_msg void OnBnDoubleclickedMfcbuttonSigseqFive();
	afx_msg void OnBnDoubleclickedMfcbuttonSigseqSix();

	afx_msg void OnEnChangeEditLampGreen1();
	afx_msg void OnEnChangeEditLampGreen2();
	afx_msg void OnEnChangeEditLampGreen3();
	afx_msg void OnEnChangeEditLampGreen4();
	afx_msg void OnEnChangeEditLampGreen5();
	afx_msg void OnEnChangeEditLampGreen6();

	afx_msg void OnEnChangeEditLampYellow1();
	afx_msg void OnEnChangeEditLampYellow2();
	afx_msg void OnEnChangeEditLampYellow3();
	afx_msg void OnEnChangeEditLampYellow4();
	afx_msg void OnEnChangeEditLampYellow5();
	afx_msg void OnEnChangeEditLampYellow6();
	
};
