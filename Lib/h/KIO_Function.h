#pragma once
class KIO_Function
{
	protected:
		int		m_ioPnt;
		char	m_pszName[128+1];

	public:
		KIO_Function(char* pszSeqName);
		KIO_Function(int iType, char* pszSeqName);

		int Redefine( int iotype, char* pszName);

		inline char* Get_Name() { return m_pszName; }
		inline int Get_Index() { return m_ioPnt; }

		Module_Status RUN( char* pszParam );
		BOOL RUN_SET( char* pszParam );
		Module_Status RUN_FREE( char* pszParam );
		void RUN_SET_FREE( char* pszParam );

		void WRITE_FUNCTION_EVENT( char* pszEvent );
		int READ_FUNCTION_EVENT();
		
		void READ_UPLOAD_MESSAGE( char* pszRetMsg );

		Module_Status READ_FUNCTION();
		void RUN_ABORT();

		Module_Status	Check_Wait();

		~KIO_Function(void);
};

