
/**
* @brief 각 단계 모형의 마지막 결과를 저장하는 필드 네임을 변경하는 Dialog class
* @remark 
* @warning 
* @version 1.0
* @author jin835@qbicware.com
* @date 2012.01.06
*/

#pragma once


#include "resource.h"


// KGChangeFieldName 대화 상자입니다.

class KGChangeFieldName : public KDialogEx
{
	DECLARE_DYNAMIC(KGChangeFieldName)

public:
	KGChangeFieldName(CString a_strCurrentName,CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGChangeFieldName();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6191_ChangeFieldName };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CString m_strCurrentName;
	CString m_strNewName;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
