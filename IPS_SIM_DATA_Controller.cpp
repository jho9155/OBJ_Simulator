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

#include "IPS_SIM_DATA_Controller.h"
#include "IPS_SIM_PM_DATA.h"

extern KStr*	pStr;



IPS_SIM_DATA_Controller::IPS_SIM_DATA_Controller(unsigned int iMaxSize)
{
	unsigned int		i;

	
	
	// memory clear -------------------------------------------------------------
	m_iMaxSize = iMaxSize;
	m_szSIMData = new IPS_SIM_DATA[iMaxSize];

	for ( i = 0 ; i < iMaxSize ; i++ ) {
		memset( &m_szSIMData[i], 0, sizeof( IPS_SIM_DATA ) );
	}

	m_iDataCnt = 0;
	// --------------------------------------------------------------------------

	// PM Data ------------------------------------------------------------------
	m_szSIM_PM_Data = new IPS_SIM_PM_DATA[ MAX_PORT * MAX_PM_MDL_NO * MAX_PM_ACTION ];
	m_piRunPointer = new int[ MAX_PORT ];
	for ( i = 0 ; i < MAX_PORT * MAX_PM_MDL_NO * MAX_PM_ACTION ; i++ ) {
		memset( &m_szSIM_PM_Data[i], 0, sizeof( IPS_SIM_PM_DATA ));
	}

	for ( i = 0 ; i < MAX_PORT ; i++ ) {
		m_piRunPointer[i] = 0;
	}
	// --------------------------------------------------------------------------

	m_pReadIni = new IPS_PM_CfgFile( -1, "" );
}


IPS_SIM_DATA_Controller::~IPS_SIM_DATA_Controller(void)
{
	delete[] m_szSIMData;
	delete[] m_szSIM_PM_Data;
	delete[] m_piRunPointer;

	delete m_pReadIni;
}


void IPS_SIM_DATA_Controller::Set_Max_Size( unsigned int iSize )
{
	m_iMaxSize = iSize;
}


unsigned int IPS_SIM_DATA_Controller::Get_Max_Size()
{
	return m_iMaxSize;
}

void IPS_SIM_DATA_Controller::Init()
{
	int		i;
	

	for ( i = 0 ; i < m_iMaxSize ; i++ ) {
		m_szSIMData[i].Set_Data( NULL, NULL, NULL, NULL, NULL );
		m_szSIMData[i].Set_Run_Flag( true );
	}
}

bool IPS_SIM_DATA_Controller::Append( int ino, char* pszRecipe, int iStart, int iend, char* pszConfigData )
{
	unsigned int		i;

	for ( i = 0 ; i < m_iMaxSize ; i++ ) {
		if (m_szSIMData[i].Get_Data( NULL, NULL, NULL, NULL, NULL, NULL ) == false )	break;
	}

	if ( i >= m_iMaxSize )	return false;
	
	m_szSIMData[i].Set_Data( ino, pszRecipe, iStart, iend, pszConfigData );
	m_szSIMData[i].Set_Run_Flag( false );

	m_iDataCnt++;

	return true;
}

bool IPS_SIM_DATA_Controller::Append( int ino, char* pszJobFile, char* pszConfig )
{
	unsigned int		i;

	for ( i = 0 ; i < m_iMaxSize ; i++ ) {
		if (m_szSIMData[i].Get_Data( NULL, NULL, NULL, NULL, NULL, NULL) == false )	break;
	}

	if ( i >= m_iMaxSize )	return false;

	m_szSIMData[i].Set_Data( ino, "<JOB>", pszJobFile, pszConfig );
	m_szSIMData[i].Set_Run_Flag( false );

	m_iDataCnt++;

	return true;
}



bool IPS_SIM_DATA_Controller::GetSimData( int ino, int* piReg, char* pszRecipe, int* piStart, int* piEnd, char* pszConfigData, char* pszJobData )
{
	unsigned int	i;
	int		ino2 = 0 ;
	char	szRecipe[100] = { 0, };
	int		iStart = -1, iEnd = -1;
	char	szConfig[MAX_PATH] = { 0, };
	char	szJobConfig[MAX_PATH] = { 0, };

	if ( ino == -1 ) {		// GUI Start
		for ( i = 0 ; i < m_iMaxSize ; i++ ) {
			if (m_szSIMData[i].Get_Data( &ino2, szRecipe, &iStart, &iEnd, szConfig, szJobConfig ) == false )	break;
			if ( m_szSIMData[i].Get_Run_Flag() ) continue;
			//-------------------------------------------------------------------------------------------------------
			if ( piReg != NULL )			*piReg = ino2;
			if ( pszRecipe != NULL )		strncpy( pszRecipe, szRecipe, 100 );
			if ( piStart != NULL )			*piStart = iStart;
			if ( piEnd != NULL )			*piEnd = iEnd;
			if ( pszConfigData != NULL )	strncpy( pszConfigData, szConfig, MAX_PATH - 1 );
			if ( pszJobData != NULL ) 		strncpy( pszJobData, szJobConfig, MAX_PATH - 1 );
			//-------------------------------------------------------------------------------------------------------

			return true;
		}
	}
	else if ( ino >= 1000 ) {
		_IO_COMMON_PRINTF ( 3 , "SIMUL" , "RegNo. Err" , "Number of Registry > 1000" );
		return false;
	}
	else {
	
		
			//-------------------------------------------------------------------------------------------------------
		memset( szRecipe, 0, sizeof( char ) * 100 );
		iStart = -1;	iEnd = -1;
		memset( szConfig, 0, sizeof( char ) * MAX_PATH );
		//-------------------------------------------------------------------------------------------------------
		if (m_szSIMData[ino].Get_Data( &ino2, szRecipe, &iStart, &iEnd, szConfig, szJobConfig ) == false )	return false;
		else {
			if ( piReg != NULL )			*piReg = ino2;
			if ( pszRecipe != NULL )		strncpy( pszRecipe, szRecipe, 100 );
			if ( piStart != NULL )			*piStart = iStart;
			if ( piEnd != NULL )			*piEnd = iEnd;
			if ( pszConfigData != NULL )	strncpy( pszConfigData, szConfig, MAX_PATH - 1 );
			if ( pszJobData != NULL ) 		strncpy( pszJobData, szJobConfig, MAX_PATH - 1 );

				return true;
			}
			//-------------------------------------------------------------------------------------------------------
		
	}

	return false;
}


int	IPS_SIM_DATA_Controller::Get_Data_Count()
{
	return this->m_iDataCnt;
}

void IPS_SIM_DATA_Controller::Set_Data_Count( int iCnt )
{
	m_iDataCnt = iCnt;
}

// PM의 Test 조건을 Read한다.
bool IPS_SIM_DATA_Controller::Read_PM_Test_Condition( int iPortidx, char* pszCondFile )
{
	m_pReadIni->Set_Port_Index( iPortidx );
	if ( m_pReadIni->ReadData( pszCondFile ) == false )		return false;

	return true;
}

void IPS_SIM_DATA_Controller::PM_Init( int iPort_idx )
{
	int		i;
	int		iStartPnt = 0;

	for ( i = 0 ; i < MAX_PM_MDL_NO * MAX_PM_ACTION ; i++ ) {
		iStartPnt = iPort_idx * MAX_PM_MDL_NO * MAX_PM_ACTION + i;
		m_szSIM_PM_Data[iStartPnt].Set_Status( ePM_NONE );
	}
}


bool IPS_SIM_DATA_Controller::PM_Append_Data( int iPort_idx, int iStart, int iEnd, CHAMBER iChamber, DWORD dwDelayTime, CH_ACT iChAction, char* szGrpName, char* szActParam1, char* szActParam2, char* szActParam3, char* szActParam4, char* szActParam5  )
{
	int		i;
	int		iCnt = 0;			// Data OverFlow 때문에 Check 용으로
	int		iStartPnt = 0;		// Wafer Start Pointer
	
	iStartPnt = iPort_idx * MAX_PM_MDL_NO * MAX_PM_ACTION;													// Start Pointer를 변경할 것 (각 PM의 정보를 다르게 저장해야 한다)
	iStartPnt = iStartPnt + ( ( iChamber - ePM1 ) * MAX_PM_ACTION );

	for ( i = 0 ; i < MAX_PM_ACTION ; i++ ) {
		if ( m_szSIM_PM_Data[i+iStartPnt].Get_Status() == ePM_NONE )	break;
	}

	if ( i >= MAX_PM_ACTION )		return false;

	//--------------------------------------------------------------------------------------
	m_szSIM_PM_Data[i+iStartPnt].Set_Status( ePM_REG );
	m_szSIM_PM_Data[i+iStartPnt].Set_Wafer_Range( iStart, iEnd );
	m_szSIM_PM_Data[i+iStartPnt].Set_Delay_Time( dwDelayTime );
	m_szSIM_PM_Data[i+iStartPnt].Set_Chamber( iChamber );
	m_szSIM_PM_Data[i+iStartPnt].Set_Chamber_Action( iChAction );
	m_szSIM_PM_Data[i+iStartPnt].Set_GrpName( szGrpName );

	if (( iChAction == eFC_EVNT ) || ( iChAction == eFC_RUN )) {
		m_szSIM_PM_Data[i+iStartPnt].Set_TargetName( szActParam1 );
		m_szSIM_PM_Data[i+iStartPnt].Set_RunMsg( szActParam2 );
	}
	else if ( iChAction == eIO_SET ) {
		m_szSIM_PM_Data[i+iStartPnt].Set_TargetName( szActParam1 );
		m_szSIM_PM_Data[i+iStartPnt].Set_Type( pStr->Get_IO_Type_From_String( szActParam2 ));
		m_szSIM_PM_Data[i+iStartPnt].Set_RunMsg( szActParam3 );
	}
	//--------------------------------------------------------------------------------------

	return true;
}


int  IPS_SIM_DATA_Controller::Get_PM_Data_Index( int iPort_Idx, CHAMBER iChamber )
{
	int	i, iDataidx = -1;
	int	iStartPnt = iPort_Idx * MAX_PM_ACTION;
	iStartPnt = iStartPnt + ( ( iChamber - ePM1 ) * MAX_PM_ACTION );

	for ( i = 0 ; i < MAX_PM_ACTION ; i++ ) {
		if ( m_szSIM_PM_Data[iStartPnt+i].Get_Status() == ePM_REG ) {
			iDataidx = i;

			break;
		}
	}

	return iDataidx;
}


int IPS_SIM_DATA_Controller::Get_SimData_Status( int idx )
{
	return m_szSIM_PM_Data[idx].Get_Status();
}


void IPS_SIM_DATA_Controller::Set_SimData_Status( int idx, CH_ACT_STS ists )
{
	m_szSIM_PM_Data[idx].Set_Status( ists );
}


int IPS_SIM_DATA_Controller::Get_SimData_Start_Slot( int idx )
{
	return m_szSIM_PM_Data[idx].Get_Start_Wafer();
}


int IPS_SIM_DATA_Controller::Get_SimData_End_Slot( int idx )
{
	return m_szSIM_PM_Data[idx].Get_End_Wafer();
}


CHAMBER IPS_SIM_DATA_Controller::Get_SimData_Chamber( int idx )
{
	return m_szSIM_PM_Data[idx].Get_Chamber();
}


DWORD IPS_SIM_DATA_Controller::Get_SimData_DelayTime( int idx )
{
	return m_szSIM_PM_Data[idx].Get_Delay_Time();
}


CH_ACT IPS_SIM_DATA_Controller::Get_SimData_Chamber_Action( int idx )
{
	return m_szSIM_PM_Data[idx].Get_Chamber_Action();
}


char* IPS_SIM_DATA_Controller::Get_SimData_TargetName( int idx )
{
	return m_szSIM_PM_Data[idx].Get_TargetName();
}

char* IPS_SIM_DATA_Controller::Get_SimData_RunMsg( int idx )
{
	return m_szSIM_PM_Data[idx].Get_RunMsg();
}

int IPS_SIM_DATA_Controller::Get_SimData_Type( int idx )
{
	return m_szSIM_PM_Data[idx].Get_Type();
}

char* IPS_SIM_DATA_Controller::Get_SimData_GrpName( int idx )
{
	return m_szSIM_PM_Data[idx].Get_GrpName();
}