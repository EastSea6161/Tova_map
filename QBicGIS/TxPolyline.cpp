#include "stdafx.h"
#include "TxPolyline.h"
#include "TxMath.h"
#include "TxVector.h"
#include "TxConst.h"

TxPolyline::TxPolyline() : ITxGeometry(GeoPolylineType), m_pPointsX(nullptr), m_pPointsY(nullptr)
{
    m_nxPointCnt = 0;

    delete[] m_pPointsX;
    delete[] m_pPointsY;
}

TxPolyline::TxPolyline( const TxPolyline& right ) : ITxGeometry(GeoPolylineType), m_pPointsX(nullptr), m_pPointsY(nullptr)
{
    if (right.m_nxPointCnt > 0)
    {
        size_t size = sizeof(double) * right.m_nxPointCnt;

        m_pPointsX = new double[right.m_nxPointCnt];
        m_pPointsY = new double[right.m_nxPointCnt];
        
        memcpy((void*)m_pPointsX, (void*)right.m_pPointsX, size);
        memcpy((void*)m_pPointsY, (void*)right.m_pPointsY, size);
    }

    m_nxPointCnt = right.m_nxPointCnt;
    m_oEnvelope  = right.m_oEnvelope;
}

TxPolyline::TxPolyline( QBicBinary& oBinary, size_t nIndex ) : ITxGeometry(GeoPolylineType), m_pPointsX(nullptr), m_pPointsY(nullptr)
{
    int nGeometryType(0);
    oBinary.Position(nIndex);
    oBinary.Read(nGeometryType);
    
    int nCount(0);
    oBinary.Read(nCount);

    std::vector<TxPoint> vecPoint;
    for (int i=0; i<nCount; i++)
    {
        BinDouble dX(0);
        BinDouble dY(0);

        oBinary.Read(dX);
        oBinary.Read(dY);

        vecPoint.push_back(TxPoint((double)dX, (double)dY));
    }

    SetPoints(vecPoint);
}

TxPolyline::TxPolyline( const std::vector<TxPoint>& vecPoint ) : ITxGeometry(GeoPolylineType), m_pPointsX(nullptr), m_pPointsY(nullptr)
{
    SetPoints(vecPoint);
}

TxPolyline::TxPolyline( const std::vector<TxPointPtr>& vecPoint ) : ITxGeometry(GeoPolylineType), m_pPointsX(nullptr), m_pPointsY(nullptr)
{
    delete[] m_pPointsX;
    delete[] m_pPointsY;

    size_t nxCount = vecPoint.size();

    m_nxPointCnt = nxCount;
    if (nxCount > 0) {
        m_pPointsX = new double[nxCount];
        m_pPointsY = new double[nxCount];
    }
    else {
        m_pPointsX = nullptr;
        m_pPointsY = nullptr;
    }
    
    for (size_t i=0; i<nxCount; i++) {
        TxPointPtr spPoint = vecPoint[i];
        m_pPointsX[i] = spPoint->X;
        m_pPointsY[i] = spPoint->Y;
    }

    ResetEnvelope();
}

TxPolyline::TxPolyline( std::shared_ptr<TxPolyline> spPLine ) : ITxGeometry(GeoPolylineType), m_pPointsX(nullptr), m_pPointsY(nullptr)
{
    SetPoints(spPLine->GetPoints());
}

TxPolyline::TxPolyline( TxPoint oFrom, TxPoint oTo ) : ITxGeometry(GeoPolylineType), m_pPointsX(nullptr), m_pPointsY(nullptr)
{
    std::vector<TxPoint> vecPoint; {
        vecPoint.push_back(oFrom);
        vecPoint.push_back(oTo);
    }    

    SetPoints(vecPoint);
}

TxPolyline::TxPolyline( TxPointPtr spFrom, TxPointPtr spTo ) : ITxGeometry(GeoPolylineType), m_pPointsX(nullptr), m_pPointsY(nullptr)
{
    std::vector<TxPointPtr> vecPoint; {
        vecPoint.push_back(spFrom);
        vecPoint.push_back(spTo);
    }

    delete[] m_pPointsX;
    delete[] m_pPointsY;

    size_t nxCount = vecPoint.size();

    m_nxPointCnt = nxCount;
    if (nxCount > 0) {
        m_pPointsX = new double[nxCount];
        m_pPointsY = new double[nxCount];
    }
    else {
        m_pPointsX = nullptr;
        m_pPointsY = nullptr;
    }

    for (size_t i=0; i<nxCount; i++) {
        TxPointPtr spPoint = vecPoint[i];
        m_pPointsX[i] = spPoint->X;
        m_pPointsY[i] = spPoint->Y;
    }

    ResetEnvelope();
}

TxPolyline::~TxPolyline()
{
    delete[] m_pPointsX;
    delete[] m_pPointsY;
}

TxPolyline& TxPolyline::operator=( const TxPolyline& right )
{
    if (&right == this)
        return *this;

    delete[] m_pPointsX;
    delete[] m_pPointsY;

    if (right.m_nxPointCnt > 0)
    {
        size_t size = sizeof(double) * right.m_nxPointCnt;
        m_pPointsX = new double[right.m_nxPointCnt];
        m_pPointsY = new double[right.m_nxPointCnt];
        memcpy((void*)m_pPointsX, (void*)right.m_pPointsX, size);
        memcpy((void*)m_pPointsY, (void*)right.m_pPointsY, size);
    }
    else
    {
        m_pPointsX = nullptr;
        m_pPointsY = nullptr;
    }

    m_nxPointCnt = right.m_nxPointCnt;
    m_oEnvelope  = right.m_oEnvelope;

    return *this;
}

TxEnvelope TxPolyline::Envelope() const
{
    return m_oEnvelope;
}

void TxPolyline::ResetEnvelope()
{
    if (m_nxPointCnt == 0) {
        m_oEnvelope.SetEnvelope(0, 0, 0, 0);
        return;
    }

    double dMinX( m_pPointsX[0]);
    double dMinY( m_pPointsY[0]);
    double dMaxX( m_pPointsX[0]);
    double dMaxY( m_pPointsY[0]);

    for (size_t i = 1; i < m_nxPointCnt; i++) {        
        double dX = m_pPointsX[i];
        double dY = m_pPointsY[i];

        if (dMinX > dX) 
            dMinX = dX;

        if (dMinY > dY) 
            dMinY = dY;

        if (dMaxX < dX)
            dMaxX = dX;

        if (dMaxY < dY)
            dMaxY = dY;        
    }

    m_oEnvelope.SetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
}

void TxPolyline::SetPoints( const std::vector<TxPoint>& vecPoint )
{   
    delete[] m_pPointsX;
    delete[] m_pPointsY;

    size_t nxCount = vecPoint.size(); {
        m_nxPointCnt = nxCount;
    }
    
    if (nxCount > 0) {
        m_pPointsX = new double[nxCount];
        m_pPointsY = new double[nxCount];
    }
    else {
        m_pPointsX = nullptr;
        m_pPointsY = nullptr;
    }
    
    for (size_t i=0; i<m_nxPointCnt; i++) {
        const TxPoint& oPt = vecPoint[i];
        m_pPointsX[i] = oPt.X;
        m_pPointsY[i] = oPt.Y;
    }

    ResetEnvelope();
}

void TxPolyline::GetBinary( QBicBinary& oBinary )
{
    oBinary.PushBack((int)m_enGeometryType);

    int nSize = (int)m_nxPointCnt;
    oBinary.PushBack(nSize);

    for (size_t i=0; i<m_nxPointCnt; i++)
    {        
        oBinary.PushBack((BinDouble)m_pPointsX[i]);
        oBinary.PushBack((BinDouble)m_pPointsY[i]);
    }
}

CString TxPolyline::ToString()
{
    CString strOut(_T(""));
    strOut.Format(_T("\nTxPolyline : #0) Vertex(%d)"), m_nxPointCnt);
    for(size_t i=0; i<m_nxPointCnt; i++) {
        if (i % 5 == 0)
            strOut.Append(_T("\n"));

        strOut.AppendFormat(_T(" %s "), TxPoint((double)m_pPointsX[i], (double)m_pPointsY[i]).ToString());        
    }

    return strOut;
}


bool TxPolyline::Intersect( const TxEnvelope& _oEnvelope ) const
{
    if (m_nxPointCnt == 0)
        return false;

    if (m_nxPointCnt == 1) {
        return _oEnvelope.IsInPoint(TxPoint(m_pPointsX[0], m_pPointsY[0]));
    }

    double dMinX, dMinY, dMaxX, dMaxY;
    _oEnvelope.GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
    for (size_t i=0; i<m_nxPointCnt-1; i++)
    {
        TxPoint oFromPt(m_pPointsX[i],   m_pPointsY[i]);
        TxPoint oToPt  (m_pPointsX[i+1], m_pPointsY[i+1]);

        if (_oEnvelope.IsInPoint(oFromPt) == true)
            return true;

        TxPoint oIP;
        bool bIntersect(false);
        bIntersect = TxMath::GetIntersectPoint(oFromPt, oToPt, TxPoint(dMinX, dMinY), TxPoint(dMinX, dMaxY), oIP, false);
        if (bIntersect == true) return true;
        bIntersect = TxMath::GetIntersectPoint(oFromPt, oToPt, TxPoint(dMinX, dMaxY), TxPoint(dMaxX, dMaxY), oIP, false);
        if (bIntersect == true) return true;

        bIntersect = TxMath::GetIntersectPoint(oFromPt, oToPt, TxPoint(dMaxX, dMaxY), TxPoint(dMaxX, dMinY), oIP, false);
        if (bIntersect == true) return true;
        bIntersect = TxMath::GetIntersectPoint(oFromPt, oToPt, TxPoint(dMinX, dMinY), TxPoint(dMaxX, dMinY), oIP, false);
        if (bIntersect == true) return true;
    }

    return false;
}


bool TxPolyline::Intersect( TxEnvelopePtr spEnv ) const
{
    if (m_nxPointCnt == 0)
        return false;

    if (m_nxPointCnt == 1) {
        return spEnv->IsInPoint(TxPoint(m_pPointsX[0], m_pPointsY[0]));
    }

    double dMinX, dMinY, dMaxX, dMaxY;
    spEnv->GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
    for (size_t i=0; i<m_nxPointCnt-1; i++)
    {
        TxPoint oFromPt(m_pPointsX[i],   m_pPointsY[i]);
        TxPoint oToPt  (m_pPointsX[i+1], m_pPointsY[i+1]);

        if (spEnv->IsInPoint(oFromPt) == true)
            return true;

        TxPoint oIP;
        bool bIntersect(false);
        bIntersect = TxMath::GetIntersectPoint(oFromPt, oToPt, TxPoint(dMinX, dMinY), TxPoint(dMinX, dMaxY), oIP, false);
        if (bIntersect == true) return true;
        bIntersect = TxMath::GetIntersectPoint(oFromPt, oToPt, TxPoint(dMinX, dMaxY), TxPoint(dMaxX, dMaxY), oIP, false);
        if (bIntersect == true) return true;

        bIntersect = TxMath::GetIntersectPoint(oFromPt, oToPt, TxPoint(dMaxX, dMaxY), TxPoint(dMaxX, dMinY), oIP, false);
        if (bIntersect == true) return true;
        bIntersect = TxMath::GetIntersectPoint(oFromPt, oToPt, TxPoint(dMinX, dMinY), TxPoint(dMaxX, dMinY), oIP, false);
        if (bIntersect == true) return true;
    }

    return false;
}

bool TxPolyline::Intersect( const TxPolyline& rOther ) const
{
    size_t nxCountA  = m_nxPointCnt;
    size_t nxCountB  = rOther.m_nxPointCnt;

    //★ 라인위의 한점은 교차한다고 인정하지 않는다. 
    //★ GetIntersectPoint(점1,점1, 점2, 점3) -> 평행으로 인식
    //★ 동일한 두 라인은 평행하기 때문에 교차 -> False
    if (nxCountA <= 1 || nxCountB <= 1) {        
        return false;
    }

    for (size_t i=0; i<nxCountA-1; i++)
    {
        TxPoint oAFrom(m_pPointsX[i],   m_pPointsY[i]);
        TxPoint oATo  (m_pPointsX[i+1], m_pPointsY[i+1]);

        for (size_t j=0; j<nxCountB-1; j++)
        {
            TxPoint oBFrom(rOther.m_pPointsX[j],   rOther.m_pPointsY[j]);
            TxPoint oBTo  (rOther.m_pPointsX[j+1], rOther.m_pPointsY[j+1]);

            TxPoint oIP;
            bool bIntersect = TxMath::GetIntersectPoint(oAFrom, oATo, oBFrom, oBTo, oIP, false);

            if (bIntersect)
                return true;
        }
    }

    return false;
}


bool TxPolyline::Intersect( std::shared_ptr<TxPolyline> spLine ) const
{
    size_t nxCountA  = m_nxPointCnt;
    size_t nxCountB  = spLine->m_nxPointCnt;

    //★ 라인위의 한점은 교차한다고 인정하지 않는다. 
    //★ GetIntersectPoint(점1,점1, 점2, 점3) -> 평행으로 인식
    //★ 동일한 두 라인은 평행하기 때문에 교차 -> False
    if (nxCountA <= 1 || nxCountB <= 1) {        
        return false;
    }
    
    if ( m_oEnvelope.Intersect(spLine->Envelope()) == false )
        return false;
        
    for (size_t i=0; i<nxCountA-1; i++)
    {
        TxPoint oAFrom(m_pPointsX[i],   m_pPointsY[i]);
        TxPoint oATo  (m_pPointsX[i+1], m_pPointsY[i+1]);

        for (size_t j=0; j<nxCountB-1; j++)
        {
            TxPoint oBFrom(spLine->m_pPointsX[j],   spLine->m_pPointsY[j]);
            TxPoint oBTo  (spLine->m_pPointsX[j+1], spLine->m_pPointsY[j+1]);

            TxPoint oIP;
            bool bIntersect = TxMath::GetIntersectPoint(oAFrom, oATo, oBFrom, oBTo, oIP, false);

            if (bIntersect)
                return true;
        }
    }

    return false;
}

std::vector<TxPoint> TxPolyline::GetPoints() const
{
    std::vector<TxPoint> vecPoint;
    for(size_t i=0; i<m_nxPointCnt; i++) {
        vecPoint.push_back(TxPoint((double)m_pPointsX[i], (double)m_pPointsY[i]));
    }

    return vecPoint;
}

std::vector<TxPoint> TxPolyline::GetPoints( double dMinVertexDist ) const
{
    std::vector<TxPoint> vecPoint;
    
    if ( m_nxPointCnt <= 2)
    {
        for(size_t i=0; i<m_nxPointCnt; i++) {
            vecPoint.push_back(TxPoint((double)m_pPointsX[i], (double)m_pPointsY[i]));
        }

        return vecPoint;
    }
    else if ( m_nxPointCnt == 3) {
        TxPoint oFirstPoint((double)m_pPointsX[0], (double)m_pPointsY[0]);
        vecPoint.push_back(oFirstPoint);

        TxPoint oNxtPoint(m_pPointsX[1], m_pPointsY[1]);
        double dDist = TxMath::Dist(oFirstPoint, oNxtPoint);
        if (dDist >= dMinVertexDist) 
            vecPoint.push_back(oNxtPoint);

        vecPoint.push_back(TxPoint(m_pPointsX[m_nxPointCnt-1], m_pPointsY[m_nxPointCnt-1]));
    }
    else {
        TxPoint oFirstPoint((double)m_pPointsX[0], (double)m_pPointsY[0]);
        vecPoint.push_back(oFirstPoint);

        for(size_t i=1; i<m_nxPointCnt-1; i++)
        {
            TxPoint oNxtPoint(m_pPointsX[i], m_pPointsY[i]);
            double dDist = TxMath::Dist(oFirstPoint, oNxtPoint);
            if (dDist >= dMinVertexDist) {
                vecPoint.push_back(oNxtPoint);
                oFirstPoint = oNxtPoint;
            }
        }
        vecPoint.push_back(TxPoint(m_pPointsX[m_nxPointCnt-1], m_pPointsY[m_nxPointCnt-1]));
    }
        
    return vecPoint;
}


void TxPolyline::Flip()
{
    if (m_nxPointCnt <= 1)
        return;

    size_t nCount = m_nxPointCnt / 2;
    for (size_t i=0; i<nCount; i++)
    {
        double dX = m_pPointsX[i];
        double dY = m_pPointsY[i];

        size_t nIndex = m_nxPointCnt - (i+1);
        m_pPointsX[i] = m_pPointsX[nIndex];
        m_pPointsY[i] = m_pPointsY[nIndex];

        m_pPointsX[nIndex] = dX;
        m_pPointsY[nIndex] = dY;
    }
}

std::vector<TxPoint> TxPolyline::GetOffsetPoint( double dOffset ) const
{
    if (abs(dOffset) <= TxConst::Epsilon())
        return GetPoints();

    std::vector<TxPoint> vecOffsetPoint;

    size_t nxCount = m_nxPointCnt;
    if (nxCount == 0) {        
        return vecOffsetPoint;
    }
    else if (nxCount <= 1) {
        vecOffsetPoint.push_back(TxPoint(m_pPointsX[0], m_pPointsY[0]));
        return vecOffsetPoint;
    }
    else if (nxCount == 2)
    {
        TxPoint oFrom(m_pPointsX[0], m_pPointsY[0]);
        TxPoint oTo  (m_pPointsX[1], m_pPointsY[1]);

        std::vector<TxPoint> vecPoint = TxMath::GetLineOffsetPoints(oFrom, oTo, dOffset);
        return vecPoint;
    }

    // 폴리라인을 구성하는 버텍스를 Offset
    struct TLine
    {
        TxPoint m_oFrom;
        TxPoint m_oTo;
    };

    std::vector<TLine> vecLine;
    for (size_t i=0; i<nxCount-1; i++)
    {
        TxPoint oFrom(m_pPointsX[i],   m_pPointsY[i])  ;
        TxPoint oTo  (m_pPointsX[i+1], m_pPointsY[i+1]);

        std::vector<TxPoint> vecPoint = TxMath::GetLineOffsetPoints(oFrom, oTo, dOffset);

        TLine oLine;
        oLine.m_oFrom = vecPoint[0];
        oLine.m_oTo   = vecPoint[1];

        vecLine.push_back(oLine);
    }

    nxCount = vecLine.size();

    // 절점이 두개인 경우는 바로 리턴
    if (nxCount == 1)
    {
        TLine& oLine  = vecLine[0];
        vecOffsetPoint.push_back(oLine.m_oFrom);
        vecOffsetPoint.push_back(oLine.m_oTo);

        return vecOffsetPoint;
    }

    // 교차체크 - 교차하면 무조건 통과 
    // 교차되지 않는 경우 
    //  'ㅗ' 모양 : 과도하게 Offset된 경우
    //  'ㅜ' 모양 : 기본적으로 만날 수 없다.

    vecOffsetPoint.push_back(vecLine[0].m_oFrom);

    for (size_t i=0; i<nxCount-1; i++)
    {
        TLine& oFirstLine  = vecLine[i];
        TLine& oSecondLine = vecLine[i+1];

        TxPoint oIP;
        int nIntersectType = TxMath::IntersectPoint(oFirstLine.m_oFrom, oFirstLine.m_oTo, oSecondLine.m_oTo, oSecondLine.m_oFrom, oIP);        
        // 두직선의 교점 : 9(평행), 0(교차), 1(두직선 모두 진행방향에서 교차), -1(두직선 모두 역방향에서 교차), 2
        if (nIntersectType == 0)
        {//교차
            vecOffsetPoint.push_back(oIP);
        }
        else if (nIntersectType == 1)
        {//연장선에서 교차한다
            // 각도가 날카로운 경우(점간의 간격이 짧게 엇갈린격우) 지나치게 뾰족하게 표현되어... 
            // vecOffsetPoint.push_back(oIP); -> 수정
            vecOffsetPoint.push_back(oFirstLine.m_oTo);
            vecOffsetPoint.push_back(oSecondLine.m_oFrom);
        }        
    }

    // 마지막점 추가
    vecOffsetPoint.push_back(vecLine[nxCount-1].m_oTo);

    return vecOffsetPoint;
}

std::vector<TxPoint> TxPolyline::GetOffsetPoint( double dOffset, double dMinDist ) const
{
    if (abs(dOffset) <= TxConst::Epsilon())
        return GetPoints();

    std::vector<TxPoint> vecOffsetPoint;
    size_t nxCount = m_nxPointCnt;
    if (nxCount == 0) {
        return vecOffsetPoint;
    }
    else if (nxCount <= 1) {
        vecOffsetPoint.push_back(TxPoint(m_pPointsX[0], m_pPointsY[0]));
        return vecOffsetPoint;
    }
    else if (nxCount == 2) {
        TxPoint oFrom(m_pPointsX[0], m_pPointsY[0]);
        TxPoint oTo  (m_pPointsX[1], m_pPointsY[1]);

        std::vector<TxPoint> vecPoint = TxMath::GetLineOffsetPoints(oFrom, oTo, dOffset);
        return vecPoint;
    }

    std::vector<TxPoint> vecMiddlePoint;

    // 3개 이상의 포인트 처리
    size_t  nxEnd = nxCount -1;
    TxPoint oStart(m_pPointsX[0],     m_pPointsY[0]);
    TxPoint oEnd  (m_pPointsX[nxEnd], m_pPointsY[nxEnd]);
    
    TxPoint oAnchor = oStart;
    for (size_t i=1; i<nxEnd; i++) 
    {
        TxPoint oTo  (m_pPointsX[i], m_pPointsY[i]);
        if ( TxMath::Dist(oAnchor, oTo) > dMinDist ) {
            if (i == nxEnd-1)
            {
                if (TxMath::Dist(oTo, oEnd) > dMinDist) 
                    vecMiddlePoint.push_back(oTo);
            }
            else {
                vecMiddlePoint.push_back(oTo);
            }
            
            oAnchor = oTo;
        }
    }
    
    size_t nxMiddleCount = vecMiddlePoint.size();
    if (nxMiddleCount == 0) {
        std::vector<TxPoint> vecPoint = TxMath::GetLineOffsetPoints(oStart, oEnd, dOffset);
        return vecPoint;
    }

    // 최종적으로 남은 절점이 3개 이상이다.
    struct TLine {
        TxPoint m_oFrom;
        TxPoint m_oTo;
    };

    std::vector<TLine> vecLine;

    oAnchor = oStart;
    for (size_t i=0; i<nxMiddleCount; i++)
    {
        std::vector<TxPoint> vecPoint = TxMath::GetLineOffsetPoints(oAnchor, vecMiddlePoint[i], dOffset);

        TLine oLine;
        oLine.m_oFrom = vecPoint[0];
        oLine.m_oTo   = vecPoint[1];

        vecLine.push_back(oLine);
        oAnchor = vecMiddlePoint[i];
    }

    // 마지막 라인 추가
    {
        std::vector<TxPoint> vecPoint = TxMath::GetLineOffsetPoints(oAnchor, oEnd, dOffset);
        TLine oLine;
        oLine.m_oFrom = vecPoint[0];
        oLine.m_oTo   = vecPoint[1];

        vecLine.push_back(oLine);
    }
    
    nxCount = vecLine.size();
    
    // 교차체크 - 교차하면 무조건 통과 
    // 교차되지 않는 경우 
    //  'ㅗ' 모양 : 과도하게 Offset된 경우
    //  'ㅜ' 모양 : 기본적으로 만날 수 없다.

    vecOffsetPoint.push_back(vecLine[0].m_oFrom);

    for (size_t i=0; i<nxCount-1; i++)
    {
        TLine& oFirstLine  = vecLine[i];
        TLine& oSecondLine = vecLine[i+1];

        TxPoint oIP;
        int nIntersectType = TxMath::IntersectPoint(oFirstLine.m_oFrom, oFirstLine.m_oTo, oSecondLine.m_oTo, oSecondLine.m_oFrom, oIP);        
        // 두직선의 교점 : 9(평행), 0(교차), 1(두직선 모두 진행방향에서 교차), -1(두직선 모두 역방향에서 교차), 2
        if (nIntersectType == 0)
        {//교차
            vecOffsetPoint.push_back(oIP);
        }
        else if (nIntersectType == 1)
        {//연장선에서 교차한다
            // 각도가 날카로운 경우(점간의 간격이 짧게 엇갈린격우) 지나치게 뾰족하게 표현되어... 
            // vecOffsetPoint.push_back(oIP); -> 수정
            vecOffsetPoint.push_back(oFirstLine.m_oTo);
            vecOffsetPoint.push_back(oSecondLine.m_oFrom);
        }        
    }

    // 마지막점 추가
    vecOffsetPoint.push_back(vecLine[nxCount-1].m_oTo);

    return vecOffsetPoint;
}

double TxPolyline::MinDist( const TxPoint& oGivePt )
{
    double dMinDist(TxConst::MaxDbl());

    // 각 절점에서의 최소 거리계산
    for (size_t i=0; i<m_nxPointCnt; i++)
    {
        TxPoint oPtLoop(m_pPointsX[i], m_pPointsY[i]);
        double  dDist = TxMath::Dist(oPtLoop, oGivePt);

        if (dMinDist > dDist)
            dMinDist = dDist;
    }

    for (size_t i=1; i<m_nxPointCnt; i++)
    {
        TxPoint oA1(m_pPointsX[i-1], m_pPointsY[i-1]);
        TxPoint oA2(m_pPointsX[i],   m_pPointsY[i]);

        TxPoint oIP;
        double  dDistanceOnLine(0);
        
        if (TxMath::GetIntersectPoint(oA1, oA2, oGivePt, oIP, dDistanceOnLine) == 0)         
        {
            double dDist = TxMath::Dist(oGivePt, oIP);
            if (dMinDist > dDist)
                dMinDist = dDist;
        }
    }

    return dMinDist;
}

bool TxPolyline::LineOnPoint( const TxPoint& oGivePt, TxPoint& outIP, double& dLineTraceLen, size_t& nxFindIdx, int nFlag)
{
    //★ Flag 정리
    //★ 0 : 단순하게 outIP 만 처리
    //★ 1 : outIP, dLineTraceLen, nxFindIdx

    if (m_nxPointCnt == 0)
        return false;

    {//★ 초기화        
        dLineTraceLen = 0;
        nxFindIdx     = 0; 
    }

    if (m_nxPointCnt == 1) {
        outIP.X = m_pPointsX[0];
        outIP.Y = m_pPointsY[0];
        return true;
    }

    bool   bFind(false);
    double dMinDist(TxConst::MaxDbl());
    double dSumDist(0);

    //★ 1번 인덱스 부터 시작
    for (size_t i=1; i<m_nxPointCnt; i++) {
        TxPoint oA1(m_pPointsX[i-1], m_pPointsY[i-1]);
        TxPoint oA2(m_pPointsX[i],   m_pPointsY[i]);

        TxPoint oIPm;
        double  dDistanceOnLine(0);

        int nType = TxMath::GetIntersectPoint(oA1, oA2, oGivePt, oIPm, dDistanceOnLine);
        if (nType == 0) {
            double dDist = TxMath::Dist(oGivePt, oIPm);
            if (dMinDist >= dDist) { //★ >= 가장 작은 마지막 정보
                dMinDist = dDist;

                outIP = oIPm;                
                bFind = true;

                nxFindIdx  = i;
                if (nFlag > 0) /*★마지막 거리*/
                    dLineTraceLen = TxMath::Dist(oA1, oIPm); 
            }
        }        
    }

    if (bFind == true && nFlag > 0) {
        for (size_t i=1; i<nxFindIdx; i++) {
            TxPoint oA1(m_pPointsX[i-1], m_pPointsY[i-1]);
            TxPoint oA2(m_pPointsX[i],   m_pPointsY[i]);

            dLineTraceLen += TxMath::Dist(oA1, oA2);
        }
    }

    if (nxFindIdx != 0) {
        nxFindIdx--;
    }

    return bFind;
}

bool TxPolyline::LineOnPoint( const TxPoint& oGivePt, TxPoint& outIP)
{
    double dLineTraceLen(0);
    size_t nxFindIdx(0);
    return LineOnPoint(oGivePt, outIP, dLineTraceLen, nxFindIdx, 0);
}

bool TxPolyline::LineOnPoint( const TxPoint& oGivePt, TxPoint& outIP, double& dLineTraceLen )
{
    size_t nxFindIdx(0);
    return LineOnPoint(oGivePt, outIP, dLineTraceLen, nxFindIdx, 1);
}

bool TxPolyline::LineOnPoint( const TxPoint& oGivePt, TxPoint& outIP, double& dLineTraceLen, size_t& nxFindIdx )
{
    return LineOnPoint(oGivePt, outIP, dLineTraceLen, nxFindIdx, 1);    
}

void TxPolyline::LineOnPoint( double dLineTraceLen, TxPoint& outOnPt, TxPoint& outOffsetPt, double& outAngle, double dOffset )
{
    //★ 폴리라인을 따라서 일정거리 떨어진 지점의 좌표와 Offset 좌표, 각도 정보 계산
    double dLength = Length();

    if (dLength == 0) {
        if (m_nxPointCnt == 0) {
            throw 1;
        } else {
            outOnPt.X = m_pPointsX[0];
            outOnPt.Y = m_pPointsY[0];

            outOffsetPt = outOnPt;
            outAngle = 0;
        }

        return;
    }

    if (dLineTraceLen < 0.0)
        dLineTraceLen = 0.0;
    else if (dLineTraceLen > dLength)
        dLineTraceLen = dLength;

    double dSumDistFrom(0.0);
    double dSumDistTo(0.0);

    for (size_t i=0; i<m_nxPointCnt-1; i++) {
        TxPoint oPt1(m_pPointsX[i],   m_pPointsY[i]);
        TxPoint oPt2(m_pPointsX[i+1], m_pPointsY[i+1]);

        double  dDist = TxMath::Dist(oPt1, oPt2);

        dSumDistTo += dDist;

        if ( dLineTraceLen <= dSumDistTo) {
            double dTemp = dLineTraceLen - dSumDistFrom;
            TxMath::LineOnPoint(oPt1, oPt2, dTemp, outOnPt, outOffsetPt, outAngle, dOffset);
            break;
        }

        dSumDistFrom += dDist;
    }
}

void TxPolyline::LineOnPoint( double dLineTraceLen, TxPoint& outOnPt )
{
    TxPoint outOffsetPt;
    double  dAng(0);

    LineOnPoint(dLineTraceLen, outOnPt, outOffsetPt, dAng);
}

bool TxPolyline::LineOnPointAngleDist( const TxPoint& oGivePt, TxPoint& outIP, double& outAngle, double& outDist)
{
	if (m_nxPointCnt < 2) //라인상의 점의 갯수가 2개이하이면 방위각을 구하지 못함
		return false;

	double dMinDist(TxConst::MaxDbl());
	double dSumDist(0);
	size_t nxFindIndex(0);

    int nPreType(0);
	for (size_t i=1; i<m_nxPointCnt; i++) {
		TxPoint oA1(m_pPointsX[i-1], m_pPointsY[i-1]);
		TxPoint oA2(m_pPointsX[i],   m_pPointsY[i]);

		TxPoint oIPm;
		double  dDistanceOnLine(0);

		//-1:역방향교차, 0:직선위의점, 1:직선진행방향 ==>함수내에서는 모든걸 확인
		int nType = TxMath::GetIntersectPoint(oA1, oA2, oGivePt, oIPm, dDistanceOnLine);
		if (nType == 0) {
			double dDist = TxMath::Dist(oGivePt, oIPm);
            if (dMinDist > dDist) {
                nxFindIndex = i;
                dMinDist = dDist;
                outIP = oIPm;
            }
		}
        else { // 교차되지 않지만.. 산모양... 처리..
            if (nPreType > 0 && nType < 0) {
                double dDist = TxMath::Dist(oGivePt, oA1);
                if (dMinDist >= dDist && TxMath::Dist(oA1, oA2) >= dDistanceOnLine) { //★ >= 가장 작은 마지막 정보
                    dMinDist = dDist;

                    outIP = oA1;
                    nxFindIndex = i;
                }
            }
        }

        if (nType > 0) {
            double dDist = TxMath::Dist(oA1, oA2);
            if (dDist * 2 < dDistanceOnLine) {
                nType = 0;
            }
        }

        nPreType = nType;
	}

	if (nxFindIndex == 0) {
		return false;
	}

	TxVector vA(m_pPointsX[nxFindIndex-1], m_pPointsY[nxFindIndex-1]);
	TxVector vB(m_pPointsX[nxFindIndex],   m_pPointsY[nxFindIndex]);
	TxVector vAB  = vB - vA;

	// 각도
	double dAngle = vAB.Azimuth();
	outAngle = TxMath::Radian2Degree(dAngle);
 	outDist  = dMinDist;
	
	return true;
}


bool TxPolyline::LineOnPointAngleLeftRightOutDist( const TxPoint& oGivePt, double& outAngle, double& doutDist, double& dLeftDist, double& dRightDist )
{
    TxPoint outIP(0, 0);
    return LineOnPointAngleLeftRightOutDist(oGivePt, outIP, outAngle, doutDist, dLeftDist, dRightDist);
}


bool TxPolyline::LineOnPointAngleLeftRightOutDist(const TxPoint& oGivePt, TxPoint& outIP, double& outAngle, double& doutDist, double& dLeftDist, double& dRightDist)
{
    if (m_nxPointCnt < 2)
        return false;

    {// 초기화        
        doutDist = 0;
        dLeftDist = 0;
        dRightDist = 0;
    }

    size_t  nxFindIdx(0);

    bool   bFind(false);
    double dMinDist(TxConst::MaxDbl());
    double dSumDist(0);

    int nPreType(0);
    for (size_t i = 1; i < m_nxPointCnt; i++) {
        TxPoint oA1(m_pPointsX[i - 1], m_pPointsY[i - 1]);
        TxPoint oA2(m_pPointsX[i], m_pPointsY[i]);

        TxPoint oIPm;
        double  dDistanceOnLine(0);

        //-1:역방향교차, 0:직선위의점, 1:직선진행방향 ==>함수내에서는 모든걸 확인
        int nType = TxMath::GetIntersectPoint(oA1, oA2, oGivePt, oIPm, dDistanceOnLine);
        if (nType == 0) {
            double dDist = TxMath::Dist(oGivePt, oIPm);
            if (dMinDist >= dDist) { //★ >= 가장 작은 마지막 정보
                dMinDist = dDist;

                outIP = oIPm;
                bFind = true;

                nxFindIdx = i;
                dLeftDist = TxMath::Dist(oA1, oIPm);
            }
        }
        else { // 교차되지 않지만.. 산모양... 처리..
            if (nPreType > 0 && nType < 0) {
                double dDist = TxMath::Dist(oGivePt, oA1);
                if (dMinDist >= dDist && TxMath::Dist(oA1, oA2) >= dDistanceOnLine) { //★ >= 가장 작은 마지막 정보
                    dMinDist = dDist;

                    outIP = oA1;
                    bFind = true;

                    nxFindIdx = i;
                    dLeftDist = 0;
                }
            }
        }

        if (nType > 0) {
            double dDist = TxMath::Dist(oA1, oA2);
            if (dDist * 2 < dDistanceOnLine) {
                nType = 0;
            }
        }

        nPreType = nType;
    }

    if (bFind == false) {
        return false;
    }

    //이전 거리
    if (bFind == true) {
        for (size_t i = 1; i < nxFindIdx; i++) {
            TxPoint oA1(m_pPointsX[i - 1], m_pPointsY[i - 1]);
            TxPoint oA2(m_pPointsX[i], m_pPointsY[i]);
            dLeftDist += TxMath::Dist(oA1, oA2);
        }
    }

    //이후 거리
    double dTotalDist = Length();
    dRightDist = dTotalDist - dLeftDist;

    //점과 라인과의 거리
    doutDist = dMinDist;

    //각도
    TxVector vA(m_pPointsX[nxFindIdx - 1], m_pPointsY[nxFindIdx - 1]);
    TxVector vB(m_pPointsX[nxFindIdx], m_pPointsY[nxFindIdx]);
    TxVector vAB = vB - vA;

    // 각도
    double dAngle = vAB.Azimuth();
    outAngle = TxMath::Radian2Degree(dAngle);

    return bFind;
}

bool TxPolyline::LineOnPointOutDistAndPoint(const TxPoint& oGivePt, TxPoint& outIP, double& doutDist, int &nOnType)
{
	if (m_nxPointCnt < 2)
		return false;

	// 초기화        
	{
		doutDist = 0;
	}

	int     nMinType(0);
	size_t  nxFindIdx(0);
	bool    bFind(false);
	double  dMinDist(TxConst::MaxDbl());
	double  dSumDist(0);

	//★ 1번 인덱스 부터 시작
	for (size_t i = 1; i < m_nxPointCnt; i++) {
		TxPoint oA1(m_pPointsX[i - 1], m_pPointsY[i - 1]);
		TxPoint oA2(m_pPointsX[i], m_pPointsY[i]);

		TxPoint oIPm;
		double  dDistanceOnLine(0);

		int nType = TxMath::GetIntersectPoint(oA1, oA2, oGivePt, oIPm, dDistanceOnLine);
		if (1 == i)
		{
			if ( 0 == nType )
			{
				double dDist = TxMath::Dist(oGivePt, oIPm);
				if (dDist < dMinDist)
				{
					bFind = true;
					dMinDist = dDist;
					outIP = oIPm;
					nMinType = nType;
				}
			}
			else if (-1 == nType || 1 == nType)
			{
				TxPoint oStdPoint(m_pPointsX[0], m_pPointsY[0]);
				double dDist = TxMath::Dist(oGivePt, oStdPoint);
				if (dDist < dMinDist)
				{
					bFind = true;
					dMinDist = dDist;
					outIP = oStdPoint;
					nMinType = nType;
				}
			}
		}

		if ((m_nxPointCnt-1) == i)
		{
			if (0 == nType )
			{
				double dDist = TxMath::Dist(oGivePt, oIPm);
				if (dDist < dMinDist)
				{
					bFind = true;
					dMinDist = dDist;
					outIP = oIPm;
					nMinType = nType;
				}
			}
			else if (1 == nType || -1 == nType)
			{
				TxPoint oStdPoint(m_pPointsX[m_nxPointCnt-1], m_pPointsY[m_nxPointCnt - 1]);
				double dDist = TxMath::Dist(oGivePt, oStdPoint);
				if (dDist < dMinDist)
				{
					bFind = true;
					dMinDist = dDist;
					outIP = oStdPoint;
					nMinType = nType;
				}
			}
		}

        if (i > 1 && i < (m_nxPointCnt - 1))
		{
			if (nType == 0)
			{
				double dDist = TxMath::Dist(oGivePt, oIPm);
				if (dDist < dMinDist)
				{
					bFind		= true;
					dMinDist	= dDist;
					outIP		= oIPm;
					nMinType	= nType;
				}
			}
		}
	}

	nOnType = nMinType;
	doutDist = dMinDist;

	return bFind;
}

void TxPolyline::LinePercentPt(double dRate, TxPoint& outPt, double& outAngle, double dOffset)
{
    double dLength = Length();
    if (dLength <= 0.0000000001) {
        if (m_nxPointCnt > 0) {
            outPt.X = m_pPointsX[0];
            outPt.Y = m_pPointsY[0];
            outAngle  = 0;
        }
        else {
            throw 1;
        }
        return;
    }

    if (dRate < 0) {
        dRate = 0;
    } else if (dRate >= 1) {
        dRate = 1;
    }

    double _dDist  = dLength * dRate;
    double dSumDistFrom(0.0);
    double dSumDistTo(0.0);
    for (size_t i=0; i<m_nxPointCnt-1; i++)
    {
        TxPoint oPt1(m_pPointsX[i],   m_pPointsY[i]);
        TxPoint oPt2(m_pPointsX[i+1], m_pPointsY[i+1]);

        double  dDist = TxMath::Dist(oPt1, oPt2);

        dSumDistTo += dDist;

        if ( _dDist <= dSumDistTo + 0.0000000001) {
            double dTemp = _dDist - dSumDistFrom;
            TxPoint oOnPt;
            TxMath::LineOnPoint(oPt1, oPt2, dTemp, oOnPt, outPt, outAngle, dOffset);
            break;
        }

        dSumDistFrom += dDist;
    }
}

void TxPolyline::LinePercentPt( double dRate, TxPoint& outPt, double dOffset /*= 0*/ )
{
    double outAngle(0);
    LinePercentPt(dRate, outPt, outAngle, dOffset);
}

double TxPolyline::Length()
{
    if (m_nxPointCnt <= 1)
        return 0;

    double dLength(0.0);
    for (size_t i=0; i<m_nxPointCnt-1; i++)
    {
        TxPoint oPt1(m_pPointsX[i], m_pPointsY[i]);
        TxPoint oPt2(m_pPointsX[i+1], m_pPointsY[i+1]);

        double  dDist = TxMath::Dist(oPt1, oPt2);

        dLength += dDist;
    }
    return dLength;
}


ITxGeometryPtr TxPolyline::CopyGeometry()
{
    return ITxGeometryPtr(new TxPolyline(*this));
}

void TxPolyline::ExtractLine( double _dDist, std::vector<TxPoint>& vecLeft, std::vector<TxPoint>& vecRight )
{
    if (m_nxPointCnt <= 1)
        return;

    TxPoint _oOnPt;
    TxPoint _oOffsetPt;
    double  _dAngle(0);
    double  _dOffset(0);

    vecLeft.clear();
    vecRight.clear();

    double dLength = Length();
    if (_dDist < 0.0)
        _dDist = 0.0;
    else if (_dDist > dLength)
        _dDist = dLength;

    double dSumDistFrom(0.0);
    double dSumDistTo  (0.0); 
    bool   bLeft(true);

    if (_dDist == 0) {
        for (size_t i=0; i<m_nxPointCnt; i++) {
            TxPoint oPt1(m_pPointsX[i],   m_pPointsY[i]);
            vecRight.push_back(oPt1);
        }        
        return;
    }

    if (_dDist == dLength) {
        for (size_t i=0; i<m_nxPointCnt; i++) {
            TxPoint oPt1(m_pPointsX[i],   m_pPointsY[i]);
            vecLeft.push_back(oPt1);
        }
        return;
    }

    for (size_t i=0; i<m_nxPointCnt-1; i++)
    {
        TxPoint oPt1(m_pPointsX[i],   m_pPointsY[i]);
        TxPoint oPt2(m_pPointsX[i+1], m_pPointsY[i+1]);

        if (bLeft == true) {
            double  dDist = TxMath::Dist(oPt1, oPt2);
            dSumDistTo += dDist;

            if ( _dDist < dSumDistTo) {
                double dTemp = _dDist - dSumDistFrom;
                if (dTemp > 0) {
                    TxMath::LineOnPoint(oPt1, oPt2, dTemp, _oOnPt, _oOffsetPt, _dAngle, _dOffset);
                    vecLeft.push_back(oPt1);
                    vecLeft.push_back(_oOnPt);
                    vecRight.push_back(_oOnPt);
                } else {
                    vecLeft.push_back(oPt1);
                    vecRight.push_back(oPt1);
                }
                
                bLeft = false; /*★*/
            }

            dSumDistFrom += dDist;
        }
        
        if (bLeft == true) {
            vecLeft.push_back(oPt1);
        } else {
            vecRight.push_back(oPt2);
        }        
    }
}

std::vector<TxPoint> TxPolyline::ExtractLine( double dMinDist, double dMaxDist )
{
    std::vector<TxPoint> vecPoint;
    if (m_nxPointCnt <= 1)
        return vecPoint;

    TxPoint _oOnPt;
    TxPoint _oOffsetPt;
    double  _dAngle(0);
    double  _dOffset(0);
        
    double dLength = Length();
    if (dMinDist < 0.0)
        dMinDist = 0.0;
    else if (dMinDist > dLength)
        dMinDist = dLength;

    if (dMaxDist > dLength || dMinDist > dMaxDist) {
        dMaxDist = dLength;
    }    

    double dSumDistFrom(0.0);
    double dSumDistTo(0.0);
    bool   bFirstFind(false);
    for (size_t i=0; i<m_nxPointCnt-1; i++)
    {
        TxPoint oPt1(m_pPointsX[i],   m_pPointsY[i]);
        TxPoint oPt2(m_pPointsX[i+1], m_pPointsY[i+1]);

        double  dDist = TxMath::Dist(oPt1, oPt2);
        dSumDistTo += dDist;

        if (bFirstFind == false) {
            if (dMinDist < dSumDistTo) {
                double dTemp = dMinDist - dSumDistFrom;
                if (dTemp > 0) {
                    TxMath::LineOnPoint(oPt1, oPt2, dTemp, _oOnPt, _oOffsetPt, _dAngle, _dOffset);
                    vecPoint.push_back(_oOnPt);
                } else {
                    vecPoint.push_back(oPt1);
                }
                
                bFirstFind = true; /*★*/
            }            
        } 

        if (bFirstFind == true && dMinDist == dMaxDist) {
            break;
        }

        if (bFirstFind == true) {
            if ( dMaxDist <= dSumDistTo) {
                double dTemp = dMaxDist - dSumDistFrom;
                TxMath::LineOnPoint(oPt1, oPt2, dTemp, _oOnPt, _oOffsetPt, _dAngle, _dOffset);
                vecPoint.push_back(_oOnPt);
                break;
            } 

            vecPoint.push_back(oPt2);
        }

        dSumDistFrom += dDist;
    }    
    
    return vecPoint;
}

void TxPolyline::AddVertex( const TxPoint& oPt )
{
    std::vector<TxPoint> vecPoint;
    for (size_t i=0; i<m_nxPointCnt; i++) {
        TxPoint oPt(m_pPointsX[i], m_pPointsY[i]);
        vecPoint.push_back(oPt);
    }

    vecPoint.push_back(oPt);    
    SetPoints(vecPoint);
}

void TxPolyline::RemoveVertex( int nIdx )
{
    std::vector<TxPoint> vecPoint;
    for (size_t i=0; i<m_nxPointCnt; i++) {
        if ( nIdx != (int)i ) {
            TxPoint oPt(m_pPointsX[i], m_pPointsY[i]);
            vecPoint.push_back(oPt);
        }        
    }

    SetPoints(vecPoint);
}

void TxPolyline::MoveVertex( int nIdx, const TxPoint& oPt )
{
    if (0 <= nIdx && nIdx < (int) m_nxPointCnt) {
        m_pPointsX[nIdx] = oPt.X;
        m_pPointsY[nIdx] = oPt.Y;
    }
    
    ResetEnvelope();
}

void TxPolyline::InsertVertex( int nIdx, const TxPoint& oPt )
{
    if (0 <= nIdx && nIdx < (int) m_nxPointCnt) {}
    else {
        return;
    }

    std::vector<TxPoint> vecPoint;
    for (size_t i=0; i<m_nxPointCnt; i++) {
        if (nIdx == (int) i) {
            vecPoint.push_back(oPt);
        }
                
        vecPoint.push_back(TxPoint(m_pPointsX[i], m_pPointsY[i]));
    }

    SetPoints(vecPoint);
}

void TxPolyline::StartPtMove( const TxPoint& oPt )
{
    if (m_nxPointCnt == 0)
        return;

    m_pPointsX[0] = oPt.X;
    m_pPointsY[0] = oPt.Y;

    ResetEnvelope();
}

void TxPolyline::EndPtMove( const TxPoint& oPt )
{
    if (m_nxPointCnt == 0)
        return;

    m_pPointsX[m_nxPointCnt-1] = oPt.X;
    m_pPointsY[m_nxPointCnt-1] = oPt.Y;

    ResetEnvelope();
}

TxPoint TxPolyline::StartPoint()
{
    if (m_nxPointCnt == 0)
        throw 1;

    return TxPoint(m_pPointsX[0], m_pPointsY[0]);
}

TxPoint TxPolyline::EndPoint()
{
    if (m_nxPointCnt == 0)
        throw 1;

    return TxPoint(m_pPointsX[m_nxPointCnt-1], m_pPointsY[m_nxPointCnt-1]);
}

int TxPolyline::IsLineRight(const TxPoint& oPt )
{
    if (m_nxPointCnt < 1)
        return 0;

    std::vector<TxPoint> vecPt; 
    for (size_t i=0; i<m_nxPointCnt; i++) {
        vecPt.push_back(TxPoint(m_pPointsX[i], m_pPointsY[i]));
    }
    vecPt.push_back(oPt);

    int i(0), j(0), k(0);
    int nCount = 0;
    double dTemp(0);

    int nLoop = (int)vecPt.size();
    if (nLoop < 3)
        return(0);

    for (i=0;i<nLoop;i++) {
        j = (i + 1) % nLoop;
        k = (i + 2) % nLoop;

        dTemp  = (vecPt[j].X - vecPt[i].X) * (vecPt[k].Y - vecPt[j].Y);
        dTemp -= (vecPt[j].Y - vecPt[i].Y) * (vecPt[k].X - vecPt[j].X);
        
        if (dTemp < 0)
            nCount--;
        else if (dTemp > 0)
            nCount++;
    }

    if (nCount > 0)
        return -1;
    else if (nCount < 0) 
        return 1;

    return 0;
}

std::vector<TxPoint> TxPolyline::ExtractVertex( double dMinDist )
{
    std::vector<TxPoint> vecPoint;
    if (m_nxPointCnt <= 0)
        return vecPoint;

    if (dMinDist <= 0)
        return vecPoint;

    for (size_t i=0; i<m_nxPointCnt-1; i++)
    {
        TxPoint oPt1(m_pPointsX[i],   m_pPointsY[i]);
        TxPoint oPt2(m_pPointsX[i+1], m_pPointsY[i+1]);

        vecPoint.push_back(oPt1);

        double dDist = TxMath::Dist(oPt1, oPt2);
        if (dDist <= dMinDist) {            
            continue;
        }

        //★ 분할한다.
        TxVector oA(oPt1.X, oPt1.Y);
        TxVector oB(oPt2.X, oPt2.Y);
        TxVector oAB = oB - oA; 

        int nLoop = (int) (dDist / dMinDist);
        if (dDist == dMinDist * nLoop)
            nLoop--;

        for (int i=1; i<=nLoop; i++) {
            oAB.SetLength(dMinDist * i);
            oAB.GetI();
            oAB.GetJ();

            TxPoint oPx(oPt1.X + oAB.GetI(), oPt1.Y + oAB.GetJ());
            vecPoint.push_back(oPx);
        }        
    }

    //★ 마지막 점 추가
    vecPoint.push_back(EndPoint());
    return vecPoint;
}

std::shared_ptr<TxPolyline> TxPolyline::BackwardLine()
{
    std::vector<TxPoint> vecPoint;

    for (int i= (int)m_nxPointCnt-1; i>=0; i--) {
        TxPoint oPt(m_pPointsX[i], m_pPointsY[i]);
        vecPoint.push_back(oPt);
    }

    return TxPolylinePtr(new TxPolyline(vecPoint));
}


TxMPolyline::TxMPolyline() : ITxGeometry(GeoMPolylineType)
{
}

TxMPolyline::TxMPolyline( const TxMPolyline& right ) : ITxGeometry(GeoMPolylineType)
{
    m_vecPolyline = right.m_vecPolyline;
    m_oEnvelope   = right.m_oEnvelope;
}

TxMPolyline::TxMPolyline( QBicBinary& oBinary, size_t nIndex/*=0*/ )
{
    int nGeometryType(0);
    int nCount(0);

    oBinary.Position(nIndex);
    oBinary.Read(nGeometryType);
    
    oBinary.Read(nCount);

    std::vector<TxPoint> vecPoint;
    for (int i=0; i<nCount; i++)
    {
        TxPolyline oPolyline(oBinary, oBinary.Position());
        m_vecPolyline.push_back(oPolyline);
    }
}

TxMPolyline& TxMPolyline::operator=( const TxMPolyline& right )
{
    if (&right == this)
        return *this;

    m_vecPolyline = right.m_vecPolyline;
    m_oEnvelope   = right.m_oEnvelope;

    return *this;
}

TxMPolyline::~TxMPolyline()
{

}

TxEnvelope TxMPolyline::Envelope() const
{
    return m_oEnvelope;
}

void TxMPolyline::Add(const TxPolyline& oPline )
{
   m_vecPolyline.push_back(oPline);
   m_oEnvelope += oPline.Envelope();
}

void TxMPolyline::GetBinary( QBicBinary& oBinary )
{
    oBinary.PushBack((int)m_enGeometryType);

    int nCount = (int)m_vecPolyline.size();
    oBinary.PushBack(nCount);
    
    for (size_t i=0; i<m_vecPolyline.size(); i++)
    {
        TxPolyline& oPolyline = m_vecPolyline[i];
        oPolyline.GetBinary(oBinary);
    }
}

double TxMPolyline::MinDist( const TxPoint& oPt )
{
    double dMinDist(TxConst::MaxDbl());
    for (size_t i=0; i<m_vecPolyline.size(); i++)
    {
        TxPolyline& oPolyline = m_vecPolyline[i];
        double      dDist     = oPolyline.MinDist(oPt);

        if (dMinDist > dDist)
            dMinDist = dDist;
    }
    
    return dMinDist;
}

bool TxMPolyline::Intersect( const TxEnvelope& _oEnvelope ) const
{
    bool bIntersetct(false);
    for (size_t i=0; i<m_vecPolyline.size(); i++)
    {
        const TxPolyline& oPolyline = m_vecPolyline[i];
        if ( oPolyline.Intersect(_oEnvelope) == true )
            return true;
    }

    return bIntersetct;
}

ITxGeometryPtr TxMPolyline::CopyGeometry()
{
    return ITxGeometryPtr(new TxMPolyline(*this));
}


