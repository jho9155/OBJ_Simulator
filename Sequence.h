#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>


#ifdef __cplusplus 
extern "C" { 
#endif 

#include "CimSeqnc.h"

#ifdef __cplusplus 
}
#endif 

#include "Parameter.h"

#include "KSeqUtil.h"
#include "APP_SIM_Port.h"

#include "KIO_Function.h"
#include "KIO_String.h"
#include "IPS_JOB_CfgFile.h"


class IPS_SIM_PORT : public APP_SIM_PORT 
{
	private :
		int				m_iPortIdx;
		int				m_iRegNo;						// 시뮬레이션의 Regist Number를 기억한다.


		KIO_Function*	m_pGUIStart_Func;
		KIO_Function*	m_pPMFunc[MAX_PM_MDL_NO];
		KIO_Function*	m_pPMIntf[MAX_PM_MDL_NO];
		KIO_Function*	m_pTMIntf[MAX_TM_MDL_NO];
		KIO_Function*	m_pFMIntf[MAX_FM_MDL_NO];
		KIO_Function*	m_pBMIntf[MAX_BM_MDL_NO];

		KIO_Function*	m_pPM_MAINT[MAX_PM_MDL_NO];
		KIO_Function*	m_pBM_MAINT[MAX_BM_MDL_NO];

		KIO_Function*	m_pSCH;

		KIO_Digital*	m_pAccessBtn;
		KIO_Digital*	m_pClampSts;
		KIO_String*		m_peSch;
		//bool			m_bPortRun[MAX_PORT];
		bool			m_bPortRun;
		bool			m_bSchFail;

		IPS_JOB_CfgFile*	m_pJobCfgFile;

		KIO_String*		m_pPortMID;


	public :
		IPS_SIM_PORT( int index, char* pszIO_Foup1, char* pszIO_Foup2, char* pszIO_LoadSignal, char* pszIO_UnloadSignal, char* pszIO_LdButton, char* pszIO_UldButton, char* pszGUIFunc );
		~IPS_SIM_PORT();

		int	Get_Port_Index();
		bool Get_Port_Run();
		void Set_Port_Run( bool ists );

		bool Get_Start_Wait();																		// Run Flag -> Run 시킬때 Simulation_Run에서 True로 만들고
																									//			   Port Monitor에서 Run이 완료되면 False로 전환시킨다.
		bool Get_End_Status();

		void Set_Reg_No(int ino);
		int	 Get_Reg_No();

		void Set_Scheduler_Fail_Status( bool bSchSts );
		bool Get_Scheduler_Fail_Status();

		Module_Status GUI_Start();																	// GUI Start를 시킨다.
		Module_Status JOB_Start( char* pszJobConf );												// 2019.05.26 - Job Start 방법


		Module_Status PORT_Load( int ists );
		Module_Status Wafer_Range( char* pszStart, char* pszEnd );
		Module_Status Recipe( char* szRecipe );

		Module_Status Monitor(void);
		Module_Status PM_Maint_Count_Zero(void);

		Module_Status End_BM_Pump();
		Module_Status End_Port_Error();			// 코딩해야할 것

		Module_Status Simulation_Run( char* pszRecipe, int iStart, int iEnd, char* pszJobConf, bool bEndWait );

		void Port_Abort( int iPortIdx );

		static void Run_Monitor( void* lpDummy );
		static void Port_Monitor( void* lport );
		bool Get_Port_MID(char* pszRetPortID);
};