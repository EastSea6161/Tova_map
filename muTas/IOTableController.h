/**
 * @file IOTableController.h
 * @brief KIOTableController 선언 파일
 * @author 
 * @date 2011.04.13
 * @remark
 */



#pragma once

#include "PropertiesDlg.h"
#include "TransitPathViewDlg.h"
#include "EditAccessLineStationDlg.h"
#include "PrepareChartTransitDlg.h"
#include "LinkViaTransitSearchDlg.h"
#include "IntersectionEditMainDlg.h"
#include "intersection_common.h"
//^#include "ParseString.h"
/* Forward declarations */
class KIORow;
class KIOTable;
class KIOTableView;
class KTarget;
class KMapView;
class KDlgTransitStyle;

/**
 * @brief View와 Data의 controller class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.13
 */
class KIOTableController
{
public:
	KIOTableController(KTarget* pTarget);
	virtual ~KIOTableController(void);
    
public :
    KMapView* ShowMapView(bool bShow = true);
	/**
	 * @brief Contoller에 model과 view를 할당한다.
	 * @param[in] pTable - model
	 * @param[in] pView - view
	 */
	void Attach(KIOTable* pTable, KIOTableView* pView);

	/**
	 * @brief Controller의 이름을 반환한다.
	 *          Controller의 이름은 model이 이름이며 Attach시 할당된다.
	 * @return LPCTSTR - Controller의 이름
	 */
	LPCTSTR Name(void);

	/**
	 * @brief TableView에서 선택 된 row만 보이도록 처리한다.
	 * @return bool - true;정상수행, false:실패
	 */
	bool SelectRow(CString a_strOriginalFilter);

	/**
	 * @brief TableView에서 선택 된 row만 보이도록 처리한다. SelectRow Upgrade 버젼
	 * @return bool - true;정상수행, false:실패
	 */
	bool QuickSearch(void);

	/**
	 * @brief Model에 row를 추가한다.
	 */
	void AddRow();

    void DeleteRowData(const KIORow* pIORow);

	/**
	* @brief Table의 field를 특정 값으로 채운다.
	* @return bool - true;정상수행, false:실패
	*/
	bool CalculateField(CString a_strFilter=_T(""));

	/**
	 * @brief 주어진 table에 field를 추가한다.
	 * @return bool - true;정상수행, falsle:실패
	 */
	bool AddField(void);

	/**
	 * @brief 주어진 table에 field의 Caption을 변경 한다.
	 * @param[in] strColumnName - 변경 할 column 이름
	 * @return bool - true;정상수행, falsle:실패
	 */
	bool RenameField(LPCTSTR strColumnName);

	/**
	 * @brief 주어진 table에 field 정보를 변경 한다.
	 * @param[in] strColumnName - 변경 할 column 이름
	 * @return bool - true;정상수행, falsle:실패
	 */
	bool EditField(LPCTSTR strColumnName);

	/**
	* @brief 파일로 부터 주어진 table로 data를 읽어오다.
	* @param[in] pTable - table 정보
	* @return bool - true;정상수행, falsle:실패
	*/
	bool ImportTable(void);

	/**
	* @brief 주어진 table의 data를 파일로 저장한다.
	* @param[in] pTable - table 정보
	* @return bool - true;정상수행, falsle:실패
	*/
	bool ExportTable(CString a_strFilter=_T(""));

	/**
	 * @brief Table에서 column을 삭제한다.
	 * @param[in] strColumnName - 삭제 할 column 이름
	 * @return bool - true;정상수행, falsle:실패
	 */
	bool DeleteColumn(LPCTSTR strColumnName);

	/**
	 * @brief 주어진 row의 정보를 이용하여 Networkview의 해당 객체로 영역을 확대/축소한다
	 * @param[in] pRow - Networkview의 객체 정보
	 */
	void ZoomTo(const KIORow* pRow);
	void ZoomNode(Integer a_nxNodeID);
	
	/**
	 * @brief 주어진 시/종점 정보를 이용하여 해당영역을 지도상에 표출한다.
	 */
	void ExtentToNodes(std::vector<Integer> a_vecNodes);

	/**
	* @brief 주어진 row의 정보를 이용하여 Networkview의 해당 객체로 영역을 이동한다.
	* @param[in] pRow - Networkview의 객체 정보
	*/
	void PanTo(const KIORow* pRow);

    void SelectHighwayShortestPathStartNode(const KIORow* pRow);
    void SelectHighwayShortestPathEndNode(const KIORow* pRow);
	/**
	* @brief View가 종료 될때 호출되며 control의 종료를 처리한다.
	*/
	void ViewClosed(void);

	void ViewCalculatePathLength(void);

	void ViewCalculateTimeCost(void);
	
	void ViewCalculateZoneDistance(void);

	void ShowProperties(const KIORow* pRow);

	void ShowAccessLineStationModify(Integer &a_nxZoneID, Integer &a_nxLineID);
	void DeleteAccessLineStation(Integer &a_nxZoneID, Integer &a_nxLineID);

	void ShowTransitChart(Integer a_xTransitID);
    void TransitLinkViaTransit();
    void TransitNodeViaTransit();
    void TransitLinkStyle();

	/**
	 * @brief Table에서 해당 column Type이 User Define 인지 확인 한다.
	 * @param[in] strColumnName - 확인 할 column 이름
	 * @return bool - true;User_Define_Column, false: 그외 타입_Column
	 */
	bool IsUserDefineColumn(LPCTSTR strColumnName);
public:
    void TableResetData();
private:
    static unsigned __stdcall TableResetDataProcessThreadCaller(void* p);
    void TableResetDataProcess();
protected :
	KIOTable*     m_pIOTable;	        
	KIOTableView* m_pIOTableView;	
	KTarget*      m_pTarget;	    
	CString       m_strTableName;	
	KPropertiesDlgPtr             m_spPropertiesDlg;
    KTransitPathViewDlgPtr        m_spTransitPathViewDlg;
	KEditAccessLineStationDlgPtr  m_spEditAccessLineStationDlg;
	KPrepareChartTransitDlgPtr    m_spChartTransitDlg;
	KIntersectionEditMainDlgPtr   m_spIntersectionEditMainDlg;
    std::shared_ptr<KDlgTransitStyle> m_spTransitStyleDlg;

public:
	void ActivatedFrameWndNotifyProcess();
	void DeActivatedFrameWndNotifyProcess();

public:
	void ShowTransitLineInfo(Integer a_nxTransitID);
	void ShowIntersectionModify(TIntersectionBase &a_oTSelectIntersection, std::map<Integer, TBoundInfo> &a_mapBoundInfo);
};

typedef std::shared_ptr<KIOTableController> KIOTableControllerPtr;