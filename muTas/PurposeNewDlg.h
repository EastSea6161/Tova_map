/**
 * @file PurposeNewDlg.h
 * @brief KPurposeNewDlg 선언 파일
 * @author jyk@qbicware.com
 * @date 2012.01.02
 * @remark
 */


#pragma once



/**
* @brief 새로운 Purpose을 생성하는 Dialog class
* @remark 
* @warning 
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.01.02
*/
class KPurposeNewDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KPurposeNewDlg)

public:
	KPurposeNewDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	virtual ~KPurposeNewDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_1120_AddPurposeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

public:
	CString m_strPurposeName;
};
