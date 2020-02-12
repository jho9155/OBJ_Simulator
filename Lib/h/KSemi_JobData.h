#pragma once

#include "KIO.h"
#include "SchJobIntf.h"

#define MAX_WAFER_NUMBER	64

enum { eStartMode_Auto, eStartMode_Manual };

class KSemi_JobData : public KIO
{
	private :
		int		m_iPJ_StartMode;
		int		m_iCJ_StartMode;

		bool	m_bOpen;

		bool	m_bExtAPI;
		bool	m_bCreateMem;
		int		m_iExtCode;
		char	m_szJobMemName[MAX_MULTI_STRING_SIZE+1];

		int		m_iWaferInfoList[MAX_MULTI_CASS_SLOT_SIZE][MAX_WAFER_NUMBER];

		char	m_szRecipeName[MAX_MULTI_STRING_SIZE+1];
		
		char	m_szLotList[MAX_MULTI_CASS_SLOT_SIZE][MAX_MULTI_STRING_SIZE+1];
		char	m_szPJList_For_CJ[MAX_MULTI_CTJOB_CASSETTE][MAX_MULTI_STRING_SIZE+1];

	public:
		KSemi_JobData(void);
		KSemi_JobData(char* pszSchName, bool bExtApi, char* pszsysName, bool bCreateMem, int iExtCode_or_WaitTime);
		~KSemi_JobData(void);

		bool JobMem_Open();
		bool JobMem_Open( bool bExtApi, char* pszsysName, bool bCreateMem, int iExtCode_or_WaitTime );

		char* Get_Scheduler_Name();
		void Set_Scheduler_Name( char* pszSchName );

		void Clear_LotList_Info();
		bool Add_LotList_Info( char* pszLotName );
		bool Set_LotList_Info( int index, char* pszLotName );
		char* Get_LotList_Info( int index );
		void Delete_LotList_Info( int index );

		void Clear_Wafer_Info( int icass_idx );
		bool Add_Wafer_Info( int icass_idx, int iNumber );
		bool Set_Wafer_Info( int icass_idx, int idx, int iNumber );
		int  Get_Wafer_Info( int icass_idx, int idx );
		void Delete_Wafer_Info( int icass_idx, int idx );

		void Set_ProcessJob_StartMode( int iMode );
		int Get_ProcessJob_StartMode();

		void Set_ControlJob_StartMode( int iMode );
		int Get_ControlJob_StartMode();

		void Clear_ProcessJobList();
		int  Get_ProcessJobList_Number();
		void Set_ProcessJobLIst_ProcessJob( int idx, char* pszPJName);
		bool Add_ProcessJobLIst_ProcessJob( char* pszPJName );
		void Delete_ProcessJobLIst( int index );

		char* Get_Recipe();
		void Set_Recipe( char* pszRecipe );

		//--------------------------------------------------------------------------------------------------------
		bool Write_Make_Event( bool bControlJob );				// bControlJob : True  - Send control job make event  
																//               False - Send process job make event
		//--------------------------------------------------------------------------------------------------------
		bool Create_Process_Job( char* pszPjName,  char* pszLotName );
		bool Create_Control_Job( char* pszCjName,  char* pszLotName );
		//--------------------------------------------------------------------------------------------------------

		Module_Status Send_Mapping_Command( int iPortIdx );
		Module_Status Send_MapVerify_Command( int iPortIdx );

};

