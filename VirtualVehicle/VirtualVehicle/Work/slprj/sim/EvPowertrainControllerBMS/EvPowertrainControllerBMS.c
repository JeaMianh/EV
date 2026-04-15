#include "EvPowertrainControllerBMS.h"
#include "rtwtypes.h"
#include "EvPowertrainControllerBMS_private.h"
#include <emmintrin.h>
#include "mwmathutil.h"
#include "EvPowertrainControllerBMS_capi.h"
#include "look1_binlcapw.h"
#include "look2_binlcapw.h"
static RegMdlInfo rtMdlInfo_EvPowertrainControllerBMS [ 40 ] = { {
"ngz2pfeakpt" , MDL_INFO_NAME_MDLREF_DWORK , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "gyajxgdclx" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "o3rn2hxtju" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "ctsaghyzop" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "g20grabgda" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "cvxbj1nfpu" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "blkvh0pnud" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "dczz10nv5n" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "ewhlpgmju5" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "mha1vmsaor" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "hveluuzkmw" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "elpuusdl0x" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "ab4o0w050u" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "lsoeg0nito" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "nfhm0izzpq" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "b3eofzzeql" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "nar4n4keem" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "cdlti3213i" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "aqzgcfjqsf" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "f5tbg1pt2m" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "a4hfskmsh4" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "EvPowertrainControllerBMS" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , 0 , ( NULL ) } , { "bhvhtz1xwv" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "c5hhynsofi4" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "nfq1kx1roz" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , { "obv4iumo0b" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"EvPowertrainControllerBMS" } , {
"mr_EvPowertrainControllerBMS_GetSimStateDisallowedBlocks" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "EvPowertrainControllerBMS"
} , { "mr_EvPowertrainControllerBMS_extractBitFieldFromCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "EvPowertrainControllerBMS"
} , { "mr_EvPowertrainControllerBMS_cacheBitFieldToCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "EvPowertrainControllerBMS"
} , { "mr_EvPowertrainControllerBMS_restoreDataFromMxArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "EvPowertrainControllerBMS"
} , { "mr_EvPowertrainControllerBMS_cacheDataToMxArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "EvPowertrainControllerBMS"
} , { "mr_EvPowertrainControllerBMS_extractBitFieldFromMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "EvPowertrainControllerBMS"
} , { "mr_EvPowertrainControllerBMS_cacheBitFieldToMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "EvPowertrainControllerBMS"
} , { "mr_EvPowertrainControllerBMS_restoreDataFromMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "EvPowertrainControllerBMS"
} , { "mr_EvPowertrainControllerBMS_cacheDataAsMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "EvPowertrainControllerBMS"
} , { "mr_EvPowertrainControllerBMS_RegisterSimStateChecksum" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "EvPowertrainControllerBMS"
} , { "mr_EvPowertrainControllerBMS_SetDWork" , MDL_INFO_ID_MODEL_FCN_NAME ,
0 , - 1 , ( void * ) "EvPowertrainControllerBMS" } , {
"mr_EvPowertrainControllerBMS_GetDWork" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , -
1 , ( void * ) "EvPowertrainControllerBMS" } , {
"EvPowertrainControllerBMS.h" , MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( NULL )
} , { "EvPowertrainControllerBMS.c" , MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( void
* ) "EvPowertrainControllerBMS" } } ; c5hhynsofi4 c5hhynsofi = { 0.0 , 2.0 ,
1.0 , 2.0 , - 1.0 , 1.0 , 0.0 , - 1.0 , 0.0 , 9.5492965855137211 , { 0.0 ,
1333.3333333333333 , 2666.6666666666665 , 3999.9999999999995 ,
4477.5590656519889 , 5333.333333333333 , 6666.6666666666661 ,
7999.9999999999991 , 9333.3333333333339 , 10666.666666666666 , 12000.0 ,
13333.333333333332 , 14666.666666666666 , 15999.999999999998 } , 0.01 , - 1.0
, - 1.0 , 9.5492965855137211 , { 0.0 , 1333.3333333333333 ,
2666.6666666666665 , 3999.9999999999995 , 4477.5590656519889 ,
5333.333333333333 , 6666.6666666666661 , 7999.9999999999991 ,
9333.3333333333339 , 10666.666666666666 , 12000.0 , 13333.333333333332 ,
14666.666666666666 , 15999.999999999998 } , - 1.0 , 0.0 , 0.0 , 0.0 ,
9.5492965855137211 , { 0.0 , 1333.3333333333333 , 2666.6666666666665 ,
3999.9999999999995 , 4477.5590656519889 , 5333.333333333333 ,
6666.6666666666661 , 7999.9999999999991 , 9333.3333333333339 ,
10666.666666666666 , 12000.0 , 13333.333333333332 , 14666.666666666666 ,
15999.999999999998 } , 100.0 , 9.5492965855137211 , { 0.0 ,
1333.3333333333333 , 2666.6666666666665 , 3999.9999999999995 ,
4477.5590656519889 , 5333.333333333333 , 6666.6666666666661 ,
7999.9999999999991 , 9333.3333333333339 , 10666.666666666666 , 12000.0 ,
13333.333333333332 , 14666.666666666666 , 15999.999999999998 } , INFINITY ,
0.0 , 1.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 1.0 , { 13U , 10U } } ; void
cdlti3213i ( real_T * i2aqjuli5s , real_T * f5q1lumvzg , real_T * j2naykddwm
, real_T * ea4r4f3bwk , real_T * cewk4ryfod ) { * i2aqjuli5s = c5hhynsofi .
P_37 ; * f5q1lumvzg = c5hhynsofi . P_47 ; * j2naykddwm = c5hhynsofi . P_48 ;
* ea4r4f3bwk = c5hhynsofi . P_49 ; * cewk4ryfod = c5hhynsofi . P_50 ; } void
f5tbg1pt2m ( real_T * i2aqjuli5s , real_T * f5q1lumvzg , real_T * j2naykddwm
, real_T * ea4r4f3bwk , real_T * cewk4ryfod , lsoeg0nito * localB ) { *
i2aqjuli5s = c5hhynsofi . P_37 ; localB -> mf3c0f3rln = 4.0 ; * f5q1lumvzg =
c5hhynsofi . P_47 ; * j2naykddwm = c5hhynsofi . P_48 ; * ea4r4f3bwk =
c5hhynsofi . P_49 ; * cewk4ryfod = c5hhynsofi . P_50 ; } void
EvPowertrainControllerBMS ( const real_T * ot1unth1hd , const real_T
fju13otua5 [ 4 ] , const real_T * gam2gozpi4 , const real_T * givymazxjb ,
const real_T * k443icl0sn , const real_T * iodoakv4fh , const real_T *
k0pcor1dt1 , const real_T * nirwbhd0cx , real_T * i2aqjuli5s , real_T *
ou3e1khm3v , real_T gpmtm1hqzh [ 4 ] , real_T maa0jy4xlg [ 6 ] , real_T *
f5q1lumvzg , real_T * j2naykddwm , real_T * ea4r4f3bwk , real_T * cewk4ryfod
, lsoeg0nito * localB ) { __m128d tmp ; real_T tmp_p [ 2 ] ; real_T
a4mqelxst3 ; real_T ddba0mkjqi ; real_T elxt5tm0e2 ; real_T kegmafxhj4 ;
real_T lsgne3iaae ; real_T ma0ydso2am ; real_T mnkkir5qa3 ; real_T oc3izlann4
; real_T ofef4pz50x ; real_T tmp_e ; * i2aqjuli5s = c5hhynsofi . P_37 ; tmp =
_mm_mul_pd ( _mm_mul_pd ( _mm_set1_pd ( rtP__PlntBrkRearBias2f2PassVeh_sldd_
) , _mm_loadu_pd ( & fju13otua5 [ 2 ] ) ) , _mm_set1_pd ( rtP__PlntBrkPrsFctr2f2PassVeh_sldd_ ) ) ; _mm_storeu_pd ( & tmp_p [ 0 ] , tmp ) ; ddba0mkjqi = rtP__PlntBrkKinFricCffFrnt2f2PassVeh_sldd_ * 3.1415926535897931 * 0.0025000000000000005 * rtP__PlntBrkPadMeanRadiusFrnt2f2PassVeh_sldd_ * rtP__PlntBrkNumPadsFrnt2f2PassVeh_sldd_ / 4.0 ; lsgne3iaae = rtP__PlntBrkKinFricCffRear2f2PassVeh_sldd_ * 3.1415926535897931 * 0.0025000000000000005 * rtP__PlntBrkPadMeanRadiusRear2f2PassVeh_sldd_ * rtP__PlntBrkNumPadsRear2f2PassVeh_sldd_ / 4.0 ; lsgne3iaae = ( ( rtP__PlntBrkFrntBias2f2PassVeh_sldd_ * fju13otua5 [ 0 ] * rtP__PlntBrkPrsFctr2f2PassVeh_sldd_ * ddba0mkjqi + rtP__PlntBrkFrntBias2f2PassVeh_sldd_ * fju13otua5 [ 1 ] * rtP__PlntBrkPrsFctr2f2PassVeh_sldd_ * ddba0mkjqi ) + lsgne3iaae * tmp_p [ 0 ] ) + lsgne3iaae * tmp_p [ 1 ] ; ofef4pz50x = c5hhynsofi . P_40 * * k443icl0sn ; elxt5tm0e2 = muDoubleScalarMin ( lsgne3iaae , look1_binlcapw ( c5hhynsofi . P_38 * * givymazxjb , c5hhynsofi . P_39 , rtP__PlntEM1Trq2f2ElectricMachine1_sldd_ , 13U ) * rtP__PlntDiffrntlRatio2f2Drivetrain_sldd_ ) * ( look1_binlcapw ( * gam2gozpi4 , rtP__CtrlVcuEvRegenBrkSpdBpt2f2EvVCU_sldd_ , rtP__CtrlVcuEvRegenBrkCutOff2f2EvVCU_sldd_ , 1U ) * look1_binlcapw ( ofef4pz50x , rtP__CtrlVcuEvSocBpt2f2EvVCU_sldd_ , rtP__CtrlVcuEvChrgLmtBpt2f2EvVCU_sldd_ , 10U ) ) ; if ( * ot1unth1hd > c5hhynsofi . P_18 ) { ma0ydso2am = look1_binlcapw ( c5hhynsofi . P_32 * * givymazxjb , c5hhynsofi . P_33 , rtP__PlntEM1Trq2f2ElectricMachine1_sldd_ , 13U ) * * ot1unth1hd ; } else { ma0ydso2am = 1.0 / rtP__PlntDiffrntlRatio2f2Drivetrain_sldd_ * elxt5tm0e2 * c5hhynsofi . P_34 ; } if ( ! ( muDoubleScalarAbs ( * givymazxjb ) < c5hhynsofi . P_19 ) ) { kegmafxhj4 = c5hhynsofi . P_25 * * k0pcor1dt1 * * iodoakv4fh ; tmp_e = * givymazxjb * ma0ydso2am ; if ( tmp_e >= c5hhynsofi . P_26 ) { a4mqelxst3 = c5hhynsofi . P_22 ; } else { a4mqelxst3 = c5hhynsofi . P_23 ; } mnkkir5qa3 = look2_binlcapw ( muDoubleScalarAbs ( c5hhynsofi . P_27 * * givymazxjb ) , muDoubleScalarAbs ( ma0ydso2am ) , c5hhynsofi . P_28 , rtP__PlntEM1TrqEff2f2ElectricMachine1_sldd_ , rtP__PlntEM1EffTbl2f2ElectricMachine1_sldd_ , c5hhynsofi . P_53 , 14U ) * c5hhynsofi . P_29 ; if ( ( mnkkir5qa3 < 0.0 ) && ( a4mqelxst3 > muDoubleScalarFloor ( a4mqelxst3 ) ) ) { a4mqelxst3 = - muDoubleScalarPower ( - mnkkir5qa3 , a4mqelxst3 ) ; } else { a4mqelxst3 = muDoubleScalarPower ( mnkkir5qa3 , a4mqelxst3 ) ; } mnkkir5qa3 = tmp_e * a4mqelxst3 ; oc3izlann4 = c5hhynsofi . P_30 * * nirwbhd0cx * * iodoakv4fh ; if ( ( ! ( kegmafxhj4 - mnkkir5qa3 >= c5hhynsofi . P_35 ) ) || ( ! ( oc3izlann4 - mnkkir5qa3 <= c5hhynsofi . P_36 ) ) ) { if ( ( * givymazxjb >= - c5hhynsofi . P_21 ) && ( * givymazxjb <= c5hhynsofi . P_21 ) ) { if ( * givymazxjb >= c5hhynsofi . P_24 ) { tmp_e = c5hhynsofi . P_52 ; } else { tmp_e = localB -> obiqgwz5nk ; } ma0ydso2am = 4.0 / ( 3.0 - muDoubleScalarPower ( * givymazxjb / 2.0 , 2.0 ) ) * tmp_e ; } else { ma0ydso2am = * givymazxjb ; } if ( ! ( mnkkir5qa3 > kegmafxhj4 ) ) { if ( mnkkir5qa3 < oc3izlann4 ) { kegmafxhj4 = oc3izlann4 ; } else { kegmafxhj4 = mnkkir5qa3 ; } } ma0ydso2am = kegmafxhj4 / a4mqelxst3 / ma0ydso2am ; } } kegmafxhj4 = look1_binlcapw ( c5hhynsofi . P_41 * * givymazxjb , c5hhynsofi . P_42 , rtP__PlntEM1Trq2f2ElectricMachine1_sldd_ , 13U ) ; if ( ofef4pz50x > c5hhynsofi . P_20 ) { ofef4pz50x = kegmafxhj4 ; } else { ofef4pz50x = c5hhynsofi . P_51 ; } if ( ma0ydso2am > ofef4pz50x ) { * ou3e1khm3v = ofef4pz50x ; } else { ofef4pz50x = c5hhynsofi . P_31 * kegmafxhj4 ; if ( ma0ydso2am < ofef4pz50x ) { * ou3e1khm3v = ofef4pz50x ; } else { * ou3e1khm3v = ma0ydso2am ; } } lsgne3iaae -= elxt5tm0e2 ; if ( lsgne3iaae > c5hhynsofi . P_43 ) { lsgne3iaae = c5hhynsofi . P_43 ; } else if ( lsgne3iaae < c5hhynsofi . P_44 ) { lsgne3iaae = c5hhynsofi . P_44 ; } lsgne3iaae = lsgne3iaae / ( ddba0mkjqi * 2.0 ) * ( 1.0 / rtP__PlntBrkPrsFctr2f2PassVeh_sldd_ ) ; if ( lsgne3iaae > c5hhynsofi . P_45 ) { lsgne3iaae = c5hhynsofi . P_45 ; } else if ( lsgne3iaae < c5hhynsofi . P_46 ) { lsgne3iaae = c5hhynsofi . P_46 ; } gpmtm1hqzh [ 0 ] = lsgne3iaae ; gpmtm1hqzh [ 1 ] = lsgne3iaae ; ma0ydso2am = 1.0 / rtP__PlntBrkPrsFctr2f2PassVeh_sldd_ ; gpmtm1hqzh [ 2 ] = ma0ydso2am * tmp_p [ 0 ] ; gpmtm1hqzh [ 3 ] = ma0ydso2am * tmp_p [ 1 ] ; ddba0mkjqi = ( ( ( gpmtm1hqzh [ 0 ] + gpmtm1hqzh [ 1 ] ) + gpmtm1hqzh [ 2 ] ) + gpmtm1hqzh [ 3 ] ) / localB -> mf3c0f3rln * rtP__CtrlBcuTrlrBrkGain2f2PassVeh_sldd_ ; maa0jy4xlg [ 0 ] = ddba0mkjqi ; maa0jy4xlg [ 1 ] = ddba0mkjqi ; maa0jy4xlg [ 2 ] = ddba0mkjqi ; maa0jy4xlg [ 3 ] = ddba0mkjqi ; maa0jy4xlg [ 4 ] = ddba0mkjqi ; maa0jy4xlg [ 5 ] = ddba0mkjqi ; * f5q1lumvzg = c5hhynsofi . P_47 ; * j2naykddwm = c5hhynsofi . P_48 ; * ea4r4f3bwk = c5hhynsofi . P_49 ; * cewk4ryfod = c5hhynsofi . P_50 ; } void EvPowertrainControllerBMSTID1 ( lsoeg0nito * localB ) { localB -> obiqgwz5nk = - c5hhynsofi . P_52 ; } void b3eofzzeql ( obv4iumo0b * const ftj5mqhdtx ) { if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( ftj5mqhdtx -> _mdlRefSfcnS , "EvPowertrainControllerBMS" , "SIMSTATUS_TERMINATING_MODELREF_ACCEL_EVENT" ) ; } } void aqzgcfjqsf ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , obv4iumo0b * const ftj5mqhdtx , lsoeg0nito * localB , void * sysRanPtr , int contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) { ( void ) memset ( ( void * ) ftj5mqhdtx , 0 , sizeof ( obv4iumo0b ) ) ; ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 0 ] = mdlref_TID0 ; ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 1 ] = mdlref_TID1 ; ftj5mqhdtx -> _mdlRefSfcnS = ( _mdlRefSfcnS ) ; if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( ftj5mqhdtx -> _mdlRefSfcnS , "EvPowertrainControllerBMS" , "START_OF_SIM_MODEL_MODELREF_ACCEL_EVENT" ) ; } { localB -> mf3c0f3rln = 0.0 ; localB -> obiqgwz5nk = 0.0 ; } EvPowertrainControllerBMS_InitializeDataMapInfo ( ftj5mqhdtx , sysRanPtr , contextTid ) ; if ( ( rt_ParentMMI != ( NULL ) ) && ( rt_ChildPath != ( NULL ) ) ) { rtwCAPI_SetChildMMI ( * rt_ParentMMI , rt_ChildMMIIdx , & ( ftj5mqhdtx -> DataMapInfo . mmi ) ) ; rtwCAPI_SetPath ( ftj5mqhdtx -> DataMapInfo . mmi , rt_ChildPath ) ; rtwCAPI_MMISetContStateStartIndex ( ftj5mqhdtx -> DataMapInfo . mmi , rt_CSTATEIdx ) ; } } void mr_EvPowertrainControllerBMS_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) { * retVal = 0 ; { boolean_T regSubmodelsMdlinfo = false ; ssGetRegSubmodelsMdlinfo ( mdlRefSfcnS , & regSubmodelsMdlinfo ) ; if ( regSubmodelsMdlinfo ) { } } * retVal = 0 ; ssRegModelRefMdlInfo ( mdlRefSfcnS , modelName , rtMdlInfo_EvPowertrainControllerBMS , 40 ) ; * retVal = 1 ; } static void mr_EvPowertrainControllerBMS_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) ; static void mr_EvPowertrainControllerBMS_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) { mxArray * newArray = mxCreateUninitNumericMatrix ( ( size_t ) 1 , numBytes , mxUINT8_CLASS , mxREAL ) ; memcpy ( ( uint8_T * ) mxGetData ( newArray ) , ( const uint8_T * ) srcData , numBytes ) ; mxSetFieldByNumber ( destArray , i , j , newArray ) ; } static void mr_EvPowertrainControllerBMS_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) ; static void mr_EvPowertrainControllerBMS_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) { memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) , numBytes ) ; } static void mr_EvPowertrainControllerBMS_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) ; static void mr_EvPowertrainControllerBMS_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) { mxSetFieldByNumber ( destArray , i , j , mxCreateDoubleScalar ( ( real_T ) bitVal ) ) ; } static uint_T mr_EvPowertrainControllerBMS_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) ; static uint_T mr_EvPowertrainControllerBMS_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) { const uint_T varVal = ( uint_T ) mxGetScalar ( mxGetFieldByNumber ( srcArray , i , j ) ) ; return varVal & ( ( 1u << numBits ) - 1u ) ; } static void mr_EvPowertrainControllerBMS_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) ; static void mr_EvPowertrainControllerBMS_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) { uint8_T * varData = ( uint8_T * ) mxGetData ( mxGetFieldByNumber ( destArray , i , j ) ) ; memcpy ( ( uint8_T * ) & varData [ offset * numBytes ] , ( const uint8_T * ) srcData , numBytes ) ; } static void mr_EvPowertrainControllerBMS_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) ; static void mr_EvPowertrainControllerBMS_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) { const uint8_T * varData = ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) ; memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) & varData [ offset * numBytes ] , numBytes ) ; } static void mr_EvPowertrainControllerBMS_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) ; static void mr_EvPowertrainControllerBMS_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) { mxSetCell ( mxGetFieldByNumber ( destArray , i , j ) , offset , mxCreateDoubleScalar ( ( real_T ) fieldVal ) ) ; } static uint_T mr_EvPowertrainControllerBMS_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) ; static uint_T mr_EvPowertrainControllerBMS_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) { const uint_T fieldVal = ( uint_T ) mxGetScalar ( mxGetCell ( mxGetFieldByNumber ( srcArray , i , j ) , offset ) ) ; return fieldVal & ( ( 1u << numBits ) - 1u ) ; } mxArray * mr_EvPowertrainControllerBMS_GetDWork ( const ngz2pfeakpt * mdlrefDW ) { static const char_T * ssDWFieldNames [ 3 ] = { "rtb" , "NULL->rtdw" , "NULL->rtzce" , } ; mxArray * ssDW = mxCreateStructMatrix ( 1 , 1 , 3 , ssDWFieldNames ) ; mr_EvPowertrainControllerBMS_cacheDataAsMxArray ( ssDW , 0 , 0 , ( const void * ) & ( mdlrefDW -> rtb ) , sizeof ( mdlrefDW -> rtb ) ) ; ( void ) mdlrefDW ; return ssDW ; } void mr_EvPowertrainControllerBMS_SetDWork ( ngz2pfeakpt * mdlrefDW , const mxArray * ssDW ) { ( void ) ssDW ; ( void ) mdlrefDW ; mr_EvPowertrainControllerBMS_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtb ) , ssDW , 0 , 0 , sizeof ( mdlrefDW -> rtb ) ) ; } void mr_EvPowertrainControllerBMS_RegisterSimStateChecksum ( SimStruct * S ) { const uint32_T chksum [ 4 ] = { 3523760296U , 1595081137U , 3034845175U , 580575907U , } ; slmrModelRefRegisterSimStateChecksum ( S , "EvPowertrainControllerBMS" , & chksum [ 0 ] ) ; } mxArray * mr_EvPowertrainControllerBMS_GetSimStateDisallowedBlocks ( ) { return ( NULL ) ; }
#if defined(_MSC_VER)
#pragma warning(disable: 4505) //unreferenced local function has been removed
#endif
