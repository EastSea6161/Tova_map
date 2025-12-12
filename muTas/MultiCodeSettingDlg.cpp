// MultiCodeSettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MultiCodeSettingDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"

// KMultiCodeSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMultiCodeSettingDlg, KResizeDialogEx)

KMultiCodeSettingDlg::KMultiCodeSettingDlg(KTarget* a_pTarget, KIOColumn* a_pColumn, CString strCodeList,CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KMultiCodeSettingDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pColumn(a_pColumn)
	, m_strCodeList(strCodeList)
{

}

KMultiCodeSettingDlg::~KMultiCodeSettingDlg()
{
}

void KMultiCodeSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportContrl);
}


BEGIN_MESSAGE_MAP(KMultiCodeSettingDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KMultiCodeSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KMultiCodeSettingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &KMultiCodeSettingDlg::OnBnClickedCheckAll)
END_MESSAGE_MAP()


// KMultiCodeSettingDlg 메시지 처리기입니다.


BOOL KMultiCodeSettingDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportContrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportContrl);

	InitReportHeader();
	UpdaeteReportData();

	ResizeComponet();
	CString strStatic(_T(""));
	strStatic.Format(_T("::%s"), m_pColumn->Caption());

	SetDlgItemText(IDC_STATIC1, strStatic);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KMultiCodeSettingDlg::InitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = NULL;

		pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(0,	_T("Select"), 15));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);

		pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(1,	_T("ID"),	25));
		pColumn->SetHeaderAlignment(DT_CENTER);	

		pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(2,	_T("Name"),	60));
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

void KMultiCodeSettingDlg::UpdaeteReportData()
{
	std::set<int> setSelectID;
	SelectCodeConvertToSet(setSelectID);

	KCodeManager* pCodeMgr		= m_pTarget->CodeManager();
	KCodeGroup*	  pCodeGroup	= pCodeMgr->FindCodeGroup(m_pColumn->CodeGroup());

	std::map<int,CString> mapCode;
	pCodeGroup->GetCodes(mapCode);

	AutoType iter = mapCode.begin();
	AutoType end  = mapCode.end();
	while(iter != end)
	{
		CXTPReportRecord*     pRecord			= m_wndReportContrl.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem				= nullptr;

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetItemData((DWORD_PTR)iter->first);

		AutoType findCode = setSelectID.find(iter->first);
		AutoType endCode  = setSelectID.end();
		if (findCode != endCode)
		{
			pItem->SetChecked(TRUE);
		}
		CString strCode(_T(""));
		strCode.Format(_T("%d"), iter->first);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCode));
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetAlignment(DT_CENTER);
		pItem->SetEditable(FALSE);
		

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(iter->second));
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		++iter;
	}

	m_wndReportContrl.Populate();
}

void KMultiCodeSettingDlg::SelectCodeConvertToSet( std::set<int>& a_setCode )
{
	try
	{
		a_setCode.clear();

		std::vector<CString> vecSelect = KParseString::ParseString(m_strCodeList, _T(','));

		size_t nCount = vecSelect.size();
		for (size_t i = 0; i < nCount; i++)
		{
			CString strCode = vecSelect[i];
			int nCode = _ttoi(strCode);
			a_setCode.insert(nCode);
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


void KMultiCodeSettingDlg::OnBnClickedOk()
{
	SaveReusult();
	KResizeDialogEx::OnOK();
}


void KMultiCodeSettingDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KMultiCodeSettingDlg::SaveReusult()
{

	try
	{
		CString strCodeList(_T(""));
		int nIndex(0);

		CXTPReportRecordItem* pItem				= nullptr;
		CXTPReportRecordItemText* pItemTxt		= nullptr;

		CXTPReportRecords* pRecords = m_wndReportContrl.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);

			pItem = pRecord->GetItem(0);
			if (pItem->IsChecked() == FALSE)
			{
				continue;
			}

			pItemTxt = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			CString strCode = pItemTxt->GetValue();
			if (nIndex == 0)
			{
				strCodeList.AppendFormat(_T("%s"), strCode);
			}
			else
			{
				strCodeList.AppendFormat(_T(",%s"), strCode);
			}
			nIndex++;
		}

		m_strCodeList = strCodeList;

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

CString KMultiCodeSettingDlg::GetResult()
{
	return m_strCodeList;
}


void KMultiCodeSettingDlg::OnBnClickedCheckAll()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK1);

	CXTPReportRecordItem* pItem				= nullptr;
	CXTPReportRecordItemText* pItemTxt		= nullptr;

	CXTPReportRecords* pRecords = m_wndReportContrl.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for (int i = 0; i < nRecordCount; i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);

		pItem = pRecord->GetItem(0);
		pItem->SetChecked(bCheck);		
	}
	
	m_wndReportContrl.Populate();
}

void KMultiCodeSettingDlg::ResizeComponet()
{
	try
	{
		SetResize(IDC_STATIC1,			SZ_TOP_LEFT,		SZ_TOP_LEFT);
		SetResize(IDC_REPORT,			SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
		SetResize(IDC_SEPRATOR1,		SZ_BOTTOM_LEFT,     SZ_BOTTOM_RIGHT);
		SetResize(IDC_CHECK1,			SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
		SetResize(IDCANCEL,				SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);
		SetResize(IDOK,					SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);
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
