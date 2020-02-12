#include <stdio.h>
#include <Windows.h>

#include "Parameter.h"
#include "KSeqUtil.h"

#include "IPS_JOB_CfgFile.h"

extern KStr*	pStr;

IPS_JOB_CfgFile::IPS_JOB_CfgFile(int iPortIdx, char* pszFile )
	: KFile_Ini( pszFile )
{
	m_PortIdx = iPortIdx;
}

IPS_JOB_CfgFile::~IPS_JOB_CfgFile(void)
{
}

int IPS_JOB_CfgFile::Get_ControlJob_Name( char* pszRetCJName, int iMaxStrSize )
{
	if ( pszRetCJName == NULL )			return 0;

	Read( "CONTROLJOB", "NAME", pszRetCJName, iMaxStrSize );
	return strlen( pszRetCJName );
}


JOB_START_MODE IPS_JOB_CfgFile::Get_Control_Job_StartMode()
{
	char	szReadBuf[32] = { 0, };
	if ( Read( "CONTROLJOB", "StartMode", szReadBuf, 31 ) ) {
		if ( _stricmp( szReadBuf, "MANUAL" ) == 0 )		return eSTART_MANUAL ;
	}

	return eSTART_AUTO;
}


int IPS_JOB_CfgFile::Get_ProcessJob_Number(void)
{
	int		i, iNumberOfPj = -1;
	char	szBuf[MAX_MULTI_STRING_SIZE + 1 ] = { 0, };

	if ( Read( "CONTROLJOB", "PJOB_NUM", &iNumberOfPj ) ) {
	}
	else {
		iNumberOfPj = 0;
		for (  i = 0 ; i < MAX_MULTI_CTJOB_CASSETTE ; i++ ) {
			if ( Get_ProcessJob_Name( i, szBuf, MAX_MULTI_STRING_SIZE ) > 0 )	iNumberOfPj++;
			else										break;
		}
	}

	return iNumberOfPj; 
}


int IPS_JOB_CfgFile::Get_ProcessJob_Name(int index, char* pszRetPJName, int iMaxStrSize )
{
	char	szGroupName[128] = { 0, };

	sprintf( szGroupName, "PROCESSJOB%d", index+1 );
	if ( pszRetPJName == NULL )		return 0;

	if ( Read( szGroupName, "NAME", pszRetPJName, iMaxStrSize ) == FALSE ) {
		return 0;
	}

	return strlen( pszRetPJName );
}


JOB_START_MODE IPS_JOB_CfgFile::Get_Process_Job_StartMode(int iProcessJob_Index)							// Default Auto Start Mode
{
	char	szGroupName[32] = {0 , };
	char	szReadBuf[32] = { 0, };

	sprintf( szGroupName, "PROCESSJOB%d", iProcessJob_Index + 1 );

	if ( Read( szGroupName, "StartMode", szReadBuf, 31 ) ) {
		if ( _stricmp( szReadBuf, "MANUAL" ) == 0 )		return eSTART_MANUAL ;
	}

	return eSTART_AUTO;
}


int IPS_JOB_CfgFile::Get_ProcessJob_Recipe(int iProcessJob_Index, char* pszRecipeName, int iMaxStrSize)
{
	char	szGroupName[32] = {0 , };

	sprintf( szGroupName, "PROCESSJOB%d", iProcessJob_Index + 1 );

	if ( Read( szGroupName, "Recipe", pszRecipeName, iMaxStrSize ) ) {
		return strlen( pszRecipeName );
	}

	return 0;
}


//int IPS_JOB_CfgFile::Get_ProcessJob_Use_Cassette_Number(int iProcessJob_Index)
//{
//	char	szGroupName[32] = {0 , };
//	int		inum = 0;
//
//	sprintf( szGroupName, "PROCESSJOB%d", iProcessJob_Index + 1 );
//
//	if ( Read( szGroupName, "CassNo", &inum ) ) {
//		return inum;
//	}
//
//	return 0;
//}

int	IPS_JOB_CfgFile::Get_ProcessJob_Slot_String(int iProcessJob_Index, int iCassette_Index, char* pszSlotString, int iMaxStrSize)
{
	char	szGroupName[32] = {0 , };
	char	szSlotStr[64 + 1] = {0, };

	if ( pszSlotString == NULL )		return 0;

	sprintf( szGroupName, "PROCESSJOB%d", iProcessJob_Index + 1 );
	sprintf( szSlotStr, "Cass%d_SLOT", iCassette_Index + 1 );

	if ( Read( szGroupName, szSlotStr, pszSlotString, iMaxStrSize ) ) {
		return strlen( pszSlotString );
	}

	return 0;
}


int IPS_JOB_CfgFile::Get_ProcessJob_Slot_Info( int iProcessJob_Index, int* piWfrList )
{
	int		i, iStart, iEnd, iCnt = 0;
	char	szWfrInfoStr[64 + 1] = { 0, };

	if ( Get_ProcessJob_Slot_String( iProcessJob_Index, 0, szWfrInfoStr, 64 ) <= 0 )		return 0;
	if ( piWfrList == NULL )	return 0;

	if ( _stricmp( szWfrInfoStr, "ALL" ) == 0 ) {
		for ( i = 0 ; i < MAX_PORT_SLOT ; i++ ) {
			*(piWfrList+i) = 1;
			iCnt++;
		}

		return iCnt;
	}

	iStart = 0;
	iEnd = 0;

	if ( pStr->Str_Has_Char( szWfrInfoStr, '|' ) >= 0 ) {
		pStr->Str_Extract_With_Seperator( szWfrInfoStr, '|', 1, &iStart );
		pStr->Str_Extract_With_Seperator( szWfrInfoStr, '|', 2, &iEnd );

		for ( i = iStart - 1 ; i < iEnd  ; i++ ) {
			*(piWfrList+i) = 1;
			iCnt++;
		}

		return iCnt;
	}
	else if ( pStr->Str_Has_Char( szWfrInfoStr, ',' ) >= 0 ) {

		iEnd = pStr->Get_Char_Count_In_Str( szWfrInfoStr, ',', strlen( szWfrInfoStr ));

		for ( i = 0 ; i <= iEnd ; i++ ) {
			pStr->Str_Extract_With_Seperator( szWfrInfoStr, ',', i+1, &iStart );
			*(piWfrList+(iStart-1)) = 1;
			iCnt++;
		}

		return iCnt;
	}
	else if ( pStr->NumStr_Entirety_Check( szWfrInfoStr ) ) {
		iStart = atoi ( szWfrInfoStr );
		*(piWfrList+iStart-1) = 1;
		iCnt++;
		Sleep(10);
		return iCnt;
	}
	return 0;
}