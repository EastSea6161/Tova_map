/**
 * @file SelectTargetYearDlg.h
 * @brief KSelectTargetYearDlg 선언
 * @author 
 * @date 2010.07.30
 * @remark
 */


#pragma once



/**
 * @brief Base의 Target year를 입력받는 Dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.07.30
 */
class KSelectTargetYearDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KSelectTargetYearDlg)
	
	/**************************************
	* Constructors and Destructor         *
	**************************************/
public:
	KSelectTargetYearDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~KSelectTargetYearDlg();

	// Dialog Data
	enum { IDD = IDD_1110_PrjTargetYearDlg };


	/**************************************
	* Accessors                           *
	**************************************/


	/**************************************
	* Operations                          *
	**************************************/
public :

	/**
	* @brief 선택된 Year를 추가한다.
	* @param[in] strYear - Year
	*/
	void addCheckedYear( const CString& strYear );

	/**
	* @brief Target Year를 설정한다.
	* @param[in] nYear - Target year
	*/
	void addTargetYear( int nYear );


protected :
	void updateTargetYearEdit(void);


	/**************************************
	* Overrides                           *
	**************************************/
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();


	/**************************************
	* Properties and Variables            *
	**************************************/
public :
	int m_nBaseYear;
	CButton m_btnYear01;
	CButton m_btnYear02;
	CButton m_btnYear03;
	CButton m_btnYear04;
	CButton m_btnYear05;
	CButton m_btnYear06;
	CButton m_btnYear07;
	CButton m_btnYear08;
	CButton m_btnYear09;
	CButton m_btnYear10;
	CButton m_btnYear11;
	CButton m_btnYear12;
	CButton m_btnYear13;
	CButton m_btnYear14;
	CButton m_btnYear15;
	CButton m_btnYear16;
	CButton m_btnYear17;
	CButton m_btnYear18;
	CButton m_btnYear19;
	CButton m_btnYear20;
	CButton m_btnYear21;
	CButton m_btnYear22;
	CButton m_btnYear23;
	CButton m_btnYear24;
	CButton m_btnYear25;
	CButton m_btnYear26;
	CButton m_btnYear27;
	CButton m_btnYear28;
	CButton m_btnYear29;
	CButton m_btnYear30;
	CButton m_btnYear31;
	CButton m_btnYear32;
	CButton m_btnYear33;
	CButton m_btnYear34;
	CButton m_btnYear35;
	CString m_strSelectYear;

protected :
	CImageList m_ilButtonImage;
	CButton m_btnPrev;
	CButton m_btnNext;
	std::vector<CButton*> m_arryButtons;
	std::vector<int> m_arrCheckedYear;
	std::map<CString, CString> m_mapCheckedYear;
	std::set< int > m_TargetYearSet;

	/**************************************
	* Message handlers                    *
	**************************************/
protected : 
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedTybtnPervyear();
	afx_msg void OnBnClickedTybtnNextyear();
	afx_msg void OnCheckYearRange(UINT nID);
};
