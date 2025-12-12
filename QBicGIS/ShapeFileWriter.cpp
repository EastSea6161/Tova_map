#include "stdafx.h"
#include "ShapeFileWriter.h"

ShapeFileWriter::ShapeFileWriter()
{
    m_hSHP = nullptr;
    m_hDBF = nullptr;

    m_bOpened    = false;
    m_nRecordID  = 0;
    m_nShapeType = 0;
}

ShapeFileWriter::~ShapeFileWriter()
{
    Close();
}

int ShapeFileWriter::CreateShapeFile( CString strPath, CString strName, int nShapeType, std::vector<DbfFieldInfo>& vecFieldInfo )
{
#pragma region PathNameInit
    strPath.MakeLower(); {
        int nLen = strPath.GetLength();
        int nIdx = strPath.ReverseFind(_T('\\'));

        if (nIdx >= 0 && nIdx == nLen-1) {
            strPath = strPath.Mid(0, nIdx);
        }
    }
	CString strTempName = strName;
    strTempName.MakeLower(); {
        int nIdx = strTempName.Find(_T(".shp"));

        if (nIdx >= 0) {
            strName = strName.Mid(0, nIdx);
        }
    }    
#pragma endregion PathNameInit

    try
    {
        // 검사 : 필드 정보 체크
        size_t nxCount = vecFieldInfo.size();
        if (nxCount == 0)
            throw 1;

        // 검사 : 필드명칭, 필드타입검사
        for(size_t i=0; i<nxCount; i++)
        {
            DbfFieldInfo& oShapeField = vecFieldInfo[i];
            if (oShapeField.m_strFieldName.Trim().IsEmpty())
                throw 2;

            if (oShapeField.m_nFiledType > 2 || oShapeField.m_nFiledType < 0)
                throw 2;
        }

        CString strShpName(_T(""));
        strShpName.Format(_T("%s\\%s.shp"), strPath, strName);

        SHPHandle hSHP = nullptr;
        hSHP = SHPCreate( CString2MBString(strShpName), nShapeType );
        if (hSHP == nullptr)
            throw 3;

        SHPClose( hSHP );

        CString strDbfName(_T(""));
        strDbfName.Format(_T("%s\\%s.dbf"), strPath, strName);

        DBFHandle	hDBF;
        hDBF = DBFCreate( CString2MBString(strDbfName) );

        if (hDBF == nullptr)
            throw 4;

        for (size_t i=0; i<vecFieldInfo.size(); i++)
        {
            DbfFieldInfo& oShapeField = vecFieldInfo[i];

            DBFAddField(hDBF, CString2MBString(oShapeField.m_strFieldName), (DBFFieldType)oShapeField.m_nFiledType, oShapeField.m_nWidth, oShapeField.m_nDecimals);
        }

        DBFClose( hDBF );    
        return 1;
    }
    catch (int& ex)
    {
        switch (ex)
        {
        case 1 :
            TxLogDebugException();
            break;
        case 2 :
            TxLogDebugException();
            break;
        case 3 :
            TxLogDebugException();
            break;
        case 4 :
            TxLogDebugException();
            break;
        default:
            TxLogDebugException();
            break;
        }   
    }
    catch (...)
    {
        TxLogDebugException();
    }

    return 0;
}


void ShapeFileWriter::AppendShapeFile( SHPHandle hSHP, TxPointPtr spPoint )
{
    int		nShapeType(1/*포인트*/), nVertices(0), nParts(0), *panParts;
    double	*padfX, *padfY, *padfZ = NULL, *padfM = NULL;

    SHPObject* psObject;

    panParts = new int[1];

    nParts      = 1;       /*1개 Part*/
    panParts[0] = 0;       /*첫번째 인덱스 위치*/
    nVertices   = 1;       /*버텍스 갯수*/

    // 좌표 정보
    padfX = new double[nVertices];
    padfY = new double[nVertices];

    padfX[0] = spPoint->X; padfY[0] = spPoint->Y;

    // Append : nShapeID(-1)
    psObject = SHPCreateObject( nShapeType, /*nShapeID*/-1, nParts, panParts, NULL, nVertices, padfX, padfY, NULL, NULL );

    if (psObject == nullptr)
        throw 12;

    SHPWriteObject  ( hSHP, /*nShapeID*/-1, psObject );
    SHPDestroyObject( psObject );

    delete[] panParts;
    delete[] padfX;
    delete[] padfY;
}

void ShapeFileWriter::AppendShapeFile( SHPHandle hSHP, TxPolylinePtr spPolyline )
{
    // 포인트의 갯수
    std::vector<TxPoint> vecPoint = spPolyline->GetPoints();
    int nCount = (int)vecPoint.size();

    if (nCount == 0)
        throw 11;

    /* 가정 : 멀티 라인은 기능에서 제공하지 않는다. */

    int		nShapeType(3), nVertices(0), nParts(0), *panParts;
    double	*padfX, *padfY, *padfZ = NULL, *padfM = NULL;

    panParts = new int[1];

    nParts      = 1;       /*1개 Part*/
    panParts[0] = 0;       /*첫번째 인덱스 위치*/
    nVertices   = nCount;  /*버텍스 갯수*/

    // 좌표 정보
    padfX = new double[nVertices];
    padfY = new double[nVertices];

    for(int i=0; i<nCount; i++) {
        TxPoint& oPoint = vecPoint[i];
        padfX[i] = oPoint.X; padfY[i] = oPoint.Y;
    }
    
    // Append : nShapeID(-1)
    SHPObject* psObject = SHPCreateObject( nShapeType, /*nShapeID*/-1, nParts, panParts, NULL, nVertices, padfX, padfY, NULL, NULL );
    if (psObject == nullptr)
        throw 12;

    SHPWriteObject( hSHP, /*nShapeID*/-1, psObject );
    SHPDestroyObject( psObject );

    delete[] panParts;
    delete[] padfX;
    delete[] padfY;
}

void ShapeFileWriter::AppendShapeFile( SHPHandle hSHP, TxPolygonPtr spPolygon )
{    
    std::map<int, std::vector<TxPoint>> mapRingPt; 

    int nKey(0); 
    if (/*Outring*/true) {
        std::vector<TxPoint> vecPt = spPolygon->OutRing();
        mapRingPt.insert(std::make_pair(nKey, vecPt));
        nKey++;
    }
    
    size_t nxHoleCnt = spPolygon->GetHoleCount();
    for (size_t i = 0; i < nxHoleCnt; i++) {
        std::vector<TxPoint> vecPt = spPolygon->GetHolePoints(i);
        mapRingPt.insert(std::make_pair(nKey, vecPt));
        nKey++;
    }

    int		nShapeType(5),  nVertices(0),  nParts(0),  *panParts;
    double	*padfX, *padfY, *padfZ = NULL, *padfM = NULL;
    
    for (auto iter = mapRingPt.begin(); iter != mapRingPt.end(); ++iter) {
        std::vector<TxPoint>& vecPt = iter->second;
        nVertices += (int)vecPt.size();
    }

    nParts   = (int)mapRingPt.size();
    panParts = new int[nParts];

    // 좌표 정보
    padfX = new double[nVertices];
    padfY = new double[nVertices];

    int nIdx(0);
    for (auto iter = mapRingPt.begin(); iter != mapRingPt.end(); ++iter) {        
        panParts[iter->first] = nIdx;
        std::vector<TxPoint>& vecPt = iter->second;
        for (size_t i=0; i<vecPt.size(); i++) {                  
            TxPoint& oPt = vecPt[i];
            padfX[nIdx] = oPt.X;
            padfY[nIdx] = oPt.Y;

            nIdx++;
        }        
    }

    // Append : nShapeID(-1)
    SHPObject* psObject = SHPCreateObject( nShapeType, /*nShapeID*/-1, nParts, panParts, NULL, nVertices, padfX, padfY, NULL, NULL );
    if (psObject == nullptr)
        throw 12;

    SHPWriteObject( hSHP, /*nShapeID*/-1, psObject );
    SHPDestroyObject( psObject );

    delete[] panParts;
    delete[] padfX;
    delete[] padfY;
}

void ShapeFileWriter::AppendShapeFile( SHPHandle hSHP, TxMPolygonPtr spMPolygon )
{
    std::map<int, std::vector<TxPoint>> mapRingPt; 
    size_t nxSubCnt = spMPolygon->SubPolygonCount();
    
    int nKey(0);
    for (size_t i = 0; i < nxSubCnt; i++) {
        TxPolygon* pPolygon = spMPolygon->SubPolygon(i);

        if (/*Outring*/true) {
            std::vector<TxPoint> vecPt = pPolygon->OutRing();
            mapRingPt.insert(std::make_pair(nKey, vecPt));

            nKey++;
        }

        size_t nxHoleCnt = pPolygon->GetHoleCount();
        for (size_t i = 0; i < nxHoleCnt; i++) {
            std::vector<TxPoint> vecPt = pPolygon->GetHolePoints(i);
            mapRingPt.insert(std::make_pair(nKey, vecPt));
            nKey ++;
        }
    }
    
    int		nShapeType(5),  nVertices(0),  nParts(0),  *panParts;
    double	*padfX, *padfY, *padfZ = NULL, *padfM = NULL;

    for (auto iter = mapRingPt.begin(); iter != mapRingPt.end(); ++iter) {
        std::vector<TxPoint>& vecPt = iter->second;
        nVertices += (int)vecPt.size();
    }

    nParts   = (int)mapRingPt.size();
    panParts = new int[nParts];

    // 좌표 정보
    padfX = new double[nVertices];
    padfY = new double[nVertices];

    int nIdx(0);
    for (auto iter = mapRingPt.begin(); iter != mapRingPt.end(); ++iter) {        
        panParts[iter->first] = nIdx;
        std::vector<TxPoint>& vecPt = iter->second;
        for (size_t i=0; i<vecPt.size(); i++) {                  
            TxPoint& oPt = vecPt[i];
            padfX[nIdx] = oPt.X;
            padfY[nIdx] = oPt.Y;

            nIdx++;
        }        
    }

    // Append : nShapeID(-1)
    SHPObject* psObject = SHPCreateObject( nShapeType, /*nShapeID*/-1, nParts, panParts, NULL, nVertices, padfX, padfY, NULL, NULL );
    if (psObject == nullptr)
        throw 12;

    SHPWriteObject( hSHP, /*nShapeID*/-1, psObject );
    SHPDestroyObject( psObject );

    delete[] panParts;
    delete[] padfX;
    delete[] padfY;
}

void ShapeFileWriter::WriteDbf( DBFHandle hDBF, int nRecordID, std::vector<DbfFieldValue>& vecRecord )
{
    #pragma region comment
    /*
    1) DBF 필드 타입 정의에서 자리수를 제약할 수 있음. 
       필드의 자리 수 제약에 대한 테스트가 이루어 지지 않음
    2) 필드 인덱스에 대한 에러 체크는 없음
    */
#pragma endregion comment
    size_t nxCount = vecRecord.size();
    if (nxCount == 0)
        throw 21;
    
    for (size_t i=0; i<nxCount; i++) {
        DbfFieldValue& oFieldValue = vecRecord[i];
        
        int     nFieldIndex = oFieldValue.m_nFieldIndex;
        CString strValue    = oFieldValue.m_strValue;

        DBFFieldType oFieldType = DBFGetFieldInfo(hDBF, nFieldIndex, nullptr, nullptr, nullptr);
        
        if (oFieldType == FTInteger) {
            int nFieldValue = _ttoi(strValue);
            DBFWriteIntegerAttribute(hDBF, nRecordID, nFieldIndex, nFieldValue);
        }
        else if (oFieldType == FTDouble) {
            double dFieldValue = _ttof(strValue);
            DBFWriteDoubleAttribute(hDBF, nRecordID, nFieldIndex, dFieldValue);
        }
        else if (oFieldType == FTString) {
            DBFWriteStringAttribute(hDBF, nRecordID, nFieldIndex, CString2MBString(strValue));
        }
        else {
            throw 22;
        }
    }
}

int ShapeFileWriter::WriteBulkPoint( CString strPath, CString strName, std::vector<ITxGeometryPtr>& vecGeometry, std::vector<std::vector<DbfFieldValue>>& vecRecords )
{
    #pragma region PathNameInit
    strPath.MakeLower(); {
        int nLen = strPath.GetLength();
        int nIdx = strPath.ReverseFind(_T('\\'));

        if (nIdx >= 0 && nIdx == nLen-1) {
            strPath = strPath.Mid(0, nIdx);
        }
    }
    strName.MakeLower(); {
        int nIdx = strName.Find(_T(".shp"));

        if (nIdx >= 0) {
            strName = strName.Mid(0, nIdx);
        }
    }    
    #pragma endregion PathNameInit
    
    size_t nxGeoCount = vecGeometry.size();
    size_t nxRecCount = vecRecords.size();

    if (nxGeoCount != nxRecCount) // Geometry와 데이터 갯수 체크
        throw 1;

    if (nxGeoCount == 0)
        return 1;

    CString strShpName(_T(""));
    strShpName.Format(_T("%s\\%s.shp"), strPath, strName);

    CString strDbfName(_T(""));
    strDbfName.Format(_T("%s\\%s.dbf"), strPath, strName);

    DBFHandle	hDBF;
    hDBF = DBFOpen( CString2MBString(strDbfName), "r+b" );
    
    if (hDBF == nullptr)
        throw 2;

    SHPHandle	hSHP;
    hSHP = SHPOpen( CString2MBString(strShpName), "r+b" );
    if (hSHP == nullptr) {
        DBFClose(hDBF);
        throw 3;
    }

    int nShapeType(0);
    SHPGetInfo( hSHP, NULL, &nShapeType, NULL, NULL );

    if (nShapeType != SHPT_POINT /*포인트*/) {
        DBFClose(hDBF);
        SHPClose(hSHP);
        throw 4;
    }

    // 여기서 분기한다. -> Shape / Dbf Write
    int nRecordID(0);
    for (size_t i=0; i<nxGeoCount; i++) {
        ITxGeometryPtr spGeometry = vecGeometry[i];
        TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);
        std::vector<DbfFieldValue>& vecRecord = vecRecords[i];

        if (spPoint != nullptr) {
            AppendShapeFile(hSHP, spPoint);
            WriteDbf       (hDBF, nRecordID, vecRecord);
            nRecordID++;
        }        
    }

    DBFClose(hDBF);
    SHPClose(hSHP);
    return 1;
}

int ShapeFileWriter::WriteBulkPolyLine( CString strPath, CString strName, std::vector<ITxGeometryPtr>& vecGeometry, std::vector<std::vector<DbfFieldValue>>& vecRecords )
{
#pragma region PathNameInit
    strPath.MakeLower(); {
        int nLen = strPath.GetLength();
        int nIdx = strPath.ReverseFind(_T('\\'));

        if (nIdx >= 0 && nIdx == nLen-1) {
            strPath = strPath.Mid(0, nIdx);
        }
    }
    strName.MakeLower(); {
        int nIdx = strName.Find(_T(".shp"));

        if (nIdx >= 0) {
            strName = strName.Mid(0, nIdx);
        }
    }    
#pragma endregion PathNameInit

    size_t nxGeoCount = vecGeometry.size();
    size_t nxRecCount = vecRecords.size();

    if (nxGeoCount != nxRecCount) // Geometry와 데이터 갯수 체크
        throw 1;

    if (nxGeoCount == 0)
        return 1;

    CString strShpName(_T(""));
    strShpName.Format(_T("%s\\%s.shp"), strPath, strName);

    CString strDbfName(_T(""));
    strDbfName.Format(_T("%s\\%s.dbf"), strPath, strName);

    DBFHandle	hDBF;
    hDBF = DBFOpen( CString2MBString(strDbfName), "r+b" );

    if (hDBF == nullptr)
        throw 2;

    SHPHandle	hSHP;
    hSHP = SHPOpen( CString2MBString(strShpName), "r+b" );
    if (hSHP == nullptr)
    {
        DBFClose(hDBF);
        throw 3;
    }

    int nShapeType(0);
    SHPGetInfo( hSHP, NULL, &nShapeType, NULL, NULL );

    if (nShapeType != SHPT_ARC /*폴리라인*/)
    {
        DBFClose(hDBF);
        SHPClose(hSHP);
        throw 4;
    }

    // 여기서 분기한다. -> Shape / Dbf Write
    int nRecordID(0);
    for (size_t i=0; i<nxGeoCount; i++)
    {
        ITxGeometryPtr spGeometry = vecGeometry[i];
        TxPolylinePtr  spPolyline = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
                
        std::vector<DbfFieldValue>& vecRecord = vecRecords[i];

        if (spPolyline != nullptr){
            AppendShapeFile(hSHP, spPolyline);
            WriteDbf       (hDBF, nRecordID, vecRecord);
            nRecordID++;
        }
    }

    DBFClose(hDBF);
    SHPClose(hSHP);
    return 1;
}


bool ShapeFileWriter::Open( CString strPath, CString strName )
{
#pragma region PathNameInit
    strPath.MakeLower(); {
        int nLen = strPath.GetLength();
        int nIdx = strPath.ReverseFind(_T('\\'));

        if (nIdx >= 0 && nIdx == nLen-1) {
            strPath = strPath.Mid(0, nIdx);
        }
    }
    strName.MakeLower(); {
        int nIdx = strName.Find(_T(".shp"));

        if (nIdx >= 0) {
            strName = strName.Mid(0, nIdx);
        }
    }    
#pragma endregion PathNameInit
        
    Close();

    CString strShpName(_T(""));
    strShpName.Format(_T("%s\\%s.shp"), strPath, strName);

    CString strDbfName(_T(""));
    strDbfName.Format(_T("%s\\%s.dbf"), strPath, strName);

    DBFHandle	hDBF;
    hDBF = DBFOpen( CString2MBString(strDbfName), "r+b" );

    if (hDBF == nullptr)
        throw 2;

    SHPHandle	hSHP;
    hSHP = SHPOpen( CString2MBString(strShpName), "r+b" );
    if (hSHP == nullptr) {
        DBFClose(hDBF);
        throw 3;
    }

    SHPGetInfo( hSHP, NULL, &m_nShapeType, NULL, NULL );

    m_nRecordID = DBFGetRecordCount(hDBF);

    m_hSHP = hSHP;
    m_hDBF = hDBF;

    m_bOpened = true;
    return true;
}

void ShapeFileWriter::Close()
{
    m_bOpened    = false;
    m_nRecordID  = 0;
    m_nShapeType = 0;
    DBFClose(m_hDBF);
    SHPClose(m_hSHP);

    m_hSHP = nullptr;
    m_hDBF = nullptr;
}

int ShapeFileWriter::AppendShape( ITxGeometryPtr spGeometry, std::vector<DbfFieldValue>& vecFieldValue )
{
    if (m_bOpened == false)
        return 0;

    if (m_nShapeType == 1 ) {
        TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);
        if (spPoint != nullptr) {
            AppendShapeFile(m_hSHP, spPoint);
            WriteDbf       (m_hDBF, m_nRecordID, vecFieldValue);
            m_nRecordID++;
        } 
        else {
            return 0;
        }
    }
    else if (m_nShapeType == 3 ) {
        TxPolylinePtr spPolyline = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        if (spPolyline != nullptr) {
            AppendShapeFile(m_hSHP, spPolyline);
            WriteDbf       (m_hDBF, m_nRecordID, vecFieldValue);
            m_nRecordID++;
        }
        else {
            return 0;
        }
    }
    else if (m_nShapeType == 5) {
        if ( spGeometry->GeometryType() == GeoPolygonType ) {
            TxPolygonPtr spPolygon = std::dynamic_pointer_cast<TxPolygon>(spGeometry);
            if (spPolygon != nullptr) {
                AppendShapeFile(m_hSHP, spPolygon);
                WriteDbf       (m_hDBF, m_nRecordID, vecFieldValue);
                m_nRecordID++;
            }
            else {
                return 0;
            }            
        }
        else if ( spGeometry->GeometryType() == GeoMPolygonType) {
            TxMPolygonPtr spMPolygon = std::dynamic_pointer_cast<TxMPolygon>(spGeometry);     
            if (spMPolygon != nullptr) {
                AppendShapeFile(m_hSHP, spMPolygon);
                WriteDbf       (m_hDBF, m_nRecordID, vecFieldValue);
                m_nRecordID++;
            }
            else {
                return 0;
            }
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }

    return 1;
}

