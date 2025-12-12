#include "stdafx.h"
#include <stdio.h>
#include "ShapeFileReader.h"
#include "TxMapEnumType.h"
#include "TxPoint.h"
#include "TxPolygon.h"
#include "TxMath.h"
#include "../QBicCommon/CString2MBString.h"

ShapeFileReader::ShapeFileReader() : m_hShp(nullptr), m_hDBF(nullptr), m_nEncoding(0)
{    
}

ShapeFileReader::~ShapeFileReader()
{
    Close();
}

void ShapeFileReader::SetEncoding(int a_nEncoding)
{
	m_nEncoding = a_nEncoding;
}

int ShapeFileReader::Open(CString strPath, CString strName/* const char* pszShapeFile */)
{
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

    CString strShp(_T(""));
    CString strDbf(_T(""));

    strShp.Format(_T("%s\\%s.shp"), strPath, strName);
    strDbf.Format(_T("%s\\%s.dbf"), strPath, strName);

    CString2MBString szShp(strShp);
    CString2MBString szDbf(strDbf);

    /*if (m_hShp) SHPClose(m_hShp);
    if (m_hDBF) DBFClose(m_hDBF);*/
    Close();

    int	   nShapeType,    nEntities;
    double adfBndsMin[4], adfBndsMax[4];

    SHPHandle hSHP = SHPOpen((const char*)szShp, "rb" );
    if (hSHP == nullptr) {
        Close();
        return 0;
    }

    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfBndsMin, adfBndsMax );
   
    m_oShpInfo.ShapeType = nShapeType;
    m_oShpInfo.Entities  = nEntities;
    m_oShpInfo.BoundMin[0] = adfBndsMin[0];
    m_oShpInfo.BoundMin[1] = adfBndsMin[1];
    m_oShpInfo.BoundMin[2] = adfBndsMin[2];
    m_oShpInfo.BoundMin[3] = adfBndsMin[3];

    m_oShpInfo.BoundMax[0] = adfBndsMax[0];
    m_oShpInfo.BoundMax[1] = adfBndsMax[1];
    m_oShpInfo.BoundMax[2] = adfBndsMax[2];
    m_oShpInfo.BoundMax[3] = adfBndsMax[3];

    m_hShp = hSHP;

    DBFHandle	hDbf;
    hDbf = DBFOpen(szDbf, "rb" );
    if (hDbf == nullptr) {
        Close();
        return 0;
    }

    m_hDBF = hDbf;

    return 1;
}


void ShapeFileReader::Close()
{
    if (m_hShp)
        SHPClose(m_hShp);

    if (m_hDBF)
        DBFClose(m_hDBF);

    m_hShp = nullptr;
    m_hDBF = nullptr;
}


int ShapeFileReader::ShapeInfo(const char* pszShapeFile, ShapeFileInfo& oShapeFileInfo)
{    
    int	   nShapeType,    nEntities;
    double adfBndsMin[4], adfBndsMax[4];
    
    SHPHandle hSHP = SHPOpen(pszShapeFile, "rb" );
    if (hSHP == nullptr) return 0;

    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfBndsMin, adfBndsMax );
    SHPClose  ( hSHP );

    oShapeFileInfo.ShapeType = nShapeType;
    oShapeFileInfo.Entities  = nEntities;
    oShapeFileInfo.BoundMin[0] = adfBndsMin[0];
    oShapeFileInfo.BoundMin[1] = adfBndsMin[1];
    oShapeFileInfo.BoundMin[2] = adfBndsMin[2];
    oShapeFileInfo.BoundMin[3] = adfBndsMin[3];

    oShapeFileInfo.BoundMax[0] = adfBndsMax[0];
    oShapeFileInfo.BoundMax[1] = adfBndsMax[1];
    oShapeFileInfo.BoundMax[2] = adfBndsMax[2];
    oShapeFileInfo.BoundMax[3] = adfBndsMax[3];

    return 1;
}

void ShapeFileReader::ReadShape( const char * pszShapeFile, ShapeFileInfo& oShapeFileInfo )
{
    SHPHandle	hSHP;
    int		    nShapeType, nEntities, i, iPart, bValidate = 1,nInvalidCount=0;
    int         bHeaderOnly = 0;
    const char 	*pszPlus;
    double 	    adfMinBound[4], adfMaxBound[4];
        
    hSHP = SHPOpen( pszShapeFile, "rb" );

    if( hSHP == NULL )
    {
        return;
    }

    FILE *out;
    out = fopen("ShapeFileRead.txt", "w"); // 출력을 저장할 파일을 만들어서 열기

    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );

    fprintf(out, "Shapefile Type: %s   # of Shapes: %d\n\n",
        SHPTypeName( nShapeType ), nEntities );

    fprintf(out, "File Bounds: (%.15g,%.15g,%.15g,%.15g)\n"
        "         to  (%.15g,%.15g,%.15g,%.15g)\n",
        adfMinBound[0], 
        adfMinBound[1], 
        adfMinBound[2], 
        adfMinBound[3], 
        adfMaxBound[0], 
        adfMaxBound[1], 
        adfMaxBound[2], 
        adfMaxBound[3] );

    /* -------------------------------------------------------------------- */
    /*	Skim over the list of shapes, printing all the vertices.	*/
    /* -------------------------------------------------------------------- */
    for( i = 0; i < nEntities && !bHeaderOnly; i++ )
    {
        int		j;
        SHPObject	*psShape;

        psShape = SHPReadObject( hSHP, i );

        if( psShape == NULL )
        {
            break;
        }

        if( psShape->bMeasureIsUsed )
            fprintf(out, "\nShape:%d (%s)  nVertices=%d, nParts=%d\n"
            "  Bounds:(%.15g,%.15g, %.15g, %.15g)\n"
            "      to (%.15g,%.15g, %.15g, %.15g)\n",
            i, SHPTypeName(psShape->nSHPType),
            psShape->nVertices, psShape->nParts,
            psShape->dfXMin, psShape->dfYMin,
            psShape->dfZMin, psShape->dfMMin,
            psShape->dfXMax, psShape->dfYMax,
            psShape->dfZMax, psShape->dfMMax );
        else
            fprintf(out, "\nShape:%d (%s)  nVertices=%d, nParts=%d\n"
            "  Bounds:(%.15g,%.15g, %.15g)\n"
            "      to (%.15g,%.15g, %.15g)\n",
            i, SHPTypeName(psShape->nSHPType),
            psShape->nVertices, psShape->nParts,
            psShape->dfXMin, psShape->dfYMin,
            psShape->dfZMin,
            psShape->dfXMax, psShape->dfYMax,
            psShape->dfZMax );

        if( psShape->nParts > 0 && psShape->panPartStart[0] != 0 )
        {
            fprintf(out, "panPartStart[0] = %d, not zero as expected.\n",
                psShape->panPartStart[0] );
        }

        for( j = 0, iPart = 1; j < psShape->nVertices; j++ )
        {
            const char	*pszPartType = "";

            if( j == 0 && psShape->nParts > 0 )
                pszPartType = SHPPartTypeName( psShape->panPartType[0] );

            if( iPart < psShape->nParts
                && psShape->panPartStart[iPart] == j )
            {
                pszPartType = SHPPartTypeName( psShape->panPartType[iPart] );
                iPart++;
                pszPlus = "+";
            }
            else
                pszPlus = " ";

            if( psShape->bMeasureIsUsed )
                fprintf(out, "   %s (%.15g,%.15g, %.15g, %.15g) %s \n",
                pszPlus,
                psShape->padfX[j],
                psShape->padfY[j],
                psShape->padfZ[j],
                psShape->padfM[j],
                pszPartType );
            else
                fprintf(out, "   %s (%.15g,%.15g, %.15g) %s \n",
                pszPlus,
                psShape->padfX[j],
                psShape->padfY[j],
                psShape->padfZ[j],
                pszPartType );
        }

        if( bValidate )
        {
            int nAltered = SHPRewindObject( hSHP, psShape );

            if( nAltered > 0 )
            {
                fprintf(out, "  %d rings wound in the wrong direction.\n",
                    nAltered );
                nInvalidCount++;
            }
        }

        SHPDestroyObject( psShape );
    }

    SHPClose( hSHP );

    if( bValidate )
    {
        fprintf(out, "%d object has invalid ring orderings.\n", nInvalidCount );
    }

    fclose(out); 
}

int ShapeFileReader::ReadShape( int nIndex, QBicBinary& oBinary, int nBinaryType )
{
    SHPObject* psShape(nullptr);

    try
    {        
        psShape = SHPReadObject( m_hShp, nIndex);

        if (psShape == nullptr)
            throw 0;

        // 안전하게 보정
        SHPRewindObject( m_hShp, psShape );

        if (psShape->bMeasureIsUsed)
            throw -1;

        int nShapeType = psShape->nSHPType;
        if (nShapeType == 1)
        {
            ReadPoint(psShape, oBinary, nBinaryType);
        }
        else if (nShapeType == 3)
        {
            ReadPolyline(psShape, oBinary, nBinaryType);
        }
        else if (nShapeType == 5)
        {
            ReadPolygon(psShape, oBinary, nBinaryType);            
        }
        else
        {
            TxLogDebugException();
            throw -2;
        }
    }
    catch (int& ex)
    {
    	switch (ex)
        {
        case 0:
            break;
        case -1:
            SHPDestroyObject( psShape );
            break;
        case -2:
            SHPDestroyObject( psShape );
            break;
        case -10:
            SHPDestroyObject( psShape );
            break;
        default:
            break;
        }

        CString strCode(_T(""));
        strCode.Format(_T("Exception No : %d"), ex);
        TxLogDebug((LPCTSTR)strCode);
        return ex;
    }
    catch (...)
    {
    	TxLogDebugException();
    }
    
    SHPDestroyObject( psShape );
    return 1;
}

int ShapeFileReader::EntityCount()
{
    return m_oShpInfo.Entities;
}

void ShapeFileReader::ReadPoint( SHPObject* psShape, QBicBinary& oBinary, int nBinaryType )
{    
    double dX = psShape->padfX[0];
    double dY = psShape->padfY[0];

    oBinary.PushBack((int)GeoPointType);
    oBinary.PushBack((BinDouble)dX);
    oBinary.PushBack((BinDouble)dY);
}

void ShapeFileReader::ReadPolyline( SHPObject* psShape, QBicBinary& oBinary, int nBinaryType )
{
    int nPartCount = psShape->nParts;
    if (nPartCount <= 0)
        throw -20;

    int nVertexCount = psShape->nVertices;

    if (nPartCount == 1)
    {
        oBinary.PushBack((int)GeoPolylineType);
        oBinary.PushBack(nVertexCount);

        for (int i=0; i<nVertexCount; i++)
        {
            double dX(0.0);
            double dY(0.0);

            dX = psShape->padfX[i];
            dY = psShape->padfY[i];

            oBinary.PushBack((BinDouble)dX);
            oBinary.PushBack((BinDouble)dY);
        }
    }
    else
    {
        //★ 멀티 파트는 지원하지 않는다.
        //oBinary.PushBack((int)GeoMPolylineType);
        
        oBinary.PushBack((int)GeoPolylineType);
        oBinary.PushBack(nVertexCount);

        for (int i=0; i<nVertexCount; i++)
        {
            double dX(0.0);
            double dY(0.0);

            dX = psShape->padfX[i];
            dY = psShape->padfY[i];

            oBinary.PushBack((BinDouble)dX);
            oBinary.PushBack((BinDouble)dY);
        }
        
        std::map<int, std::vector<TxPoint>> mapPartPt;
        for (int i=0, nPart=0; i<nVertexCount; i++) {
            int nEnd(0);
            if (nPartCount > 1 && nPart < nPartCount - 1) {
                nEnd = psShape->panPartStart[nPart + 1];
            }
            else {
                nEnd = nVertexCount; //한개 파트로 구성
            }

            double dX(0.0);
            double dY(0.0);

            dX = (double)psShape->padfX[i];
            dY = (double)psShape->padfY[i];

            std::vector<TxPoint>& vecPoint = mapPartPt[nPart];
            vecPoint.push_back(TxPoint(dX, dY));

            if (nEnd -1 == i) {//마지막 포인트
                nPart++;
            }
        }

        ConvertClosewiseRing(mapPartPt);

        //★ 시작하는 파트를 선택한다.
        int nStart(-1), nEnd(-1);        
        OutLineInfo(mapPartPt, nStart, nEnd);

        if (nStart == -1) {
            nStart = 0;
        }

        std::vector<TxPoint> vecPoint; {
            auto iFind = mapPartPt.find(nStart);
            vecPoint = iFind->second;
            mapPartPt.erase(iFind);
        }

        std::vector<TxPoint> vecEnd; {
            auto iFind = mapPartPt.find(nEnd);
            if (iFind != mapPartPt.end()) {
                vecEnd = iFind->second;
                mapPartPt.erase(iFind);
            }            
        }

        int nLoop(0);
        while(nLoop < nPartCount && mapPartPt.size() != 0) 
        {
            nLoop++;

            TxPoint& oFrom = vecPoint[0];
            TxPoint& oTo   = vecPoint[vecPoint.size()-1];

            AppendLine(oFrom, oTo, mapPartPt, vecPoint);
        }
        
        if (mapPartPt.size() == 0) {
            //★ 마지막 추가
            if (vecEnd.size() > 0) 
            {
                TxPoint& oFrom = vecPoint[0];
                TxPoint& oTo   = vecPoint[vecPoint.size()-1];

                std::map<int, std::vector<TxPoint>> mapAddPart;
                mapAddPart[0] = vecEnd;
                AppendLine(oFrom, oTo, mapAddPart, vecPoint);
            }
            
            oBinary.Position(0);
            oBinary.PushBack((int)GeoPolylineType);
            oBinary.PushBack((int)vecPoint.size());

            for (size_t i=0; i<vecPoint.size(); i++) {
                TxPoint& oPt = vecPoint[i];
                oBinary.PushBack((BinDouble)oPt.X);
                oBinary.PushBack((BinDouble)oPt.Y);
            }
        }        
    }
}


void ShapeFileReader::OutLineInfo( std::map<int, std::vector<TxPoint>>& mapPartPt, int& nStart, int& nEnd)
{
    double dEipsilon(0.0000001);
    std::map<int, int> mapFromKeyCnt;
    std::map<int, int> mapToKeyCnt;

    for (auto iter = mapPartPt.begin(); iter != mapPartPt.end(); ++iter) {
        int nKey = iter->first; { //초기화
            mapFromKeyCnt[nKey];
            mapToKeyCnt[nKey];
        }

        std::vector<TxPoint>& vecPart = iter->second;
        TxPoint& oFrom = vecPart[0];
        TxPoint& oTo   = vecPart[vecPart.size()-1];
        
        for (auto iter2 = mapPartPt.begin(); iter2 != mapPartPt.end(); ++iter2) {
            if (iter == iter2)
                continue;

            std::vector<TxPoint>& vecPt = iter2->second;
            TxPoint& oPartFrom = vecPt[0];
            TxPoint& oPartTo   = vecPt[vecPt.size()-1];

            double dDist1 = TxMath::Dist(oFrom, oPartFrom);
            double dDist2 = TxMath::Dist(oFrom, oPartTo);
                        
            if (dDist1 < dEipsilon || dDist2 < dEipsilon) {
                int& nCnt = mapFromKeyCnt[nKey];
                nCnt++;
            } 

            dDist1 = TxMath::Dist(oTo, oPartFrom);
            dDist2 = TxMath::Dist(oTo, oPartTo);

            if (dDist1 < dEipsilon || dDist2 < dEipsilon) {
                int& nCnt = mapToKeyCnt[nKey];
                nCnt++;
            } 
        }
    }
        
    nStart = -1;
    nEnd   = -1;

    std::set<int> setZero;
    for (auto iter = mapFromKeyCnt.begin(); iter != mapFromKeyCnt.end(); ++iter) {
        int nKey = iter->first;
        int nCnt = iter->second;

        if (nCnt == 0) {
            setZero.insert(nKey);

            nStart = nKey;
        }
    }

    for (auto iter = mapToKeyCnt.begin(); iter != mapToKeyCnt.end(); ++iter) {
        int nKey = iter->first;
        int nCnt = iter->second;

        if (nCnt == 0) {
            setZero.insert(nKey);
        }
    }
       
    if (setZero.size() == 0) {
        return;
    }

    for (auto iter = setZero.begin(); iter != setZero.end(); ++iter) {
        int nKey = *iter;
        if (nStart < 0) {
            nStart = nKey;
            continue;
        }

        if (nStart != nKey)
            nEnd = nKey;
    }
}


int ShapeFileReader::NeighborLine( TxPoint& oPt, std::map<int, std::vector<TxPoint>>& mapPartPt, double& dDist, int& nFromToFlag )
{
    nFromToFlag = 0;
    int nR(-1);
    double dMinDist(9999999999);
    for (auto iter = mapPartPt.begin(); iter != mapPartPt.end(); ++iter) 
    {
        std::vector<TxPoint>& vecPart = iter->second;
        TxPoint& oPartFrom = vecPart[0];
        TxPoint& oPartTo   = vecPart[vecPart.size()-1];
        
        double dFromDist = TxMath::Dist(oPt, oPartFrom);
        double dToDist   = TxMath::Dist(oPt, oPartTo);

        if (dFromDist <= dMinDist) {
            dMinDist = dFromDist;
            nFromToFlag = 0;
            nR = iter->first;
        }

        if (dToDist <= dMinDist) {
            dMinDist = dToDist;
            nFromToFlag = 1;
            nR = iter->first;
        }
                
        if (dFromDist == 0 && dToDist == 0) {
            dMinDist = dToDist;
            nFromToFlag = 2;
            nR = iter->first;
        }
    }
    
    dDist = dMinDist;
    return nR;
}

void ShapeFileReader::AppendLine(TxPoint& oFrom, TxPoint& oTo, std::map<int, std::vector<TxPoint>>& mapPartPt, std::vector<TxPoint>& vecPoint)
{
    int    nFIdx(-1), nFromFixFlag(0), nTIdx(-1), nToFixFlag(0);
    double dFromDist(0), dToDist(0);

    nFIdx = NeighborLine(oFrom, mapPartPt, dFromDist, nFromFixFlag);
    nTIdx = NeighborLine(oTo,   mapPartPt, dToDist,   nToFixFlag);
    
    //★ 동일 거리 시 -> To
    if (dFromDist < dToDist) {
        auto iFind = mapPartPt.find(nFIdx);
        std::vector<TxPoint>& vecPart = iFind->second;

        std::vector<TxPoint> vecTemp;                
        if (nFromFixFlag == 0) 
        {//★ 기준시점 - 어떤 라인의 시점과 가까이 있다.
            for (auto iter = vecPart.rbegin(); iter != vecPart.rend(); ++iter) {
                vecTemp.push_back(*iter);
            }
        }
        else if (nFromFixFlag == 1) 
        {//★ 기준시점 - 어떤 라인의 시점과 가까이 있다.
            vecTemp = vecPart;
        }
        else 
        {
            vecTemp = vecPart;
        }

        for (size_t i=0; i<vecPoint.size(); i++) {
            vecTemp.push_back(vecPoint[i]);
        }

        vecPoint = vecTemp;
        mapPartPt.erase(iFind);
    }
    else 
    {
        auto iFind = mapPartPt.find(nTIdx);
        std::vector<TxPoint>& vecPart = iFind->second;

        std::vector<TxPoint> vecTemp;

        if (nToFixFlag == 0) {
            vecTemp = vecPart;
        }
        else if (nToFixFlag == 1) {
            for (auto iter = vecPart.rbegin(); iter != vecPart.rend(); ++iter) {
                vecTemp.push_back(*iter);
            }
        }
        else {
            //★ 시계 방향
            vecTemp = vecPart;

            //★ 반시계방향
            //for (auto iter = vecPart.rbegin(); iter != vecPart.rend(); ++iter) {
            //    vecTemp.push_back(*iter);
            //}
        }

        for (size_t i=0; i<vecTemp.size(); i++) {
            vecPoint.push_back(vecTemp[i]);
        }
        mapPartPt.erase(iFind);
    }
}

void ShapeFileReader::ConvertClosewiseRing( std::map<int, std::vector<TxPoint>>& mapPartPt )
{
    double dEipsilon(0.0000001);

    for (auto iter = mapPartPt.begin(); iter != mapPartPt.end(); ++iter) 
    {
        std::vector<TxPoint>& vecPart = iter->second;
        TxPoint& oFrom = vecPart[0];
        TxPoint& oTo   = vecPart[vecPart.size()-1];

        double   dDist = TxMath::Dist(oFrom, oTo);
        if (dDist < dEipsilon && vecPart.size() >= 3 ) {
            TxPolygon oPolygon(vecPart);
            // 시계방향 회전
            vecPart = oPolygon.OutRing();
        }
    }
}

void ShapeFileReader::ReadPolygon( SHPObject* psShape, QBicBinary& oBinary, int nBinaryType )
{
    // 파트 개수
    int nPartCount = psShape->nParts;
    if (nPartCount <= 0)
        throw -20;

    // 전체 버텍스 카운트
    int nVertexCount = psShape->nVertices;

    std::vector<std::vector<TxPoint>> vecPolyPoint;
    std::vector<TxPoint> vecPoint;

    for (int i=0, nPart=0; i<nVertexCount; i++) {
        int nEnd(0);
        if (nPartCount > 1 && nPart < nPartCount - 1) {
            nEnd = psShape->panPartStart[nPart + 1];
        }
        else {
            nEnd = nVertexCount; //한개 파트로 구성
        }

        double dX(0.0);
        double dY(0.0);

        dX = (double)psShape->padfX[i];
        dY = (double)psShape->padfY[i];

        vecPoint.push_back(TxPoint(dX, dY));

        if (nEnd -1 == i) {//마지막 포인트
            nPart++;

            vecPolyPoint.push_back(vecPoint);
            vecPoint.clear();
        }
    }
    
    std::vector<TxPolygon> vecSimplePolygon;
    for (size_t i=0; i<vecPolyPoint.size(); i++)
    {        
        const std::vector<TxPoint>& vecPoint = vecPolyPoint[i];

        if (vecPoint.size() > 3) {
            TxPolygon oSimplePoly;
            oSimplePoly.OutRing(vecPoint);
            vecSimplePolygon.push_back(oSimplePoly);
        }        
    }

    TxMPolygon oPolygon;     
    for (size_t i=0; i<vecSimplePolygon.size(); i++)
    {
        TxPolygon& oSimplePoly = vecSimplePolygon[i];
        TxEnvelope oEnvelope   = oSimplePoly.Envelope();

        for (size_t j=i+1; j<vecSimplePolygon.size(); j++) {            
            try
            {
                TxPolygon  oNxtPoly     = vecSimplePolygon[j];
                TxEnvelope oNxtEnvelope = oNxtPoly.Envelope();

                if(oEnvelope.Contains(oNxtEnvelope) == true) { //Hole 체크
                    std::vector<TxPoint> vecPoint = oNxtPoly.OutRing();
                    
                    //★ 2016-04-06 홀 체크시 break 문을 두번 사용해야 함 
                    bool isHole(true);
                    for (size_t m=0; m<vecPoint.size(); m++) {
                        if ( oSimplePoly.IsInPoint(vecPoint[m]) == false ) {
                            isHole = false;
                            break; //★                        
                        }
                    }

                    if (isHole == false) {
                        break; //★ 
                    }

                    // Hole 이면 Hole 정보를 추가
                    oSimplePoly.AddHolePoint(vecPoint);                    
                    i = j; /* 상위에서 i++ */
                }
                else {
                    break;
                }
            }    
            catch (...)
            {
                TxLogDebugException();
                throw -5;
            }
        }

        oPolygon.AddSubPolygon(oSimplePoly);
    }
    
    if (oPolygon.SubPolygonCount() == 1) {
        TxPolygon* pSimplePolygon = oPolygon.SubPolygon(0);
        pSimplePolygon->GetBinary(oBinary);
    }
    else if ( oPolygon.SubPolygonCount() > 1) {
        oPolygon.GetBinary(oBinary);
    }
    else {        
    }
}

int ShapeFileReader::RecordCount()
{
    if (m_hDBF == nullptr)
        throw 1;

    int nRecordCount = DBFGetRecordCount(m_hDBF);
    return nRecordCount;
}

int ShapeFileReader::FieldCount()
{
    if (m_hDBF == nullptr)
        throw 1;

    int nFieldCount = DBFGetFieldCount(m_hDBF);
    return nFieldCount;
}

std::vector<DbfFieldInfo> ShapeFileReader::FieldInfo()
{
    if (m_hDBF == nullptr)
        throw 1;

    std::vector<DbfFieldInfo> vecFieldInfo;
    int nFieldCount = DBFGetFieldCount(m_hDBF);
    for (int i=0; i<nFieldCount; i++) {
        char szFieldName[12] = {};
        int  nWidth(0);
        int  nDecimals(0);

        DBFFieldType eType;
        try {
            eType = DBFGetFieldInfo(m_hDBF, i, szFieldName, &nWidth, &nDecimals); 
        }
        catch (...) {
            throw 2;
        }

        DbfFieldInfo oFieldInfo;
        oFieldInfo.m_strFieldName = szFieldName;
        oFieldInfo.m_nFiledType   = (int)eType;
        oFieldInfo.m_nWidth       = nWidth;
        oFieldInfo.m_nDecimals    = nDecimals;

        vecFieldInfo.push_back(oFieldInfo);
    }

    return vecFieldInfo;
}

std::vector<DbfFieldValue> ShapeFileReader::ReadRecord( int nRecord )
{
    if (m_hDBF == nullptr)
        throw 1;

    std::vector<DbfFieldValue> vecValue;

    int nFieldCount = DBFGetFieldCount(m_hDBF);        
    for (int i=0; i<nFieldCount; i++) {
        const char* pValue = DBFReadStringAttribute(m_hDBF, nRecord, i);
        
        DbfFieldValue oValue;
        oValue.m_nFieldIndex = i;

        if (pValue != nullptr) {
			if (0 == m_nEncoding) {
				oValue.m_strValue = CString(pValue);
			} else {
				oValue.m_strValue = QBicTextEncoding::UTF8ToWChar(pValue);
			}
        } 

        vecValue.push_back(oValue);
    }

    return vecValue;
}


