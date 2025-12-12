// FDefaultPolicyContainerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FDefaultPolicyContainerDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"


// KFDefaultPolicyContainerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KFDefaultPolicyContainerDlg, KResizeDialogEx)

KFDefaultPolicyContainerDlg::KFDefaultPolicyContainerDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KFDefaultPolicyContainerDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)

{
	m_oSelectZoneIDCaption.TID = _T("-1");
}

KFDefaultPolicyContainerDlg::~KFDefaultPolicyContainerDlg()
{
}

void KFDefaultPolicyContainerDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportContrl);
}


BEGIN_MESSAGE_MAP(KFDefaultPolicyContainerDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &KFDefaultPolicyContainerDlg::OnBnClickedButtonADD)
	ON_BN_CLICKED(IDC_BUTTON2, &KFDefaultPolicyContainerDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDOK, &KFDefaultPolicyContainerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KFDefaultPolicyContainerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON3, &KFDefaultPolicyContainerDlg::OnBnClickedButtonInitialze)
END_MESSAGE_MAP()


// KFDefaultPolicyContainerDlg 메시지 처리기입니다.


BOOL KFDefaultPolicyContainerDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KDBaseDefPolicyContainer::GetDefaultPolicyContainerInfo(m_mapPolicyContainerInfo);
	KDBaseDefPolicyContainer::GetAllZoneInfo(m_pTarget, m_mapAllZoneInfo);
	KReportCtrlSetting::Default(m_wndReportContrl,		 TRUE);

	IniteReportHeader();
	UpdateReportData();

	return TRUE;
}

void KFDefaultPolicyContainerDlg::IniteReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(0,	_T("Sel"),30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(1,	_T("Zone_ID"),	50));
	pColumn->SetHeaderAlignment(DT_CENTER);	

	pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(2,	_T("Port_Name"),	80));
	pColumn->SetHeaderAlignment(DT_CENTER);	

	pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(3,	_T("Zone_Name"),	80));
	pColumn->SetHeaderAlignment(DT_CENTER);	

	pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(4,	_T("수입(TEU)"),	80));
	pColumn->SetHeaderAlignment(DT_CENTER);	

	pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(5,	_T("수출(TEU)"),	80));
	pColumn->SetHeaderAlignment(DT_CENTER);	
}

void KFDefaultPolicyContainerDlg::UpdateReportData()
{
	m_wndReportContrl.ResetContent(TRUE);

	try
	{
		AutoType iter = m_mapPolicyContainerInfo.begin();
		AutoType end  = m_mapPolicyContainerInfo.end();
		while(iter != end)
		{
			KPolicyContainerInfo oPInfo = iter->second;

			CXTPReportRecord*		pRecord = m_wndReportContrl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem   = nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetChecked(FALSE);

			CString strZoneID(_T(""));
			strZoneID.Format(_T("%I64d"), oPInfo.TnxZoneID);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strZoneID));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData((DWORD_PTR)oPInfo.TnxZoneID);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oPInfo.TstrPortName));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable(TRUE);

			CString strZoneName(_T("-"));
			AutoType find = m_mapAllZoneInfo.find(oPInfo.TnxZoneID);
			AutoType end  = m_mapAllZoneInfo.end();
			if (find != end)
			{
				strZoneName = find->second;
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strZoneName));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oPInfo.TdProduction, _T("%.0f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_RIGHT);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oPInfo.TdConsumption, _T("%.0f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_RIGHT);

			++iter;
		}

		m_wndReportContrl.Populate();

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KFDefaultPolicyContainerDlg::OnBnClickedButtonADD()
{
	ZoneNodeDataToIDFindData();

	m_spImTasIDFind = KImChampIDFindPtr(new KImChampIDFind(this));
	m_spImTasIDFind->SetModelessReceiver(this);
	m_spImTasIDFind->SetSelectedID(-1);
	m_spImTasIDFind->SetIDCaption(m_vecZoneNodeIDCaption);
	m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapAllZoneInfo, ImChampMapDefineSymbol::ZoneSymbol);

	m_spImTasIDFind->Create(KImChampIDFind::IDD, this);
	m_spImTasIDFind->ShowWindow(SW_SHOW);
}


void KFDefaultPolicyContainerDlg::OnBnClickedButtonDel()
{
	try
	{
		std::vector<CXTPReportRecord*> vecRecord;
		CXTPReportRecords* pRecords = m_wndReportContrl.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem = pRecord->GetItem(0);
			
			if (TRUE == pItem->IsChecked())
			{
				vecRecord.push_back(pRecord);

			}
		}

		if (vecRecord.size() < 1)
		{
			AfxMessageBox(_T("삭제할 데이터가 없습니다.\n삭제할 데이터를 선택 후 삭제 버튼을 클릭해 주세요."));
			return;
		}

		for (size_t i = 0 ; i < vecRecord.size(); i++)
		{
			m_wndReportContrl.GetRecords()->RemoveRecord(vecRecord[i]);
			m_wndReportContrl.Populate();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KFDefaultPolicyContainerDlg::OnBnClickedOk()
{
	if (SaveData() == false)
	{
		return;
	}

	AfxMessageBox(_T("저장하였습니다."));
	KResizeDialogEx::OnOK();
}


void KFDefaultPolicyContainerDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

bool KFDefaultPolicyContainerDlg::SaveData()
{
	std::map<Integer, KPolicyContainerInfo> mapResultInfo;

	try
	{
		CXTPReportRecordItem*        pItem		   = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;
		CXTPReportRecordItemNumber*  pItemNum	   = nullptr;

		CXTPReportRecords* pRecords = m_wndReportContrl.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			KPolicyContainerInfo oInfo;

			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			Integer nxZoneID = (Integer)pItemText->GetItemData();
			oInfo.TnxZoneID = nxZoneID;

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(2);
			oInfo.TstrPortName = pItemText->GetCaption(NULL);

			pItemNum  = (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
			oInfo.TdProduction = pItemNum->GetValue();

			pItemNum  = (CXTPReportRecordItemNumber*)pRecord->GetItem(5);
			oInfo.TdConsumption = pItemNum->GetValue();

			mapResultInfo.insert(std::make_pair(oInfo.TnxZoneID, oInfo));
		}

		if (KDBaseDefPolicyContainer::SaveContainerInfo(mapResultInfo) == false)
		{
			throw -1;
		}
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return false;
	}

	return true;
}


void KFDefaultPolicyContainerDlg::OnBnClickedButtonInitialze()
{
	UpdateReportData();
}

void KFDefaultPolicyContainerDlg::ModelessOKFeedback( UINT nDialogID )
{
	if (KImChampIDFind::IDD == nDialogID)
	{
		if (m_spImTasIDFind)
		{
			try
			{
				Integer nxZoneID = m_spImTasIDFind->GetSelectedID();
				AddReportData(nxZoneID);
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}

void KFDefaultPolicyContainerDlg::ModelessCancleFeedback( UINT nDialogID )
{

}

void KFDefaultPolicyContainerDlg::ZoneNodeDataToIDFindData()
{
	m_vecZoneNodeIDCaption.clear();
	std::set<Integer> setSelectZoneData;

	try
	{

		CXTPReportRecords* pRecords = m_wndReportContrl.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem = pRecord->GetItem(1);
			Integer nxZoneID = (Integer)pItem->GetItemData();
			setSelectZoneData.insert(nxZoneID);
		}

		AutoType iter = m_mapAllZoneInfo.begin();
		AutoType end  = m_mapAllZoneInfo.end();
		while(iter != end)
		{
			AutoType find = setSelectZoneData.find(iter->first);
			AutoType fend = setSelectZoneData.end();
			if (find != fend)
			{
				++iter;
				continue;
			}

			CString strZoneID(_T(""));
			strZoneID.Format(_T("%I64d"), iter->first);
			KIDCaption oCaption;
			oCaption.TID		= strZoneID;
			oCaption.TCaption	= iter->second;
			m_vecZoneNodeIDCaption.push_back(oCaption);

			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KFDefaultPolicyContainerDlg::AddReportData(Integer a_nxZoneID)
{
	try
	{
		CXTPReportRecord*		pRecord = m_wndReportContrl.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem*	pItem   = nullptr;

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->SetChecked(FALSE);

		CString strZoneID(_T(""));
		strZoneID.Format(_T("%I64d"), a_nxZoneID);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strZoneID));
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetAlignment(DT_CENTER);
		pItem->SetItemData((DWORD_PTR)a_nxZoneID);
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pItem->SetAlignment(DT_CENTER);
		pItem->SetEditable(TRUE);

		CString strZoneName(_T("-"));
		AutoType find = m_mapAllZoneInfo.find(a_nxZoneID);
		AutoType end  = m_mapAllZoneInfo.end();
		if (find != end)
		{
			strZoneName = find->second;
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strZoneName));
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetAlignment(DT_CENTER);
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(0, _T("%.0f")));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_RIGHT);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(0, _T("%.0f")));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_RIGHT);

		m_wndReportContrl.Populate();

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}
