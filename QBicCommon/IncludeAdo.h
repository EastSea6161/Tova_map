#pragma once

#ifdef WIN64 
#import "C:\Program Files\common files\system\ado\msado15.dll" named_guids rename("EOF", "bEOF") rename("BOF", "bBOF") rename ("_Connection", "AdoConnection") rename ("_Command", "AdoCommand") rename("Parameter", "AdoParameter") rename ("_Recordset", "AdoRecordset") rename("Record", "AdoRecord")
#else
#import "C:\Program Files (x86)\common files\system\ado\msado15.dll" named_guids rename("EOF", "bEOF") rename("BOF", "bBOF") rename ("_Connection", "AdoConnection") rename ("_Command", "AdoCommand") rename("Parameter", "AdoParameter") rename ("_Recordset", "AdoRecordset") rename("Record", "AdoRecord")
#endif

#pragma warning(default:4146)
using namespace ADODB; 
