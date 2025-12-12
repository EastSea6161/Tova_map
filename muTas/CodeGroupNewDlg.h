/**
 * @file CodeGroupNewDlg.h
 * @brief KCodeGroupNewDlg 선언 파일
 * @author 
 * @date 2011.07.08
 * @remark
 */


#pragma once



/**
* @brief 새로운 Code group을 생성하는 Dialog class
* @remark 
* @warning 
* @version 1.0
* @author 
* @date 2011.08.23
*/
class KCodeGroupNewDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KCodeGroupNewDlg)

public:
	KCodeGroupNewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCodeGroupNewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4220_AddCodeGroupDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	
public:
	int m_nGroupKey;
	CString m_strGroupName;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
