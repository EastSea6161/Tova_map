/**
 * @file DImpedanceFunctionDlg.h
 * @brief KDImpedanceFunctionDlg 선언 파일
 * @author 
 * @date 2011.05.25
 * @remark
 */


#pragma once

#include "DFormulaDefaultDlg.h"
#include "DBaseDefaultDistribution.h"

/* Forward declarations */
class KDistributionInfo;
class KDistributionFunctionBase;



/**
* @brief KSTImpedanceElement Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.28
*/
struct KSTImpedanceElement
{
	KDistributionInfo* pInfo;
	KDistributionFunctionBase* pFunction;
};


class KTarget;

/**
* @brief KSTParaColumn Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.28
*/
class KSTParaColumn
{
public:
    CString ColumnName;
    CString ColumnCaption;
};

/**
 * @brief 분포모형에서 Impedance function을 설정하는 dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.25
 */
class KDImpedanceFunctionDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDImpedanceFunctionDlg)

public:
	KDImpedanceFunctionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDImpedanceFunctionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6320_DImpedanceDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :
	/**
	 * @brief 분포 모형에서 Output을 설정 할 목적/수단 정보를 추가한다.
	 * @param[in] pInfo - 분포 모델의 목적/수단 정보
	 */
	void AddDistributionInfo(KDistributionInfo* pInfo);

	/**
	* @brief Parameter OD Column list를 추가한다.
	* @param[in] paraODColumnNames - Parameter OD Column list
	*/
    void AddParaODColumns(std::list<KSTParaColumn>& paraODColumnNames );

	/**
	 * @brief 분포 모형의 목적/수단 정보를 주어진 list로 설정한다.
	 * @param[in] infoList - 분포 모형의 목적/수단 정보 list
	 */
	void SetDistributionInfo(std::list<KDistributionInfo*>& infoList);

	void SetTarget(KTarget* a_pTarget);

	void SetDefaultparameter(std::map<int, KDistributionParaInfo> a_mapParaInfo);

	std::map<int, KDistributionParaInfo>	m_mapDefaultParameter;
protected :

	KTarget* m_pTarget;
	CListBox m_lstDistInfo;
	CComboBox m_cmbFunction;
	CComboBox m_cmbVariable;
	CEdit m_edtParameter1;
	CEdit m_edtParameter2;
	CEdit m_edtParameter3;


	// Impedance Function은 모형선택에서 Gravity가 선택되었을 경우 활성화 
	// Gravity는 OD by Purpose에서만 선택 가능
	std::list<KSTImpedanceElement*> m_ElementList;

    /**
    * @brief List Control Update
    */
	void updateListControl(void);

    /**
    * @brief Variable Control Update
    */
    void updateVariableControl();

    /**
    * @brief Function Set
    */
	void updateSetFunction(void);

    /**
    * @brief Function Get
    */
    void updateGetFunction(void);

    /**
    * @brief Clear Element
    */
	void clearElementList(void);

    /**
    * @brief Function 복제
    * @param[in] pFunction - Original Function
    */
	KDistributionFunctionBase* createAndCopyFunction(KDistributionFunctionBase* pFunction);

    /**
    * @brief check the empty input filed
    */
	bool IsEmptyValue(KDistributionFunctionBase* pFunctionNew);

	void UpdataDefFunction();

private: 
    int m_nCurSelected;
    std::list<KSTParaColumn> m_paraODColumnNames;
    int ParaODColumnIndex(CString strColumnName);
	int ParaODColumnIndexToCaption(CString strColumnCaption, CString& ar_strColumnName);
    CString ParaODColumnName(int index);
	KDFormulaDefaultDlgPtr   m_spKDFormulaDefaultDlg;



public:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombo1();
    afx_msg void OnLbnSelchangeList1();
    afx_msg void OnLbnSelcancelList1();
	afx_msg void OnBnClickedBtnDefaultFunction();
};
