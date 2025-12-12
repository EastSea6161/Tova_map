#pragma once
#include "afxwin.h"


class KTarget;
class KIOTable;
class KIOColumn;


// KCalculateZoneDistance 대화 상자입니다.

class KCalculateZoneDistanceDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KCalculateZoneDistanceDlg)

public:
	KCalculateZoneDistanceDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCalculateZoneDistanceDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4305_CalculateZoneDistanceDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:

	KTarget*								m_pTarget;
	KIOTable*								m_pTable;
	KIOColumn*                              m_pColumn;

	std::vector<KIOColumn*>					m_vecParameterDoubleColumn;
	std::set<Integer>						m_setZoneNode;
	std::map<Integer, Integer>				m_mapZoneNodeInfo;
	std::map<KODKey, KSDoubleRecord*>		m_mapResultRecord;
	std::map<Integer, KNodeArrivedMinLink*> m_mapRoadOriginMinLink;
	KLinkCollection          m_oRoadColLink;



public:
	void SetInfo(KTarget* a_pTarget, KIOTable* a_pTable);


protected:
	virtual BOOL OnInitDialog();

	void GetParameterDolubleColumn();

	void initColumnList();

	void InsertComboBoxValue();

	KIOColumn* findDoubleIOColumnInfo(CString a_strColumnName);

	bool GetZoneNodeInfo();

	bool GetRoadLinkData();

	void PathFindAllZoneInfo();

	void GetPathFindDistanceResult(Integer a_nxStartNodeID, Integer a_StartZoneID);

	void ClearInfoData();

	void CalculatePathFind();


	static unsigned __stdcall CalaculatePathThreadCaller(void* p);

	static unsigned __stdcall ClearPathThreadCaller(void* p);


protected:

	CComboBox m_cboZoneDistance;


protected:

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
