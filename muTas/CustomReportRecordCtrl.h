#pragma once
class KCustomReportRecordCtrl : public CXTPReportControl
{
public:
	KCustomReportRecordCtrl(void);
	~KCustomReportRecordCtrl(void);

private:
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point, int nState);
};

class KColumnReportRecordItem : public CXTPReportRecordItem
{
public :
	KColumnReportRecordItem( int nIndex ) : m_nIndex( nIndex )
	{

	}

	CString GetCaption(CXTPReportColumn* pColumn)
	{
		CXTPReportRecordItemConstraint* pConstraint = pColumn->GetEditOptions()->FindConstraint( m_nIndex );
		ASSERT(pConstraint);
		return pConstraint->m_strConstraint;
	}

	int getColumnIndex( void )
	{
		return m_nIndex;
	}

	DWORD GetSelectedConstraintData(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/)
	{
		return m_nIndex;
	}

	virtual void OnConstraintChanged( XTP_REPORTRECORDITEM_ARGS* pItemArgs, 
		CXTPReportRecordItemConstraint* pConstraint )
	{
		if( pConstraint )
		{
			m_nIndex = pConstraint->m_dwData;
		}
	}
	int m_nIndex;
};