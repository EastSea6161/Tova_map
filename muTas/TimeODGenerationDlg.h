/**
 * @file KTimeODGenerationDlg.h
 * @brief KTimeODGenerationDlg 선언 파일
 * @author jyk@qbicware.com
 * @date 2012.05.14
 * @remark
 */


#pragma once


#include "timeod_common.h"
#include "afxwin.h"
#include "MarkupStatic.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"

class KIOColumns;
class KIOColumn;
class KMode;
class KPurpose;
class KTarget;


/**
 * @brief Time OD Dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author jyk@qbicware.com
 * @date 2012.05.14
 */
class KTimeODGenerationDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KTimeODGenerationDlg)

public:
	KTimeODGenerationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTimeODGenerationDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8000_TimeODGenerationDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public :

	/**
	* @brief Target year를 설정한다.
	* @param[in] pTarget - Target year 정보 
	*/
	void SetTarget(KTarget* pTarget);

protected:

	/**
    * @brief Control Redraw
    */
    void KRedrawWindow();

	void InitPurposeData();

	void InitModeData();

	void InitZoneCharacterData();

	void InitTimeComboData();

	void ODTimeControl();

	void InitZoneCount();

	/**
    * @brief 'Run' 버튼 활성화 비활성화 컨트롤
    */  
	void RunButtonControl();


	bool IsEmptySurveyCodeSet();

    /**
    * @brief Argument 파일 생성
    */
	bool CreateArgument();
	
    /**
    * @brief 기존 Argument, data 파일 삭제
    */
	bool ClearFiles();

	/**
    * @brief Argument Write
    * @param[in] of - File
    */
	bool ArgumentWriteCaluateType(CStdioFileEx& of);

	/**
    * @brief Argument Write
    * @param[in] of - File
    */
	bool ArgumentWriteModeODParameter(CStdioFileEx& of);

	/**
    * @brief Argument Write
    * @param[in] of - File
    */
	bool ArgumentWriteZoneCharacter(CStdioFileEx& of);

	/**
    * @brief Argument Write
    * @param[in] of - File
    */
	bool ArgumentWritePurpose(CStdioFileEx& of);

	/**
    * @brief Argument Write
    * @param[in] of - File
    */
	bool ArgumentWriteMode(CStdioFileEx& of);

	/**
    * @brief Argument Write
    * @param[in] of - File
    */
	bool ArgumentWriteZone(CStdioFileEx& of);


	bool ReadOutPutFile(void );

public:
	
	static unsigned __stdcall TimeODThreadCaller(void* pParam);
	static void AddStatusMessage(CString a_strMsg);
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	bool        RunTimeODGeneration();
	static bool m_bRunTimeODSucceess;

protected:
	// TimeODGeneration dll
	typedef int (*FTIMEODGENERATION)(void *pParam, void* pCallback);
	FTIMEODGENERATION TimeODProcPtr;

protected :
	KTarget*            m_pTarget;
	KEMTimeODGeneration m_emTimeOD;
	int                 m_nFromTime;
	int                 m_nToTime;
	int                 m_nZoneCount;

	std::vector<TInputModeInfo>     m_vecInputModeInfo;
	std::vector<TZoneCharacterInfo> m_vecZoneCharacterInfo;
	std::vector<TSurveyPurpose>     m_vecSurveyPurpose;
	std::vector<TSurveyMode>        m_vecSurveyMode; 
	std::vector<TOutputModeInfo>    m_vecOutputModeInfo;
	KTDSurveyCodeSet                m_setTotalPurpseSurveyCode;
	KTDSurveyCodeSet                m_setTotalModeSurveyCode;

protected:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioClicked(UINT nID);
	afx_msg void OnBnClickedInput();
	afx_msg void OnBnClickedCharacterization();
	afx_msg void OnBnClickedDefinePurposeNmode();
	afx_msg void OnBnClickedOutputdesign();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedViewTripRate();
	afx_msg void OnBnClickedClose();
	
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	CButton m_btnHourlyTotal;
	CButton m_btnHourly;

	// CMarkupStatic
	CMarkupStatic m_lblTimeOD;
	CMarkupStatic m_lblSetTime;
	CMarkupStatic m_uiBackImage;

	ZArrowStatic m_uiArrow1;
	ZArrowStatic m_uiArrow2;
	ZArrowStatic m_uiArrow3;
	ZArrowStatic m_uiArrow4;
	ZArrowStatic m_uiArrow5;

	CXTPButton m_btnInput;       
	CXTPButton m_btnZoneCharacter;
	CXTPButton m_btnDefinePurposeMode;
	CXTPButton m_btnOutputDesign; 
	CXTPButton m_btnViewTable;

	CComboBox m_cboFromTime;
	CComboBox m_cboToTime;

	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

private:
	int       m_nResultCode;
};
