/**
 * @file TargetCopyDlg.h
 * @brief KTargetCopyDlg 선언
 * @author 
 * @date 2010.09.07
 * @remark
 */


#pragma once
#include "afxcmn.h"


/* forward declarations */
class KScenario;
class KTarget;


/**
 * @brief KTarget의 데이터파일을 다른 KTarget으로 복사하기 위한 다이얼로그
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.09.07
 */
class KTargetCopyDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KTargetCopyDlg)

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_2150_CopyYearScenarioDlg };


	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	KTargetCopyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTargetCopyDlg();


	/**************************************
	* Accessors                           *
	**************************************/
public :

	/**
	* @brief 복사할 Target year를 설정한다.
	* @param[in] pSoruceTarget - Target year
	*/
	inline void SetSourceTarget( KTarget* pSourceTarget )
	{
		m_pSourceTarget = pSourceTarget;
	}


	/**************************************
	* Operations                          *
	**************************************/
protected :
	void addScenarioToTree( KScenario* pScenario );
	void addTargetToTree( HTREEITEM hScenario, KTarget* pTarget );
	bool CopyTarget( void );



	/**************************************
	* Overrides                           *
	**************************************/
public :
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	/**************************************
	* Properties / Variables              *
	**************************************/
public:
	ZTriStateTreeCtrl m_treeTarget;
	bool              m_bCopyResult;

protected :
	KTarget* m_pSourceTarget;
	CImageList m_ilTreeState;


	/**************************************
	* Message handlers                    *
	**************************************/
protected :
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
