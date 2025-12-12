// DefaultDistributionParaSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultDistributionParaSettingDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "DefineNamesModels.h"
#include "PurposeInfoCollection.h"
#include "Purpose.h"
#include "DefineNamesDefaultPara.h"
#include "CustomReportRecordItem.h"

// DefaultDistributionParaSetting 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultDistributionParaSettingDlg, KResizeDialogEx)

	KDefaultDistributionParaSettingDlg::KDefaultDistributionParaSettingDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultDistributionParaSettingDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KDefaultDistributionParaSettingDlg::~KDefaultDistributionParaSettingDlg()
{
}

void KDefaultDistributionParaSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportContrl);
}


BEGIN_MESSAGE_MAP(KDefaultDistributionParaSettingDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDDEFAULT, &KDefaultDistributionParaSettingDlg::OnBnClickedDefault)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT,	OnReportItemValueChangedType)
	ON_BN_CLICKED(IDOK, &KDefaultDistributionParaSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KDefaultDistributionParaSettingDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// DefaultDistributionParaSetting 메시지 처리기입니다.


BOOL KDefaultDistributionParaSettingDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	ResizeComponet();

	KReportCtrlSetting::Default(m_wndReportContrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportContrl);

	InitDistributionParaInfo();
	InitReportHeader();
	UpdateReportData();

	return TRUE;  
}

void KDefaultDistributionParaSettingDlg::InitReportHeader( void )
{
	try
	{
		CXTPReportColumn* pColumn = NULL;

		if (KmzSystem::IsPassengerSystem())
		{
			pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(0,	_T("Purpose"), 50));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetAlignment(xtpColumnIconCenter);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pColumn->SetCaption(_T("통행목적"));
			}
		}
		else
		{
			pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(0,	_T("Commodity"),50));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetAlignment(xtpColumnIconCenter);
		}

		pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(1,	_T("Function Type"),40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->GetEditOptions()->AddComboButton(TRUE);
		pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
		pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("모형식 유형"));
		}

		CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
		CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
		pEditOptions->AddConstraint(KDistrbutionFunction::Function1_Name, KDistrbutionFunction::Function1_Code);
		pEditOptions->AddConstraint(KDistrbutionFunction::Function2_Name, KDistrbutionFunction::Function2_Code);
		pEditOptions->AddConstraint(KDistrbutionFunction::Function3_Name, KDistrbutionFunction::Function3_Code);

		pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(2,	_T("Parameter(α)"),	30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("계수(α)"));
		}

		pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(3,	_T("Parameter(β)"),	30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("계수(β)"));
		}

		pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(4,	_T("Parameter(θ)"),	30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("계수(θ)"));
		}

		pColumn = m_wndReportContrl.AddColumn(new CXTPReportColumn(5,	_T("Variable(Rij)"),50));
		pColumn->SetHeaderAlignment(DT_CENTER);	
		pColumn->GetEditOptions()->AddComboButton(TRUE);
		pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
		pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("변수(Rij)"));
		}


		pEditOptions = pColumn->GetEditOptions();
		pConstraints = pEditOptions->GetConstraints();

		KIOTable* pODTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
		const KIOColumns* pColumns = pODTable->Columns();
		int nColumnCount = pColumns->ColumnCount();
		for(int i = 0; i < nColumnCount; ++i)
		{
			KIOColumn* pODColumn = pColumns->GetColumn(i);
			if (nullptr != pODColumn &&  KEMIODataTypeDouble ==  pODColumn->DataType())
			{
				pEditOptions->AddConstraint(pODColumn->Caption(), (DWORD_PTR)pODColumn);
			}
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

void KDefaultDistributionParaSettingDlg::InitDistributionParaInfo( void )
{
	try
	{
		m_mapDistributionPara.clear();

		std::map<int, std::map<int, KDistributionParaInfo>> mapGetParaInfo;
		KDBaseDefaultDistribution::GetDefaultDistributionParameter(mapGetParaInfo);

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
				std::map<int, KDistributionParaInfo> mapParaInfo = ofind->second;

				AutoType msfind = mapParaInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
				AutoType mufind = mapParaInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
				AutoType mend   = mapParaInfo.end();

				if (mufind != mend)
				{
					KDistributionParaInfo oPara = mufind->second;
					oPara.TstrPurposeName = pPurpose->PurposeName();
					m_mapDistributionPara.insert(std::make_pair(pPurpose->PurposeID(), oPara));

				}
				else if(msfind != mend)
				{
					KDistributionParaInfo oPara = msfind->second;
					oPara.TstrPurposeName = pPurpose->PurposeName();
					m_mapDistributionPara.insert(std::make_pair(pPurpose->PurposeID(), oPara));
				}
				else
				{
					KDistributionParaInfo oPara;
					oPara.TstrPurposeName = pPurpose->PurposeName();
					oPara.TnPurposeID	  = pPurpose->PurposeID();
					oPara.TnFunctionType  = KDistrbutionFunction::Function1_Code;
					oPara.TnMasterCode	  = KDefaultParameterMasterCode::NEW_MASTER_CODE;
					m_mapDistributionPara.insert(std::make_pair(pPurpose->PurposeID(), oPara));
				}
			}
			else
			{
				KDistributionParaInfo oPara;
				oPara.TstrPurposeName = pPurpose->PurposeName();
				oPara.TnPurposeID	  = pPurpose->PurposeID();
				oPara.TnFunctionType  = KDistrbutionFunction::Function1_Code;
				oPara.TnMasterCode	  = KDefaultParameterMasterCode::NEW_MASTER_CODE;
				m_mapDistributionPara.insert(std::make_pair(pPurpose->PurposeID(), oPara));
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

void KDefaultDistributionParaSettingDlg::UpdateReportData( void )
{
	m_wndReportContrl.ResetContent();
	try
	{
		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		AutoType iter = m_mapDistributionPara.begin();
		AutoType end  = m_mapDistributionPara.end();
		while(iter != end)
		{
			KDistributionParaInfo oInfo = iter->second;

			pRecord = m_wndReportContrl.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.TstrPurposeName));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetItemData(oInfo.TnPurposeID);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CCustomReportRecordItem);
			pItem->SetItemData(oInfo.TnFunctionType);
			pItem->SetAlignment(DT_CENTER);

			if (oInfo.TnFunctionType == KDistrbutionFunction::Function2_Code ||oInfo.TnFunctionType == KDistrbutionFunction::Function3_Code )
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdParameterAlpa, _T("%.6f")));
				pItem->SetAlignment(DT_RIGHT);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.6f")));
				pItem->SetAlignment(DT_RIGHT);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdParameterBeta, _T("%.6f")));
			pItem->SetAlignment(DT_RIGHT);
			
			if (oInfo.TnFunctionType == KDistrbutionFunction::Function3_Code )
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdParameterTheta, _T("%.6f")));
				pItem->SetAlignment(DT_RIGHT);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.6f")));
				pItem->SetAlignment(DT_RIGHT);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
			}

			pItem = pRecord->AddItem(new CCustomReportRecordItem);

			KIOTable* pODTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
			const KIOColumns* pColumns = pODTable->Columns();
			int nColumnCount = pColumns->ColumnCount();
			for(int i = 0; i < nColumnCount; ++i)
			{
				KIOColumn* pODColumn = pColumns->GetColumn(i);
				if (nullptr != pODColumn &&  KEMIODataTypeDouble ==  pODColumn->DataType())
				{
					CString strCaption = pODColumn->Caption();
					if (oInfo.TstrVariable.CompareNoCase(strCaption) == 0)
					{
						pItem->SetItemData((DWORD_PTR) pODColumn);
					}
				}
			}
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


void KDefaultDistributionParaSettingDlg::OnBnClickedDefault()
{
	UpdateReportData();
}


void KDefaultDistributionParaSettingDlg::OnBnClickedOk()
{
	if (SaveResultParameter()== false)
	{
		return;
	}

	KResizeDialogEx::OnOK();
}


void KDefaultDistributionParaSettingDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KDefaultDistributionParaSettingDlg::OnReportItemValueChangedType( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		
		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == 1)
		{
			CXTPReportRecord* pRecord				= pItemNotify->pRow->GetRecord();
			CCustomReportRecordItem* pTypeComboitem = (CCustomReportRecordItem*)pRecord->GetItem(1);
			CXTPReportRecordItemNumber*	pItemNum	= nullptr;

			int nType = pTypeComboitem->GetItemData();
			if (nType == KDistrbutionFunction::Function1_Code)
			{
				 pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
				 pItemNum->SetEditable(FALSE);
				 pItemNum->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				 pItemNum->SetValue(0.0);

				 pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
				 pItemNum->SetEditable(TRUE);
				 double dGetBeta = pItemNum->GetValue();
				 pItemNum->SetBackgroundColor(RGB(255, 255, 255));
				 pItemNum->SetValue(dGetBeta);

				 pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
				 pItemNum->SetEditable(FALSE);
				 pItemNum->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				 pItemNum->SetValue(0.0);

			}
			else if (nType == KDistrbutionFunction::Function2_Code)
			{
				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
				pItemNum->SetEditable(TRUE);
				double dGetAlpha = pItemNum->GetValue();
				pItemNum->SetBackgroundColor(RGB(255, 255, 255));
				pItemNum->SetValue(dGetAlpha);

				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
				pItemNum->SetEditable(TRUE);
				double dGetBeta = pItemNum->GetValue();
				pItemNum->SetBackgroundColor(RGB(255, 255, 255));
				pItemNum->SetValue(dGetBeta);

				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
				pItemNum->SetEditable(FALSE);
				pItemNum->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItemNum->SetValue(0.0);

			}
			else if (nType == KDistrbutionFunction::Function3_Code)
			{
				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
				pItemNum->SetEditable(TRUE);
				double dGetAlpha = pItemNum->GetValue();
				pItemNum->SetBackgroundColor(RGB(255, 255, 255));
				pItemNum->SetValue(dGetAlpha);

				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
				pItemNum->SetEditable(TRUE);
				double dGetBeta = pItemNum->GetValue();
				pItemNum->SetBackgroundColor(RGB(255, 255, 255));
				pItemNum->SetValue(dGetBeta);

				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
				pItemNum->SetEditable(TRUE);
				double dTheta = pItemNum->GetValue();
				pItemNum->SetBackgroundColor(RGB(255, 255, 255));
				pItemNum->SetValue(dTheta);
			}
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

bool KDefaultDistributionParaSettingDlg::SaveResultParameter( void )
{
	std::vector<KDistributionParaInfo> vecReusultPara;

	try
	{
		CXTPReportRecordItem* pItem				= nullptr;
		CXTPReportRecordItemNumber* pItemNum	= nullptr;

		CXTPReportRecords* pRecords = m_wndReportContrl.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			
			pItem = pRecord->GetItem(0);
			int nPursposeID = (int)pItem->GetItemData();

			KDistributionParaInfo oInfo;
			AutoType find = m_mapDistributionPara.find(nPursposeID);
			AutoType end  = m_mapDistributionPara.end();
			if (find != end)
			{
				oInfo = find->second;
			}
			else
			{
				oInfo.TnPurposeID = nPursposeID;
			}

			pItem = pRecord->GetItem(1);
			oInfo.TnFunctionType = (int)pItem->GetItemData();

			if (oInfo.TnFunctionType == KDistrbutionFunction::Function1_Code)
			{
				oInfo.TdParameterAlpa = 0.0;

				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
				oInfo.TdParameterBeta = pItemNum->GetValue();

				oInfo.TdParameterTheta = 0.0;
			}
			else if (oInfo.TnFunctionType == KDistrbutionFunction::Function2_Code)
			{
				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
				oInfo.TdParameterAlpa = pItemNum->GetValue();

				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
				oInfo.TdParameterBeta = pItemNum->GetValue();

				oInfo.TdParameterTheta = 0.0;
			}
			else if (oInfo.TnFunctionType == KDistrbutionFunction::Function3_Code)
			{
				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
				oInfo.TdParameterAlpa = pItemNum->GetValue();

				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
				oInfo.TdParameterBeta = pItemNum->GetValue();

				pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
				oInfo.TdParameterTheta = pItemNum->GetValue();
			}
			else
			{
				continue;
			}

			pItem = pRecord->GetItem(5);

// 			CCustomReportRecordItem* pComboItem = (CCustomReportRecordItem*)pRecord->GetItem(5);
// 			KIOColumn* pTColumn = (KIOColumn*)pComboItem->GetItemData();

			KIOColumn* pODColumn = (KIOColumn*)pItem->GetItemData();
			oInfo.TstrVariable = pODColumn->Caption();

			vecReusultPara.push_back(oInfo);
		}

		KDBaseDefaultDistribution::SetDefaultDistributionParameter(vecReusultPara);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

void KDefaultDistributionParaSettingDlg::ResizeComponet( void )
{
	try
	{
		SetResize(IDC_STATIC_SETTING,		SZ_TOP_LEFT,    SZ_TOP_LEFT);
		SetResize(IDC_REPORT,				SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC10,			SZ_BOTTOM_LEFT,     SZ_BOTTOM_RIGHT);
		SetResize(IDDEFAULT,			SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
		SetResize(IDOK,					SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,				SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);

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
