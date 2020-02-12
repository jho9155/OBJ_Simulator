#ifndef __PARAMETER__
#define __PARAMETER__

#define MAX_PM_MDL_NO		2
#define MAX_TM_MDL_NO		2
#define MAX_FM_MDL_NO		1
#define MAX_BM_MDL_NO		2


#define	MAX_PORT			3

#define	MAX_SIM_LINE		512

#define MAX_PM_ACTION		50

#define MAX_PORT_SLOT		25

#define	JOB_BASE_RUN_CODE	10000

typedef enum { eCh_UNK, eCh_Disable, eCh_Enable, eFC_EVNT, eFC_RUN, eIO_SET, eCh_Disable_PM1 = 100, eCh_Enable_PM1 = 200 } CH_ACT;
typedef enum { ePM_ALL = -1, ePM1 = 1, ePM2, ePM3, ePM4, ePM5, ePM6, ePM7, ePM8, ePM9, ePM10, ePM11, ePM12, ePM13, ePM14, ePM15, ePM16, ePM17, ePM18, ePM19, ePM20, ePM21, ePM22, ePM23, ePM24, ePM25, ePM26, ePM27, ePM28, ePM29, ePM30 } CHAMBER;
typedef enum { ePM_NONE, ePM_REG, ePM_RUN, ePM_FINISH } CH_ACT_STS;

typedef enum { eCH_SET_DISABLE, eCH_SET_ENABLE } eCH_SET_STS;

enum { Idle , Running , Pausing , Paused , Aborting , Disable , Waiting , WaitHandOff };

#endif

