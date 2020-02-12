#pragma once

#include "Parameter.h"

#include "IPS_SIM_DATA.h"
#include "IPS_SIM_PM_DATA.h"
#include "IPS_PM_CfgFile.h"
#include "IPS_JOB_CfgFile.h"

class IPS_SIM_DATA_Controller
{
	private:
		IPS_SIM_DATA*		m_szSIMData;
		IPS_PM_CfgFile*		m_pReadIni;
		unsigned int		m_iMaxSize;
		int					m_iDataCnt;
		int*				m_piRunPointer;

	public:
		IPS_SIM_PM_DATA*	m_szSIM_PM_Data;


		IPS_SIM_DATA_Controller(unsigned int iMaxSize);												// 최대 Item 갯수
		~IPS_SIM_DATA_Controller(void);

		void Set_Max_Size( unsigned int iSize );
		unsigned int Get_Max_Size();
		int	Get_Data_Count();
		void Set_Data_Count( int iCnt );
		void Init ( void );
		void PM_Init( int iPort_idx );
		bool Append( int ino, char* pszRecipe, int iStart, int iend, char* szConfig );
		bool Append( int ino, char* pszJobFile, char* pszConfig );

		bool GetSimData( int ino, int* piReg, char* pszRecipe, int* piStart, int* piEnd, char* pszConfigData, char* pszJobData );		// ino == -1이면 UnExecuted Data를 찾아서 리턴한다.

		bool Read_PM_Test_Condition( int iPort_idx, char* pszCondFile );
		bool PM_Append_Data( int iPort_idx, int iStart, int iEnd, CHAMBER iChamber, DWORD dwDelayTime, CH_ACT iChAction, char* GrpName, char* szActParam1, char* szActParam2, char* szActParam3, char* szActParam4, char* szActParam5  );
		int  Get_PM_Data_Index( int iPort_Idx, CHAMBER iChamber );
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------
		int Get_SimData_Status( int idx );
		void Set_SimData_Status( int idx, CH_ACT_STS ists );
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------
		int Get_SimData_Start_Slot( int idx );
		int Get_SimData_End_Slot( int idx );
		char* Get_SimData_GrpName ( int idx );
		CHAMBER Get_SimData_Chamber( int idx );
		DWORD Get_SimData_DelayTime( int idx );
		CH_ACT Get_SimData_Chamber_Action( int idx );

		char* Get_SimData_TargetName( int idx );
		char* Get_SimData_RunMsg( int idx );
		int Get_SimData_Type( int idx );
};
