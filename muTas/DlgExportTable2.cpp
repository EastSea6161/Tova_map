#include "stdafx.h"
#include "KmzApp.h"
#include "DlgExportTable.h"
#include "afxdialogex.h"
#include "ImportTableInfo.h"
#include "..\QBicCommon\QBicTxtWriter.h"
#include "Target.h"
#include "ImChampFrameWindow.h"

void KDlgExportTable::OnBnClickedOk()
{
    UpdateData( TRUE );

    CString strFile; {
        m_editFile.GetWindowText( strFile );
        if( strFile.IsEmpty() == true ) {
            AfxMessageBox( _T("파일명이 비어있습니다.") );
            return;
        }
    }

    BOOL bExportColumn; {
        if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
            bExportColumn = TRUE;
        else
            bExportColumn = FALSE;
    }

    int nEncoding = KEMCSVEncodingUTF8; {
        int nCur = m_cboEncoding.GetCurSel();
        if (nCur >= 0) {
            nEncoding = (int)m_cboEncoding.GetItemData(nCur);
        }
    }

    CString strSeparator; {
        int nSelRadioID = GetCheckedRadioButton(IDC_RADIO8, IDC_RADIO11);
        if (IDC_RADIO8 == nSelRadioID)
            strSeparator = _T(",");
        else if (IDC_RADIO9 == nSelRadioID)
            strSeparator = _T(";");
        else if (IDC_RADIO10 == nSelRadioID)
            strSeparator = _T("|");
        else if (IDC_RADIO11 == nSelRadioID)
            strSeparator = _T("\t");
        else
            return;
    }

    m_oExportColumns.Clear(); {
        CXTPReportRecords*	  pRecords = nullptr;
        CXTPReportRecord*     pRecord  = nullptr;
        CXTPReportRecordItem* pItem    = nullptr;

        pRecords         = m_ctrlReport.GetRecords();
        int nRecordCount = pRecords->GetCount();

        for (int i= 0; i< nRecordCount; i++)
        {
            pRecord    = pRecords->GetAt(i);
            pItem      = pRecord->GetItem(0); 

            if (pItem->IsChecked()) {
                KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();
                m_oExportColumns.AddNewColumn(pColumn);
            }
        }
    }

    try 
    {
        ThreadPara oPara(this);

        if (true) {
            oPara.TKeyInt32[0] = bExportColumn;   // First Header 여부
            oPara.TKeyInt32[1] = nEncoding;       // 인코딩 정보
            oPara.TKeyStr[0] = strFile;         // 파일path
            oPara.TKeyStr[1] = strSeparator;    // 구분자

            RThreadInfo.Init();

			// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
			int nLang = 1;
			if (KmzSystem::GetLanguage() == KEMKorea)
			{
				nLang = 0;
			}

            QBicSimpleProgressThread::ExecuteThread(ThreadRun, &oPara, false, nLang);

            if (RThreadInfo.IsOK() == false) {
                ThrowException(RThreadInfo.ErrorMsg());
            }
        }
    } catch (int& ex) {
        CString strError(_T(""));
        strError.Format(_T("오류 : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
        AfxMessageBox(strError);
        return;
    } catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
        AfxMessageBox(ex->GetErrorMessage());
        return;
    } catch (...) {
        TxLogDebugException();
        AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
        return;
    }

    OnOK();
}

unsigned __stdcall KDlgExportTable::ThreadRun( void* p )
{
    QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
    if (pParameter != nullptr) {
        ThreadPara* pPara = (ThreadPara*)pParameter->GetParameter(); {
            KDlgExportTable* pDlg = (KDlgExportTable*)pPara->TWindow;
            pDlg->Execute(pPara);
        }
    }

    return 1;
}

void KDlgExportTable::Execute(ThreadPara* pPara)
{
    try 
    {
        ExportTable(pPara);
    } catch (int& ex) {
        CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
        RThreadInfo.SetErrorFlag(-1, strError);
    } catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
        RThreadInfo.SetErrorFlag(-1, ex->GetErrorMessage());
    } catch (...) {
        TxLogDebugException();
        RThreadInfo.SetErrorFlag(-1, _T("오류가 발생하였습니다."));
    }
}

void KDlgExportTable::ExportTable( ThreadPara* pPara )
{
    TxLogDebugStartMsg();

    BOOL    bExportColumn = (BOOL)pPara->TKeyInt32[0]; // First Header 여부
    int     nEncoding     = (int)pPara->TKeyInt32[1]; // 인코딩 정보
    CString strFileName   = pPara->TKeyStr[0]; // 파일path
    CString strSeparator  = pPara->TKeyStr[1]; // 구분자

    QBicTxtWriter oWriter;
    if (oWriter.Open(strFileName, nEncoding) == false) {
        ThrowException(_T("파일을 열 수 없습니다."));
    }

    int nColumnCount = m_oExportColumns.ColumnCount();

    //transit export시, Node 선택시의 로직
    if (m_bSetDataType && m_cboDataType.GetItemData(m_cboDataType.GetCurSel()) == KEMBaseNode) {
        //write column header
        if(TRUE == bExportColumn)	{
            CString strColumnLine = _T(""); {
                for(int i= 0; i< nColumnCount; i++) {
                    KIOColumn* pColumn = m_oExportColumns.GetColumn(i);
                    if(0 < i) {
                        strColumnLine.Append(strSeparator);
                    }
                    strColumnLine.Append(_T("\""));

                    CString strColumnName = pColumn->Name();
                    if (strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0) {
                        strColumnLine.Append(COLUMN_NODE_ID);
                    } else {
                        strColumnLine.Append(pColumn->Caption());
                    }

                    strColumnLine.Append(_T("\""));
                }
            }

            oWriter.Write(strColumnLine);
        }
        //write data
        ExportTransit2TxtTypeNode(strSeparator, oWriter);
    } else {
        //write Column header
        if(TRUE == bExportColumn) {
            CString strColumnLine = _T("");
            for(int i= 0; i< nColumnCount; i++) {
                KIOColumn* pColumn = m_oExportColumns.GetColumn(i);
                if(0 < i) {
                    strColumnLine.Append(strSeparator);
                }
                strColumnLine.Append(_T("\""));
                strColumnLine.Append(pColumn->Caption());
                strColumnLine.Append(_T("\""));
            }

            oWriter.Write(strColumnLine);
        }

        ExportDB2Txt(strSeparator, oWriter);
    }

    oWriter.Close();

    TxLogDebugEndMsg();
}

void KDlgExportTable::ExportDB2Txt(LPCTSTR lpszSeparator, QBicTxtWriter &oWriter )
{
    try 
    {
        CString strQuery(_T(""));
        strQuery.Append(_T(" SELECT "));

        int nColumnCount = m_oExportColumns.ColumnCount();
        for(int i= 0; i< nColumnCount; i++) {
            KIOColumn* pColumn = m_oExportColumns.GetColumn(i);

            if(i != 0) {
                strQuery.Append(_T("||'"));
                strQuery.Append(lpszSeparator);
                strQuery.Append(_T("'||"));
            }

            if(KEMIODataTypeString == pColumn->DataType()) {
                strQuery.Append(_T("'\"'||"));
                strQuery.Append(_T("IFNULL("));
                strQuery.Append(pColumn->Name());
                strQuery.Append(_T(", '')"));
                strQuery.Append(_T("||'\"'"));
            } else {
                strQuery.Append(_T("IFNULL("));
                strQuery.Append(pColumn->Name());
                strQuery.Append(_T(", '0')"));
            }
        }
        strQuery.Append(_T(" From "));
        strQuery.Append(m_pTable->Name());

        if (m_strFilter.IsEmpty() == false) {
            strQuery.AppendFormat(_T(" Where %s "), m_strFilter);
        }
        
        //Write Text	
        KDBaseConPtr spDBase     = m_pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet = spDBase->ExecuteQuery(strQuery);

        while (spResultSet->Next()) {
            CString strLine = spResultSet->GetValueString(0);
            oWriter.Write(strLine);
        }
    } catch (int& ex) {
        CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
    } catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
    } catch (...) {
        TxLogDebugException();
    }
}

void KDlgExportTable::LoadLinkFTInfo(std::map<__int64, TFTNode>& mapLinkFTInfo)
{
    mapLinkFTInfo.clear();

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select link_id, fnode_id, tnode_id From link "));
    }
    KDBaseConPtr  spDBase  = m_pTarget->GetDBaseConnection();
    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);

    __int64 nxLinkdID(0);

    while (spResult->Next()) {
        nxLinkdID        = spResult->GetValueI64(0);
        TFTNode oFTNode ; {
            oFTNode.FNodeID  = spResult->GetValueI64(1);
            oFTNode.TNodeID  = spResult->GetValueI64(2);
        }
        mapLinkFTInfo.insert(std::make_pair(nxLinkdID, oFTNode));
    }
}

//★ 트랜짓-라인 데이터 Export
void KDlgExportTable::ExportTransit2TxtTypeNode(LPCTSTR lpszSep, QBicTxtWriter &oWriter )
{
    try 
    {        
        std::map<__int64, TFTNode> mapLinkFTInfo; {
            LoadLinkFTInfo(mapLinkFTInfo);
        }

        int nColumnCnt = m_oExportColumns.ColumnCount();
               
        //★ 첫번째 컬럼은 무조건 << TransitID/LinkID >>를 조회한다. -> 정보 저장을 위해
        //★ 주의사항 - 사용자는 TransitID, LinkID 항목을 Export 선택을 안할 수 있다.

        CString strSQL(_T("")); {
            strSQL.Append(_T(" Select transit_id, link_id "));
            for(int i= 0; i< nColumnCnt; i++) {            
                KIOColumn* pColumn = m_oExportColumns.GetColumn(i);
                CString    strName = pColumn->Name();
                strSQL.AppendFormat(_T(" ,%s "), strName);
            }
            strSQL.AppendFormat(_T(" From %s "), TABLE_TRANSITLINKS);

            if (m_strFilter.IsEmpty() == false)
                strSQL.AppendFormat(_T(" Where %s"), m_strFilter);

            strSQL.Append(_T(" Order By transit_id, seq"));
        }

        class TSubPath {
        public:
            TSubPath() {
                TLinkID = -1;
            }
        public:
            __int64 TLinkID;
            std::vector<CString> TValues;
        };

        KDBaseConPtr  spDBase  = m_pTarget->GetDBaseConnection();
        KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
           
        //★ Line_Id, <사용자가 선택한 컬럼>
        std::map<__int64, std::vector<TSubPath>> mapPath;
        while (spResult->Next()) {
            __int64 nxLineID = spResult->GetValueI64(0);
            __int64 nxLinkID = spResult->GetValueI64(1);

            //★ 일단 라인별로 데이터를 조회한다.
            std::vector<TSubPath>& vecSubPath = mapPath[nxLineID];
            CString strOut(_T(""));

            TSubPath oPath; {
                oPath.TLinkID = nxLinkID;
            }
            std::vector<CString>& vecValues = oPath.TValues;

            for (int i=0; i<nColumnCnt; i++) {
                KIOColumn* pIOColumn = m_oExportColumns.GetColumn(i);

                CString strValue(_T(""));
                if (pIOColumn->DataType() == KEMIODataTypeDouble) {
                    strValue.Format(_T("%f"),    spResult->GetValueDbl(i+2));
                } 
                else if (pIOColumn->DataType() == KEMIODataTypeInteger) {
                    strValue.Format(_T("%I64d"), spResult->GetValueI64(i+2));
                }
                else {
                    strValue.Format(_T("%s"),    spResult->GetValueStr(i+2));
                } 

                vecValues.push_back(strValue);
            }

            vecSubPath.push_back(oPath);
        }

        if (mapPath.size() == 0)
            return;

        std::set<CString> setAlight; {
            AlightColumn(setAlight);
        }

        //★ 지금부터 노드베이스 데이터를 만든다.
        std::map<__int64, std::vector<TSubPath>> mapOutPath;
        for (auto iter = mapPath.begin(); iter != mapPath.end(); ++iter) 
        {
            __int64 nxLineID = iter->first;
            CString strLineID(_T("")); {
                strLineID.Format(_T("%I64d"), nxLineID);
            }
            std::vector<TSubPath>& vecSubPath = iter->second;
            if (vecSubPath.size() == 0)
                continue;

            std::vector<TSubPath>& vecOutSubPath = mapOutPath[nxLineID];            
            size_t nxCnt = vecSubPath.size();
            //★ 빈방을 만든다. 노드베이스이므로 +1 개
            //★ 모든 값을 0으로 채운다.
            for (size_t idx=0; idx<=nxCnt; idx++) 
            {
                TSubPath oOutPath;
                std::vector<CString>& vecOutValue = oOutPath.TValues;
                for (int i=0; i<nColumnCnt; i++) {
                    KIOColumn* pIOColumn = m_oExportColumns.GetColumn(i);
                    CString    strName   = pIOColumn->Name();

                    if (strName.CollateNoCase(COLUMN_TRANSIT_ID) == 0) {
                        vecOutValue.push_back(strLineID);
                        continue;
                    }

                    if (strName.CompareNoCase(COLUMN_LINK_ID) == 0) {                        
                        CString strNodeID(_T(""));
                        if (idx == nxCnt) {
                            TSubPath& oPathLink = vecSubPath[idx-1];
                            TFTNode& oInfo = mapLinkFTInfo[oPathLink.TLinkID];

                            strNodeID.Format(_T("%I64d"), oInfo.TNodeID);
                        }
                        else {
                            TSubPath& oPathLink = vecSubPath[idx];
                            TFTNode& oInfo = mapLinkFTInfo[oPathLink.TLinkID];
                            strNodeID.Format(_T("%I64d"), oInfo.FNodeID);
                        }

                        vecOutValue.push_back(strNodeID);
                        continue;
                    }

                    if (strName.CompareNoCase(COLUMN_TRANSIT_LINK_SEQ) == 0) {
                        CString strTemp(_T("")); {
                            strTemp.Format(_T("%d"), idx + 1);
                        }
                        vecOutValue.push_back(strTemp);
                        continue;
                    }

                    if (strName.CompareNoCase(COLUMN_TRANSIT_LINK_STATION_YN) == 0) {
                        if (idx == 0 || idx == nxCnt) {
                            vecOutValue.push_back(_T("1"));
                            continue;
                        }     
                        else {
                            TSubPath& oPathLink = vecSubPath[idx-1];
                            vecOutValue.push_back(oPathLink.TValues[i]);
                            continue;
                        }
                    }                    

                    //★ 여기까지 살아 남의면.. Boarding, Alighting, Passenger
                    bool bBoarding(false);
                    auto iFind = setAlight.find(strName);
                    if (iFind == setAlight.end()) {
                        bBoarding = true;
                    }

                    if (bBoarding == true) {
                        if (idx == nxCnt) {
                            vecOutValue.push_back(_T("0"));
                        }
                        else {
                            TSubPath& oPathLink = vecSubPath[idx];
                            vecOutValue.push_back(oPathLink.TValues[i]);
                            continue;
                        }
                    }
                    else {
                        if (idx == 0) {
                            vecOutValue.push_back(_T("0"));
                        }
                        else {
                            TSubPath& oPathLink = vecSubPath[idx-1];
                            vecOutValue.push_back(oPathLink.TValues[i]);
                            continue;
                        }
                    }
                }

                vecOutSubPath.push_back(oOutPath);
            }
        }
        
        //★ 출력
        for (auto iter = mapOutPath.begin(); iter != mapOutPath.end(); ++iter) {
            std::vector<TSubPath>& vecOutSubPath = iter->second;
            
            for (size_t i=0; i<vecOutSubPath.size(); i++) {
                TSubPath& oPath = vecOutSubPath[i];
                
                std::vector<CString>& vecValues = oPath.TValues;
                CString strOut(_T("")); 
                for (size_t i=0; i<vecValues.size(); i++) {
                    if (i == 0) {
                        strOut.AppendFormat(_T("%s"), vecValues[i]);
                    }
                    else {
                        strOut.AppendFormat(_T("%s%s"), lpszSep, vecValues[i]);
                    }                    
                }

                oWriter.Write(strOut);
            }
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
}


void KDlgExportTable::AlightColumn( std::set<CString>& setAlight )
{
    KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  Description, "));
        strSQL.Append(_T("  line_board_net,  line_board_Transfer,  line_board_Total, "));
        strSQL.Append(_T("  line_alight_net, line_alight_Transfer, line_alight_Total,"));
        strSQL.Append(_T("  line_passenger "));
        strSQL.AppendFormat(_T(" From %s "), TABLE_ASSIGN_MODEL);
        strSQL.AppendFormat(_T(" Where type in (%d, %d) ORDER BY run_seq "), 5, 6);
    }

    KResultSetPtr spResult  = spDBase->ExecuteQuery(strSQL);
    while(spResult->Next()) {
        spResult->GetValueStr(0);

        CString strName(_T(""));
        strName = spResult->GetValueStr(1); {
        }

        strName = spResult->GetValueStr(2); {
        }
        strName = spResult->GetValueStr(3); {
        }

        strName = spResult->GetValueStr(4); {
            strName.MakeLower();
            setAlight.insert(strName);
        }

        strName = spResult->GetValueStr(5); {
            strName.MakeLower();
            setAlight.insert(strName);
        }

        strName = spResult->GetValueStr(6); {
            strName.MakeLower();
            setAlight.insert(strName);
        }

        strName = spResult->GetValueStr(7); {
            strName.MakeLower();
            setAlight.insert(strName);
        }
    }    
}
