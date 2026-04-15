#ifndef NoVCU_h_
#define NoVCU_h_
#ifndef NoVCU_COMMON_INCLUDES_
#define NoVCU_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "math.h"
#endif
#include "NoVCU_types.h"
#include "rtw_modelmap_simtarget.h"
#include "rt_nonfinite.h"
#include <string.h>
#include <stddef.h>
struct jreu3daqbnf_ { real_T P_0 ; real_T P_1 ; real_T P_2 ; real_T P_3 ;
real_T P_4 ; real_T P_5 ; } ; struct jhjsuetnyk { struct SimStruct_tag *
_mdlRefSfcnS ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ; sysRanDType *
systemRan [ 2 ] ; int_T systemTid [ 2 ] ; } DataMapInfo ; struct { int_T
mdlref_GlobalTID [ 1 ] ; } Timing ; } ; typedef struct { ihzswjibhm rtm ; }
iehsp021cqq ; extern void o0hfpjqo3f ( SimStruct * _mdlRefSfcnS , int_T
mdlref_TID0 , ihzswjibhm * const h3shmud0vz , void * sysRanPtr , int
contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T *
rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void
mr_NoVCU_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T
* retVal ) ; extern mxArray * mr_NoVCU_GetDWork ( const iehsp021cqq *
mdlrefDW ) ; extern void mr_NoVCU_SetDWork ( iehsp021cqq * mdlrefDW , const
mxArray * ssDW ) ; extern void mr_NoVCU_RegisterSimStateChecksum ( SimStruct
* S ) ; extern mxArray * mr_NoVCU_GetSimStateDisallowedBlocks ( ) ; extern
const rtwCAPI_ModelMappingStaticInfo * NoVCU_GetCAPIStaticMap ( void ) ;
extern void g3t213vzya ( real_T * jvcfozwdgt , real_T * dgdabdvuec , real_T *
az4stcu3cn , real_T * itze42kmuw , real_T * nrnef03skt ) ; extern void
n1nyrzxu5v ( real_T * jvcfozwdgt , real_T * dgdabdvuec , real_T * az4stcu3cn
, real_T * itze42kmuw , real_T * nrnef03skt ) ; extern void NoVCU ( const
real_T * aqghp3o52a , const real_T doa4j1mbo4 [ 4 ] , const real_T ioro3cuvdj
[ 6 ] , real_T * dvmswuccxa , real_T * jvcfozwdgt , real_T mrx5be2wgj [ 4 ] ,
real_T eyl0qjuqme [ 6 ] , real_T * dgdabdvuec , real_T * az4stcu3cn , real_T
* itze42kmuw , real_T * nrnef03skt ) ; extern void cfmi0pqnbo ( ihzswjibhm *
const h3shmud0vz ) ;
#endif
