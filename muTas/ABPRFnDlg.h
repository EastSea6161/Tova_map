#pragma once
#include "afxwin.h"
#include "model_common.h"

//#include "StdioFileEx.h"
#include "Target.h"

//^^ #include "IOColumns.h"
#include "DBaseDefaultParaHighwayAssign.h"


// KABPRFnDlg 대화 상자입니다.
#pragma pack(push, 4) 
struct TNewFixedLink
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

class KABPRFnDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KABPRFnDlg)

public:
	KABPRFnDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KABPRFnDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6720_ABPRFnDlg };

private:
	std::vector<std::vector<CString>> m_vecReportData;
	static KEMAssignmentBPRFunctionType m_emBPRFnType;

	std::map<int, std::vector<double>> m_mapEachFN;

public:
	bool WriteInputArgument(CStdioFileEx& of);

	void Target(KTarget* a_pTarget);

	void UsedDefault(bool a_bUsedDefault);

	bool ExportDB2LinkFile(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString a_strAddQuery, CString strFileName=_T("link.dat"));

	bool ExportDB2LinkExcludeType(KDBaseConPtr a_spDBaseConnection, CString a_strAddQuery, std::set<Integer> a_setIncludeLink,CString strFileName=_T("link.dat"));

	CString GetAddQuery(void)
	{
		return m_strAddQuery;
	};

	/*AssignModel Default Setting을 위한 외부제어 함수 추가.*/
	inline void	ValueOfTime(double a_dVOT)
	{
		m_dVOT = a_dVOT;
	}
	
	inline double ValueOfTime(void)
	{
		return m_dVOT;
	}

	inline void	AllLinkAlpha(double a_dAllLinkAlpha)
	{
		m_dAllLinkAlpha = a_dAllLinkAlpha;
	}

	inline double AllLinkAlpha(void)
	{
		return m_dAllLinkAlpha;
	}

	inline void	AllLinkBeta(double a_dAllLinkBeta)
	{
		m_dAllLinkBeta = a_dAllLinkBeta;
	}
	
	inline double AllLinkBeta(void)
	{
		return m_dAllLinkBeta;
	}

	void InitButtonContol();

	void SaveInterModalLinkOption(CString a_strSQL);
	CString MultiScenarioLinkQuery(KTarget* a_pRunTarget);

private:
	void InitComboBoxEachFn(void);
	void InitComboBoxEachLink(void);

	void InitReportCtrl(void);
	void UpdateReportCtrl(void);
	void UpdateReportCtrlByEachLink(void);

	void UpdateCtrlState(void);

	void GenerateLinkQuery(void);

	void DefaultVDFSetting();
	void DefaultComboBoxEachLink();
	void DefaultComboBoxEachFn();
	void DefaultVDFReportCtrl();
	void DefaultRadioButton();

	void ConvertSysToDispColumn(std::vector<CString>& a_vecColName);

private:
	std::map<int, KVDFPara>						m_mapDefaultVDFPara;
	std::map<int, KODTime>						m_mapDefaultODTime;
	std::map<int, std::vector<KVDFFunction>>	m_mapDefaultVDFFuntion;
	std::map<CString, int>						m_mapDefaultVDFFuntionGroup;

private:

	bool		m_bUsedDefault;
	double		m_dVOT;
	double		m_dAllLinkAlpha;
	double		m_dAllLinkBeta;

	ZNumberEdit m_edtVOT;

	ZNumberEdit m_edtAllAlpha;
	ZNumberEdit m_edtAllBeta;

	CComboBox	m_comboFn;
	CComboBox	m_comboLinkAlpha;
	CComboBox	m_comboLinkBeta;
	CComboBox	m_comboLinkA;
	CComboBox	m_comboLinkB;

	int         m_nEachFnCombo;
	int			m_nComboFnCursor;
	int			m_nComboLinkAllAlphaCursor;
	int			m_nComboLinkAllBetaCursor;
	int			m_nComboLinkAllACursor;
	int			m_nComboLinkAllBCursor;
	CString     m_strUseFunctionGroup;

	KTarget*	m_pTarget;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	CString		m_strAddQuery;
	int			m_nFnGroupID;

	CString m_strCaptionAlpha;
	CString m_strCaptionBeta;
	CString m_strCaptionA;
	CString m_strCaptionB;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSetBPRFunction();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnRadioClicked(UINT nID);
	afx_msg void OnBnClickedDefault();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnStnClickedSeprator3();

};
