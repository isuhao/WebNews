// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <atlstr.h>
#include "WebBrowser.h"
#include "whwindow.h"
#include <tchar.h>


class CMainDlg : public CDuiHostWnd
    , public CWHRoundRectFrameHelper<CMainDlg>	//需要圆角窗口时启用
{
public:
    CMainDlg();
    virtual ~CMainDlg();
    
    virtual HWND OnRealWndCreate( CDuiRealWnd *pRealWnd )
    {
        HWND hWndContainer = m_hWnd;
        CRect rcWindow;
        UINT uCmdID = pRealWnd->GetCmdID();
        pRealWnd->GetRect( &rcWindow );
        
        const CDuiRealWndParam & paramRealWnd = pRealWnd->GetRealWndParam();
        if ( _tcscmp( paramRealWnd.m_strClassName, _T( "IECTRL" ) ) == 0 )
        {
            CAsyncWebBrowser *pView = new CAsyncWebBrowser;
            pView->Create( hWndContainer, rcWindow );
            pRealWnd->SetData( pView );
            //            const TCHAR* sUrl = _T( "http://www.baidu.com" );
            const TCHAR* sUrl = _T( "http://minisite2012.qq.com/mini_site_index.htm?geo_location=%E5%8C%97%E4%BA%AC%E5%B8%82&time=true" );
            pView->Navigate( sUrl );
            return pView->m_hWnd;
        }
        return __super::OnRealWndCreate( pRealWnd );
    }
    
    virtual void OnRealWndDestroy( CDuiRealWnd *pRealWnd )
    {
        const CDuiRealWndParam & paramRealWnd = pRealWnd->GetRealWndParam();
        if ( _tcscmp( paramRealWnd.m_strClassName, _T( "IECTRL" ) ) == 0 )
        {
        
            CAsyncWebBrowser *pWebBrowser = ( CAsyncWebBrowser* ) pRealWnd->GetData();
            
            if ( pWebBrowser )
            {
                pWebBrowser->Destroy();
                delete pWebBrowser;
                pRealWnd->SetData( 0 );
            }
            if ( ::IsWindow( pRealWnd->GetRealHwnd( FALSE ) ) )
            {
                ::DestroyWindow( pRealWnd->GetRealHwnd( FALSE ) );
            }
        }
        else
            __super::OnRealWndDestroy( pRealWnd );
    }
    
    void OnClose()
    {
        AnimateHostWindow( 200, AW_CENTER | AW_HIDE );
        EndDialog( IDCANCEL );
    }
    void OnMaximize()
    {
        SendMessage( WM_SYSCOMMAND, SC_MAXIMIZE );
    }
    void OnRestore()
    {
        SendMessage( WM_SYSCOMMAND, SC_RESTORE );
    }
    void OnMinimize()
    {
        SendMessage( WM_SYSCOMMAND, SC_MINIMIZE );
    }
    
    void OnSize( UINT nType, CSize size )
    {
        SetMsgHandled( FALSE );
        if ( !m_bLayoutInited ) return;
        if ( nType == SIZE_MAXIMIZED )
        {
            CDuiWindow* pBtnRestore = FindChildByCmdID( 3 );
            CDuiWindow* pBtnMax = FindChildByCmdID( 2 );
            if ( pBtnRestore )
            {
                pBtnRestore->SetVisible( TRUE );
            }
            if ( pBtnMax )
            {
                pBtnMax->SetVisible( FALSE );
            }
        }
        else if ( nType == SIZE_RESTORED )
        {
            CDuiWindow* pBtnRestore = FindChildByCmdID( 3 );
            CDuiWindow* pBtnMax = FindChildByCmdID( 2 );
            if ( pBtnRestore )
            {
                pBtnRestore->SetVisible( FALSE );
            }
            if ( pBtnMax )
            {
                pBtnMax->SetVisible( TRUE );
            }
        }
    }
    
    BOOL OnInitDialog( HWND hWnd, LPARAM lp )
    {
        m_bLayoutInited = TRUE;
        SetMsgHandled( FALSE );
        
        return FALSE;
    }
    
    int OnCreate( LPCREATESTRUCT lpCreateStruct );
    void OnShowWindow( BOOL bShow, UINT nStatus );
    
protected:

    DUI_NOTIFY_MAP( IDC_RICHVIEW_WIN )
    DUI_NOTIFY_ID_COMMAND( 1, OnClose )
    DUI_NOTIFY_ID_COMMAND( 2, OnMaximize )
    DUI_NOTIFY_ID_COMMAND( 3, OnRestore )
    DUI_NOTIFY_ID_COMMAND( 5, OnMinimize )
    DUI_NOTIFY_MAP_END()
    
    BEGIN_MSG_MAP_EX( CMainDlg )
    CHAIN_MSG_MAP( CWHRoundRectFrameHelper<CMainDlg> )
    MSG_WM_CREATE( OnCreate )
    MSG_WM_CLOSE( OnClose )
    MSG_WM_SIZE( OnSize )
    MSG_WM_INITDIALOG( OnInitDialog )
    MSG_WM_SHOWWINDOW( OnShowWindow )
    MSG_DUI_NOTIFY( IDC_RICHVIEW_WIN )
    CHAIN_MSG_MAP( CDuiHostWnd )
    REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()
    
private:
    BOOL			m_bLayoutInited;
    int				m_iStep;
    //	CUIHander *    m_pUiHandler;
};
