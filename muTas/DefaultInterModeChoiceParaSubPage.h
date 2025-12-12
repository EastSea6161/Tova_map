#pragma once
#include "DBaseDefaultParaInterUrbanModeChoice.h"

struct TUrbanChoiceValues;


// KDefaultInterModeChoiceParaSubPage 대화 상자입니다.

class KDefaultInterModeChoiceParaSubPage : public KDialogEx
{
	DECLARE_DYNAMIC(KDefaultInterModeChoiceParaSubPage)

public:
	KDefaultInterModeChoiceParaSubPage(int a_nPageID, CString& strObjName, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultInterModeChoiceParaSubPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8511_Default_Inter_ModeChoiceParaSubPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void         SetParameterData   (std::map<int, TUrbanChoiceValues> a_mapInterModeChoicePara);
	void         GetNewParameterData(std::map<int, TUrbanChoiceValues> &a_mapNewInterModeChoicePara);
public:
	int GetPageID()
	{
		return m_nPageID;
	}

	CString GetObjectName() { return m_strObjName; }

protected:
	void         InitReportHeader(void);

public:
	void         UpdateReportData(void);
	void         UpdateReportData(std::map<int, TUrbanChoiceValues> a_mapInterModeChoicePara); //Paste시 사용

protected:
	std::map<int, TUrbanChoiceValues> m_mapInterModeChoicePara;
private:
	int          m_nPageID;
	CString		 m_strObjName;	// 목적 명

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;
};

typedef std::shared_ptr<KDefaultInterModeChoiceParaSubPage> KDefaultInterModeChoiceParaSubPagePtr;