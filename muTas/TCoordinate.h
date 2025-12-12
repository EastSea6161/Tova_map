#pragma once

class TCoordinate
{
public:
    TCoordinate(double x, double y)  { Tx = x; Ty = y; }
    ~TCoordinate() {}

public:
    double Tx;
    double Ty;
};