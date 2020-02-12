#pragma once
class KSCH_Event
{
	protected:
		int		m_ioSch;

	public:
		KSCH_Event(void);
		~KSCH_Event(void);
		//--------------------------------------------------------------------------------------------------------
		int	Find_Sch_Address();
		//--------------------------------------------------------------------------------------------------------
		void Send_Event_PM_Disable( int iPMNo );			//	iPMNo = 3 -> PM3번의 Disable Event를 송부한다.
		void Send_Event_PM_Enable( int iPMNo );				//	iPMNo = 3 -> PM3번의 Enable Event를 송부한다.
		//--------------------------------------------------------------------------------------------------------
		void Send_SCH_Event(char* pszMsg);						
		//--------------------------------------------------------------------------------------------------------
};

