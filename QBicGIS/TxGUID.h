#pragma once

class AFX_EXT_CLASS TxGUID
{
private:
    TxGUID() {};
    ~TxGUID(){};
public:
    static CString GUID()
    {
        CString strGUID(_T(""));
        _GUID oGUID;
        CoCreateGuid(&oGUID);

        strGUID.Format(_T("%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X"), 
            oGUID.Data1,    oGUID.Data2,    oGUID.Data3,    oGUID.Data4[0],
            oGUID.Data4[1], oGUID.Data4[2], oGUID.Data4[3], oGUID.Data4[4],
            oGUID.Data4[5], oGUID.Data4[6], oGUID.Data4[7]);

        return strGUID;
    }
};