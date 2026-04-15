#ifndef SimpleECU_h_
#define SimpleECU_h_
#ifndef SimpleECU_COMMON_INCLUDES_
#define SimpleECU_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "math.h"
#endif
#include "SimpleECU_types.h"
#include "rtw_modelmap_simtarget.h"
#include "rt_nonfinite.h"
#include <string.h>
#include <stddef.h>
struct odvzwogg5bn_ { real_T P_1 ; real_T P_2 [ 2 ] ; real_T P_3 [ 2 ] ;
real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ; real_T P_8 ; real_T P_9 ;
real_T P_10 ; real_T P_11 ; real_T P_12 ; real_T P_13 ; real_T P_14 ; real_T
P_15 ; real_T P_16 ; real_T P_17 ; real_T P_18 ; real_T P_19 ; } ; struct
oat5daxizt { struct SimStruct_tag * _mdlRefSfcnS ; struct {
rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; sysRanDType * systemRan [ 5 ] ; int_T systemTid [ 5 ] ;
} DataMapInfo ; struct { int_T mdlref_GlobalTID [ 1 ] ; } Timing ; } ;
typedef struct { inrj4312kz rtm ; } gdwzb1w12tj ; extern real_T
rtP__CtrlEcuRevLim2f2SiEngineController_sldd_ ; extern void ls4iwfjhmk ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , inrj4312kz * const e44s5qfgut , void * sysRanPtr , int contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void mr_SimpleECU_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray * mr_SimpleECU_GetDWork ( const gdwzb1w12tj * mdlrefDW ) ; extern void mr_SimpleECU_SetDWork ( gdwzb1w12tj * mdlrefDW , const mxArray * ssDW ) ; extern void mr_SimpleECU_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_SimpleECU_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * SimpleECU_GetCAPIStaticMap ( void ) ; extern void hpdbk3e53r ( real_T * h0uquagyg0 , real_T * m1h2iw23s1 , real_T * hjbjee1qf3 , real_T * p1qwr4kg4w , real_T * djdsbxjjcs , real_T * aakgidicwv , real_T * m0ncuwdtjo , real_T * jcvlomlnhi , real_T * l45dkyhzbm , real_T * hvpmgazb03 , real_T * njjcdejhvz , real_T * j35keouu1s , real_T * jn0muy2z31 , real_T * j4qz2y4at2 , real_T * anrpwz2hpc , real_T * eac1s300pn ) ; extern void fgxsnqfu34 ( real_T * h0uquagyg0 , real_T * m1h2iw23s1 , real_T * hjbjee1qf3 , real_T * p1qwr4kg4w , real_T * djdsbxjjcs , real_T * aakgidicwv , real_T * m0ncuwdtjo , real_T * jcvlomlnhi , real_T * l45dkyhzbm , real_T * hvpmgazb03 , real_T * njjcdejhvz , real_T * j35keouu1s , real_T * jn0muy2z31 , real_T * j4qz2y4at2 , real_T * anrpwz2hpc , real_T * eac1s300pn ) ; extern void SimpleECU ( const real_T * ipvvddyov3 , const real_T * av2xyxaxr3 , const boolean_T * dewtouimsp , real_T * h0uquagyg0 , real_T * m1h2iw23s1 , real_T * hjbjee1qf3 , real_T * p1qwr4kg4w , real_T * djdsbxjjcs , real_T * aakgidicwv , real_T * m0ncuwdtjo , real_T * jcvlomlnhi , real_T * l45dkyhzbm , real_T * hvpmgazb03 , real_T * njjcdejhvz , real_T * j35keouu1s , real_T * jn0muy2z31 , real_T * j4qz2y4at2 , real_T * ouv21new2s , real_T * anrpwz2hpc , real_T * eac1s300pn ) ; extern void npdtakcntc ( inrj4312kz * const e44s5qfgut ) ;
#endif
