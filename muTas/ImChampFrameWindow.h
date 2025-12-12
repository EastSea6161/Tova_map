#pragma once
#include "KmzApp.h"
#include "Project.h"
#include "MainFrameWnd.h"
#include "ChildFrm.h"
#include "MapFrameWnd.h"
#include "MapView.h"
#include "TocFormView.h"
#include "Target.h"

class ImChampFrameWindow
{
public:
    ImChampFrameWindow(void);
    ~ImChampFrameWindow(void);

public:
    static KProject* GetProject()
    {
        KProject* pProject(nullptr);

        try
        {
            CKmzApp* pApp = (CKmzApp*)(AfxGetApp());
            pProject = pApp->GetProject();
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            TxLogDebugException();
        }
        
        return pProject;
    }

    static CMainFrameWnd* GetMainFrameWnd()
    {
        CMainFrameWnd* pFrame = nullptr;

        try
        {
        	pFrame = (CMainFrameWnd*)AfxGetApp()->GetMainWnd();	
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
        	TxLogDebugException();
        }

        return pFrame;
    }

    static CChildFrame* GetActiveFrame()
    {
        try
        {
            CXTPMDIFrameWnd* pMainFrame  = (CXTPMDIFrameWnd*)AfxGetApp()->GetMainWnd();
            CChildFrame*     pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());
            //★ 아래 함수의 경우 null을 return하지 않는다. 내부적으로 MDIGetActive 함수를 쓰고
            //   null인경우 this(MainFrameWnd)를 리턴
            //★ CChildFrame*     pChildFrame = (CChildFrame*)pFrame->GetActiveFrame();

            return pChildFrame;
        }
        catch (...)
        {
            return nullptr;
        }        
    }

	static KTarget* GetActiveFrameTarget()
	{
		try
		{
			CXTPMDIFrameWnd* pMainFrame  = (CXTPMDIFrameWnd*)AfxGetApp()->GetMainWnd();
			CChildFrame*     pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());
            //★ 아래 함수의 경우 null을 return하지 않는다. 내부적으로 MDIGetActive 함수를 쓰고
            //   null인경우 this(MainFrameWnd)를 리턴
            //★ CChildFrame*     pChildFrame = (CChildFrame*)pFrame->GetActiveFrame();

            if (pChildFrame == nullptr)
                return nullptr;

			return pChildFrame->GetTarget();
		}
		catch (...)
		{
			return nullptr;
		}        
	}

    static KMapView* GetActiveMapView()
    {
        try
        {            
            CChildFrame* pChildFrame = GetActiveFrame();
            if (pChildFrame == nullptr)
                return nullptr;

            KMapView* pMapView = pChildFrame->GetMapView();

            return pMapView;
        }
        catch (...)
        {
            return nullptr;
        }        
    }

	static KMapView* GetMapView(KTarget* a_pTarget) // ChildFrame이 없다면 생성하여, MapView객체가 생성된다.
	{
		try
		{
            if (a_pTarget == nullptr)
                return nullptr;

			CMainFrameWnd* pFrame      = (CMainFrameWnd*)AfxGetApp()->GetMainWnd();		
			CChildFrame*   pChildFrame = pFrame->GetCreateTargetChildFrame(a_pTarget);
			KMapView*      pMapView    = pChildFrame->GetMapView();

			return pMapView;
		}
		catch (...)
		{
			return nullptr;
		}   
	}

	static KMapView* GetCreatedMapView(KTarget* a_pTarget) // ChildFrame이 없다면, 
	{
		try
		{
			KMapView*      pMapView = nullptr;

			CMainFrameWnd* pFrame       = (CMainFrameWnd*)AfxGetApp()->GetMainWnd();		
			CChildFrame*   pChildFrame  = pFrame->GetCreatedTargetChildFrame(a_pTarget);
			if (nullptr != pChildFrame) {
				KMapView*  pMapView     = pChildFrame->GetMapView();
				return pMapView;
			} // 생성된 Frame이 없으면 null 
		}
		catch (...)
		{
			return nullptr;
		}  

		return nullptr;
	}

	static KMapView* GetShowMapView(KTarget* a_pTarget)
	{
		try
		{
			CMainFrameWnd* pFrame      = (CMainFrameWnd*)AfxGetApp()->GetMainWnd();		
			CChildFrame*   pChildFrame = pFrame->GetCreateTargetChildFrame(a_pTarget);
			pChildFrame->ShowMapView(true);

			KMapView*      pMapView    = pChildFrame->GetMapView();
			return pMapView;
		}
		catch (...)
		{
			return nullptr;
		}   
	}

    static KIOTableView* ShowTableView(KTarget* a_pTarget, CString a_strTableName)
    {
        try
        {
            CMainFrameWnd* pFrame      = (CMainFrameWnd*)AfxGetApp()->GetMainWnd();
            KIOTableView* pIoTableView = pFrame->ShowTableView(a_pTarget, a_strTableName);	
            return pIoTableView;
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
        	TxLogDebugException();
        }

        return nullptr;
    }

    static KIOView* GetTableView(KMapView* a_pMapView)
    {
        try
        {
            CChildFrame*  pChildFrame  = GetChildFrame(a_pMapView);
            if (pChildFrame != nullptr)
                return pChildFrame->GetIOView();
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            TxLogDebugException();
        }

        return nullptr;
    }

    static KTocFormView* GetTocFormView(KMapView* a_pMapView)
    {
        try
        {
            CChildFrame*  pChildFrame  = GetChildFrame(a_pMapView);
            KTocFormView* pTocFormView = pChildFrame->GetTocFormView();

            return pTocFormView;
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
        	TxLogDebugException();
        }
        return nullptr;
    }

    static CChildFrame* GetChildFrame(KMapView* a_pMapView)
    {
        try
        {            
            KMapFrameWnd* pFrameWnd   = (KMapFrameWnd*)(a_pMapView->GetParentFrame());
            CChildFrame*  pChildFrame = (CChildFrame*) pFrameWnd->GetParentFrame();

            return pChildFrame;
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            TxLogDebugException();
        }
        return nullptr;
    }
};
