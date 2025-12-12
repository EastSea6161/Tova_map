#include "StdAfx.h"
#include "DBaseIOTerminalScheduleTableView.h"

#include "Target.h"


KDBaseIOTerminalScheduleTableView::KDBaseIOTerminalScheduleTableView(void)
{
}


KDBaseIOTerminalScheduleTableView::~KDBaseIOTerminalScheduleTableView(void)
{
}

void KDBaseIOTerminalScheduleTableView::GetODTerimnalBySEQ( KTarget* a_pTarget, std::set<Integer> a_setTerminalID, std::vector<TTerminalInfo>& a_vecTerminalInfo )
{
	a_vecTerminalInfo.clear();
	CString strQuery = _T("SELECT seq, o_terminal, d_terminal FROM terminal_schedule WHERE seq = ?");
	
	try
	{
		KDBaseConPtr   spDBaseConnection   = a_pTarget->GetDBaseConnection();
		KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strQuery);

		std::set<Integer>::iterator bIter = a_setTerminalID.begin();
		std::set<Integer>::iterator eIter = a_setTerminalID.end();

		while (bIter != eIter)
		{
			spPreparedStatement->BindInt64(1, *bIter);
			
			KResultSetPtr spResultSet = spPreparedStatement->ExecuteQuery();

			if (!spResultSet->Next())
				ThrowException(_T("일치하는 Terminal Seq 데이터가 없습니다."));

			TTerminalInfo oTerminalInfo = {};

			oTerminalInfo.nxSeq       = spResultSet->GetValueInt64(0);
			oTerminalInfo.nxOTerminal = spResultSet->GetValueInt64(1);
			oTerminalInfo.nxDTerminal = spResultSet->GetValueInt64(2);

			a_vecTerminalInfo.push_back(oTerminalInfo);

			spPreparedStatement->Reset();

			++bIter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}
