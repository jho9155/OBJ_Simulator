#include <stdio.h>
#include <Windows.h>

#include "Parameter.h"

#include "IPS_SIM_PM_DATA.h"


IPS_SIM_PM_DATA::IPS_SIM_PM_DATA(void)
{
	m_iChSts = ePM_NONE;
}


IPS_SIM_PM_DATA::~IPS_SIM_PM_DATA(void)
{
}


void IPS_SIM_PM_DATA::Set_Wafer_Range( int iStart, int iEnd )
{
	if ( iStart > 0 )	m_iWafer_Start = iStart;
	if ( iEnd > 0 )		m_iWafer_End = iEnd;
}


int IPS_SIM_PM_DATA::Get_Start_Wafer()								// 지정되지 않은 경우 -1을 리턴한다.
{
	return m_iWafer_Start;
}


int IPS_SIM_PM_DATA::Get_End_Wafer()								// 지정되지 않은 경우 -1을 리턴한다.
{
	return m_iWafer_End;
}


void IPS_SIM_PM_DATA::Set_Delay_Time( unsigned long dwDelay_mSec )	// Delay 시간을 지정한다.	// mSec 단위로 지정한다.
{
	m_dwDelayTime = dwDelay_mSec;
}


void IPS_SIM_PM_DATA::Set_Delay_Time( int dwDelay_Sec )				// Delay 시간을 지정한다.	// Sec 단위로 지정한다.
{
	m_dwDelayTime = dwDelay_Sec * 1000;
}


unsigned long IPS_SIM_PM_DATA::Get_Delay_Time()						// Delay 시간을 리턴한다.
{
	return m_dwDelayTime;
}


CH_ACT IPS_SIM_PM_DATA::Get_Chamber_Action()
{
	return m_iCh_Action;
}


void IPS_SIM_PM_DATA::Set_Chamber_Action( CH_ACT iAction )
{
	m_iCh_Action = iAction;
}


void IPS_SIM_PM_DATA::Set_Status( CH_ACT_STS iStatus )
{
	m_iChSts = iStatus;
}


CH_ACT_STS IPS_SIM_PM_DATA::Get_Status()
{
	return m_iChSts;
}


void IPS_SIM_PM_DATA::Set_TargetName( char* pszName )
{
	strncpy( m_szTargetName, pszName, 128 );
}


char* IPS_SIM_PM_DATA::Get_TargetName()
{
	return m_szTargetName;
}


void IPS_SIM_PM_DATA::Set_Type( int iType )
{
	m_iType = iType;
}


int IPS_SIM_PM_DATA::Get_Type()
{
	return m_iType;
}


void IPS_SIM_PM_DATA::Set_RunMsg( char* pszRunMsg )
{
	strncpy( m_szRunMsg, pszRunMsg, 512 );
}


char* IPS_SIM_PM_DATA::Get_RunMsg()
{
	return m_szRunMsg;
}

void IPS_SIM_PM_DATA::Set_Chamber( CHAMBER ich )					// 동작 Chamber를 Setting 한다.
{
	m_iPM = ich;
}

CHAMBER IPS_SIM_PM_DATA::Get_Chamber()								// 동작 Chamber를 Return 한다.
{
	return m_iPM;
}

void IPS_SIM_PM_DATA::Set_GrpName( char* pszGrpName )
{
	strncpy( m_szGrpName , pszGrpName , 4 );
}

char* IPS_SIM_PM_DATA::Get_GrpName()
{
	return m_szGrpName;
}