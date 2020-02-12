#pragma once
class KApp_Thread
{
	private:
		int m_RunStatus;
		int m_prevSts;
		static void ThreadHandler(void* lpParam);

	protected:
		int	m_iPortIdx;
	
	public:
		KApp_Thread(void);
		~KApp_Thread(void);

		void Set_Port_Idx( int idx );
		int  Get_Port_Idx();

		void Pause();
		void Resume();
		void Stop();

		bool Run();
		bool Run_Once();

		int Monitor();

		int	 Get_Thread_Status();

		virtual bool Task();
};

