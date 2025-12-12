#include "stdafx.h"
#include "MakeTurnDispShp.h"

TxPolylinePtr MakeTurnDispShp::MakeTurnDisp( TxPolylinePtr _spInLine, TxPolylinePtr _spOutLine )
{
    TxPolylinePtr spPLine;

    double dOffset(5), dFlag(10);
    double dLinePosRate(0.5);

    TxPolylinePtr spInLine; {
        std::vector<TxPoint> vecPt = _spInLine->GetOffsetPoint(dOffset, dFlag);
        spInLine = TxPolylinePtr(new TxPolyline(vecPt));
    }
    
    TxPolylinePtr spOutLine; {
        std::vector<TxPoint> vecPt = _spOutLine->GetOffsetPoint(dOffset, dFlag);
        spOutLine = TxPolylinePtr(new TxPolyline(vecPt));
    }

    double dTrLen(0);
    double dDistIn  = spInLine->Length();
    double dDistOut = spOutLine->Length();

    double dMinLen(0);
    if (dDistIn < dDistOut) {
        dTrLen  = dDistIn * dLinePosRate;
        dMinLen = dDistIn;
    }
    else {
        dTrLen  = dDistOut * dLinePosRate;
        dMinLen = dDistOut;
    }

    if (dTrLen <= 20) {
        dTrLen = dMinLen;
    }

    TxPoint oPt1; {
        spInLine->LineOnPoint(dTrLen, oPt1);
    }
    TxPoint oPt2 = spInLine->EndPoint();
    
    TxPoint oPt3 = spOutLine->StartPoint();
    TxPoint oPt4; {
        spOutLine->LineOnPoint(dTrLen, oPt4);
    }
     
    std::vector<TxPoint> vecCurvePt = TxMakeCurve::SimpleCurve(oPt1, oPt2, oPt3, oPt4);
    spPLine = TxPolylinePtr(new TxPolyline(vecCurvePt));
    return spPLine;
}
