#include "stdafx.h"
#include "KDlgNodeZoneTypeEdit.h"
#include "afxdialogex.h"

#include "Target.h"

IMPLEMENT_DYNAMIC(KDlgNodeZoneTypeEdit, CDialogEx)
    BEGIN_MESSAGE_MAP(KDlgNodeZoneTypeEdit, CDialogEx)
        ON_BN_CLICKED(IDOK, &KDlgNodeZoneTypeEdit::OnBnClickedOk)
        ON_BN_CLICKED(IDCANCEL, &KDlgNodeZoneTypeEdit::OnBnClickedCancel)
    END_MESSAGE_MAP()

KDlgNodeZoneTypeEdit::KDlgNodeZoneTypeEdit(KTarget* pTarget, CWnd* pParent /*=NULL*/)
	: CDialogEx(KDlgNodeZoneTypeEdit::IDD, pParent)
{
    m_pTarget = pTarget;
}

KDlgNodeZoneTypeEdit::~KDlgNodeZoneTypeEdit()
{
}

void KDlgNodeZoneTypeEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT1, m_wndReport);
}

BOOL KDlgNodeZoneTypeEdit::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE, FALSE);

    if (true) {
        CXTPReportColumn* pColumn = NULL;
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Node Type"), 60, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("노드 타입"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Count"),     35));
        pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("개수"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Select Zone Type"),   65));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetEditable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("존 노드 선택"));
		}

        m_wndReport.Populate();
    }

    LoadReportData();

    return TRUE;
}

void KDlgNodeZoneTypeEdit::LoadReportData()
{
    m_wndReport.ResetContent();

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T(" node_type, count(*) "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T(" Node "));
        strSQL.Append(_T(" Group By node_type "));
    }

    KDBaseConPtr  spDBase  = m_pTarget->GetDBaseConnection();
    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);

    CXTPReportRecord     *pRecord = NULL;
    CXTPReportRecordItem *pItem   = NULL;
    CString strTemp(_T(""));

    while(spResult->Next()) {
        int nType = spResult->GetValueInt(0);
        int nCnt  = spResult->GetValueInt(1);

        pRecord = m_wndReport.AddRecord(new CXTPReportRecord()); 

        pItem = pRecord->AddItem(new CXTPReportRecordItem); {
            strTemp.Format(_T("%d"), nType);
            pItem->SetCaption(strTemp);
            pItem->SetAlignment(DT_CENTER);
        }

        pItem = pRecord->AddItem(new CXTPReportRecordItem); {
            strTemp.Format(_T("%d"), nCnt);
            pItem->SetCaption(strTemp);
            pItem->SetAlignment(DT_CENTER);
        }

        pItem = pRecord->AddItem(new CXTPReportRecordItem); {
            pItem->HasCheckbox(TRUE);
            pItem->SetEditable(TRUE);
            pItem->SetAlignment(xtpColumnIconCenter);
            if (nType == 0) {
                pItem->SetChecked(TRUE);
            }
            else {
                pItem->SetChecked(FALSE);
            }
        }
    }

    m_wndReport.Populate();
}

void KDlgNodeZoneTypeEdit::OnBnClickedOk()
{
    try {
        if (UpdateType() == false)
            return;
    }
    catch (...) {

    }

    CDialogEx::OnOK();
}

bool KDlgNodeZoneTypeEdit::UpdateType()
{
    CXTPReportRecords* pRecords = m_wndReport.GetRecords();
    int nCnt = pRecords->GetCount();

    if (nCnt == 0)
        return true;

    CXTPReportRecord*     pRecord = nullptr;
    CXTPReportRecordItem* pItem   = nullptr;

    std::vector<int> vecZoneType;
    bool bZero2Max(false);
    bool bExistZero(false);

    int nMaxType(0);
    int nZoneCnt(0);
    for (int i=0; i<nCnt; i++) {
        pRecord = pRecords->GetAt(i);
        int nType(0); {
            pItem = pRecord->GetItem(0);
            nType = _ttoi(pItem->GetCaption());

            if (nMaxType <= nType) {
                nMaxType = nType;
            }

            if (nType == 0) {
                bExistZero = true;
            }
        }

        pItem = pRecord->GetItem(2);
        if (pItem->IsChecked() == TRUE) {
            if (nType != 0) {
                vecZoneType.push_back(nType);
            } 

            nZoneCnt++;
        }
        else {
            if (nType == 0) {
                bZero2Max = true;
            }            
        }
    }
    
    //★ 존노드(0) 선택되었고, 나머지는 비선택 -> 변경이 없다.
    if (bExistZero == true && bZero2Max == false && vecZoneType.size() == 0) {
        return true;
    }

    if ( nZoneCnt == 0 ) { //★ 존 노드가 없다.
        CString strMsg(_T("TOVA에서 존 노드/존 연결링크의 타입 코드는 0으로 설정됩니다."));
        strMsg.Append(_T("\n선택된 존 노드 타입 정보가 없습니다."));

        if (bZero2Max == true) {
            strMsg.AppendFormat(_T("\n타입코드 0 -> %d"), nMaxType+1);
        }
        
        strMsg.Append(_T("\n계속 진행하시겠습니까?"));

        if ( AfxMessageBox(strMsg, MB_YESNO) == IDNO)
            return false;
    }
    else {
        CString strMsg(_T("TOVA에서 존 노드/존 연결링크의 타입 코드는 0으로 설정됩니다."));
        if (bZero2Max == true) {
            strMsg.AppendFormat(_T("\n타입코드 0 -> %d"), nMaxType+1);
        }

        if (vecZoneType.size() > 0) {
            CString strTemp(_T(""));
            for (size_t i=0; i<vecZoneType.size(); i++) {
                if (i == 0) {
                    strTemp.AppendFormat(_T("%d"), vecZoneType[i]);
                }
                else {
                    strTemp.AppendFormat(_T(", %d"), vecZoneType[i]);
                }
            }
            strMsg.AppendFormat(_T("\n타입코드 %s -> 0"), strTemp);
        }

        strMsg.Append(_T("\n계속 진행하시겠습니까?"));
        
        if ( AfxMessageBox(strMsg, MB_YESNO) == IDNO) {
            return false;
        }
    }    

    RThreadInfo.Init(); 
    try 
    {

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
        QBicSimpleProgressThread::ExecuteThread(ThreadRun, this, false, nLang); 

        if (RThreadInfo.IsOK() == false) {

        }
        else {

        }
    } 
    catch (int& ex) {
    	CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
    } catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
    } catch (...) {
        TxLogDebugException();
    }

    return true;
}


unsigned __stdcall KDlgNodeZoneTypeEdit::ThreadRun( void* p )
{
    QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
    if (pParameter != nullptr) {
        KDlgNodeZoneTypeEdit* pDlg = (KDlgNodeZoneTypeEdit*)pParameter->GetParameter(); {
            pDlg->Execute();
        }
    }

    return 1;
}

void KDlgNodeZoneTypeEdit::Execute()
{       
    CXTPReportRecords* pRecords = m_wndReport.GetRecords();
    int nCnt = pRecords->GetCount();

    if (nCnt == 0)
        return;

    CXTPReportRecord*     pRecord = nullptr;
    CXTPReportRecordItem* pItem   = nullptr;

    std::vector<int> vecZoneType;
    bool bZero2Max(false);

    int nMaxType(0);
    for (int i=0; i<nCnt; i++) {
        pRecord = pRecords->GetAt(i);
        int nType(0); {
            pItem = pRecord->GetItem(0);
            nType = _ttoi(pItem->GetCaption());

            if (nMaxType <= nType) {
                nMaxType = nType;
            }
        }

        pItem = pRecord->GetItem(2);

        //★ 
        // vecZoneType : 0을 제외한 노드 타입
        // bZero2Max   : 0번이 체크가 풀린 경우, 0번이 없는 경우
        if (pItem->IsChecked() == TRUE) {
            if (nType != 0) {
                vecZoneType.push_back(nType);
            }   
        }
        else {
            if (nType == 0) {
                bZero2Max = true;
            }            
        }
    }

    KDBaseConPtr spDBase = m_spMapServer->Connection();

    //★ 노드 전체 Geometry 조회
    m_spLayerNode->GetGeometry(m_mapGeoNode);
    std::set<__int64> setOut;
    if (vecZoneType.size() > 0) {
        setOut = LoadNode(spDBase, vecZoneType);
    }
       
    if (bZero2Max == true) {
        //★ 기존 존 노드는 제거
        ZoneToNewNodeType(spDBase, nMaxType + 1);
    }

    if (setOut.size() > 0) {
        NodeToZoneType(spDBase, vecZoneType, setOut);
    }
}

std::set<__int64> KDlgNodeZoneTypeEdit::LoadNode( KDBaseConPtr spDBase, std::vector<int> vecNodeType )
{
    std::set<__int64> setOut;

    CString strTemp(_T(""));
    for (size_t i=0; i < vecNodeType.size(); i++) {
        int nType = vecNodeType[i];
        if (i == 0) {            
            strTemp.AppendFormat(_T(" '%d' "), nType);
        }
        else {
            strTemp.AppendFormat(_T(",'%d' "), nType);
        }
    }

    CString strSQL(_T("")); {
        strSQL.AppendFormat(_T(" Select node_id From Node Where node_type in (%s) "), strTemp);
    }

    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
    while(spResult->Next()) {
        setOut.insert(spResult->GetValueI64(0));
    }

    return setOut;
}

void KDlgNodeZoneTypeEdit::ZoneToNewNodeType(KDBaseConPtr spDBase, int nNewType)
{
    //★ 기존 0번 존노드가 풀리는 경우
    
    //★ 노드 타입 정보 갱신
    CString strSQL(_T("")); {
        strSQL.AppendFormat(_T(" Update Node Set node_type = '%d' Where node_type = '0' "), nNewType);
    }
    spDBase->ExecuteUpdate(strSQL);

    //★ 존 레이어 삭제
    m_spLayerZone->Delete();
}

void KDlgNodeZoneTypeEdit::NodeToZoneType( KDBaseConPtr spDBase, std::vector<int>& vecNodeType, std::set<__int64>& setID )
{
    //★ 노드 타입 정보 갱신 / 신규 존 노드에 대한 처리   
    CString strTemp(_T(""));
    for (size_t i=0; i < vecNodeType.size(); i++) {
        int nType = vecNodeType[i];
        if (i == 0) {            
            strTemp.AppendFormat(_T(" '%d' "), nType);
        }
        else {
            strTemp.AppendFormat(_T(",'%d' "), nType);
        }
    }

    CString strSQL(_T("")); {
        strSQL.AppendFormat(_T(" Update Node Set node_type = '0' Where node_type in (%s) "), strTemp);
    }
    
    spDBase->ExecuteUpdate(strSQL);

    //★ 존 정보 생성
    for (auto iter = setID.begin(); iter != setID.end(); ++iter) {
        __int64 nxID = *iter;
        
        auto iFind = m_mapGeoNode.find(nxID);
        if (iFind == m_mapGeoNode.end())
            continue;
        
        ITxGeometryPtr spGeometry = iFind->second;
        TxPointPtr     spPoint    = std::dynamic_pointer_cast<TxPoint>(spGeometry);
        double dX = spPoint->X;
        double dY = spPoint->Y;

        std::vector<TxPoint> vecPoint; { //x, y 좌표를 기점으로 +-50
            vecPoint.push_back(TxPoint(dX+50, dY+50)); // 1사분면
            vecPoint.push_back(TxPoint(dX-50, dY+50)); // 2사분면
            vecPoint.push_back(TxPoint(dX-50, dY-50)); // 3사분면
            vecPoint.push_back(TxPoint(dX+50, dY-50)); // 4사분면
            vecPoint.push_back(TxPoint(dX+50, dY+50));
        }

        spGeometry = TxPolygonPtr(new TxPolygon(vecPoint));
        TxRecord oRecord; {
            oRecord.AddItem(_T("zone_id"), nxID);
        }
        m_spLayerZone->Insert(nxID, spGeometry, oRecord);
    }    
}

void KDlgNodeZoneTypeEdit::OnBnClickedCancel()
{   
    // ESC 키
    //CDialogEx::OnCancel();
}