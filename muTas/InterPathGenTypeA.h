#pragma once
//^#include "ResizeDialogEx.h"
#include "DBaseInterModal.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"

class KTarget;
// KInterPathGenTypeA 대화 상자입니다.

class KInterPathGenTypeA : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterPathGenTypeA)

public:
	KInterPathGenTypeA(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterPathGenTypeA();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6840_INTERMODALPATHGEN_A };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetTarget(KTarget* a_pTarget);


protected:
	KTarget* m_pTarget;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlOD;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlUrban;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlRegional;

protected:

	void InitReportHeaderOD(void);
	void InitReportHeaderUrban(void);
	void InitReportHeaderRegional(void);

	void UpdateReportDataOD(void);
	void UpdateReportDataUrban(void);
	void UpdateReportDataRegion(void);

	void InitUrbanModeData(std::map<int, TUrbanPathInfo>& a_mapUrbanMode);
	void InitRegionModeData(std::map<int, TRegionPathInfo>& a_mapRegionMode);

public:
	void ArgumentPurPoseOD(KIOColumns& oPurposeColumnCollection, CStdioFileEx& of);
	void ArgumentUrbanPath(CStdioFileEx& of);
	void ArgumentRegionalPath(CStdioFileEx& of);

	void SaveResultOD2DB(void);
	void SaveResultUrban2DB(void);
	void SaveResultRegion2DB(void);

private:
    virtual BOOL OnInitDialog();
};
