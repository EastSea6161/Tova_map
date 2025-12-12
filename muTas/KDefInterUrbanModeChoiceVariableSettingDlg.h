#pragma once

#include "DefInterUrbanModeChoiceParaTypes.h"


class KScenario;
class KTarget;
class KIOColumn;





// KDefInterUrbanModeChoiceVariableSettingDlg 대화 상자입니다.

class KDefInterUrbanModeChoiceVariableSettingDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefInterUrbanModeChoiceVariableSettingDlg)

public:
	KDefInterUrbanModeChoiceVariableSettingDlg(KTarget* pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefInterUrbanModeChoiceVariableSettingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_8511_Default_Inter_ModeChoiceVariableSettingDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


public: 
	void SetDescriptionVariableInfo(std::map<int, TDescriptionVariableInfo>& mapVariableInfo);
	void SetZoneVariableInfo(std::map<int, TZoneVariableInfo>& mapZoneVariableInfo);
	void SetUserZoneColumnInfo(std::map<int, TZoneUserColumnInfo>& mapZoneColumn);
	std::vector<TDescriptionVariableInfo> GetSelectedVariables();
	std::vector<TZoneVariableInfo> GetSelectedZoneVariables();
	
protected:
	KTarget* m_pTarget;
	std::map<int, TDescriptionVariableInfo> m_mapVariableInfo;
	std::map<int, TZoneVariableInfo> m_mapZoneVariableInfo;

	struct STIOColumnWrapper
	{
		KIOColumn* pColumn;
		bool bSelected;
		STIOColumnWrapper() : pColumn(NULL), bSelected(false) {};
	};

	std::map<CString, STIOColumnWrapper> m_mapUserZoneColumn;

	STIOColumnWrapper* m_pLastSelectedCW;


	//std::map<int, KIOColumn*> m_mapUserZoneColumn;
	//std::map<CString, KIOColumn*> m_mapSelectableZoneColumn;

	//std::map<int, TZoneUserColumnInfo> m_mapUserZoneColumnInfo;
	//std::map<CString, TZoneUserColumnInfo> m_mapSelectableZoneColumn;


	std::vector<TDescriptionVariableInfo> m_arrSelectedVariable;
	std::vector<TZoneVariableInfo> m_arrSelectedZoneVariable;
	

	// 사용안함
	//struct TGenerationModelZoneColumnInfo
	//{
	//	int nObjGroupCode;
	//	int nDetailObjId;
	//	CString strPAFlag;
	//	CString strColumnName;
	//	CString strStartDate;
	//	CString strEndDate;
	//	int nRunSeq;
	//	CString strFixedFieldYN;
	//	int nModelCode;
	//	CString strZoneGroupYN;
	//	int nZoneGroupStructureId;
	//	int nGeneralGenerationMethodId;
	//};

	//std::vector<TGenerationModelZoneColumnInfo> m_arrGenModelZoneColumn;


	void ResizeComponet();
	void InitReport1();
	void InitReport2();
	void UpdateReport1();
	void UpdateReport2();

	void InitUserZoneColumn();

	void UpdateZoneVarName(CXTPReportRecord* pRecord);

	/**
	* @brief 리포트 컨르롤에서 선택된 설명변수 항목을 읽어온다.
	*/
	void MakeSelectedVariables();

	/**
	* @brief 리포트 컨르롤에서 선택된 존특성변수 항목을 읽어온다.
	*/
	void MakeSelectedZoneVariables();


	///**
	//* @brief 현재 Target의 사용자 정의 존 컬럼 정보에서 컬럼명으로 ID를 검색
	//* @param str 검색할 컬럼의 컬럼 명
	//* @return int 검색 결과 컬럼의 ID, 검색 실패 시 -1 반환
	//*/
	//int GetZoneColumnId(CString& str);


	///**
	//* @brief 현재 선택가능한 사용자 컬럼 목록을 초기화
	//*/
	//void UpdateSelectableColumnMap();


	/**
	* @brief 존특성변수 체크 가능 상태를 갱신
	* 선택 가능한 컬럼이 모두 선택되면 나머지 체크되지 않은 컬럼변수 항목의 체크박스를 비활성화 한다
	*/
	void UpdateReport2CheckboxState();

	DECLARE_MESSAGE_MAP()

protected:
	CXTPOfficeBorder<CXTPReportControl, false> m_wndReport1;
	CXTPOfficeBorder<CXTPReportControl, false> m_wndReport2;
	CXTPReportRecord* m_pCurrentSelectedRecord;



public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnReport2ItemChecked(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReport2ItemValueChanged(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReport2SelChanged(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
};
