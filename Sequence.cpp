#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include <process.h>


#ifdef __cplusplus 
extern "C" { 
#endif 

#include "CimSeqnc.h"
#include "kutlStr.h"

#ifdef __cplusplus 
}
#endif 

#include "KSemi_JobData.h"

#include "Parameter.h"

#include "KSeqUtil.h"
#include "APP_SIM_Port.h"
#include "IPS_SIM_File.h"
#include "IPS_SIM_DATA_Controller.h"
#include "IPS_PM_Monitor.h"


#include "Sequence.h"

KStr*						gpStr = NULL;
IPS_SIM_PORT*				gpPort[MAX_PORT];
IPS_SIM_File*				gpSimFile = NULL;
IPS_SIM_DATA_Controller*	gpDataController = NULL;
IPS_PM_Monitor*				gpModMonitor = NULL;

KIO_Digital*				gpSeqRun_Mode = NULL;

KSemi_JobData*				gpJobData = NULL;

BOOL						gbDebug = FALSE;
int							LPSeq[3] = { 0, };
int							PORTCnt=-1;
IO_Name_String_Map IO_STR_Map_Table[] = {
	""
};

IPS_SIM_PORT::IPS_SIM_PORT( int index, char* pszIO_Foup1, char* pszIO_Foup2, char* pszIO_LoadSignal, char* pszIO_UnloadSignal, char* pszIO_LdButton, char* pszIO_UldButton, char* pszGUIFunc )
	: APP_SIM_PORT( pszIO_Foup1, pszIO_Foup2, pszIO_LoadSignal, pszIO_UnloadSignal, pszIO_LdButton, pszIO_UldButton )
{
	int		i,ics;
	char	szMdlStr[64] = { 0, };
	char	szIOStr[32] = { 0 , };

	m_iRegNo = -1;
	m_iPortIdx = index;
	m_pGUIStart_Func = new KIO_Function( pszGUIFunc );

	for ( i = 1 ; i <= MAX_PM_MDL_NO ; i++ ) {
		sprintf( szMdlStr, "SCHEDULER_PROCESS_PM%d", i );
		m_pPMFunc[i-1] = new KIO_Function( szMdlStr );

		sprintf( szMdlStr, "SCHEDULER_INTERFACE_PM%d", i );
		m_pPMIntf[i-1] = new KIO_Function( szMdlStr );

		sprintf( szMdlStr, "SCHEDULER_MAINT_PM%d", i );
		m_pPM_MAINT[i-1] = new KIO_Function( szMdlStr );
	}

	for ( i = 1 ; i <= MAX_TM_MDL_NO ; i++ ) {
		sprintf( szMdlStr, "SCHEDULER_INTERFACE_TM%d", i );
		m_pTMIntf[i-1] = new KIO_Function( szMdlStr );
	}

	for ( i = 1 ; i <= MAX_FM_MDL_NO ; i++ ) {
		sprintf( szMdlStr, "SCHEDULER_INTERFACE_FM%d", i );
		m_pFMIntf[i-1] = new KIO_Function( szMdlStr );
	}

	for ( i = 1 ; i <= MAX_BM_MDL_NO ; i++ ) {
		sprintf( szMdlStr, "SCHEDULER_INTERFACE_BM%d", i );
		m_pBMIntf[i-1] = new KIO_Function( szMdlStr );

		sprintf( szMdlStr, "SCHEDULER_MAINT_BM%d", i );
		m_pBM_MAINT[i-1] = new KIO_Function( szMdlStr );
	}

	m_pSCH = new KIO_Function( "SCHEDULER" );
	m_peSch = new KIO_String( "e:SCHEDULER" );
	m_bPortRun = false;
	m_bSchFail = false;

	// 2019.05.27 Append Start ----------------------------------------------------------------------------------
	if ( index == 0 )		strcpy( szIOStr, "CTC.MID_NAME" );
	else					sprintf( szIOStr, "CTC.MID_NAME%d", index+1 );
	m_pPortMID = new KIO_String( szIOStr );												// Get MID
	
	
		
	// 2019.05.27 Append End   ----------------------------------------------------------------------------------

	// 2019.06.04 Append Start ----------------------------------------------------------------------------------
	switch ( index ) {
		case 0:
			sprintf( szIOStr, "LPMA.Switch2LoadDI" );						// Access Button
			m_pAccessBtn = new KIO_Digital( szIOStr );
			sprintf( szIOStr, "LPMA.ClampUnclampXI" );
			m_pClampSts = new KIO_Digital( szIOStr );
			break;
		case 1:
			sprintf( szIOStr, "LPMB.Switch2LoadDI" );						// Access Button
			m_pAccessBtn = new KIO_Digital( szIOStr );
			sprintf( szIOStr, "LPMB.ClampUnclampXI" );
			m_pClampSts = new KIO_Digital( szIOStr );
			break;
		case 2:
			sprintf( szIOStr, "LPMC.Switch2LoadDI" );						// Access Button
			m_pAccessBtn = new KIO_Digital( szIOStr );
			sprintf( szIOStr, "LPMC.ClampUnclampXI" );
			m_pClampSts = new KIO_Digital( szIOStr );
			break;
	}


	

	//sprintf( szIOStr, "LP%d.CLAMP_STATUS", index + 1 );									// Clamp Status
	
	// 2019.06.04 Append End   ----------------------------------------------------------------------------------

}


IPS_SIM_PORT::~IPS_SIM_PORT()
{
	int		i;

	delete m_pGUIStart_Func;	m_pGUIStart_Func = NULL;
	delete gpJobData;			gpJobData =NULL;
	delete m_pPortMID;			m_pPortMID = NULL;
	delete m_pAccessBtn;		m_pAccessBtn = NULL;
	delete m_pClampSts;			m_pClampSts = NULL;
	delete m_pSCH;				m_pSCH = NULL;
	delete m_peSch;				m_peSch = NULL;
	for ( i = 1 ; i < MAX_PM_MDL_NO ; i++ ) {	delete m_pPMFunc[i];		m_pPMFunc[i] = NULL; }
	for ( i = 1 ; i < MAX_TM_MDL_NO ; i++ ) {  delete m_pTMIntf[i];		m_pTMIntf[i] = NULL; }
	for ( i = 1 ; i < MAX_FM_MDL_NO ; i++ ) {	delete m_pFMIntf[i];		m_pFMIntf[i] = NULL; }
	for ( i = 1 ; i < MAX_BM_MDL_NO ; i++ ) {	delete m_pBMIntf[i];		m_pBMIntf[i] = NULL; }
}


void IPS_SIM_PORT::Set_Scheduler_Fail_Status( bool bSchSts )
{
	m_bSchFail = bSchSts;
}


bool IPS_SIM_PORT::Get_Scheduler_Fail_Status()
{
	return m_bSchFail;
}


int	IPS_SIM_PORT::Get_Port_Index()
{
	return m_iPortIdx;
}

Module_Status IPS_SIM_PORT::GUI_Start()
{
	char	szRunMsg[32] = { 0, };

	printf( "Function index = %d\n", m_pGUIStart_Func->Get_Index() );

	if ( Get_Port_Index() == 0 ) sprintf( szRunMsg, "START(F)" );
	else sprintf( szRunMsg , "START%d(F)" , Get_Port_Index()+1);
	if ( m_pGUIStart_Func->Get_Index() < 0 )		return SYS_ABORTED;
	return m_pGUIStart_Func->RUN( szRunMsg );
}


Module_Status IPS_SIM_PORT::JOB_Start( char* pszJobConf )
{
	int		i, j, iPjobNo = 0, ioval;
	int		iPortIdx = Get_Port_Index();
	char	szWfrStr[128] = { 0, };
	char	szMIDStr[128] = { 0, };
	char	szTimeData[64] = { 0, };
	char	szJobName[MAX_MULTI_STRING_SIZE + 1] = { 0, };
	char	szJobFullName[MAX_MULTI_STRING_SIZE + 1] = { 0, };
	char	szRecipe[MAX_MULTI_STRING_SIZE + 1] = { 0, };
	int		iPJWfr_Info[MAX_PORT_SLOT] = { 0, };
	int		iCommSts;
	bool	bCommSts;

	// Clamp
	m_pAccessBtn->Write( 1, &iCommSts );
	do {
		// Clamp Check
		if ( m_pClampSts->Read( &iCommSts ) == 1 ) {
			break;
		}

		if ( !WAIT_SECONDS( 0.1 ) )		return SYS_ABORTED;
	} while( true );


	
	// Mapping 관련
	if ( gpJobData->Send_Mapping_Command( iPortIdx ) == SYS_ABORTED ) {
		return SYS_ABORTED;
	}

	do {
		ioval = Get_Main_Status( &bCommSts );
		if ( ioval == 4 ) {
			if ( m_pAccessBtn->Read( &iCommSts ) == 1 ) {
				m_pAccessBtn->Write( 0, &iCommSts );
			}
		}
		else if ( ioval == 5 ) {
			break;
		}
		else if (( ioval == 6 ) || ( ioval == 17 )){
			return SYS_ABORTED;
		}

		if ( !WAIT_SECONDS( 0.1 ) )		return SYS_ABORTED;

	} while( true );


	// 


	m_pJobCfgFile = new IPS_JOB_CfgFile( iPortIdx, pszJobConf );
	iPjobNo = m_pJobCfgFile->Get_ProcessJob_Number();
	if ( Get_Port_MID( szMIDStr ) == false )		return SYS_ABORTED;

	if ( m_pPortMID->Read( szMIDStr ) == false ) {
		_IO_COMMON_PRINTF( 3, "SIMUL", "ERROR", "PORT Mid Read Fail (%s)\n", szMIDStr );
		return SYS_ABORTED;
	}


	gpJobData->Clear_ProcessJobList();

	gpJobData->Clear_LotList_Info();
	gpJobData->Add_LotList_Info( szMIDStr );				// MID 등록

	// ProcessJob Data ----------------------------------------------------------------------------------------------------------------------------------
	// 여기 계속 추가할 것
	for ( i = 0 ; i < iPjobNo ; i++ ) {
		memset( iPJWfr_Info, 0, sizeof( int ) * MAX_PORT_SLOT );															// Wafer Slot info Clear
		memset( szTimeData, 0, sizeof( char ) * 64 );

		gpStr->GetTimeData( szTimeData, TIME_HMSM_CONTINUE );
		if ( m_pJobCfgFile->Get_ProcessJob_Name( i, szJobName, MAX_MULTI_STRING_SIZE ) <= 0 )	continue;
		if ( m_pJobCfgFile->Get_ProcessJob_Slot_Info( i, iPJWfr_Info ) <= 0 )	continue;
	
		// Wafer 정보 등록
		gpJobData->Clear_Wafer_Info( 0 );
		for ( j = 0 ; j < MAX_PORT_SLOT ; j++ ) {
			if ( iPJWfr_Info[j] > 0 ) {
				gpJobData->Add_Wafer_Info( 0, j+1 );
			}
		}

		m_pJobCfgFile->Get_ProcessJob_Recipe( i, szRecipe, MAX_MULTI_STRING_SIZE );
		gpJobData->Set_Recipe( szRecipe );


		sprintf( szJobFullName, "%s_%s", szJobName, szTimeData );

		// Process Job 등록
		gpJobData->Create_Process_Job( szJobFullName, szMIDStr );
		Sleep(100);
		gpJobData->Add_ProcessJobLIst_ProcessJob( szJobFullName );				// ControlJob에 ProcesJob을 등록하기 위해서 
		Sleep(100);
	}

	// ControlJob Data ----------------------------------------------------------------------------------------------------------------------------------
	m_pJobCfgFile->Get_ControlJob_Name( szJobName, MAX_MULTI_STRING_SIZE );

	memset( szTimeData, 0, sizeof( char ) * 64 );
	gpStr->GetTimeData( szTimeData, TIME_HMSM_CONTINUE );

	sprintf( szJobFullName, "%s_%s", szJobName, szTimeData );
	gpJobData->Create_Control_Job( szJobFullName, NULL );						// SemiJob Data에서 뒤쪽의 MID String 빼버릴 것

	// Start Event 
	   // Material Event 추가 할 것
	if ( gpJobData->Send_MapVerify_Command( iPortIdx ) == SYS_ABORTED ) {
		return SYS_ABORTED;
	}


	return SYS_SUCCESS;
}



Module_Status IPS_SIM_PORT::PORT_Load( int ists )
{
	Set_Carrier( ists );

	return SYS_SUCCESS;
}


Module_Status IPS_SIM_PORT::Monitor(void)
{
	int		i;

	//----------------------------------------------------------------------------------------------------
	for ( i = 0 ; i < MAX_PM_MDL_NO ; i++ ) {
		if ( m_pPMFunc[i]->READ_FUNCTION() == SYS_RUNNING )		return SYS_SUCCESS;
		if ( m_pPMIntf[i]->READ_FUNCTION() == SYS_RUNNING )		return SYS_SUCCESS;
	}

	//----------------------------------------------------------------------------------------------------

	for ( i = 0 ; i < MAX_TM_MDL_NO ; i++ ) {
		if ( m_pTMIntf[i]->READ_FUNCTION() == SYS_RUNNING )		return SYS_SUCCESS;
	}

	for ( i = 0 ; i < MAX_FM_MDL_NO ; i++ ) {
		if ( m_pFMIntf[i]->READ_FUNCTION() == SYS_RUNNING )		return SYS_SUCCESS;
	}

	for ( i = 0 ; i < MAX_BM_MDL_NO ; i++ ) {
		if ( m_pBMIntf[i]->READ_FUNCTION() == SYS_RUNNING )		return SYS_SUCCESS;
	}

	return SYS_ABORTED;
}

bool IPS_SIM_PORT::Get_Port_Run()
{
	return m_bPortRun;
}


void IPS_SIM_PORT::Set_Port_Run( bool ists )
{
	m_bPortRun = ists;
}

void IPS_SIM_PORT::Port_Monitor( void* lpDummy )
{
	int		ists, idx = -1;
	bool	bCommSts;
	IPS_SIM_PORT*	pMod = (IPS_SIM_PORT *) lpDummy;

	while( true ) 
	{
		if ( pMod->Get_Main_Control( &bCommSts ) == Running )	break;
		if ( pMod->Get_Main_Status( &bCommSts ) == 17 ) {
			pMod->Set_Scheduler_Fail_Status ( true );
			break;
		}
		Sleep( 2000 );
	}

	while ( TRUE ) 
	{
		ists = pMod->Get_Main_Control( &bCommSts ); 
		if ( pMod->Get_Main_Status( &bCommSts ) == 17 || ists == Idle  ) {
			printf ( "<<<< Cause SCHEDULER_FAIL\n" );
			pMod->Set_Scheduler_Fail_Status ( true );
			break;
		}
		else if (( ists != Running ) && ( ists != Pausing ) && ( ists != Paused ) && ( ists != Waiting )) {
			printf ( "<<<< Cause Wait_Hand_Off\n" );
			break;
		}

		Sleep ( 2000 );
	}


	// Scheduler Fail이 아닌 경우
	if ( !pMod->Get_Scheduler_Fail_Status() ) { 

		_IO_COMMON_PRINTF( 3, "SIMUL", "Success", "Test Success Regist No = %d\n", pMod->Get_Reg_No() );

		while( true) {
			if ( pMod->Get_Main_Status( &bCommSts ) == 19 ) {
				pMod->Set_Carrier( 0 ); 
			}

			//-------------------------------------------------------------------
			Sleep( 10 );
			//-------------------------------------------------------------------

			// Carrier를 Unload 하고 Main Status가 Unloading 상태에서 (Loading 상태로) 변하기 전에 다음 Carrier를 시작하는 문제가 발생하여 추가
			if (( pMod->Get_Carrier() <= 0 ) && 
				( pMod->Get_Main_Status( &bCommSts ) != 19 )) {
					break;
			}
		}
	}
	else {
		_IO_COMMON_PRINTF( 3, "SIMUL", "ABORT", "Test Fail Regist No = %d\n", pMod->Get_Reg_No() );
	}

	idx = pMod->Get_Port_Index();
	gpModMonitor->End_Sequence( idx );
	pMod->Set_Scheduler_Fail_Status(false);
	pMod->Set_Port_Run( false );

	PORTCnt++;
	if ( PORTCnt == 6 ) PORTCnt = 0;
	//gpModMonitor[idx].Stop();

	_endthread ( );
}

void IPS_SIM_PORT::Run_Monitor( void* lpDummy )
{
	int				i;
	bool			bSchFail = false;
	DWORD			dwStart = GetTickCount();

	char			szMsg[64] = { 0, };

	IPS_SIM_PORT*	pMod = (IPS_SIM_PORT *) lpDummy;

	do {
		if ( pMod->Monitor() == SYS_SUCCESS )		dwStart = GetTickCount();

		Sleep( 100 );

		if (( GetTickCount() - dwStart ) >= ( 60 * 1000 * 3 )) {
			//bSchFail = true;
			pMod->Set_Scheduler_Fail_Status( true );
			break;
		}
	} while ( true );

	if ( pMod->Get_Scheduler_Fail_Status() ) {
		if ( pMod->Get_Port_Run() ) {
			
			_IO_COMMON_PRINTF( 3, "SIMUL", "ABORT", "Test Start Fail Regist No = %d\n", pMod->Get_Reg_No() );

			for ( i = 0 ; i < MAX_PORT ; i++ ) {		// All Port Abort Send
			//-------------------------------------------------------------------------------------------
				sprintf( szMsg, "ABORT(F) CM%d", i+1 );
				pMod->m_pSCH->WRITE_FUNCTION_EVENT( szMsg );
				_IO_COMMON_PRINTF( 3, "SIMUL", "ABORT", "Abort Message Send [%s]\n", szMsg );
				Sleep( 5000 );
			}
			//-------------------------------------------------------------------------------------------
		}
	}

	_endthread();
}

bool IPS_SIM_PORT::Get_Start_Wait()
{
	bool	cs;

	while( true ) {
		if ( Get_Main_Control( &cs ) == Waiting || Get_Main_Control( &cs ) == Running )		break;
		if ( !WAIT_SECONDS( 0.1 ) )		return false;
	}

	return true;
}


Module_Status IPS_SIM_PORT::PM_Maint_Count_Zero(void)
{
	int		i;
	char	szMdlStr[64] = { 0, };

	for ( i = 0 ; i < MAX_PM_MDL_NO ; i++ ) {
		if ( m_pPM_MAINT[i]->RUN_SET( "COUNT_ZERO" ) == false )	return SYS_ABORTED;
	}

	while( true ) {
		for ( i = 0 ; i < MAX_PM_MDL_NO ; i++ ) {
			if ( m_pPM_MAINT[i]->READ_FUNCTION() == SYS_RUNNING )	break;
		}

		if ( i >= MAX_PM_MDL_NO )		return SYS_SUCCESS;
		if ( !WAIT_SECONDS( 0.1 ) )		return SYS_ABORTED;
	}

	return SYS_ABORTED;
}


bool IPS_SIM_PORT::Get_End_Status()
{
	return false;
}


Module_Status IPS_SIM_PORT::End_BM_Pump()
{
	int		i;

	while( true ) {
		for ( i = 0 ; i < End_BM_Pump ; i++ ) {
			if ( m_pBM_MAINT[i]->READ_FUNCTION() == SYS_RUNNING )	break;
		}

		if ( i >= End_BM_Pump )	break;
		if (!WAIT_SECONDS( 0.1 ))	return SYS_ABORTED;
	}

	m_pBM_MAINT[0]->RUN_SET( "PUMP" );
	m_pBM_MAINT[1]->RUN_SET( "PUMP" );

	while( true ) {
		for ( i = 0 ; i < 2 ; i++ ) {
			if ( m_pBM_MAINT[i]->READ_FUNCTION() == SYS_RUNNING )	break;
		}

		if ( i >= 2 )	break;
		if (!WAIT_SECONDS( 0.1 ))	return SYS_ABORTED;
	}

	return SYS_SUCCESS;
}

void IPS_SIM_PORT::Set_Reg_No( int ino ) 
{
	m_iRegNo = ino;
}


int IPS_SIM_PORT::Get_Reg_No()
{
	return m_iRegNo;
}


Module_Status IPS_SIM_PORT::Simulation_Run( char* pszRecipe, int iStart, int iEnd, char* pszJobConf, bool bEndWait )
{
	bool	bcs;
	char StrBuffer[64];
	int CommStatus;
	memset( StrBuffer , 0 , sizeof( StrBuffer ) -1  );
	Set_Carrier( 1 );

	Set_Wafer_Range( iStart, iEnd );

	Set_Recipe( pszRecipe );

	Set_Scheduler_Fail_Status( false );

//	_beginthread( Run_Monitor, 0, (void *) this );

	sprintf( StrBuffer , "CASSETTE CM%d|SET" , Get_Port_Index() +1 );
	m_peSch->Write ( StrBuffer , &CommStatus );
	sprintf ( StrBuffer , "TEST%d" , Get_Port_Index()+1);
	m_pPortMID->Write( StrBuffer , &CommStatus);
	//if ( GUI_Start() == SYS_ABORTED )		return	SYS_ABORTED;						// 2019.05.26
	if ( iStart == JOB_BASE_RUN_CODE ) {
		// Manual Start Mode는 지원하면 안될 듯
		if ( JOB_Start( pszJobConf ) == SYS_ABORTED )		return SYS_ABORTED;
	}
	else {
		if ( GUI_Start() == SYS_ABORTED )		return	SYS_ABORTED;					// 2019.05.26
	}

	Set_Port_Run( true );
	_beginthread( Port_Monitor, 0, (void *) this );

	while( true ) {
		if (( Get_Main_Control( &bcs ) == Waiting ) ||
			( Get_Main_Control( &bcs ) == Running ))	break;

		if ( !WAIT_SECONDS( 0.1 ) )		return SYS_ABORTED;
	}
	
	if ( bEndWait ) {
		while( true ) {
			if ( Get_Port_Run() == false )	break;
			if ( !WAIT_SECONDS( 0.1 ) )		return SYS_ABORTED;
		}

		if ( Get_Scheduler_Fail_Status() ) {
			return SYS_ABORTED;
		}
	}
	PORTCnt++;
	return SYS_SUCCESS;
}


void IPS_SIM_PORT::Port_Abort( int iPortIdx )
{
	char	szMsg[100] = { 0, };

	sprintf( szMsg, "ABORT(F) CM%d", iPortIdx+1 );
	m_pSCH->WRITE_FUNCTION_EVENT( szMsg );
}


bool IPS_SIM_PORT::Get_Port_MID(char* pszRetPortID)
{
	if ( m_pPortMID->Get_Index() < 0 )		return false;
	else {
		if ( pszRetPortID == NULL )			return false;
		return m_pPortMID->Read( pszRetPortID );
	}

	return false;
}


int Get_Idle_Port()
{
	int		i, iCarSts;
	bool	bsts;
	
	if ( LPSeq[0] != 0 && LPSeq[1] != 0 && LPSeq[2] != 0 ) {
		for ( i = 0 ; i < MAX_PORT ; i++ ) {
			iCarSts = gpPort[LPSeq[i]-1]->Get_Carrier();
			bsts = gpPort[LPSeq[i]-1]->Get_Port_Run();
			
			if ( gbDebug ) { printf( "[%s] --> [i=%d] / Port [%d] / Get Carrier[%d]\n","Get_Idle_Port", i, ( bsts ? 1 : 0 ), iCarSts ); }

			if ( bsts )	continue;
			if ( iCarSts > 0 )	continue;
			i = LPSeq[i]-1;
			break;
		}
	}
	else {
		for ( i = 0 ; i < MAX_PORT ; i++ ) {
			iCarSts = gpPort[i]->Get_Carrier();
			bsts = gpPort[i]->Get_Port_Run();
			
			if ( gbDebug ) { printf( "[%s] --> [i=%d] / Port [%d] / Get Carrier[%d]\n","Get_Idle_Port", i, ( bsts ? 1 : 0 ), iCarSts ); }

			if ( bsts )	continue;
			if ( iCarSts > 0 )	continue;
			break;
		}
	}

	if ( i >= MAX_PORT )		return -1;

	if ( gbDebug ) { printf( "[%s] --> return value : %d\n", "Get_Idle_Port", i+1 ); }

	return i+1;
}

bool All_Port_Idle()
{
	int	i;
	bool bsts;

	for ( i = 0 ; i < MAX_PORT ; i++ ) {
		bsts = gpPort[i]->Get_Port_Run();
		if ( bsts )		return false;
	}

	return true;
}

Module_Status Simulation_Launcher( int iPort, int ino, char* pszRecipe, int iStart, int iEnd, char* pszPMCond, char* pszJobConf, bool bEndWait , bool b3PortOp)
{
	int		i, idx = -1;
	int		iRunPort = -1;

	if ( b3PortOp == TRUE && PORTCnt >= 3 ) return SYS_WAITING;
	if ( iPort == -1 ) {
		iPort = Get_Idle_Port();
		if ( iPort < 0 )		return SYS_WAITING;
	}

	idx = iPort - 1;

	if ( idx < 0 )		return SYS_ABORTED;

	if ( gpPort[idx]->Get_Port_Run() ) {
		return SYS_ABORTED;
	}

	 _IO_COMMON_PRINTF( 3, "SIMUL", "START", "Regno[%d] / Port[%d] / Recipe[%s] / Slot[%d:%d]\n", ino, iPort, pszRecipe, iStart, iEnd );

	gpPort[idx]->Set_Reg_No( ino );
	gpDataController->Read_PM_Test_Condition( idx, pszPMCond );			// PM Test Condition Read

	//for ( i = 0 ; i < MAX_PM_MDL_NO ; i++ ) {
	//	gpModMonitor[i].Run();
	//}

	return gpPort[idx]->Simulation_Run( pszRecipe, iStart, iEnd, pszJobConf, bEndWait );
}

void RunMonitor( void* lParam )
{
	int		i, j, iRunPort = 0;

	while( true ) {

		for ( i = 0 ; i < MAX_PORT ; i++ ) {
			iRunPort = 0;

			if ( gpPort[i] == NULL )			continue;
			if ( gpPort[i]->Get_Port_Run() ) {
				iRunPort++;
				continue;
			}

			if ( gpPort[i]->Get_Scheduler_Fail_Status() ) {
				_IO_COMMON_PRINTF( 3, "SIMUL", "START", "Port[%d] Scheduler Fail...\n", i+1 );
				for ( j = 0 ; j < MAX_PORT ; j++ ) {
					if ( gpPort[j] == NULL )			continue;
					
					_IO_COMMON_PRINTF( 3, "SIMUL", "START", "Port[%d] Send Abort signal...\n", i+1 );
					gpPort[j]->Port_Abort( j );
				}

				_endthread();
			}
		}

		if ( iRunPort == 0 )							break;			// 모든 Port가 Run상태가 아니면 실행이 되지 않는다.

		Sleep( 100 );
	}

	_endthread();
}

Module_Status Program_Main()
{
	int		iPort = -1, i, iregno = -1;
	
	char	szCmd[256] = { 0, };
	char	szCmd2[256] = { 0, };
	char	szCmd3[256] = { 0, };
	char	szParam[256] = { 0, };
	char	szConfig[MAX_PATH] = { 0, };
	char	szJobConfig[MAX_PATH] = { 0, };			// 2019.05.26

	char	szRecipe[100] = { 0, };
	int		iStart = -1, iEnd = -1;
	
	Module_Status ms = SYS_ABORTED;

	gpDataController->Set_Data_Count(0);
	gpDataController->Init();

	strncpy( szParam, PROGRAM_PARAMETER_READ(), 255 );

	//gpStr->Str_Extract_With_Seperator( szParam, ' ', 1, szCmd, 255 );
	STR_SEPERATE( szParam, szCmd, szCmd2, 255 );

	if		( strcmp( szCmd, "LOAD1" ) == 0 ){ gpPort[0]->Set_Carrier( 1 ); ms = SYS_SUCCESS; }
	else if ( strcmp( szCmd, "LOAD2" ) == 0 ){ gpPort[1]->Set_Carrier( 1 ); ms = SYS_SUCCESS; }
	else if ( strcmp( szCmd, "LOAD3" ) == 0 ){ gpPort[2]->Set_Carrier( 1 ); ms = SYS_SUCCESS; }
	else if ( strcmp( szCmd, "LOAD4" ) == 0 ){ gpPort[3]->Set_Carrier( 1 ); ms = SYS_SUCCESS; }

	else if	( strcmp( szCmd, "UNLOAD1" ) == 0 ){ gpPort[0]->Set_Carrier( 0 ); ms = SYS_SUCCESS; }
	else if ( strcmp( szCmd, "UNLOAD2" ) == 0 ){ gpPort[1]->Set_Carrier( 0 ); ms = SYS_SUCCESS; }
	else if ( strcmp( szCmd, "UNLOAD3" ) == 0 ){ gpPort[2]->Set_Carrier( 0 ); ms = SYS_SUCCESS; }
	else if ( strcmp( szCmd, "UNLOAD4" ) == 0 ){ gpPort[3]->Set_Carrier( 0 ); ms = SYS_SUCCESS; }

	// SET_SLOT1 1:25
	else if ( strncmp( szCmd, "SET_SLOT", 8 ) == 0 ) {
		int		iSlot_S = 0, iSlot_E = 0;
		char	szSLOT_S[32] = { 0, };
		char	szSLOT_E[32] = { 0, };
		gpStr->Str_Extract_With_Seperator( szParam, ' ', 2, szCmd2, 255 );
		STR_SEPERATE_CHAR( szCmd2, ':', szSLOT_S, szSLOT_E, 31 );

		if ( !gpStr->NumStr_Entirety_Check( szSLOT_S ) )		return SYS_ABORTED;
		if ( !gpStr->NumStr_Entirety_Check( szSLOT_E ) )		return SYS_ABORTED;

		iSlot_S = atoi( szSLOT_S );
		iSlot_E = atoi( szSLOT_E );

		if ( iSlot_S <= 0 )										return SYS_ABORTED;
		if ( iSlot_E > 25 )										return SYS_ABORTED;
		if ( iSlot_E < iSlot_S )								return SYS_ABORTED;

		if		( strncmp( szCmd, "SET_SLOT1", 8 ) == 0 )	ms = ( gpPort[0]->Set_Wafer_Range( iSlot_S, iSlot_E ) ? SYS_SUCCESS : SYS_ABORTED );
		else if ( strncmp( szCmd, "SET_SLOT2", 8 ) == 0 ) ms = ( gpPort[1]->Set_Wafer_Range( iSlot_S, iSlot_E ) ? SYS_SUCCESS : SYS_ABORTED );
		else if ( strncmp( szCmd, "SET_SLOT3", 8 ) == 0 ) ms = ( gpPort[2]->Set_Wafer_Range( iSlot_S, iSlot_E ) ? SYS_SUCCESS : SYS_ABORTED );
		else if ( strncmp( szCmd, "SET_SLOT4", 8 ) == 0 ) ms = ( gpPort[3]->Set_Wafer_Range( iSlot_S, iSlot_E ) ? SYS_SUCCESS : SYS_ABORTED );
		else												ms = SYS_ABORTED;
	}
	else if ( strncmp( szCmd, "SET_RECIPE", strlen( "SET_RECIPE" ) ) == 0 ) {
		gpStr->Str_Extract_With_Seperator( szParam, ' ', 2, szCmd2, 255 );

		if		( strncmp( szCmd, "SET_RECIPE1", 8 ) == 0 ) ms = ( gpPort[0]->Set_Recipe( szCmd2 ) ? SYS_SUCCESS : SYS_ABORTED );
		else if ( strncmp( szCmd, "SET_RECIPE2", 8 ) == 0 ) ms = ( gpPort[1]->Set_Recipe( szCmd2 ) ? SYS_SUCCESS : SYS_ABORTED );
		else if ( strncmp( szCmd, "SET_RECIPE3", 8 ) == 0 ) ms = ( gpPort[2]->Set_Recipe( szCmd2 ) ? SYS_SUCCESS : SYS_ABORTED );
		else if ( strncmp( szCmd, "SET_RECIPE4", 8 ) == 0 ) ms = ( gpPort[3]->Set_Recipe( szCmd2 ) ? SYS_SUCCESS : SYS_ABORTED );
		else												  ms = SYS_ABORTED;
	}
	else if ( strncmp( szCmd, "GUI_START", strlen( "GUI_START" ) ) == 0 ) {
		if		( strncmp( szParam, "GUI_START1", 8 ) == 0 ) ms = gpPort[0]->GUI_Start();
		else if ( strncmp( szParam, "GUI_START2", 8 ) == 0 ) ms = gpPort[1]->GUI_Start();
		else if ( strncmp( szParam, "GUI_START3", 8 ) == 0 ) ms = gpPort[2]->GUI_Start();
		else if ( strncmp( szParam, "GUI_START4", 8 ) == 0 ) ms = gpPort[3]->GUI_Start();
		else												 ms = SYS_ABORTED;
	}
	else if ( strncmp( szCmd, "COUNT_ZERO", strlen( "COUNT_ZERO" ) ) == 0 ) {
		ms = gpPort[0]->PM_Maint_Count_Zero();
	}
	// AUTO_RUN  1|
	// 	char	szRecipe[100] = { 0, };
	//	int		iStart = -1, iEnd = -1;
	else if ( strncmp( szCmd, "AUTO_RUN", strlen( "AUTO_RUN" ) ) == 0 ) {
		if ( gpStr->Str_Extract_With_Seperator( szCmd2, '|', 1, &iPort )					== 0 )		return SYS_ABORTED;
		if ( gpStr->Str_Extract_With_Seperator( szCmd2, '|', 2, szRecipe, 99 )				== 0 )		return SYS_ABORTED;
		if ( gpStr->Str_Extract_With_Seperator( szCmd2, '|', 3, &iStart )					== 0 )		return SYS_ABORTED;
		if ( gpStr->Str_Extract_With_Seperator( szCmd2, '|', 4, &iEnd )						== 0 )		return SYS_ABORTED;
		if ( gpStr->Str_Extract_With_Seperator( szCmd2, '|', 4, szConfig, MAX_PATH-1 )		== 0 )		return SYS_ABORTED;

		//---------------------------------------------------------------------------------------------------------------
		//ms = Simulation_Launcher( iPort, -1, szRecipe, iStart, iEnd, szConfig, true );		// 2019.05.26
		ms = Simulation_Launcher( iPort, -1, szRecipe, iStart, iEnd, szConfig, NULL, true , false);		// 2019.05.26
		//---------------------------------------------------------------------------------------------------------------
	}
	else if ( strncmp( szCmd, "RUN2", strlen( "RUN2" ) )	== 0 ) {
		PORTCnt = 0;
		for ( int k = 0 ; k<3; k++ ) {
			LPSeq[k] = 0;
		}
		STR_SEPERATE ( szCmd2 , szCmd3 , szCmd2 , 256 );
		if ( strlen(szCmd2) <=0 ) sprintf ( szCmd2 , szCmd3 );
		else {
			int a= atoi ( szCmd3 );
			LPSeq[0] = a/100;
			a -= LPSeq[0]*100;
			LPSeq[1] = a/10;
			a -= LPSeq[1]*10;
			LPSeq[2] = a;
		}
		_IO_COMMON_PRINTF( 3, "SIMUL", "START", "\n\n==== Simulation Start : [%s] ==== \n", szCmd2 );
		
		if ( gpJobData == NULL ) {
			gpJobData = new KSemi_JobData();
			if ( gpJobData->Redefine_IO( "SCHEDULER", _K_F_IO ) < 0 ) {
				printf( "SCHEDULER find Fail....\n" );
				return SYS_ABORTED;
			}

			if ( gpJobData->JobMem_Open() == false ) {
				_IO_COMMON_PRINTF( 3, "SIMUL", "ERROR", "Job Memory Open Fail...\n" );
			}
		}

		gpSimFile->SetName( szCmd2, 256 );
		gpSimFile->Read();												// 시뮬레이션 메인 파일 Read (Run 정보와 PM Test 정보 (파일 이름 까지만) )
																		// PM Test Condition은 Simulation_Launcher 안에서 읽는다.

		for ( i = 0 ; i < gpDataController->Get_Data_Count() ; i++ ) {
			while( true ) {
				
				if ( gpDataController->GetSimData( i, &iregno, szRecipe, &iStart, &iEnd, szConfig, szJobConfig ) == false )	break; 
				
				//---------------------------------------------------------------------------------------------------------------
				ms = Simulation_Launcher( -1, iregno, szRecipe, iStart, iEnd, szConfig, szJobConfig, false ,TRUE);	// 2019.05.26
				//---------------------------------------------------------------------------------------------------------------
				if ( ms == SYS_ABORTED ) {	// All Port Abort
				} 
				else if ( ms == SYS_SUCCESS )	{
					_IO_COMMON_PRINTF( 3, "SIMUL", "START", "%d.Run Data : Recipe [%s] / Slot [%d:%d] / Config [%s]\n", iregno, szRecipe, iStart, iEnd, szConfig );
					if ( i == 0 ) 	_beginthread( RunMonitor, 0, 0 );
					break;
				}

				if ( !WAIT_SECONDS( 0.1 ) )		return SYS_ABORTED;
			}
		}
		
		while( true ) 
		{
			if ( All_Port_Idle() )	break;
			if ( MANAGER_ABORT() )	return SYS_ABORTED;
			Sleep( 1 );
		}
		
		return SYS_SUCCESS;
	}
	else if ( strncmp( szCmd, "RUN", strlen( "RUN" ) )	== 0 ) {
		for ( int k = 0 ; k<3; k++ ) {
			LPSeq[k] = 0;
		}
		STR_SEPERATE ( szCmd2 , szCmd3 , szCmd2 , 256 );
		if ( strlen(szCmd2) <=0 ) sprintf ( szCmd2 , szCmd3 );
		else {
			int a= atoi ( szCmd3 );
			LPSeq[0] = a/100;
			a -= LPSeq[0]*100;
			LPSeq[1] = a/10;
			a -= LPSeq[1]*10;
			LPSeq[2] = a;
		}
		_IO_COMMON_PRINTF( 3, "SIMUL", "START", "\n\n==== Simulation Start : [%s] ==== \n", szCmd2 );
		
		if ( gpJobData == NULL ) {
			gpJobData = new KSemi_JobData();
			if ( gpJobData->Redefine_IO( "SCHEDULER", _K_F_IO ) < 0 ) {
				printf( "SCHEDULER find Fail....\n" );
				return SYS_ABORTED;
			}

			if ( gpJobData->JobMem_Open() == false ) {
				_IO_COMMON_PRINTF( 3, "SIMUL", "ERROR", "Job Memory Open Fail...\n" );
			}
		}

		gpSimFile->SetName( szCmd2, 256 );
		gpSimFile->Read();												// 시뮬레이션 메인 파일 Read (Run 정보와 PM Test 정보 (파일 이름 까지만) )
																		// PM Test Condition은 Simulation_Launcher 안에서 읽는다.

		for ( i = 0 ; i < gpDataController->Get_Data_Count() ; i++ ) {
			while( true ) {
				
				if ( gpDataController->GetSimData( i, &iregno, szRecipe, &iStart, &iEnd, szConfig, szJobConfig ) == false )	break; 
				
				//---------------------------------------------------------------------------------------------------------------
				ms = Simulation_Launcher( -1, iregno, szRecipe, iStart, iEnd, szConfig, szJobConfig, false , false);	// 2019.05.26
				//---------------------------------------------------------------------------------------------------------------
				if ( ms == SYS_ABORTED ) {	// All Port Abort
				} 
				else if ( ms == SYS_SUCCESS )	{
					_IO_COMMON_PRINTF( 3, "SIMUL", "START", "%d.Run Data : Recipe [%s] / Slot [%d:%d] / Config [%s]\n", iregno, szRecipe, iStart, iEnd, szConfig );
					if ( i == 0 ) 	_beginthread( RunMonitor, 0, 0 );
					break;
				}

				if ( !WAIT_SECONDS( 0.1 ) )		return SYS_ABORTED;
			}
		}

		


		while( true ) 
		{
			if ( All_Port_Idle() )	break;
			if ( MANAGER_ABORT() )	return SYS_ABORTED;
			Sleep( 1 );
		}
		
		return SYS_SUCCESS;
	}
	
	return ms;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void Display_Port_Status()
{
	int		i;

	for ( i = 0 ; i < MAX_PORT ; i++ ) {
		printf( "[%d] Run = %d / Carrier = %d\n", gpPort[i]->Get_Port_Run(), gpPort[i]->Get_Carrier() );
	}
}
//---------------------------------------------------------------------------------------
void Event_Message_Received() 
{
	char	szEvnt[512] = { 0, };

	strncpy( szEvnt, PROGRAM_EVENT_READ(), 511 );

	if		( STRCMP_L( szEvnt, "PORT_STS" ) )		Display_Port_Status();
	else if ( STRCMP_L( szEvnt, "DEBUG_ON" ) )		gbDebug = TRUE;
	else if ( STRCMP_L( szEvnt, "DEBUG_OFF" ) )		gbDebug = FALSE;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
void Screen_Nav_Info( int no ) 
{
	if ( no < 0 ) {
		delete gpStr;		gpStr = NULL;
	}
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
void Program_Enter_Code() 
{
	
	REGIST_FILE_INFO( "SCHEDULER SIMULATOR" );
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
BOOL Program_Init_Code() 
{
	int		i ;
	char	szName[128] = { 0, };

	gpStr = new KStr();
	gpSimFile = new IPS_SIM_File( "", 256 );
	gpDataController = new IPS_SIM_DATA_Controller( 1000 );
	gpModMonitor = new IPS_PM_Monitor[MAX_PM_MDL_NO];

	gpSeqRun_Mode  = new KIO_Digital( "IO_Driver_Loding" );


	gpPort[0] = new IPS_SIM_PORT( 0, "LPMA.F_ProperlyPlaceDI", "LPMA.FOUP_PresentDI", "CM1.LOADING_SIGNAL", "CM1.UNLOADING_SIGNAL", "LPMA.Switch2LoadDI", "LPMA.Switch1UnloadDI", "GEM.OPERATORSTARTINT_CONTROL" );
	gpPort[1] = new IPS_SIM_PORT( 1, "LPMB.F_ProperlyPlaceDI", "LPMB.FOUP_PresentDI", "CM2.LOADING_SIGNAL", "CM2.UNLOADING_SIGNAL", "LPMB.Switch2LoadDI", "LPMB.Switch1UnloadDI", "GEM.OPERATORSTARTINT_CONTROL2" );
	gpPort[2] = new IPS_SIM_PORT( 2, "LPMC.F_ProperlyPlaceDI", "LPMC.FOUP_PresentDI", "CM3.LOADING_SIGNAL", "CM3.UNLOADING_SIGNAL", "LPMC.Switch2LoadDI", "LPMC.Switch1UnloadDI", "GEM.OPERATORSTARTINT_CONTROL3" );

	gpPort[0]->Set_IO_Wafer_Range( "CTC.START_SLOT",  "CTC.END_SLOT"  );
	gpPort[1]->Set_IO_Wafer_Range( "CTC.START_SLOT2", "CTC.END_SLOT2" );
	gpPort[2]->Set_IO_Wafer_Range( "CTC.START_SLOT3", "CTC.END_SLOT3" );

	// Thread Run --------------------------------------------------------------------------
	for ( i = 0 ; i < MAX_PORT ; i++ ) {
		//----------------------------------------------------------------------------------
		// Recipe
		//----------------------------------------------------------------------------------
		if ( i == 0 )	strcpy( szName, "CTC.RECIPE_FILE" );
		else			sprintf( szName, "CTC.RECIPE_FILE%d", i+1 );

		gpPort[i]->Set_IO_Recipe( szName );

		//----------------------------------------------------------------------------------
		// Main Control
		//----------------------------------------------------------------------------------
		if ( i == 0 )	strcpy( szName, "CTC.MAIN_CONTROL" );
		else			sprintf( szName, "CTC.MAIN_CONTROL%d", i+1 );

		gpPort[i]->Set_IO_MainControl( szName );

		//----------------------------------------------------------------------------------
		// Main Status
		//----------------------------------------------------------------------------------
		if ( i == 0 )	strcpy( szName, "CTC.MAIN_STATUS" );
		else			sprintf( szName, "CTC.MAIN_STATUS%d", i+1 );

		gpPort[i]->Set_IO_MainStatus( szName );

		//----------------------------------------------------------------------------------
		// Monitor
		//----------------------------------------------------------------------------------
//		gpModMonitor[i].Set_Port_Idx( i );			// PM별로 Monitoring 해야 하므로 삭제함
	}

	for ( i = 0 ; i < MAX_PM_MDL_NO ; i++ ) {
		gpModMonitor[i].Set_PM_Idx( ePM1+i );
		//gpModMonitor[i].Ready();

		gpModMonitor[i].Monitor();
		//gpModMonitor[i].Run();
	}

	return TRUE;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

