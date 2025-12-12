#pragma once

class MakeTurnDispShp
{
public:
    MakeTurnDispShp() {}
    ~MakeTurnDispShp(){}

public:
    static TxPolylinePtr MakeTurnDisp(TxPolylinePtr spInLine, TxPolylinePtr spOutLine);
};
