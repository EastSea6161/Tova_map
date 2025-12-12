/**
 * @file CodeNewDlg.h
 * @brief KCodeNewDlg 선언 파일
 * @author 
 * @date 2011.07.11
 * @remark
 */


#pragma once


/**
* @brief 새로운 Code를 생성하는 Dialog class
* @remark 
* @warning 
* @version 1.0
* @author 
* @date 2011.08.23
*/
class KCodeNewDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KCodeNewDlg)

public:
	KCodeNewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCodeNewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4230_AddCodeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public :
	int m_nCode;
	CString m_strValue;


public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
