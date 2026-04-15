#ifndef EvPowertrainControllerBMS_h_
#define EvPowertrainControllerBMS_h_
#ifndef EvPowertrainControllerBMS_COMMON_INCLUDES_
#define EvPowertrainControllerBMS_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif
#include "EvPowertrainControllerBMS_types.h"
#include "rtw_modelmap_simtarget.h"
#include <string.h>
#include <stddef.h>
typedef struct { real_T mf3c0f3rln ; real_T obiqgwz5nk ; } lsoeg0nito ;
struct c5hhynsofi4_ { real_T P_18 ; real_T P_19 ; real_T P_20 ; real_T P_21 ;
real_T P_22 ; real_T P_23 ; real_T P_24 ; real_T P_25 ; real_T P_26 ; real_T
P_27 ; real_T P_28 [ 14 ] ; real_T P_29 ; real_T P_30 ; real_T P_31 ; real_T
P_32 ; real_T P_33 [ 14 ] ; real_T P_34 ; real_T P_35 ; real_T P_36 ; real_T
P_37 ; real_T P_38 ; real_T P_39 [ 14 ] ; real_T P_40 ; real_T P_41 ; real_T
P_42 [ 14 ] ; real_T P_43 ; real_T P_44 ; real_T P_45 ; real_T P_46 ; real_T
P_47 ; real_T P_48 ; real_T P_49 ; real_T P_50 ; real_T P_51 ; real_T P_52 ;
uint32_T P_53 [ 2 ] ; } ; struct nfq1kx1roz { struct SimStruct_tag *
_mdlRefSfcnS ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ; sysRanDType *
systemRan [ 11 ] ; int_T systemTid [ 11 ] ; } DataMapInfo ; struct { int_T
mdlref_GlobalTID [ 2 ] ; } Timing ; } ; typedef struct { lsoeg0nito rtb ;
obv4iumo0b rtm ; } ngz2pfeakpt ; extern real_T
rtP__CtrlBcuTrlrBrkGain2f2PassVeh_sldd_ ; extern real_T
rtP__CtrlVcuEvChrgLmtBpt2f2EvVCU_sldd_ [ 11 ] ; extern real_T
rtP__CtrlVcuEvRegenBrkCutOff2f2EvVCU_sldd_ [ 2 ] ; extern real_T
rtP__CtrlVcuEvRegenBrkSpdBpt2f2EvVCU_sldd_ [ 2 ] ; extern real_T
rtP__CtrlVcuEvSocBpt2f2EvVCU_sldd_ [ 11 ] ; extern real_T
rtP__PlntBrkFrntBias2f2PassVeh_sldd_ ; extern real_T
rtP__PlntBrkKinFricCffFrnt2f2PassVeh_sldd_ ; extern real_T
rtP__PlntBrkKinFricCffRear2f2PassVeh_sldd_ ; extern real_T
rtP__PlntBrkNumPadsFrnt2f2PassVeh_sldd_ ; extern real_T
rtP__PlntBrkNumPadsRear2f2PassVeh_sldd_ ; extern real_T
rtP__PlntBrkPadMeanRadiusFrnt2f2PassVeh_sldd_ ; extern real_T
rtP__PlntBrkPadMeanRadiusRear2f2PassVeh_sldd_ ; extern real_T
rtP__PlntBrkPrsFctr2f2PassVeh_sldd_ ; extern real_T
rtP__PlntBrkRearBias2f2PassVeh_sldd_ ; extern real_T
rtP__PlntDiffrntlRatio2f2Drivetrain_sldd_ ; extern real_T
rtP__PlntEM1EffTbl2f2ElectricMachine1_sldd_ [ 154 ] ; extern real_T
rtP__PlntEM1Trq2f2ElectricMachine1_sldd_ [ 14 ] ; extern real_T
rtP__PlntEM1TrqEff2f2ElectricMachine1_sldd_ [ 11 ] ; extern void aqzgcfjqsf ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , obv4iumo0b * const ftj5mqhdtx , lsoeg0nito * localB , void * sysRanPtr , int contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void mr_EvPowertrainControllerBMS_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray * mr_EvPowertrainControllerBMS_GetDWork ( const ngz2pfeakpt * mdlrefDW ) ; extern void mr_EvPowertrainControllerBMS_SetDWork ( ngz2pfeakpt * mdlrefDW , const mxArray * ssDW ) ; extern void mr_EvPowertrainControllerBMS_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_EvPowertrainControllerBMS_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * EvPowertrainControllerBMS_GetCAPIStaticMap ( void ) ; extern void cdlti3213i ( real_T * i2aqjuli5s , real_T * f5q1lumvzg , real_T * j2naykddwm , real_T * ea4r4f3bwk , real_T * cewk4ryfod ) ; extern void f5tbg1pt2m ( real_T * i2aqjuli5s , real_T * f5q1lumvzg , real_T * j2naykddwm , real_T * ea4r4f3bwk , real_T * cewk4ryfod , lsoeg0nito * localB ) ; extern void EvPowertrainControllerBMS ( const real_T * ot1unth1hd , const real_T fju13otua5 [ 4 ] , const real_T * gam2gozpi4 , const real_T * givymazxjb , const real_T * k443icl0sn , const real_T * iodoakv4fh , const real_T * k0pcor1dt1 , const real_T * nirwbhd0cx , real_T * i2aqjuli5s , real_T * ou3e1khm3v , real_T gpmtm1hqzh [ 4 ] , real_T maa0jy4xlg [ 6 ] , real_T * f5q1lumvzg , real_T * j2naykddwm , real_T * ea4r4f3bwk , real_T * cewk4ryfod , lsoeg0nito * localB ) ; extern void EvPowertrainControllerBMSTID1 ( lsoeg0nito * localB ) ; extern void b3eofzzeql ( obv4iumo0b * const ftj5mqhdtx ) ;
#endif
