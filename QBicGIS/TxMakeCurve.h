#pragma once
#include "TxMath.h"

class AFX_EXT_CLASS TxMakeCurve
{
public:
    TxMakeCurve() {}
    virtual ~TxMakeCurve() {}

public:
    //★ 두개의 직선(oPt1~oPt2, oPt3~oPt4)이 주어졌을 때. 두직선을 연결한 Simple Curve(단곡선)을 리턴
    static std::vector<TxPoint> SimpleCurve(TxPoint oPt1, TxPoint oPt2, TxPoint oPt3, TxPoint oPt4, double dTailLen = 30);
};