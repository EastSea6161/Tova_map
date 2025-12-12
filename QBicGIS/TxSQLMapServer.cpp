#include "stdafx.h"
#include "TxSQLMapServer.h"
#include "TxMapEnumType.h"
#include "TxPoint.h"
#include "TxPolyline.h"
#include "TxPolygon.h"
#include "TxGridTileIndex.h"
#include "TxFeatureLayer.h"
#include "ThunderMapdrawProcess.h"
#include "TxConst.h"

std::vector<TxLayerInfoPtr> TxSQLMapServer::LayerInfo()
{
    // 임시...
    std::vector<TxLayerInfoPtr> vecLayerInfo;
    if (m_bConnected == false) 
        return vecLayerInfo;

    CString strSQL(_T(""));
    strSQL = _T(" Select LayerID, GeometryType, TableName, TxID_Field, Caption, GridSize, MinX, MinY, MaxX, MaxY From TxMapLayers ");
    KResultSetPtr spResultSet = m_spDBConnection->ExecuteQuery(strSQL);

    while(spResultSet->Next()) {
        int     nLayerID     = spResultSet->GetValueInt   (0);
        int     nGeoType     = spResultSet->GetValueInt   (1);
        CString strName      = spResultSet->GetValueString(2);
        CString strTxIDField = spResultSet->GetValueString(3);
        CString strCaption   = spResultSet->GetValueString(4);
        double  dGridSize    = spResultSet->GetValueDouble(5);
        double  dMinX        = spResultSet->GetValueDouble(6);
        double  dMinY        = spResultSet->GetValueDouble(7);
        double  dMaxX        = spResultSet->GetValueDouble(8);
        double  dMaxY        = spResultSet->GetValueDouble(9);

        TxLayerInfoSQLPtr spLayerInfo(new TxLayerInfoSQL((EnumTxGeoType)nGeoType, strName, strTxIDField, strCaption));
        spLayerInfo->SetGridSize(dGridSize);
        spLayerInfo->SetExtent(dMinX, dMinY, dMaxX, dMaxY);

        vecLayerInfo.push_back(spLayerInfo);
    }

    return vecLayerInfo;
}

int TxSQLMapServer::Connect()
{
    if (m_enMapServerType == TxMapDataServerSqlite) {
        m_spDBConnection = KDBaseConnectionPtr(new KSQLiteConnection(m_strConnectionString));
    }
    else{
        m_spDBConnection = KDBaseConnectionPtr(new KAdoConnection(m_strConnectionString));
    }

    if (nullptr == m_spDBConnection) {
        m_bConnected = false;
        return 0;
    }
	
	// TODO : 검색 필요(레이어 정보가 정상적으로 존재하는지, 서버구조가 맞는지)
    m_bConnected = true;
    return 1;
}

int TxSQLMapServer::Connect( KDBaseConnectionPtr spDBase )
{
    m_spDBConnection = spDBase;

    if (nullptr == m_spDBConnection) {
        m_bConnected = false;
        return 0;
    }

    m_bConnected = true;
    return 1;
}

void TxSQLMapServer::LoadServerData( TxLayerInfoPtr _spLayerInfo, TxEnvelope oEnvelope, std::map<__int64, ITxGeometryPtr>& mapGeometry)
{
    if (m_bConnected == false) 
        return;

    TxLayerInfoSQLPtr spLayerInfo = std::dynamic_pointer_cast<TxLayerInfoSQL>(_spLayerInfo);
    if (spLayerInfo == nullptr)
        return;
    try
    {
        CString  strLayerName = spLayerInfo->Name();

        TxGridTileIndex oGridTileIndex(spLayerInfo->GetGridSize());

        __int64 MinGx(0), MinGy(0);
        oGridTileIndex.GetGeoGridID(oEnvelope.GetMinX(), oEnvelope.GetMinY(), MinGx, MinGy);

        __int64 MaxGx(0), MaxGy(0);
        oGridTileIndex.GetGeoGridID(oEnvelope.GetMaxX(), oEnvelope.GetMaxY(), MaxGx, MaxGy);

        CString strSQL(_T(""));
        strSQL.Append(_T(" Select Tb.TxID, Tb.MinX, Tb.MinY, Tb.MaxX, Tb.MaxY, Tb.TxGeometry "));
        strSQL.Append(_T("     From "));
        strSQL.Append(_T("     ( "));
        strSQL.Append(_T("     Select TxID ")); 
        strSQL.AppendFormat(_T("     From TxIDX_%s "), strLayerName);
        strSQL.Append(_T("     Where "));
        strSQL.AppendFormat(_T("     (Gx Between %I64d and %I64d) And "), MinGx, MaxGx);
        strSQL.AppendFormat(_T("     (Gy Between %I64d and %I64d) "),     MinGy, MaxGy);
        strSQL.Append(_T("     Group By TxID "));
        strSQL.AppendFormat(_T("     ) Ta, TxShp_%s Tb "), strLayerName);
        strSQL.Append(_T("     Where Ta.TxID = Tb.TxID "));

        KResultSetPtr spResultSet = m_spDBConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {        
            __int64 nxID  = spResultSet->GetValueInt64 (0);
            double  dMinX = spResultSet->GetValueDouble(1);
            double  dMinY = spResultSet->GetValueDouble(2);
            double  dMaxX = spResultSet->GetValueDouble(3);
            double  dMaxY = spResultSet->GetValueDouble(4);

            TxEnvelope oGeoExtent(dMinX, dMinY, dMaxX, dMaxY);
            if ( oGeoExtent.Intersect(oEnvelope) == false )
                continue;

            int nSize(0);
            const void * pValue = spResultSet->GetValueBytes(5, nSize);

            QBicBinary oBinary(nSize+10);  
            oBinary.PushBack(pValue, (int)nSize);

            int nGeometryType;
            oBinary.Position(0);
            oBinary.Read(nGeometryType);

            if (nGeometryType == 1)
            {
                ITxGeometryPtr spGeometry(new TxPoint(oBinary, 0));
                mapGeometry.insert(std::make_pair(nxID, spGeometry));
            }
            else if (nGeometryType == 3)
            {
                ITxGeometryPtr spGeometry(new TxPolyline(oBinary, 0));
                if (spGeometry->Intersect(oEnvelope))
                    mapGeometry.insert(std::make_pair(nxID, spGeometry));
            }    
            else if (nGeometryType == 4)
            {
                ITxGeometryPtr spGeometry(new TxPolygon(oBinary, 0));
                if (spGeometry->Intersect(oEnvelope))
                    mapGeometry.insert(std::make_pair(nxID, spGeometry));
            }   
            else if (nGeometryType == 5)
            {
                ITxGeometryPtr spGeometry(new TxMPolygon(oBinary, 0));
                if (spGeometry->Intersect(oEnvelope))
                    mapGeometry.insert(std::make_pair(nxID, spGeometry));
            }   
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void TxSQLMapServer::LoadServerData( TxLayerInfoPtr _spLayerInfo, TxEnvelope oEnvelope, double dMinExtent, std::map<__int64, ITxGeometryPtr>& mapGeometry, ThunderMapdrawProcess* pDrawProcess)
{
    if (m_bConnected == false) 
        return;

    TxLayerInfoSQLPtr spLayerInfo = std::dynamic_pointer_cast<TxLayerInfoSQL>(_spLayerInfo);
    if (spLayerInfo == nullptr)
        return;

    try
    {
        CString  strLayerName = spLayerInfo->Name();
        TxGridTileIndex oGridTileIndex(spLayerInfo->GetGridSize());

        __int64 MinGx(0), MinGy(0);
        oGridTileIndex.GetGeoGridID(oEnvelope.GetMinX(), oEnvelope.GetMinY(), MinGx, MinGy);

        __int64 MaxGx(0), MaxGy(0);
        oGridTileIndex.GetGeoGridID(oEnvelope.GetMaxX(), oEnvelope.GetMaxY(), MaxGx, MaxGy);

        CString strSQL(_T(""));
        strSQL.Append(_T(" Select Tb.TxID, Tb.MinX, Tb.MinY, Tb.MaxX, Tb.MaxY, Tb.TxGeometry "));
        strSQL.Append(_T("     From "));
        strSQL.Append(_T("     ( "));
        strSQL.Append(_T("     Select TxID ")); 
        strSQL.AppendFormat(_T("     From TxIDX_%s "), strLayerName);
        strSQL.Append(_T("     Where "));
        strSQL.AppendFormat(_T("     (Gx Between %I64d and %I64d) And "), MinGx, MaxGx);
        strSQL.AppendFormat(_T("     (Gy Between %I64d and %I64d) "),     MinGy, MaxGy);
        strSQL.Append(_T("     Group By TxID "));
        strSQL.AppendFormat(_T("     ) Ta, TxShp_%s Tb "), strLayerName);
        strSQL.Append(_T("     Where Ta.TxID = Tb.TxID "));
        strSQL.AppendFormat(_T(" And Tb.Avg_Extent >= %f "), dMinExtent);

        KResultSetPtr spResultSet = m_spDBConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {        
            if (pDrawProcess->GetMapDrawStatus() == 0)
                break;

            __int64 nxID  = spResultSet->GetValueInt64 (0);
            double  dMinX = spResultSet->GetValueDouble(1);
            double  dMinY = spResultSet->GetValueDouble(2);
            double  dMaxX = spResultSet->GetValueDouble(3);
            double  dMaxY = spResultSet->GetValueDouble(4);

            TxEnvelope oGeoExtent(dMinX, dMinY, dMaxX, dMaxY);
            if ( oGeoExtent.Intersect(oEnvelope) == false )
                continue;

            int nSize(0);
            const void * pValue = spResultSet->GetValueBytes(5, nSize);

            QBicBinary oBinary(nSize+10);  
            oBinary.PushBack(pValue, (int)nSize);

            int nGeometryType;
            oBinary.Position(0);
            oBinary.Read(nGeometryType);

            if (nGeometryType == 1)
            {
                ITxGeometryPtr spGeometry(new TxPoint(oBinary, 0));
                mapGeometry.insert(std::make_pair(nxID, spGeometry));
            }
            else if (nGeometryType == 3)
            {
                ITxGeometryPtr spGeometry(new TxPolyline(oBinary, 0));
                mapGeometry.insert(std::make_pair(nxID, spGeometry));
            }    
            else if (nGeometryType == 4)
            {
                ITxGeometryPtr spGeometry(new TxPolygon(oBinary, 0));
                mapGeometry.insert(std::make_pair(nxID, spGeometry));
            }   
            else if (nGeometryType == 5)
            {
                ITxGeometryPtr spGeometry(new TxMPolygon(oBinary, 0));
                mapGeometry.insert(std::make_pair(nxID, spGeometry));
            }   
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

int TxSQLMapServer::LoadServerData( TxLayerInfoPtr spLayerInfo, std::map<__int64, ITxGeometryPtr>& mapGeometry)
{
    if (m_bConnected == false) 
        return 0;

    CString     strLayerName = spLayerInfo->Name();
    LoadGeometry(strLayerName, mapGeometry);

    return 1;
}


int TxSQLMapServer::LoadGeometry( CString strLayerName, std::map<__int64, ITxGeometryPtr>& mapGeometry)
{
    if (m_bConnected == false) 
        return 0;

    CString strSQL(_T(""));
    strSQL.Format(_T("Select TxID, TxGeometry From TxShp_%s"), strLayerName);

    KResultSetPtr spResultSet = m_spDBConnection->ExecuteQuery(strSQL);
    while (spResultSet->Next())
    {        
        __int64 nxID = spResultSet->GetValueInt64(0);

        int nSize(0);
        const void * pValue = spResultSet->GetValueBytes(1, nSize);

        QBicBinary oBinary(nSize+10);  
        oBinary.PushBack(pValue, (int)nSize);

        int nGeometryType;
        oBinary.Position(0);
        oBinary.Read(nGeometryType);

        if (nGeometryType == 1)
        {
            ITxGeometryPtr spGeometry(new TxPoint(oBinary, 0));
            mapGeometry.insert(std::make_pair(nxID, spGeometry));
        }
        else if (nGeometryType == 3)
        {
            ITxGeometryPtr spGeometry(new TxPolyline(oBinary, 0));
            mapGeometry.insert(std::make_pair(nxID, spGeometry));
        }    
        else if (nGeometryType == 4)
        {
            ITxGeometryPtr spGeometry(new TxPolygon(oBinary, 0));
            mapGeometry.insert(std::make_pair(nxID, spGeometry));
        }   
        else if (nGeometryType == 5)
        {
            ITxGeometryPtr spGeometry(new TxMPolygon(oBinary, 0));
            mapGeometry.insert(std::make_pair(nxID, spGeometry));
        }   
    }

    return 1;
}

ITxGeometryPtr TxSQLMapServer::LoadGeometry( TxLayerInfoPtr spLayerInfo, __int64 nxID )
{
    if (m_bConnected == false) 
        return nullptr;

    CString strLayerName = spLayerInfo->Name();

    CString strSQL(_T(""));
    strSQL.Format(_T(" Select TxID, TxGeometry From TxShp_%s Where TxID = %I64d "), strLayerName, nxID);

    ITxGeometryPtr spGeometry;
    KResultSetPtr spResultSet = m_spDBConnection->ExecuteQuery(strSQL);
    if (spResultSet->Next())
    {        
        __int64 nxID = spResultSet->GetValueInt64(0);

        int nSize(0);
        const void * pValue = spResultSet->GetValueBytes(1, nSize);

        QBicBinary oBinary(nSize+10);  
        oBinary.PushBack(pValue, (int)nSize);

        int nGeometryType;
        oBinary.Position(0);
        oBinary.Read(nGeometryType);

        if (nGeometryType == 1)
        {
            spGeometry = ITxGeometryPtr(new TxPoint(oBinary, 0));
        }
        else if (nGeometryType == 3)
        {
            spGeometry = ITxGeometryPtr(new TxPolyline(oBinary, 0));
        }    
        else if (nGeometryType == 4)
        {
            spGeometry = ITxGeometryPtr(new TxPolygon(oBinary, 0));
        }   
        else if (nGeometryType == 5)
        {
            spGeometry = ITxGeometryPtr(new TxMPolygon(oBinary, 0));
        }   
    }

    return spGeometry;
}

void TxSQLMapServer::BeginTransaction()
{
    if (m_bConnected == false) 
        return;
    
    m_spDBConnection->BeginTransaction();
}

void TxSQLMapServer::Commit()
{
    if (m_bConnected == false) 
        return;
    
    m_spDBConnection->Commit();
}

void TxSQLMapServer::Rollback()
{
    if (m_bConnected == false) 
        return;

    m_spDBConnection->RollBack();
}

void TxSQLMapServer::Insert( TxLayerInfoPtr _spLayerInfo, __int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord )
{
    if (m_bConnected == false) 
        throw 1;

    size_t nxItemCnt = oRecord.ItemCount();
    if (nxItemCnt == 0) 
        throw 2;

    if (spGeometry == nullptr)
        throw 3;

    TxLayerInfoSQLPtr spLayerInfo = std::dynamic_pointer_cast<TxLayerInfoSQL>(_spLayerInfo);
    if (spLayerInfo == nullptr)
        throw 4;
        
    bool bFindTxID(false);
    for (size_t i=0; i<nxItemCnt; i++) {
        TxItem* pItem = oRecord.GetItem(i);
        if (pItem == nullptr) 
            throw 5;

        if (_tcsicmp(pItem->ItemName(), spLayerInfo->TxIDField()) == 0) {
            boost::any oValue = pItem->Value();
            if ( boost::any_cast<__int64>(oValue) == nxID) {
                bFindTxID = true;
            }            
        }
    }

    if (bFindTxID == false)
        throw 6;

    CString  strLayerName = spLayerInfo->Name();

    CString strSqlTable(_T(""));
    // 속성 정보 : vecField    
    strSqlTable.Format(_T("Insert Into %s "), strLayerName);
    strSqlTable.Append(_T(" ( "));
    for (size_t i=0; i<oRecord.ItemCount(); i++) {
        TxItem* pItem = oRecord.GetItem(i);
        if (i==0) {
            strSqlTable.AppendFormat(_T(" %s "), pItem->ItemName());
        } else {
            strSqlTable.AppendFormat(_T(" ,%s "), pItem->ItemName());
        }
    }
    strSqlTable.Append(_T(" ) "));

    strSqlTable.Append(_T(" Values "));
    strSqlTable.Append(_T(" ( "));
	for (size_t i=0; i<oRecord.ItemCount(); i++) {
		if (i==0) {
			strSqlTable.Append(_T("?"));
		}
		else {
			strSqlTable.Append(_T(",?"));
		}
	}
	strSqlTable.Append(_T(" ) "));   

	KPreparedStatementPtr spPrepared = m_spDBConnection->PrepareStatement(strSqlTable);
    
    for (int i=0; i < (int)oRecord.ItemCount(); i++) {
        TxItem* pItem = oRecord.GetItem(i);
        boost::any oValue = pItem->Value();

		if (oValue.type() == typeid(int)) {
			spPrepared->BindInt(i+1, boost::any_cast<int>(oValue));
		}
		else if (oValue.type() == typeid(__int64)){
			spPrepared->BindInt64(i+1, boost::any_cast<__int64>(oValue));
		}
		else if (oValue.type() == typeid(float)) {
			spPrepared->BindDouble(i+1, (double)boost::any_cast<float>(oValue));
		}
		else if (oValue.type() == typeid(double)) {
			spPrepared->BindDouble(i+1, boost::any_cast<double>(oValue));
		}
		else if (oValue.type() == typeid(CString)) {
			CString strValue = boost::any_cast<CString>(oValue);
			strValue.Replace(_T("'"), _T("''"));
			spPrepared->BindText(i+1, strValue);
		}
		else {
			spPrepared->BindNull(i+1);
		}  
    }
    
	spPrepared->ExecuteUpdate();
	spPrepared->Reset();

    Insert(_spLayerInfo, nxID, spGeometry);
}

void TxSQLMapServer::Insert( TxLayerInfoPtr _spLayerInfo, __int64 nxID, ITxGeometryPtr spGeometry )
{
    // TxMapLayers - GridSize(읽기전용) / Update : (MinX, MinY, MaxX, MaxY)
    // TxShp_~     - TxID, Avg_Extent, MinX, MinY, MaxX, MaxY, TxGeometry, Version
    //             * TxID는 vecField 정보를 사용하지 않는다.
    //             * Version 필드는 입력할 수 없다.
    // TxIDX_~     - 
    
    if (m_bConnected == false) 
        throw 1;

    if (spGeometry == nullptr)
        throw 2;

    TxLayerInfoSQLPtr spLayerInfo = std::dynamic_pointer_cast<TxLayerInfoSQL>(_spLayerInfo);
    if (spLayerInfo == nullptr)
        throw 3;

    CString strLayerName   = spLayerInfo->Name();

    double dLayerMinX = spLayerInfo->GetMinX();
    double dLayerMinY = spLayerInfo->GetMinY();
    double dLayerMaxX = spLayerInfo->GetMaxX();
    double dLayerMaxY = spLayerInfo->GetMaxY();

    if (dLayerMinX == 0.0 && dLayerMinY == 0.0 && dLayerMaxX == 0.0 && dLayerMaxY == 0.0) {
        dLayerMinX = TxConst::MaxDbl();
        dLayerMinY = TxConst::MaxDbl();

        dLayerMaxX = TxConst::MinDbl();
        dLayerMaxY = TxConst::MinDbl();
    }

    TxGridTileIndex oGridTileIndex(spLayerInfo->GetGridSize());

    CString strSqlMapLayers(_T(""));
    CString strSqlShp(_T(""));
    CString strSqlIdx(_T(""));
    CString strSqlTable(_T(""));

    bool bMapLayersChanged(false);
    EnumTxGeoType enGeoType = spGeometry->GeometryType();    
    if (GeoPointType == enGeoType) 
    {
        TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);
        double dX = spPoint->X;
        double dY = spPoint->Y;

        if (dLayerMinX > dX) {
            dLayerMinX = dX;  bMapLayersChanged = true;
        }
        if (dLayerMinY > dY) {
            dLayerMinY = dY;  bMapLayersChanged = true;
        }
        if (dLayerMaxX < dX) {
            dLayerMaxX = dX;  bMapLayersChanged = true;
        }
        if (dLayerMaxY < dY) {
            dLayerMaxY = dY;  bMapLayersChanged = true;
        }
    }
    else  
    {
        TxEnvelope    oEnvelope = spGeometry->Envelope();

        if (dLayerMinX > oEnvelope.GetMinX()) {
            dLayerMinX = oEnvelope.GetMinX();  bMapLayersChanged = true;
        }
        if (dLayerMinY > oEnvelope.GetMinY()) {
            dLayerMinY = oEnvelope.GetMinY();  bMapLayersChanged = true;
        }
        if (dLayerMaxX < oEnvelope.GetMaxX()) {
            dLayerMaxX = oEnvelope.GetMaxX();  bMapLayersChanged = true;
        }
        if (dLayerMaxY < oEnvelope.GetMaxY()) {
            dLayerMaxY = oEnvelope.GetMaxY();  bMapLayersChanged = true;
        }
    }

    TxEnvelope oEnvelope = spGeometry->Envelope();

    // TxShp_~
    strSqlShp.Format(_T("Insert Into TxShp_%s(TxID, Avg_Extent, MinX, MinY, MaxX, MaxY, TxGeometry, Version) VALUES (?, ?, ?, ?, ?, ?, ?, 0 )"), strLayerName);
    {
        KPreparedStatementPtr spPreparedStatement = m_spDBConnection->PrepareStatement(strSqlShp);

        QBicBinary oBinary(100);
        spGeometry->GetBinary(oBinary);

        double dAvgExtent = ( oEnvelope.GetWidth() + oEnvelope.GetHeight() ) * 0.5;
        if (dAvgExtent <= 0.0) { // 포인트 타입은 0.0이므로
            dAvgExtent = 999999999999.0;
        }

        spPreparedStatement->BindInt64 (1, nxID);
        spPreparedStatement->BindDouble(2, dAvgExtent);
        spPreparedStatement->BindDouble(3, oEnvelope.GetMinX());
        spPreparedStatement->BindDouble(4, oEnvelope.GetMinY());
        spPreparedStatement->BindDouble(5, oEnvelope.GetMaxX());
        spPreparedStatement->BindDouble(6, oEnvelope.GetMaxY());
        spPreparedStatement->BindBlob  (7, oBinary, (int)oBinary.Size());

        int nR = spPreparedStatement->ExecuteUpdate(); // 101, 0
        spPreparedStatement->Reset();	

        if (!(nR == 101 || nR == 0)){
            return; //SQLite(101), ADO(0)
        }
    }   

    // TxIDX_~    
    std::set<TxGridID> setGrid;    
    if (GeoPointType == enGeoType) {
        __int64 Gx(0), Gy(0);
        oGridTileIndex.GetGeoGridID(oEnvelope.GetMinX(), oEnvelope.GetMinY(), Gx, Gy);
        setGrid.insert(TxGridID(Gx, Gy));
    }
    else if (GeoPolylineType == enGeoType) {
        TxPolylinePtr spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);

        std::vector<TxPoint> vecPt = spPLine->ExtractVertex(spLayerInfo->GetGridSize() * 0.1);
        for (size_t i=0; i<vecPt.size(); i++) {
            TxPoint& oPt = vecPt[i];

            __int64 Gx(0), Gy(0);
            oGridTileIndex.GetGeoGridID(oPt.X, oPt.Y, Gx, Gy);
            setGrid.insert(TxGridID(Gx, Gy));
        }
    }
    else if (GeoPolygonType == enGeoType) {
        setGrid = oGridTileIndex.GetGeoGridID(oEnvelope.GetMinX(), oEnvelope.GetMinY(), oEnvelope.GetMaxX(), oEnvelope.GetMaxY());
    }
    else if (GeoMPolygonType == enGeoType) {
        TxMPolygonPtr spMPoly = std::dynamic_pointer_cast<TxMPolygon>(spGeometry);
        size_t nxCount = spMPoly->SubPolygonCount();
        for (size_t i=0; i<nxCount; i++) {
            TxEnvelope oSubEnvelope = spMPoly->SubPolygon(i)->Envelope();
            std::set<TxGridID> setSubGrid = oGridTileIndex.GetGeoGridID(oSubEnvelope.GetMinX(), oSubEnvelope.GetMinY(), oSubEnvelope.GetMaxX(), oSubEnvelope.GetMaxY());

            for (auto iter = setSubGrid.begin(); iter != setSubGrid.end(); ++iter) {
                setGrid.insert(*iter);
            }
        }
    }    
    else {
        setGrid = oGridTileIndex.GetGeoGridID(oEnvelope.GetMinX(), oEnvelope.GetMinY(), oEnvelope.GetMaxX(), oEnvelope.GetMaxY());
    }

    strSqlIdx.Format(_T("Insert Into TxIDX_%s(Gx, GY, TxID) Values (?, ?, %I64d)"), strLayerName, nxID);
    KPreparedStatementPtr spPreparedStatement = m_spDBConnection->PrepareStatement(strSqlIdx);
    for (auto iter = setGrid.begin(); iter != setGrid.end(); ++iter){
        TxGridID oGrid = *iter;
        spPreparedStatement->BindInt64(1, oGrid.Gx);
        spPreparedStatement->BindInt64(2, oGrid.Gy);

        spPreparedStatement->ExecuteUpdate();
        spPreparedStatement->Reset();	
    }	
    
    if (bMapLayersChanged == true) {
        strSqlMapLayers.Format(_T("Update TxMapLayers Set MinX = %f, MinY = %f, MaxX = %f, MaxY = %f Where TableName = '%s' "), dLayerMinX, dLayerMinY, dLayerMaxX, dLayerMaxY, strLayerName);
        m_spDBConnection->ExecuteUpdate(strSqlMapLayers);
        spLayerInfo->SetExtent(dLayerMinX, dLayerMinY, dLayerMaxX, dLayerMaxY);
    }
}

void TxSQLMapServer::Delete( TxLayerInfoPtr _spLayerInfo, __int64 nxID )
{
    if (m_bConnected == false) 
        throw 1;

    TxLayerInfoSQLPtr spLayerInfo = std::dynamic_pointer_cast<TxLayerInfoSQL>(_spLayerInfo);
    if (spLayerInfo == nullptr)
        throw 1;

    CString strLayerName = spLayerInfo->Name();
    CString strTxIDField = spLayerInfo->TxIDField();

    CString strSqlShp(_T(""));
    CString strSqlIdx(_T(""));
    CString strSqlTable(_T(""));

    strSqlShp.Format  (_T(" Delete From TxShp_%s Where TxID = %I64d"), strLayerName, nxID);
    strSqlIdx.Format  (_T(" Delete From TxIdx_%s Where TxID = %I64d"), strLayerName, nxID);
    strSqlTable.Format(_T(" Delete From %s Where %s = %I64d"),         strLayerName, strTxIDField, nxID);

    m_spDBConnection->ExecuteUpdate(strSqlShp);
    m_spDBConnection->ExecuteUpdate(strSqlIdx);
    m_spDBConnection->ExecuteUpdate(strSqlTable);
}


void TxSQLMapServer::RemoveAll( TxLayerInfoPtr _spLayerInfo )
{
    if (m_bConnected == false) 
        throw 1;

    TxLayerInfoSQLPtr spLayerInfo = std::dynamic_pointer_cast<TxLayerInfoSQL>(_spLayerInfo);
    if (spLayerInfo == nullptr)
        throw 1;

    CString strLayerName = spLayerInfo->Name();
    CString strTxIDField = spLayerInfo->TxIDField();

    CString strSqlShp(_T(""));
    CString strSqlIdx(_T(""));
    CString strSqlTable(_T(""));

    strSqlShp.Format  (_T(" Delete From TxShp_%s "), strLayerName);
    strSqlIdx.Format  (_T(" Delete From TxIdx_%s "), strLayerName);
    strSqlTable.Format(_T(" Delete From %s "),       strLayerName);

    m_spDBConnection->ExecuteUpdate(strSqlShp);
    m_spDBConnection->ExecuteUpdate(strSqlIdx);
    m_spDBConnection->ExecuteUpdate(strSqlTable);
}


void TxSQLMapServer::Update( TxLayerInfoPtr _spLayerInfo, __int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord )
{
    if (m_bConnected == false) 
        throw 1;;

    if (oRecord.ItemCount() == 0)
        throw 1;;

    if (spGeometry == nullptr)
        throw 1;;

    Update(_spLayerInfo, nxID, oRecord);
    Update(_spLayerInfo, nxID, spGeometry);
}

void TxSQLMapServer::Update( TxLayerInfoPtr _spLayerInfo, __int64 nxID, ITxGeometryPtr spGeometry )
{
    if (m_bConnected == false) 
        throw 1;;

    if (spGeometry == nullptr)
        throw 1;;

    TxLayerInfoSQLPtr spLayerInfo = std::dynamic_pointer_cast<TxLayerInfoSQL>(_spLayerInfo);
    if (spLayerInfo == nullptr)
        throw 1;;

    CString strLayerName = spLayerInfo->Name();
    CString strTxIDField = spLayerInfo->TxIDField();

    CString strSqlMapLayers(_T(""));
    CString strSqlShp(_T(""));
    CString strSqlIdx(_T(""));

    double dLayerMinX = spLayerInfo->GetMinX();
    double dLayerMinY = spLayerInfo->GetMinY();
    double dLayerMaxX = spLayerInfo->GetMaxX();
    double dLayerMaxY = spLayerInfo->GetMaxY();

    TxGridTileIndex oGridTileIndex(spLayerInfo->GetGridSize());

    bool bMapLayersChanged(false);
    EnumTxGeoType enGeoType = spGeometry->GeometryType();    
    if (GeoPointType == enGeoType) 
    {
        TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);
        double dX = spPoint->X;
        double dY = spPoint->Y;

        if (dLayerMinX > dX) {
            dLayerMinX = dX;  bMapLayersChanged = true;
        }
        if (dLayerMinY > dY) {
            dLayerMinY = dY;  bMapLayersChanged = true;
        }
        if (dLayerMaxX < dX) {
            dLayerMaxX = dX;  bMapLayersChanged = true;
        }
        if (dLayerMaxY < dY) {
            dLayerMaxY = dY;  bMapLayersChanged = true;
        }
    }
    else  
    {
        TxEnvelope    oEnvelope = spGeometry->Envelope();

        if (dLayerMinX > oEnvelope.GetMinX()) {
            dLayerMinX = oEnvelope.GetMinX();  bMapLayersChanged = true;
        }
        if (dLayerMinY > oEnvelope.GetMinY()) {
            dLayerMinY = oEnvelope.GetMinY();  bMapLayersChanged = true;
        }
        if (dLayerMaxX < oEnvelope.GetMaxX()) {
            dLayerMaxX = oEnvelope.GetMaxX();  bMapLayersChanged = true;
        }
        if (dLayerMaxY < oEnvelope.GetMaxY()) {
            dLayerMaxY = oEnvelope.GetMaxY();  bMapLayersChanged = true;
        }
    }

    // TxMapLayers
    if (bMapLayersChanged == true) {
        strSqlMapLayers.Format(_T("Update TxMapLayers Set MinX = %f, MinY = %f, MaxX = %f, MaxY = %f Where TableName = '%s' "), dLayerMinX, dLayerMinY, dLayerMaxX, dLayerMaxY, strLayerName);
        m_spDBConnection->ExecuteUpdate(strSqlMapLayers);
    }      

    // TxIDX_~
    TxEnvelope oEnvelope = spGeometry->Envelope();
    std::set<TxGridID> setGrid;  

    if (GeoPointType == enGeoType) {
        __int64 Gx(0), Gy(0);
        oGridTileIndex.GetGeoGridID(oEnvelope.GetMinX(), oEnvelope.GetMinY(), Gx, Gy);
        setGrid.insert(TxGridID(Gx, Gy));
    }
    else if (GeoPolylineType == enGeoType) {
        TxPolylinePtr spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        
        std::vector<TxPoint> vecPt = spPLine->ExtractVertex(spLayerInfo->GetGridSize() * 0.1);
        for (size_t i=0; i<vecPt.size(); i++) {
            TxPoint& oPt = vecPt[i];

            __int64 Gx(0), Gy(0);
            oGridTileIndex.GetGeoGridID(oPt.X, oPt.Y, Gx, Gy);
            setGrid.insert(TxGridID(Gx, Gy));
        }
    }
    else if (GeoPolygonType == enGeoType) {
        setGrid = oGridTileIndex.GetGeoGridID(oEnvelope.GetMinX(), oEnvelope.GetMinY(), oEnvelope.GetMaxX(), oEnvelope.GetMaxY());
    }
    else if (GeoMPolygonType == enGeoType) {
        TxMPolygonPtr spMPoly = std::dynamic_pointer_cast<TxMPolygon>(spGeometry);
        size_t nxCount = spMPoly->SubPolygonCount();
        for (size_t i=0; i<nxCount; i++) {
            TxEnvelope oSubEnvelope = spMPoly->SubPolygon(i)->Envelope();
            std::set<TxGridID> setSubGrid = oGridTileIndex.GetGeoGridID(oSubEnvelope.GetMinX(), oSubEnvelope.GetMinY(), oSubEnvelope.GetMaxX(), oSubEnvelope.GetMaxY());

            for (auto iter = setSubGrid.begin(); iter != setSubGrid.end(); ++iter) {
                setGrid.insert(*iter);
            }
        }
    }    
    else {
        setGrid = oGridTileIndex.GetGeoGridID(oEnvelope.GetMinX(), oEnvelope.GetMinY(), oEnvelope.GetMaxX(), oEnvelope.GetMaxY());
    }

    strSqlIdx.Format(_T("Delete From TxIDX_%s Where TxID = %I64d"), strLayerName, nxID);
    m_spDBConnection->ExecuteUpdate(strSqlIdx);

    strSqlIdx.Format(_T("Insert Into TxIDX_%s(Gx, GY, TxID) Values (?, ?, %I64d)"), strLayerName, nxID);
    KPreparedStatementPtr spPreparedStatement = m_spDBConnection->PrepareStatement(strSqlIdx);
    for (auto iter = setGrid.begin(); iter != setGrid.end(); ++iter){
        TxGridID oGrid = *iter;
        spPreparedStatement->BindInt64(1, oGrid.Gx);
        spPreparedStatement->BindInt64(2, oGrid.Gy);

        spPreparedStatement->ExecuteUpdate();
        spPreparedStatement->Reset();	
    }

    // TxShp_~
    strSqlShp.Format(_T(" Update TxShp_%s Set Avg_Extent = ?, MinX = ?, MinY = ?, MaxX = ?, MaxY = ?, TxGeometry = ?, "), strLayerName);
	strSqlShp.AppendFormat(_T("Version =  (Select Max(Version)+1 From TxShp_%s ) "), strLayerName);
    strSqlShp.AppendFormat(_T(" Where TxID = %I64d "), nxID);
    spPreparedStatement = m_spDBConnection->PrepareStatement(strSqlShp);

    QBicBinary oBinary(100);
    spGeometry->GetBinary(oBinary);

    double dAvgExtent = ( oEnvelope.GetWidth() + oEnvelope.GetHeight() ) * 0.5;   
    if (dAvgExtent <= 0.0) { // 포인트 타입은 0.0이므로
        dAvgExtent = 999999999999.0;
    }

    spPreparedStatement->BindDouble(1, dAvgExtent);
    spPreparedStatement->BindDouble(2, oEnvelope.GetMinX());
    spPreparedStatement->BindDouble(3, oEnvelope.GetMinY());
    spPreparedStatement->BindDouble(4, oEnvelope.GetMaxX());
    spPreparedStatement->BindDouble(5, oEnvelope.GetMaxY());
    spPreparedStatement->BindBlob(6, oBinary, (int)oBinary.Size());
    spPreparedStatement->ExecuteUpdate();
    spPreparedStatement->Reset();	

    if (bMapLayersChanged == true) {
        spLayerInfo->SetExtent(dLayerMinX, dLayerMinY, dLayerMaxX, dLayerMaxY);
    }
}

void TxSQLMapServer::Update( TxLayerInfoPtr _spLayerInfo, __int64 nxID, TxRecord& oRecord )
{
    if (m_bConnected == false) 
        throw 1;

    if (oRecord.ItemCount() == 0)
        throw 1;
    
    TxLayerInfoSQLPtr spLayerInfo = std::dynamic_pointer_cast<TxLayerInfoSQL>(_spLayerInfo);
    if (spLayerInfo == nullptr)
        throw 1;

    CString strLayerName = spLayerInfo->Name();

    CString strSqlTable(_T(""));
    // 속성 정보 : vecField    
    strSqlTable.Format(_T("Update %s "), strLayerName);
    for (size_t i=0; i<oRecord.ItemCount(); i++) {
        TxItem* pItem = oRecord.GetItem(i);
        if (i==0) {
            strSqlTable.AppendFormat(_T(" Set %s = ? "), pItem->ItemName());
        } else {
            strSqlTable.AppendFormat(_T(" , %s = ?"), pItem->ItemName());
        }
    }
    strSqlTable.AppendFormat(_T(" Where %s = %I64d"), spLayerInfo->TxIDField(), nxID);

	KPreparedStatementPtr spPrepared = m_spDBConnection->PrepareStatement(strSqlTable);

	for (int i=0; i < (int)oRecord.ItemCount(); i++) {
		TxItem* pItem = oRecord.GetItem(i);

		boost::any oValue = pItem->Value();

		if (oValue.type() == typeid(int)) {
			spPrepared->BindInt(i+1, boost::any_cast<int>(oValue));
		}
		else if (oValue.type() == typeid(__int64)){
			spPrepared->BindInt64(i+1, boost::any_cast<__int64>(oValue));
		}
		else if (oValue.type() == typeid(float)) {
			spPrepared->BindDouble(i+1, (double)boost::any_cast<float>(oValue));
		}
		else if (oValue.type() == typeid(double)) {
			spPrepared->BindDouble(i+1, boost::any_cast<double>(oValue));
		}
		else if (oValue.type() == typeid(CString)) {
			CString strValue = boost::any_cast<CString>(oValue);
			strValue.Replace(_T("'"), _T("''"));
			spPrepared->BindText(i+1, strValue);
		}
		else {
			spPrepared->BindNull(i+1);
		}            
	}

	spPrepared->ExecuteUpdate();
	spPrepared->Reset();
}

void TxSQLMapServer::RemoveLayer( CString strLayerName )
{
    if (m_bConnected == false) 
        return;

    CString strSQL(_T(""));
    strSQL.Format(_T(" Drop Table TxShp_%s "), strLayerName);
    m_spDBConnection->ExecuteUpdate(strSQL);
    
    strSQL.Format(_T(" Drop Table TxIdx_%s "), strLayerName);
    m_spDBConnection->ExecuteUpdate(strSQL);
    
    strSQL.Format(_T(" Drop Table %s ")      , strLayerName);
    m_spDBConnection->ExecuteUpdate(strSQL);

    strSQL.Format(_T(" Delete From TxMapLayers Where Lower(TableName) = Lower('%s') "), strLayerName);
    m_spDBConnection->ExecuteUpdate(strSQL);

    strSQL.Format(_T(" Delete From TxMapTableColumns Where Lower(Table_Name) = Lower('%s') "), strLayerName);
    m_spDBConnection->ExecuteUpdate(strSQL);
}

TxLayerInfoPtr TxSQLMapServer::AddLayer( EnumTxGeoType enGeoType, CString strLayerName, CString strCaption, CString strTxIDField, TxColumns& oColumns)
{
    TxLayerInfoPtr spLayerInfo = nullptr;
    if (m_bConnected == false) 
        return spLayerInfo;
    
    if (ServerType() == TxMapDataServerSqlite) {
        spLayerInfo = AddLayerSqlite(enGeoType, strLayerName, strCaption, strTxIDField, oColumns);
    } else if (ServerType() == TxMapDataServerSQL) {
        spLayerInfo = AddLayerMSSql(enGeoType, strLayerName, strCaption, strTxIDField, oColumns);
    }

    return spLayerInfo;
}

TxLayerInfoPtr TxSQLMapServer::AddLayerSqlite( EnumTxGeoType enGeoType, CString strLayerName, CString strCaption, CString strTxIDField, TxColumns& oColumns )
{    
    // TxMapLayers, TxShp_~, TxIDX_~
    double dGridSize(1000);    
    if (enGeoType == GeoShapeType) {
        dGridSize = 1000.0;
    } 
    else if (enGeoType == GeoPointType)   {
        dGridSize = 100.0; //★ 포인트는 그리드 갯수와 Shape의 갯수가 동일하다. //1.0 에서 100으로 변경
    } 
    else if (enGeoType == GeoPolylineType || enGeoType == GeoMPolylineType){
        dGridSize = 1000.0;
    } 
    else if (enGeoType == GeoPolygonType  || enGeoType == GeoMPolygonType) {
        dGridSize = 5000.0;
    }
    else if (enGeoType == GeoMPointType)   {
        dGridSize = 100.0; //★ 포인트는 그리드 갯수와 Shape의 갯수가 동일하다.
    }
    else {
        throw 1;
    } 

    // 1.0 : TxMapLayers
    CString strSQL(_T(""));
    strSQL.Append(_T(" Insert Into TxMapLayers "));
    strSQL.Append(_T(" ( "));
    strSQL.Append(_T("   LayerID, GeometryType, TableName, TxID_Field, Caption, GridSize, MinX, MinY, MaxX, MaxY "));
    strSQL.Append(_T(" ) "));
    strSQL.AppendFormat(_T(" Select Max(LayerID)+1, %d, '%s', '%s', '%s', %f, 0.0, 0.0, 0.0, 0.0 From TxMapLayers "), (int)enGeoType, strLayerName, strTxIDField, strCaption, dGridSize);

    try {
        m_spDBConnection->ExecuteUpdate(strSQL);
    } catch (...) {
        TxLogDebugException();
        throw 1;
    }

    // 2.0 : TxShp_~
    strSQL.Format(_T(" Create Table TxShp_%s "), strLayerName);
    strSQL.Append(_T(" ( "));
    strSQL.Append(_T(" [TxID] INTEGER PRIMARY KEY, [Avg_Extent] DOUBLE, [MinX] DOUBLE, [MinY] DOUBLE, [MaxX] DOUBLE, [MaxY] DOUBLE, [TxGeometry] BLOB, [Version] INTEGER "));
    strSQL.Append(_T(" ) "));

    try {
        m_spDBConnection->ExecuteUpdate(strSQL);
    } catch (...) {
        TxLogDebugException();
        throw 2;
    }

    strSQL.Format(_T(" CREATE INDEX [TxShp_%s_VersionIDX] ON [TxShp_%s] ([Version]) "), strLayerName, strLayerName);

    try {
        m_spDBConnection->ExecuteUpdate(strSQL);
    } catch (...) {
        TxLogDebugException();
        throw 2;
    }

    // 3.0 : TxIDX_~
    strSQL.Format(_T(" CREATE TABLE [TxIDX_%s] ([TxID] INTEGER, [Gx] INTEGER, [Gy] INTEGER, PRIMARY KEY([TxID], [Gx], [Gy])) "), strLayerName);

    try {
        m_spDBConnection->ExecuteUpdate(strSQL);
    } catch (...) {
        TxLogDebugException();
        throw 3;
    }

    // 4.0 BusinessTable
    CString strColumns(_T(""));
    CString strPrimary(_T(""));

    size_t nxCount = oColumns.ColumnCount();
    for (size_t i=0; i<nxCount; i++) {
        TxColumn*  pColumn       = oColumns.GetColumn(i);
        CString    strColumnName = pColumn->Name();
        TxDataType enType        = pColumn->ColumnType();

        if ( strColumns.GetLength() != 0 ) {
            strColumns.Append(_T(","));
        }
        if (enType == EnInt32) {
            if (pColumn->IsPrimaryKey())
                strColumns.AppendFormat(_T(" [%s] [INT] NOT NULL"), strColumnName);
            else
                strColumns.AppendFormat(_T(" [%s] [INT] "), strColumnName);
        } else if (enType == EnInt64) {
            if (pColumn->IsPrimaryKey())
                strColumns.AppendFormat(_T(" [%s] [INTEGER] NOT NULL"), strColumnName);
            else
                strColumns.AppendFormat(_T(" [%s] [INTEGER] "), strColumnName);
        } else if (enType == EnFloat || enType == EnDouble) {
            if (pColumn->IsPrimaryKey())
                strColumns.AppendFormat(_T(" [%s] [Double] NOT NULL"), strColumnName);
            else
                strColumns.AppendFormat(_T(" [%s] [Double] "), strColumnName);
        } else {
            if (pColumn->IsPrimaryKey())
                strColumns.AppendFormat(_T(" [%s] [Varchar2] NOT NULL"), strColumnName);
            else
                strColumns.AppendFormat(_T(" [%s] [Varchar2] "), strColumnName);
        }

        if (pColumn->IsPrimaryKey()) {
            if ( strPrimary.GetLength() != 0 ) {
                strPrimary.Append(_T(","));
            }

            strPrimary.AppendFormat(_T(" [%s] "), strColumnName);
        } 
    }

    if (strPrimary.GetLength() == 0)
        strSQL.Format(_T(" Create Table [%s] (%s)"), strLayerName, strColumns);
    else
        strSQL.Format(_T(" Create Table [%s] (%s, PRIMARY KEY(%s))"), strLayerName, strColumns, strPrimary);

    try {
        m_spDBConnection->ExecuteUpdate(strSQL);
    } catch (...) {
        TxLogDebugException();
        throw 4;
    }

    // TxMapTableColumns
    strSQL = (_T("Insert Into TxMapTableColumns(Table_Name, Column_Name, Column_Caption, Data_Type) Values (?,?,?,?) "));
    KPreparedStatementPtr spPreparedStatement = m_spDBConnection->PrepareStatement(strSQL);

    for (size_t i=0; i<nxCount; i++) {
        TxColumn*  pColumn       = oColumns.GetColumn(i);

        CString    strColumnName    = pColumn->Name();
        CString    strColumnCaption = pColumn->Caption();
        TxDataType enType           = pColumn->ColumnType();

        int nDataType(-1); {
            if (enType == EnString ) 
                nDataType = 0;
            else if (enType == EnInt32 ) 
                nDataType = 1;
            else if (enType == EnDouble) 
                nDataType = 2;
            else if (enType == EnInt64 ) 
                nDataType = 3;
            else if (enType == EnFloat ) 
                nDataType = 4;
        }
        
        spPreparedStatement->BindText(1, strLayerName);
        spPreparedStatement->BindText(2, strColumnName);
        spPreparedStatement->BindText(3, strColumnCaption);
        spPreparedStatement->BindInt (4, nDataType);

        spPreparedStatement->ExecuteUpdate();
        spPreparedStatement->Reset();
    }

    TxLayerInfoSQLPtr spLayerInfo = TxLayerInfoSQLPtr(new TxLayerInfoSQL(enGeoType, strLayerName, strTxIDField, strLayerName));
    spLayerInfo->SetGridSize(dGridSize);
    spLayerInfo->SetExtent(0,0,0,0);

    return spLayerInfo;
}

TxLayerInfoPtr TxSQLMapServer::AddLayerMSSql( EnumTxGeoType enGeoType, CString strLayerName, CString strCaption, CString strTxIDField, TxColumns& oColumns )
{
    // TxMapLayers, TxShp_~, TxIDX_~
    double dGridSize(1000);    
    if (enGeoType == GeoShapeType) {
        dGridSize = 1000.0;
    } 
    else if (enGeoType == GeoPointType)   {
        dGridSize = 1.0; //★ 포인트는 그리드 갯수와 Shape의 갯수가 동일하다.
    } 
    else if (enGeoType == GeoPolylineType || enGeoType == GeoMPolylineType){
        dGridSize = 1000.0;
    } 
    else if (enGeoType == GeoPolygonType  || enGeoType == GeoMPolygonType) {
        dGridSize = 5000.0;
    }
    else if (enGeoType == GeoMPointType)   {
        dGridSize = 100.0; //★ 포인트는 그리드 갯수와 Shape의 갯수가 동일하다.
    }
    else {
        throw 1;
    } 

    // 1.0 : TxMapLayers
    CString strSQL(_T(""));
    strSQL.Append(_T(" Insert Into TxMapLayers "));
    strSQL.Append(_T(" ( "));
    strSQL.Append(_T("   LayerID, GeometryType, TableName, TxID_Field, Caption, GridSize, MinX, MinY, MaxX, MaxY "));
    strSQL.Append(_T(" ) "));
    strSQL.AppendFormat(_T(" Select Max(LayerID)+1, %d, '%s', '%s', '%s', %f, 0.0, 0.0, 0.0, 0.0 From TxMapLayers "), (int)enGeoType, strLayerName, strTxIDField, strCaption, dGridSize);

    try {
        m_spDBConnection->ExecuteUpdate(strSQL);
    } catch (...) {
        TxLogDebugException();
        throw 1;
    }

    // 2.0 : TxShp_~
    strSQL.Format(_T(" CREATE TABLE [TxShp_%s] ( "), strLayerName );
    strSQL.Append(_T("    [TxID] [bigint] NOT NULL, "));
    strSQL.Append(_T("    [Avg_Extent] [numeric](30, 10) NULL, "));
    strSQL.Append(_T("    [MinX] [numeric](30, 10) NULL, "));
    strSQL.Append(_T("    [MinY] [numeric](30, 10) NULL, "));
    strSQL.Append(_T("    [MaxX] [numeric](30, 10) NULL, "));
    strSQL.Append(_T("    [MaxY] [numeric](30, 10) NULL, "));
    strSQL.Append(_T("    [TxGeometry] [varbinary](max) NULL, "));
    strSQL.Append(_T("    [Version] [bigint] NULL, "));
    strSQL.AppendFormat(_T("    CONSTRAINT [PK_TxShp_%s] PRIMARY KEY CLUSTERED "), strLayerName);
    strSQL.Append(_T("    ( "));
    strSQL.Append(_T("    [TxID] ASC "));
    strSQL.Append(_T("    )WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY] "));
    strSQL.Append(_T(") ON [PRIMARY] TEXTIMAGE_ON [PRIMARY] "));

    try {
        m_spDBConnection->ExecuteUpdate(strSQL);
    } catch (...) {
        TxLogDebugException();
        throw 2;
    }

    // 3.0 : TxIDX_~
    strSQL.Format(_T(" CREATE TABLE [TxIDX_%s]( "), strLayerName);
    strSQL.Append(_T("    [Gx] [bigint] NOT NULL, "));
    strSQL.Append(_T("    [Gy] [bigint] NOT NULL, "));
    strSQL.Append(_T("    [TxID] [bigint] NOT NULL, "));
    strSQL.AppendFormat(_T("     CONSTRAINT [PK_TxIDX_%s] PRIMARY KEY CLUSTERED "), strLayerName); 
    strSQL.Append(_T("    ( "));
    strSQL.Append(_T("    [Gx] ASC, "));
    strSQL.Append(_T("    [Gy] ASC, "));
    strSQL.Append(_T("    [TxID] ASC "));
    strSQL.Append(_T("    )WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY] "));
    strSQL.Append(_T(") ON [PRIMARY] "));

    try {
        m_spDBConnection->ExecuteUpdate(strSQL);
    } catch (...) {
        TxLogDebugException();
        throw 3;
    }

    // 4.0 BusinessTable
    CString strColumns(_T(""));
    CString strPrimary(_T(""));

    size_t nxCount = oColumns.ColumnCount();
    for (size_t i=0; i<nxCount; i++) {
        TxColumn*  pColumn       = oColumns.GetColumn(i);
        CString    strColumnName = pColumn->Name();
        TxDataType enType        = pColumn->ColumnType();

        if ( strColumns.GetLength() != 0 ) {
            strColumns.Append(_T(","));
        }

        if (enType == EnInt32) {
            if (pColumn->IsPrimaryKey())
                strColumns.AppendFormat(_T(" [%s] [INT] NOT NULL"), strColumnName);
            else
                strColumns.AppendFormat(_T(" [%s] [INT] "), strColumnName);
        } else if (enType == EnInt64) {
            if (pColumn->IsPrimaryKey())
                strColumns.AppendFormat(_T(" [%s] [bigint] NOT NULL"), strColumnName);
            else
                strColumns.AppendFormat(_T(" [%s] [bigint] "), strColumnName);
        } else if (enType == EnFloat) {
            if (pColumn->IsPrimaryKey())
                strColumns.AppendFormat(_T(" [%s] [float] NOT NULL"), strColumnName);
            else
                strColumns.AppendFormat(_T(" [%s] [float] "), strColumnName);
        } else if (enType == EnDouble) {
            if (pColumn->IsPrimaryKey())
                strColumns.AppendFormat(_T(" [%s] [float] NOT NULL"), strColumnName);
            else
                strColumns.AppendFormat(_T(" [%s] [float] "), strColumnName);
        } else {
            if (pColumn->IsPrimaryKey())
                strColumns.AppendFormat(_T(" [%s] [Varchar(max)] NOT NULL"), strColumnName);
            else
                strColumns.AppendFormat(_T(" [%s] [Varchar(max)] "), strColumnName);
        }

        if (pColumn->IsPrimaryKey()) {
            if ( strPrimary.GetLength() != 0 ) {
                strPrimary.Append(_T(","));
            }

            strPrimary.AppendFormat(_T(" [%s] ASC "), strColumnName);
        }
    } 

    if (strPrimary.GetLength() == 0) {
        strSQL.Format(_T(" Create Table [%s] (%s)"), strLayerName, strColumns);
    } 
    else {
        strSQL.Format(_T(" Create Table [%s] (%s, "), strLayerName, strColumns);
        strSQL.AppendFormat(_T(" CONSTRAINT [PK_%s] PRIMARY KEY CLUSTERED "), strLayerName); 
        strSQL.Append(_T("    ( "));
        strSQL.AppendFormat(_T(" %s "), strPrimary);
        strSQL.Append(_T("    )WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY] "));
        strSQL.Append(_T(") ON [PRIMARY] "));
    }

    try {
        m_spDBConnection->ExecuteUpdate(strSQL);
    } catch (...) {
        TxLogDebugException();
        throw 4;
    }

    TxLayerInfoSQLPtr spLayerInfo = TxLayerInfoSQLPtr(new TxLayerInfoSQL(enGeoType, strLayerName, strTxIDField, strLayerName));
    spLayerInfo->SetGridSize(dGridSize);
    spLayerInfo->SetExtent(0,0,0,0);

    return spLayerInfo;
}

TxEnvelope TxSQLMapServer::GetExtent( TxLayerInfoPtr spLayerInfo )
{
    TxEnvelope oEnv; 
    if (m_bConnected == false)
        return oEnv;

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  Min(MinX), Min(MinY), Max(MaxX), Max(MaxY) "));
        strSQL.Append(_T(" From "));
        strSQL.AppendFormat(_T(" TxShp_%s "), spLayerInfo->Name());
    }

    try {
        KResultSetPtr spResult = m_spDBConnection->ExecuteQuery(strSQL);
        if (spResult->Next()) {
            double dMinX = spResult->GetValueDouble(0);
            double dMinY = spResult->GetValueDouble(1);
            double dMaxX = spResult->GetValueDouble(2);
            double dMaxY = spResult->GetValueDouble(3);

            oEnv.SetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
        }
    } catch(...) {
        
    }

    return oEnv;
}

__int64 TxSQLMapServer::EntityCount( TxLayerInfoPtr spLayerInfo )
{
    __int64 nxCnt(0);
    if (m_bConnected == false)
        return nxCnt;

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  Count(*) "));
        strSQL.Append(_T(" From "));
        strSQL.AppendFormat(_T(" TxShp_%s "), spLayerInfo->Name());
    }

    try {
        KResultSetPtr spResult = m_spDBConnection->ExecuteQuery(strSQL);
        if (spResult->Next()) {
            nxCnt = spResult->GetValueInt64(0);
        }
    } catch(...) {
        
    }

    return nxCnt;
}

__int64 TxSQLMapServer::MaxID(TxLayerInfoPtr spLayerInfo)
{
    __int64 nxMaxID(0);
    if (m_bConnected == false)
        return nxMaxID;

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  Max(TxID) "));
        strSQL.Append(_T(" From "));
        strSQL.AppendFormat(_T(" TxShp_%s "), spLayerInfo->Name());
    }

    try {
        KResultSetPtr spResult = m_spDBConnection->ExecuteQuery(strSQL);
        if (spResult->Next()) {
            nxMaxID = spResult->GetValueInt64(0);
        }
    } catch(...) {
        
    }
    return nxMaxID;
}

void TxSQLMapServer::RebuildIndex( TxLayerInfoPtr _spLayerInfo, double a_dExGridSize)
{    
    if (m_bConnected == false) 
        throw 1;

    TxLayerInfoSQLPtr spLayerInfo = std::dynamic_pointer_cast<TxLayerInfoSQL>(_spLayerInfo);
    if (spLayerInfo == nullptr)
        throw 3;

    CString strLayerName = spLayerInfo->Name();

    double dGridSize(0);
    if (a_dExGridSize <= 0.0) { // 외부에서 입력한 GridSize가 없으면
        EnumTxGeoType enGeoType = spLayerInfo->GeoType();
        //★ 포인트 타입은 제외
        if (enGeoType == GeoPointType)
			return;
    }

    //★ 평균 그리드 사이즈 계산

    double dMinX(0), dMinY(0), dMaxX(0), dMaxY(0); {
        CString strSQL(_T("")); {
            strSQL.Append(_T(" Select "));
            strSQL.Append(_T("  Avg(Avg_Extent), Min(MinX), Min(MinY), Max(MaxX), Max(MaxY) "));
            strSQL.Append(_T(" From "));
            strSQL.AppendFormat(_T(" TxShp_%s "), strLayerName);
        }

        KResultSetPtr spResult = m_spDBConnection->ExecuteQuery(strSQL);
        if (spResult->Next()) {
            dGridSize = spResult->GetValueDbl(0);
            dMinX = spResult->GetValueDbl(1);
            dMinY = spResult->GetValueDbl(2);
            dMaxX = spResult->GetValueDbl(3);
            dMaxY = spResult->GetValueDbl(4);
        }
        else {
            return;
        }
    }

    if (a_dExGridSize > 0.0) { // 외부에서 입력한 GridSize가 있다면
        dGridSize = a_dExGridSize;
    }
    
    TxGridTileIndex oGridTileIndex(dGridSize);
    std::map<__int64, std::set<TxGridID>> mapTxGridInfo;

    CString strSQL(_T(""));
    strSQL.Format(_T("Select TxID, TxGeometry From TxShp_%s"), strLayerName);

    KResultSetPtr spResultSet = m_spDBConnection->ExecuteQuery(strSQL);
    while (spResultSet->Next())
    {        
        __int64 nxID = spResultSet->GetValueInt64(0);

        int nSize(0);
        const void * pValue = spResultSet->GetValueBytes(1, nSize);

        QBicBinary oBinary(nSize+10);  
        oBinary.PushBack(pValue, (int)nSize);

        int nGeometryType;
        oBinary.Position(0);
        oBinary.Read(nGeometryType);

        std::set<TxGridID> setGrid;
        ITxGeometryPtr spGeometry;

        if (nGeometryType == 1) {
            TxPoint oPt(oBinary, 0);
            __int64 Gx(0), Gy(0);
            oGridTileIndex.GetGeoGridID(oPt.X, oPt.Y, Gx, Gy);
            setGrid.insert(TxGridID(Gx, Gy));
        }
        else if (nGeometryType == 3) {
            TxPolyline oPLine(oBinary, 0);
            std::vector<TxPoint> vecPt = oPLine.ExtractVertex(dGridSize * 0.1);
            for (size_t i=0; i<vecPt.size(); i++) {
                TxPoint& oPt = vecPt[i];

                __int64 Gx(0), Gy(0);
                oGridTileIndex.GetGeoGridID(oPt.X, oPt.Y, Gx, Gy);
                setGrid.insert(TxGridID(Gx, Gy));
            }
        }    
        else if (nGeometryType == 4) {
            TxPolygon  oPolygon  = (oBinary, 0);
            TxEnvelope oEnvelope = oPolygon.Envelope();
            setGrid = oGridTileIndex.GetGeoGridID(oEnvelope.GetMinX(), oEnvelope.GetMinY(), oEnvelope.GetMaxX(), oEnvelope.GetMaxY());
        }   
        else if (nGeometryType == 5) {
            TxMPolygonPtr spMPoly = TxMPolygonPtr(new TxMPolygon(oBinary, 0));

            size_t nxCount = spMPoly->SubPolygonCount();
            for (size_t i=0; i<nxCount; i++) {
                TxEnvelope oSubEnvelope = spMPoly->SubPolygon(i)->Envelope();
                std::set<TxGridID> setSubGrid = oGridTileIndex.GetGeoGridID(oSubEnvelope.GetMinX(), oSubEnvelope.GetMinY(), oSubEnvelope.GetMaxX(), oSubEnvelope.GetMaxY());

                for (auto iter = setSubGrid.begin(); iter != setSubGrid.end(); ++iter) {
                    setGrid.insert(*iter);
                }
            }
        }   
        else {
            continue;
        }

        mapTxGridInfo[nxID] = setGrid;
    }
        
    if (mapTxGridInfo.size() == 0)
        return;

    m_spDBConnection->BeginTransaction();
    try 
    {
        /*레이어 정보*/ 
        CString strSQL(_T("")); {            
            strSQL.Format(_T("Update TxMapLayers Set GridSize = '%f', MinX = '%f', MinY = '%f', MaxX = '%f', MaxY = '%f' Where TableName = '%s' "), dGridSize, dMinX, dMinY, dMaxX, dMaxY, spLayerInfo->Name());
            m_spDBConnection->ExecuteUpdate(strSQL);
        }

        /*인덱스 삭제*/ {
            strSQL.Format(_T("Delete From TxIDX_%s"), spLayerInfo->Name());
            m_spDBConnection->ExecuteUpdate(strSQL);
        }
        /*인덱스 정보*/
        strSQL.Format(_T("Insert Into TxIDX_%s(TxID, Gx, GY) Values (?, ?, ?) "), spLayerInfo->Name());
        KPreparedStatementPtr spPreparedStatement = m_spDBConnection->PrepareStatement(strSQL);
        
        for (auto iter = mapTxGridInfo.begin(); iter != mapTxGridInfo.end(); ++iter) {
            __int64 TxID = iter->first;
            std::set<TxGridID>& setGrid = iter->second;
            
            for (auto iter2 = setGrid.begin(); iter2 != setGrid.end(); ++iter2) {
                TxGridID oGrid = *iter2;
                spPreparedStatement->BindInt64(1, TxID);
                spPreparedStatement->BindInt64(2, oGrid.Gx);
                spPreparedStatement->BindInt64(3, oGrid.Gy);

                spPreparedStatement->ExecuteUpdate();
                spPreparedStatement->Reset();	
            }
        }
        m_spDBConnection->Commit();

        spLayerInfo->SetExtent(dMinX, dMinY, dMaxX, dMaxY);
        spLayerInfo->SetGridSize(dGridSize);
    } 
    catch (...) {
        m_spDBConnection->RollBack();
		TxLogDebug(_T("Error - RebuildIndex RollBack"));
    }
}

TxColumns TxSQLMapServer::GetColumns( CString strLayerName )
{
    TxColumns oColumns;

    if (m_bConnected == false)
        return oColumns;

    CString strSQL(_T(""));
    strSQL.Append(_T(" Select Table_Name, Column_Name, Column_Caption, Data_Type "));
    strSQL.Append(_T(" From TxMapTableColumns"));
    strSQL.AppendFormat(_T(" Where Lower(Table_Name) = Lower(%s)"), strLayerName);

    KResultSetPtr spResultSet = m_spDBConnection->ExecuteQuery(strSQL);
    while (spResultSet->Next()) {
        TxDataType enType;

        CString strName    = spResultSet->GetValueString(1);
        CString strCaption = spResultSet->GetValueString(2);
        int     nDataType  = spResultSet->GetValueInt   (3); {
            if (nDataType == 0 ) 
                enType = EnString;
            else if (nDataType == 1) 
                enType = EnInt32;
            else if (nDataType == 2) 
                enType = EnDouble;
            else if (nDataType == 3) 
                enType = EnInt64;
            else if (nDataType == 4) 
                enType = EnFloat;
            else
                enType = EnString;
        }

        TxColumn* pColumn = oColumns.AddColumn(strName, enType);
        pColumn->Caption(strCaption);
    }

    return oColumns;
}

