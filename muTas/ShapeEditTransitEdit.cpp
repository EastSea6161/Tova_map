#include "stdafx.h"
#include "ShapeEditTransitEdit.h"
#include "Target.h"
#include "BulkDBaseTransit.h"
#include "TransitPathDirectionSelect.h"

KShapeEditTransitEdit::KShapeEditTransitEdit( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ ):
IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner), m_nxEditTransitID(0),
    m_emStatus(TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode), m_nCommandType(1), m_nFirstSeq(-1)
{
    KBulkDBaseLink::AllLinkFTNode(m_pIMapView->GetTarget(), m_mapLinkFTNode);
    m_nMouseFlag = 0;
}

KShapeEditTransitEdit::~KShapeEditTransitEdit( void )
{
    StopEditing();
}

void KShapeEditTransitEdit::CommandFeedback( CString a_strResult )
{
}

void KShapeEditTransitEdit::Canceled()
{
    ReStartEditing();
}


void KShapeEditTransitEdit::Draw()
{
    if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLink)
        {
            m_spScreenDrawFlashLink->Draw();
        }
    }
    else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLinkPath)
        {
            m_spScreenDrawFlashLinkPath->Draw();
        }
    }

    m_pIMapView->MapForegroundRefresh();
}

void KShapeEditTransitEdit::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (nFlags & MK_MBUTTON) {
        return;
    }

    if (m_nCommandType < 0)
        return;

    m_lMouseDownX    = X;
    m_lMouseDownY    = Y;
    m_dMouseDownMapX = mapX;
    m_dMouseDownMapY = mapY;

    m_nMouseFlag = nFlags;

    if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLink)
        {
            m_spScreenDrawFlashLink->MouseDown(nFlags, X, Y, mapX, mapY);
        }
    }
    else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLinkPath)
        {
            m_spScreenDrawFlashLinkPath->MouseDown(nFlags, X, Y, mapX, mapY);
        }
    }
}

void KShapeEditTransitEdit::OnMapRButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    m_lMouseDownX    = X;
    m_lMouseDownY    = Y;
    m_dMouseDownMapX = mapX;
    m_dMouseDownMapY = mapY;

    m_nMouseFlag = nFlags;

    if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLink)
        {
            m_spScreenDrawFlashLink->MouseDown(nFlags, X, Y, mapX, mapY);
        }
    }
    else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLinkPath)
        {
            m_spScreenDrawFlashLinkPath->MouseDown(nFlags, X, Y, mapX, mapY);
        }
    }
}


void KShapeEditTransitEdit::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (nFlags & MK_MBUTTON) {
        return;
    }

    if (m_nCommandType < 0)
        return;

    if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLink)
        {
            m_spScreenDrawFlashLink->MouseMove(nFlags, X, Y, mapX, mapY);
        }
    }
    else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLinkPath)
        {
            m_spScreenDrawFlashLinkPath->MouseMove(nFlags, X, Y, mapX, mapY);
        }
    }
    else
    {
        // Nothing
    }
}

void KShapeEditTransitEdit::StartEditing()
{
	try
	{
        m_bEditing  = true;
        m_nFirstSeq = -1;
        m_emStatus  = TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode;
        m_vecNewPath.clear();

		if (m_spTransitPathEditDlg == nullptr)
		{
			CWnd* pWnd = m_pIMapView->GetMapWindow();
			m_spTransitPathEditDlg = KTransitPathEditDlgPtr(new KTransitPathEditDlg(pWnd));
			m_spTransitPathEditDlg->Create(KTransitPathEditDlg::IDD, pWnd);
            m_spTransitPathEditDlg->SetOpener(this);
		}
		
		m_spTransitPathEditDlg->SetEditTransit(m_pIMapView->GetTarget(), m_nxEditTransitID);
		m_spTransitPathEditDlg->ShowWindow(SW_SHOW);

        // Original 노선의 Path Draw
        if (m_spForeDrawTransitPath == nullptr)
            m_spForeDrawTransitPath = KForeDrawSingleTransitPathPtr(new KEditForeDrawTransitPath(m_pIMapView));  
        
        RedrawUserSelectedPath();
        m_pIMapView->SetMapCursor(IDC_EditingEditTool);
        m_nCommandType = m_spTransitPathEditDlg->GetCommand();
        m_spScreenDrawFlashLink = KScreenDrawFlashLinkPtr(new KScreenDrawFlashLink(m_pIMapView, this));

        //std::vector<Integer> vecSelectableLink = FindSelectableLink();
        //m_spScreenDrawFlashLink->SetSelectable(vecSelectableLink, false);
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

void KShapeEditTransitEdit::ReStartEditing()
{
    try
    {
        m_bEditing  = true;
        m_nFirstSeq = -1;
        m_emStatus  = TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode;
        m_vecNewPath.clear();

        if (m_spTransitPathEditDlg == nullptr)
        {
            CWnd* pWnd = m_pIMapView->GetMapWindow();
            m_spTransitPathEditDlg = KTransitPathEditDlgPtr(new KTransitPathEditDlg(pWnd));
            m_spTransitPathEditDlg->Create(KTransitPathEditDlg::IDD, pWnd);
        }

        m_spTransitPathEditDlg->ShowWindow(SW_SHOW);

        // Original 노선의 Path Draw
        if (m_spForeDrawTransitPath == nullptr)
            m_spForeDrawTransitPath = KForeDrawSingleTransitPathPtr(new KEditForeDrawTransitPath(m_pIMapView));  
        
        RedrawUserSelectedPath();
        m_pIMapView->SetMapCursor(IDC_EditingEditTool);
        m_nCommandType = m_spTransitPathEditDlg->GetCommand();
        m_spScreenDrawFlashLink = KScreenDrawFlashLinkPtr(new KScreenDrawFlashLink(m_pIMapView, this));

        //std::vector<Integer> vecSelectableLink = FindSelectableLink();
        //m_spScreenDrawFlashLink->SetSelectable(vecSelectableLink, false);
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

void KShapeEditTransitEdit::SaveEditing()
{
}

void KShapeEditTransitEdit::StopEditing()
{
    m_bEditing  = false;
    m_nFirstSeq = -1;
    try
    {
        // m_spTransitPathEditDlg->부모를 죽일수 있음

        if (m_spScreenDrawFlashLink)
            m_spScreenDrawFlashLink.reset();

        if (m_spScreenDrawFlashLinkPath)
            m_spScreenDrawFlashLinkPath.reset();

        if (m_spForeDrawTransitPath)
            m_spForeDrawTransitPath.reset();       

        m_pIMapView->MapRefresh();
    }
    catch(...)
    {
    }	
}

WORD KShapeEditTransitEdit::GetCursorID()
{
	return IDC_EditingEditTool;
}


void KShapeEditTransitEdit::SetEditID( Integer a_nxID )
{
	m_nxEditTransitID = a_nxID;
}

void KShapeEditTransitEdit::TransitPathPopulate()
{
    try
    {
        if (!m_spTransitPathEditDlg->IsWindowVisible())
            m_spTransitPathEditDlg->ShowWindow(SW_SHOW);

        m_spTransitPathEditDlg->TransitPathPopulate();
        RedrawUserSelectedPath();
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KShapeEditTransitEdit::RedrawUserSelectedPath()
{
    try
    {
        std::vector<TTransitPath>& vecTranistPath = m_spTransitPathEditDlg->GetTransitPath();
        if (m_spForeDrawTransitPath) {
            m_spForeDrawTransitPath->SetTransitPath(false, vecTranistPath);
            m_pIMapView->MapForegroundRefresh();
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KShapeEditTransitEdit::MapSelectionFeedback( Integer a_nxID )
{
    // 출발(시점연결, 종점연결)
    // 구간변경
    try
    {
        std::vector<Integer> vecLinkID;
        vecLinkID.push_back(a_nxID);

        if (m_nCommandType == 0)
        {
            if (IsInTransitLink(a_nxID) >= 0)
            {
                AfxMessageBox(_T("기존 노선 구간 링크에 시점을 지정할 수 없습니다.\n기존 구간을 삭제하면 시점이 자동으로 변경됩니다."));
                ReStartEditing();
                return;
            }

            EditPathAppendDraw(vecLinkID);
            if (m_emStatus == TUserLinkPathSelectMode::KEMUserPathSelectMode)            
                m_spScreenDrawFlashLinkPath = KScreenDrawFlashLinkPathPtr(new KScreenDrawFlashLinkPath(m_pIMapView, this, a_nxID));
            return;
        }

        if (m_nCommandType == 1)
        {
            /*m_nFirstSeq = IsInTransitLink(a_nxID);
            if (m_nFirstSeq > 0)
            {
                AfxMessageBox(_T("노선의 연결성이 상실 됩니다.\n기존 노선 구간과 연결된 링크를 선택해 주세요."));
                //ReStartEditing();
                //return;
            }*/

            m_nFirstSeq = FindTransitPathSeqByFromNode(-1, a_nxID, true, false);
            if (m_nFirstSeq < 0)
            {
                if (m_nFirstSeq != -9)
                {
                    AfxMessageBox(_T("노선의 연결성이 상실 됩니다.\n기존 노선 구간과 연결된 링크를 선택해 주세요."));
                }
                
                ReStartEditing(); 
                return;
            }

            EditPathAppendDraw(vecLinkID);

            if (m_emStatus == TUserLinkPathSelectMode::KEMUserPathSelectMode)  
                m_spScreenDrawFlashLinkPath = KScreenDrawFlashLinkPathPtr(new KScreenDrawFlashLinkPath(m_pIMapView, this, a_nxID));

            return;
        }

        if (m_nCommandType == 2)
        {           
            std::vector<TTransitPath>& vecPreTranistPath = m_spTransitPathEditDlg->GetTransitPath();
            size_t nxPreCount = vecPreTranistPath.size();
            int    nLastSeq   = (int)nxPreCount-1;
            m_nFirstSeq = FindTransitPathSeqByFromNode(nxPreCount-1, a_nxID);

            if (m_nFirstSeq == nLastSeq)
            {
                EditPathAppendDraw(vecLinkID);
            }
            else
            {
                AfxMessageBox(_T("노선의 연결성이 상실 됩니다.\n기존 노선 종점과 연결된 링크를 선택해 주세요."));
                ReStartEditing();
            }

            return;
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

void KShapeEditTransitEdit::MapSelectionFeedback(std::vector<Integer> a_vecSelection)
{
    try
    {
        size_t nxCount = a_vecSelection.size();
        if (nxCount == 0)
            return;

        EditPathAppendDraw(a_vecSelection);

        if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode != m_emStatus)
        {
            Integer nxLinkID = a_vecSelection[nxCount-1];
            m_spScreenDrawFlashLinkPath = KScreenDrawFlashLinkPathPtr(new KScreenDrawFlashLinkPath(m_pIMapView, this, nxLinkID));
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

void KShapeEditTransitEdit::MapSelectionNoneFeedback()
{
    //AfxMessageBox(_T("MapSelectionNoneFeedback"));
}

void KShapeEditTransitEdit::TransitPathEditFeedback()
{
    try
    {
        ReStartEditing();
    }
    catch (...)
    {
        TxLogDebugException();
    }    
}

void KShapeEditTransitEdit::TransitPathStationEditFeedback()
{
    try
    {
        RedrawUserSelectedPath();
    }
    catch(...)
    {
        TxLogDebugException();
    }
}


void KShapeEditTransitEdit::TransitSaveFeedback(__int64 nxID)
{
    try
    {
        ReStartEditing();
    }
    catch (...)
    {
        TxLogDebugException();
    }    
}

void KShapeEditTransitEdit::TransitPathCommand( int nCommand )
{
    try
    {   
        m_nCommandType = nCommand;

        if (nCommand == -1)
            StopEditing();
        else
            ReStartEditing();
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


int KShapeEditTransitEdit::FindTransitPathSeqByFromNode( int a_nFirstSeq, Integer a_nxID, bool a_bUserQuestion, bool a_bEnablePass )
{
    int nSeq(-1);

    try
    {
        std::map<Integer, TLinkFTNodeID>::iterator iter = m_mapLinkFTNode.begin();
        std::map<Integer, TLinkFTNodeID>::iterator end  = m_mapLinkFTNode.end();

        iter = m_mapLinkFTNode.find(a_nxID);
        if (iter == end)
        {
            return nSeq;
        }

        Integer nxUserFromNode = iter->second.TFromNodeID;
        Integer nxTransitToNode(0);

        Integer nxMinGap(999999), nxTemp(0);

        std::vector<Integer> vecSelectSeq;   
        std::vector<TTransitPath>& vecTranistPath = m_spTransitPathEditDlg->GetTransitPath();
        size_t nxCount = vecTranistPath.size();
        for (size_t i=0; i<nxCount; i++)
        {
            Integer nxLinkID = vecTranistPath[i].TLinkID;
            iter = m_mapLinkFTNode.find(nxLinkID);

            if (iter != end)
            {
                nxTransitToNode = iter->second.TToNodeID;
                if (nxTransitToNode == nxUserFromNode)
                {
                    // cjy627 : x64 의 경우, 타입에 따른 컴파일 오류 발생
                    nxTemp = abs((long long)(i - (Integer)a_nFirstSeq));

                    if (nxMinGap >= nxTemp)
                    {
                        nSeq = (int)i; 
                        nxMinGap = nxTemp;
                    }             

                    vecSelectSeq.push_back(i);
                }
            }
        }   

        nxCount = vecSelectSeq.size();
        if (nxCount > 0 && a_bUserQuestion)
        {            
            CPoint ptMouse(m_lMouseDownX, m_lMouseDownY);
            CWnd* pMapWindow = m_pIMapView->GetMapWindow();
            pMapWindow->ClientToScreen(&ptMouse);

            std::vector<CString> vecSelect; 
            
            int nIndex(0);

            for (size_t i=0; i<vecSelectSeq.size(); i++)
            {
                CString strTemp;
                strTemp.Format(_T("%I64d"), vecSelectSeq[i]+1);
                vecSelect.push_back(strTemp);

                if (vecSelectSeq[i] == (Integer)nSeq)
                {
                    nIndex = (int)i;
                }
            }

            KTransitPathDirectionSelect dlg;
            dlg.SetSelect(_T("Pre Seq ?"), vecSelect, nIndex);
            dlg.SetEnablePass(a_bEnablePass);
            dlg.SetWindowPosition(ptMouse);

            if (dlg.DoModal() != IDOK)
            {
                nSeq = -9;
            }
            else
            {
                int nIndex = dlg.GetSelectedIndex();
                if (nIndex < 0)
                {
                    nSeq = -9; /*Pass*/
                }
                else
                {
                    nSeq = (int)vecSelectSeq[nIndex];
                }                
            }
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

    return nSeq;
}

int KShapeEditTransitEdit::FindTransitPathSeqByToNode( int a_nFirstSeq, Integer a_nxID, bool a_bUserQuestion, bool a_bEnablePass )
{
    int nSeq(-1);

    try
    {
        std::map<Integer, TLinkFTNodeID>::iterator iter = m_mapLinkFTNode.begin();
        std::map<Integer, TLinkFTNodeID>::iterator end  = m_mapLinkFTNode.end();

        iter = m_mapLinkFTNode.find(a_nxID);
        if (iter == end)
        {
            return nSeq;
        }

        Integer nxUserToNode = iter->second.TToNodeID;
        Integer nxTransitFromNode(0);
        Integer nxMinGap(999999), nxTemp(0);

        std::vector<Integer> vecSelectSeq;                       
        std::vector<TTransitPath>& vecTranistPath = m_spTransitPathEditDlg->GetTransitPath();
        size_t nxCount = vecTranistPath.size();

        for (size_t i=0; i<nxCount; i++)
        {
            Integer nxLinkID = vecTranistPath[i].TLinkID;
            iter = m_mapLinkFTNode.find(nxLinkID);

            if (iter != end)
            {
                nxTransitFromNode = iter->second.TFromNodeID;
                if (nxTransitFromNode == nxUserToNode)
                {
					// cjy627 : x64 의 경우, 타입에 따른 컴파일 오류 발생                    
                    nxTemp = abs((long long)(i - (Integer)a_nFirstSeq));

                    if (nxMinGap >= nxTemp)
                    {
                        nSeq = (int)i; 
                        nxMinGap = nxTemp;
                    }       

                    vecSelectSeq.push_back(i);                    
                }
            }
        }       

        nxCount = vecSelectSeq.size();
        if (nxCount > 0 && a_bUserQuestion)
        {            
            CPoint ptMouse(m_lMouseDownX, m_lMouseDownY);
            CWnd* pMapWindow = m_pIMapView->GetMapWindow();
            pMapWindow->ClientToScreen(&ptMouse);

            std::vector<CString> vecSelect; 
            int nIndex(0);

            for (size_t i=0; i<vecSelectSeq.size(); i++)
            {
                CString strTemp;
                strTemp.Format(_T("%I64d"), vecSelectSeq[i]+1);
                vecSelect.push_back(strTemp);

                if (vecSelectSeq[i] == (Integer)nSeq)
                {
                    nIndex = (int)i;
                }
            }

            KTransitPathDirectionSelect dlg;
            dlg.SetSelect(_T("End Seq ?"), vecSelect, nIndex);
            dlg.SetEnablePass(a_bEnablePass);
            dlg.SetWindowPosition(ptMouse);

            if (dlg.DoModal() != IDOK)
            {
                nSeq = -9;
            }
            else
            {
                int nIndex = dlg.GetSelectedIndex();
                if (nIndex < 0)
                {
                    nSeq = -9; /*Pass*/
                }
                else
                {
                    nSeq = (int)vecSelectSeq[nIndex];
                }                
            }
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

    return nSeq;
}

void KShapeEditTransitEdit::EditPathAppendDraw(std::vector<Integer> vecLinkID, bool a_bStation)
{
    try
    {
        std::vector<TTransitPath>& vecPreTranistPath = m_spTransitPathEditDlg->GetTransitPath();
        size_t nxPreCount = vecPreTranistPath.size();

        size_t nxCount    = vecLinkID.size();
        size_t nxNewPathCount = m_vecNewPath.size();

        for (size_t i=0; i<nxCount; i++)
        {
            TTransitPath oTransitPath;
            oTransitPath.TTransitID = m_nxEditTransitID;
            oTransitPath.TLinkID    = vecLinkID[i];
            oTransitPath.TSeq       = 0/*nxNewPathCount + i + 1*/;

            if (i == nxCount-1 && m_nMouseFlag & MK_LBUTTON) {
                oTransitPath.TStationYN = 1;
            }
            else {
                oTransitPath.TStationYN = 0;
            }
            m_vecNewPath.push_back(oTransitPath);
        }

    	if (m_spForeDrawTransitPath == nullptr)
            return;
        
        m_spForeDrawTransitPath->SetTransitPath(true, m_vecNewPath, false, RGB(0, 0, 255));
        
        // 모든 비지니스는 여기서 판단하자.
        nxNewPathCount = m_vecNewPath.size();
        if (nxNewPathCount == 0)
            return;

        //TTransitPath oFirst = m_vecNewPath[0];
        TTransitPath oLast  = m_vecNewPath[nxNewPathCount-1];
        
        int nFirstSeq = m_nFirstSeq;
        int nLastSeq(-1);

        if (m_nCommandType == 1) {
            nLastSeq  = FindTransitPathSeqByToNode(nFirstSeq, oLast.TLinkID, true);
        }
        else {
            nLastSeq  = FindTransitPathSeqByToNode(nFirstSeq, oLast.TLinkID, false);
        }
        
        if(nFirstSeq == nxPreCount-1)
        {
            AppendTransitLast();
            ReStartEditing();
            return;
        }
        else if (nLastSeq >= 0)
        {
            if (nFirstSeq < 0)
            {
                if (nLastSeq == 0)
                {
                    AppendTransitFirst();
                    ReStartEditing();
                    return;
                }                
            }
            else
            {
                InsertTransit(nFirstSeq, nLastSeq);   
                ReStartEditing();
                return;
            }
        }

        m_pIMapView->MapForegroundRefresh();
        m_emStatus = TUserLinkPathSelectMode::KEMUserPathSelectMode;
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

void KShapeEditTransitEdit::AppendTransitFirst()
{
    try
    {    	
        std::vector<TTransitPath> vecNewTranistPath;

        size_t nxNewPathCount = m_vecNewPath.size();
        for (size_t i=0; i<nxNewPathCount; i++)
        {
            TTransitPath& oTransitPath = m_vecNewPath[i];                          
                          oTransitPath.TSeq = i + 1;
            vecNewTranistPath.push_back(oTransitPath);
        }

        std::vector<TTransitPath>& vecPreTranistPath = m_spTransitPathEditDlg->GetTransitPath();
        size_t nxCount = vecPreTranistPath.size();
        for (size_t i=0; i<nxCount; i++)
        {
            TTransitPath& oTransitPath = vecPreTranistPath[i];
                          oTransitPath.TSeq = nxNewPathCount + i + 1;

            if (i == nxCount -1)
            {
                oTransitPath.TStationYN = 1;
            }

            vecNewTranistPath.push_back(oTransitPath);
        }

        m_spTransitPathEditDlg->SetTransitPath(vecNewTranistPath);
        m_spTransitPathEditDlg->TransitPathPopulate();
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

void KShapeEditTransitEdit::AppendTransitLast()
{
    try
    {    	
        std::vector<TTransitPath> vecNewTranistPath;

        std::vector<TTransitPath>& vecPreTranistPath = m_spTransitPathEditDlg->GetTransitPath();
        size_t nxCount = vecPreTranistPath.size();
        for (size_t i=0; i<nxCount; i++)
        {
            TTransitPath& oTransitPath = vecPreTranistPath[i];
            oTransitPath.TSeq = i + 1;

            vecNewTranistPath.push_back(oTransitPath);
        }

        size_t nxNewPathCount = m_vecNewPath.size();
        for (size_t i=0; i<nxNewPathCount; i++)
        {
            TTransitPath& oTransitPath = m_vecNewPath[i];                          
            oTransitPath.TSeq = nxCount + i + 1;
            if (i == nxCount -1)
            {
                oTransitPath.TStationYN = 1;
            }
            vecNewTranistPath.push_back(oTransitPath);
        }

        m_spTransitPathEditDlg->SetTransitPath(vecNewTranistPath);
        m_spTransitPathEditDlg->TransitPathPopulate();
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

void KShapeEditTransitEdit::InsertTransit( int a_nFirst, int a_nLast )
{
    try
    {
        std::vector<TTransitPath> vecNewTranistPath;

        std::vector<TTransitPath>& vecPreTranistPath = m_spTransitPathEditDlg->GetTransitPath();
        size_t nxCount = vecPreTranistPath.size();

        int  nSeq(1);
        bool bInsert(false);

        for (size_t i=0; i<nxCount; i++)
        {
            TTransitPath& oTransitPath = vecPreTranistPath[i];
            oTransitPath.TSeq = nSeq++;

            vecNewTranistPath.push_back(oTransitPath);

            if ((int)i >= a_nFirst && bInsert == false)
            {
                size_t nxNewPathCount = m_vecNewPath.size();
                for (size_t m=0; m<nxNewPathCount; m++)
                {
                    TTransitPath& oTransitPath = m_vecNewPath[m];                          
                    oTransitPath.TSeq = nSeq++;
                    vecNewTranistPath.push_back(oTransitPath);
                }

                bInsert = true;
                i = a_nLast-1 /*For Loop i++*/;
            }            
        }        

        m_spTransitPathEditDlg->SetTransitPath(vecNewTranistPath);
        m_spTransitPathEditDlg->TransitPathPopulate();    	
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

int KShapeEditTransitEdit::IsInTransitLink( Integer a_nxLinkID )
{
    int nSeq(-1);
    try
    {
        std::vector<TTransitPath>& vecTranistPath = m_spTransitPathEditDlg->GetTransitPath();
        size_t nxCount = vecTranistPath.size();
        for (size_t i=0; i<nxCount; i++)
        {
            Integer nxLinkID = vecTranistPath[i].TLinkID;
            if (nxLinkID == a_nxLinkID)
            {
                nSeq = (int)i;
                break;
            }
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
    
    return nSeq;
}

std::vector<Integer> KShapeEditTransitEdit::FindSelectableLink()
{
    std::vector<Integer> vecSelectableLink;

    try
    {
    	std::vector<TTransitPath>& vecTranistPath = m_spTransitPathEditDlg->GetTransitPath();
        std::set<Integer> setTransitLink;
        size_t nxCount = vecTranistPath.size();

        for (size_t i=0; i<nxCount; i++)
        {
            TTransitPath& oPath = vecTranistPath[i];
            setTransitLink.insert(oPath.TLinkID);
        }

        std::set<Integer>::iterator iterTransitLink = setTransitLink.begin();
        std::set<Integer>::iterator endTransitLink  = setTransitLink.end();

        std::map<Integer, TLinkFTNodeID>::iterator iter = m_mapLinkFTNode.begin();
        std::map<Integer, TLinkFTNodeID>::iterator end  = m_mapLinkFTNode.end();

        while(iter != end)
        {
            Integer nxLinkID = iter->first;

            iterTransitLink  = setTransitLink.find(nxLinkID);
            if (iterTransitLink == endTransitLink)
            {
                vecSelectableLink.push_back(nxLinkID);
            }

            ++iter;
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

    return vecSelectableLink;
}






