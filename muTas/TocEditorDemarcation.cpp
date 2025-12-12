// TocEditorNode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TocEditorDemarcation.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"

#include "ThemeStyleSave.h"
#include "DBaseConnector.h"
#include "ImTasDBase.h"
#include "ShapeEditController.h"
#include "XTPPropertyGridUtil.h"
#include "DlgShapeDemarcation.h"
#include "MultiCodeSettingDlg.h"
// KTocEditorDemarcation 대화 상자입니다.
#define IDD_PROPERTY_GRID 1000

IMPLEMENT_DYNAMIC(KTocEditorDemarcation, KDialogExBase)

KTocEditorDemarcation::KTocEditorDemarcation(CWnd* pParent /*=NULL*/)
	: KDialogExBase(KTocEditorDemarcation::IDD, pParent), m_pZoneTable(nullptr)
{
	DemarcationCategoryName     = _T("Demarcation");
	m_nPreDemarcationTemplateID = -1;
	DemarcationTemplateType     = 3;
}

KTocEditorDemarcation::~KTocEditorDemarcation()
{
	try
	{
        if (m_pZoneTable != nullptr)
            m_pZoneTable->Unregister(this);
	}
	catch (KExceptionPtr ex)
	{
	    TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KTocEditorDemarcation::DoDataExchange(CDataExchange* pDX)
{
	KDialogExBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_DUMMY, m_stcDummy);
	DDX_Control(pDX, IDC_COMBO_MODIFY_SELECT, m_cboModifyTask);
	DDX_Control(pDX, IDC_COMBO_TEMPLATE_NODE, m_cboTemplateDemarcation);
}


BEGIN_MESSAGE_MAP(KTocEditorDemarcation, KDialogExBase)
	ON_WM_CTLCOLOR()	
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_ADD, &KTocEditorDemarcation::OnBnClickedRadioAdd)
	ON_BN_CLICKED(IDC_RADIO_MODIFY, &KTocEditorDemarcation::OnBnClickedRadioModify)
	ON_CBN_SELCHANGE(IDC_COMBO_MODIFY_SELECT, &KTocEditorDemarcation::OnCbnSelchangeComboModifySelect)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &KTocEditorDemarcation::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &KTocEditorDemarcation::OnBnClickedButtonModify)

	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_DELETE_NODE, &KTocEditorDemarcation::OnBnClickedButtonTemplateDeleteDemarcation)
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_SAVE_NODE, &KTocEditorDemarcation::OnBnClickedButtonTemplateSaveDemarcation)
	ON_CBN_SELCHANGE(IDC_COMBO_TEMPLATE_NODE, &KTocEditorDemarcation::OnCbnSelchangeComboTemplateDemarcation)
    ON_BN_CLICKED(IDC_BUTTON1, &KTocEditorDemarcation::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &KTocEditorDemarcation::OnBnClickedButton2)
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, &KTocEditorDemarcation::OnPropertyGridItemValueChange)
END_MESSAGE_MAP()


// KTocEditorDemarcation 메시지 처리기입니다.
BOOL KTocEditorDemarcation::OnInitDialog()
{
	KDialogExBase::OnInitDialog();
	KDialogExBase::UseKeyEscEnter(false, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(255,255,255));
	CRect rectDummy;
	m_stcDummy.GetWindowRect(&rectDummy);
	ScreenToClient(&rectDummy);

	m_wndPropertyGrid.Create(rectDummy, this, IDD_PROPERTY_GRID);
	m_wndPropertyGrid.HighlightChangedItems(TRUE);
	m_wndPropertyGrid.SetVariableItemsHeight(TRUE);
	m_wndPropertyGrid.SetTheme(xtpGridThemeOffice2007);
	m_wndPropertyGrid.SetShowInplaceButtonsAlways(TRUE);
	m_wndPropertyGrid.SetHelpHeight(25);
	m_wndPropertyGrid.NavigateItems(TRUE, FALSE);

	m_cboModifyTask.AddString(_T("Select by Point"));
	m_cboModifyTask.AddString(_T("Select by Rectangle"));
	m_cboModifyTask.SetCurSel(0);

    m_wndPropertyGrid.ShowWindow(SW_HIDE);

    AddImChampTooltip(IDC_BUTTON1, _T("존 경계 정보를 변경"));
    AddImChampTooltip(IDC_BUTTON2, _T("존 경계 정보를 이용하여 Zone_ID를 다시 계산"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HBRUSH KTocEditorDemarcation::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogExBase::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

	return hbr;
}


void KTocEditorDemarcation::OnSize(UINT nType, int cx, int cy)
{
	KDialogExBase::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KTocEditorDemarcation::SetTargetMap( KTarget* a_pTarget, KMapView* a_pMapView )
{
	m_pTarget       = a_pTarget;
	m_pMapView      = a_pMapView;

    try
    {
        if (m_pZoneTable != nullptr)
            m_pZoneTable->Unregister(this);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }

	m_pZoneTable       = m_pTarget->Tables()->FindTable(TABLE_ZONE);	
	m_pZoneTable->Register(this);

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_MODIFY);
	pButton->SetCheck(BST_CHECKED);
	
	LoadInitialBlankProperties();
	LoadTemplateComboBoxDemarcation();	
	LoadDemarcationPropertiesValue();
	InitialControl();
}

void KTocEditorDemarcation::NotifyProcess( LPCTSTR a_strSubjectName, Integer a_nxObjectID )
{
    try
    {
        CString strSubjectName(a_strSubjectName);

        if(strSubjectName.CompareNoCase(TABLE_ZONE) == 0)
        {
            LoadInitialBlankProperties();
            LoadDemarcationPropertiesValue();
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }	
}

void KTocEditorDemarcation::LoadInitialBlankProperties()
{
	m_wndPropertyGrid.ResetContent();
	try
	{
		const
		KIOColumns* pIOColumns = m_pZoneTable->Columns();
		KCodeManager*        pCodeMgr            = m_pTarget->CodeManager();

		int nColumnCount = pIOColumns->ColumnCount();			

		CXTPPropertyGridItem* pPropertyGridItemParent = m_wndPropertyGrid.AddCategory( DemarcationCategoryName );
		for(int i = 0; i < nColumnCount; i++)
		{
			KIOColumn* pColumn = pIOColumns->GetColumn( i );     
			CXTPPropertyGridItem* pGridItem = NULL;

			if(pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
			{				
				pGridItem = KxtpPropertyGridUtil::AddCodeItem(pPropertyGridItemParent, m_pTarget, pColumn);
				CString strColumnName(pColumn->Name());
				if (strColumnName.CompareNoCase(COLUMN_NODE_TYPE) == 0)
				{
					CXTPPropertyGridItemEnum* pGridItemEnum = static_cast<CXTPPropertyGridItemEnum*>(pGridItem);
					pGridItemEnum->SetEnum(1);  //존은 안돼...
				}
			}
			else if(pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
			{
				pGridItem = KxtpPropertyGridUtil::AddMultiCodeItem(pPropertyGridItemParent, m_pTarget, pColumn);
			}
			else
			{
				pGridItem =	pPropertyGridItemParent->AddChildItem(new CXTPPropertyGridItem(pColumn->Caption()));
				pGridItem->SetItemData((DWORD_PTR)pColumn);
			}

			EditableCheck(pGridItem, pColumn);
		}
	
		pPropertyGridItemParent->Expand();
	}	
	catch (...)
	{
		//AfxMessageBox(_T("Propertis 초기화 중 오류가 발생하였습니다."));
	}	
}


void KTocEditorDemarcation::EditableCheck( CXTPPropertyGridItem* a_pPropertyGridItem, KIOColumn* a_pIOColumn )
{
	int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_MODIFY);
	if (nCheckedRadioBtn == IDC_RADIO_MODIFY)
	{
		a_pPropertyGridItem->SetReadOnly();
		return;
	}

	CString strColumnName(a_pIOColumn->Name());
	
	if (a_pIOColumn->PrimaryKey())
	{
		a_pPropertyGridItem->SetReadOnly();
		a_pPropertyGridItem->SetValue(TSTAR);
		return;
	}
}

void KTocEditorDemarcation::OnBnClickedRadioAdd()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_ADD);
	pButton->SetCheck(BST_CHECKED);
	LoadInitialBlankProperties();
	LoadDemarcationPropertiesValue();
	InitialControl();
}

void KTocEditorDemarcation::OnBnClickedButtonAdd()
{
	
}


void KTocEditorDemarcation::OnBnClickedRadioModify()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_MODIFY);
	pButton->SetCheck(BST_CHECKED);
	LoadInitialBlankProperties();
	InitialControl();
}

void KTocEditorDemarcation::OnCbnSelchangeComboModifySelect()
{
	// Task
	
}

void KTocEditorDemarcation::OnBnClickedButtonModify()
{
	KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
	if (spShapeEditController == NULL)
	{
		spShapeEditController = m_pMapView->ShapeEditStart();
	}

	if (spShapeEditController != NULL)
	{
		spShapeEditController->SetEditingTarget(KEMShapeEditingLayerDemarcation);
		spShapeEditController->SetEditingTask(KEMShapeEditingTaskMultiSelect);
	}
}

void KTocEditorDemarcation::InitialControl()
{
	CWnd* pCtrlWindow = NULL;

	int  nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_MODIFY);
	switch(nCheckedRadioBtn)
	{
	case IDC_RADIO_ADD:
		pCtrlWindow = GetDlgItem(IDC_BUTTON_ADD);
		pCtrlWindow->EnableWindow(TRUE);

		pCtrlWindow = GetDlgItem(IDC_COMBO_MODIFY_SELECT);
		pCtrlWindow->EnableWindow(FALSE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_MODIFY);
		pCtrlWindow->EnableWindow(FALSE);

		pCtrlWindow = GetDlgItem(IDC_COMBO_TEMPLATE_NODE);
		pCtrlWindow->EnableWindow(TRUE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_DELETE_NODE);
		pCtrlWindow->EnableWindow(TRUE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_SAVE_NODE);
		pCtrlWindow->EnableWindow(TRUE);		
		break;
	case IDC_RADIO_MODIFY:
	default:
		pCtrlWindow = GetDlgItem(IDC_BUTTON_ADD);
		pCtrlWindow->EnableWindow(FALSE);

		pCtrlWindow = GetDlgItem(IDC_COMBO_MODIFY_SELECT);
		pCtrlWindow->EnableWindow(TRUE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_MODIFY);
		pCtrlWindow->EnableWindow(TRUE);

		pCtrlWindow = GetDlgItem(IDC_COMBO_TEMPLATE_NODE);
		pCtrlWindow->EnableWindow(FALSE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_DELETE_NODE);
		pCtrlWindow->EnableWindow(FALSE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_SAVE_NODE);
		pCtrlWindow->EnableWindow(FALSE);		
		break;
	}
}

void KTocEditorDemarcation::OnBnClickedButtonTemplateDeleteDemarcation()
{
	int nCurSelectedIndex = m_cboTemplateDemarcation.GetCurSel();
	if (nCurSelectedIndex < 0)
		return;

	int nID = m_cboTemplateDemarcation.GetItemData(nCurSelectedIndex);
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}
	try
	{
		KProjectDBase::DeleteNetworkTemplate(spDBaseConnection, nID, DemarcationTemplateType);
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		AfxMessageBox(_T("탬플렛 삭제 중 오류가 발생하였습니다"));
	}

	LoadTemplateComboBoxDemarcation();
	LoadDemarcationPropertiesValue();
}


void KTocEditorDemarcation::OnBnClickedButtonTemplateSaveDemarcation()
{
	if (! DemarcationTemplateDataCheck())
		return;

	int nCurSelectedIndex = m_cboTemplateDemarcation.GetCurSel();
	KThemeStyleSave dlg;
	
	if (nCurSelectedIndex >= 0)
	{
		CString strExistStyleName;
		m_cboTemplateDemarcation.GetLBText(nCurSelectedIndex, strExistStyleName);
		dlg.SetExistStyleName(strExistStyleName);
	}

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	bool bOverwrite = dlg.IsOverWrite();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	CString strTemplateValue = ConvertTemplateSaveValueDemarcation();

	if (!bOverwrite)
	{
		CString strTemplateName = dlg.GetNewStyleName();
		try
		{
			KProjectDBase::InsertNetworkTemplate(spDBaseConnection, DemarcationTemplateType, strTemplateName, strTemplateValue);
		}
		catch (KExceptionPtr ex)
		{
			AfxMessageBox(ex->GetErrorMessage());
		}
		catch(...)
		{
			AfxMessageBox(_T("탬플렛 입력 중 오류가 발생하였습니다."));
		}
	}
	else
	{
		int nSelected = m_cboTemplateDemarcation.GetCurSel();
		if (nSelected < 0)
		{
			return;
		}

		int nID = m_cboTemplateDemarcation.GetItemData(nSelected);
		try
		{
			KProjectDBase::UpdateNetworkTemplate(spDBaseConnection, nID, DemarcationTemplateType, strTemplateValue);
		}
		catch (KExceptionPtr ex)
		{
			AfxMessageBox(ex->GetErrorMessage());
		}
		catch(...)
		{
			AfxMessageBox(_T("탬플렛 입력 중 오류가 발생하였습니다."));
		}
	}
		
	LoadTemplateComboBoxDemarcation();
}

void KTocEditorDemarcation::LoadTemplateComboBoxDemarcation()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	std::vector<TNetworkTemplate> vecTemplate;
	try
	{
		KProjectDBase::NetwrokTemplate(spDBaseConnection, DemarcationTemplateType, vecTemplate);
	}
	catch(...)
	{
		AfxMessageBox(_T("NetwrokTemplate Query Exception"));
		return;
	}
	
	m_cboTemplateDemarcation.ResetContent();

	int nSelectIndex = -1;
	for (size_t i=0; i<vecTemplate.size(); i++)
	{
		TNetworkTemplate& oTemplate = vecTemplate[i];
		int nIndex = m_cboTemplateDemarcation.AddString(oTemplate.TName);
		int nID    = oTemplate.TID;
		m_cboTemplateDemarcation.SetItemData(nIndex, nID);

		if (m_nPreDemarcationTemplateID == nID)
		{
			nSelectIndex = nIndex;
		}
	}

	if (m_cboTemplateDemarcation.GetCount() > 0)
	{
		if (nSelectIndex == -1)
		{
			m_cboTemplateDemarcation.SetCurSel(0);
			m_nPreDemarcationTemplateID = m_cboTemplateDemarcation.GetItemData(0);
		}
		else
		{
			m_cboTemplateDemarcation.SetCurSel(nSelectIndex);
		}
	}
}

bool KTocEditorDemarcation::DemarcationTemplateDataCheck()
{
	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( DemarcationCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CString strValue(_T(""));
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );
		if( pPropertyGridItem->GetReadOnly() == TRUE)
			continue;

		KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

		if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		{
			int nConst = pPropertyGridItem->GetConstraints()->FindConstraint(pPropertyGridItem->GetValue());
            if (nConst >= 0)
            {
			    CXTPPropertyGridItemConstraint* pGridConst = pPropertyGridItem->GetConstraints()->GetConstraintAt( nConst );
			    strValue.Format(_T("%d"), pGridConst->m_dwData);
            }
		}
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
		{
			strValue = pPropertyGridItem->GetValue();
		}
		else
		{
			strValue = pPropertyGridItem->GetValue();
			if (pColumn->DataType() == KEMIODataTypeInteger)
			{
				if (!QBicStringChecker::IsSignedInteger(strValue) && strValue.GetLength() > 0)
				{
					CString strMsg;
					strMsg.Format(_T("%s - 정수형 숫자를 입력해 주세요."), pColumn->Caption());
					AfxMessageBox(strMsg);
					pPropertyGridItem->SetValue(_T(""));
					pPropertyGridItem->Select();
					return false;
				}
			}

			if(pColumn->DataType() == KEMIODataTypeDouble)
			{
				if (!QBicStringChecker::IsNumeric(strValue) && strValue.GetLength() > 0)
				{
					CString strMsg;
					strMsg.Format(_T("%s - 실수형 숫자를 입력해 주세요."), pColumn->Caption());
					AfxMessageBox(strMsg);
					pPropertyGridItem->SetValue(_T(""));
					pPropertyGridItem->Select();
					return false;
				}
			}
		}
	}

	return true;
}

CString KTocEditorDemarcation::ConvertTemplateSaveValueDemarcation()
{
	CString strOut;

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( DemarcationCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CString strValue(_T(""));
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
		KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

		if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		{
			int nConst = pPropertyGridItem->GetConstraints()->FindConstraint(pPropertyGridItem->GetValue());
            if (nConst >= 0)
            {
			    CXTPPropertyGridItemConstraint* pGridConst = pPropertyGridItem->GetConstraints()->GetConstraintAt( nConst );
			    strValue.Format(_T("%d"), pGridConst->m_dwData);
            }
		}
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
		{
			strValue = pPropertyGridItem->GetValue();
		}
		else
		{
			strValue = pPropertyGridItem->GetValue();
		}

		// [컬럼이름@Value]~[컬럼이름@Value] 
		CString strTemp;
		strTemp.Format(_T("[%s@%s]~"), pColumn->Name(), strValue);

		strOut.Append(strTemp);
	}

	return strOut;
}

void KTocEditorDemarcation::LoadDemarcationPropertiesValue()
{
	if (m_cboTemplateDemarcation.GetCount() < 1)
		return;

	int nSelectedIndex = m_cboTemplateDemarcation.GetCurSel();
	if (nSelectedIndex < 0)
	{
		nSelectedIndex = 0;
		m_cboTemplateDemarcation.SetCurSel(0);
	}

	int nID = m_cboTemplateDemarcation.GetItemData(nSelectedIndex);

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}
	std::map<CString, CString> mapTemplateValueNode;
	KProjectDBase::NetwrokTemplate(spDBaseConnection, nID, DemarcationTemplateType, mapTemplateValueNode);

	if (mapTemplateValueNode.size() < 1)
	{
		return;
	}

	std::map<CString, CString>::iterator iter = mapTemplateValueNode.begin();
	std::map<CString, CString>::iterator end  = mapTemplateValueNode.end();

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( DemarcationCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
		KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

		iter = mapTemplateValueNode.find(pColumn->Name());
		if (iter == end)
			continue;

		CString strValue = iter->second;

		if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		{
			CXTPPropertyGridItemEnum* pGridItem = static_cast<CXTPPropertyGridItemEnum*>(pPropertyGridItem);
			if(pGridItem == NULL)
			{
				continue;
			}

			pGridItem->SetEnum(_ttoi(strValue));
		}
		else if( pColumn->CodeType() == KEMIOCodeTypeIsMultiCode )
		{
			pPropertyGridItem->SetValue(strValue);
		}
		else
		{
			pPropertyGridItem->SetValue(strValue);
		}
	}
}


void KTocEditorDemarcation::OnCbnSelchangeComboTemplateDemarcation()
{
	LoadDemarcationPropertiesValue();
}

void KTocEditorDemarcation::ActionFeedback( KDialogFeedback::KEMActionFeedback emActionCode )
{
    try
    {
        if (emActionCode == KDialogFeedback::KEMActionFeedbackDeActivate)
        {
            StopEditing();
        }
        else if(emActionCode == KDialogFeedback::KEMActionFeedbackActivate)
        {
            LoadTemplateComboBoxDemarcation();
        }
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
        TxLogDebugException();
    }    
}

void KTocEditorDemarcation::StopEditing()
{
    try
    {
        m_pMapView->StopShapeEdit();
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
        TxLogDebugException();
    }    
}


void KTocEditorDemarcation::OnBnClickedButton1()
{
    try
    {
		KIOTable* pTableZone = m_pTarget->Tables()->FindTable(TABLE_ZONE);

        KDlgShapeDemarcation oDlg(m_pTarget, pTableZone);
        oDlg.DoModal();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }    
}


void KTocEditorDemarcation::OnBnClickedButton2()
{
    try
    {
        int nResult = -1;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nResult = AfxMessageBox(_T("경계정보를 생성하였습니다.\n새 경계 정보를 이용하여 'Zone_ID' 컬럼을 업데이트 하시겠습니까?"), MB_OKCANCEL);
		}
		else
		{
			nResult = AfxMessageBox(_T("The Zone Demarcation has been created.\nDo you want to update the 'Zone_ID' column with the new Demarcation?"), MB_OKCANCEL);
		}

        if (nResult == IDOK)
        {
            KDlgShapeDemarcation::SpatialCalculateZoneID(this, m_pTarget);

            KIOTable* pIOTable = m_pTarget->Tables()->FindTable(TABLE_NODE);
            pIOTable->Notify();
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }    
}


LRESULT KTocEditorDemarcation::OnPropertyGridItemValueChange(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_INPLACEBUTTONDOWN)
	{
		CXTPPropertyGridInplaceButton* pButton = (CXTPPropertyGridInplaceButton*)lParam;
		CXTPPropertyGridItem* pItem = pButton->GetItem();
		KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();
		if (pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
		{
			CString strCodeList = pItem->GetValue();
			KMultiCodeSettingDlg oDlg(m_pTarget, pColumn,strCodeList);
			if (oDlg.DoModal() == IDOK)
			{
				pItem->SetValue(oDlg.GetResult());
			}
		}
	}
	return 0;
}
