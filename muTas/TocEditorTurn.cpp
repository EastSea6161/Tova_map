// TocEditorNode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TocEditorTurn.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"

#include "ThemeStyleSave.h"
#include "DBaseConnector.h"
#include "ImTasDBase.h"
#include "ShapeEditController.h"
#include "XTPPropertyGridUtil.h"
#include "ImChampFrameWindow.h"
#include "MultiCodeSettingDlg.h"

// KTocEditorTurn 대화 상자입니다.
#define IDD_PROPERTY_GRID 1000

IMPLEMENT_DYNAMIC(KTocEditorTurn, KDialogExBase)

KTocEditorTurn::KTocEditorTurn(CWnd* pParent /*=NULL*/)
	: KDialogExBase(KTocEditorTurn::IDD, pParent)
{
	TurnCategoryName     = _T("Turn");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		TurnCategoryName     = _T("회전제약");
	}
	m_nPreTurnTemplateID = -1;
	TurnTemplateType     = 3;
	m_pTurnTable         = nullptr;

    m_nxTurnID = -1;
}

KTocEditorTurn::~KTocEditorTurn()
{
    try
    {
        if (m_pTurnTable != nullptr)
            m_pTurnTable->Unregister(this);
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

void KTocEditorTurn::DoDataExchange(CDataExchange* pDX)
{
	KDialogExBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_DUMMY, m_stcDummy);
	DDX_Control(pDX, IDC_COMBO_MODIFY_SELECT, m_cboModifyTask);
	DDX_Control(pDX, IDC_COMBO_TEMPLATE_TURN, m_cboTemplateTurn);
}


BEGIN_MESSAGE_MAP(KTocEditorTurn, KDialogExBase)
	ON_WM_CTLCOLOR()	
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_ADD,    &KTocEditorTurn::OnBnClickedRadioAdd)
	ON_BN_CLICKED(IDC_RADIO_MODIFY, &KTocEditorTurn::OnBnClickedRadioModify)
	ON_CBN_SELCHANGE(IDC_COMBO_MODIFY_SELECT, &KTocEditorTurn::OnCbnSelchangeComboModifySelect)
    ON_BN_CLICKED(IDC_BUTTON_START, &KTocEditorTurn::OnBnClickedButtonStart)
    ON_BN_CLICKED(IDC_BUTTON_END,   &KTocEditorTurn::OnBnClickedButtonEnd)

	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_DELETE_TURN, &KTocEditorTurn::OnBnClickedButtonTemplateDeleteTurn)
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_SAVE_TURN, &KTocEditorTurn::OnBnClickedButtonTemplateSaveTurn)
	ON_CBN_SELCHANGE(IDC_COMBO_TEMPLATE_TURN, &KTocEditorTurn::OnCbnSelchangeComboTemplateTurn)

	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, &KTocEditorTurn::OnPropertyGridItemValueChange)
END_MESSAGE_MAP()


// KTocEditorTurn 메시지 처리기입니다.
BOOL KTocEditorTurn::OnInitDialog()
{
	KDialogExBase::OnInitDialog();
	KDialogExBase::UseKeyEscEnter(false, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
    AddImChampTooltip(IDC_RADIO_ADD,    _T("회전정보 추가"));
    AddImChampTooltip(IDC_RADIO_MODIFY, _T("회전정보 삭제"));
    AddImChampTooltip(IDC_BUTTON_START, _T("편집 시작"));
    AddImChampTooltip(IDC_BUTTON_END,   _T("편집 종료"));

    AddImChampTooltip(IDC_BUTTON_TEMPLATE_DELETE_TURN, _T("회전정보 입력 템플릿 삭제"));
    AddImChampTooltip(IDC_BUTTON_TEMPLATE_SAVE_TURN,   _T("회전정보 입력 템플릿 추가"));

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
		
	m_cboModifyTask.AddString(_T("Delete Turn"));
	m_cboModifyTask.AddString(_T("Delete Turns"));
	m_cboModifyTask.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HBRUSH KTocEditorTurn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogExBase::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

	return hbr;
}


void KTocEditorTurn::OnSize(UINT nType, int cx, int cy)
{
	KDialogExBase::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KTocEditorTurn::SetTargetMap( KTarget* a_pTarget, KMapView* a_pMapView )
{
	m_pTarget       = a_pTarget;
	m_pMapView      = a_pMapView;

    try
    {
        if (m_pTurnTable != nullptr)
            m_pTurnTable->Unregister(this);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }

	m_pTurnTable       = m_pTarget->Tables()->FindTable(TABLE_TURN);	
	m_pTurnTable->Register(this);

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_ADD);
	pButton->SetCheck(BST_CHECKED);
	
	LoadInitialBlankProperties();
	LoadTemplateComboBoxTurn();	
	LoadTurnPropertiesValue();
	InitialControl();
}

void KTocEditorTurn::NotifyProcess( LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{	
    try
    {
        std::map<CString, CString> mapTemplateValue;
        GetCurrentTurnTemplateValue(mapTemplateValue);

        if(_tcsicmp(a_strSubjectName, TABLE_TURN) == 0)
        {
            LoadInitialBlankProperties();
            LoadTurnPropertiesValue(mapTemplateValue);
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

void KTocEditorTurn::LoadInitialBlankProperties()
{
	m_wndPropertyGrid.ResetContent();
	try
	{
		const
		KIOColumns* pIOColumns = m_pTurnTable->Columns();
		KCodeManager*        pCodeMgr            = m_pTarget->CodeManager();

		int nColumnCount = pIOColumns->ColumnCount();			

		CXTPPropertyGridItem* pPropertyGridItemParent = m_wndPropertyGrid.AddCategory( TurnCategoryName );
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
			}

			EditableCheck(pGridItem, pColumn);
            pGridItem->SetItemData((DWORD_PTR)pColumn);
		}
	
		pPropertyGridItemParent->Expand();
	}	
	catch (...)
	{
		//AfxMessageBox(_T("Propertis 초기화 중 오류가 발생하였습니다."));
	}	
}


void KTocEditorTurn::EditableCheck( CXTPPropertyGridItem* a_pPropertyGridItem, KIOColumn* a_pIOColumn )
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

	if (strColumnName.CompareNoCase(COLUMN_TURN_F_LINK) == 0)
	{
		a_pPropertyGridItem->SetReadOnly();
		a_pPropertyGridItem->SetValue(TSTAR);
		return;
	}

	if (strColumnName.CompareNoCase(COLUMN_TURN_NODE) == 0)
	{
		a_pPropertyGridItem->SetReadOnly();
		a_pPropertyGridItem->SetValue(TSTAR);
		return;
	}
	
	if (strColumnName.CompareNoCase(COLUMN_TURN_T_LINK) == 0)
	{
		a_pPropertyGridItem->SetReadOnly();
		a_pPropertyGridItem->SetValue(TSTAR);
		return;
	}
}

void KTocEditorTurn::OnBnClickedRadioAdd()
{
    m_nxTurnID = -1;

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_ADD);
	pButton->SetCheck(BST_CHECKED);
	LoadInitialBlankProperties();
	LoadTurnPropertiesValue();
	InitialControl();

	AddTurnStart();
}

void KTocEditorTurn::AddTurnStart()
{
    try
    {
        std::map<CString, CString> mapKeyValue;
        GetCurrentTurnTemplateValue(mapKeyValue);

        KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
        if (spShapeEditController == NULL)
        {
            spShapeEditController = m_pMapView->ShapeEditStart();
        }

        if (spShapeEditController != NULL)
        {
            spShapeEditController->SetEditingTarget(KEMShapeEditingLayerTurn);
            spShapeEditController->SetEditingTask(KEMShapeEditingTaskInsert);
            spShapeEditController->SetDefaultTurnInfo(mapKeyValue);
        }

        ImChampFrameWindow::ShowTableView(m_pTarget, TABLE_TURN);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }
	if (TurnTemplateDataCheck() == false)
	{		
		return;		
	}
}

LRESULT KTocEditorTurn::OnPropertyGridItemValueChange(WPARAM wParam, LPARAM lParam)
{
    if(wParam != XTP_PGN_ITEMVALUE_CHANGED)
        return 0;

    KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
    if (TurnTemplateDataCheck()) {
        std::map<CString, CString> mapKeyValue; {
            GetCurrentTurnTemplateValue(mapKeyValue);
        }
        
        if (spShapeEditController)
            spShapeEditController->SetDefaultTurnInfo(mapKeyValue);
        
        UpdateTurn(mapKeyValue);
    }

	return 0;
}

void KTocEditorTurn::OnBnClickedRadioModify()
{
    m_nxTurnID = -1;

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_MODIFY);
	pButton->SetCheck(BST_CHECKED);
	LoadInitialBlankProperties();
	InitialControl();

	DeleteTurnStart();
}

void KTocEditorTurn::OnCbnSelchangeComboModifySelect()
{
	// Task
	
}

void KTocEditorTurn::DeleteTurnStart()
{
	// Delete
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_MODIFY);
	pButton->SetCheck(BST_CHECKED);
	LoadInitialBlankProperties();
	InitialControl();

	KShapeEditControllerPtr spShapeEditController = m_pMapView->GetShapeEditController();
	if (spShapeEditController == NULL)
	{
		spShapeEditController = m_pMapView->ShapeEditStart();
	}

	if (spShapeEditController != NULL)
	{
		spShapeEditController->SetEditingTarget(KEMShapeEditingLayerTurn);
		spShapeEditController->SetEditingTask(KEMShapeEditingTaskMultiSelect);
	}
    ImChampFrameWindow::ShowTableView(m_pTarget, TABLE_TURN);
}

void KTocEditorTurn::OnBnClickedButtonStart()
{
    int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_MODIFY);

    switch(nCheckedRadioBtn)
    {
    case IDC_RADIO_ADD:
        AddTurnStart();
        break;
    case IDC_RADIO_MODIFY:
        DeleteTurnStart();
        break;
    default:
        break;
    }
}

void KTocEditorTurn::OnBnClickedButtonEnd()
{
    m_nxTurnID = -1;
	StopEditing();
    InitialControl();
}

void KTocEditorTurn::InitialControl()
{
	CWnd* pCtrlWindow = NULL;

	int  nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_ADD, IDC_RADIO_MODIFY);
	switch(nCheckedRadioBtn)
	{
	case IDC_RADIO_ADD:
		pCtrlWindow = GetDlgItem(IDC_COMBO_MODIFY_SELECT);
		pCtrlWindow->EnableWindow(FALSE);
        		
		pCtrlWindow = GetDlgItem(IDC_COMBO_TEMPLATE_TURN);
		pCtrlWindow->EnableWindow(TRUE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_DELETE_TURN);
		pCtrlWindow->EnableWindow(TRUE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_SAVE_TURN);
		pCtrlWindow->EnableWindow(TRUE);		
		break;
	case IDC_RADIO_MODIFY:
	default:
		pCtrlWindow = GetDlgItem(IDC_COMBO_MODIFY_SELECT);
		pCtrlWindow->EnableWindow(TRUE);

		pCtrlWindow = GetDlgItem(IDC_COMBO_TEMPLATE_TURN);
		pCtrlWindow->EnableWindow(FALSE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_DELETE_TURN);
		pCtrlWindow->EnableWindow(FALSE);

		pCtrlWindow = GetDlgItem(IDC_BUTTON_TEMPLATE_SAVE_TURN);
		pCtrlWindow->EnableWindow(FALSE);		
		break;
	}

    if (nCheckedRadioBtn == IDC_RADIO_ADD) {
        if (m_nxTurnID >= 0)
            m_wndPropertyGrid.ShowWindow(SW_SHOW);
        else
            m_wndPropertyGrid.ShowWindow(SW_HIDE);
    }
    else {
        m_wndPropertyGrid.ShowWindow(SW_HIDE);
    }    
}

void KTocEditorTurn::OnBnClickedButtonTemplateDeleteTurn()
{
	int nCurSelectedIndex = m_cboTemplateTurn.GetCurSel();
	if (nCurSelectedIndex < 0)
		return;

	int nID = m_cboTemplateTurn.GetItemData(nCurSelectedIndex);
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}
	try
	{
		KProjectDBase::DeleteNetworkTemplate(spDBaseConnection, nID, TurnTemplateType);
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		AfxMessageBox(_T("탬플렛 삭제 중 오류가 발생하였습니다"));
	}

	LoadTemplateComboBoxTurn();
	LoadTurnPropertiesValue();
}

void KTocEditorTurn::OnBnClickedButtonTemplateSaveTurn()
{
	if (! TurnTemplateDataCheck())
		return;

	int nCurSelectedIndex = m_cboTemplateTurn.GetCurSel();
	KThemeStyleSave dlg;
	
	if (nCurSelectedIndex >= 0)
	{
		CString strExistStyleName;
		m_cboTemplateTurn.GetLBText(nCurSelectedIndex, strExistStyleName);
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

	CString strTemplateValue = ConvertTemplateSaveValueTurn();

	if (!bOverwrite)
	{
		CString strTemplateName = dlg.GetNewStyleName();
		try
		{
			KProjectDBase::InsertNetworkTemplate(spDBaseConnection, TurnTemplateType, strTemplateName, strTemplateValue);
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
		int nSelected = m_cboTemplateTurn.GetCurSel();
		if (nSelected < 0)
		{
			return;
		}

		int nID = m_cboTemplateTurn.GetItemData(nSelected);
		try
		{
			KProjectDBase::UpdateNetworkTemplate(spDBaseConnection, nID, TurnTemplateType, strTemplateValue);
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
		
	LoadTemplateComboBoxTurn();
}

void KTocEditorTurn::LoadTemplateComboBoxTurn()
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
		KProjectDBase::NetwrokTemplate(spDBaseConnection, TurnTemplateType, vecTemplate);
	}
	catch(...)
	{
		AfxMessageBox(_T("NetwrokTemplate Query Exception"));
		return;
	}
	
	m_cboTemplateTurn.ResetContent();

	int nSelectIndex = -1;
	for (size_t i=0; i<vecTemplate.size(); i++)
	{
		TNetworkTemplate& oTemplate = vecTemplate[i];
		int nIndex = m_cboTemplateTurn.AddString(oTemplate.TName);
		int nID    = oTemplate.TID;
		m_cboTemplateTurn.SetItemData(nIndex, nID);

		if (m_nPreTurnTemplateID == nID)
		{
			nSelectIndex = nIndex;
		}
	}

	if (m_cboTemplateTurn.GetCount() > 0)
	{
		if (nSelectIndex == -1)
		{
			m_cboTemplateTurn.SetCurSel(0);
			m_nPreTurnTemplateID = m_cboTemplateTurn.GetItemData(0);
		}
		else
		{
			m_cboTemplateTurn.SetCurSel(nSelectIndex);
		}
	}
}

bool KTocEditorTurn::TurnTemplateDataCheck()
{
	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( TurnCategoryName );
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

CString KTocEditorTurn::ConvertTemplateSaveValueTurn()
{
	CString strOut;

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( TurnCategoryName );
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

void KTocEditorTurn::GetCurrentTurnTemplateValue(std::map<CString, CString>& ar_mapTemplateValue)
{
	ar_mapTemplateValue.clear();

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( TurnCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CString strValue(_T(""));
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
		KIOColumn* pColumn       = (KIOColumn*)pPropertyGridItem->GetItemData();
		CString    strColumnName = pColumn->Name();

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

		ar_mapTemplateValue.insert(std::make_pair(strColumnName, strValue));
	}
}

void KTocEditorTurn::LoadTurnPropertiesValue()
{
	if (m_cboTemplateTurn.GetCount() < 1)
		return;

	int nSelectedIndex = m_cboTemplateTurn.GetCurSel();
	if (nSelectedIndex < 0)
	{
		nSelectedIndex = 0;
		m_cboTemplateTurn.SetCurSel(0);
	}

	int nID = m_cboTemplateTurn.GetItemData(nSelectedIndex);

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}
	std::map<CString, CString> mapTemplateValueNode;
	KProjectDBase::NetwrokTemplate(spDBaseConnection, nID, TurnTemplateType, mapTemplateValueNode);

	if (mapTemplateValueNode.size() < 1)
	{
		return;
	}

	std::map<CString, CString>::iterator iter = mapTemplateValueNode.begin();
	std::map<CString, CString>::iterator end  = mapTemplateValueNode.end();

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( TurnCategoryName );
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

void KTocEditorTurn::LoadTurnPropertiesValue(std::map<CString, CString>& ar_mapTemplateValueNode)
{
	std::map<CString, CString>::iterator iter = ar_mapTemplateValueNode.begin();
	std::map<CString, CString>::iterator end  = ar_mapTemplateValueNode.end();

	CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( TurnCategoryName );
	CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
	int nChildCount = pPropertyGridItems->GetCount();

	for (int i = 0; i < nChildCount; i++)
	{
		CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt(i);		
		KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

		iter = ar_mapTemplateValueNode.find(pColumn->Name());
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

void KTocEditorTurn::OnCbnSelchangeComboTemplateTurn()
{
	StopEditing();
	LoadTurnPropertiesValue();
}

void KTocEditorTurn::ActionFeedback( KDialogFeedback::KEMActionFeedback emActionCode )
{
    try
    {
        if (emActionCode == KDialogFeedback::KEMActionFeedbackDeActivate)
        {
            StopEditing();
        }
        else if(emActionCode == KDialogFeedback::KEMActionFeedbackActivate)
        {
            LoadTemplateComboBoxTurn();
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

void KTocEditorTurn::StopEditing()
{
    m_nxTurnID = -1;

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

void KTocEditorTurn::UpdateTurn( std::map<CString, CString>& mapKeyValue )
{
    if (m_nxTurnID < 0 || m_pTarget == nullptr)
        return;

    auto iter = mapKeyValue.find(COLUMN_TURN_TYPE);
    if (iter == mapKeyValue.end()) {
        return;
    }
        
    CString strSQL(_T("")); {
        strSQL.AppendFormat(_T(" Update %s Set "), TABLE_TURN); 
        strSQL.AppendFormat(_T(" %s = '%s' "), COLUMN_TURN_TYPE, iter->second);
        strSQL.AppendFormat(_T(" Where %s = '%I64d' "), COLUMN_TURN_ID, m_nxTurnID);
    }

    KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
    spDBase->ExecuteUpdate(strSQL);

    if (m_pMapView)
        m_pMapView->AddTurnMapDraw(m_nxTurnID, true);

    KIOTables* pIOTables = m_pTarget->Tables();
    if (pIOTables == nullptr)
        return;

    KIOTable* pIOTable = pIOTables->FindTable(TABLE_TURN);
    if (pIOTable == nullptr)
        return;

    pIOTable->Notify();
}

void KTocEditorTurn::NotifyTurnEdit( std::vector<__int64>& vecID )
{
    if (vecID.size() != 4) {
        m_nxTurnID = -1;
        InitialControl();
        return;
    }

    m_nxTurnID = vecID[0];
    CXTPPropertyGridItem*  pPropertyGridItemParent = m_wndPropertyGrid.GetCategories()->FindItem( TurnCategoryName );
    CXTPPropertyGridItems* pPropertyGridItems      = pPropertyGridItemParent->GetChilds();
    int nChildCount = pPropertyGridItems->GetCount();

    for (int i = 0; i < nChildCount; i++)
    {
        CXTPPropertyGridItem* pPropertyGridItem = pPropertyGridItems->GetAt( i );
        KIOColumn* pColumn = (KIOColumn*)pPropertyGridItem->GetItemData();

        if (_tcsicmp(pColumn->Name(), COLUMN_TURN_ID) == 0) {
            CString strValue(_T("")); {
                strValue.Format(_T("%I64d"), m_nxTurnID);
            }
            pPropertyGridItem->SetValue(strValue);
            CXTPPropertyGridItemMetrics* pItem = pPropertyGridItem->GetValueMetrics();
            pItem->m_clrBack = CXTPPaintManagerColor(RGB(0, 0, 255));
        }

        if (_tcsicmp(pColumn->Name(), COLUMN_TURN_F_LINK) == 0) {
            CString strValue(_T("")); {
                strValue.Format(_T("%I64d"), vecID[1]);
            }
            pPropertyGridItem->SetValue(strValue);
        }

        if (_tcsicmp(pColumn->Name(), COLUMN_TURN_NODE) == 0) {
            CString strValue(_T("")); {
                strValue.Format(_T("%I64d"), vecID[2]);
            }
            pPropertyGridItem->SetValue(strValue);
        }

        if (_tcsicmp(pColumn->Name(), COLUMN_TURN_T_LINK) == 0) {
            CString strValue(_T("")); {
                strValue.Format(_T("%I64d"), vecID[3]);
            }
            pPropertyGridItem->SetValue(strValue);
        }
    }

    InitialControl();
}

