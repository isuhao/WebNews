// WebNews.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "WebNews.h"
#include "MainDlg.h"

CAppModule _Module;
BOOL LoadSkinFromFile( DuiSystem* pDuiSystem )
{
    //     TCHAR szCurrentDir[MAX_PATH];
    //     memset( szCurrentDir, 0, sizeof( szCurrentDir ) );
    //     GetModuleFileName( NULL, szCurrentDir, sizeof( szCurrentDir ) );
    //     LPTSTR lpInsertPos = _tcsrchr( szCurrentDir, _T( '\\' ) );
    //     *lpInsertPos = _T( '\0' );
    //
    //     _tcscat( szCurrentDir, _T( "\\skin" ) );
    DuiResProviderFiles *pResFiles = new DuiResProviderFiles;
    if ( !pResFiles->Init( _T( "skin" ) ) )
    {
        DUIASSERT( 0 );
        return FALSE;
    }
    pDuiSystem->SetResProvider( pResFiles );
    
    return TRUE;
}

int APIENTRY _tWinMain( _In_ HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR    lpCmdLine, int       nCmdShow )
{
    HRESULT hRes = OleInitialize( NULL );
    DUIASSERT( SUCCEEDED( hRes ) );
    
    _Module.Init( NULL, hInstance );
    CMessageLoop theLoop;
    _Module.AddMessageLoop( &theLoop );
    
    DuiSystem duiSystem( hInstance );
#ifdef _DEBUG
    if ( !LoadSkinFromFile( &duiSystem ) )
    {
        return -1;
    }
#else
    duiSystem.SetResProvider( new DuiResProviderPE( hInstance ) );
#endif
    
    BOOL bOK = duiSystem.Init( IDR_DUI_INIT );
    
    CMainDlg dlgMain;
    int nRet = dlgMain.DoModal();
    
    delete duiSystem.GetResProvider();
    
    _Module.RemoveMessageLoop();
    _Module.Term();
    OleUninitialize();
    return nRet;
}

