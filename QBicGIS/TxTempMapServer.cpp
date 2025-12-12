#include "stdafx.h"
#include "TxTempMapServer.h"
#include "TxGUID.h"
#include "TxPoint.h"
#include "TxPolyline.h"
#include "TxPolygon.h"
#include "ThunderMapdrawProcess.h"
#include "../QBicCommon/QBicFile.h"

TxTempMapServer::TxTempMapServer()
{
    m_nxPosition       = 0;
    m_nxDataHeaderSize = 16; 

    TCHAR szCurrentDirectory[MAX_PATH] = {0};
    ::GetCurrentDirectory( MAX_PATH, szCurrentDirectory );
    CString strFilePath(szCurrentDirectory);

    strFilePath.MakeLower(); {
        int nLen = strFilePath.GetLength();
        int nIdx = strFilePath.ReverseFind(_T('\\'));

        if (nIdx >= 0 && nIdx == nLen-1) {
            strFilePath = strFilePath.Mid(0, nIdx);
        }
    }

    m_strName.Format(_T("%s\\%s.qbank"), strFilePath, TxGUID::GUID());
    //★ 메모리맵 오픈오류(0 Byte)
    std::ofstream oBlank(CString2MBString(m_strName), std::ios::binary); {
        char cEnd(9);
        oBlank.write(reinterpret_cast<char*>(&cEnd), sizeof(char));
    }    
    oBlank.close();

    try {
        std::string shpPath = CString2MBString(m_strName);
        m_mmapShp.open(shpPath);
    }
    catch (...) {
        throw 1;
    }    
}

TxTempMapServer::~TxTempMapServer()
{
    if (m_mmapShp.is_open() == true) {
        m_mmapShp.close();
    }
        
    QBicFile::DeleteFile(m_strName);
}

void TxTempMapServer::Clear()
{
    m_mmapShp.resize(1);
    m_oRTree.clear();
}

std::vector<IDShiftPos> TxTempMapServer::Intersect( const TxEnvelope& oEnvelope )
{
    std::vector<IDShiftPos> vecResult;
    if (m_oRTree.size() < 1) {
        return vecResult;
    }

    double dMinX(0.0), dMinY(0.0), dMaxX(0.0), dMaxY(0.0);
    oEnvelope.GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);

    BoostEnvelope oBoostEnvelope(BoostPoint(dMinX, dMinY), BoostPoint(dMaxX, dMaxY));

    std::vector<TreeValue> vecTreeValue;
    m_oRTree.query(BoostGeometry::index::intersects(oBoostEnvelope), std::back_inserter(vecTreeValue));

    for (size_t i=0; i<vecTreeValue.size(); i++)
    {
        TreeValue& oNode = vecTreeValue[i];
        vecResult.push_back(oNode.second);
    }

    return vecResult;
}

ITxGeometryPtr TxTempMapServer::LoadGeometry( __int64 nxID )
{
    ITxGeometryPtr spGeometry = nullptr;
    BoostEnvelope oBoostEnvelope = m_oRTree.bounds();
    auto iter = m_oRTree.qbegin(BoostGeometry::index::intersects(oBoostEnvelope));
    auto end  = m_oRTree.qend();

    for (/**/; iter != end; ++iter)
    {
        TreeValue  oNode = *iter;
        IDShiftPos oIDShiftPos = oNode.second;

        if (oIDShiftPos.FirstKey == nxID) {
            spGeometry = ReadBin2Geo((size_t)oIDShiftPos.SecondKey);
            break;
        }
    }

    return spGeometry;
}

void TxTempMapServer::LoadGeometry( TxEnvelope oEnvelope, std::map<__int64, ITxGeometryPtr>& mapGeometry)
{
    std::vector<IDShiftPos> vecGeo = Intersect(oEnvelope);
    for (size_t i=0; i<vecGeo.size(); i++) {
        IDShiftPos& oIDShiftPos = vecGeo[i];

        ITxGeometryPtr spGeometry = ReadBin2Geo((size_t)oIDShiftPos.SecondKey);
        if (spGeometry) {
            mapGeometry.insert(std::make_pair(oIDShiftPos.FirstKey, spGeometry));
        }
    }
}

void TxTempMapServer::LoadGeometry( TxEnvelope oEnvelope, std::map<__int64, ITxGeometryPtr>& mapGeometry, ThunderMapdrawProcess* pDrawProcess)
{
    std::vector<IDShiftPos> vecGeo = Intersect(oEnvelope);
    for (size_t i=0; i<vecGeo.size(); i++) {
        if (pDrawProcess->GetMapDrawStatus() == 0)
            break;

        IDShiftPos& oIDShiftPos = vecGeo[i];

        ITxGeometryPtr spGeometry = ReadBin2Geo((size_t)oIDShiftPos.SecondKey);
        if (spGeometry) {
            mapGeometry.insert(std::make_pair(oIDShiftPos.FirstKey, spGeometry));
        }
    }
}

ITxGeometryPtr TxTempMapServer::ReadBin2Geo( size_t nxShiftPos )
{
    size_t nxSize = m_mmapShp.size();    
    size_t nxPosition(nxShiftPos);

    const char* pPos = m_mmapShp.const_data();
    if (nxSize < nxShiftPos + m_nxDataHeaderSize + 12/*최소한개포인트*/ + 1/*End*/)
        return nullptr;

    pPos += nxPosition;

    int nFlag(0), nGeoSize(1);
    __int64 nxID;
    memcpy(&nFlag,    pPos, sizeof(int));     pPos += sizeof(int);
    memcpy(&nxID,     pPos, sizeof(__int64)); pPos += sizeof(__int64);
    memcpy(&nGeoSize, pPos, sizeof(int));     pPos += sizeof(int);    

    nxPosition += m_nxDataHeaderSize;

    if (nxPosition + nGeoSize >= nxSize) 
        return nullptr;

    QBicBinary oBinary(nGeoSize);
    oBinary.PushBack(pPos, nGeoSize);

    // Geometry Parsing
    int nGeometryType(0);
    oBinary.Position(0);
    oBinary.Read(nGeometryType);

    ITxGeometryPtr spGeometry;

    if (nGeometryType == 1) {
        spGeometry = ITxGeometryPtr(new TxPoint(oBinary));
    }
    else if (nGeometryType == 3) {
        spGeometry = ITxGeometryPtr(new TxPolyline(oBinary));
    }   
    else if (nGeometryType == 4) {
        spGeometry = ITxGeometryPtr(new TxPolygon(oBinary));
    }  
    else if (nGeometryType == 5) {
        spGeometry = ITxGeometryPtr(new TxMPolygon(oBinary));
    }  
    else if (nGeometryType == 8) {
        spGeometry = ITxGeometryPtr(new TxMPoint(oBinary));
    }  
    else if (nGeometryType == 9) {
        spGeometry = ITxGeometryPtr(new TxMPolyline(oBinary));
    }  
    /*
    GeoUnknownType   = 0,
    GeoPointType     = 1,
    GeoPolylineType  = 3,
    GeoPolygonType   = 4,
    GeoMPolygonType  = 5,
    GeoMPointType    = 8,
    GeoMPolylineType = 9
    */
    return spGeometry;
}

void TxTempMapServer::SoftFileDelete( __int64 nxID )
{
    ITxGeometryPtr spGeometry;
    BoostEnvelope  oBoostEnvelope = m_oRTree.bounds();
    auto iter = m_oRTree.qbegin(BoostGeometry::index::intersects(oBoostEnvelope));
    auto end  = m_oRTree.qend();

    for (/**/; iter != end; ++iter)
    {
        TreeValue  oNode = *iter;
        IDShiftPos oIDShiftPos = oNode.second;

        if (oIDShiftPos.FirstKey == nxID)
        {
            size_t nxSize = m_mmapShp.size();    
            char*  pPos   = m_mmapShp.data();

            __int64 nxShiftPos = oIDShiftPos.SecondKey;
            if (nxSize <= nxShiftPos + sizeof(int))
                return;

            pPos += nxShiftPos;
            int nFlag(0);
            memcpy(pPos, &nFlag, sizeof(int));            

            m_oRTree.remove(oNode);      

            return;
        }
    }
}

void TxTempMapServer::SoftFileDelete()
{
    BoostEnvelope oBoostEnvelope = m_oRTree.bounds();
    auto iter = m_oRTree.qbegin(BoostGeometry::index::intersects(oBoostEnvelope));
    auto end  = m_oRTree.qend();

    for (/**/; iter != end; ++iter)
    {
        TreeValue  oNode = *iter;
        IDShiftPos oIDShiftPos = oNode.second;

        size_t nxSize = m_mmapShp.size();    
        char*  pPos   = m_mmapShp.data();

        __int64 nxShiftPos = oIDShiftPos.SecondKey;
        if (nxSize <= nxShiftPos + sizeof(int))
            continue;

        pPos += nxShiftPos;
        int nFlag(0);
        memcpy(pPos, &nFlag, sizeof(int));
    }

    m_oRTree.clear();
}

void TxTempMapServer::Delete()
{
    /*SoftFileDelete();*/
    Clear();
}

void TxTempMapServer::Delete( __int64 nxID )
{
    SoftFileDelete(nxID);
}

void TxTempMapServer::Update( TxFeaturePtr spFeature )
{
    SoftFileDelete(spFeature->TxObjectID);
    Insert(spFeature);
}

void TxTempMapServer::Insert( TxFeaturePtr spFeature )
{
    std::map<__int64, ITxGeometryPtr> mapGeometry;
    mapGeometry.insert(std::make_pair(spFeature->TxObjectID, spFeature->Geometry()));

    Insert(mapGeometry);
}

void TxTempMapServer::Insert( std::map<__int64, ITxGeometryPtr>& mapGeometry )
{
    size_t nxSize = m_mmapShp.size();
    Insert(mapGeometry, nxSize-1);
}

void TxTempMapServer::Insert( std::map<__int64, ITxGeometryPtr>& mapGeometry, size_t nxPos )
{   
    if (mapGeometry.size() < 1) 
        return;
    
    size_t nxSize = m_mmapShp.size();
    if (nxSize -1 < nxPos)
        return;

    auto iter = mapGeometry.begin();
    auto end  = mapGeometry.end();

    size_t nxFileSize(0);
    for (iter = mapGeometry.begin(); iter != end; ++iter) {
        __int64        nxID       = iter->first;
        ITxGeometryPtr spGeometry = iter->second;

        if (spGeometry == nullptr)
            continue;

        QBicBinary oGeoBin; {
            spGeometry->GetBinary(oGeoBin);
            oGeoBin.Position(0);
        }        

        size_t nxGeoSize = oGeoBin.Size();
        nxFileSize = nxFileSize + m_nxDataHeaderSize + nxGeoSize;
    }
        
    m_mmapShp.resize(nxSize + nxFileSize);

    size_t nxFilePos(nxPos);

    for (iter = mapGeometry.begin(); iter != end; ++iter) {
        __int64        nxID       = iter->first;
        ITxGeometryPtr spGeometry = iter->second;

        if (spGeometry == nullptr)
            continue;

        QBicBinary oGeoBin;
        spGeometry->GetBinary(oGeoBin);
        oGeoBin.Position(0);

        size_t nxGeoSize = oGeoBin.Size();
        char* pPos = m_mmapShp.data();
        pPos += nxFilePos;

        {//Writing
            int nFlag(1);
            int nGeoSize = (int)nxGeoSize;

            memcpy(pPos, &nFlag,    sizeof(int));      pPos += sizeof(int);
            memcpy(pPos, &nxID,     sizeof(__int64));  pPos += sizeof(__int64);
            memcpy(pPos, &nGeoSize, sizeof(int));      pPos += sizeof(int);
            memcpy(pPos, oGeoBin,   nGeoSize);         pPos += nGeoSize;
        }

        double x1, y1, x2, y2;
        TxEnvelope oEnvelope = spGeometry->Envelope();

        oEnvelope.GetEnvelope(x1, y1, x2, y2);
        IDShiftPos oIDShiftPos(nxID, nxFilePos);
        BoostEnvelope oBoostEnvelope(BoostPoint(x1, y1), BoostPoint(x2, y2));
        m_oRTree.insert(std::make_pair(oBoostEnvelope, oIDShiftPos)); 
        
        nxFilePos = nxFilePos + m_nxDataHeaderSize + nxGeoSize;
    }
}

TxEnvelope TxTempMapServer::Extent()
{
    BoostEnvelope oEnvelope = m_oRTree.bounds();
    return TxEnvelope(oEnvelope);
}

__int64 TxTempMapServer::EntityCount()
{
    return m_oRTree.size();
}

__int64 TxTempMapServer::MaxID()
{
    __int64 nxMaxID(0);

    BoostEnvelope oBoostEnvelope = m_oRTree.bounds();
    auto iter = m_oRTree.qbegin(BoostGeometry::index::intersects(oBoostEnvelope));
    auto end  = m_oRTree.qend();

    for (/**/; iter != end; ++iter) {
        TreeValue  oNode = *iter;
        IDShiftPos oIDShiftPos = oNode.second;

        __int64 nxID = oIDShiftPos.FirstKey;
        if (nxMaxID < nxID) {
            nxMaxID = nxID;
        }
    }

    return nxMaxID;
}
