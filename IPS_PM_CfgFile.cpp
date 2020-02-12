#include <stdio.h>
#include <Windows.h>

#include "KSeqUtil.h"
#include "Parameter.h"

#include "IPS_PM_CfgFile.h"
#include "IPS_SIM_DATA_Controller.h"


extern IPS_SIM_DATA_Controller*	gpDataController;

extern KStr*	pStr;

IPS_PM_CfgFile::IPS_PM_CfgFile( int iPortIdx, char* pszFile )
	: KFile_Ini( pszFile )
{
	m_PortIdx = iPortIdx;
}


IPS_PM_CfgFile::~IPS_PM_CfgFile(void)
{
}


bool IPS_PM_CfgFile::ReadData( char* pszFileName )
{
	int		iCnt = 0, iMdlNo = 0, chidx;
	int		i, j, k , l , iStart = -1, iEnd = -1, iDelay = 0;
	DWORD	dwDelay = 0;
	CHAMBER	ch;
	CH_ACT	iCHAction = eCh_UNK;
	BOOL	bResult = FALSE;

	char	szGrp[64]   = { 0, };
	char	szItem[64]  = { 0, };
	char	szData[512] = { 0, };
	char	szActionStr[4][256] = { 0, };

	Set_Object( pszFileName );
	gpDataController->PM_Init( m_PortIdx );

	for ( i = 1 ; i <= MAX_PM_MDL_NO ; i++ ) {
		sprintf( szGrp, "PM%d", i );
 		ch = (CHAMBER) (ePM1 + ( i - 1 ) );

		// Slot 정보는 여기서 읽어야...............................
		Read( szGrp, "START_SLOT", &iStart );
		Read( szGrp, "END_SLOT", &iEnd );

		for ( j = 1 ; j <= MAX_PM_ACTION ; j++ ) {
			sprintf( szItem, "OP%d", j );
			memset( szData, 0, sizeof( char ) * 512 );

			//-------------------------------------------------------------------------------------------------------------
			iCHAction = eCh_UNK;
			//iStart = -1;	iEnd = -1;		
			iDelay = 0;
			//-------------------------------------------------------------------------------------------------------------

			for ( k = 0 ; k < 4 ; k++ ) {
				memset( szActionStr[k], 0, sizeof( char ) * 256 );
			}
			
			if ( Read( szGrp, szItem, szData, 512 ) ) {
				chidx=-1;
				//---------------------------------------------------------------------------------------------------------
				if ( pStr->Str_Has_Char( szData, ',' ) < 0 )	continue;
				while( true ) {
					chidx = pStr->Str_Has_Char( szData, '\t' );
					if( chidx < 0 ) break; 
					for( l = chidx ; i < sizeof(szData) ; l++ ) {
							szData[l] = szData[l+1];
							if(szData[l+1]==NULL) break;
					}
				}


				//---------------------------------------------------------------------------------------------------------
				//if ( pStr->Str_Extract_With_Seperator( szData, ',',  1, &iStart )				<= 0 )		continue;
				//if ( pStr->Str_Extract_With_Seperator( szData, ',',  2, &iEnd )					<= 0 )		continue;
				//if ( pStr->Str_Extract_With_Seperator( szData, ',',  3, &iDelay )				<= 0 )		continue;
				//if ( pStr->Str_Extract_With_Seperator( szData, ',',  4, szActionStr[0], 256 )	<= 0 )		continue;

				if ( pStr->Str_Extract_With_Seperator( szData, ',',  1, &iDelay )				<= 0 )		continue;
				if ( pStr->Str_Extract_With_Seperator( szData, ',',  2, szActionStr[0], 256 )	<= 0 )		continue;

				//---------------------------------------------------------------------------------------------------------
				if ( pStr->Str_Extract_With_Seperator( szActionStr[0], ':',  1, szItem, 64 )	<= 0 )		continue;

				if		( strncmp( szItem, "DISABLE", strlen( "DISABLE" ) ) == 0 ) {
					if ( strcmp( szItem, "DISABLE" ) == 0 )	{
						iCnt++;
						iCHAction = eCh_Disable;
					}
					else {
						iMdlNo = pStr->Check_Integer_And_Return( &szItem[7], 64 - 7, &bResult );
						if ( bResult == FALSE )		return false;

						iCnt++;
						iCHAction = (CH_ACT) (eCh_Disable_PM1 + ( iMdlNo - 1 ));
					}

					gpDataController->PM_Append_Data( m_PortIdx, iStart, iEnd, ch, iDelay * 1000, iCHAction, szGrp, "", "", "", "", ""  );
				}
				else if ( strncmp( szItem, "ENABLE", strlen( "ENABLE" )) == 0 ) {
					if ( strcmp( szItem, "ENABLE" ) == 0 )	{
						iCnt++;
						iCHAction = eCh_Enable;
					}
					else {
						iMdlNo = pStr->Check_Integer_And_Return( &szItem[6], 64 - 6, &bResult );
						if ( bResult == FALSE )		return false;

						iCnt++;
						iCHAction = (CH_ACT) (eCh_Enable_PM1 + ( iMdlNo - 1 ));
					}

					gpDataController->PM_Append_Data( m_PortIdx, iStart, iEnd, ch, iDelay * 1000, iCHAction, szGrp, "", "", "", "", ""  );
				}
				else if ( strcmp( szItem, "EVENT"	) == 0 ) {
					iCnt++;
					iCHAction = eFC_EVNT;
					if ( pStr->Str_Extract_With_Seperator( szActionStr[0], ':',  2, szActionStr[1], 64 )	<= 0 )		continue;
					if ( pStr->Str_Extract_With_Seperator( szActionStr[0], ':',  3, szActionStr[2], 64 )	<= 0 )		continue;

					gpDataController->PM_Append_Data( m_PortIdx, iStart, iEnd, ch, iDelay * 1000, iCHAction, szGrp, szActionStr[1], szActionStr[2], "", "", ""  );
				}
				else if ( strcmp( szItem, "RUN"		) == 0 ) {
					iCnt++;
					iCHAction = eFC_RUN;
					if ( pStr->Str_Extract_With_Seperator( szActionStr[0], ':',  2, szActionStr[1], 64 )	<= 0 )		continue;
					if ( pStr->Str_Extract_With_Seperator( szActionStr[0], ':',  3, szActionStr[2], 64 )	<= 0 )		continue;
					gpDataController->PM_Append_Data( m_PortIdx, iStart, iEnd, ch, iDelay * 1000, iCHAction, szGrp, szActionStr[1], szActionStr[2], "", "", ""  );
				}
				else if ( strcmp( szItem, "IO_SET"	) == 0 ) {
					iCnt++;
					iCHAction = eIO_SET;
					if ( pStr->Str_Extract_With_Seperator( szActionStr[0], ':',  2, szActionStr[1], 64 )	<= 0 )		continue;
					if ( pStr->Str_Extract_With_Seperator( szActionStr[0], ':',  3, szActionStr[2], 64 )	<= 0 )		continue;
					if ( pStr->Str_Extract_With_Seperator( szActionStr[0], ':',  4, szActionStr[3], 64 )	<= 0 )		continue;
					gpDataController->PM_Append_Data( m_PortIdx, iStart, iEnd, ch, iDelay * 1000, iCHAction, szGrp, szActionStr[1], szActionStr[2], szActionStr[3], "", ""  );
				}
				
			}
		}
	}

	if ( iCnt > 0 )		return true;
	return false;
}


void IPS_PM_CfgFile::Set_Port_Index( int idx )
{
	m_PortIdx = idx;
}


int IPS_PM_CfgFile::Get_Port_index()
{
	return m_PortIdx;
}
