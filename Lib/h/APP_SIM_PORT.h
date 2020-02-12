#pragma once

#ifdef __cplusplus 
extern "C" { 
#endif 

#include "CimSeqnc.h"

#ifdef __cplusplus 
}
#endif 

#include "KIO_Digital.h"
#include "KIO_String.h"
#include "KIO_Function.h"


class APP_SIM_PORT
{
	private:
		KIO_Digital*	m_pIO_Foup;
		KIO_Digital*	m_pIO_Foup2;
		KIO_Digital*	m_pIO_LoadSignal;
		KIO_Digital*	m_pIO_UnLoadSignal;
		KIO_Digital*	m_pIO_LoadButton;
		KIO_Digital*	m_pIO_UnLoadButton;

		KIO_Digital*	m_pIO_Range_Start;
		KIO_Digital*	m_pIO_Range_End;

		KIO_String*		m_pIO_Recipe;

		KIO_Digital*	m_pIO_MainCtrl;

		KIO_Digital*	m_pIO_Status;

	public:
		APP_SIM_PORT( char* pszIO_Foup1, char* pszIO_Foup2, char* pszIO_LoadSignal, char* pszIO_UnloadSignal, char* pszIO_LdButton, char* pszIO_UldButton );
		~APP_SIM_PORT(void);

		bool Set_IO_Wafer_Range( char* pszStartIO, char* pszEndIO );
		bool Set_IO_Recipe( char* pszRecipeIO );
		bool Set_IO_MainControl( char* pszCtrlIO );
		bool Set_IO_MainStatus( char* pszStatusIO );					// m_pIO_Status;

		int Set_Carrier(int ists);
		int Get_Carrier();												// return ( sts2 * 10 ) + sts1;
		bool Set_Wafer_Range( int iStart, int iEnd );
		bool Set_Recipe( char* pszRecipe );

		int	 Get_Main_Control( bool* pbcs );
		int	 Get_Main_Status( bool* pbcs );								// m_pIO_Status;

};

