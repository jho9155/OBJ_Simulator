#include <stdio.h>
#include <Windows.h>

#ifdef __cplusplus 
extern "C" { 
#endif 

#include "CimSeqnc.h"
#include "kutlStr.h"

#ifdef __cplusplus 
}
#endif 

#include "KSeqUtil.h"
#include "Parameter.h"

#include "KIO.h"
#include "KIO_Function.h"

#include "IPS_SCH_EVENT.h"


IPS_SCH_EVENT::IPS_SCH_EVENT(void)
{
}


IPS_SCH_EVENT::~IPS_SCH_EVENT(void)
{
}


void IPS_SCH_EVENT::Send_PM_Disable_Event( char cPM )
{
	char ch = toupper( cPM );
	int	 ino = ch - 'A' + 1;

	Send_Event_PM_Disable( ino );		Sleep( 500 );
	Send_Event_PM_Disable( ino+1 );
}


void IPS_SCH_EVENT::Send_PM_Enable_Event( char cPM )
{
	char ch = toupper( cPM );
	int	 ino = ch - 'A' + 1;

	Send_Event_PM_Enable( ino );		Sleep( 500 );
	Send_Event_PM_Enable( ino+1 );
}


bool IPS_SCH_EVENT::Send_Run( char* pszName, char* szMsg )
{
	KIO_Function*	pFunc = NULL;
	bool bResult =	false;

	pFunc = new KIO_Function( pszName );
	if ( pFunc->RUN_SET( szMsg ))	bResult = true;

	delete pFunc;
	return bResult;
}


void IPS_SCH_EVENT::Send_Event( char* pszName, char* szMsg )
{
	KIO_Function*	pFunc = NULL;
	pFunc = new KIO_Function( pszName );
	pFunc->WRITE_FUNCTION_EVENT( szMsg );
	delete pFunc;
}


bool IPS_SCH_EVENT::Set_IO( char* pszName, int ioType, char* szData )
{
	KIO*	pIO = NULL;
	bool	bResult = false;
	pIO = new KIO( pszName, ioType );
	bResult = pIO->Write_IO( szData );
	delete pIO;

	return bResult;
}


