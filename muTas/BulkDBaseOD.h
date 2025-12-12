#pragma once

class KBulkDBaseOD
{
public:
    KBulkDBaseOD() {}
    ~KBulkDBaseOD(){}

static void DeleteOD(KDBaseConPtr a_spDBaseConnection, CString a_strTableName, Integer a_nxZoneID);

};