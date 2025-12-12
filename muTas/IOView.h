/**
 * @file IOView.h
 * @brief KIOView 선언 파일
 * @author 
 * @date 2011.03.31
 * @remark
 */


#pragma once



/* Forward declarations */
class KIOTable;
class KIOTableView;



/**
 * @brief KIOTableView의 tab control 관리 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.03.31
 */
class KIOView : public CView
{
	DECLARE_DYNCREATE(KIOView)

protected:
	KIOView();
	virtual ~KIOView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	
	class KIOTabControl : public CXTPTabControl
	{
		void OnNavigateButtonClick(CXTPTabManagerNavigateButton* pButton);
	};

public :
	/**
	 * @brief 새로운 Table view를 생성하고 생성된 view를 반환한다.
	 * @param[in] pTable - View에 표출될 table 정보
	 * @return KIOTableView* - 새로 생성된 table view
	 */
	KIOTableView* CreateNewTableView(KIOTable* pTable);

	/**
	 * @brief 주어진 이름의 table을 표출 중인 view를 반환
	 * @param[in] strTableName - Table 이름
	 * @return KIOTableView* - strtableName의 table을 표출 중인 view 정보,
	                             해당 view가 없을 경우 NULL을 반환한다.
	 */
	KIOTableView* FindTableView(LPCTSTR strTableName);

	/**
	 * @brief 주어진 이름의 table을 표출 중인 view의 tab 순서를 반환한다.
	 * @param[in] strTableName - Table 이름
	 * @return int - 해당 view의 tab 순서(index), -1:해당 view 없음
	 */
	int GetTableViewIndex(LPCTSTR strTableName);

	/**
	 * @brief 주어진 이름의 table을 표출중인 view 또는 해당 Index의 view를 active view로 설정한다.
	 * @param[in] strTableName - table 이름
	 * @param[in] nIndex - View의 Index
	 */
	void SetActiveTableView(LPCTSTR strTAbleName);
	void SetActiveTableView(int nIndex);


	/**
	 * @brief 주어진 이름의 table을 표출 하는 view를 닫는다.
	 * @param[in] strTableName - Table 이름
	 */
	void CloseTableView(LPCTSTR strTableName);

	/**
	 * @brief 현재 활성된 Table view를 반환한다.
	 * @return KIOTableView* - 활성 table view
	 */
	KIOTableView* GetActiveTableView(void);

	/**
	* @brief IO View가 포함하고 있는 Table View의 갯수를 반환한다.
	* @return int - Table View의 갯수 
	*/
	int GetTableViewCount(void);

	/**
	* @brief 해당 Index의 Table View를 반환한다.
	* @param[in] nIndex - Table view의 Index
	* @return KIOTableView - 해당 Indexd의 table view
	*/
	KIOTableView* GetTableView(int nIndex);

protected :
	KIOTabControl m_wndTabCtrl;
	//CXTPTabControl m_wndTabCtrl;
	KIOTableView* CreateIOTableView(KIOTable* pTable);
	KIOTableView* CreateTransitTableView(KIOTable* pTable);
    KIOTableView* CreateTurnTableView(KIOTable* pTable);
    KIOTableView* CreateIntersectionTableView(KIOTable* pTable);
	KIOTableView* CreateTerminalScheduleTableView(KIOTable* pTable);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();

public:
	void ActivatedFrameWndNotifyProcess() ;
	void DeActivatedFrameWndNotifyProcess();
};


