#include "stdafx.h"
#include "TxPolygon.h"
#include "TxMath.h"

TxPolygon::TxPolygon() : ITxGeometry(GeoPolygonType), m_pOutRing(nullptr)
{

}

TxPolygon::TxPolygon( BoostPolygon& oBoostPoly ) : ITxGeometry(GeoPolygonType), m_pOutRing(nullptr)
{    
    if (true) {
        std::vector<BoostPoint>& vecOut = oBoostPoly.outer();
        
        std::vector<TxPoint> vecPt;
        for (size_t i=0; i<vecOut.size(); i++) {
            BoostPoint& oPt = vecOut[i];
            vecPt.push_back(TxPoint(oPt.get<0>(), oPt.get<1>()));
        }
        OutRing(vecPt);
    }

    auto inners = oBoostPoly.inners();
    for (auto iter = inners.begin(); iter != inners.end(); ++iter) {
        std::vector<BoostPoint>& vecOut = *iter;
        
        std::vector<TxPoint> vecPt;
        for (size_t i=0; i<vecOut.size(); i++) {
            BoostPoint& oPt = vecOut[i];
            vecPt.push_back(TxPoint(oPt.get<0>(), oPt.get<1>()));
        }
        AddHolePoint(vecPt);
    }
}

TxPolygon::TxPolygon( const TxPolygon& right ) : ITxGeometry(GeoPolygonType), m_pOutRing(nullptr)
{
    TxPolyline* pRight = right.m_pOutRing;
    if (pRight)
        m_pOutRing = new TxPolyline(*pRight);

    size_t nxHoleCount = right.GetHoleCount();
    for (size_t i=0; i<nxHoleCount; i++)
    {
        pRight = right.m_vecHoleRing[i];
        if (pRight)
            m_vecHoleRing.push_back(new TxPolyline(*pRight));
    }

    m_oEnvelope = right.m_oEnvelope;
}

TxPolygon::TxPolygon( std::shared_ptr<TxPolygon> spPoly ) : ITxGeometry(GeoPolygonType), m_pOutRing(nullptr)
{
    TxPolyline* pRight = spPoly->m_pOutRing;
    if (pRight)
        m_pOutRing = new TxPolyline(*pRight);

    size_t nxHoleCount = spPoly->GetHoleCount();
    for (size_t i=0; i<nxHoleCount; i++)
    {
        pRight = spPoly->m_vecHoleRing[i];
        if (pRight)
            m_vecHoleRing.push_back(new TxPolyline(*pRight));
    }

    m_oEnvelope = spPoly->m_oEnvelope;
}


TxPolygon::TxPolygon( const std::vector<TxPoint>& vecPoint ) : ITxGeometry(GeoPolygonType), m_pOutRing(nullptr)
{
    if (vecPoint.size() >= 3)
        OutRing(vecPoint);
}


TxPolygon::TxPolygon( QBicBinary& oBinary, size_t nIndex/*=0*/ ) : ITxGeometry(GeoPolygonType), m_pOutRing(nullptr)
{   
    // 타입
    // OutRing 포인트 갯수
    // OutRing 버텍스 정보
    // 홀의 갯수
    // #1..n 홀의 포인트 갯수
    // #1..n 홀의 버텍스 정보
    int nGeometryType(0);
    oBinary.Position(nIndex);
    oBinary.Read(nGeometryType);

    int nCount(0);
    oBinary.Read(nCount);

    std::vector<TxPoint> vecPoint;
    for (int m=0; m<nCount; m++)
    {
        BinDouble dX(0);
        BinDouble dY(0);

        oBinary.Read(dX);
        oBinary.Read(dY);

        vecPoint.push_back(TxPoint((double)dX, (double)dY));
    }

    m_pOutRing = new TxPolyline();
    m_pOutRing->SetPoints(vecPoint);

    int nHoleCount(0);
    oBinary.Read(nHoleCount);
    
    for (int i=0; i<nHoleCount; i++)
    {
        vecPoint.clear();

        oBinary.Read(nCount);        
        for (int m=0; m<nCount; m++)
        {
            BinDouble dX(0);
            BinDouble dY(0);

            oBinary.Read(dX);
            oBinary.Read(dY);

            vecPoint.push_back(TxPoint((double)dX, (double)dY));
        }

        TxPolyline* pPolyline = new TxPolyline();
        pPolyline->SetPoints(vecPoint);
        m_vecHoleRing.push_back(pPolyline);
    }

    if (m_pOutRing)
        m_oEnvelope = m_pOutRing->Envelope();
}


TxPolygon::~TxPolygon()
{
    delete m_pOutRing;
    for (size_t i=0; i<m_vecHoleRing.size(); i++)
    {
        TxPolyline* pPolyline = m_vecHoleRing[i];
        delete pPolyline;
    }

    m_vecHoleRing.clear();
}

TxEnvelope TxPolygon::Envelope() const
{
    return m_oEnvelope;
}

CString TxPolygon::ToString()
{
    CString strOut(_T(""));
    strOut.Format(_T("\nTxSimplePolygon - OutRing : %s"), m_pOutRing->ToString());
    
    size_t nxHoleCount = m_vecHoleRing.size();
    strOut.AppendFormat(_T("\nHole Count : %d"), nxHoleCount);
    for (size_t i=0; i<nxHoleCount; i++)
    {
        TxPolyline* pPolyline = m_vecHoleRing[i];

        strOut.AppendFormat(_T("\nHole Index : %d"), i);
        strOut.AppendFormat(_T("%s"), pPolyline->ToString());
    }

    return strOut;
}

void TxPolygon::GetBinary( QBicBinary& oBinary )
{    
    // 타입
    // OutRing 포인트 갯수
    // OutRing 버텍스 정보
    // 홀의 갯수
    // #1..n 홀의 포인트 갯수
    // #1..n 홀의 버텍스 정보
    oBinary.PushBack((int)m_enGeometryType);
    std::vector<TxPoint> vecPoint = m_pOutRing->GetPoints();
    size_t nxPointCount = vecPoint.size();

    oBinary.PushBack((int)nxPointCount);
    for (size_t m=0; m<nxPointCount; m++)
    {      
        TxPoint& oPoint = vecPoint[m];
        oBinary.PushBack((BinDouble)oPoint.X);
        oBinary.PushBack((BinDouble)oPoint.Y);
    }
    
    // 홀의 갯수
    oBinary.PushBack((int)m_vecHoleRing.size());

    for (size_t i=0; i<m_vecHoleRing.size(); i++)
    {
        TxPolyline* pPolyline = m_vecHoleRing[i];

        vecPoint = pPolyline->GetPoints();
        nxPointCount = vecPoint.size();

        oBinary.PushBack((int)nxPointCount);
        for (size_t m=0; m<nxPointCount; m++)
        {      
            TxPoint& oPoint = vecPoint[m];
            oBinary.PushBack((BinDouble)oPoint.X);
            oBinary.PushBack((BinDouble)oPoint.Y);
        }
    }
}

void TxPolygon::OutRing( const std::vector<TxPoint>& vecPoint, int nType/*=0*/ ) 
{
    delete m_pOutRing; {
        m_pOutRing = new TxPolyline();
        m_pOutRing->SetPoints(vecPoint);
    }    

    //★ Hole은 삭제한다.
    for (size_t i=0; i<m_vecHoleRing.size(); i++) {
        TxPolyline* pPolyline = m_vecHoleRing[i];
        delete pPolyline;
    }
    m_vecHoleRing.clear();

    ConvertClockwiseOutRing();
    if (m_pOutRing)
        m_oEnvelope = m_pOutRing->Envelope();
}


void TxPolygon::ConvertClockwiseOutRing()
{
    double dArea = Area(m_pOutRing, 0);
    if (dArea > 0)
        m_pOutRing->Flip();
}

double TxPolygon::Area()
{
    double dArea(0.0);
    dArea = Area(m_pOutRing, 1);

    for (size_t i=0; i<m_vecHoleRing.size(); i++) {
        TxPolyline* pPolyline = m_vecHoleRing[i];
        dArea -= Area(pPolyline, 1);
    }

    return dArea;
}

double TxPolygon::Area( TxPolyline* pPolyline, int nType )
{
    size_t  nxPointCount = pPolyline->m_nxPointCnt;
    double* pPointsX     = pPolyline->m_pPointsX;
    double* pPointsY     = pPolyline->m_pPointsY;
    
    double dTemp(0.0);
    if (nxPointCount < 3 )
        return dTemp;

    for (size_t i=0; i<nxPointCount-1; i++) {
        dTemp += pPointsX[i] * pPointsY[i+1] - pPointsX[i+1] * pPointsY[i];
    }

    double dArea(0.0);
    
    //★ 양수(반시계방향) / 음수(시계방향)
    if (nType == 1)
        dTemp = fabs(dTemp);
    
    dArea = dTemp * 0.5f;

    return dArea;
}


bool TxPolygon::IsInPoint(const TxPoint& oPt ) const
{    
    size_t  nxPointCount = m_pOutRing->m_nxPointCnt;
    double* pPointsX     = m_pOutRing->m_pPointsX;
    double* pPointsY     = m_pOutRing->m_pPointsY;

    if (nxPointCount < 3 )
        return false;
        
    if ( m_oEnvelope.Intersect(TxEnvelope(oPt.X, oPt.Y, oPt.X, oPt.Y)) == false )
        return false;

    double dWidth  = m_oEnvelope.GetWidth();
    double dHeight = m_oEnvelope.GetHeight();

    int nRightTruePointCount(0);

    TxPoint oLnFrom    = oPt;
    TxPoint oLnToRight = oLnFrom;

    //★ Ray : 우측으로 매우 큰값
    oLnToRight.X = oLnFrom.X + dWidth + 1000000000;
    //★ 절점에서 만나는 경우
    TxPoint oPreIP(-999999999, -999999999);

    // OutRing Check
    for (size_t i=0; i<nxPointCount-1; i++)
    {
        TxPoint oPnFrom(pPointsX[i],   pPointsY[i]  );
        TxPoint oPnTo  (pPointsX[i+1], pPointsY[i+1]);

        TxPoint oIP;
        bool bIntersect = TxMath::GetIntersectPoint(oPnFrom, oPnTo, oLnFrom, oLnToRight, oIP, false);
        if (bIntersect) {
            if (oPreIP == oIP) {
                continue;
            }
            nRightTruePointCount++;

            oPreIP = oIP;
        }
        else {
            oPreIP = TxPoint(-999999999, -999999999);
        }
    }

    for (size_t i=0; i<m_vecHoleRing.size(); i++)
    {
        TxPolyline* pPolyline = m_vecHoleRing[i];

        size_t   nxPointCount = pPolyline->m_nxPointCnt;
        double*  pPointsX     = pPolyline->m_pPointsX;
        double*  pPointsY     = pPolyline->m_pPointsY;

        if (nxPointCount < 3 )
            return false;

        TxEnvelope oEnvelope = pPolyline->Envelope();
        if ( oEnvelope.Intersect(TxEnvelope(oPt.X, oPt.Y, oPt.X, oPt.Y)) == false )
            continue;

        double dWidth  = oEnvelope.GetWidth();
        double dHeight = oEnvelope.GetHeight();

        TxPoint oLnFrom    = oPt;
        TxPoint oLnToRight = oLnFrom;

        //★ Ray : 우측으로 매우 큰값
        oLnToRight.X = oLnFrom.X + dWidth + 1000000000;

        //★ 절점에서 만나는 경우
        TxPoint oPreIP(-999999999, -999999999);

        for (size_t i=0; i<nxPointCount-1; i++)
        {
            TxPoint oPnFrom(pPointsX[i],   pPointsY[i]  );
            TxPoint oPnTo  (pPointsX[i+1], pPointsY[i+1]);

            TxPoint oIP;
            bool bIntersect = TxMath::GetIntersectPoint(oPnFrom, oPnTo, oLnFrom, oLnToRight, oIP, false);
            if (bIntersect) {
                if (oPreIP == oIP) {
                    continue;
                }
                nRightTruePointCount++;
                
                oPreIP = oIP;
            }   
            else {
                oPreIP = TxPoint(-999999999, -999999999);
            }
        }
    }

    if ( nRightTruePointCount % 2 == 0)
        return false;

    return true;
}

bool TxPolygon::IsInPoint( TxPointPtr spPt ) const
{
    TxPoint oPt(spPt->X, spPt->Y);

    return IsInPoint(oPt);
}

bool TxPolygon::AddHolePoint( const std::vector<TxPoint>& vecPoint )
{
    if (vecPoint.size() < 3)
        return false;

    TxPolyline* pPolyline = new TxPolyline;
    pPolyline->SetPoints(vecPoint);

    double dArea = Area(pPolyline, 0);
    if (dArea < 0)
        pPolyline->Flip(); //Hole은 반시계방향    

    if ( m_oEnvelope.Contains(pPolyline->Envelope()) == false ) {
        delete pPolyline;
        return false;
    }
        
    if (m_pOutRing->Intersect(*pPolyline) == true) {
        delete pPolyline;
        return false;
    }
    
    size_t nxHoleCount = m_vecHoleRing.size();
    for (size_t i=0; i<nxHoleCount; i++)
    {
        if (pPolyline->Intersect(*m_vecHoleRing[i]) == true) {
            delete pPolyline;
            return false;
        }
    }
    
    m_vecHoleRing.push_back(pPolyline);
    return true;
}

std::vector<TxPoint> TxPolygon::OutRing() const
{
    std::vector<TxPoint> vecPoint;
    size_t nxPointCount = m_pOutRing->m_nxPointCnt;
    for (size_t i=0; i<nxPointCount; i++)
    {
        vecPoint.push_back(TxPoint(m_pOutRing->m_pPointsX[i], m_pOutRing->m_pPointsY[i]));
    }

    return vecPoint;
}

double TxPolygon::MinDist( const TxPoint& oPt )
{
    double dMinDist(TxConst::MaxDbl());
    double dDist(0.0);
    dDist = m_pOutRing->MinDist(oPt);

    if (dMinDist > dDist)
        dMinDist = dDist;

    for (size_t i=0; i<m_vecHoleRing.size(); i++)
    {
        TxPolyline* pLine = m_vecHoleRing[i];
        dDist = pLine->MinDist(oPt);

        if (dMinDist > dDist)
            dMinDist = dDist;
    }

    return dMinDist;
}

bool TxPolygon::Intersect( const TxEnvelope& _oEnvelope ) const
{   
    // 1. Envelope 교차 체크
    if (_oEnvelope.Contains(m_oEnvelope) == true) // 요청한 Envelope 안에 폴리곤이 포함된 경우
        return true;

    if ( _oEnvelope.Intersect(m_oEnvelope) == false)
        return false;

    // 2. 폴리곤 포함 여부 체크(한개의 점이라도 포함됐다면) 
    if ( IsInPoint(TxPoint(_oEnvelope.GetMinX(), _oEnvelope.GetMinY())) == true )
        return true;

    if ( IsInPoint(TxPoint(_oEnvelope.GetMinX(), _oEnvelope.GetMaxY())) == true )
        return true;

    if ( IsInPoint(TxPoint(_oEnvelope.GetMaxX(), _oEnvelope.GetMaxY())) == true )
        return true;

    if ( IsInPoint(TxPoint(_oEnvelope.GetMaxX(), _oEnvelope.GetMinY())) == true )
        return true;

    // 3. 모든 점이 Hole에 포함되는가 ?
    for (size_t i=0; i<GetHoleCount(); i++) {
        std::vector<TxPoint> vecPoint = GetHolePoints(i);
        TxPolygonPtr spPolygon = TxPolygonPtr(new TxPolygon(vecPoint));
        int nInCount(0);
        if ( spPolygon->IsInPoint(TxPoint(_oEnvelope.GetMinX(), _oEnvelope.GetMinY())) )
            nInCount ++;

        if ( spPolygon->IsInPoint(TxPoint(_oEnvelope.GetMinX(), _oEnvelope.GetMaxY())) == true )
            nInCount ++;

        if ( spPolygon->IsInPoint(TxPoint(_oEnvelope.GetMaxX(), _oEnvelope.GetMaxY())) == true )
            nInCount ++;

        if ( spPolygon->IsInPoint(TxPoint(_oEnvelope.GetMaxX(), _oEnvelope.GetMinY())) == true )
            nInCount ++;

        if (nInCount == 4)
            return false;
    }
    
    // 4. 교차 여부 체크
    if ( m_pOutRing->Intersect(_oEnvelope) == true )
        return true;

    return false;
}

bool TxPolygon::Intersect( std::shared_ptr<TxPolyline> spPolyline ) const
{
    // 1. Envelope 교차 체크
    TxEnvelope _oEnvelope = spPolyline->Envelope();
    if ( _oEnvelope.Intersect(m_oEnvelope) == false)
        return false;

    // 2. 폴리라인을 루핑하면서 폴리곤 InPoint체크 및 교차 체크를 진행한다.
    //    폴리곤에 폴리라인의 한점이 포함된 경우 교차
    //    폴리곤의 버텍스~버텍스와 폴리라인 버텍스~버텍스 간의 라인 교차를 체크
    std::vector<TxPoint> vecPolylinePt = spPolyline->GetPoints();
    size_t nxVertexCount = vecPolylinePt.size();
    if (nxVertexCount < 2)
        return false;
        
    // 확률적으로 대부분 폴리곤에 포함되었을 것이다.
    TxPoint oPt = vecPolylinePt[0];       // 시점체크
    if (IsInPoint(oPt) == true) 
        return true;

    oPt = vecPolylinePt[nxVertexCount-1]; // 종점체크
    if (IsInPoint(oPt) == true) 
        return true;

    // 여기까지 왔으면 : 폴리곤 홀에 포함된 경우, 시/종점이 폴리곤 외부에 위치하여 관통, 완벽하게 외부
    // 3. 모든 점이 Hole에 포함되는가 ?
    for (size_t i=0; i<GetHoleCount(); i++) {
        std::vector<TxPoint> vecPoint = GetHolePoints(i);
        TxPolygonPtr spPolygon = TxPolygonPtr(new TxPolygon(vecPoint));

        size_t nxInCount(0);
        for (size_t j=0; j<nxVertexCount; j++) {
            oPt = vecPolylinePt[0];  
            if ( spPolygon->IsInPoint(oPt) == true )
                nxInCount ++;
        }

        if (nxInCount == nxVertexCount)
            return false;
    }

    size_t  nxCount  = m_pOutRing->m_nxPointCnt;
    double* pPointsX = m_pOutRing->m_pPointsX;
    double* pPointsY = m_pOutRing->m_pPointsY;

    if (nxCount < 3)
        return false;

    TxPoint oIP;  bool bIntersect(false);

    for (size_t i=1; i<nxVertexCount; i++)
    {
        TxPoint& oFromPt = vecPolylinePt[i-1];
        TxPoint& oToPt   = vecPolylinePt[i  ];

        for (size_t j=1; j<nxCount; j++) 
        {
            TxPoint oFromPtPoly(pPointsX[j-1], pPointsY[j-1]);
            TxPoint oToPtPoly  (pPointsX[j  ], pPointsY[j  ]);

            bIntersect = TxMath::GetIntersectPoint(oFromPt, oToPt, oFromPtPoly, oToPtPoly, oIP, false);
            if (bIntersect == true)
                return true;
        }

        if (IsInPoint(oToPt) == true) 
            return true;
    }

    return false;
}

bool TxPolygon::Intersect( const TxPolyline& oPolyline ) const
{
    TxPolylinePtr spPolyline(new TxPolyline(oPolyline.GetPoints()));
    return Intersect(spPolyline);
}

bool TxPolygon::Intersect( std::shared_ptr<TxPolygon> spPolygon ) const
{
    // 1. Envelope 교차 체크
    TxEnvelope _oEnvelope = spPolygon->Envelope();
    if ( _oEnvelope.Intersect(m_oEnvelope) == false)
        return false;

    std::vector<TxPoint> vecMePoint = OutRing();
    std::vector<TxPoint> vecYuPoint = spPolygon->OutRing();
    // 2.   
    if (m_oEnvelope.Area() > _oEnvelope.Area()) {
        {// 나 자신의 외곽선안에 한점이라도 들어 오면 교차        
            TxPolygonPtr spOutRing(new TxPolygon(vecMePoint));                
            for (size_t i=0; i<vecYuPoint.size(); i++) {
                if (spOutRing->IsInPoint(vecYuPoint[i]) == true)
                    return true;
            }
        }
        {// 다른 놈의 외곽선에 한점이라도 들어 가면 교차
            TxPolygonPtr spOutRing(new TxPolygon(vecYuPoint));        
            for (size_t i=0; i<vecMePoint.size(); i++) {
                if (spOutRing->IsInPoint(vecMePoint[i]) == true)
                    return true;
            }
        }
    }
    else
    {
        {// 다른 놈의 외곽선에 한점이라도 들어 가면 교차
            TxPolygonPtr spOutRing(new TxPolygon(vecYuPoint));        
            for (size_t i=0; i<vecMePoint.size(); i++) {
                if (spOutRing->IsInPoint(vecMePoint[i]) == true)
                    return true;
            }
        }
        {// 나 자신의 외곽선안에 한점이라도 들어 오면 교차        
            TxPolygonPtr spOutRing(new TxPolygon(vecMePoint));                
            for (size_t i=0; i<vecYuPoint.size(); i++) {
                if (spOutRing->IsInPoint(vecYuPoint[i]) == true)
                    return true;
            }
        }
    }
        
    // 3. 라인의 교차 체크
    for (size_t i=0; i<vecMePoint.size()-1; i++) {
        TxPoint& oMeFm = vecMePoint[i];
        TxPoint& oMeTo = vecMePoint[i+1];

        for (size_t j=0; j<vecYuPoint.size()-1; j++) {
            TxPoint& oYuFm = vecYuPoint[j];
            TxPoint& oYuTo = vecYuPoint[j+1];

            TxPoint oIP;
            if ( TxMath::GetIntersectPoint(oMeFm, oMeTo, oYuFm, oYuTo, oIP, false) == true)
                return true;
        }
    }

    return true;
}

bool TxPolygon::Intersect( const TxPolygon& oPolygon ) const
{
    std::vector<TxPoint> vecPoint = oPolygon.OutRing();
    TxPolyline oPline(vecPoint);

    return Intersect(oPline);
}

bool TxPolygon::Intersect( std::shared_ptr<TxMPolygon> spMPolygon ) const
{
    size_t nxCount = spMPolygon->SubPolygonCount();
    for (size_t i=0; i<nxCount; i++) {
        TxPolygon*   pPolygon = spMPolygon->SubPolygon(i);
        TxPolygonPtr spPolygon(new TxPolygon(*pPolygon));

        if (Intersect(spPolygon) == true)
            return true;
    }

    return false;
}


ITxGeometryPtr TxPolygon::CopyGeometry()
{  
    return ITxGeometryPtr(new TxPolygon(*this));
}

bool TxPolygon::Contains( std::shared_ptr<TxPolyline> spPolyline ) const
{
    // 1. Envelope 교차 체크
    TxEnvelope _oEnvelope = spPolyline->Envelope();
    if (m_oEnvelope.Contains(_oEnvelope) == false)
        return false;

    // 2. 폴리라인을 루핑하면서 폴리곤 InPoint체크 : 모든 점은 폴리곤 안에 있어야 한다.
    std::vector<TxPoint> vecPoint = spPolyline->GetPoints();
    for (size_t i=0; i<vecPoint.size(); i++) {
        TxPoint& oPt = vecPoint[i];
        if (IsInPoint(oPt) == false) 
            return false;
    }
    // 3. 폴리 라인이 홀과 교차되는가 여부 : 교차되면 Out
    for (size_t i=0; i<m_vecHoleRing.size(); i++) {
        TxPolyline* pPolylineHole = m_vecHoleRing[i]; 
        size_t   nxCount  = pPolylineHole->m_nxPointCnt;
        double*  pPointsX = pPolylineHole->m_pPointsX;
        double*  pPointsY = pPolylineHole->m_pPointsY;
        
        for (size_t j=0; j<nxCount-1; j++)  {
            TxPoint oPnFrom(pPointsX[j],   pPointsY[j]  );
            TxPoint oPnTo  (pPointsX[j+1], pPointsY[j+1]);

            for (size_t k=0; k<vecPoint.size()-1; k++) {
                TxPoint& oLnFrom    = vecPoint[k];
                TxPoint& oLnToRight = vecPoint[k+1];

                TxPoint oIP;
                bool bIntersect = TxMath::GetIntersectPoint(oPnFrom, oPnTo, oLnFrom, oLnToRight, oIP, false);

                if (bIntersect)
                    return false;
            }
        }
    }

    return true;
}
/*
bool TxPolygon::IsContained( std::shared_ptr<TxPolyline> spPolyline ) const
{
    // 1. Envelope 교차 체크
    TxEnvelope _oEnvelope = spPolyline->Envelope();
    if ( _oEnvelope.IsIntersect(m_oEnvelope) == false)
        return false;
    // 2. 폴리라인을 루핑하면서 폴리곤 InPoint체크 및 교차 체크를 진행한다.
    std::vector<TxPoint> vecPolylinePt = spPolyline->GetPoints();
    size_t nxVertexCount = vecPolylinePt.size();
    if (nxVertexCount < 2)
        return false;

    TxPoint oPt = vecPolylinePt[0];       // 시점이 폴리곤 외부에 있자.
    if (IsInPoint(oPt) == false) 
        return false;

    oPt = vecPolylinePt[nxVertexCount-1]; // 종점이 폴리곤 외부에 있자.
    if (IsInPoint(oPt) == false) 
        return false;

    size_t  nxCount  = m_pOutRing->m_nxPointCount;
    double* pPointsX = m_pOutRing->m_pPointsX;
    double* pPointsY = m_pOutRing->m_pPointsY;

    if (nxCount < 3)
        return false;

    TxPoint oIP;  bool bIntersect(false);

    for (size_t i=1; i<nxVertexCount; i++)
    {
        TxPoint& oFromPt = vecPolylinePt[i-1];
        TxPoint& oToPt   = vecPolylinePt[i  ];

        for (size_t j=1; j<nxCount; j++) 
        {
            TxPoint oFromPtPoly(pPointsX[j-1], pPointsY[j-1]);
            TxPoint oToPtPoly  (pPointsX[j  ], pPointsY[j  ]);

            bIntersect = TxMath::GetIntersectPoint(oFromPt, oToPt, oFromPtPoly, oToPtPoly, oIP, false);
            if (bIntersect == true)
                return false;
        }

        if (IsInPoint(oToPt) == false) 
            return false;
    }

    return true;
}
*/
bool TxPolygon::Contains( const TxPolyline& oPolyline ) const
{
    std::shared_ptr<TxPolyline> spPolyline(new TxPolyline(oPolyline.GetPoints()));
    return Contains(spPolyline);
}


bool TxPolygon::Contains( const TxPoint& oPt ) const
{
    return IsInPoint(oPt);
}

bool TxPolygon::Contains(std::shared_ptr<TxPoint> spPoint) const
{
	TxPoint oPt(spPoint->X, spPoint->Y);
	return IsInPoint(oPt);
}


bool TxPolygon::Contains( std::shared_ptr<TxPolygon> spPolygon ) const
{
    // 외곽라인에 모두 포함되어야 한다.
    std::vector<TxPoint> vecPoint = spPolygon->OutRing();
    for (size_t i=0; i<vecPoint.size(); i++) {
        TxPoint& oPt = vecPoint[i];
        if (Contains(oPt) == false)
            return false;
    }

    // 홀안쪽에 있는지 여부 체크
    for (size_t i=0; i<m_vecHoleRing.size(); i++) {
        TxPolyline*  pPolylineHole = m_vecHoleRing[i]; 
        TxPolygonPtr spPolyHole(new TxPolygon(pPolylineHole->GetPoints()));
        if ( spPolyHole->Contains(spPolygon) == true )
            return false;
    }

    return true;
}

bool TxPolygon::Contains( std::shared_ptr<TxMPolygon> spMPolygon ) const
{
    size_t nxCount = spMPolygon->SubPolygonCount();
    for (size_t i=0; i<nxCount; i++) {
        TxPolygon* pPolygon = spMPolygon->SubPolygon(i);
        bool bContained = Contains(TxPolygonPtr(new TxPolygon(*pPolygon)));
        if (bContained == false)
            return false;
    }

    return true;
}

double TxPolygon::OverLineLength( std::shared_ptr<TxPolyline> spPolyline ) const
{
    TxEnvelope _oEnvelope = spPolyline->Envelope();
    if (m_oEnvelope.Intersect(_oEnvelope) == false)
        return 0.0;

    double dOverLineLength(0);
    std::vector<TxPoint> vecYuPlinePt   = spPolyline->GetPoints();
    std::vector<TxPoint> vecMeOutRingPt = OutRing();

    size_t nxCntYuPlinePt = vecYuPlinePt.size();
    size_t nxCntOutRingPt = vecMeOutRingPt.size();

    if (nxCntYuPlinePt < 2 || nxCntOutRingPt < 3)
        return 0.0;

    // 외곽 폴리곤과 교차하는 길이를 계산한다.
    for (size_t i=0; i<nxCntYuPlinePt-1; i++) {
        TxPoint& oYuFromPt = vecYuPlinePt[i  ];
        TxPoint& oYuToPt   = vecYuPlinePt[i+1];

        bool bYuFromPtIn = IsInPoint(oYuFromPt);
        bool bYuToPtIn   = IsInPoint(oYuToPt);

        if (bYuFromPtIn == true && bYuToPtIn == true) { // 두점이 폴리곤 안쪽에 있는 경우
            double dLength = TxMath::Dist(oYuFromPt, oYuToPt);
            dOverLineLength+= dLength;
            continue;
        }
        else {                                         // 두점이 폴리곤 밖에 있는 경우(관통), 두점 중 한쪽이 폴리곤 안에 있는 경우
            std::vector<TxPoint> vecIP;
            for (size_t j=0; j<nxCntOutRingPt-1; j++) {
                TxPoint& oMeFromPt = vecMeOutRingPt[j  ];
                TxPoint& oMeToPt   = vecMeOutRingPt[j+1];

                TxPoint oIP;
                if ( TxMath::GetIntersectPoint(oYuFromPt, oYuToPt, oMeFromPt, oMeToPt, oIP, false) == true )
                    vecIP.push_back(oIP);
            } 

            // 두점이 폴리곤 밖에 있으면서 교차가 없는 것은 정말 밖에 있는 선분
            if ((bYuFromPtIn == false && bYuToPtIn == false) && vecIP.size() == 0)
                continue;

            std::vector<TxPoint> vecPt;
            if (bYuFromPtIn == true)
                vecPt.push_back(oYuFromPt);

            for (size_t m=0; m<vecIP.size(); m++)
                vecPt.push_back(vecIP[m]);

            if (bYuToPtIn == true)
                vecPt.push_back(oYuToPt);

            // 라인의 시점 또는 종점이 폴리곤 선상에 있다.
            if (vecPt.size() == 1) continue;

            // 폴리곤의 모양이 골짜기일 경우에 대한 처리
            for (size_t k=0; 2*k+1 < vecPt.size(); k++) {
                double dLength  =  TxMath::Dist(vecPt[2*k], vecPt[2*k+1]);
                dOverLineLength += dLength;
            }
        }               
    }
    
    // 홀 폴리곤과 교차하는 길이를 계산한다. (외곽 폴리곤과 교차하는 길이 - 홀 폴리곤과 교차하는 길이)
    double dHoleOverLineLength(0.0);
    for (size_t i=0; i<m_vecHoleRing.size(); i++) {
        TxPolyline*   pHPolyline = m_vecHoleRing[i];
        TxPolygonPtr  spPolygon(new TxPolygon(pHPolyline->GetPoints()));
        
        dHoleOverLineLength += spPolygon->OverLineLength(spPolyline);
    }

    dOverLineLength -= dHoleOverLineLength;
    if (dOverLineLength < 0.0)
        dOverLineLength = 0.0; // 안전하게

    return dOverLineLength;
}

TxPoint TxPolygon::Centroid()
{ // 홀에 대해서는 배려하지 않는다.
    TxPoint oCPt(0,0);
    double signedArea = 0.0;
    double x0 = 0.0; // Current vertex X
    double y0 = 0.0; // Current vertex Y
    double x1 = 0.0; // Next vertex X
    double y1 = 0.0; // Next vertex Y
    double a = 0.0;  // Partial signed area

    size_t  nxPointCount = m_pOutRing->m_nxPointCnt;
    double* pPointsX     = m_pOutRing->m_pPointsX;
    double* pPointsY     = m_pOutRing->m_pPointsY;

    for (size_t i=0; i<nxPointCount-1; ++i)
    {
        x0 = pPointsX[i];
        y0 = pPointsY[i];
        x1 = pPointsX[(i+1) % nxPointCount];
        y1 = pPointsY[(i+1) % nxPointCount];
        a = x0*y1 - x1*y0;
        signedArea += a;

        oCPt.X += (x0 + x1)*a;
        oCPt.Y += (y0 + y1)*a;
    }

    signedArea *= 0.5;
    oCPt.X /= (6.0*signedArea);
    oCPt.Y /= (6.0*signedArea);


    return oCPt;
}

BoostPolygon TxPolygon::ToBoost()
{
    BoostPolygon oBoostPoly;

    std::vector<BoostPoint> vecBxOut;
    size_t nxPointCount = m_pOutRing->m_nxPointCnt;
    for (size_t i=0; i<nxPointCount; i++) {
        vecBxOut.push_back(BoostPoint(m_pOutRing->m_pPointsX[i], m_pOutRing->m_pPointsY[i]));
    }
    //★ OutRing 추가
    boost::geometry::append(oBoostPoly, vecBxOut);
    
    //★ Hole 추가
    size_t nxHoleCnt = GetHoleCount();
    if (nxHoleCnt <= 0)
        return oBoostPoly;

    boost::geometry::interior_rings(oBoostPoly).resize(nxHoleCnt);
    for (size_t i=0; i<nxHoleCnt; i++) {
        std::vector<TxPoint> vecHole = GetHolePoints(i);

        std::vector<BoostPoint> vecBxIn;
        for (size_t k=0; k<vecHole.size(); k++) 
        {
            TxPoint& oPt = vecHole[k];
            vecBxIn.push_back(oPt);
        }
        boost::geometry::append(oBoostPoly, vecBxIn, (int)i);
    } 

    return oBoostPoly;
}

TxMPolygon::TxMPolygon() : ITxGeometry(GeoMPolygonType)
{
}

TxMPolygon::TxMPolygon( const TxMPolygon& right ) : ITxGeometry(GeoMPolygonType)
{
    size_t nxCount = right.m_vecSimplePolygon.size();
    for (size_t i=0; i<nxCount; i++)
    {
        TxPolygon* pSimplePolygon = right.m_vecSimplePolygon[i];
        if (pSimplePolygon)
        {
            m_vecSimplePolygon.push_back(new TxPolygon(*pSimplePolygon));            
        }
    }

    m_oEnvelope = right.m_oEnvelope;
}

TxMPolygon::TxMPolygon( QBicBinary& oBinary, size_t nIndex/*=0*/ ) : ITxGeometry(GeoMPolygonType)
{
    oBinary.Position(nIndex);

    int nGeometryType(0);
    oBinary.Read(nGeometryType);

    int nSimplePolyCount(0);
    oBinary.Read(nSimplePolyCount);

    for(int i=0; i<nSimplePolyCount; i++)
    {
        TxPolygon* pSimplePolygon = new TxPolygon(oBinary, oBinary.Position());
        m_vecSimplePolygon.push_back(pSimplePolygon);

        if (i == 0)
            m_oEnvelope = pSimplePolygon->Envelope();
        else
            m_oEnvelope.Union(pSimplePolygon->Envelope());
    }
}

TxMPolygon::~TxMPolygon()
{
    Clear();
}

TxMPolygon& TxMPolygon::operator=( const TxMPolygon& right )
{
    return *this;
}

void TxMPolygon::GetBinary( QBicBinary& oBinary )
{
    size_t nxCount = m_vecSimplePolygon.size();

    // 타입 
    // SimplePolygon 갯수
    // SimplePolygon 정보
    oBinary.PushBack((int)m_enGeometryType);
    oBinary.PushBack((int)nxCount);
    
    for (size_t i=0; i<nxCount; i++)
    {
        TxPolygon* pSimplePolygon = m_vecSimplePolygon[i];
        pSimplePolygon->GetBinary(oBinary);
    }
}

void TxMPolygon::Clear()
{
    size_t nxCount = m_vecSimplePolygon.size();
    for (size_t i=0; i<nxCount; i++)
    {
        TxPolygon* pSimplePolygon = m_vecSimplePolygon[i];
        delete pSimplePolygon;
    }

    m_vecSimplePolygon.clear();
    m_oEnvelope = TxEnvelope(0.0, 0.0);
}

double TxMPolygon::Area()
{
    double dArea(0.0);

    size_t nxCount = m_vecSimplePolygon.size();
    for (size_t i=0; i<nxCount; i++)
    {
        TxPolygon* pSimplePolygon = m_vecSimplePolygon[i];
        dArea += pSimplePolygon->Area();
    }

    return dArea;
}

bool TxMPolygon::IsInPoint( TxPoint& oPt )
{
    size_t nxCount = m_vecSimplePolygon.size();
    for (size_t i=0; i<nxCount; i++)
    {
        TxPolygon* pSimplePolygon = m_vecSimplePolygon[i];
        if (pSimplePolygon->IsInPoint(oPt) == true)
            return true;
    }

    return false;
}

void TxMPolygon::AddSubPolygon( const TxPolygon& oSimplePoly )
{
    TxPolygon* pSimplePolygon = new TxPolygon(oSimplePoly);
    m_vecSimplePolygon.push_back(pSimplePolygon);

    m_oEnvelope.Union(oSimplePoly.Envelope());    
}

void TxMPolygon::AddSubPolygon( std::shared_ptr<TxPolygon> spPoly )
{
    TxPolygon* pSimplePolygon = new TxPolygon(spPoly);
    m_vecSimplePolygon.push_back(pSimplePolygon);

    m_oEnvelope.Union(spPoly->Envelope());   
}

CString TxMPolygon::ToString()
{
    size_t nxCount = m_vecSimplePolygon.size();

    CString strOut(_T(""));
    strOut.AppendFormat(_T("\nTxPolygon : Count<%d>"), nxCount);
    
    for (size_t i=0; i<nxCount; i++)
    {
        TxPolygon* pSimplePolygon = m_vecSimplePolygon[i];
        strOut.AppendFormat(_T("\n[%d])"), i);
        strOut.AppendFormat(_T("%s"), pSimplePolygon->ToString());
    }

    return strOut;
}

double TxMPolygon::MinDist( const TxPoint& oPt )
{
    double dMinDist(TxConst::MaxDbl());
    double dDist(0.0);
    for (size_t i=0; i<m_vecSimplePolygon.size(); i++)
    {
        TxPolygon* pPolygon = m_vecSimplePolygon[i];
        dDist = pPolygon->MinDist(oPt);

        if (dMinDist > dDist)
            dMinDist = dDist;
    }
    
    return dMinDist;
}

bool TxMPolygon::Intersect( const TxEnvelope& _oEnvelope ) const
{
    bool bResult(false);

    for (size_t i=0; i<m_vecSimplePolygon.size(); i++)
    {
        TxPolygon* pSimplePoly = m_vecSimplePolygon[i];
        if ( pSimplePoly->Intersect(_oEnvelope) == true )
        {
            bResult = true;
            break;
        }
    }

    return bResult;
}

bool TxMPolygon::Intersect( const TxPolyline& oPolyline ) const
{
    bool bResult(false);

    for (size_t i=0; i<m_vecSimplePolygon.size(); i++)
    {
        TxPolygon* pSimplePoly = m_vecSimplePolygon[i];
        if ( pSimplePoly->Intersect(oPolyline) == true )
        {
            bResult = true;
            break;
        }
    }

    return bResult;
}

bool TxMPolygon::Intersect( std::shared_ptr<TxPolyline> spPolyline ) const
{
    bool bResult(false);

    for (size_t i=0; i<m_vecSimplePolygon.size(); i++)
    {
        TxPolygon* pSimplePoly = m_vecSimplePolygon[i];
        if ( pSimplePoly->Intersect(spPolyline) == true )
        {
            bResult = true;
            break;
        }
    }

    return bResult;
}

bool TxMPolygon::Intersect( std::shared_ptr<TxPolygon> spPolygon ) const
{
    bool bResult(false);

    for (size_t i=0; i<m_vecSimplePolygon.size(); i++)
    {
        TxPolygon* pSimplePoly = m_vecSimplePolygon[i];
        if ( pSimplePoly->Intersect(spPolygon) == true )
        {
            bResult = true;
            break;
        }
    }

    return bResult;
}

bool TxMPolygon::Intersect( const TxPolygon& oPolygon ) const
{
    bool bResult(false);

    for (size_t i=0; i<m_vecSimplePolygon.size(); i++)
    {
        TxPolygon* pSimplePoly = m_vecSimplePolygon[i];
        if ( pSimplePoly->Intersect(oPolygon) == true )
        {
            bResult = true;
            break;
        }
    }

    return bResult;
}

bool TxMPolygon::Contains( const TxPolyline& oPolyline ) const
{
    bool bResult(false);

    for (size_t i=0; i<m_vecSimplePolygon.size(); i++)
    {
        TxPolygon* pSimplePoly = m_vecSimplePolygon[i];
        if ( pSimplePoly->Contains(oPolyline) == true )
        {
            bResult = true;
            break;
        }
    }

    return bResult;
}

bool TxMPolygon::Contains( std::shared_ptr<TxPolyline> spPolyline ) const
{
    bool bResult(false);

    for (size_t i=0; i<m_vecSimplePolygon.size(); i++)
    {
        TxPolygon* pSimplePoly = m_vecSimplePolygon[i];
        if ( pSimplePoly->Contains(spPolyline) == true )
        {
            bResult = true;
            break;
        }
    }

    return bResult;
}

bool TxMPolygon::Contains(const TxPoint& oPt ) const
{
    bool bResult(false);

    for (size_t i=0; i<m_vecSimplePolygon.size(); i++)
    {
        TxPolygon* pSimplePoly = m_vecSimplePolygon[i];
        if ( pSimplePoly->Contains(oPt) == true )
        {
            bResult = true;
            break;
        }
    }

    return bResult;
}

bool TxMPolygon::Contains(std::shared_ptr<TxPoint> spPoint) const
{
	bool bResult(false);

	TxPoint oPt(spPoint->X, spPoint->Y);

	for (size_t i=0; i<m_vecSimplePolygon.size(); i++)
	{
		TxPolygon* pSimplePoly = m_vecSimplePolygon[i];
		if ( pSimplePoly->Contains(oPt) == true )
		{
			bResult = true;
			break;
		}
	}

	return bResult;
}

bool TxMPolygon::Contains( std::shared_ptr<TxPolygon> spPolygon ) const
{
    bool bResult(false);

    for (size_t i=0; i<m_vecSimplePolygon.size(); i++)
    {
        TxPolygon* pSimplePoly = m_vecSimplePolygon[i];
        if ( pSimplePoly->Contains(spPolygon) == true )
        {
            bResult = true;
            break;
        }
    }

    return bResult;
}

ITxGeometryPtr TxMPolygon::CopyGeometry()
{
    return ITxGeometryPtr(new TxMPolygon(*this));
}

double TxMPolygon::OverLineLength( std::shared_ptr<TxPolyline> spPolyline ) const
{
    double dOverLineLength(0);

    for (size_t i=0; i<m_vecSimplePolygon.size(); i++)
    {
        TxPolygon* pSimplePoly = m_vecSimplePolygon[i];
        dOverLineLength += pSimplePoly->OverLineLength(spPolyline);
    }

    return dOverLineLength;
}
