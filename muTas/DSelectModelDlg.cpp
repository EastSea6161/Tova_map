/**
 * @file DSelectModelDlg.cpp
 * @brief KDSelectModelDlg 구현 파일
 * @author 
 * @date 2011.05.19
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "DSelectModelDlg.h"
#include "afxdialogex.h"
#include "DistributionInfo.h"
#include "Purpose.h"
#include "KmzSystem.h"

/* Report item index */
#define DMODEL_PURPOSE		0
#define DMODEL_SELECT		1
#define DMODEL_FRATAR		2
#define DMODEL_GRAVITY		3


IMPLEMENT_DYNAMIC(KDSelectModelDlg, KDialogEx)


KDSelectModelDlg::KDSelectModelDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KDSelectModelDlg::IDD, pParent)
	, m_nODType(0)
{

}

KDSelectModelDlg::~KDSelectModelDlg()
{
}

void KDSelectModelDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
	DDX_Radio(pDX, IDC_RADIO1, m_nODType);
}


BEGIN_MESSAGE_MAP(KDSelectModelDlg, KDialogEx)
	ON_COMMAND_RANGE(IDC_RADIO1, IDC_RADIO2, OnRadioClicked)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, OnReportCheckItem)
//    ON_BN_CLICKED(IDOK, &KDSelectModelDlg::OnBnClickedOk)
ON_BN_CLICKED(IDC_CHECK1, &KDSelectModelDlg::OnBnClickedCheckSelectAll)
END_MESSAGE_MAP()



void KDSelectModelDlg::AddDistributionInfo(KEMDODType emODType, KDistributionInfo* pInfo)
{
	std::list<KDistributionInfo*>::iterator itInfo, itEnd;
	if(KEMDODTypePurpose == emODType)
	{
		itEnd = m_PurposeInfoList.end();
		for(itInfo = m_PurposeInfoList.begin(); itInfo != itEnd; ++itInfo)
		{
			if(*itInfo == pInfo)
			{
				return;
			}
		}
		m_PurposeInfoList.push_back(pInfo);
	}
	else
	{
		itEnd = m_ModeInfoList.end();
		for(itInfo = m_ModeInfoList.begin(); itInfo != itEnd; ++itInfo)
		{
			if(*itInfo == pInfo)
			{
				return;
			}
		}
		m_ModeInfoList.push_back(pInfo);
	}
}


void KDSelectModelDlg::SetDistributionInfo(KEMDODType emODType, std::list<KDistributionInfo*>& infoList)
{
	if(KEMDODTypePurpose == emODType)
	{
		m_PurposeInfoList = infoList;
	}
	else
	{
		m_ModeInfoList = infoList;
	}
}


KEMDODType KDSelectModelDlg::DistributionODType(void)
{
	m_emODType =  (0 == m_nODType) ? KEMDODTypePurpose : KEMDODTypeMode;
	return m_emODType;
}


void KDSelectModelDlg::DistributionODType(KEMDODType emODType)
{
	m_emODType = emODType;
	m_nODType = (KEMDODTypePurpose == m_emODType) ? 0 : 1;
	initReportColumn();
	updateReportRecords();
}


void KDSelectModelDlg::initReportColumn(void)
{
	CXTPReportColumn* pColumn = NULL;
	m_wndReport.ResetContent(TRUE);
	m_wndReport.GetColumns()->Clear();

	if(KEMDODTypePurpose == m_emODType)
	{
        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(DMODEL_PURPOSE, _T("Purpose"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행목적"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(DMODEL_SELECT, _T("Select"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(DMODEL_FRATAR, _T("Fratar"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("프라타 모형"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(DMODEL_GRAVITY, _T("Gravity"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("중력모형"));
        }
	}
	else
	{
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(DMODEL_PURPOSE, _T("Mode"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("수단"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(DMODEL_SELECT, _T("Select"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(DMODEL_FRATAR, _T("Fratar"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("프라타 모형"));
        }
	}
}


void KDSelectModelDlg::updateReportRecords(void)
{
	m_wndReport.ResetContent(TRUE);
	
	CXTPReportColumns* pColumns = m_wndReport.GetColumns();
	std::list<KDistributionInfo*>::iterator itInfo, itEnd;
	if(KEMDODTypePurpose == m_emODType)
	{
		itEnd = m_PurposeInfoList.end();
		for(itInfo = m_PurposeInfoList.begin(); itInfo != itEnd; ++itInfo)
		{
			KDistributionInfo* pInfo = *itInfo;
			CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetCaption(pInfo->PurposeName());
			pItem->SetItemData((DWORD_PTR)pInfo);

			BOOL bEditable = FALSE;
			// select
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->HasCheckbox(TRUE);
			if(pInfo->Selected() == true)
			{
				pItem->SetChecked(TRUE);
				bEditable = TRUE;
			}

			// Fratar
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			//pItem->SetAlignment(DT_CENTER);
			//if(pInfo->DistributionModel() == KEMDistributionModelFratar)
			//{
			//	pItem->SetCaption(_T("V"));
			//}
			pItem->HasCheckbox(TRUE);
			if(pInfo->ModelType() == KEMDistributionModelFratar)
			{
				pItem->SetChecked(TRUE);
			}
			pItem->SetEditable(bEditable);
			
			// Gravity
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			//pItem->SetAlignment(DT_CENTER);
			//if(pInfo->DistributionModel() == KEMDistributionModelGravity)
			//{
			//	pItem->SetCaption(_T("V"));
			//}
			pItem->HasCheckbox(TRUE);
			if(pInfo->ModelType() == KEMDistributionModelGravity)
			{
				pItem->SetChecked(TRUE);
			}
			pItem->SetEditable(bEditable);
		}
	}
	else
	{
		itEnd = m_ModeInfoList.end();
		for(itInfo = m_ModeInfoList.begin(); itInfo != itEnd; ++itInfo)
		{
			KDistributionInfo* pInfo = *itInfo;
			CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetCaption(pInfo->PurposeName());

			BOOL bEditable = FALSE;
			// select
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->HasCheckbox(TRUE);
			if(pInfo->Selected() == true)
			{
				pItem->SetChecked(TRUE);
				bEditable = TRUE;
			}

			// Fratar
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			//pItem->SetAlignment(DT_CENTER);
			//pItem->SetCaption(_T("V"));
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(TRUE);
			pItem->SetEditable(bEditable);
		}
	}

	m_wndReport.Populate();
}


// KDSelectModelDlg 메시지 처리기입니다.
BOOL KDSelectModelDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReport);
	initReportColumn();
	updateReportRecords();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDSelectModelDlg::OnOK()
{
	CXTPReportRecords*   pRecords = m_wndReport.GetRecords();
	int nRecordCount   = pRecords->GetCount();
	int nSelectedCount = 0; 

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(DMODEL_PURPOSE);
		KDistributionInfo* pInfo = (KDistributionInfo*)(pItem->GetItemData());
		
		//CString strCaption = pRecord->GetItem(DMODEL_FRATAR)->GetCaption(NULL);

		//if(strCaption.IsEmpty() == FALSE)
		//{
		//	pInfo->DistributionModel(KEMDistributionModelFratar);
		//}
		//else
		//{
		//	pInfo->DistributionModel(KEMDistributionModelGravity);
		//}
		if(pRecord->GetItem(DMODEL_SELECT)->IsChecked() == TRUE)
		{
			pInfo->Selected(true);
			nSelectedCount++;
		}
		else
		{
			pInfo->Selected(false);
		}

		if(pRecord->GetItem(DMODEL_FRATAR)->IsChecked() == TRUE)
		{
			pInfo->ModelType(KEMDistributionModelFratar);
		}
		else
		{
			pInfo->ModelType(KEMDistributionModelGravity);
		}
	}

	if(nSelectedCount < 1)
	{
		AfxMessageBox(_T("선택된 정보가 존재하지 않습니다."));
		return;
	}

	KDialogEx::OnOK();
}


void KDSelectModelDlg::OnRadioClicked(UINT nID)
{
	if(IDC_RADIO1 == nID)
	{
		DistributionODType(KEMDODTypePurpose);
	}
	else
	{
		DistributionODType(KEMDODTypeMode);
	}
}


void KDSelectModelDlg::OnReportItemClick(NMHDR * pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if(KEMDODTypePurpose == m_emODType)
	{
		//pItemNotify->pItem->SetCaption(_T("V"));
		//if(DMODEL_FRATAR == nIndex)
		//{
		//	pItemNotify->pRow->GetRecord()->GetItem(DMODEL_GRAVITY)->SetCaption(_T(""));
		//}
		//else if(DMODEL_GRAVITY == nIndex)
		//{
		//	pItemNotify->pRow->GetRecord()->GetItem(DMODEL_FRATAR)->SetCaption(_T(""));
		//}

		if( TRUE == pItemNotify->pRow->GetRecord()->GetItem(DMODEL_SELECT)->IsChecked() )
		{
			if(DMODEL_FRATAR == nIndex)
			{
				pItemNotify->pItem->SetChecked(TRUE);
				pItemNotify->pRow->GetRecord()->GetItem(DMODEL_GRAVITY)->SetChecked(FALSE);
			}
			else if(DMODEL_GRAVITY == nIndex)
			{
				pItemNotify->pItem->SetChecked(TRUE);
				pItemNotify->pRow->GetRecord()->GetItem(DMODEL_FRATAR)->SetChecked(FALSE);
			}
		}
	}
	
}


void KDSelectModelDlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{

	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( DMODEL_SELECT == nIndex)
	{
		if( TRUE == pItemNotify->pRow->GetRecord()->GetItem(DMODEL_SELECT)->IsChecked() )
		{
			pItemNotify->pRow->GetRecord()->GetItem(DMODEL_FRATAR)->SetEditable(TRUE);

			if(KEMDODTypePurpose == m_emODType)
			{
				pItemNotify->pRow->GetRecord()->GetItem(DMODEL_GRAVITY)->SetEditable(TRUE);
			}
		}
		else
		{
			pItemNotify->pRow->GetRecord()->GetItem(DMODEL_FRATAR)->SetEditable(FALSE);

			if(KEMDODTypePurpose == m_emODType)
			{
				pItemNotify->pRow->GetRecord()->GetItem(DMODEL_GRAVITY)->SetEditable(FALSE);
			}

			CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
		}
	}
}


//void KDSelectModelDlg::OnBnClickedOk()
//{
//    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//    KDialogEx::OnOK();
//}


void KDSelectModelDlg::OnBnClickedCheckSelectAll()
{
	if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
	{
		UpdateReportSelectStatus(TRUE);
	}
	else
	{
		UpdateReportSelectStatus(FALSE);
	}
}


void KDSelectModelDlg::UpdateReportSelectStatus(BOOL a_bCheck)
{
	bool bSelect(false);
	if (TRUE == a_bCheck)
		bSelect = true;
	else
		bSelect = false;

	CXTPReportRecords*   pRecords = m_wndReport.GetRecords();
	int nRecordCount   = pRecords->GetCount();

	for (int i= 0; i< nRecordCount; i++)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(DMODEL_PURPOSE);
		KDistributionInfo*    pInfo   = (KDistributionInfo*)(pItem->GetItemData());

		pItem = pRecord->GetItem(DMODEL_SELECT);
		pItem->SetChecked(a_bCheck);
		pInfo->Selected(bSelect);

		if(pRecord->GetItem(DMODEL_FRATAR)->IsChecked() == TRUE)
		{
			pInfo->ModelType(KEMDistributionModelFratar);
		}
		else
		{
			pInfo->ModelType(KEMDistributionModelGravity);
		}

		pRecord->GetItem(DMODEL_FRATAR)->SetEditable(a_bCheck);
		pRecord->GetItem(DMODEL_GRAVITY)->SetEditable(a_bCheck);
	}

	m_wndReport.Populate();
}