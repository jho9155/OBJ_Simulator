#pragma once
class IPS_SIM_DATA
{
	private :
		int		m_ino;
		char	m_szRecipe[100];
		int		m_iStart;
		int		m_iEnd;
		bool	m_bExecuted;
		char	m_szConfigData[MAX_PATH];
		char	m_szJobCfgData[MAX_PATH];

	public:
		IPS_SIM_DATA(void);
		~IPS_SIM_DATA(void);

		void Set_Data( int ino, char *pszRecipe, int iStart, int iEnd, char* pszConfigData );
		void Set_Data( int ino, char *pszRecipe, char* pszJobFile, char* pszConfigData );
		//bool Get_Data( int* pino, char* pszRecipe, int* piStart, int* piEnd, char* pszConfigData );
		bool Get_Data( int* pino, char* pszRecipe, int* piStart, int* piEnd, char* pszConfigData, char* pszJobCfg );
		bool Get_Run_Flag();
		void Set_Run_Flag( bool bSts );

};

