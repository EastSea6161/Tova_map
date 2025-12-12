#include "stdafx.h"
#include "MultiModalMainDlg.h"

#include "KBulkDBase.h"

void KMultiModalMainDlg::UpdateLineInfoFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rLineInfoColumnCollection )
{

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*		 pLineInfoTalbe	   = pIOTables->FindTable(TABLE_TRANSIT);
	int nColumnCount = rLineInfoColumnCollection.ColumnCount();

	if (nColumnCount == 0)
	{
		ThrowException(_T("적용 컬럼 정보가 없습니다."));
	}

	try
	{
		CString strOutLinkFile = ConvertSavePath(_T("line_info_out.dat"));
		std::ifstream ifs( strOutLinkFile, std::ios::binary );    
		KIDKeyDoubleRecords records(nColumnCount);
		__int64 nxLineID;
		double dValue;

		while (!ifs.eof())
		{
			int nIndex = 0;
			ifs.read( reinterpret_cast<char*>(&nxLineID), sizeof(Integer) );
			if(ifs.eof())
				break;
			KSDoubleRecord* pRecord = records.AddRecord(nxLineID);


			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //승객수
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //총수송거리
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //인당평균수송거리
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //최대승차인원
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //평균승차인원
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //최대재차인원
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //평균재차인원
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //최대입석인원
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //평균입석인원
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //최대 혼잡율
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //평균 혼잡율
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //km 당 수송인원
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //1회 운행당 수송인원
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //km당 1회 수송인원
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

		}
		ifs.close();

		KBulkDBase::BulkLineinfoUpdate(spDBaseConnection, pLineInfoTalbe, rLineInfoColumnCollection, records);

	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
	TxLogDebugEndMsg();
}


void KMultiModalMainDlg::UpdateLineFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& oTransitLineColumns )
{
    //★ KInterModalMainDlg::UpdateLineFile2DB 함수와 동일
    KIOTables* pIOTables  = m_pTarget->Tables();
    KIOTable*  pLineTable = pIOTables->FindTable(TABLE_TRANSITNODES);
    int nColumnCount = oTransitLineColumns.ColumnCount();

    if (nColumnCount == 0) {
        ThrowException(_T("적용 컬럼 정보가 없습니다."));
    }

    CString strFilePath = ConvertSavePath(_T("line_out.dat"));
    std::ifstream ifs(strFilePath, std::ios::binary );    
    if( ! ifs ) {
        ThrowException(_T("File Not Found"));
    }

    //★ 변수 초기화
    __int64 nxLineID(0);
    int		nSeq(0);
    double  dBoardnet(0), dBoardTransfer(0), dBoardTotal(0), dAlightnet(0), dAlightTransfer(0), dAlighterTotal(0), dPassenger(0);

    class TTemp {
    public:
        TTemp() {
            TSeq = -1;
            TBoard[0] = 0;
            TBoard[1] = 0;
            TBoard[2] = 0;

            TAlight[0] = 0;
            TAlight[1] = 0;
            TAlight[2] = 0;

            TPassenger = 0;
        }
    public:
        int    TSeq;
        double TBoard [3];
        double TAlight[3];
        double TPassenger;
    };

    //★ 모형 결과를 일차적으로 담는다.
    std::map<__int64, std::vector<TTemp>> mapData;

    while (!ifs.eof()) {			
        ifs.read( reinterpret_cast<char*>(&nxLineID), sizeof(__int64));
        if(ifs.eof())
            break;

        //★ Sequence
        ifs.read( reinterpret_cast<char*>(&nSeq),			sizeof(int));					

        //★ Board
        ifs.read( reinterpret_cast<char*>(&dBoardnet),		sizeof(double));
        ifs.read( reinterpret_cast<char*>(&dBoardTransfer),	sizeof(double));
        ifs.read( reinterpret_cast<char*>(&dBoardTotal),	sizeof(double));

        //★ Alight
        ifs.read( reinterpret_cast<char*>(&dAlightnet),		sizeof(double));
        ifs.read( reinterpret_cast<char*>(&dAlightTransfer),sizeof(double));
        ifs.read( reinterpret_cast<char*>(&dAlighterTotal), sizeof(double));

        //★ 재차인원
        ifs.read( reinterpret_cast<char*>(&dPassenger),		sizeof(double));

        TTemp oData; {
            oData.TSeq       = nSeq;

            oData.TBoard[0]  = dBoardnet < 0.0 ? 0.0 : dBoardnet;
            oData.TBoard[1]  = dBoardTransfer < 0.0 ? 0.0 : dBoardTransfer;
            oData.TBoard[2]  = dBoardTotal < 0.0 ? 0.0 : dBoardTotal;

            oData.TAlight[0] = dAlightnet < 0.0 ? 0.0 : dAlightnet;
            oData.TAlight[1] = dAlightTransfer < 0.0 ? 0.0 : dAlightTransfer;
            oData.TAlight[2] = dAlighterTotal < 0.0 ? 0.0 : dAlighterTotal;

            oData.TPassenger = dPassenger;
        }

        std::vector<TTemp>& vecData = mapData[nxLineID];
        vecData.push_back(oData);
    }
    
    CString strSQL (_T("")); {
        CString strColumn1 = oTransitLineColumns.GetColumn(0)->Name();
        CString strColumn2 = oTransitLineColumns.GetColumn(1)->Name();
        CString strColumn3 = oTransitLineColumns.GetColumn(2)->Name();
        CString strColumn4 = oTransitLineColumns.GetColumn(3)->Name();
        CString strColumn5 = oTransitLineColumns.GetColumn(4)->Name();
        CString strColumn6 = oTransitLineColumns.GetColumn(5)->Name();
        CString strColumn7 = oTransitLineColumns.GetColumn(6)->Name();

        strSQL.Format(
            _T(" Update %s set  ")
            _T(" %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?  ")
            _T(" Where Transit_ID = ? and Seq = ? "), TABLE_TRANSITNODES, oTransitLineColumns.GetColumn(0)->Name(),
            oTransitLineColumns.GetColumn(1)->Name(), oTransitLineColumns.GetColumn(2)->Name(),
            oTransitLineColumns.GetColumn(3)->Name(), oTransitLineColumns.GetColumn(4)->Name(),
            oTransitLineColumns.GetColumn(5)->Name(), oTransitLineColumns.GetColumn(6)->Name());
    }

    KPreparedPtr spPrepare = spDBaseConnection->PrepareStatement(strSQL);

    for (auto iter = mapData.begin(); iter != mapData.end(); ++iter) {
        __int64 nxLineID = iter->first;
        std::vector<TTemp>& vecData = iter->second;
        for (size_t i=0; i<vecData.size(); i++) {
            TTemp& oInfo = vecData[i];

            spPrepare->BindDouble(1, oInfo.TBoard[0]);
            spPrepare->BindDouble(2, oInfo.TBoard[1]);
            spPrepare->BindDouble(3, oInfo.TBoard[2]);
            spPrepare->BindDouble(4, oInfo.TAlight[0]);
            spPrepare->BindDouble(5, oInfo.TAlight[1]);
            spPrepare->BindDouble(6, oInfo.TAlight[2]);
            spPrepare->BindDouble(7, oInfo.TPassenger);
            spPrepare->BindInt64 (8, nxLineID);
            spPrepare->BindInt   (9, oInfo.TSeq);

            spPrepare->ExecuteUpdate();
            spPrepare->Reset();
        }        
    }
}

/*
void KMultiModalMainDlg::UpdateLineFile2DB_Old20161005( KDBaseConPtr spDBaseConnection, KIOColumns& rLineColumnCollection )
{
	KIOTables* pIOTables  = m_pTarget->Tables();
	KIOTable*  pLineTable = pIOTables->FindTable(TABLE_TRANSITLINKS); //★ 확인
	int nColumnCount = rLineColumnCollection.ColumnCount();

	if (nColumnCount == 0) {
		ThrowException(_T("적용 컬럼 정보가 없습니다."));
	}

	try
	{
		CString strOutLinkFile = ConvertSavePath(_T("line_out.dat"));
		std::ifstream ifs(strOutLinkFile, std::ios::binary );    
		if( !ifs )
			ThrowException(_T("File Not Found"));

		std::vector<TMATransitLine> vecTransitLine;

		__int64 nxLineID;
		int		nSeq;
		double  dBoardnet, dBoardTransfer, dBoardTotal, dAlightnet, dAlightTransfer, dAlighterTotal, dPassenger;

        // 모형에서 넘겨준값을 정확하게 저장
        std::vector<TMATransitLine> vecModelTransitLine;
        std::map<KODKey, TMATransitLine> mapModelTransitLine;

		while (!ifs.eof()) 
		{
			TMATransitLine oTransitLine;			
            ifs.read( reinterpret_cast<char*>(&nxLineID),     sizeof(Integer));
			if(ifs.eof())
				break;

			ifs.read( reinterpret_cast<char*>(&nSeq),				sizeof(int));					
            ifs.read( reinterpret_cast<char*>(&dBoardnet),			sizeof(double));
			ifs.read( reinterpret_cast<char*>(&dBoardTransfer),		sizeof(double));
			ifs.read( reinterpret_cast<char*>(&dBoardTotal),		sizeof(double));
			ifs.read( reinterpret_cast<char*>(&dAlightnet),			sizeof(double));
			ifs.read( reinterpret_cast<char*>(&dAlightTransfer),    sizeof(double));
			ifs.read( reinterpret_cast<char*>(&dAlighterTotal),     sizeof(double));
			ifs.read( reinterpret_cast<char*>(&dPassenger),			sizeof(double));
			
            // 모형 시퀀스-노드기준, Imtas->링크기준
            // 모형 시퀀스-1 값이 Imtas 시퀀스 : 
            //     모형의 첫번째 시퀀스 값은 두번째 시퀀스 값과 합산되어
            //     Imtas 링크값에 저장하도록 수정(nombara..2013/2/16)
            
            //정수호 2013_04_06
            oTransitLine.TnxLineID			= nxLineID;
            oTransitLine.TnSeq				= nSeq -1;	
            oTransitLine.TdBoardNet			= dBoardnet;
            oTransitLine.TdBoardTransfer    = dBoardTransfer;
            oTransitLine.TdBoardTotal		= dBoardTotal;
            oTransitLine.TdAllightNet		= dAlightnet;
            oTransitLine.TdAllightTransfer	= dAlightTransfer;
            oTransitLine.TdAllightTotal		= dAlighterTotal;
            oTransitLine.TdPassenger		= dPassenger;

            mapModelTransitLine.insert(std::make_pair(KODKey(nxLineID, nSeq-1), oTransitLine));
            vecModelTransitLine.push_back(oTransitLine);            
		}

        std::map<KODKey, TMATransitLine>::iterator iter = mapModelTransitLine.begin();
        std::map<KODKey, TMATransitLine>::iterator end  = mapModelTransitLine.end();

        size_t nxCountModel = vecModelTransitLine.size();
        for (size_t i=0; i<nxCountModel; i++)
        {
            TMATransitLine& oReadData  = vecModelTransitLine[i];

            // 0번 시퀀스는 필터링
            if (oReadData.TnSeq > 0)
            {
                iter = mapModelTransitLine.find(KODKey(oReadData.TnxLineID, oReadData.TnSeq-1));
                if (iter != end)
                {
                    TMATransitLine& oPreData = iter->second;

                    if (oReadData.TnSeq == 1)
                    {
                        oReadData.TdBoardNet      = oPreData.TdBoardNet      + oPreData.TdPassenger;
                        oReadData.TdBoardTransfer = oPreData.TdBoardTransfer + 0.0; //oPreData.TdPassenger
                        oReadData.TdBoardTotal    = oPreData.TdBoardTotal    + oPreData.TdPassenger;
                    }
                    else
                    {
                        oReadData.TdBoardNet      = oPreData.TdBoardNet;
                        oReadData.TdBoardTransfer = oPreData.TdBoardTransfer;
                        oReadData.TdBoardTotal    = oPreData.TdBoardTotal;
                    }

                    vecTransitLine.push_back(oReadData);
                }
            }
        }

		CString strPreparedQuery (_T(""));

		CString strColumn1 = rLineColumnCollection.GetColumn(0)->Name();
		CString strColumn2 = rLineColumnCollection.GetColumn(1)->Name();
		CString strColumn3 = rLineColumnCollection.GetColumn(2)->Name();
		CString strColumn4 = rLineColumnCollection.GetColumn(3)->Name();
		CString strColumn5 = rLineColumnCollection.GetColumn(4)->Name();
		CString strColumn6 = rLineColumnCollection.GetColumn(5)->Name();
		CString strColumn7 = rLineColumnCollection.GetColumn(6)->Name();

		strPreparedQuery.Format(
			_T(" Update %s set  ")
			_T(" %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?  ")
			_T(" Where Transit_ID = ? and Seq = ? "), TABLE_TRANSITLINKS, rLineColumnCollection.GetColumn(0)->Name(), //★ 확인
			rLineColumnCollection.GetColumn(1)->Name(), rLineColumnCollection.GetColumn(2)->Name(),
			rLineColumnCollection.GetColumn(3)->Name(), rLineColumnCollection.GetColumn(4)->Name(),
			rLineColumnCollection.GetColumn(5)->Name(), rLineColumnCollection.GetColumn(6)->Name());

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strPreparedQuery);

		int nCount = vecTransitLine.size();
		for (int i =0; i < nCount; i++)
		{
			TMATransitLine oTransitLine = vecTransitLine[i];
			spPrepareStatement->BindDouble(1, oTransitLine.TdBoardNet       );
			spPrepareStatement->BindDouble(2, oTransitLine.TdBoardTransfer  );
			spPrepareStatement->BindDouble(3, oTransitLine.TdBoardTotal     );
			spPrepareStatement->BindDouble(4, oTransitLine.TdAllightNet     );
			spPrepareStatement->BindDouble(5, oTransitLine.TdAllightTransfer);
			spPrepareStatement->BindDouble(6, oTransitLine.TdAllightTotal   );
			spPrepareStatement->BindDouble(7, oTransitLine.TdPassenger      );
			spPrepareStatement->BindInt64 (8, oTransitLine.TnxLineID        );
			spPrepareStatement->BindInt   (9, oTransitLine.TnSeq            );

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
	TxLogDebugEndMsg();
}
*/

/*
void KMultiModalMainDlg::UpdateLineFile2DB_201610_28( KDBaseConPtr spDBaseConnection, KIOColumns& oTransitLineColumns )
{
    //★ KInterModalMainDlg::UpdateLineFile2DB 함수와 동일
    KIOTables* pIOTables  = m_pTarget->Tables();
    KIOTable*  pLineTable = pIOTables->FindTable(TABLE_TRANSITLINKS); //★ 확인  
    int nColumnCount = oTransitLineColumns.ColumnCount();

    if (nColumnCount == 0) {
        ThrowException(_T("적용 컬럼 정보가 없습니다."));
    }

    CString strFilePath = ConvertSavePath(_T("line_out.dat"));
    std::ifstream ifs(strFilePath, std::ios::binary );    
    if( ! ifs ) {
        ThrowException(_T("File Not Found"));
    }

    //★ 변수 초기화
    __int64 nxLineID(0);
    int		nSeq(0);
    double  dBoardnet(0), dBoardTransfer(0), dBoardTotal(0), dAlightnet(0), dAlightTransfer(0), dAlighterTotal(0), dPassenger(0);

    class TTemp {
    public:
        TTemp() {
            TSeq = -1;
            TBoard[0] = 0;
            TBoard[1] = 0;
            TBoard[2] = 0;

            TAlight[0] = 0;
            TAlight[1] = 0;
            TAlight[2] = 0;

            TPassenger = 0;
        }
    public:
        int    TSeq;
        double TBoard [3];
        double TAlight[3];
        double TPassenger;
    };

    //★ 모형 결과를 일차적으로 담는다.
    std::map<__int64, std::vector<TTemp>> mapData;

    while (!ifs.eof()) {			
        ifs.read( reinterpret_cast<char*>(&nxLineID), sizeof(__int64));
        if(ifs.eof())
            break;

        //★ Sequence
        ifs.read( reinterpret_cast<char*>(&nSeq),			sizeof(int));					

        //★ Board
        ifs.read( reinterpret_cast<char*>(&dBoardnet),		sizeof(double));
        ifs.read( reinterpret_cast<char*>(&dBoardTransfer),	sizeof(double));
        ifs.read( reinterpret_cast<char*>(&dBoardTotal),	sizeof(double));

        //★ Alight
        ifs.read( reinterpret_cast<char*>(&dAlightnet),		sizeof(double));
        ifs.read( reinterpret_cast<char*>(&dAlightTransfer),sizeof(double));
        ifs.read( reinterpret_cast<char*>(&dAlighterTotal), sizeof(double));

        //★ 재차인원
        ifs.read( reinterpret_cast<char*>(&dPassenger),		sizeof(double));

        TTemp oData; {
            oData.TSeq       = nSeq;

            oData.TBoard[0]  = dBoardnet;
            oData.TBoard[1]  = dBoardTransfer;
            oData.TBoard[2]  = dBoardTotal;

            oData.TAlight[0] = dAlightnet;
            oData.TAlight[1] = dAlightTransfer;
            oData.TAlight[2] = dAlighterTotal;

            oData.TPassenger = dPassenger;
        }

        std::vector<TTemp>& vecData = mapData[nxLineID];
        vecData.push_back(oData);
    }

    std::map<__int64, std::vector<TTemp>> mapResult;
    for (auto iter = mapData.begin(); iter != mapData.end(); ++iter) {
        __int64 nxLineID = iter->first;
        std::vector<TTemp>& vecData = iter->second;
        size_t nxCnt = vecData.size();
        if (nxCnt < 2)
            continue;

        std::vector<TTemp>& vecResult = mapResult[nxLineID];

        //★ 0번 부터 시작
        for (size_t i=0; i<nxCnt-1; i++) {
            TTemp& oCurInfo = vecData[i];
            TTemp& oNxtInfo = vecData[i+1];

            TTemp oData; {
                oData.TSeq       = oCurInfo.TSeq;

                oData.TBoard[0]  = oCurInfo.TBoard[0];
                oData.TBoard[1]  = oCurInfo.TBoard[1];
                oData.TBoard[2]  = oCurInfo.TBoard[2];

                oData.TAlight[0] = oNxtInfo.TAlight[0];
                oData.TAlight[1] = oNxtInfo.TAlight[1];
                oData.TAlight[2] = oNxtInfo.TAlight[2];

                oData.TPassenger = oNxtInfo.TPassenger;
            }

            vecResult.push_back(oData);
        }
    }

    CString strSQL (_T("")); {
        CString strColumn1 = oTransitLineColumns.GetColumn(0)->Name();
        CString strColumn2 = oTransitLineColumns.GetColumn(1)->Name();
        CString strColumn3 = oTransitLineColumns.GetColumn(2)->Name();
        CString strColumn4 = oTransitLineColumns.GetColumn(3)->Name();
        CString strColumn5 = oTransitLineColumns.GetColumn(4)->Name();
        CString strColumn6 = oTransitLineColumns.GetColumn(5)->Name();
        CString strColumn7 = oTransitLineColumns.GetColumn(6)->Name();

        strSQL.Format(
            _T(" Update %s set  ")
            _T(" %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?  ")
            _T(" Where Transit_ID = ? and Seq = ? "), TABLE_TRANSITLINKS, oTransitLineColumns.GetColumn(0)->Name(), //★ 확인
            oTransitLineColumns.GetColumn(1)->Name(), oTransitLineColumns.GetColumn(2)->Name(),
            oTransitLineColumns.GetColumn(3)->Name(), oTransitLineColumns.GetColumn(4)->Name(),
            oTransitLineColumns.GetColumn(5)->Name(), oTransitLineColumns.GetColumn(6)->Name());
    }

    KPreparedStatementPtr spPrepare = spDBaseConnection->PrepareStatement(strSQL);

    for (auto iter = mapResult.begin(); iter != mapResult.end(); ++iter) {
        __int64 nxLineID = iter->first;
        std::vector<TTemp>& vecData = iter->second;
        for (size_t i=0; i<vecData.size(); i++) {
            TTemp& oInfo = vecData[i];

            spPrepare->BindDouble(1, oInfo.TBoard[0]);
            spPrepare->BindDouble(2, oInfo.TBoard[1]);
            spPrepare->BindDouble(3, oInfo.TBoard[2]);
            spPrepare->BindDouble(4, oInfo.TAlight[0]);
            spPrepare->BindDouble(5, oInfo.TAlight[1]);
            spPrepare->BindDouble(6, oInfo.TAlight[2]);
            spPrepare->BindDouble(7, oInfo.TPassenger);
            spPrepare->BindInt64 (8, nxLineID);
            spPrepare->BindInt   (9, oInfo.TSeq);

            spPrepare->ExecuteUpdate();
            spPrepare->Reset();
        }        
    }
}
*/