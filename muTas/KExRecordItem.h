/**
 * @file KExRecordItem.h
 * @brief KExRecordItemCheck, KExRecordItem2GroupTextNo 구현 클래스
 * @author nomplus2@qbicware.com
 * @date 2011.08.26
 * @remark 
 */


#pragma once


/**
* @brief Codejock RecordItem Check Box
* @remark 
* @warning 
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class KExRecordItemCheck : public CXTPReportRecordItem
{
    DECLARE_SERIAL(KExRecordItemCheck)
public:
        
    /**
    * @brief Constructs record item with the initial checkbox value.
    * @param[in] bCheck - Check 여부
    */
    KExRecordItemCheck(BOOL bCheck = FALSE);

    /**
    * @brief Provides custom group captions depending on checkbox value
    * @param[in] bCheck - Check 여부
    * @return caption string ID to be read from application resources
    */
    virtual int GetGroupCaptionID(CXTPReportColumn* pColumn);
    
    /**
    * @brief Provides custom records comparison by this item based on checkbox value instead of based on captions
    * @param[in] pColumn(컬럼)
    * @param[in] pItem(아이템)
    * @return 0, 1
    */
    virtual int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);
};

/**
* @brief KExRecordItem2GroupTextNo
* @remark 발생, 도착 그룹핑 
* @warning 
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class KExRecordItem2GroupTextNo : public CXTPReportRecordItemNumber
{
    DECLARE_SERIAL(KExRecordItem2GroupTextNo)
public:    
    
    KExRecordItem2GroupTextNo(double dValue = .0);
    KExRecordItem2GroupTextNo(CString strName, double dValue = .0);   
    
    /**
    * @brief Group Caption 정보 조회
    * @param[in] pColumn
    * @return Caption
    */
    virtual CString GetGroupCaption(CXTPReportColumn* pColumn);

    /**
    * @brief Group Caption 비교
    * @param[in] pColumn - 컬럼
    * @param[in] pItem   - 아이템
    * @return A negative value if the first string is less than the second string, 0 if the two strings are equal, or a positive value if the first string is greater than the second string.
    * @remark 
    * @warning 
    */
    virtual int CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);
private:    
    CString m_strName; /// Caption 정보 저장
};

class KExRecordItemCombo : public CXTPReportRecordItem
{
public :
    KExRecordItemCombo( int nIndex ) : m_nIndex( nIndex )
    {

    }

    CString GetCaption(CXTPReportColumn* pColumn)
    {
        CXTPReportRecordItemConstraint* pConstraint = pColumn->GetEditOptions()->FindConstraint( m_nIndex );
        ASSERT(pConstraint);
        return pConstraint->m_strConstraint;
    }

    int GetIndex( void )
    {
        return m_nIndex;
    }

    DWORD GetSelectedConstraintData(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/)
    {
        return m_nIndex;
    }

    virtual void OnConstraintChanged( XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraint* pConstraint )
    {
        if( pConstraint )
        {
            m_nIndex = pConstraint->m_dwData;
        }
    }
private:
    int m_nIndex;
};


class CXTPReportRecordItemComboEx : public CXTPReportRecordItem
{
public :
    CXTPReportRecordItemComboEx( int nIndex = -1) : m_nSelectedIndex( nIndex )
    {
        CXTPReportRecordItemEditOptions* pEditOptions = GetEditOptions(NULL);
        pEditOptions->AddComboButton(TRUE);
		
		pEditOptions->m_bAllowEdit = TRUE;
		//pEditOptions->m_bConstraintEdit = TRUE;
		pEditOptions->m_bExpandOnSelect = TRUE;
    }

    CString GetCaption(/*CXTPReportColumn* pColumn*/)
    {
        if (m_nSelectedIndex < 0)
            return _T("");

        KeyData& oKeyData  = m_vecComboItem[m_nSelectedIndex];
        return oKeyData.m_strCaption;
    }

    CString GetCaption(CXTPReportColumn* pColumn)
    {
        return GetCaption();
    }

    virtual void OnConstraintChanged( XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraint* pConstraint )
    {
        if( pConstraint )
        {
            m_nSelectedIndex = pConstraint->GetIndex();        
        }
    }
private:
    int m_nSelectedIndex;
    
    class KeyData
    {
    public:
        KeyData() : m_strCaption(_T("")), m_dwpData(0)
        {
        }
        KeyData(CString a_strCaption) : m_strCaption(a_strCaption), m_dwpData(0)
        {
        }
        KeyData(CString a_strCaption, DWORD_PTR a_dwpData) : m_strCaption(a_strCaption), m_dwpData(a_dwpData)
        {
        }
    public:
        CString   m_strCaption;
        DWORD_PTR m_dwpData;
    };

private:
    std::vector<KeyData> m_vecComboItem;
public:
    int AddString(CString a_strCaption, DWORD_PTR a_dwpData=0)
    {
        m_vecComboItem.push_back(KeyData(a_strCaption, a_dwpData));
        int nIndex = (int)m_vecComboItem.size() -1;

        CXTPReportRecordItemEditOptions* pEditOptions = GetEditOptions(NULL);
        pEditOptions->AddConstraint(a_strCaption, a_dwpData);

        return nIndex;
    }

    void SetItemData(int a_nIndex, DWORD_PTR a_dwpData)
    {
        int nSize = (int)m_vecComboItem.size();
        if (a_nIndex < 0)
            return;
        if (nSize <= a_nIndex)
            return;

        KeyData& oKeyData  = m_vecComboItem[a_nIndex];
        oKeyData.m_dwpData = a_dwpData;
    }

    DWORD_PTR GetItemData(int a_nIndex)
    {
        int nSize = (int)m_vecComboItem.size();
        if (a_nIndex < 0)
            return 0;
        if (nSize <= a_nIndex)
            return 0;

        KeyData& oKeyData  = m_vecComboItem[a_nIndex];
        return oKeyData.m_dwpData;
    }

    int GetCurSel( void )
    {
        return m_nSelectedIndex;
    }  

    void SetCurSel(int a_nIndex)
    {
        int nSize = (int)m_vecComboItem.size();
        if (a_nIndex < 0)
            return;
        if (nSize <= a_nIndex)
            return;

        m_nSelectedIndex = a_nIndex;
    }

    void ResetContent()
    {
        m_nSelectedIndex = -1;
        m_vecComboItem.clear();

        CXTPReportRecordItemEditOptions* pEditOptions = GetEditOptions(NULL);
        CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
        if (pConstraints)
            pConstraints->RemoveAll();
    }
};