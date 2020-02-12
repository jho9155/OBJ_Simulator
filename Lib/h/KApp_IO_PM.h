#pragma once
#include "KIO_Digital.h"

class KApp_IO_PM
{
	private:
		KIO_Digital* m_pIOStatus;
		KIO_Digital* m_pIOSource;
		KIO_Digital* m_pIOResult;

	public:
		KApp_IO_PM(void);
		~KApp_IO_PM(void);

		void Set_Make_PM_Info(int iPM_Num, int iSlotNumber);

		int Get_Source(int iSlotNo);
		int Get_Status(int iSlotNo);
		int Get_Result(int iSlotNo);
		
		int	Get_Port(int iSlotNo);
		int Get_Slot_No(int iSlotNo);

		void Set_Source(int iSlotNo, int iValue);
		void Set_Status(int iSlotNo, int iValue);
		void Set_Result(int iSlotNo, int iValue);
};

