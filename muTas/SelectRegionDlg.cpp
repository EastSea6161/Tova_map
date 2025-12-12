// SelectRegionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SelectRegionDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "ZoneSelectDlg.h"
#include "BulkDBaseDemarcation.h"

// KSelectRegionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSelectRegionDlg, KDialogEx)

KSelectRegionDlg::KSelectRegionDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KSelectRegionDlg::IDD, pParent)
{

}

KSelectRegionDlg::~KSelectRegionDlg()
{
}

void KSelectRegionDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_O, m_wndOriginZoneReportCtrl);
	DDX_Control(pDX, IDC_REPORT_D, m_wndDestinationZoneReportCtrl);
	DDX_Radio(pDX,	 IDC_RADIO1,   m_nBtnRadioIndex);
	
}


BEGIN_MESSAGE_MAP(KSelectRegionDlg, KDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &KSelectRegionDlg::OnBnClickedDestination)
	ON_BN_CLICKED(IDC_BUTTON2, &KSelectRegionDlg::OnBnClickedOrigion)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO1, IDC_RADIO2, &KSelectRegionDlg::OnBnClickedRadio)
	ON_BN_CLICKED(IDOK, &KSelectRegionDlg::OnBnClickedOk)
END_MESSAGE_MAP()




BOOL KSelectRegionDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	TxLogDebugStartMsg();
	UpdateData(TRUE);

	m_nBtnRadioIndex = 0;

	UpdateButtonControl();

	CXTPReportColumn* pColumn = NULL;
	m_wndOriginZoneReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);

	pColumn = m_wndOriginZoneReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Zone ID"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndOriginZoneReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Zone Name"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	m_wndOriginZoneReportCtrl.AllowEdit(TRUE);
	m_wndOriginZoneReportCtrl.EditOnClick(TRUE);
	m_wndOriginZoneReportCtrl.SetMultipleSelection(FALSE);
	m_wndOriginZoneReportCtrl.Populate();

	m_wndDestinationZoneReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);

	pColumn = m_wndDestinationZoneReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Zone ID"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndDestinationZoneReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Zone Name"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	m_wndDestinationZoneReportCtrl.AllowEdit(TRUE);
	m_wndDestinationZoneReportCtrl.EditOnClick(TRUE);
	m_wndDestinationZoneReportCtrl.SetMultipleSelection(FALSE);
	m_wndDestinationZoneReportCtrl.Populate();

	UpdateData(FALSE);
	TxLogDebugEndMsg();
	return TRUE;  // return TRUE unless you set the focus to a control

}

void KSelectRegionDlg::OnBnClickedOrigion()
{
	TxLogDebugStartMsg();
	std::map<Integer, CString>::iterator iter = m_mapOrigionZone.begin();
	std::map<Integer, CString>::iterator end  = m_mapOrigionZone.end();
	
	std::vector<Integer> vecZoneID;
	
	while (iter != end)
	{
		vecZoneID.push_back(iter->first);
		++iter;
	}

	KZoneSelectDlg oZoneSelectDlg;
	oZoneSelectDlg.SetTarget(m_pTarget);
	oZoneSelectDlg.SetInitialSelectedZone(vecZoneID);

	if (oZoneSelectDlg.DoModal() != IDOK)
	{
		return;
	}

	iter = m_mapOrigionZone.begin();
	end  = m_mapOrigionZone.end();

	vecZoneID = oZoneSelectDlg.GetSelectedZone();

	std::set<Integer> setSelectedZone;

	 size_t nxCount = vecZoneID.size();
	for (size_t i =0; i< nxCount; i++)
	{
		iter = m_mapOrigionZone.find(vecZoneID[i]);
		if (iter == end)
		{
			std::vector<KZoneInfo> vecZoneInfo;

			CString strZoneName = _T("");
			KBulkDBaseDemarcation::SelectZone(m_pTarget, vecZoneInfo);
			for (size_t nindex=0; nindex<vecZoneInfo.size(); nindex++)
			{
				 KZoneInfo oZoneInfo = vecZoneInfo[nindex];
				 if (oZoneInfo.TZoneID == vecZoneID[i])
				 {
					 strZoneName = oZoneInfo.TZoneName;
				 }
			}
			
			m_mapOrigionZone.insert(std::make_pair(vecZoneID[i],strZoneName));
		}
		 setSelectedZone.insert(vecZoneID[i]);
	}

	std::set<Integer>::iterator iterSelectedZone = setSelectedZone.begin();
	std::set<Integer>::iterator endSelectedZone  = setSelectedZone.end();

	iter = m_mapOrigionZone.begin();
	end  = m_mapOrigionZone.end();

	while(iter != end)
	{
		Integer nxZoneID = iter->first;
		iterSelectedZone = setSelectedZone.find(nxZoneID);
		if (iterSelectedZone == endSelectedZone)
		{
			m_mapOrigionZone.erase(iter);
			iter = m_mapOrigionZone.begin();
			end  = m_mapOrigionZone.end();
		}
		else
		{
			++iter;
		}
	}

	DisplayOriginZone();
	TxLogDebugEndMsg();
}

void KSelectRegionDlg::OnBnClickedDestination()
{
	TxLogDebugStartMsg();
	std::map<Integer, CString>::iterator iter = m_mapDestinationZone.begin();
	std::map<Integer, CString>::iterator end  = m_mapDestinationZone.end();

	std::vector<Integer> vecZoneID;

	while (iter != end)
	{
		vecZoneID.push_back(iter->first);
		++iter;
	}

	KZoneSelectDlg oZoneSelectDlg;
	oZoneSelectDlg.SetTarget(m_pTarget);
	oZoneSelectDlg.SetInitialSelectedZone(vecZoneID);

	if (oZoneSelectDlg.DoModal() != IDOK)
	{
		return;
	}

	iter = m_mapDestinationZone.begin();
	end  = m_mapDestinationZone.end();

	vecZoneID = oZoneSelectDlg.GetSelectedZone();

	std::set<Integer> setSelectedZone;

	size_t nxCount = vecZoneID.size();
	for (size_t i =0; i< nxCount; i++)
	{
		iter = m_mapDestinationZone.find(vecZoneID[i]);
		if (iter == end)
		{
			std::vector<KZoneInfo> vecZoneInfo;

			CString strZoneName = _T("");
			KBulkDBaseDemarcation::SelectZone(m_pTarget, vecZoneInfo);
			for (size_t nindex=0; nindex<vecZoneInfo.size(); nindex++)
			{
				KZoneInfo oZoneInfo = vecZoneInfo[nindex];
				if (oZoneInfo.TZoneID == vecZoneID[i])
				{
					strZoneName = oZoneInfo.TZoneName;
				}
			}

			m_mapDestinationZone.insert(std::make_pair(vecZoneID[i],strZoneName));
		}
		setSelectedZone.insert(vecZoneID[i]);
	}

	std::set<Integer>::iterator iterSelectedZone = setSelectedZone.begin();
	std::set<Integer>::iterator endSelectedZone  = setSelectedZone.end();

	iter = m_mapDestinationZone.begin();
	end  = m_mapDestinationZone.end();

	while(iter != end)
	{
		Integer nxZoneID = iter->first;
		iterSelectedZone = setSelectedZone.find(nxZoneID);
		if (iterSelectedZone == endSelectedZone)
		{
			m_mapDestinationZone.erase(iter);
			iter = m_mapDestinationZone.begin();
			end  = m_mapDestinationZone.end();
		}
		else
		{
			++iter;
		}
	}

	DisplayDestinationZone();
	TxLogDebugEndMsg();
}

void KSelectRegionDlg::DisplayOriginZone()
{
	TxLogDebugStartMsg();
	std::map<Integer, CString>::iterator iter = m_mapOrigionZone.begin();
	std::map<Integer, CString>::iterator end  = m_mapOrigionZone.end();

	CXTPReportRecord*     pRecord       = NULL;
	CXTPReportRecordItem* pRecordItem   = NULL;

	m_wndOriginZoneReportCtrl.ResetContent(FALSE);
	CString strZoneID;
	while (iter != end)
	{
		Integer nxZoneID	= iter->first;
		CString strZoneName	= iter->second;

		strZoneID.Format(_T("%I64d"), nxZoneID);

		pRecord = m_wndOriginZoneReportCtrl.AddRecord(new CXTPReportRecord());
		pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(strZoneID));
		pRecordItem->SetAlignment(DT_CENTER);
		pRecordItem->SetItemData((DWORD_PTR)nxZoneID);
		pRecordItem->SetEditable(FALSE);

		pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(strZoneName));
		pRecordItem->SetAlignment(DT_CENTER);
		pRecordItem->SetEditable(FALSE);
		++iter;
	}
	m_wndOriginZoneReportCtrl.Populate();
	TxLogDebugEndMsg();
}

void KSelectRegionDlg::DisplayDestinationZone()
{
	TxLogDebugStartMsg();
	std::map<Integer, CString>::iterator iter = m_mapDestinationZone.begin();
	std::map<Integer, CString>::iterator end  = m_mapDestinationZone.end();

	CXTPReportRecord*     pRecord       = NULL;
	CXTPReportRecordItem* pRecordItem   = NULL;

	m_wndDestinationZoneReportCtrl.ResetContent(FALSE);
	CString strZoneID;
	while (iter != end)
	{
		Integer nxZoneID	= iter->first;
		CString strZoneName	= iter->second;

		strZoneID.Format(_T("%I64d"), nxZoneID);

		pRecord = m_wndDestinationZoneReportCtrl.AddRecord(new CXTPReportRecord());
		pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(strZoneID));
		pRecordItem->SetAlignment(DT_CENTER);
		pRecordItem->SetItemData((DWORD_PTR)nxZoneID);
		pRecordItem->SetEditable(FALSE);

		pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(strZoneName));
		pRecordItem->SetAlignment(DT_CENTER);
		pRecordItem->SetEditable(FALSE);
		++iter;
	}
	m_wndDestinationZoneReportCtrl.Populate();
	TxLogDebugEndMsg();
}

void KSelectRegionDlg::OnBnClickedRadio(UINT nID)
{
	UpdateData(TRUE);
	UpdateButtonControl();
	UpdateData(FALSE);
}

void KSelectRegionDlg::UpdateButtonControl()
{
	switch(m_nBtnRadioIndex)
	{
	case 0:	
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		m_wndDestinationZoneReportCtrl.ResetContent(FALSE);
		m_wndOriginZoneReportCtrl.ResetContent(FALSE);
		break;

	case 1:	
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		break;


	default:
		return;
	}
}

void KSelectRegionDlg::SelectOriginZoneData()
{
	m_vecResultOrigionZone.clear();

	CXTPReportRecords* pRecords = m_wndOriginZoneReportCtrl.GetRecords();

	int nRecordCount   = pRecords->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord   = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = NULL;

		pItem = pRecord->GetItem(0);
		CString strZoneID = pItem->GetCaption(NULL);

		Integer nxZoneID = _ttoi64(strZoneID);

		m_vecResultOrigionZone.push_back(nxZoneID);
	}
}

void KSelectRegionDlg::SelectDestinationZoneData()
{
	m_vecResultDestinationZone.clear();

	CXTPReportRecords* pRecords = m_wndDestinationZoneReportCtrl.GetRecords();

	int nRecordCount   = pRecords->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord   = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = NULL;

		pItem = pRecord->GetItem(0);
		CString strZoneID = pItem->GetCaption(NULL);

		Integer nxZoneID = _ttoi64(strZoneID);

		m_vecResultDestinationZone.push_back(nxZoneID);
	}
}

void KSelectRegionDlg::GetSelectZoneID(std::vector<Integer>& a_vecOZone, std::vector<Integer>& a_vecDZone)
{
	a_vecOZone = m_vecResultOrigionZone;
	a_vecDZone = m_vecResultDestinationZone;
}


void KSelectRegionDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	switch(m_nBtnRadioIndex)
	{
	case 0:	
		//SelectAllZoneData();
		m_vecResultDestinationZone.clear();
		m_vecResultOrigionZone.clear();
		break;

	case 1:	
		SelectOriginZoneData();
		SelectDestinationZoneData();
		break;
	default:
		return;
	}
	UpdateData(FALSE);
	KDialogEx::OnOK();
}

void KSelectRegionDlg::SelectAllZoneData()
{
	m_vecResultDestinationZone.clear();
	m_vecResultOrigionZone.clear();

	std::vector<KZoneInfo> vecZoneInfo;

	KBulkDBaseDemarcation::SelectZone(m_pTarget, vecZoneInfo);
	for (size_t i=0; i<vecZoneInfo.size(); i++)
	{
		KZoneInfo oZoneInfo = vecZoneInfo[i];
		Integer nxZoneID = oZoneInfo.TZoneID;

		m_vecResultDestinationZone.push_back(nxZoneID);
		m_vecResultOrigionZone.push_back(nxZoneID);
	}
}

