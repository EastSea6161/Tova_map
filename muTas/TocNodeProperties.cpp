// KTocNodeProperties.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "TocNodeProperties.h"

#include "Target.h"
#include "MapView.h"

#include "KPointMarkerSelectDlg.h"

#include "LayerXmlRenderer.h"
#include "TocConfigLabelDlg.h"
#include "DBaseConnector.h"
#include "BulkDBaseNode.h"

#include "ImTasDBase.h"
#include "KLayerID.h"

IMPLEMENT_DYNAMIC(KTocNodeProperties, KDialogEx)

KTocNodeProperties::KTocNodeProperties(CWnd* pParent /*=NULL*/)
	: KDialogEx(KTocNodeProperties::IDD, pParent), m_pTable(nullptr)
{
    m_pMapView = nullptr;
    m_fntMarker.CreatePointFont(120, _T("QBicF"));

	m_nMarker     = 48;
	m_bUseEditBox = false;
}

KTocNodeProperties::~KTocNodeProperties()
{
    try
    {
        if (m_pTable != nullptr)
            m_pTable->Unregister(this);

        m_fntMarker.DeleteObject();
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

void KTocNodeProperties::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_MARKER,       m_btnMarker);
	DDX_Control(pDX, IDC_BUTTON_MARKER_COLOR, m_btnSymbolColorPicker);
	DDX_Control(pDX, IDC_COMBO_TEMPLATE,      m_cboTemplate);
	DDX_Control(pDX, IDC_REPORT,              m_ctrlReport);
}


BEGIN_MESSAGE_MAP(KTocNodeProperties, KDialogEx)
	ON_WM_CTLCOLOR()	
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_MARKER, &KTocNodeProperties::OnBnClickedButtonMarker)	
	ON_BN_CLICKED(IDC_RADIO1, &KTocNodeProperties::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &KTocNodeProperties::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON_MIN_SCALE, &KTocNodeProperties::OnBnClickedButtonMinScale)
	ON_BN_CLICKED(IDC_BUTTON_MAX_SCALE, &KTocNodeProperties::OnBnClickedButtonMaxScale)
	ON_BN_CLICKED(IDC_BUTTON_RESET,  &KTocNodeProperties::OnBnClickedReset)
	ON_BN_CLICKED(IDC_BUTTON_APPLY,  &KTocNodeProperties::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KTocNodeProperties::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDC_BUTTON_LABEL, &KTocNodeProperties::OnBnClickedButtonLabel)
	ON_BN_CLICKED(IDC_BUTTON_LABEL_SCALE, &KTocNodeProperties::OnBnClickedButtonLabelScale)
	ON_BN_CLICKED(IDC_CHECK_LABEL_SCALE, &KTocNodeProperties::OnBnClickedCheckLabelScale)

	ON_CBN_SELCHANGE(IDC_COMBO_TEMPLATE, &KTocNodeProperties::OnBnSelChangeTemplate)
	ON_NOTIFY(NM_CLICK, IDC_REPORT,   &KTocNodeProperties::OnReportItemClick)
    
    ON_CPN_XTP_SELENDOK(IDC_BUTTON_MARKER_COLOR, OnBnClickedButtonMarkerColor)
    ON_EN_CHANGE(IDC_EDIT_NODE_SIZE, &KTocNodeProperties::OnEnChangeEditNodeSize)
    ON_BN_CLICKED(IDC_CHECK_TEMPLATE, &KTocNodeProperties::OnBnClickedCheckTemplate)

    ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO3, IDC_RADIO4, &KTocNodeProperties::OnBnClickedRadioType)

	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT,   &KTocNodeProperties::OnReportColumnRButtonClick)
	ON_COMMAND(ID_SELECT_ALL,    &KTocNodeProperties::OnSelectAll)
	ON_COMMAND(ID_SELECT_CANCEL, &KTocNodeProperties::OnDeSelectAll)
	ON_EN_SETFOCUS(IDC_EDIT_NODE_SIZE, &KTocNodeProperties::OnEnSetfocusEditNodeSize)
	ON_EN_KILLFOCUS(IDC_EDIT_NODE_SIZE, &KTocNodeProperties::OnEnKillfocusEditNodeSize)
END_MESSAGE_MAP()


// KTocNodeProperties 메시지 처리기입니다.
BOOL KTocNodeProperties::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(false, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
    try
    {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddImChampTooltip(IDC_BUTTON_MARKER, _T("노드 심볼 모양 설정"));
			AddImChampTooltip(IDC_BUTTON_MARKER_COLOR, _T("노드 심볼 색상 설정"));
			AddImChampTooltip(IDC_EDIT_NODE_SIZE, _T("노드 심볼 크기 설정"));
			AddImChampTooltip(IDC_CHECK_TEMPLATE, _T("노드 타입별 심볼 설정\n-타입별 심볼 색상 및 크기 변경"));
			AddImChampTooltip(IDC_REPORT, _T("Type Draw 선택 후 \n타입별 심볼 색상 및 크기 변경"));
			AddImChampTooltip(IDC_RADIO1, _T("항상 지도에 표현"));
			AddImChampTooltip(IDC_RADIO2, _T("특정 스케일로 확대 되었을 경우만 지도에 표현"));
			AddImChampTooltip(IDC_EDIT2, _T("노드 Map Display On/Off 스케일 값:\n -특정 스케일로 확대 되었을 경우만 \n지도에 표현"));
			AddImChampTooltip(IDC_BUTTON_MIN_SCALE, _T("현재 지도의 스케일 값 가져오기"));
			AddImChampTooltip(IDC_CHECK_LABEL, _T("라벨 On/Off\n -라벨 컬럼 정의"));
			AddImChampTooltip(IDC_BUTTON_LABEL, _T("라벨 컬럼 정의"));
			AddImChampTooltip(IDC_CHECK_LABEL_SCALE, _T("선택-라벨 On/Off 스케일 정의\n해제-항상 라벨을 표현"));
			AddImChampTooltip(IDC_BUTTON_LABEL_SCALE, _T("현재 지도의 스케일 값 가져오기"));
			AddImChampTooltip(IDC_BUTTON_RESET, _T("마지막 설정한 정보를 가져오기"));
			AddImChampTooltip(IDC_BUTTON_APPLY, _T("지도 그리기"));
		}
		else
		{
			AddImChampTooltip(IDC_BUTTON_MARKER, _T("Node symbol setting"));
			AddImChampTooltip(IDC_BUTTON_MARKER_COLOR, _T("Node color setting"));
			AddImChampTooltip(IDC_EDIT_NODE_SIZE, _T("Node size setting"));
			AddImChampTooltip(IDC_CHECK_TEMPLATE, _T("노드 타입별 심볼 설정\n-타입별 심볼 색상 및 크기 변경"));
			AddImChampTooltip(IDC_REPORT, _T("Edit Node color and size by Type"));
			AddImChampTooltip(IDC_RADIO1, _T("항상 지도에 표현"));
			AddImChampTooltip(IDC_RADIO2, _T("특정 스케일로 확대 되었을 경우만 지도에 표현"));
			AddImChampTooltip(IDC_EDIT2, _T("노드 Map Display On/Off 스케일 값:\n -특정 스케일로 확대 되었을 경우만 \n지도에 표현"));
			AddImChampTooltip(IDC_BUTTON_MIN_SCALE, _T("현재 지도의 스케일 값 가져오기"));
			AddImChampTooltip(IDC_CHECK_LABEL, _T("Label On/Off\n -Select column for Label"));
			AddImChampTooltip(IDC_BUTTON_LABEL, _T("Select column for Label"));
			AddImChampTooltip(IDC_CHECK_LABEL_SCALE, _T("선택-라벨 On/Off 스케일 정의\n해제-항상 라벨을 표현"));
			AddImChampTooltip(IDC_BUTTON_LABEL_SCALE, _T("현재 지도의 스케일 값 가져오기"));
			AddImChampTooltip(IDC_BUTTON_RESET, _T("마지막 설정한 정보를 가져오기"));
			//AddImChampTooltip(IDC_BUTTON_APPLY, _T("지도 그리기"));
		}

        SetBackgroundColor(RGB(255,255,255));
        m_btnMarker.SetFont(&m_fntMarker);

        LoadInitTemplate();

        KReportCtrlSetting::Default(m_ctrlReport, TRUE, FALSE, FALSE);
        KReportCtrlSetting::SelectionEnableFalse(m_ctrlReport);	
        InitRptHeader();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


HBRUSH KTocNodeProperties::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{    
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void KTocNodeProperties::OnSize(UINT nType, int cx, int cy)
{
	KDialogEx::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KTocNodeProperties::SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView)
{
    try
    {
        m_pTarget  = a_pTarget;
        m_pMapView = a_pMapView;

        try
        {
            if (m_pTable != nullptr)
                m_pTable->Unregister(this);
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            TxLogDebugException();
        }

        m_pTable = m_pTarget->Tables()->FindTable(TABLE_NODE);
        m_pTable->Register(this);

        Reset();
        LoadRptCtrlData();
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

void KTocNodeProperties::NotifyProcess( LPCTSTR a_strSubjectName, Integer a_nxObjectID )
{
    try
    {
        Reset();
        LoadRptCtrlData();
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

void KTocNodeProperties::OnBnClickedRadio1()
{
    try
    {
    	ActionShowLayerAllScaleSelect();
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

void KTocNodeProperties::ActionShowLayerAllScaleSelect()
{
    try
    {
        CButton* pButton = NULL;

        pButton = (CButton*)GetDlgItem(IDC_RADIO1);
        pButton->SetCheck(TRUE);
        pButton = (CButton*)GetDlgItem(IDC_RADIO2);
        pButton->SetCheck(FALSE);

        pButton = (CButton*)GetDlgItem(IDC_EDIT2);
        pButton->EnableWindow(FALSE);

        pButton = (CButton*)GetDlgItem(IDC_EDIT3);
        pButton->EnableWindow(FALSE);
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

void KTocNodeProperties::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	try
	{
		ActionShowLayerZoomScaleSelect();
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

void KTocNodeProperties::ActionShowLayerZoomScaleSelect()
{
    try
    {
        CButton* pButton = NULL;

        pButton = (CButton*)GetDlgItem(IDC_RADIO1);
        pButton->SetCheck(FALSE);
        pButton = (CButton*)GetDlgItem(IDC_RADIO2);
        pButton->SetCheck(TRUE);

        pButton = (CButton*)GetDlgItem(IDC_EDIT2);
        pButton->EnableWindow(TRUE);

        pButton = (CButton*)GetDlgItem(IDC_EDIT3);
        pButton->EnableWindow(TRUE);
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

void KTocNodeProperties::OnBnClickedButtonMinScale()
{
    try
    {
        CString strScale;
        double  dMapScale = m_pMapView->GetRequestMapScale();
        strScale.Format(_T("%.0f"), dMapScale + 0.5);
        SetDlgItemText(IDC_EDIT2, strScale);
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

void KTocNodeProperties::OnBnClickedButtonMaxScale()
{
    try
    {
        CString strScale;
        double  dMapScale = m_pMapView->GetRequestMapScale();
        strScale.Format(_T("%.0f"), dMapScale);
        SetDlgItemText(IDC_EDIT3, strScale);
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

void KTocNodeProperties::OnBnClickedReset()
{
	Reset();
}


void KTocNodeProperties::Reset()
{
    try
    {
        ITxLayerPtr spLayer = m_pMapView->MapGetLayer(KLayerID::Node());
        if (spLayer == nullptr)
            return;

        m_spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
        if (m_spFLayer == nullptr)
            return;
        
        KNodeLayerDispInfo oInfo; {
            KLayerXmlRenderer oLayerXmlRenderer(m_pTarget);
            oLayerXmlRenderer.LoadNodeProperty(oInfo);
        }

        CString strTemp(_T("")); {
            strTemp.AppendChar((TCHAR)oInfo.TMarker);    
        }
        
        SetDlgItemText(IDC_BUTTON_MARKER, strTemp);
        m_nMarker = oInfo.TMarker;

        strTemp.Format(_T("%.1f"), oInfo.TMarkerSize);
        SetDlgItemText(IDC_EDIT_NODE_SIZE, strTemp);

        m_btnSymbolColorPicker.SetColor( oInfo.TMarkerColor );
        m_btnSymbolColorPicker.SetWindowText( _T("") );
        m_btnSymbolColorPicker.ShowText( TRUE );
        m_btnSymbolColorPicker.ModifyCPStyle( 0, CPS_XT_NOFILL | CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE );

        double dMinScale = oInfo.TMinScale;
        double dMaxScale = oInfo.TMaxScale;

        strTemp.Format(_T("%.0f"), dMaxScale);
        SetDlgItemText(IDC_EDIT2, strTemp);

        strTemp.Format(_T("%.0f"), dMaxScale);
        SetDlgItemText(IDC_EDIT3, strTemp);

        if (dMinScale == 0.0 && dMaxScale == 0.0) {
            ActionShowLayerAllScaleSelect();
        } else {
            ActionShowLayerZoomScaleSelect();
        }

        if (oInfo.TUseDrawType != 0) {
            CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO4);
        } 
        else {
            CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO3);
        }

        CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_LABEL);
        if (oInfo.TLabelShow == 1) {
            pButton->SetCheck(BST_CHECKED);
        } else {
            pButton->SetCheck(BST_UNCHECKED);
        }

        m_strLabelFields = oInfo.TLabelField;

        pButton = (CButton*)GetDlgItem(IDC_CHECK_LABEL_SCALE);
        if (oInfo.TUseLabelScale == 1) {
            pButton->SetCheck(BST_CHECKED);
        } else {
            pButton->SetCheck(BST_UNCHECKED);
        }

        strTemp.Format(_T("%.0f"), oInfo.TLabelScale);
        SetDlgItemText(IDC_EDIT_LABEL_SCALE, strTemp);

        LabelFieldNameDisplay(m_strLabelFields);
        LabelCheckEvent();        
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

void KTocNodeProperties::OnBnClickedButtonApply()
{
    std::vector<TNodeTypeDefine> vecTemplateType;
    int nCurSel = m_cboTemplate.GetCurSel();
    if (nCurSel < 0)
        return;

    int nIDTemplate = m_cboTemplate.GetItemData(nCurSel);
    if (nIDTemplate < 0)
        return;

    try
    {   
        CXTPReportRecords* pRecords = m_ctrlReport.GetRecords();
        int nCntRecord = pRecords->GetCount();

        CXTPReportRecord*     pRecord = nullptr;
        CXTPReportRecordItem* pItem   = nullptr;

        for(int i = 0; i < nCntRecord; ++i)
        {
            pRecord = pRecords->GetAt(i);

            TNodeTypeDefine oNodeTypeDefine = {};

            oNodeTypeDefine.nID = nIDTemplate;

            //0 - node_type
            pItem = pRecord->GetItem(0);
            int nTypeNode = pItem->GetItemData();
            oNodeTypeDefine.nNodeType = nTypeNode;

            if (pItem->IsChecked() == TRUE) {
                oNodeTypeDefine.bDraw = true;
            } else {
                oNodeTypeDefine.bDraw = false;
            }

            // 1 - Symbol
            pItem = pRecord->GetItem(1);

            oNodeTypeDefine.clrNode = pItem->GetTextColor();
            oNodeTypeDefine.nCharacterIndex = pItem->GetItemData();

            // 2 - Size
            CXTPReportRecordItemNumber* pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);

            oNodeTypeDefine.dSize = pNumber->GetValue();

            vecTemplateType.push_back( oNodeTypeDefine );
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        AfxMessageBox(ex->GetErrorMessage());
        return;
    }
    catch (...)
    {
    	TxLogDebugException();
        AfxMessageBox(_T("Exception : Report Control Error(OnBnClickedButtonApply)"));
        return;
    }
	
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();

	try
	{
        BOOL bChecked(FALSE);
        if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO4) {
            bChecked = TRUE;
        }

        if (bChecked == TRUE) {
            KProjectDBase::SetDefaultNodeParent(spDBaseConnection, nIDTemplate);
            KProjectDBase::UpdateTemplateNodeChildInfo(spDBaseConnection, vecTemplateType);  
        }     
	}
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        AfxMessageBox(ex->GetErrorMessage());
        return;
    }
	catch (...)
	{
		TxLogDebugException();
        AfxMessageBox(_T("Exception : KProjectDBase Update Error"));
        return;
	}

	try
	{
		Apply();
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

void KTocNodeProperties::Apply()
{
    int nCurSel = m_cboTemplate.GetCurSel();
    if (nCurSel < 0)
        return;

    int nIDTemplate = m_cboTemplate.GetItemData(nCurSel);
    if (nIDTemplate < 0)
        return;

	CString  strTemp;
	CButton* pButton;
	COLORREF markerColor = m_btnSymbolColorPicker.GetColor();	

	GetDlgItemText(IDC_EDIT_NODE_SIZE, strTemp);
	if (!QBicStringChecker::IsNumeric(strTemp))
	{
		AfxMessageBox(_T("숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_NODE_SIZE)->SetFocus();
		return;
	}

	double   dSize = _ttof(strTemp);
	if (dSize <= 0)
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_NODE_SIZE)->SetFocus();
		return;
	}

	double dMinScaleValue(0.0), dMaxScaleValue(0.0);

	int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO2)
	{
		GetDlgItemText(IDC_EDIT2, strTemp);
		if (!QBicStringChecker::IsNumeric(strTemp))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT2)->SetFocus();
			return;
		}

		dMaxScaleValue = _ttof(strTemp);
		if (dMaxScaleValue < 0)	{
			AfxMessageBox(_T("0보다 크거나 같은 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT2)->SetFocus();
			return;
		}

		if (dMinScaleValue == 0.0 && dMaxScaleValue == 0.0) {
			pButton = (CButton*)GetDlgItem(IDC_RADIO1);
			pButton->SetCheck(TRUE);

			pButton = (CButton*)GetDlgItem(IDC_RADIO2);
			pButton->SetCheck(FALSE);
		}
	}

	int    nLabelShow;
	int    nLabelScaleOn;
	double dLabelOnScale;

	BOOL bCheckedLabel = IsDlgButtonChecked(IDC_CHECK_LABEL);    
	if (bCheckedLabel == TRUE) {
		nLabelShow = 1;
	} else {
		nLabelShow = 0;
	}

	BOOL bCheckedLabelScaleOn = IsDlgButtonChecked(IDC_CHECK_LABEL_SCALE);    
	if (bCheckedLabelScaleOn == TRUE) {
		nLabelScaleOn = 1;
	} else {
		nLabelScaleOn = 0;
	}

	GetDlgItemText(IDC_EDIT_LABEL_SCALE, strTemp);
	if (!QBicStringChecker::IsNumeric(strTemp))
	{
		if (bCheckedLabel && bCheckedLabelScaleOn) {
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_LABEL_SCALE)->SetFocus();
			return;
		} else {
			strTemp = _T("0.0");
		}
	}

	dLabelOnScale = _ttof(strTemp);
	if (dLabelOnScale < 0) {
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_LABEL_SCALE)->SetFocus();
		return;
	}

    int nUseTemplate(0);
    BOOL bChecked(FALSE);
    if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO4) {
        bChecked = TRUE;
    }

    if (bChecked == TRUE) {
        nUseTemplate = 1;
    }

    int nLayerOn(1); {
        ITxLayerPtr spLayer = m_pMapView->MapGetLayer(KLayerID::Node());
        if (spLayer->LayerOn() == false)
            nLayerOn = 0;
    }

    KNodeLayerDispInfo oInfo; {
        oInfo.TLayerOn       = nLayerOn;
        oInfo.TMarker        = m_nMarker;
        oInfo.TMarkerColor   = markerColor;
        oInfo.TMarkerSize    = dSize;
        oInfo.TMinScale      = dMinScaleValue;
        oInfo.TMaxScale      = dMaxScaleValue;
        oInfo.TLabelShow     = nLabelShow;
        oInfo.TLabelField    = m_strLabelFields;
        oInfo.TUseLabelScale = nLabelScaleOn;
        oInfo.TLabelScale    = dLabelOnScale;
        oInfo.TUseDrawType   = nUseTemplate;
        oInfo.TDrawTypeID    = nIDTemplate;
    }

	KLayerXmlRenderer oLayerXmlRenderer(m_pTarget);
	oLayerXmlRenderer.SaveNodeProperty(oInfo);

    m_pMapView->ReloadRender(true, 1);
	Reset();

    try
    {
        m_pMapView->NodeLayerSettingChanged();
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


void KTocNodeProperties::OnBnClickedCheckLabel()
{
	LabelCheckEvent();
}

void KTocNodeProperties::LabelCheckEvent()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON_LABEL);

	BOOL bChecked = IsDlgButtonChecked(IDC_CHECK_LABEL);    
	if (bChecked == TRUE)
	{
		pButton->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_LABEL_SCALE)->EnableWindow(TRUE);
		if (IsDlgButtonChecked(IDC_CHECK_LABEL_SCALE))
		{
			GetDlgItem(IDC_EDIT_LABEL_SCALE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_LABEL_SCALE)->EnableWindow(TRUE);       

            if (m_strLabelFields.GetLength() == 0)
                LoadDefaultLabel();
		}
		else
		{
			GetDlgItem(IDC_EDIT_LABEL_SCALE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_LABEL_SCALE)->EnableWindow(FALSE);
		}		
	}
	else
	{
		pButton->EnableWindow(FALSE);

		GetDlgItem(IDC_CHECK_LABEL_SCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LABEL_SCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LABEL_SCALE)->EnableWindow(FALSE);
	}
}

void KTocNodeProperties::LoadDefaultLabel()
{
    CString strDefaultColumnName(_T(""));

    KIOColumns* pIOColumns = m_pTarget->Tables()->FindTable(TABLE_NODE)->Columns();	
    int nColumnCount = pIOColumns->ColumnCount();
    for(int i= 0; i< nColumnCount; i++)
    {
        KIOColumn* pColumn        = pIOColumns->GetColumn(i);
        CString    strDisplayName = pColumn->Caption();
        CString    strColumnName  = pColumn->Name();

        strDefaultColumnName = pColumn->Name();
        if (_tcsicmp(strDefaultColumnName, COLUMN_NODE_ID) == 0)
        {
            break;    
        }
    }

    m_strLabelFields = strDefaultColumnName;
    LabelFieldNameDisplay(strDefaultColumnName);
}

void KTocNodeProperties::OnBnClickedButtonLabel()
{
	KTocConfigLabelDlg dlg(this);
	dlg.SetTarget(m_pTarget, NODE, m_strLabelFields);
	if (dlg.DoModal() == IDOK)
	{
		m_strLabelFields = dlg.GetSelectColumns();
		LabelFieldNameDisplay(m_strLabelFields);
	}
}

void KTocNodeProperties::LabelFieldNameDisplay( CString a_strColumnNames )
{
	CString strTok;
	std::set<CString> setSelectedColumnName;

	int nTokenPos = 0; 
	while (AfxExtractSubString(strTok, a_strColumnNames, nTokenPos++, ',')) {
		setSelectedColumnName.insert(strTok.MakeLower());
	}

	if (m_pTarget == NULL) {
		TxLogDebugException();
		return;
	}

	std::set<CString>::iterator iter;
	std::set<CString>::iterator end = setSelectedColumnName.end();

	std::vector<CString> vecSelectedColumnName;
	const KIOColumns* pIOColumns = m_pTarget->Tables()->FindTable(TABLE_NODE)->Columns();	
	int nColumnCount = pIOColumns->ColumnCount();
	for(int i= 0; i< nColumnCount; i++)
	{
		KIOColumn* pColumn        = pIOColumns->GetColumn(i);
		CString    strDisplayName = pColumn->Caption();
		CString    strColumnName  = pColumn->Name();
		iter = setSelectedColumnName.find(strColumnName.MakeLower());
		if (iter != end)
			vecSelectedColumnName.push_back(strDisplayName);
	}

	CString strDisplayColumnNames;
	size_t nxCount = vecSelectedColumnName.size();
	for (size_t i=0; i<nxCount; i++)
	{
		if (i == nxCount-1)
		{
			strDisplayColumnNames = strDisplayColumnNames + vecSelectedColumnName[i];
		}
		else
		{
			strDisplayColumnNames = strDisplayColumnNames + vecSelectedColumnName[i] + _T("-");
		}		
	}

	SetDlgItemText(IDC_EDIT_LABEL, strDisplayColumnNames);
}

void KTocNodeProperties::OnBnClickedCheckLabelScale()
{
	LabelCheckEvent();
}

void KTocNodeProperties::OnBnClickedButtonLabelScale()
{
	CString strScale;
	double  dMapScale = m_pMapView->GetRequestMapScale();
	strScale.Format(_T("%.0f"), dMapScale + 0.5);
	SetDlgItemText(IDC_EDIT_LABEL_SCALE, strScale);
}


void KTocNodeProperties::LoadInitTemplate( void )
{
	m_cboTemplate.ResetContent();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();

	try
	{
		if (spDBaseConnection == NULL)
			ThrowException(_T("db error connect failed"));

		std::vector<TNodeTemplate> vecNodeTemplate;
		KProjectDBase::NodeTemplateInfo(spDBaseConnection, vecNodeTemplate);

		int nCntTemplate = vecNodeTemplate.size();

		if (nCntTemplate == 0)
			ThrowException(_T("템플릿 데이터가 존재하지 않습니다."));

		int nCurSel(0);

		for (int i = 0; i < nCntTemplate; ++i)
		{
			TNodeTemplate oNodeTemplate = vecNodeTemplate[i];

			int nIndex = m_cboTemplate.InsertString(-1, oNodeTemplate.TName);
			             m_cboTemplate.SetItemData(nIndex, (DWORD_PTR)oNodeTemplate.TID);
            if (oNodeTemplate.TFlag == 1)
            {
                nCurSel = i;
            }
		}

		m_cboTemplate.SetCurSel(nCurSel);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KTocNodeProperties::OnBnSelChangeTemplate( void )
{
	LoadRptCtrlData();
}

void KTocNodeProperties::LoadRptCtrlData( void )
{
	UpdateRptData();
}

void KTocNodeProperties::InitRptHeader( void )
{
	CXTPReportColumn* pColumn = nullptr;

	int nIdxColumn(0);

    UINT dpi = GetDpiForWindow(m_hWnd);
    int nColumnWidth = MulDiv(40, dpi, USER_DEFAULT_SCREEN_DPI);

	pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(nIdxColumn++, _T("Type"), nColumnWidth, TRUE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	//pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("타입"));
	}

    nColumnWidth = MulDiv(30, dpi, USER_DEFAULT_SCREEN_DPI);

	pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(nIdxColumn++, _T("Symbol"), nColumnWidth, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("기호"));
	}

	pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(nIdxColumn++, _T("Size"), nColumnWidth, FALSE));
	//pColumn->GetEditOptions()->AddSpinButton(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("크기"));
	}

	m_ctrlReport.Populate();
}

void KTocNodeProperties::UpdateRptData( void )
{
    BOOL bChecked(FALSE);
    if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO4) {
        bChecked = TRUE;
    }
    m_cboTemplate.EnableWindow(bChecked);

	int nIDTemplate = m_cboTemplate.GetItemData(m_cboTemplate.GetCurSel());

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();

	std::map<int, TNodeTypeDefine> mapTemplateType;
	TNodeTypeDefine oNodeTypeDefault = {};

	try
	{
		if (spDBaseConnection == NULL)
			ThrowException(_T("db error connect failed"));

		KProjectDBase::NodeTemplateChildInfo(spDBaseConnection, nIDTemplate, mapTemplateType);
		KProjectDBase::GetDefaultNodeTypeDefine(spDBaseConnection, oNodeTypeDefault);
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

    try
    {
        CString strTemp(_T("1.0"));
        GetDlgItemText(IDC_EDIT_NODE_SIZE, strTemp);
        if (!QBicStringChecker::IsNumeric(strTemp))
        {
            strTemp = _T("1.0");
        }
        double   dSize = _ttof(strTemp);

        KCodeManager* pCodeMgr       = m_pTarget->CodeManager();
        KCodeGroup*   pCodeUserGroup = pCodeMgr->FindCodeGroup(USER_NODECODE_GROUP_KEY);

		std::map<int, CString> mapCode;
		pCodeUserGroup->GetCodes(mapCode);
		
        m_ctrlReport.ResetContent(FALSE);

        CXTPReportRecord*     pRecord = nullptr;
        CXTPReportRecordItem* pItem   = nullptr;
		
        CString strValue;

		std::map<int, CString>::iterator iter, endIter = mapCode.end();

		for (iter = mapCode.begin(); iter != endIter; ++iter)
		{
			pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord());

			int      nTypeNode = iter->first;
			int      nCharacterIndex;
			double   dWidth(0.0);
			COLORREF linkColor;
            bool     bDraw(true);

			AutoType findIter = mapTemplateType.find(nTypeNode);
			AutoType endIter  = mapTemplateType.end();

			if (findIter != endIter)
			{
				TNodeTypeDefine oNodeTypeDefine = findIter->second;

				dWidth          = oNodeTypeDefine.dSize;
				linkColor       = oNodeTypeDefine.clrNode;
				nCharacterIndex = oNodeTypeDefine.nCharacterIndex;

                if (oNodeTypeDefine.bDraw == false)
                {
                    bDraw = false;
                }
			}
			else
			{
				dWidth          = oNodeTypeDefault.dSize;
				linkColor       = oNodeTypeDefault.clrNode;
				nCharacterIndex = oNodeTypeDefault.nCharacterIndex;
			}

			//0-Type
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pCodeUserGroup->SingleCodeValue(nTypeNode, 1)));
			pItem->SetItemData(nTypeNode);
            pItem->HasCheckbox(TRUE);
            if (bDraw)
            {
                pItem->SetChecked(TRUE);
            }
            else
            {
                pItem->SetChecked(FALSE);
            }

            if (bChecked == TRUE)
            {
                pItem->SetEditable(TRUE);
            }
            else
            {
                pItem->SetEditable(FALSE);
            }

			//1-Symbol
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			CString strSymbol;			
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable(FALSE);
			pItem->SetFont(&m_fntMarker);			
			
            if (bChecked == TRUE)
            {
                strSymbol.AppendChar( nCharacterIndex );
                pItem->SetCaption( strSymbol );
                pItem->SetTextColor( linkColor );
                pItem->SetItemData( nCharacterIndex );
            }
            else
            {
                strSymbol.AppendChar( m_nMarker );
                pItem->SetCaption( strSymbol );
                pItem->SetTextColor(m_btnSymbolColorPicker.GetColor());
                pItem->SetItemData( m_nMarker );
            }

			//2-Size
            if (bChecked == TRUE)
            {
                pItem = pRecord->AddItem(new CXTPReportRecordItemNumber( dWidth, _T("%.1f")));
                pItem->SetEditable(TRUE);
            }
            else
            {
                pItem = pRecord->AddItem(new CXTPReportRecordItemNumber( dSize, _T("%.1f")));
                pItem->SetEditable(FALSE);
            }
			
			pItem->SetAlignment(DT_CENTER);
		}

		m_ctrlReport.Populate();
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

void KTocNodeProperties::UpdateRptDataPart(int a_nRedrawMode)
{
	try
	{
		CString strSize(_T("1.0"));
		GetDlgItemText(IDC_EDIT_NODE_SIZE, strSize);
		if (!QBicStringChecker::IsNumeric(strSize))
		{
			strSize = _T("1.0");
		}
		double   dSize = _ttof(strSize);

		CXTPReportRecords*          pRecords  = m_ctrlReport.GetRecords();
		CXTPReportRecord*           pRecord   = nullptr;
		CXTPReportRecordItemText*   pItemText = nullptr;
		CXTPReportRecordItemNumber* pItemNum  = nullptr;

		int nRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord    = pRecords->GetAt(i);

			if (1 == a_nRedrawMode)
			{
				pItemText  = (CXTPReportRecordItemText*)pRecord->GetItem(1);
				CString strSymbol(_T(""));
				strSymbol.AppendChar( m_nMarker );
				pItemText->SetCaption( strSymbol );
				pItemText->SetItemData( m_nMarker );
			}
			else if (2 == a_nRedrawMode)
			{
				pItemText  = (CXTPReportRecordItemText*)pRecord->GetItem(1);
				pItemText->SetTextColor(m_btnSymbolColorPicker.GetColor());
			}
			else if (3 == a_nRedrawMode)
			{
				pItemNum   = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
				pItemNum->SetValue(dSize);
			}
		}

		m_ctrlReport.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KTocNodeProperties::OnReportItemClick( NMHDR* pNotifyStruct, LRESULT* pResult )
{
    BOOL bChecked(FALSE);
    if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO4) {
        bChecked = TRUE;
    }

    if (bChecked == FALSE)
    {
        return;
    }

	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if ( 1 == nIndex )
	{
		KPointMarkerSelectDlg dlg;
		int      nSymbol = pItemNotify->pItem->GetItemData();
		COLORREF oColor  = pItemNotify->pItem->GetTextColor();
		dlg.SetInitialData(nSymbol, oColor);

		if (IDOK == dlg.DoModal())
		{
			int nSelectMarkerIndex = dlg.GetSelectedMarkerIndex();
			if (nSelectMarkerIndex > 0)
			{
				CString strSymbol;
				strSymbol.AppendChar(nSelectMarkerIndex);        
				pItemNotify->pItem->SetCaption(strSymbol);
				pItemNotify->pItem->SetItemData(nSelectMarkerIndex);

				pItemNotify->pItem->SetTextColor(dlg.GetSelectedMarkerColor());
			}
		}
		m_ctrlReport.RedrawControl();
	}
}


void KTocNodeProperties::OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    BOOL bChecked(FALSE);
    if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO4) {
        bChecked = TRUE;
    }

    if (bChecked == FALSE)
        return;

	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_SELECT_CHECK);
	CMenu* pPopup = menu.GetSubMenu(0);

	CXTPReportColumn* pClickedJeader = pItem->pColumn;
	int nIndex = pClickedJeader->GetItemIndex();

	if( 0 == nIndex)
	{
		CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
		CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
		pCommandBars->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this);
	}

	*pResult = 0;
}


void KTocNodeProperties::OnSelectAll()
{
	ReportSelectControl(TRUE);
}


void KTocNodeProperties::OnDeSelectAll()
{
	ReportSelectControl(FALSE);
}


void KTocNodeProperties::ReportSelectControl(BOOL a_bSelect)
{
	try
	{
		CXTPReportRecords*    pRecords = m_ctrlReport.GetRecords();
		CXTPReportRecord*     pRecord(nullptr);
		CXTPReportRecordItem* pItem  (nullptr);

		int nRecordCnt = pRecords->GetCount();

		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(0);
			pItem->SetChecked(a_bSelect);
		}

		m_ctrlReport.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTocNodeProperties::OnBnClickedButtonMarker()
{
    //TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KPointMarkerSelectDlg dlg;
	dlg.SetInitialData(m_nMarker, m_btnSymbolColorPicker.GetColor());
    if (IDOK == dlg.DoModal())
    {
        int nSelectMarkerIndex = dlg.GetSelectedMarkerIndex();
        if (nSelectMarkerIndex > 0)
        {
            CString strSymbol;
            strSymbol.AppendChar(nSelectMarkerIndex);     
            m_nMarker = nSelectMarkerIndex;
            SetDlgItemText(IDC_BUTTON_MARKER, strSymbol);
        }

        BOOL bChecked(FALSE);
        if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO4) {
            bChecked = TRUE;
        }

		if (bChecked != TRUE)
			UpdateRptData();
		else
			UpdateRptDataPart(1);
    }
}

void KTocNodeProperties::OnBnClickedCheckTemplate()
{
}


void KTocNodeProperties::OnBnClickedRadioType( UINT nID )
{
    int nCheck = GetCheckedRadioButton( IDC_RADIO3, IDC_RADIO4);
    if (nCheck == IDC_RADIO3) {
        m_cboTemplate.EnableWindow(FALSE);
    }
    else {
        m_cboTemplate.EnableWindow(TRUE);
    }

    UpdateRptData();
}

void KTocNodeProperties::OnBnClickedButtonMarkerColor()
{
    BOOL bChecked(FALSE);
    if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO4) {
        bChecked = TRUE;
    }

    if (bChecked != TRUE)
        UpdateRptData();
	else
		UpdateRptDataPart(2);
}


void KTocNodeProperties::OnEnChangeEditNodeSize()
{
    BOOL bChecked(FALSE);
    if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO4) {
        bChecked = TRUE;
    }

    if (bChecked != TRUE)
	{
        UpdateRptData();
	}
	else
	{
		if (m_bUseEditBox)
		{
			UpdateRptDataPart(3);
		}
	}
}

CString KTocNodeProperties::GetCheckedTypeToWhere()
{
    CString strOut(_T(""));

    try
    {
    	CXTPReportRecords* pRecords = m_ctrlReport.GetRecords();
        int nCount = pRecords->GetCount();

        for (int i=0; i<nCount; i++)
        {
            CXTPReportRecord*     pRecord = pRecords->GetAt(i);
            CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

            if (pItem->IsChecked() == TRUE)
            {
                int nType = (int)pItem->GetItemData();
                if (strOut.GetLength() > 0)
                {
                    strOut.AppendFormat(_T(", '%d'"), nType);
                }
                else
                {
                    strOut.AppendFormat(_T("'%d'"), nType);
                }
            }
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

    return strOut;
}


void KTocNodeProperties::OnEnSetfocusEditNodeSize()
{
	m_bUseEditBox = true;
}


void KTocNodeProperties::OnEnKillfocusEditNodeSize()
{
	m_bUseEditBox = false;
}
