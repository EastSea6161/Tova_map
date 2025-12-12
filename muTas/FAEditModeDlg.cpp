// KFAEditModeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FAEditModeDlg.h"
#include "afxdialogex.h"
#include "Target.h"

// KFAEditModeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KFAEditModeDlg, KResizeDialogEx)

KFAEditModeDlg::KFAEditModeDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KFAEditModeDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_nMaxCode(0)
{

}

KFAEditModeDlg::~KFAEditModeDlg()
{
}

void KFAEditModeDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KFAEditModeDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &KFAEditModeDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON2, &KFAEditModeDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDOK, &KFAEditModeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KFAEditModeDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KFAEditModeDlg 메시지 처리기입니다.


BOOL KFAEditModeDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReport, TRUE, FALSE);
	InitReportHeader();
	UpdaetReportData();

	return TRUE; 
}

void KFAEditModeDlg::InitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;
		
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0 , _T("Select"), 20));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1 , _T("ID"), 20));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2 , _T("Name"), 20));
		pColumn->SetHeaderAlignment(DT_CENTER);

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

void KFAEditModeDlg::UpdaetReportData()
{
	try
	{

		KDBaseAssignImport::GetFmodeInfo(m_pTarget, m_mapFAModeInfo);

		m_wndReport.ResetContent(TRUE);

		CXTPReportRecords*			 pRecords	  = m_wndReport.GetRecords();
		CXTPReportRecord*			 pRecord	  = nullptr;

		AutoType iter = m_mapFAModeInfo.begin();
		AutoType end  = m_mapFAModeInfo.end();
		while(iter != end)
		{
			KFAModeInfo oInfo = iter->second;
			pRecord = m_wndReport.AddRecord(new CXTPReportRecord);

			CXTPReportRecordItem*        pItem   	  = nullptr;

			if (oInfo.nSystemType == KEMFREIGHT_SYSTEM)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetItemData((DWORD_PTR)oInfo.nFmode_id);
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

				CString strID(_T(""));
				strID.Format(_T("%d"),oInfo.nFmode_id);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strID));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strFomdeName));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItem);
				pItem->SetItemData((DWORD_PTR)oInfo.nFmode_id);
				pItem->HasCheckbox(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				pItem->SetChecked(FALSE);
				pItem->SetBackgroundColor(RGB(255,255,255));

				CString strID(_T(""));
				strID.Format(_T("%d"),oInfo.nFmode_id);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strID));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strFomdeName));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(RGB(255,255,255));
				pItem->SetEditable(TRUE);
			}

			if (m_nMaxCode < oInfo.nFmode_id)
			{
				m_nMaxCode = oInfo.nFmode_id;
			}

			++iter;
		}
		m_wndReport.Populate();

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


void KFAEditModeDlg::OnBnClickedButtonAdd()
{
	try
	{

		m_nMaxCode = m_nMaxCode + 1;

		CXTPReportRecord*			 pRecord	  = m_wndReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem*		 pItem		  = nullptr;

		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->SetChecked(FALSE);
		pItem->SetItemData((DWORD_PTR)m_nMaxCode);

		CString strCode(_T(""));
		strCode.Format(_T("%d"), m_nMaxCode);

		pItem	= pRecord->AddItem(new CXTPReportRecordItemText(strCode));
		pItem->SetEditable(FALSE);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetAlignment(DT_CENTER);


		pItem	= pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_CENTER);

		m_wndReport.Populate();

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

void KFAEditModeDlg::OnBnClickedButtonDel()
{
	try
	{
		CXTPReportRecords*           pRecords      = nullptr;
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pExtItemCheck = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;

		int nCount(0);

		std::vector<CXTPReportRecord*> vecRecord;
		pRecords			= m_wndReport.GetRecords();
		int nRecordCount	= pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pExtItemCheck = pRecord->GetItem(0);
			if ( TRUE == pExtItemCheck->IsChecked() && TRUE == pExtItemCheck->GetHasCheckbox())
			{
				vecRecord.push_back(pRecord);
			}
		}

		int nvecCount = vecRecord.size();
		for (int ivec = 0; ivec < nvecCount; ivec++)
		{
			pRecord = vecRecord[ivec];
			m_wndReport.GetRecords()->RemoveRecord(pRecord);
			m_wndReport.Populate();
			pRecord = nullptr;
		}

		if (nvecCount == 0)
		{
			AfxMessageBox(_T("선택된 코드 그룹이 존재하지 않습니다."));
			return;
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

void KFAEditModeDlg::GetMaxCode( int& a_nMaxCode )
{
	try
	{
		CXTPReportRecords*          pRecords      = nullptr;
		CXTPReportRecord*           pRecord       = nullptr;
		CXTPReportRecordItem*		pItem		   = nullptr;

		a_nMaxCode = 0;

		try
		{
			pRecords = m_wndReport.GetRecords();
			int nRecordCount = pRecords->GetCount();

			for (int i =0 ; i < nRecordCount; i++)
			{
				pRecord = pRecords->GetAt(i);
				pItem = (CXTPReportRecordItem*)pRecord->GetItem(0);
				int nCode = pItem->GetItemData();

				if (nCode > a_nMaxCode)
				{
					a_nMaxCode = nCode;
				}
			}

			if (a_nMaxCode < 1001)
			{
				a_nMaxCode = 1001;
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
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KFAEditModeDlg::OnBnClickedOk()
{
	try
	{
		std::map<int, KFAModeInfo> mapCodeInfo;

		CXTPReportRecords*           pRecords		= nullptr;
		CXTPReportRecord*            pRecord		= nullptr;
		CXTPReportRecordItem*        pItem			= nullptr;

		pRecords = m_wndReport.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for (int i =0 ; i < nRecordCount; i++)
		{
			KFAModeInfo oInfo;
			pRecord = pRecords->GetAt(i);
			pItem = (CXTPReportRecordItem*)pRecord->GetItem(0);
			oInfo.nFmode_id = pItem->GetItemData();

			if (pItem->GetHasCheckbox() == FALSE)
			{
				continue;
			}
			
			pItem = (CXTPReportRecordItemText*)pRecord->GetItem(2);
			oInfo.strFomdeName = pItem->GetCaption();

			if (pItem->GetCaption() == _T(""))
			{
				CString strMsg(_T(""));
				strMsg.Format(_T("%d 코드의 Name이 없습니다."), oInfo.nFmode_id);
				AfxMessageBox(strMsg);
				return;
			}

			AutoType iter = mapCodeInfo.begin();
			AutoType end  = mapCodeInfo.end();
			while(iter != end)
			{
				KFAModeInfo oExistInfo = iter->second;

				if (oExistInfo.strFomdeName == oInfo.strFomdeName)
				{
					CString strMsg(_T(""));
					strMsg.Format(_T("%s의 명칭이 중복됩니다."), oInfo.strFomdeName);
					AfxMessageBox(strMsg);
				}

				++iter;
			}

			mapCodeInfo.insert(std::make_pair(oInfo.nFmode_id, oInfo));
		}

		KDBaseAssignImport::InsertFModeCode(m_pTarget, mapCodeInfo);
		KDBaseAssignImport::DelFmodeModelData(m_pTarget);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	KResizeDialogEx::OnOK();
}


void KFAEditModeDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}
