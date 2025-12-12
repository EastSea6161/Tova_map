#pragma once

extern const int DLL_USER_STOPPING;
extern const int DLL_UNEXPECTED_ERROR;
extern const int DLL_LOAD_ERROR;
extern const int DLL_EXPECTED_ERROR;
extern const int DLL_PLANNED_TERMINATION;
extern const int DLL_MESSAGE_RECEIVE;
extern const int DLL_CREATE_ARGFILE_ERROR;
extern const int DLL_READ_OUTPUTFILE_ERROR;
extern const int KLAUNCHER_USER_FORCE_STOP;

class KLauncher
{
public:
    KLauncher();
    ~KLauncher();
public:
    static void Run(CString a_strWindowName, int a_nBusinessProcessID); 
    static void Run64(CString a_strWindowName, int a_nBusinessProcessID);
};

class KPolicyLauncher
{
public:
    KPolicyLauncher();
    ~KPolicyLauncher();
public:
    static void Run(CString a_strPolicyPath, int a_nBusinessProcessID ); 
};