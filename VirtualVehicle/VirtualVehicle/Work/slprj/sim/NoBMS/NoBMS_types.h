#ifndef NoBMS_types_h_
#define NoBMS_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_SOCBUS_
#define DEFINED_TYPEDEF_FOR_SOCBUS_
typedef struct { real32_T SOC_CC ; uint8_T sl_padding0 [ 4 ] ; } SOCBUS ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_BMSInfoBUS_
#define DEFINED_TYPEDEF_FOR_BMSInfoBUS_
typedef struct { boolean_T Faults ; uint8_T sl_padding0 [ 3 ] ; real32_T
BMSState ; SOCBUS SOC ; } BMSInfoBUS ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_CurrentLimitsBUS_
#define DEFINED_TYPEDEF_FOR_CurrentLimitsBUS_
typedef struct { real32_T DischargeCurrentLimit ; real32_T ChargeCurrentLimit
; } CurrentLimitsBUS ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_ContactorsCmdBUS_
#define DEFINED_TYPEDEF_FOR_ContactorsCmdBUS_
typedef struct { boolean_T PosContactorChgrCmd ; boolean_T
PreChargeRelayChgrCmd ; boolean_T NegContactorChgrCmd ; boolean_T
PosContactorInvtrCmd ; boolean_T PreChargeRelayInvtrCmd ; boolean_T
NegContactorInvtrCmd ; uint8_T sl_padding0 [ 2 ] ; } ContactorsCmdBUS ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_BMSCmdBUS_
#define DEFINED_TYPEDEF_FOR_BMSCmdBUS_
typedef struct { CurrentLimitsBUS CurrentLimitsBus ; real32_T
ChargeCurrentReq ; uint8_T sl_padding0 [ 4 ] ; ContactorsCmdBUS
ContactorsCmdBus ; boolean_T BalCmd [ 96 ] ; } BMSCmdBUS ;
#endif
#ifndef SS_UINT64
#define SS_UINT64 22
#endif
#ifndef SS_INT64
#define SS_INT64 23
#endif
typedef struct cyj2c0ijz0q_ cyj2c0ijz0q ; typedef struct ex0cf43ywr
dl05wgqvsd ;
#endif
