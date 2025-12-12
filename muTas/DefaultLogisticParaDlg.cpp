// DefaultLogisticParaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultLogisticParaDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "PurposeInfoCollection.h"
#include "Purpose.h"
#include "DefineNamesDefaultPara.h"

// KDefaultLogisticParaDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultLogisticParaDlg, KResizeDialogEx)

KDefaultLogisticParaDlg::KDefaultLogisticParaDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultLogisticParaDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KDefaultLogisticParaDlg::~KDefaultLogisticParaDlg()
{

}

void KDefaultLogisticParaDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportContrl);
}

BEGIN_MESSAGE_MAP(KDefaultLogisticParaDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDDEFAULT, &KDefaultLogisticParaDlg::OnBnClickedDefault)
	ON_BN_CLICKED(IDOK, &KDefaultLogisticParaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KDefaultLogisticParaDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL KDefaultLogisticParaDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportContrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportContrl);

	InitLogisticParaInfo();
	InitReportHeader();
	UpdateReportData();

	ResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDefaultLogisticParaDlg::InitReportHeader( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(0,	_T("Commodity"),50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(1,	_T("Ratio(%)"),	30));
	pColumn->SetHeaderAlignment(DT_CENTER);	

}

void KDefaultLogisticParaDlg::UpdateReportData( void )
{
	m_wndReportContrl.ResetContent();
	try
	{
		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter = m_mapLogisticParaInfo.begin();
		AutoType end  = m_mapLogisticParaInfo.end();
		while(iter != end)
		{
			KLogisticParaInfo oInfo = iter->second;

			pRecord = m_wndReportContrl.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (oInfo.TstrPurposeName));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetItemData(oInfo.TnPurposeID);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);


			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdRatio, _T("%.3f")));
			pItem->SetAlignment(DT_RIGHT);
			pItem->SetEditable(TRUE);

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

void KDefaultLogisticParaDlg::InitLogisticParaInfo( void )
{
	m_mapLogisticParaInfo.clear();
	try
	{
		std::map<int, std::map<int, KLogisticParaInfo>> mapGetParaInfo;
		KDBaseDefaultLogistic::GetDefaultLogisticParameter(mapGetParaInfo);

		KPurposeInfoCollection oPurposeCollection(m_pTarget);

		AutoType iter = oPurposeCollection.begin();
		AutoType end  = oPurposeCollection.end();
		while(iter != end)
		{
			KPurpose* pPurpose = *iter;

			AutoType ofind = mapGetParaInfo.find(pPurpose->PurposeID());
			AutoType oend  = mapGetParaInfo.end();
			if (ofind != oend)
			{
				std::map<int, KLogisticParaInfo> mapParaInfo = ofind->second;

				AutoType msfind = mapParaInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
				AutoType mufind = mapParaInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
				AutoType mend   = mapParaInfo.end();

				if (mufind != mend)
				{
					KLogisticParaInfo oPara = mufind->second;
					oPara.TstrPurposeName = pPurpose->PurposeName();
					m_mapLogisticParaInfo.insert(std::make_pair(pPurpose->PurposeID(), oPara));

				}
				else if(msfind != mend)
				{
					KLogisticParaInfo oPara = msfind->second;
					oPara.TstrPurposeName = pPurpose->PurposeName();
					m_mapLogisticParaInfo.insert(std::make_pair(pPurpose->PurposeID(), oPara));
				}
				else
				{
					KLogisticParaInfo oPara;
					oPara.TstrPurposeName = pPurpose->PurposeName();
					oPara.TnPurposeID	  = pPurpose->PurposeID();
					oPara.TnMasterCode	  = KDefaultParameterMasterCode::NEW_MASTER_CODE;
					m_mapLogisticParaInfo.insert(std::make_pair(pPurpose->PurposeID(), oPara));
				}
			}
			else
			{
				KLogisticParaInfo oPara;
				oPara.TstrPurposeName = pPurpose->PurposeName();
				oPara.TnPurposeID	  = pPurpose->PurposeID();
				oPara.TnMasterCode	  = KDefaultParameterMasterCode::NEW_MASTER_CODE;
				m_mapLogisticParaInfo.insert(std::make_pair(pPurpose->PurposeID(), oPara));
			}
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


void KDefaultLogisticParaDlg::OnBnClickedDefault()
{
	UpdateReportData();
}


void KDefaultLogisticParaDlg::OnBnClickedOk()
{
	try
	{
		std::vector<KLogisticParaInfo> vecLogisticParaInfo;

		CXTPReportRecordItem* pItem				= nullptr;
		CXTPReportRecordItemNumber* pItemNum	= nullptr;

		CXTPReportRecords* pRecords = m_wndReportContrl.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);

			pItem = pRecord->GetItem(0);
			int nPursposeID = (int)pItem->GetItemData();

			KLogisticParaInfo oInfo;
			AutoType find = m_mapLogisticParaInfo.find(nPursposeID);
			AutoType end  = m_mapLogisticParaInfo.end();
			if (find != end)
			{
				oInfo = find->second;
			}
			else
			{
				oInfo.TnPurposeID = nPursposeID;
			}

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			oInfo.TdRatio = pItemNum->GetValue();

			vecLogisticParaInfo.push_back(oInfo);
		}
		KDBaseDefaultLogistic::SetDefaultLogisticParameter(vecLogisticParaInfo);

	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return;
	}
	KResizeDialogEx::OnOK();
}


void KDefaultLogisticParaDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KDefaultLogisticParaDlg::ResizeComponent( void )
{
	try
	{
		SetResize(IDC_STATIC_SETTING,		SZ_TOP_LEFT,       SZ_TOP_LEFT);
		SetResize(IDC_REPORT,				SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC10,				SZ_BOTTOM_LEFT,     SZ_BOTTOM_RIGHT);
		SetResize(IDDEFAULT,				SZ_BOTTOM_LEFT,     SZ_BOTTOM_LEFT);
		SetResize(IDOK,						SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,					SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);
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
