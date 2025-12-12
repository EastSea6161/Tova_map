// LinkAnalysisMapDisplayDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "LinkAnalysisMapDisplayDlg.h"
#include "afxdialogex.h"
#include "..\QBicCommon\ColorRangeGen.h"

enum
{
	_0_COLUMN_COLOR = 0,
	_1_COLUMN_WIDTH,
	_2_COLUMN_MINIMUM,
	_3_COLUMN_MAXIMUM
};

#define MunsellColorIndex 0

#define WIDTH_COMBO_INITVALUE     1.0	/* 'width combo'  init value */
#define WIDTH_COMBO_INCREMENT     1.0	/* 'width combo'  increment */
#define WIDTH_COMBO_DATACOUNT     10	/* 'width combo'  data count */	
#define WIDTH_COMBO_INITINDEX     0	    /* 'width combo'  init index */	

#define INCREMENT_COMBO_INITVALUE 0.0	/* 'increment combo'  init value */
#define INCREMENT_COMBO_INCREMENT 0.5	/* 'increment combo'  increment */
#define INCREMENT_COMBO_DATACOUNT 11	/* 'increment combo'  data count */	
#define INCREMENT_COMBO_INITINDEX 1	    /* 'increment combo'  init index */	

// KLinkAnalysisMapDisplayDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KLinkAnalysisMapDisplayDlg, KDialogEx)

KLinkAnalysisMapDisplayDlg::KLinkAnalysisMapDisplayDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KLinkAnalysisMapDisplayDlg::IDD, pParent)
	, m_bChkOptimizeDevide(FALSE)
	, m_bChkDisplayLabel(FALSE)
	, m_dMaxBushVol(0.0)
	, m_dMinBushVol(0.0)
{
	m_clrBack = RGB ( 255, 0, 0 );
}

KLinkAnalysisMapDisplayDlg::~KLinkAnalysisMapDisplayDlg()
{
}

void KLinkAnalysisMapDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT2, m_wndReportCtrl);
	DDX_Control(pDX, IDC_CBO_CLASSESCOUNT, m_cboClasses);
	DDX_Control(pDX, IDC_CBO_COLORRAMP, m_cboColorRamp);
	DDX_Control(pDX, IDC_CBO_WIDTH2, m_cmbWidth);
	DDX_Control(pDX, IDC_CBO_INCRETMENT2, m_cmbIncrement);
	DDX_Check(pDX, IDC_CHECK_OPTIMIZEDEVIDE, m_bChkOptimizeDevide);
	DDX_Check(pDX, IDC_CHECK_LABEL,          m_bChkDisplayLabel);
	DDX_Control(pDX, IDC_CLR_BACK,           m_cpBackColor);
	DDX_XTPColorPicker(pDX, IDC_CLR_BACK, m_clrBack);
}


BEGIN_MESSAGE_MAP(KLinkAnalysisMapDisplayDlg, KDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT2, OnReportItemClick)
	ON_CBN_SELCHANGE(IDC_CBO_CLASSESCOUNT, &KLinkAnalysisMapDisplayDlg::OnCbnSelchangeCboClassescount)
	ON_CBN_SELCHANGE(IDC_CBO_COLORRAMP, &KLinkAnalysisMapDisplayDlg::OnCbnSelchangeCboColorramp)
	ON_CBN_SELCHANGE(IDC_CBO_WIDTH2, &KLinkAnalysisMapDisplayDlg::OnCbnSelchangeCboWidth)
	ON_CBN_SELCHANGE(IDC_CBO_INCRETMENT2, &KLinkAnalysisMapDisplayDlg::OnCbnSelchangeCboIncretment)
	ON_BN_CLICKED(IDC_CHECK_OPTIMIZEDEVIDE, &KLinkAnalysisMapDisplayDlg::OnBnClickedCheckOptimizedevide)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KLinkAnalysisMapDisplayDlg::OnBnClickedCheckDisplayLabel)
	ON_CPN_XTP_SELENDOK(IDC_CLR_BACK, &KLinkAnalysisMapDisplayDlg::OnSelEndOkBackClr)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// KLinkAnalysisMapDisplayDlg 메시지 처리기입니다.

bool KLinkAnalysisMapDisplayDlg::UserBushInputCheck(bool a_bShowErrMsg)
{
	try
	{
		double dMinValue(0.0), dMaxValue(0.0);
		double dMinSize(0.0),  dMaxSize(0.0);

		CString strValue(_T(""));

		if (m_vecLinkVolume.size() < 1)
		{
			if (a_bShowErrMsg) {
				//AfxMessageBox(_T("먼저 링크/구간을 지정해 주세요."));
				AfxMessageBox(_T("경로 분석 결과가 존재하지 않습니다."));
			}
			return false;
		}

		//min value 체크
		GetDlgItemText(IDC_EDIT_MIN_VALUE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MIN_VALUE)->SetFocus();		
			}

			return false;
		}
		dMinValue = _ttof(strValue);

		//IDC_EDIT_MAX_VALUE
		GetDlgItemText(IDC_EDIT_MAX_VALUE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MAX_VALUE)->SetFocus();		
			}

			return false;
		}
		dMaxValue = _ttof(strValue);

		//IDC_EDIT_MINSIZE
		GetDlgItemText(IDC_EDIT_MINSIZE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MINSIZE)->SetFocus();		
			}

			return false;
		}
		dMinSize = _ttof(strValue);

		//IDC_EDIT_MAXSIZE
		GetDlgItemText(IDC_EDIT_MAXSIZE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MAXSIZE)->SetFocus();		
			}

			return false;
		}
		dMaxSize = _ttof(strValue);

		if (dMinValue > dMaxValue)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MIN_VALUE)->SetFocus();		
			}

			return false;
		}

		if (dMinSize > dMaxSize)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_MINSIZE)->SetFocus();		
			}

			return false;
		}        
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


bool KLinkAnalysisMapDisplayDlg::OldGetLinkInfo(std::vector<TDrawLinkInfo> &a_vecDrawLinkInfo)
{
	TxLogDebugStartMsg();

	a_vecDrawLinkInfo.clear();

	std::vector<TDrawLinkInfo>  vecDrawLinkInfo;
	TDrawLinkInfo               oTDrawLinkInfo;
	TMinMaxInfo                 oTMinMaxInfo;
	CXTPReportRecord*           pRecord     = NULL;
	CXTPReportRecordItem*       pItemColor  = NULL;
	CXTPReportRecordItemNumber* pItemNumber = NULL;
	CXTPReportRecordItem*       pItemMinValue  = NULL;
	CXTPReportRecordItem*       pItemMaxValue  = NULL;

	std::map<double, std::vector<TDrawLinkInfo>> mapDrawLinkInfo;

	for(size_t idx= 0; idx< m_vecLinkVolume.size(); idx++)
	{
		m_vecLinkVolume[idx].nxLinkID;
		Integer nxLinkId	= m_vecLinkVolume[idx].nxLinkID;
		double  dValue      = m_vecLinkVolume[idx].dVolume;

		bool bCheck = false;
		for(size_t i= 0; i< m_vecMinMaxInfo.size(); i++)
		{
			oTMinMaxInfo   = m_vecMinMaxInfo[i];
			double dMaxVal = oTMinMaxInfo.dMaxValue;

			if(dValue <= dMaxVal)
			{
				bCheck = true;
				break;
			}
		}

		if( false == bCheck )
		{
			continue;
		}

		pRecord     = oTMinMaxInfo.pRecord;
		pItemColor  = pRecord->GetItem(_0_COLUMN_COLOR);
		pItemNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(_1_COLUMN_WIDTH));
		pItemMinValue  = pRecord->GetItem(_2_COLUMN_MINIMUM);
		pItemMaxValue  = pRecord->GetItem(_3_COLUMN_MAXIMUM);

		oTDrawLinkInfo.TLinkID   = nxLinkId;
		oTDrawLinkInfo.TColorRGB = pItemColor->GetBackgroundColor();
		double dWidth            = pItemNumber->GetValue();
		oTDrawLinkInfo.TWidth    = dWidth;

		CString strLabel = _T("");
		/*strLabel.Format(_T("%s ~ %s"), ((LPCTSTR)pItemMinValue->GetCaption(NULL)), (pItemMaxValue->GetCaption(NULL)));*/
		strLabel.Format(_T("%.1f"), dValue);
		
		oTDrawLinkInfo.TLabel    = strLabel;
		
		/*a_vecDrawLinkInfo.push_back(oTDrawLinkInfo);*/
		std::map<double, std::vector<TDrawLinkInfo>>::iterator iterTest =  mapDrawLinkInfo.find(dWidth);		
		if( iterTest != mapDrawLinkInfo.end())
		{
			std::vector<TDrawLinkInfo> &vecDrawLinkInfo = iterTest->second;
			vecDrawLinkInfo.push_back(oTDrawLinkInfo);
		}
		else
		{
			std::vector<TDrawLinkInfo> vecDrawLinkInfo;
			vecDrawLinkInfo.push_back(oTDrawLinkInfo);
			mapDrawLinkInfo.insert( std::make_pair(dWidth, vecDrawLinkInfo) );
		}
	}

	std::map<double, std::vector<TDrawLinkInfo>>::reverse_iterator iterBeginMap = mapDrawLinkInfo.rbegin();
	std::map<double, std::vector<TDrawLinkInfo>>::reverse_iterator iterEndMap   = mapDrawLinkInfo.rend();

	while(iterBeginMap != iterEndMap)
	{
		std::vector<TDrawLinkInfo> vecInfoPart = iterBeginMap->second;

		for(size_t i= 0; i< vecInfoPart.size(); i++)
		{
			TDrawLinkInfo oTDrawLinkInfo = vecInfoPart[i];
			a_vecDrawLinkInfo.push_back(oTDrawLinkInfo);
		}
		++iterBeginMap;
	}
	TxLogDebugEndMsg();
	return true;
}


void KLinkAnalysisMapDisplayDlg::CalculateValue2Width(double a_dTrip, TPathVolumeSettingInfo &a_oSettingInfo, double& dWidth )
{
	if (a_oSettingInfo.dMaxTrip == a_oSettingInfo.dMinTrip)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
	else
	{
		dWidth = ( a_oSettingInfo.dMaxSize - a_oSettingInfo.dMinSize ) / (a_oSettingInfo.dMaxTrip - a_oSettingInfo.dMinTrip) * (a_dTrip - a_oSettingInfo.dMinTrip) + a_oSettingInfo.dMinSize;
	}

	if (dWidth > a_oSettingInfo.dMaxSize)
	{
		dWidth = a_oSettingInfo.dMaxSize;
	}

	if (dWidth < a_oSettingInfo.dMinSize)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
}


bool KLinkAnalysisMapDisplayDlg::NewGetLinkInfo(std::vector<TDrawLinkInfo> &a_vecDrawLinkInfo, bool a_bShowErrMsg)
{
	TxLogDebugStartMsg();

	a_vecDrawLinkInfo.clear();

	if (UserBushInputCheck(a_bShowErrMsg) == false)
	{
		return false;
	}

	TPathVolumeSettingInfo oTBushVolumeSettingInfo;
	oTBushVolumeSettingInfo.bUseOutRange = false;
	oTBushVolumeSettingInfo.clrOutRange  = RGB(255, 255, 255);
	oTBushVolumeSettingInfo.clrPath      = m_cpBackColor.GetColor();

	CString strValue(_T(""));
	GetDlgItemText(IDC_EDIT_MIN_VALUE, strValue);
	oTBushVolumeSettingInfo.dMinTrip    = _ttof(strValue);
	GetDlgItemText(IDC_EDIT_MAX_VALUE, strValue);
	oTBushVolumeSettingInfo.dMaxTrip    = _ttof(strValue);
	GetDlgItemText(IDC_EDIT_MINSIZE, strValue);
	oTBushVolumeSettingInfo.dMinSize    = _ttof(strValue);
	GetDlgItemText(IDC_EDIT_MAXSIZE, strValue);
	oTBushVolumeSettingInfo.dMaxSize    = _ttof(strValue);

	TDrawLinkInfo               oTDrawLinkInfo;

	std::map<double, std::vector<TDrawLinkInfo>> mapDrawLinkInfo;

	std::map<Integer, double> mapLinkVolume;

	for(size_t idx= 0; idx< m_vecLinkVolume.size(); idx++)
	{
		Integer nxLinkID	= m_vecLinkVolume[idx].nxLinkID;
		double  dValue      = m_vecLinkVolume[idx].dVolume;

		AutoType iterB  = mapLinkVolume.find(nxLinkID);
		AutoType itEndB = mapLinkVolume.end();
		if (iterB != itEndB)
		{
			double &dVolume = iterB->second;
			dVolume += dValue;
		}
		else
		{
			mapLinkVolume.insert(std::make_pair(nxLinkID, dValue));
		}
	}


	AutoType iterLink  = mapLinkVolume.begin();
	AutoType itLinkEnd = mapLinkVolume.end();

	while (iterLink != itLinkEnd)
	{
		TDrawLinkInfo oLinkInfo;
		Integer nxLinkID		= iterLink->first;
		double  dVolume		    = iterLink->second;

		CString &strVolume      = oLinkInfo.TLabel;          
		strVolume.Format(_T("%.3f"), dVolume);

		oLinkInfo.TLinkID = nxLinkID;

		if (dVolume > oTBushVolumeSettingInfo.dMaxTrip)
		{
			if (false == oTBushVolumeSettingInfo.bUseOutRange)
			{
				++iterLink;
				continue;
			}
			oLinkInfo.TColorRGB = oTBushVolumeSettingInfo.clrOutRange;
			oLinkInfo.TWidth	= oTBushVolumeSettingInfo.dMinSize;
		}
		else if (dVolume < oTBushVolumeSettingInfo.dMinTrip)
		{
			if (false == oTBushVolumeSettingInfo.bUseOutRange)
			{
				++iterLink;
				continue;
			}
			oLinkInfo.TColorRGB = oTBushVolumeSettingInfo.clrOutRange;
			oLinkInfo.TWidth	= oTBushVolumeSettingInfo.dMinSize;
		}
		else
		{
			oLinkInfo.TColorRGB = oTBushVolumeSettingInfo.clrPath;
			double dWidth(0.0);
			CalculateValue2Width(dVolume, oTBushVolumeSettingInfo, dWidth);
			oLinkInfo.TWidth = dWidth;
		}

		a_vecDrawLinkInfo.push_back(oLinkInfo);

		oLinkInfo.TLabel = _T("");

		++iterLink;
	}

	TxLogDebugEndMsg();
	return true;
}


void KLinkAnalysisMapDisplayDlg::InitSettingInfo()
{
	InitBushVolMinMax();
	SetDlgItemText(IDC_EDIT_MIN_VALUE,  _T(""));
	SetDlgItemText(IDC_EDIT_MAX_VALUE,  _T(""));
}


void KLinkAnalysisMapDisplayDlg::InitialColorRampCtrl()
{
	CBitmap bmp;
	/*bmp.LoadBitmap(IDB_BITMAP_COLOR_RAMP_A);*/
	bmp.LoadBitmap(IDB_BITMAP_COLOR_RAMP_B);
	m_colorRampImageList.Create(200, 18, ILC_COLOR24, 6, 6);
	m_colorRampImageList.Add(&bmp, RGB(0, 0, 0));

	m_cboColorRamp.SetImageList(&m_colorRampImageList);

	COMBOBOXEXITEM cbItem;
	::RtlSecureZeroMemory(&cbItem, sizeof(cbItem));
	cbItem.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE ;

	int nIdx = 0;
	for ( int i=0; i< 6; i++ )
	{
		cbItem.iItem = i;
		cbItem.iImage = i;
		cbItem.iSelectedImage = i;

		m_cboColorRamp.InsertItem(&cbItem);
	}

	m_cboColorRamp.SetCurSel(0);
}


void KLinkAnalysisMapDisplayDlg::SetColorRampInfo(int nSelectIndex)
{
	switch (nSelectIndex)
	{
	case 0 :
		break;
	case 1 :
		m_clrStart = RGB ( 190, 207, 219 );
		m_clrEnd   = RGB (  84,  93, 100 );
		break;
	case 2 :
		m_clrStart = RGB ( 119, 221, 221 );
		m_clrEnd   = RGB (   8,  38,  67 );
		break;
	case 3 :
		m_clrStart = RGB ( 234, 158,  69 );
		m_clrEnd   = RGB ( 105,   2,   5 );
		break;
	case 4 :
		m_clrStart = RGB ( 255, 255, 102 );
		m_clrEnd   = RGB ( 102,   2, 153 );
		break;
	case 5 :
		m_clrStart = RGB ( 255, 102, 130 );
		m_clrEnd   = RGB (   0,  79, 153 );
		break;
	default:
		return;
		break;
	}

	CXTPReportRecords*    pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	KColorRangeGen oColorRangeGen;
	oColorRangeGen.setStartColor(m_clrStart);
	oColorRangeGen.setEndColor  (m_clrEnd);
	oColorRangeGen.setSize(nRecordCount);
	oColorRangeGen.createColorRange();

	KMunsellColor* pMunSellColor = new KMunsellColor(0, false);

	int nTotalRowCount = m_wndReportCtrl.GetRows()->GetCount();
	for(int i= 0; i< nTotalRowCount; i++)
	{
		CXTPReportRow* pRow = m_wndReportCtrl.GetRows()->GetAt(i);
		CXTPReportRecord* pRecord = pRow->GetRecord();
		CXTPReportRecordItem* pItemColor = pRecord->GetItem(_0_COLUMN_COLOR);

		COLORREF   colorRef;
		if( MunsellColorIndex == nSelectIndex )
		{
			colorRef = pMunSellColor->GetColor(i);
		}
		else
		{
			colorRef = oColorRangeGen.getNextColor();       
		}

		pItemColor->SetBackgroundColor(colorRef);
	}


	m_wndReportCtrl.RedrawControl();
	delete pMunSellColor;
}


void KLinkAnalysisMapDisplayDlg::SetDefaultWidthInfo()
{
	UpdateData(TRUE);

	int nWidthIdx     = m_cmbWidth.GetCurSel();
	int nIncrementIdx = m_cmbIncrement.GetCurSel();

	if( nWidthIdx < 0 || nIncrementIdx < 0)
	{
		return;
	}

	double dValue         = m_vecWidth[nWidthIdx];
	double dIncrementVal  = m_vecIncrement[nIncrementIdx];

	CXTPReportRecords*          pRecords = NULL;
	CXTPReportRecord*           pRecord  = NULL;
	CXTPReportRecordItemNumber* pItemNumber    = NULL;

	m_wndReportCtrl.GetRows();
	int nTotalRowCount = m_wndReportCtrl.GetRows()->GetCount();
	for(int i= 0; i< nTotalRowCount; i++)
	{
		CXTPReportRow* pRow = m_wndReportCtrl.GetRows()->GetAt(i);
		CXTPReportRecord* pRecord = pRow->GetRecord();

		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_WIDTH);

		pItemNumber->SetValue(dValue);

		dValue += dIncrementVal;
	}

	m_wndReportCtrl.RedrawControl();
}


void KLinkAnalysisMapDisplayDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_COLOR, _T("Color"), 50, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_WIDTH, _T("Width"), 50));
	pColumn->GetEditOptions()->AddSpinButton(TRUE);
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_MINIMUM, _T("Min"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_MAXIMUM, _T("Max"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
}


void KLinkAnalysisMapDisplayDlg::ClearReportData()
{
	m_wndReportCtrl.ResetContent(TRUE);
	m_wndReportCtrl.Populate();
}


void KLinkAnalysisMapDisplayDlg::InitReportData()
{   
	UpdateData(TRUE);

	m_vecMinMaxInfo.clear();

	int nCurSelectedClassIndex = m_cboClasses.GetCurSel();

	if ( nCurSelectedClassIndex < 0)
	{
		return;
	}

	//m_vecLinkVolume
	
	std::vector<double> a_vecData;

	for(size_t i= 0; i< m_vecLinkVolume.size(); i++)
	{
		TLinkVolume &oTLinkVolume = m_vecLinkVolume[i];
		a_vecData.push_back(oTLinkVolume.dVolume);
	}

	if(a_vecData.size() < 1)
		return;

	// 2. devide value
	int nBreakCount   = m_cboClasses.GetItemData(nCurSelectedClassIndex);

	KDevideDataInterval oDevideDataInterval;
	oDevideDataInterval.SetDataCalculate(a_vecData);

	std::vector<TDevideDataInterval> ovecDevideInfo;

	if( TRUE == m_bChkOptimizeDevide)
	{
		ovecDevideInfo = oDevideDataInterval.GetOptimizeDevide(nBreakCount);
	}
	else
	{
		ovecDevideInfo = oDevideDataInterval.GetDevide(nBreakCount);
	}

	// 3. draw grid
	m_wndReportCtrl.ResetContent(TRUE);

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	CString strTemp = _T("");

	for ( size_t i=0; i<ovecDevideInfo.size(); i++ )
	{
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

		// 색상 정의
		pItem = pRecord->AddItem(new CXTPReportRecordItem());           
		
		COLORREF clrBackColor = m_cpBackColor.GetColor();
		pItem->SetBackgroundColor(clrBackColor);  
		pItem->SetEditable(FALSE);

		// Width 정의
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0 + i, _T("%.1f")));         
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(xtpColumnTextLeft);

		// Field Name
		TDevideDataInterval& oDevideInfo = ovecDevideInfo[i];

		strTemp.Format(_T("%.3f"),  oDevideInfo.TMinValue);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(xtpColumnTextRight);

		strTemp.Format(_T("%.3f"),  oDevideInfo.TMaxValue);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));       
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(xtpColumnTextRight);

		TMinMaxInfo oTMinMaxInfo;
		oTMinMaxInfo.dMaxValue = oDevideInfo.TMaxValue;
		oTMinMaxInfo.pRecord   = pRecord;

		m_vecMinMaxInfo.push_back(oTMinMaxInfo);
	}

	m_wndReportCtrl.Populate();
	SetDefaultWidthInfo();
}


void KLinkAnalysisMapDisplayDlg::InitCombo()
{
	for (int i=2; i<20; i++)
	{
		CString str;
		str.Format(_T("%d"), i);
		int index = m_cboClasses.AddString(str);
		m_cboClasses.SetItemData(index, i);
	}

	m_cboClasses.SetCurSel(8);
}


void KLinkAnalysisMapDisplayDlg::InitWidthValCombos()
{
	int     nCurIndex   = 0;
	CString strValue    = _T("");

	double  dValue = WIDTH_COMBO_INITVALUE;
	for(int i= 0; i< WIDTH_COMBO_DATACOUNT; i++)
	{
		strValue.Format(_T("%.1f"), dValue);

		nCurIndex = m_cmbWidth.AddString(strValue);
		m_vecWidth.push_back(dValue);

		dValue +=    WIDTH_COMBO_INCREMENT;
	}

	dValue         = INCREMENT_COMBO_INITVALUE;
	for(int i= 0; i< INCREMENT_COMBO_DATACOUNT; i++)
	{
		strValue.Format(_T("%.1f"), dValue);

		nCurIndex = m_cmbIncrement.AddString(strValue);
		m_vecIncrement.push_back(dValue);

		dValue +=    INCREMENT_COMBO_INCREMENT;
	}

	m_cmbWidth.SetCurSel    (WIDTH_COMBO_INITINDEX);
	m_cmbIncrement.SetCurSel(INCREMENT_COMBO_INITINDEX);
}


BOOL KLinkAnalysisMapDisplayDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitCombo();
	InitWidthValCombos();
	InitialColorRampCtrl();

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrl);
	InitReportHeader();

	m_cpBackColor.SetColor(m_clrBack);
	m_cpBackColor.SetDefaultColor(GetSysColor(COLOR_WINDOW));
	m_cpBackColor.ShowText(FALSE);
	m_cpBackColor.SetTheme(xtpControlThemeDefault);

	SetDlgItemText(IDC_EDIT_MINSIZE,   _T("1.0"));
	SetDlgItemText(IDC_EDIT_MAXSIZE,   _T("15.0"));

	InitReportData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KLinkAnalysisMapDisplayDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if( _0_COLUMN_COLOR == nIndex )
	{
		CXTColorDialog dlg(pItemNotify->pItem->GetBackgroundColor(), pItemNotify->pItem->GetBackgroundColor()) ;
		if ( IDOK == dlg.DoModal() )
		{
			pItemNotify->pItem->SetBackgroundColor(dlg.GetColor());            
			m_wndReportCtrl.RedrawControl();
		}
	}
}


void KLinkAnalysisMapDisplayDlg::OnCbnSelchangeCboClassescount()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	InitReportData();
}


void KLinkAnalysisMapDisplayDlg::OnCbnSelchangeCboColorramp()
{
	SetColorRampInfo(m_cboColorRamp.GetCurSel());

	this->SetFocus();
}


void KLinkAnalysisMapDisplayDlg::OnCbnSelchangeCboWidth()
{
	SetDefaultWidthInfo();

	this->SetFocus();
}


void KLinkAnalysisMapDisplayDlg::OnCbnSelchangeCboIncretment()
{
	SetDefaultWidthInfo();

	this->SetFocus();
}


void KLinkAnalysisMapDisplayDlg::OnBnClickedCheckOptimizedevide()
{
	InitReportData();
}


void KLinkAnalysisMapDisplayDlg::OnBnClickedCheckDisplayLabel()
{

}


void KLinkAnalysisMapDisplayDlg::OnSelEndOkBackClr()
{
	InitReportData();
}


void KLinkAnalysisMapDisplayDlg::SetLinkVolume(std::vector<TLinkVolume> a_vecLinkVolume)
{
	m_vecLinkVolume = a_vecLinkVolume;

	std::map<Integer, double> mapLinkVolume;

	InitBushVolMinMax();


	if (a_vecLinkVolume.size() < 1)
	{
		SetDlgItemText(IDC_EDIT_MIN_VALUE,  _T(""));
		SetDlgItemText(IDC_EDIT_MAX_VALUE,  _T(""));
		return;
	}

	try
	{
		double dMaxValue = -999999999999.0;
		double dMinValue = 999999999999.0;

		for (size_t i= 0; i< m_vecLinkVolume.size(); i++)
		{
			TLinkVolume &oTLinkVolume = m_vecLinkVolume[i];

			Integer &nxLinkID = oTLinkVolume.nxLinkID;
			double  &dVolume  = oTLinkVolume.dVolume;


// 			AutoType iter = mapLinkVolume.find(nxLinkID);
// 			if (iter != mapLinkVolume.end())
// 			{
// 				double &dSumVolume = iter->second;
// 				dSumVolume += dVolume;
// 				//SetMaxBushVol(dSumVolume);
// 
// 				if (dMaxValue < dSumVolume)
// 				{
// 					dMaxValue = dSumVolume;
// 				}
// 			}
// 			else
// 			{
// 				mapLinkVolume.insert(std::make_pair(nxLinkID, dVolume));
// 				//SetMaxBushVol(dVolume);
// 
// 				if (dMaxValue < dVolume)
// 				{
// 					dMaxValue = dVolume;
// 				}
// 			}
			if (dVolume >= 0)
			{
				if (dMinValue > dVolume)
				{
					dMinValue = dVolume;
				}
			}

			if (dMaxValue < dVolume)
			{
				dMaxValue = dVolume;
			}

			//SetMinBushVol(dVolume);
		}

		m_dMaxBushVol = dMaxValue;
		m_dMinBushVol = dMinValue;

		UpdateBushMinMax();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KLinkAnalysisMapDisplayDlg::UpdateBushMinMax()
{
	CString strMinBushVol(_T(""));
	CString strMaxBushVol(_T(""));

	double dMinPathVol(0.0001);

	double dMinBushVol(0.0);
// 	dMinBushVol = m_dMinBushVol - 0.5;
// 	if(dMinBushVol < 0.0)
// 		dMinBushVol = 0.0;
// 
// 	if (dMinBushVol < dMinPathVol)
// 	{
// 		dMinBushVol = dMinPathVol;
// 	}


	dMinBushVol = m_dMinBushVol;
	// 	if(dMinBushVol < 0.0)
	// 		dMinBushVol = 0.0;

	if (dMinBushVol < dMinPathVol)
	{
		strMinBushVol.Format(_T("%g"), dMinBushVol);

		SetDlgItemText(IDC_EDIT_MIN_VALUE,  strMinBushVol);
	}
	else
	{
		int nMinBushVol = (int)dMinBushVol;
		dMinBushVol     = (double)nMinBushVol; // double 버림 처리
		strMinBushVol.Format(_T("%.0f"), dMinBushVol);
	
		SetDlgItemText(IDC_EDIT_MIN_VALUE,  strMinBushVol);
	}

	double dMaxBushVol(0.0);
	dMaxBushVol = m_dMaxBushVol + 0.6;
	strMaxBushVol.Format(_T("%.0f"), dMaxBushVol);

	SetDlgItemText(IDC_EDIT_MAX_VALUE,  strMaxBushVol);
}


void KLinkAnalysisMapDisplayDlg::InitBushVolMinMax()
{
	m_dMaxBushVol = 0.0;
	m_dMinBushVol = 0.0;
}


void KLinkAnalysisMapDisplayDlg::SetMinBushVol(double &a_dBushVol)
{
	if( a_dBushVol > 0 )
	{
		if(0.0 == m_dMinBushVol)
		{
			m_dMinBushVol = a_dBushVol;
		}

		if(m_dMinBushVol > a_dBushVol)
		{
			m_dMinBushVol = a_dBushVol;
		}
	}
}


void KLinkAnalysisMapDisplayDlg::SetMaxBushVol(double &a_dBushVol)
{
	if( a_dBushVol > 0 )
	{
		if(m_dMaxBushVol < a_dBushVol)
		{
			m_dMaxBushVol = a_dBushVol;
		}
	}
}

HBRUSH KLinkAnalysisMapDisplayDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

// 	UINT ID = pWnd->GetDlgCtrlID();
//     if( ID == IDC_FRAME )
//     {        
//         /*hbr = (HBRUSH)m_pbkBrush.GetSafeHandle();
//         pDC->SetBkMode(TRANSPARENT);*/
//         pDC->SetBkMode(TRANSPARENT);
// 
// 		hbr = (HBRUSH)m_backBrush;
//         //return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
//     }
	return hbr;
}
