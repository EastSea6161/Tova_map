#include "StdAfx.h"
#include "WriteCodeAgrument.h"
//^#include "DBaseConnector.h"
#include "Target.h"

using namespace std;

KWriteCodeAgrument::KWriteCodeAgrument(void)
{
}


KWriteCodeAgrument::~KWriteCodeAgrument(void)
{
}

void KWriteCodeAgrument::WirteMatchingCodeArument( KTarget* a_pTarget, CString a_strFolderName )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		CString strFileName = _T("DefineNetworkType.arg");

		CString strSavePath(_T(""));
		strSavePath.Format(_T("%s\\%s\\%s"), a_pTarget->GetLocation(), a_strFolderName, strFileName);

		int nResult = remove((CStringA)strSavePath);

		UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
		CStdioFileEx of(strSavePath, nOpenMode);

		std::map<int, int> mapMatchingNodeCode;
		std::map<int, int> mapMatchingLinkCode;

		CString strNodeSQL(_T(""));
		strNodeSQL.Format(_T("SELECT user_code_key, system_code_key FROM %s Where user_code_group = %d  "), TABLE_DEFINE_SYSTEM_CODE, USER_NODECODE_GROUP_KEY);

		KResultSetPtr  spNodeResultSet  = spDBaseConnection->ExecuteQuery(strNodeSQL);

		while ( spNodeResultSet->Next() )
		{
			int nUserCode	= spNodeResultSet->GetValueInt(0);
			int nSystemCode	= spNodeResultSet->GetValueInt(1);

			mapMatchingNodeCode.insert(std::make_pair(nUserCode, nSystemCode));
		}


		CString strLinkSQL(_T(""));
		strLinkSQL.Format(_T("SELECT user_code_key, system_code_key FROM %s Where user_code_group = %d  "), TABLE_DEFINE_SYSTEM_CODE, USER_LINKCODE_GROUP_KEY);

		KResultSetPtr  spLinkResultSet  = spDBaseConnection->ExecuteQuery(strLinkSQL);

		while ( spLinkResultSet->Next() )
		{
			int nUserCode	= spLinkResultSet->GetValueInt(0);
			int nSystemCode	= spLinkResultSet->GetValueInt(1);

			mapMatchingLinkCode.insert(std::make_pair(nUserCode, nSystemCode));
		}

		//argument 작성
		
		int nNodeCount = mapMatchingNodeCode.size();
		CString strOut(_T(""));
		strOut.Format(_T("Node\t%d\r\n"),nNodeCount);

		of.WriteString(strOut);
		
		AutoType Nodeiter = mapMatchingNodeCode.begin();
		AutoType Nodeend  = mapMatchingNodeCode.end();

		while(Nodeiter != Nodeend)
		{
			int nUserCode	= Nodeiter->first;
			int nSystemcode = Nodeiter->second;

			strOut.Format(_T("%d\t%d\r\n"), nUserCode, nSystemcode);
			of.WriteString(strOut);
			Nodeiter++;
		}

		//link arg 생성
		int nLinkCount = mapMatchingLinkCode.size();
		strOut.Format(_T("Link\t%d\r\n"),nLinkCount);

		of.WriteString(strOut);

		AutoType Linkiter = mapMatchingLinkCode.begin();
		AutoType Linkend  = mapMatchingLinkCode.end();

		while(Linkiter != Linkend)
		{
			int nUserCode	= Linkiter->first;
			int nSystemcode = Linkiter->second;

			strOut.Format(_T("%d\t%d\r\n"), nUserCode, nSystemcode);
			of.WriteString(strOut);
			Linkiter++;
		}

		of.Close();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}

}
