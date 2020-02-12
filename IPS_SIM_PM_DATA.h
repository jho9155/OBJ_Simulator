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
		int Get_Start_Wafer();								// �������� ���� ��� -1�� �����Ѵ�.
		int Get_End_Wafer();								// �������� ���� ��� -1�� �����Ѵ�.
		void Set_Delay_Time( unsigned long dwDelay_mSec );	// Delay �ð��� �����Ѵ�.	// mSec ������ �����Ѵ�.
		void Set_Delay_Time( int dwDelay_Sec );				// Delay �ð��� �����Ѵ�.	// Sec ������ �����Ѵ�.
		unsigned long Get_Delay_Time();						// Delay �ð��� �����Ѵ�.
		CH_ACT Get_Chamber_Action();
		void Set_Chamber_Action( CH_ACT iAction );

		void Set_Chamber( CHAMBER ich );					// ���� Chamber�� Setting �Ѵ�.
		CHAMBER Get_Chamber();								// ���� Chamber�� Return �Ѵ�.

		void Set_Status( CH_ACT_STS iSts );					// ������ �Ǿ������� ǥ���Ѵ�.
		CH_ACT_STS Get_Status();							// ������ �Ǿ����� ���� Return �Ѵ�.
};

