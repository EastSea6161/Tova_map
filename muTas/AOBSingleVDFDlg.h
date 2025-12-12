
#include "afxwin.h"
#include "model_common.h"
#include "AOBVDFInfo.h"
#include "DBaseDefaultParaHighwayAssign.h"
class KTarget;

#pragma pack(push, 4) 
struct TSingeFixedLink
{
	Integer link_id;
	Integer fnode_id;
	Integer tnode_id;
	int     link_type;
	double  length;
	int     lane;
	double	capa;
	double  speed;
	int		fuNumber;
	double	alpha;
	double	beta;
	double	a;
	double	b;
};
#pragma pack(pop)

#pragma once

// KAOBSingleVDFDlg 대화 상자입니다.

class KAOBSingleVDFDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KAOBSingleVDFDlg)

public:
	KAOBSingleVDFDlg(KTarget* a_pTarget,CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAOBSingleVDFDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6721_AOBSingleVDFDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	bool WriteInputArgument(CStdioFileEx& of);

	bool ExportDB2LinkFile(KDBaseConPtr a_spDBaseConnection);

	void GenerateLinkQuery(void);

	bool GetOBSingleInfo(TAOBSingleVDF& a_TAOBSingeVDF);

	void SetOBSingleInfo(TAOBSingleVDF& a_TAOBSingeVDF);

	void InitButtonControl(void);

	void SetDefaultSetting(void);

	void UsedDefalut(bool a_bUsedDefalut);

protected:

	void SetDefaultRadioButton(void);

	void SetDefaultValueofTime(void);

	void SetDefaultAllLink(void);

	void SetDefaultEachLink(void);

	void SetDefaultEachFn(void);

protected:

	bool		m_bUsedDefault;

	double		m_dVOT;
	double		m_dAllLinkAlpha;
	double		m_dAllLinkBeta;


	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	KTarget*		  m_pTarget;

	std::vector<std::vector<CString>> m_vecReportData;
	KEMAssignmentBPRFunctionType m_emBPRFnType;

	std::map<int, std::vector<double>> m_mapEachFN;

	int         m_nEachFnCombo;
	int			m_nComboFnCursor;
	int			m_nComboLinkAllAlphaCursor;
	int			m_nComboLinkAllBetaCursor;
	int			m_nComboLinkAllACursor;
	int			m_nComboLinkAllBCursor;
	int			m_nFnGroupID;
	int			m_nRadiobtnFunction;

	ZNumberEdit m_edtVOT;
	ZNumberEdit m_edtAllAlpha;
	ZNumberEdit m_edtAllBeta;

	std::map<int, KVDFPara>						m_mapDefaultVDFPara;
	std::map<int, KODTime>						m_mapDefaultODTime;
	std::map<int, std::vector<KVDFFunction>>	m_mapDefaultVDFFuntion;
	std::map<CString, int>						m_mapDefaultVDFFuntionGroup;

protected:

	void InitReportCtrl(void);
	void UpdataReportCtrl(void);
	void UpdateReportCtrlByEachLink(void);

	void InitComboBoxEachFn(void);
	void InitComboBoxEachLink(void);

	void SetRadioButton();

	CString GetColumnName(CString a_strColumnCaption);

	CString GetColumnCaption(CString a_strColumnName);

	void SetComboBoxInfo(TAOBSingleVDF& a_TAOBSingeVDF);

	void ConvertSysToDispColumn(std::vector<CString>& a_vecColName);

protected:

	CComboBox m_comboLinkAlpha;
	CComboBox m_comboLinkBeta;
	CComboBox m_comboLinkA;
	CComboBox m_comboLinkB;
	CComboBox m_comboFn;

public:
	afx_msg void OnBnClickedRadio(UINT nID);
	afx_msg void OnBnClickedButtonSetbprfn();
	afx_msg void OnCbnSelchangeCombo1();
	
};
