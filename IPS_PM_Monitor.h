#pragma once

#include "KApp_Thread.h"
#include "KApp_IO_PM.h"
#include "IPS_SIM_PM_DATA.h"
#include "KSCH_Event.h"
#include "KIO_Digital.h"

class IPS_PM_Monitor : public KApp_Thread
{
	private :
		int	m_iPMidx;	
		//int	m_iCondIdx;

		int	m_iRunCnt;
		DWORD m_dwDelay;

		KApp_IO_PM*	m_pPM;
		bool* m_bChk_OK_PORT_Slot;

		KSCH_Event*			m_pSchEvnt;

		//ys
		KIO_Digital* m_pPM_Sts[MAX_PM_MDL_NO];
	

	public:
		//IPS_SIM_PM_DATA*	m_pdata;

		IPS_PM_Monitor();
		~IPS_PM_Monitor(void);

		void Set_PM_Idx( int idx );
		int  Get_PM_Idx();

		bool Task();
		
		bool Load_Check_Condition( int iPortIdx, int iWfrNo, int& iCondIdx );
		bool Get_Run_Info( int idx, DWORD& dwWaitTime, CH_ACT& ChAction, char* pszTargetName, char* pszRunMsg, int& iSimDataType );
		bool Execute_Action(int iPortIdx, int iRegNo, int index);
		void Set_Run_Status( int iPortIdx, CH_ACT_STS iSts );

		//bool Load_Check_Condition( int iPMIdx, int iCount );
		//bool Get_Check_Condition_Ok(CHAMBER ch);

		bool IsCheck_Slot( int iSlot );
		void Set_Check_Slot( int iSlot, bool bsts );

		bool DelayTime( DWORD dwTime, int iCondIdx );

		void Set_Last_Chamber_Disable_Changer( int iPortIdx, int iPM_Mdl_No, int iSts );
		int  Get_Last_Chamber_Disable_Changer( int iPM_Mdl );

		int	 Check_Enable_Disable_Event( char* szEvnt, int& irPM_Num );

		void End_Sequence(int iPortIdx);
};

