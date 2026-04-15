#ifndef TransmissionController_h_
#define TransmissionController_h_
#ifndef TransmissionController_COMMON_INCLUDES_
#define TransmissionController_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "math.h"
#include "sf_runtime/sfc_sdi.h"
#endif
#include "TransmissionController_types.h"
#include "rtw_modelmap_simtarget.h"
#include "rt_nonfinite.h"
#include <string.h>
#include <stddef.h>
typedef struct { real_T ctbloljy0k ; } oromh3n2ow ; typedef struct { int8_T
czt2emm2gr ; boolean_T jdn5p4hqsj ; } o4d1nv4zcx ; typedef struct {
oromh3n2ow c1rvhddg4tu ; } esksn11ftz ; typedef struct { real_T drgyfxwvr2 ;
real_T icidstynwv ; uint32_T b5yfuhwx0t ; uint32_T av2wa1pd3n ; int8_T
fe5mmldjmq ; int8_T lyaq3qnmfw ; uint8_T dv1nobpvdc ; uint8_T a05tnifdjv ;
uint8_T oceatsapa5 ; o4d1nv4zcx c1rvhddg4tu ; } ikk0kw0e3s ; struct
g2mn4pykba_ { real_T P_0 ; real_T P_1 [ 2 ] ; real_T P_2 [ 2 ] ; real_T P_3 ;
real_T P_4 ; real_T P_5 ; real_T P_6 ; } ; struct luigmi4qrbs_ { real_T P_9 [
2 ] ; real_T P_10 [ 7 ] ; real_T P_11 [ 2 ] ; real_T P_12 [ 7 ] ; real_T P_13
; real_T P_14 [ 7 ] ; real_T P_15 [ 4 ] ; real_T P_16 ; real_T P_17 [ 7 ] ;
real_T P_18 [ 4 ] ; real_T P_19 ; real_T P_20 ; real_T P_21 ; uint32_T P_22 [
3 ] ; uint32_T P_23 [ 3 ] ; uint32_T P_24 [ 3 ] ; uint32_T P_25 [ 3 ] ;
g2mn4pykba c1rvhddg4tu ; } ; struct huwnotjefz { struct SimStruct_tag *
_mdlRefSfcnS ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ; sysRanDType *
systemRan [ 6 ] ; int_T systemTid [ 6 ] ; } DataMapInfo ; struct { int_T
mdlref_GlobalTID [ 2 ] ; } Timing ; } ; typedef struct { esksn11ftz rtb ;
ikk0kw0e3s rtdw ; prjutdmklf rtm ; } dosld0bzlxb ; extern real_T
rtP__CtrlTcuDnLdBpt2f2Drivetrain_sldd_ [ 4 ] ; extern real_T
rtP__CtrlTcuDnShSpd2f2Drivetrain_sldd_ [ 56 ] ; extern real_T
rtP__CtrlTcuInitGear2f2Drivetrain_sldd_ ; extern real_T
rtP__CtrlTcuLugSpd2f2Drivetrain_sldd_ ; extern real_T
rtP__CtrlTcuMinUpSpd2f2Drivetrain_sldd_ ; extern real_T
rtP__CtrlTcuTmDnSh2f2Drivetrain_sldd_ ; extern real_T
rtP__CtrlTcuTmUpSh2f2Drivetrain_sldd_ ; extern real_T
rtP__CtrlTcuUpLdBpt2f2Drivetrain_sldd_ [ 4 ] ; extern real_T
rtP__CtrlTcuUpShSpd2f2Drivetrain_sldd_ [ 56 ] ; extern void pffr22s23c ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , prjutdmklf * const msg2oexmhm , esksn11ftz * localB , ikk0kw0e3s * localDW , void * sysRanPtr , int contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void mr_TransmissionController_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray * mr_TransmissionController_GetDWork ( const dosld0bzlxb * mdlrefDW ) ; extern void mr_TransmissionController_SetDWork ( dosld0bzlxb * mdlrefDW , const mxArray * ssDW ) ; extern void mr_TransmissionController_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_TransmissionController_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * TransmissionController_GetCAPIStaticMap ( void ) ; extern void nsopsaho5o ( oromh3n2ow * localB , g2mn4pykba * localP ) ; extern void c1rvhddg4t ( real_T h5siubfvdv , real_T ibnrla4v0g , oromh3n2ow * localB , o4d1nv4zcx * localDW , g2mn4pykba * localP ) ; extern void bi3mvnnhxb ( real_T * bmkiryd24s , esksn11ftz * localB , ikk0kw0e3s * localDW ) ; extern void asexx13y03 ( real_T * bmkiryd24s , ikk0kw0e3s * localDW ) ; extern void TransmissionController ( const real_T * ivuhyrz123 , const real_T * njl0yujf4t , const real_T * prevjjccrx , const real_T * arho0tmwfk , const real_T * lqpwdnlioa , const boolean_T * hazcyglkvp , real_T * bmkiryd24s , esksn11ftz * localB , ikk0kw0e3s * localDW ) ; extern void g2lk0g2ith ( prjutdmklf * const msg2oexmhm ) ;
#endif
