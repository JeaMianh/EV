#ifndef NoBMS_h_
#define NoBMS_h_
#ifndef NoBMS_COMMON_INCLUDES_
#define NoBMS_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "math.h"
#endif
#include "NoBMS_types.h"
#include "rtw_modelmap_simtarget.h"
#include "rt_nonfinite.h"
#include <string.h>
#include <stddef.h>
typedef struct { BMSCmdBUS gohzarthjb ; BMSInfoBUS ceiipaydbv ; } iuqn2musmd
; struct cyj2c0ijz0q_ { BMSCmdBUS P_0 ; BMSInfoBUS P_1 ; real32_T P_2 ;
real32_T P_3 ; real32_T P_4 ; } ; struct ex0cf43ywr { struct SimStruct_tag *
_mdlRefSfcnS ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ; sysRanDType *
systemRan [ 2 ] ; int_T systemTid [ 2 ] ; } DataMapInfo ; struct { int_T
mdlref_GlobalTID [ 1 ] ; } Timing ; } ; typedef struct { iuqn2musmd rtb ;
dl05wgqvsd rtm ; } bsoby43i3xf ; extern void pngtqbyjyy ( SimStruct *
_mdlRefSfcnS , int_T mdlref_TID0 , dl05wgqvsd * const cm1i5arzqn , iuqn2musmd
* localB , void * sysRanPtr , int contextTid , rtwCAPI_ModelMappingInfo *
rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T
rt_CSTATEIdx ) ; extern void mr_NoBMS_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS
, char_T * modelName , int_T * retVal ) ; extern mxArray * mr_NoBMS_GetDWork
( const bsoby43i3xf * mdlrefDW ) ; extern void mr_NoBMS_SetDWork ( bsoby43i3xf
* mdlrefDW , const mxArray * ssDW ) ; extern void
mr_NoBMS_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray *
mr_NoBMS_GetSimStateDisallowedBlocks ( ) ; extern const
rtwCAPI_ModelMappingStaticInfo * NoBMS_GetCAPIStaticMap ( void ) ; extern
void l0xwxaulm2 ( boolean_T * ozfwzyapxq , real32_T * lt12ozwx0q , real32_T *
iuc1spellu , real32_T * e3tcpigr5k , real32_T * asgk45uw01 , real32_T *
n20puvff3j , boolean_T * o11ohhv1xc , boolean_T * ezt3jugofr , boolean_T *
iowqirvtc0 , boolean_T * izexp3fmwe , boolean_T * bbyr14q0tt , boolean_T *
otp0dt44ve , boolean_T dv5dp2lq1o [ 96 ] , iuqn2musmd * localB ) ; extern
void plmdsubdlc ( real32_T * iuc1spellu , real32_T * e3tcpigr5k , real32_T *
asgk45uw01 , iuqn2musmd * localB ) ; extern void NoBMS ( boolean_T *
ozfwzyapxq , real32_T * lt12ozwx0q , real32_T * iuc1spellu , real32_T *
e3tcpigr5k , real32_T * asgk45uw01 , real32_T * n20puvff3j , boolean_T *
o11ohhv1xc , boolean_T * ezt3jugofr , boolean_T * iowqirvtc0 , boolean_T *
izexp3fmwe , boolean_T * bbyr14q0tt , boolean_T * otp0dt44ve , boolean_T
dv5dp2lq1o [ 96 ] , iuqn2musmd * localB ) ; extern void nq2oylqrrn ( dl05wgqvsd
* const cm1i5arzqn ) ; extern const BMSInfoBUS NoBMS_rtZBMSInfoBUS ; extern
const BMSCmdBUS NoBMS_rtZBMSCmdBUS ;
#endif
