#include <stdio.h>
#include <Windows.h>

#include "Parameter.h"
#include "IPS_SIM_DATA.h"


IPS_SIM_DATA::IPS_SIM_DATA(void)
{
}


IPS_SIM_DATA::~IPS_SIM_DATA(void)
{
}


void IPS_SIM_DATA::Set_Data( int ino, char *pszRecipe, int iStart, int iEnd, char* pszConfigData )
{
	m_ino = ino;
	strncpy( m_szRecipe, pszRecipe, 99 );
	m_iStart = iStart;
	m_iEnd = iEnd;
	strncpy( m_szConfigData, pszConfigData, MAX_PATH -1 );
}


void IPS_SIM_DATA::Set_Data( int ino, char *pszRecipe, char* pszJobFile, char* pszConfigData )
{
	m_ino = ino;

	m_iStart = JOB_BASE_RUN_CODE;
	m_iEnd = JOB_BASE_RUN_CODE;

	strncpy( m_szRecipe, pszRecipe, 99 );
	strncpy( m_szConfigData, pszConfigData, MAX_PATH -1 );
	strncpy( m_szJobCfgData, pszJobFile, MAX_PATH -1 ); 
}


bool IPS_SIM_DATA::Get_Data( int* pino, char* pszRecipe, int* piStart, int* piEnd, char* pszConfigData, char* pszJobCfg )
{
	if ( strlen( m_szRecipe ) <= 0 )		return false;

	if ( pino != NULL )				*pino = m_ino;
	if ( pszRecipe != NULL )		strncpy( pszRecipe, m_szRecipe, 99 );
	if ( piStart != NULL )			*piStart = m_iStart;
	if ( piEnd != NULL )			*piEnd = m_iEnd;
	if ( pszConfigData != NULL )	strncpy( pszConfigData, m_szConfigData, MAX_PATH -1 );
	if ( pszJobCfg != NULL )		strncpy( pszJobCfg, m_szJobCfgData, MAX_PATH -1 );

	return true;
}


bool IPS_SIM_DATA::Get_Run_Flag()
{
	return m_bExecuted;
}


void IPS_SIM_DATA::Set_Run_Flag( bool bSts )
{
	m_bExecuted = bSts;
}
