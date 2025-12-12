#pragma once
#include "afxcmn.h"
#include "DefaultInterModeChoiceParaSubPage.h"
#include "KDefInterUrbanModeChoiceVariableSettingDlg.h"



class KTarget;
class KDefaultInterModeChoiceParaSubPage;
struct TUrbanChoiceValues;
struct TDescriptionVariableInfo;
struct TZoneVariableInfo;


struct STArgsColumnMissingTarget
{
	KScenario* pScenario;
	KTarget* pTarget;
	std::vector<CString> arrMissingCol;
};


// KDefInterUrbanModeChoiceParaDlg 대화 상자입니다.

class KDefInterUrbanModeChoiceParaDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefInterUrbanModeChoiceParaDlg)

public:
	KDefInterUrbanModeChoiceParaDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefInterUrbanModeChoiceParaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8511_Default_Inter_ModeChoiceParaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	void         MakeDefaultParameter(std::map<int, std::map<int, TUrbanChoiceValues>> &a_mapDefaultPara);
	void         BaseDefaultPara(std::map<int, std::map<int, TUrbanChoiceValues>> &a_mapDefaultParaBase);
	
	void         DisplayTabControl(int a_nTabIndex);
	void         ResizeComponet();
	void         CreateChildPage();
	void         SubPageRedrawWindow();
	bool         InvalidInputInfo();

protected:
	std::map<int, TDescriptionVariableInfo> m_mapVariableInfo;	// 설명변수 정보
	std::map<int, TZoneVariableInfo> m_mapZoneVariableInfo;		// 존 특성 변수 정보
	std::map<int, TDescriptionVariableInfo> m_mapDefaultVar;	// 현재값(마지막 저장된 값)
	std::map<int, TZoneVariableInfo> m_mapDefaultZoneVar;		// 현재값(마지막 저장된 값)
	std::map<int, TZoneUserColumnInfo> m_mapUserZoneColumnInfo;	// 현재 Target의 zone테이블 중 사용자 정의 존 컬럼 정보

	/**
	* @brief 변경된 설명변수 항목을 DB의 UrbanModeChoiceVariable 테이블에 저장한다.
	* @param spDBConn Project DB의 연결 정보
	* @return bool 정상적으로 실행시 true, 실패시 false 반환
	*/
	bool UpdateDBSelectedVariables(KDBaseConPtr spDBConn);

	/**
	* @brief 변경된 존특성 변수 항목을 DB의 UrbanModeChoiceVariableZone 테이블에 저장한다.
	* @param spDBConn Project DB의 연결 정보
	* @return bool 정상적으로 실행시 true, 실패시 false 반환
	*/
	bool UpdateDBSelectedZoneVariables(KDBaseConPtr spDBConn);

	/**
	* @brief 설명변수와 존특성 변수 선택창을 표출하고, 현재의 변수선택 정보를 선택된 정보로 갱신
	*/
	bool SelectAndUpdateVariables();


	/**
	* @brief Default 목적별 변수 설정
	*/
	void InitDefaultPurposePara();

	/**
	* @brief 현재 목적별 변수 설정
	*/
	void InitCurrentPurposePara();

	/**
	* @brief 선택된 설명변수와 존특성 변수를 DB에 저장
	* @param spDBConn 프로젝트 DB의 연결정보
	*/
	bool SaveVariables();

	/**
	* @brief Default 목적별 변수를 목적명별 파라미터 Map 생성
	*/
	void MakeStringPurposeParaMap();

	/**
	* @brief 다른 시나리오의 누락된 사용자 정의 존컬럼을 추가하고, 데이터를 갱신한다.
	* @return bool 정상처리시 true, 실패시 false 반환
	*/
	bool ProcessMissingZoneColumn();

	/**
	* @brief 대상 테이블에 누락된 컬럼을 추가한다.
	* @param pSrcIOTable 원본 zone 테이블
	* @param argsCMT 컬럼이 누락된 target에 대한 정보(시나리오, target, 누락된 컬럼 목록)
	* @return bool 정상처리시 true, 실패시 false 반환
	*/
	bool AddMissingColumns(KIOTable* pSrcIOTable, STArgsColumnMissingTarget& argsCMT);

	/**
	* @brief 주어진 테이블에서 컬럼의 다음 표출 순서를 반환
	* @param pIOTable 대상 테이블
	* @return int 다음 표출순서. 주어진 테이블의 컬럼목록중 최대 표출 순서값 + 1
	*/
	int GetNextColumnDisplayOrder(KIOTable* pIOTable);

	/**
	* @brief 주어진 컬럼명 배열로 SQL에 사용되는 컬럼명 문자열(예 : col1, col2, col3 )을 생성
	* @param arrColumnNames 컬럼명 배열
	* @return CString 컬럼명 문자열
	*/
	CString MakeSQLColumnNameString(std::vector<CString>& arrColumnNames);

	/**
	* @brief 해당 테이블에 이미 동일한 이름의 컬럼표출명(Caption)이 존재하는지 확인
	* @param pTable 대상 테이블
	* @param strCaption 검사할 컬럼 표출명
	* @return bool 동일한 컬럼명이 존재하면 true, 그렇지 않으면 false를 반환
	*/
	bool IsColumnCaptionExists(KIOTable* pTable, CString& strCaption);


	/**
	* @brief 현재의 Target에서 사용자 정의 존 컬럼 정보를 읽어온다.
	*/
	void ReadZoneColumnInfo();

	/**
	* @brief 대상 Target에서 사용자 정의 존 컬럼 정보를 읽어온다.
	* @param spDBConn 대상 Target의 DB 커넥션
	* @return std::vector<TZoneUserColumnInfo> 대상 Target의 사용자 정의 존 컬럼 정보
	*/
	std::vector<TZoneUserColumnInfo> ReadTargetZoneColumn(KDBaseConPtr spDBConn);

	/**
	* @brief 다른 시나리오의 컬럼과 동일명의 다른 데이터타입이 존재하는지 충돌체크와 다른 시나리오에 컬럼이 누락되었는지 체크
	*        충돌하는 정보는 m_arrConflictTarget에 문자열로 저장하고, 누락된 정보는 m_mapMissingTarget에 저장한다.
	* @param arrSelectedZoneVariable KDefInterUrbanModeChoiceVariableSettingDlg에서 선택한 존특성변수의 vector
	*/
	void CheckZoneColumnConflictAndMissing(std::vector<TZoneVariableInfo>& arrSelectedZoneVariable);

	///**
	//* @brief 대상 테이블에 누락된 컬럼 데이터를 추가한다.
	//* @param pSrcZoneColumns 원본 zone 테이블의 컬럼집합
	//* @param argsCMT 컬럼이 누락된 target에 대한 정보(시나리오, target, 누락된 컬럼 목록)
	//* @return bool 정상처리시 true, 실패시 false 반환
	//*/
	//bool InsertMissingColumnData(KIOColumns* pSrcZoneColumns, STArgsColumnMissingTarget& argsCMT);


	std::vector<CString> m_arrConflictTarget;
	std::map<int, STArgsColumnMissingTarget> m_mapMissingTarget;

private:
	KTarget*     m_pTarget;

protected:
	KDefaultInterModeChoiceParaSubPagePtr                m_spCurrentShowClientDlg;
	std::map<int, KDefaultInterModeChoiceParaSubPagePtr> m_mapSubPage;
	std::map<int, std::map<int, TUrbanChoiceValues>>     m_mapDefaultPara;	// 기본값(초기화 버튼을 누를때 설정)
	std::map<CString, std::map<int, TUrbanChoiceValues>> m_mapStrPurposeDefaultPara;	// 목적명 별 기본값
	std::map<int, std::map<int, TUrbanChoiceValues>>     m_mapCurrentPara;	// 현재값(마지막 저장된 값)
	double       m_dMeuParameter;
	std::map<int, CString>                               m_mapPurposeName;

	std::map<int, TUrbanChoiceValues> m_mapTmpInterModeChoicePara;	//Copy Data 보관
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTabObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
			void SaveParameter();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonDefault();
	
private:
	CTabCtrl      m_ctrlTab;
public:
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonPaste();
	afx_msg void OnBnClickedButtonSetVariable();
};
