#include <stdio.h>
#include <windows.h>
#include <time.h>


#include "Parameter.h"

#ifdef __cplusplus 
extern "C" { 
#endif 

#include "CimSeqnc.h"
#include "kutlStr.h"

#ifdef __cplusplus 
}
#endif 

#include "Parameter.h"
#include "KIO_Digital.h"
#include "KSeqUtil.h"

#include "KApp_IO_PM.h"

#include "IPS_PM_Monitor.h"

#include "Sequence.h"
#include "IPS_SIM_DATA_Controller.h"
#include "IPS_SIM_PM_Data.h"

#include "KIO.h"
#include "KSeqUtil.h"


extern IPS_SIM_PORT*			gpPort[MAX_PORT];
extern IPS_SIM_DATA_Controller*	gpDataController;

extern KStr*						gpStr;

int	   giLastDisable_Port[MAX_PM_MDL_NO] = { 0, };

IPS_PM_Monitor::IPS_PM_Monitor()
{
	int		i;
	char	szMdlStr[64] = { 0, };

	//m_iPortIdx = iPortIdx;
	m_iRunCnt = -1;
	//KSCH_Event*			m_pSchEvnt;
	m_pSchEvnt = new KSCH_Event();
	m_pSchEvnt->Find_Sch_Address();
	for( i=1 ; i <= MAX_PM_MDL_NO ; i++ )
	{
		sprintf( szMdlStr , "CTC.FA_STATUS_MDL_PM%d",i );
		m_pPM_Sts[i-1] = new KIO_Digital( szMdlStr ); 

		giLastDisable_Port[i-1] = -1;
	}

	m_pPM = new KApp_IO_PM();
}


IPS_PM_Monitor::~IPS_PM_Monitor(void)
{
	if ( m_pPM != NULL )	delete[] m_pPM;
}


void IPS_PM_Monitor::Set_Last_Chamber_Disable_Changer( int iPortIdx, int iPM_Mdl_No, int iSts )
{
	if ( iPM_Mdl_No <= 0 )					return;
	if ( iSts == eCH_SET_DISABLE )  {
		giLastDisable_Port[iPM_Mdl_No-1] = iPortIdx;
	}
	else {
		giLastDisable_Port[iPM_Mdl_No-1] = -1;
	}
}


int  IPS_PM_Monitor::Get_Last_Chamber_Disable_Changer( int iPM_Mdl_No )
{
	return giLastDisable_Port[iPM_Mdl_No-1];
}


void IPS_PM_Monitor::Set_PM_Idx( int idx )
{
	m_iPMidx = idx;

	if ( m_pPM == NULL )				return;
	m_pPM->Set_Make_PM_Info( idx + 1, 6 );
}

int  IPS_PM_Monitor::Get_PM_Idx()
{
	return m_iPMidx;
}

bool IPS_PM_Monitor::Load_Check_Condition(int iPortIdx, int iWfrNo, int& iCondIdx ) 
{
	int		i, iStartSlot = -1, iEndSlot = -1, iStartPnt = 0;
	int		idx = -1;
	int		iChk = false;
	int ComStatus;
	int iStartCondIdx = -1;
	iStartPnt = ( iPortIdx * MAX_PM_MDL_NO * MAX_PM_ACTION ) + ( m_iPMidx * MAX_PM_ACTION );

	for ( i = 0 ; i < MAX_PM_ACTION ; i++ ) {
		if ( gpDataController->Get_SimData_Status( iStartPnt + i ) == ePM_REG ) {
			idx = iStartPnt + i;
			break;
		}
	}

	iCondIdx = idx;		// 2019.03.20

	if ( idx == -1 )	return false;

	iStartSlot = gpDataController->Get_SimData_Start_Slot( idx );
	iEndSlot = gpDataController->Get_SimData_End_Slot( idx );


	if ( iStartSlot <= 0 )		iStartSlot = 1;
	if ( ( iEndSlot == -1 ) || ( iEndSlot >= 25 ))	iEndSlot = 25;

	for( i = iStartSlot ; i <= iEndSlot ; i++ ) {
		if ( iWfrNo == i ) {
			return true;
		}
	}

	if( iCondIdx % 50 != 0 ) {
		for ( i = 0 ; i <= MAX_PM_MDL_NO ; i++ ) {
			if ( m_pPM_Sts[i]->Read( &ComStatus ) == 2 ) return true;  //Maint 상태의 PM이 있있을때
			if ( !ComStatus ) return false;
		}
	}


	return false;
}


bool IPS_PM_Monitor::Get_Run_Info( int idx, DWORD& dwWaitTime, CH_ACT& ChAction, char* pszTargetName, char* pszRunMsg, int& iSimDataType )
{
	if ( idx < 0 )		return false;

	dwWaitTime = gpDataController->Get_SimData_DelayTime( idx );
	ChAction = gpDataController->Get_SimData_Chamber_Action( idx );
	strcpy( pszTargetName, gpDataController->Get_SimData_TargetName( idx ) );
	strcpy( pszRunMsg, gpDataController->Get_SimData_RunMsg( idx ) );
	iSimDataType = gpDataController->Get_SimData_Type( idx );

	return true;
}


void IPS_PM_Monitor::Set_Run_Status( int iPortIdx, CH_ACT_STS iSts )
{
	int		i, idx = -1, iStartPnt = 0;
	iStartPnt = ( iPortIdx * MAX_PM_MDL_NO * MAX_PM_ACTION ) + ( m_iPMidx * MAX_PM_ACTION );
	for ( i = 0 ; i < MAX_PM_ACTION ; i++ ) {
		if ( gpDataController->Get_SimData_Status( iStartPnt + i ) == ePM_REG ) {
			idx = iStartPnt + i;
			break;
		}
	}

	if ( idx == -1 )	return;

	gpDataController->Set_SimData_Status( idx, iSts );
}

// 0 : Enable/Disable Event가 아님
// 1 : Enable Event 임
// 2 : Disable Event 임
int	 IPS_PM_Monitor::Check_Enable_Disable_Event( char* szEvnt, int& irPM_Num )
{
	// SET_MODULE_INFO_LOCAL
	int		i, iMdlNo = 0 ;
	char	szBuffer[3][64];

	for ( i = 0 ; i < 3 ; i++ ) {
		memset( &szBuffer[i], 0, sizeof( char ) * 64 );
	}

	::STR_SEPERATE( szEvnt, szBuffer[0], szBuffer[1], 63 );

	if ( strncmp( szBuffer[0], "SET_MODULE_INFO_LOCAL", strlen( "SET_MODULE_INFO_LOCAL" ) ) == 0 ) {
		if ( strncmp( szBuffer[1], "PM", strlen( "PM" ) ) == 0 ) {
			STR_SEPERATE_CHAR( szBuffer[1], '|', szBuffer[0], szBuffer[2], 63 );
			iMdlNo = gpStr->Get_Module_Number( "PM", szBuffer[0] );
			if ( iMdlNo <= 0 )	return 0;
			else if ( iMdlNo > 0 ) {
				irPM_Num = iMdlNo;
				if		( strcmp( szBuffer[2], "ENABLE" ) == 0 )			return 1;
				else if ( strcmp( szBuffer[2], "DISABLE" ) == 0 )			return 2;
				else														return 0;
			}
		}
		else {
			return 0;
		}
	}

	return 0;
}

bool IPS_PM_Monitor::Execute_Action( int iPortIdx, int iRegNo, int index )
{
	int		iMdlNo = 0, iRet = 0;
	int		iPMNo = Get_PM_Idx() +1;
	CH_ACT	chAction = eCh_UNK;

	chAction = gpDataController->Get_SimData_Chamber_Action( index );

	if		( chAction == eCh_Disable ) {
		m_pSchEvnt->Send_Event_PM_Disable( iPMNo );
		Set_Last_Chamber_Disable_Changer( iPortIdx, iPMNo, eCH_SET_DISABLE );
		_IO_COMMON_PRINTF( 3, "SIMUL", "ACTION", "[ Reg No = %d ][ Port = %d ][ GroupName = %s ]\tChamber Disable = %d\n",
			iRegNo, iPortIdx+1, gpDataController->Get_SimData_GrpName( index ), iPMNo );
	}
	else if ( chAction == eCh_Enable ) {
		m_pSchEvnt->Send_Event_PM_Enable( iPMNo );
		Set_Last_Chamber_Disable_Changer( iPortIdx, iPMNo, eCH_SET_ENABLE );
		_IO_COMMON_PRINTF( 3, "SIMUL", "ACTION", "[ Reg No = %d ][ Port = %d ][ GroupName = %s ]\tChamber Enable = %d\n",
			iRegNo, iPortIdx+1, gpDataController->Get_SimData_GrpName( index ), iPMNo );
	}
	else if ( chAction == eFC_EVNT ) {
		KIO_Function*	pkFunc = NULL;
		pkFunc = new KIO_Function( gpDataController->Get_SimData_TargetName( index ) );
		pkFunc->WRITE_FUNCTION_EVENT( gpDataController->Get_SimData_RunMsg( index ) );

		iRet = Check_Enable_Disable_Event( gpDataController->Get_SimData_RunMsg( index ), iMdlNo );
		if ( iRet > 0 ) {
			if ( iRet == 1 ) {
				Set_Last_Chamber_Disable_Changer( iPortIdx, iMdlNo, eCH_SET_ENABLE );
			}
			else if ( iRet == 2 ) {
				Set_Last_Chamber_Disable_Changer( iPortIdx, iMdlNo, eCH_SET_DISABLE );
			}
		}
				
		_IO_COMMON_PRINTF( 3, "SIMUL", "ACTION", "[ Reg No = %d ][ Port = %d ][ GroupName = %s ]\tFunction Event = %d(%s)\n",
			iRegNo, iPortIdx+1, gpDataController->Get_SimData_GrpName( index ), pkFunc->Get_Index(), gpDataController->Get_SimData_RunMsg( index ) );
		delete pkFunc;
	}
	else if ( chAction == eFC_RUN ) {
		KIO_Function*	pkFunc = NULL;
		pkFunc = new KIO_Function( gpDataController->Get_SimData_TargetName( index ) );
		pkFunc->RUN_SET( gpDataController->Get_SimData_RunMsg( index ) );

		_IO_COMMON_PRINTF( 3, "SIMUL", "ACTION", "Run Set = %d(%s)\n", pkFunc->Get_Index(), gpDataController->Get_SimData_RunMsg( index ) );
		delete pkFunc;
	}
	else if ( chAction == eIO_SET ) {
		KIO* pIO = new KIO( gpDataController->Get_SimData_TargetName( index ), gpDataController->Get_SimData_Type( index ) );
		pIO->Write_IO( gpDataController->Get_SimData_RunMsg( index ) );

		_IO_COMMON_PRINTF( 3, "SIMUL", "ACTION", "IO Set = %d / Type = %d / Value = %s\n", pIO->Get_Index(), gpDataController->Get_SimData_Type( index ), gpDataController->Get_SimData_RunMsg( index ) );
		delete pIO;
	}
	else if (( chAction >= eCh_Disable_PM1 ) && ( chAction < eCh_Enable_PM1 )) {		// 다른 Chamber Disable
		iPMNo = chAction - eCh_Disable_PM1 + 1;
		m_pSchEvnt->Send_Event_PM_Disable( iPMNo );
		Set_Last_Chamber_Disable_Changer( iPortIdx, iPMNo, eCH_SET_DISABLE );

		_IO_COMMON_PRINTF( 3, "SIMUL", "ACTION", "[ Reg No = %d ][ Port = %d ][ GroupName = %s ]\tChamber Disable = %d\n",
			iRegNo, iPortIdx+1, gpDataController->Get_SimData_GrpName( index ), iPMNo );
	}
	else if ( chAction >= eCh_Enable_PM1 ) {											// 다른 Chamber Enable
		iPMNo = chAction - eCh_Enable_PM1 + 1;
		m_pSchEvnt->Send_Event_PM_Enable( iPMNo );
		Set_Last_Chamber_Disable_Changer( iPortIdx, iPMNo, eCH_SET_ENABLE );

		_IO_COMMON_PRINTF( 3, "SIMUL", "ACTION", "[ Reg No = %d ][ Port = %d ][ GroupName = %s ]\tChamber Enable = %d\n",
			iRegNo, iPortIdx+1, gpDataController->Get_SimData_GrpName( index ), iPMNo );
	}
	
	return true;
}

bool IPS_PM_Monitor::IsCheck_Slot( int iSlot )
{
	int	idx = -1;
	idx = iSlot - 1;

	return m_bChk_OK_PORT_Slot[idx];
}


void IPS_PM_Monitor::Set_Check_Slot( int iSlot, bool bsts )
{
	int	idx = -1;
	idx = iSlot - 1;

	m_bChk_OK_PORT_Slot[idx] = bsts;
}


bool IPS_PM_Monitor::DelayTime( DWORD dwTime, int iCondIdx )
{
	DWORD dwCrtTime = 0;
	DWORD dwEndTime = GetTickCount() + dwTime ;

	if ( dwTime <= 0 )		return true;

	do {
		Sleep( 1 );
		if ( MANAGER_ABORT() )		return false;
		dwCrtTime = GetTickCount();
		if (  gpDataController->Get_SimData_Status( iCondIdx ) != ePM_REG ) return true;
	} while ( dwEndTime >= dwCrtTime );

	return true;
}


bool IPS_PM_Monitor::Task()
{
	int		iSimData, iCondIdx = -1, iWfrSts = 0, iPortIdx , iRegNo;
	bool	bresult = false;
	DWORD	dwWaitTime = 0;
	CH_ACT	chAction = eCh_UNK;
	char	szTargetName[128] = { 0, };
	char	szRunMsg[64] = { 0, };

	iPortIdx = m_pPM->Get_Source( 1 );
	iRegNo = gpPort[iPortIdx]->Get_Reg_No();
	if ( Load_Check_Condition( iPortIdx, m_pPM->Get_Slot_No(1), iCondIdx ) == false )	{		// 1번 Slot에 있는 Wafer의 source, slot no을 통해서 codition index를 받는다.
		return true;		// 다음 기회에...
	}

	//========================================================================================================
	printf( "Cond Index = %d\n", iCondIdx );
	//========================================================================================================
	if ( iCondIdx < 0 )					return true;

	//========================================================================================================
	Get_Run_Info( iCondIdx, dwWaitTime, chAction, szTargetName, szRunMsg, iSimData );
	//========================================================================================================
	if ( DelayTime( dwWaitTime , iCondIdx ) == false )									return false;								// WaitTime
	Set_Run_Status( iPortIdx, ePM_RUN );
	if ( Execute_Action( iPortIdx, iRegNo, iCondIdx ) == false )		return false;

	return true;
}

void IPS_PM_Monitor::End_Sequence(int iPortIdx)
{
	int		i;
	for ( i = 0 ; i <= MAX_PM_ACTION ; i++ ) Set_Run_Status( iPortIdx, ePM_RUN );
	for ( i = 1 ; i <= MAX_PM_MDL_NO ; i++ ) {
		if ( Get_Last_Chamber_Disable_Changer( i ) == iPortIdx ) {
			m_pSchEvnt->Send_Event_PM_Enable( i );
			Set_Last_Chamber_Disable_Changer( iPortIdx, i, eCH_SET_ENABLE );
			_IO_COMMON_PRINTF( 3, "SIMUL", "ACTION", "[ Port = %d ] \tLOT_END!! PM%d => Enable\n", iPortIdx+1,i);
		}
	}

}