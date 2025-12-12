#include "stdafx.h"
#include "KmzApp.h"
#include "DlgImportTransitLine.h"
#include "afxdialogex.h"
#include "DefineNames.h"
#include "..\QBicCommon\QBicTxtReader.h"
#include "..\QBicCommon\QBicTxtWriter.h"
#include "DBaseImportTable.h"
#include "ImportCommon.h"
#include "Target.h"
#include "ImChampFrameWindow.h"


void KDlgImportTransitLine::LoadCsvNodeBaseData(ThreadPara* pPara, std::map<__int64, TInputPath>& mapOutPath, bool bExceptionFlag)
{
    TCHAR chSep     = GetSelectedSeparator();
    int   nEncoding = GetSelectedEncoding();

    bool  bContainsColumn(false); {
        if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
            bContainsColumn = true;
    }

    CString strFile(_T("")); {
        m_editFile.GetWindowText( strFile );
    }

    QBicTxtReader oReader; {
        oReader.SetEncoding(nEncoding);
        if (oReader.Open(strFile) == false)
            return;
    }

    QBicTxtWriter oWriterErr; {
        if (oWriterErr.Open(m_strErrFile) == false)
            return;
    }

    std::set<CString> setAlight; {
        AlightColumn(setAlight);
    }

    //★ 텍스트 파일을 읽어서 저장할때 매칭 정보의 인덱스 순으로 저장한다.
    /*^^*/  int nIdxLineID (-1);
    /*^^*/  int nIdxNodeID (-1);
    /*^^*/  int nIdxSeq    (-1);
    /*^^*/  int nIdxStnFlag(-1);
        
    m_vecColMatch.clear(); { // 저장필드와 입력파일의 필드idx 매칭정보 저장(Re)
        try 
        {
            CXTPReportRecords* pRecords = m_ctrlReportColumn.GetRecords();
            int nRecordCount = pRecords->GetCount();

            for (int i= 0; i< nRecordCount; i++) {
                CXTPReportRecord* pRecord = pRecords->GetAt(i);

                int nImportColumnIndex = GetTxtColumnIndex(pRecord);
                if (0 == nImportColumnIndex || pRecord->IsVisible() == FALSE)
                    continue;

                KIOColumn* pIOColumn = GetTovaIOColumn(pRecord);

                //★ 시스템의 필드와 텍스트 파일의 컬럼 인덱스 정보
                TColMatch oTColMatch; {
                    oTColMatch.TovaIOColumn = pIOColumn;
                    oTColMatch.TxtColumnIdx = nImportColumnIndex;
                }

                m_vecColMatch.push_back(oTColMatch);

                CString strName = pIOColumn->Name(); 

                if (strName.CompareNoCase(COLUMN_TRANSIT_ID) == 0 ) {
                    nIdxLineID = (int)m_vecColMatch.size() - 1;
                }
                else if (strName.CompareNoCase(COLUMN_TRANSIT_LINK_SEQ) == 0) {
                    nIdxSeq = (int)m_vecColMatch.size() - 1;
                }
                else if (strName.CompareNoCase(COLUMN_TRANSIT_LINK_ID) == 0) {
                    //★ NodeBase 이기때문에 실제로 Node_id 가 매칭된다.			
                    nIdxNodeID = (int)m_vecColMatch.size() - 1;
                }
                else if(strName.CompareNoCase(COLUMN_TRANSIT_LINK_STATION_YN) == 0) {
                    nIdxStnFlag = (int)m_vecColMatch.size() - 1;
                }
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

    int nFailedCnt(0);
    int nCurrentLine(0);
    int nReadCount(0);
    CString strError(_T(""));

    //★ 첫줄 컬럼 정보
    if( true == bContainsColumn ) {
        CString strLine(_T(""));
        oReader.ReadLine(strLine);
        nCurrentLine++;
    }

    //★ TransitID - Sequencial Data
    std::map<__int64, std::vector<TTemp>> mapRead;
    __int64 nxCnt = m_vecColMatch.size();

    //★ Read
    CString strLine(_T(""));    
    while (oReader.ReadLine(strLine)) {
        nCurrentLine++; 

        strLine.Trim();
        if (strLine.GetLength() == 0) {
            continue;
        }

        nReadCount++;
        std::vector<CString> vecReadLine; {
            ParseCSVLineString(strLine, chSep, vecReadLine);
        }

        if (nxCnt > vecReadLine.size()) {
            strError.Format(_T("Line : %d =>\t 선택한 데이터 컬럼 보다 텍스트 파일의 컬럼의 갯수가 부족합니다.[%d > %d] "), nCurrentLine, nxCnt, vecReadLine.size());
            oWriterErr.Write(strError);
            nFailedCnt++;

            continue;
        }

        //★ 데이터 추출
        __int64 nxLineID(0);
        std::vector<CString> vecReadValue;        
        for (size_t i=0; i<nxCnt; i++) {
            TColMatch& oInfo = m_vecColMatch[i]; 

            //★ 데이터 추출 후 vecColMatch 순서로 정보 저장 
            CString strValue = vecReadLine[oInfo.TxtColumnIdx-1]; {
                vecReadValue.push_back(strValue);
            }            

            const  KIOColumn* pIOColumn = oInfo.TovaIOColumn;
            CString strName = pIOColumn->Name(); 

            if (strName.CompareNoCase(COLUMN_TRANSIT_ID) == 0 ) {
                nxLineID = _ttoi64(strValue);
            }
        }

        if (nxLineID == 0) {
            strError.Format(_T("Line : %d =>\t LineID 정보가 없습니다. "), nCurrentLine);
            oWriterErr.Write(strError);
            nFailedCnt++;

            continue;
        }

        //★ 정보 저장
        TTemp oInfo; {
            oInfo.TextLineNo = nCurrentLine;
            oInfo.TReadValue = vecReadValue;
        }

        std::vector<TTemp>& vecRoutePath = mapRead[nxLineID]; {
            vecRoutePath.push_back(oInfo);
        }
    }

    //★ 노선ID 체크, 노드 존재 여부 체크, 링크(F-T)체크, 노드 연결성 체크, Seq 체크
    for (auto iter = mapRead.begin(); iter != mapRead.end(); ++iter) 
    {
        __int64 nxLineID = iter->first;
        std::vector<TTemp>& vecRoutePath = iter->second;

        if (IsErrorTransitID(nxLineID) == true) {
            TTemp& oInfo = vecRoutePath[0]; {
                strError.Format(_T("Line : %d =>\t 존재하지 않는 LineID 입니다."), oInfo.TextLineNo);
                oWriterErr.Write(strError);
            }            

            nFailedCnt += (int)vecRoutePath.size();
            continue;
        }
        
        //★ 경로 1개인 경우(우씽)
        if (vecRoutePath.size() == 1) {
            TTemp& oInfo = vecRoutePath[0]; {
                strError.Format(_T("Line : %d =>\t 출발 정류장만 존재합니다. 도착 정류장 정보를 알 수 없습니다."), oInfo.TextLineNo);
                oWriterErr.Write(strError);
            }            

            nFailedCnt += (int)vecRoutePath.size();
            continue;
        }
        
        bool bPathErr(false);
        for (size_t i=0; i<vecRoutePath.size()-1; i++) {
            int     nSeqCur(-1),     nSeqNxt(-1);
            __int64 nxNodeIDCur(-1), nxNodeIDNxt(-1); 

            TTemp& oInfoCur = vecRoutePath[i]; 
            /*-*/  std::vector<CString>& vecValueCur = oInfoCur.TReadValue; {
                nSeqCur     = _ttoi  (vecValueCur[nIdxSeq]);
                nxNodeIDCur = _ttoi64(vecValueCur[nIdxNodeID]);
            }

            TTemp& oInfoNxt = vecRoutePath[i+1];
            /*-*/  std::vector<CString>& vecValueNxt = oInfoNxt.TReadValue; {
                nSeqNxt     = _ttoi  (vecValueNxt[nIdxSeq]);
                nxNodeIDNxt = _ttoi64(vecValueNxt[nIdxNodeID]);
            }
            
            //★ 순번 체크
            if (nSeqCur != nSeqNxt-1) {
                strError.Format(_T("Line : %d =>\t 정류장 순번 정보가 잘 못 되었습니다."), oInfoCur.TextLineNo);
                oWriterErr.Write(strError);
                
                nFailedCnt++;
                bPathErr = true;
                continue;
            }

            //★ 링크 구성 체크
            auto iFind = m_mapFromToLink.find(KODKey(nxNodeIDCur, nxNodeIDNxt));
            if (iFind == m_mapFromToLink.end()) {
                strError.Format(_T("Line : %d =>\tLink 테이블에 존재하지 않는 FLink : F_NodeID = %I64d, TNodeID = %I64d"), oInfoCur.TextLineNo, nxNodeIDCur , nxNodeIDNxt);
                oWriterErr.Write(strError);

                nFailedCnt++;
                bPathErr = true;
                continue;
            }

            //★ 노드 구성 체크<불필요> 
            /*
            if (i == 0) {
                if( CheckNodeData(nxNodeIDCur) ) {
                    strErrorMsg.Format(_T("Line : %d =>\tNode 테이블에 존재하지 않는 Node_ID : %I64d"), oInfoCur.TextLineNo, nxNodeIDCur);
                    ofErr.Write(strErrorMsg);
                    
                    nFailedCnt++;
                    bPathErr = true;
                    continue;
                }
            }

            if( CheckNodeData(nxNodeIDNxt) ) {
                strErrorMsg.Format(_T("Line : %d =>\tNode 테이블에 존재하지 않는 Node_ID : %I64d"), oInfoNxt.TextLineNo, nxNodeIDNxt);
                ofErr.Write(strErrorMsg);
               
                nFailedCnt++;
                bPathErr = true;
                continue;
            }*/
        }

        if (bPathErr == true) {
            continue;
        }

        //★ 지금 부터 링크 베이스로 데이터를 구성한다.
        //★ LineID
        //★ LinkID, Values

        TInputPath& oPath = mapOutPath[nxLineID];
        
        size_t nxStnCnt = vecRoutePath.size();
        for (size_t i=0; i<nxStnCnt-1; i++) {
            TTemp& oInfoCur = vecRoutePath[i]; 
            std::vector<CString>& vecValueCur = oInfoCur.TReadValue; 
            __int64 nxNodeIDCur = _ttoi64(vecValueCur[nIdxNodeID]);

            TTemp& oInfoNxt = vecRoutePath[i+1];
            std::vector<CString>& vecValueNxt = oInfoNxt.TReadValue; 
            __int64 nxNodeIDNxt = _ttoi64(vecValueNxt[nIdxNodeID]);

            //★ 링크 구성 체크를 통과했다.
            auto iFind = m_mapFromToLink.find(KODKey(nxNodeIDCur, nxNodeIDNxt));
            __int64 nxLinkID = iFind->second;
            
            TInputSubPathInfo oSubPath;
            std::vector<CString>& vecOut = oSubPath.TValues;

            for (size_t nxI =0; nxI<m_vecColMatch.size(); nxI++) {
                TColMatch& oInfo = m_vecColMatch[nxI]; 

                const  KIOColumn* pIOColumn = oInfo.TovaIOColumn;
                CString strName = pIOColumn->Name(); {
                    strName.MakeLower();
                }

                //★ 라인 링크 ID 처리
                if (strName.CompareNoCase(COLUMN_TRANSIT_LINK_ID) == 0) {
                    CString strTemp(_T("")); {
                        strTemp.Format(_T("%I64d"), nxLinkID);
                    }

                    vecOut.push_back(strTemp);
                    /*★*/continue; 
                }

                //★ 라인 시퀀스 처리
                if (strName.CompareNoCase(COLUMN_TRANSIT_LINK_SEQ) == 0) {
                    CString strTemp(_T("")); {
                        strTemp.Format(_T("%d"), i + 1);
                    }
                    vecOut.push_back(strTemp);
                    /*★*/continue; 
                }

                CString strValueCur = vecValueCur[nxI];
                CString strValueNxt = vecValueNxt[nxI];

                //★ 정류장 처리
                //-- ★ 마지막은 무조건 정류장
                if (i == nxStnCnt-2) {
                    if (strName.CompareNoCase(COLUMN_TRANSIT_LINK_STATION_YN) == 0) {
                        vecOut.push_back(_T("1"));
                        /*★*/continue; 
                    }
                }

                //-- ★ 마지막 정류장은 앞쪽에서 해결 했으니
                if (strName.CompareNoCase(COLUMN_TRANSIT_LINK_STATION_YN) == 0) {
                    int nFlag = _ttoi(strValueNxt);
                    if (nFlag == 0) {
                        vecOut.push_back(_T("0"));
                    }
                    else {
                        vecOut.push_back(_T("1"));
                    }
                    /*★*/continue; 
                }

                //★ Boarding / Alighting - 기본적으로 Boarding 중심으로 / 
                auto iFind = setAlight.find(strName);
                
                if (iFind == setAlight.end()) {
                    //★ Boarding
                    vecOut.push_back(strValueCur);
                }
                else {
                    //★ Alight
                    vecOut.push_back(strValueNxt);
                }
            }

            oPath.TSubPaths.push_back(oSubPath);
        }
    }

    int nCount(nReadCount);
    pPara->TKeyInt32[0] = nCount;
    pPara->TKeyInt32[1] = nCount - nFailedCnt;
    pPara->TKeyInt32[2] = nFailedCnt;

    oWriterErr.Close();
    if (nFailedCnt > 0 && bExceptionFlag == true) {
        CString strErrorMsg(_T(""));
        strErrorMsg.Format(_T("Import 실패\r\n %s 파일에서 내역을 확인 하세요."), IMPORT_ERROR_FILE_NAME);
        ErrorFileOpen();
        ThrowException(strErrorMsg);
    }    
}

void KDlgImportTransitLine::ImportNodeBaseData(ThreadPara* pPara, bool bInsertFlag)
{  
    CString strColumns(_T(""));
    CString strValuess(_T(""));
    for (size_t i=0; i<m_vecColMatch.size(); i++) {
        TColMatch& oInfo = m_vecColMatch[i];
        const
        KIOColumn* pIOColumn = oInfo.TovaIOColumn;

        if (i== 0) {
            strColumns.AppendFormat(_T(" %s "), pIOColumn->Name());
            strValuess.Append(_T(" ? "));
        }
        else {
            strColumns.AppendFormat(_T(" ,%s "), pIOColumn->Name());
            strValuess.Append(_T(" ,? "));
        }
    }

    CString strSQL(_T("")); {
        strSQL.AppendFormat(_T(" Insert Into %s "), m_pTable->Name());
        strSQL.AppendFormat(_T(" ( %s ) "), strColumns);
        strSQL.AppendFormat(_T(" Values ( %s ) "), strValuess);
    }

    KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
    spDBase->BeginTransaction(); 

    if (true /*테이블 전체 삭제*/) {
        CString strSQL(_T("")); {
            strSQL.Format(_T("delete from %s"), m_pTable->Name());
            
            //★ Update 
            if ( bInsertFlag == false) {
                CString strTemp(_T(""));
                bool bFirst(true);
                for (auto iter = m_mapOutPath.begin(); iter != m_mapOutPath.end(); ++iter) {
                    __int64 nxLineID = iter->first;

                    if (bFirst) {
                        strTemp.AppendFormat(_T(" '%I64d' "), nxLineID);
                        bFirst = false;
                    }
                    else {
                        strTemp.AppendFormat(_T(", '%I64d' "), nxLineID);
                    }                    
                }

                strSQL.AppendFormat(_T(" Where %s in ( %s ) "), COLUMN_TRANSIT_ID, strTemp);
            }
            spDBase->ExecuteUpdate(strSQL);
        }
    }
    
    try 
    {
        KPreparedPtr spPrepared = spDBase->PrepareStatement(strSQL);

        for (auto iter = m_mapOutPath.begin(); iter != m_mapOutPath.end(); ++iter) 
        {
            TInputPath& oInputPath = iter->second;
            std::vector<TInputSubPathInfo>& vecSubPath = oInputPath.TSubPaths;
            
            for (size_t i=0; i<vecSubPath.size(); i++) {
                TInputSubPathInfo& oSubPath= vecSubPath[i];
                
                //★ 저장된 데이터
                std::vector<CString>& vecValues = oSubPath.TValues;
                
                for (size_t i=0; i<m_vecColMatch.size(); i++) 
                {
                    TColMatch& oInfo = m_vecColMatch[i];
                    const
                    KIOColumn* pIOColumn = oInfo.TovaIOColumn;
                    
                    CString strValue = vecValues[i];
                    if (pIOColumn->DataType() == KEMIODataTypeInteger) {
                        spPrepared->BindInt64(i+1, _ttoi64(strValue));
                    }
                    else if (pIOColumn->DataType() == KEMIODataTypeDouble) {
                        spPrepared->BindDouble(i+1, _ttof(strValue));
                    }
                    else {
                        spPrepared->BindText(i+1, strValue);
                    }
                }

                spPrepared->ExecuteUpdate();
                spPrepared->Reset();
            }
        }

        spDBase->Commit();       
    } 
    catch (int& ex) {
        CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
        spDBase->RollBack();
        throw 1;
    } 
    catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
        spDBase->RollBack();
        throw 1;
    } 
    catch (...) {
        TxLogDebugException();
        spDBase->RollBack();
        throw 1;
    }
}

void KDlgImportTransitLine::AlightColumn( std::set<CString>& setAlight )
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

