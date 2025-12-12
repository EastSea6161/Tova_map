#pragma once
#include "afxwin.h"
#include "GravityTreeCtrl.h"
#include "afxcmn.h"
#include "model_common.h"

struct STGravityCaliInfo
{
	bool				bSelected; 
	KEMGravityFunction	GravityFunction;
	CString				strPurposeName;
	//CString            strVariableName;
	int					nVariableIndex;
	int					nGrade;
	double				dUpperBound;
	double				dLowerBound;
	double				dToleranceError;
};

/*forward declare*/
class KTarget;
class KGravityCailInfo;
class KIOColumns;

const CString FILE_NAME_OUT_PURPOSE_OD	 = _T("purpose_od.dat");
const CString FILE_NAME_OUT_PARAMETER_OD = _T("parameter_od.dat");

// KGravityCalibrationDlg 대화 상자입니다.

class KGravityCalibrationDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KGravityCalibrationDlg)

public:
	KGravityCalibrationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGravityCalibrationDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5540_GravityCalibrationDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:

	KTarget*        m_pTarget;
	HICON           m_hICon;
	CTreeCtrl       m_Tree;

	int             m_nIntervalGrade;
	double          m_dIntervalError;

	ZNumberEdit		m_edtIntervalError;

	double          m_dEntropyUBound;
	double          m_dEntropyLBound;

	ZNumberEdit     m_edtEntropyUBound;
	ZNumberEdit     m_edtEntropyLBound;
	
	//라디오 버튼
	int             m_nBtnRadioIndex;
	
	//콤보박스
	CComboBox       m_cboIntervalVariable;
	CComboBox       m_cboEntropyVariable;

	//이미지 데이터
	CImageList      m_ImgTreeList;
	CImageList      m_ImgComboFunctionList;

	KEMDistributionFunction m_emFunctionType;
	std::map<HTREEITEM, STGravityCaliInfo> m_mapGravityInfo;

	int				m_nSelPurposeCount;

private:
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);

public:
	void SetTarget(KTarget* a_pTaget);
	
protected:
	void InsertTreeItemPurpose();

	void InitTreeCtrl();

	void CheckChildItems( HTREEITEM a_hItem );

	void UnCheckedChildItems( HTREEITEM a_hItem );

	void CheckParentsItems( HTREEITEM a_hItem );

	void InitVariableCombo();

	void GetTreeValue(STGravityCaliInfo a_oGavityInfo);

	bool SaveSelectedItem(HTREEITEM a_hItem ,STGravityCaliInfo& a_oGavityInfo);

	void UpdateControlState();

	void ClearComboControl();

	void ValidateInput(void);

	void RunCalibration(void);

	void ViewResultTxt(void);

	void CreateArgument(void);

	CString GetVariableName(int a_nComboIndex, KEMGravityFunction a_eumGravityFuncton);

	static unsigned __stdcall GravityCalibrationThreadCaller(void* p);

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangingTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadio(UINT nID);
	afx_msg void OnBnClickedMatrixRadio(UINT nID);
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeInterVariableCombo();
	afx_msg void OnCbnSelchangeImpedVariavleCombo();
	afx_msg void OnCbnSelchangeEntropyVariableCombo();

private:
	int  m_nResultCode;
public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	static void AddStatusMessage(CString a_strMsg);
};
