#include <stdio.h>
#include <Windows.h>

#include "Parameter.h"

#include "KSeqUtil.h"
#include "KFile.h"
#include "IPS_SIM_File.h"
#include "IPS_SIM_DATA_Controller.h"
#include "Sequence.h"

KStr*	pStr  = NULL;

extern IPS_SIM_DATA_Controller*	gpDataController;


IPS_SIM_File::IPS_SIM_File(char* pszFile, unsigned int uiMaxLine)
	: KFile( pszFile, uiMaxLine )
{
	pStr = new KStr();
	m_pkFile = new kUtlFile();
}

IPS_SIM_File::~IPS_SIM_File(void)
{
	delete pStr;
	delete m_pkFile;
}

bool IPS_SIM_File::Readline( char* pszline )
{
	char	szRecipe[100] = { 0, };
	char	szConfig[MAX_PATH] = { 0, };
	char	szDefPath[_MAX_DIR] = { 0, };
	int		ino, iStartSlot = 0 ;
	int		iEndSlot = 0;

	char	szJobPath[_MAX_DIR] = { 0, };	// 2019.05.26
	char	szJobConfig[MAX_PATH] = { 0, };	// 2019.05.26


	pStr->Char_Find_and_Replace( pszline, '\n', 0 );

	if ( strlen( pszline ) == 0 )		return true;

	if ( pStr->Str_Extract_With_Seperator( pszline, '|', 1, &ino				 )  == 0 )	return true;
	if ( pStr->Str_Extract_With_Seperator( pszline, '|', 2, szRecipe, 99		 )	== 0 )	return true;
	//if ( pStr->Str_Extract_With_Seperator( pszline, '|', 3, &iStartSlot			 )	== 0 )	return false;			// 2019.05.26
	//if ( pStr->Str_Extract_With_Seperator( pszline, '|', 4, &iEndSlot			 )	== 0 )	return false;
	//if ( pStr->Str_Extract_With_Seperator( pszline, '|', 5, szDefPath, _MAX_DIR-1 )	== 0 )	return false;
	//if ( gpDataController != NULL ) {
	//	m_pkFile->kaRelPath_To_FullPath( szDefPath, szConfig, MAX_PATH );
	//	gpDataController->Append( ino, szRecipe, iStartSlot, iEndSlot, szConfig );
	//}
	//else {
	//	return false;
	//}


	// 2019.05.26 Job기반을 위한 확장
	if ( _stricmp( szRecipe, "<JOB>" ) == 0 ) {		// 2019.05.26
		if ( pStr->Str_Extract_With_Seperator( pszline, '|', 3, szJobPath, _MAX_DIR-1 )	== 0 )	return false;		// RunFlow List File
		if ( pStr->Str_Extract_With_Seperator( pszline, '|', 4, szDefPath, _MAX_DIR-1 )	== 0 )	return false;		// RunFlow List File


		if ( gpDataController != NULL ) {
			m_pkFile->kaRelPath_To_FullPath( szJobPath, szJobConfig, MAX_PATH );
			m_pkFile->kaRelPath_To_FullPath( szDefPath, szConfig, MAX_PATH );
			gpDataController->Append( ino, szJobConfig, szConfig );
		}
		else {
			return false;
		}


	}
	else {
		if ( pStr->Str_Extract_With_Seperator( pszline, '|', 3, &iStartSlot			 )	== 0 )	return false;
		if ( pStr->Str_Extract_With_Seperator( pszline, '|', 4, &iEndSlot			 )	== 0 )	return false;
		if ( pStr->Str_Extract_With_Seperator( pszline, '|', 5, szDefPath, _MAX_DIR-1 )	== 0 )	return false;
		if ( gpDataController != NULL ) {
			m_pkFile->kaRelPath_To_FullPath( szDefPath, szConfig, MAX_PATH );
			gpDataController->Append( ino, szRecipe, iStartSlot, iEndSlot, szConfig );
		}
		else {
			return false;
		}
	}


	return true;
}