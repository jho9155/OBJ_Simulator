#pragma once


class IPS_SIM_PM_DATA
{
	private:
		int				m_iWafer_Start;
		int				m_iWafer_End;

		CHAMBER			m_iPM;

		unsigned long	m_dwDelayTime;
		CH_ACT			m_iCh_Action;
		CH_ACT_STS		m_iChSts;

		char			m_szTargetName[128];
		int				m_iType;
		char			m_szRunMsg[512];
		char			m_szGrpName[5];
	public:
		IPS_SIM_PM_DATA(void);
		~IPS_SIM_PM_DATA(void);

		void Set_TargetName( char* pszName );
		char* Get_TargetName();
		void Set_Type( int iType );
		int Get_Type();
		void Set_RunMsg( char* pszRunMsg );
		char* Get_RunMsg();
		void Set_GrpName( char* pszGrpName );
		char* Get_GrpName();

		void Set_Wafer_Range( int iStart, int iEnd );
		int Get_Start_Wafer();								// 지정되지 않은 경우 -1을 리턴한다.
		int Get_End_Wafer();								// 지정되지 않은 경우 -1을 리턴한다.
		void Set_Delay_Time( unsigned long dwDelay_mSec );	// Delay 시간을 지정한다.	// mSec 단위로 지정한다.
		void Set_Delay_Time( int dwDelay_Sec );				// Delay 시간을 지정한다.	// Sec 단위로 지정한다.
		unsigned long Get_Delay_Time();						// Delay 시간을 리턴한다.
		CH_ACT Get_Chamber_Action();
		void Set_Chamber_Action( CH_ACT iAction );

		void Set_Chamber( CHAMBER ich );					// 동작 Chamber를 Setting 한다.
		CHAMBER Get_Chamber();								// 동작 Chamber를 Return 한다.

		void Set_Status( CH_ACT_STS iSts );					// 진행이 되었는지를 표기한다.
		CH_ACT_STS Get_Status();							// 진행이 되었는지 값을 Return 한다.
};

