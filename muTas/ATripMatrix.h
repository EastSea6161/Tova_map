#pragma once

#include "model_common.h"
#include "DBaseDefaultParaHighwayAssign.h"

class KTarget;
class KIOColumn;
class KAInputInfo;

class KATripMatrix : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KATripMatrix)

public:
	KATripMatrix(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KATripMatrix();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6700_ATripMatrix };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
		/**
	* @brief 배정모형의 OD 종류를 반환한다.
	* @return KEMAssignmentODType - 배정모형 OD 종류
	*/
	KEMAssignmentODType AssignmentODType(void);

	/**
	* @brief 배정모형의 OD 종류를 설정한다.
	* @param[in] emODType - 배정모형의 OD 종류
	*/
	void AssignmentODType(KEMAssignmentODType emODType);

		/**
	* @brief 분석 시간을 반환한다.
	* @return double - 분석 시간
	*/
    double  AnalysisTime();

	/**
	* @brief 분석 시간을 설정한다.
	* @param[in] nTime - 분석 시간
	*/
    void AnalysisTime(double dTime);

	void SetModel(KEMAssignmentModel a_emAModel);

	std::list<KAInputInfo*> InputInfo();

protected:
	KTarget*		   m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndModeReport;
	bool	           m_bUseDefault;
	KEMAssignmentModel m_emAModel;
	ZNumberEdit		   m_edtAnalysisTime;
	double			   m_dAnalysisTime;
	KEMAssignmentODType m_emODType;

	std::map<int, std::vector<KOccupancyPCEMode>> m_mapDefaultOccupancyPCEMode;
	std::map<int, std::vector<KOccupancyPCEMode>> m_mapGetDefaultModeInfo;
	std::map<int, std::vector<CString>> m_mapModeChocieColumns;
	std::map<int, CString> m_mapGetModeIDModeName;
	std::map<int, CString> m_mapGetModeinfo;
	std::map<int, KODTime> m_mapDefaultODTime;
	std::list<KAInputInfo*> m_lstInputInfo;

public:
	virtual BOOL OnInitDialog();
	void InputList(std::list<KAInputInfo*>& lstInpuInfo);
	static void GetDefaultModeInfo(int a_nMasterCode, std::map<int, std::vector<KOccupancyPCEMode>>& a_mapGetDefaultModeInfo);

protected:
	void InitModeReportHeader();
	void ModeReportRecord();
	void FModeReportRecord();
	void ModeSetting();

	void UpdateModeReportRecord();

	void ApplyInputInfo();
	bool ValidateCheck();

	void DefaultModeReportRecord(int a_nMasterCode);
	void DefaultModeSetting(int a_nMasterCode);
	void DefaultFModeReportRecord();

	void ResizeComponent();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedDefault();
	afx_msg void OnReportCheckItemMode(NMHDR* pNotifyStruct, LRESULT* pResult);
};
