/**
 * @file DBaseODDlg.h
 * @brief KDBaseODDlg 선언 파일
 * @author 
 * @date 2011.05.20
 * @remark
 */


#pragma once


#include "model_common.h"
#include "DistributionInfoList.h"


/* Forward declarations */
class KDistributionInfo;
class KScenario;
class KTarget;


//class CCustomReportRecordItem : public CXTPReportRecordItem
//{
//	virtual void OnConstraintChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraint* pConstraint)
//	{
//		if(pConstraint)
//		{
//			m_dwData = pConstraint->m_dwData;
//		}
//		else
//		{
//			m_dwData = NULL;
//		}
//	}
//
//	CString GetCaption(CXTPReportColumn* pColumn)
//	{
//		CXTPReportRecordItemConstraint* pConstraint = pColumn->GetEditOptions()->FindConstraint(m_dwData);
//		if(NULL == pConstraint)
//		{
//			pConstraint = pColumn->GetEditOptions()->GetConstraints()->GetAt(0);
//			m_dwData = pConstraint->m_dwData;
//		}
//		return pConstraint->m_strConstraint;
//	}
//};


/**
 * @brief 분포 모형에서 Base OD를 선택하는 Dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.20
 */
class KDBaseODDlg : public KDialogEx, public KDistributionInfoList
{
	DECLARE_DYNAMIC(KDBaseODDlg)

public:
	KDBaseODDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDBaseODDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6310_DBaseODDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected :
    /**
    * @brief Report Control 초기화
    */
    void initReportControl(void);

    /**
    * @brief Report Tree(시나리오-목표년도) Control 초기화
    */
    void initTreeReportControl(void);

    /**
    * @brief 목적 컬럼 로드
    */
    void initPurposeReportControl(void);

    /**
    * @brief Target 변경 이벤트 처리
    */
    void updateTargetTree(void);

    /**
    * @brief 트리에 시나리오에 해당하는 목표년도 로드
    */
    void addTargetRecord(CXTPReportRecord* pParnet, KScenario* pScenario);

    /**
    * @brief 목적 데이터 업데이트
    */
    void updatePurposeRecord(void);

    /**
    * @brief 목적 컬럼 콤보 설정
    */
    void updateFieldConstraint(KTarget* pTarget);

protected :
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportTree;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportPurpose;

public:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
    afx_msg void OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result);
	virtual void OnOK();
};
