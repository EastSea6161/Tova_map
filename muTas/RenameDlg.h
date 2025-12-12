/**
 * @file RenameDlg.h
 * @brief KRenameDlg 선언
 * @author 
 * @date 2010.08.18
 * @remark
 */


#pragma once

enum KEMRenameType
{
	KEMRenameScenario = 0,
	KEMRenameTarget   = 2
};

/**
 * @brief 이름변경을 위해 새 이름을 입력받는 다이얼로그창
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.08.18
 */
class KRenameDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KRenameDlg)

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_2130_RenameTargetDlg };


	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	KRenameDlg(CWnd* pParent = NULL, KEMRenameType a_emRenameType = KEMRenameScenario);   // 표준 생성자입니다.
	virtual ~KRenameDlg();


	/**************************************
	* Accessors                           *
	**************************************/


	/**************************************
	* Operations                          *
	**************************************/
protected :

	/**
	* @brief 주어진 이름이 존재 하는지 여부를 반환
	* @remark 이름의 존재 여부(true-존재, false-미존재)
	*/
	bool isNameExist( CString strName );


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
public :
	CString m_strName;
	CString m_strCaption;
	std::vector< CString > m_NameArray;
	CString m_strOriginScenarioName;

private:
	KEMRenameType m_emRenameType;

	/**************************************
	* Message handlers                    *
	**************************************/
protected :
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedOk();

};
