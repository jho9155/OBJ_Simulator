#pragma once

#include "KFile_Ini.h"
#include "SchJobIntf.h"

typedef enum { eSTART_AUTO, eSTART_MANUAL } JOB_START_MODE;

class IPS_JOB_CfgFile : public KFile_Ini
{
	private:
		int		m_PortIdx;

	public:
		IPS_JOB_CfgFile(int iPortIdx, char* pszFile );
		~IPS_JOB_CfgFile(void);

		int Get_ControlJob_Name(char* pszRetCJName, int iMaxStrSize);
		JOB_START_MODE Get_Control_Job_StartMode();							// Default Auto Start Mode

		int Get_ProcessJob_Number(void);
		int Get_ProcessJob_Name(int index, char* pszRetPJName, int iMaxStrSize );
		JOB_START_MODE Get_Process_Job_StartMode(int iProcessJob_Index);	// Default Auto Start Mode
		int Get_ProcessJob_Recipe(int iProcessJob_Index, char* pszRecipeName, int iMaxStrSize);
		//int Get_ProcessJob_Use_Cassette_Number(int iProcessJob_Index);
		int	Get_ProcessJob_Slot_String(int iProcessJob_Index, int iCassette_Index, char* pszSlotString, int iMaxStrSize);

		int Get_ProcessJob_Slot_Info( int iProcessJob_Index, int* piWfrList );
};


