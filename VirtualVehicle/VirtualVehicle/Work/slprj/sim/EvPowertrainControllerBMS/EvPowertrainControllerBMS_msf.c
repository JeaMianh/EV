#if !defined(S_FUNCTION_NAME)
#define S_FUNCTION_NAME EvPowertrainControllerBMS_msf
#endif
#define S_FUNCTION_LEVEL 2
#if !defined(RTW_GENERATED_S_FUNCTION)
#define RTW_GENERATED_S_FUNCTION
#endif
#include <stdio.h>
#include <math.h>
#include "simstruc.h"
#include "fixedpoint.h"
#include "mwstringutil.h"
#define rt_logging_h
#include "EvPowertrainControllerBMS_types.h"
#include "EvPowertrainControllerBMS.h"
#include "EvPowertrainControllerBMS_private.h"
real_T rtP__CtrlBcuTrlrBrkGain2f2PassVeh_sldd_ ; real_T
rtP__CtrlVcuEvChrgLmtBpt2f2EvVCU_sldd_ [ 11 ] ; real_T
rtP__CtrlVcuEvRegenBrkCutOff2f2EvVCU_sldd_ [ 2 ] ; real_T
rtP__CtrlVcuEvRegenBrkSpdBpt2f2EvVCU_sldd_ [ 2 ] ; real_T
rtP__CtrlVcuEvSocBpt2f2EvVCU_sldd_ [ 11 ] ; real_T
rtP__PlntBrkFrntBias2f2PassVeh_sldd_ ; real_T
rtP__PlntBrkKinFricCffFrnt2f2PassVeh_sldd_ ; real_T
rtP__PlntBrkKinFricCffRear2f2PassVeh_sldd_ ; real_T
rtP__PlntBrkNumPadsFrnt2f2PassVeh_sldd_ ; real_T
rtP__PlntBrkNumPadsRear2f2PassVeh_sldd_ ; real_T
rtP__PlntBrkPadMeanRadiusFrnt2f2PassVeh_sldd_ ; real_T
rtP__PlntBrkPadMeanRadiusRear2f2PassVeh_sldd_ ; real_T
rtP__PlntBrkPrsFctr2f2PassVeh_sldd_ ; real_T
rtP__PlntBrkRearBias2f2PassVeh_sldd_ ; real_T
rtP__PlntDiffrntlRatio2f2Drivetrain_sldd_ ; real_T
rtP__PlntEM1EffTbl2f2ElectricMachine1_sldd_ [ 154 ] ; real_T
rtP__PlntEM1Trq2f2ElectricMachine1_sldd_ [ 14 ] ; real_T
rtP__PlntEM1TrqEff2f2ElectricMachine1_sldd_ [ 11 ] ; const char *
rt_GetMatSignalLoggingFileName ( void ) { return NULL ; } const char *
rt_GetMatSigLogSelectorFileName ( void ) { return NULL ; } void *
rt_GetOSigstreamManager ( void ) { return NULL ; } void * rt_slioCatalogue ( void ) { return NULL ; } void * rtwGetPointerFromUniquePtr ( void * uniquePtr ) { return NULL ; } void * CreateDiagnosticAsVoidPtr ( const char * id , int nargs , ... ) { void * voidPtrDiagnostic = NULL ; va_list args ; va_start ( args , nargs ) ; slmrCreateDiagnostic ( id , nargs , args , & voidPtrDiagnostic ) ; va_end ( args ) ; return voidPtrDiagnostic ; } void rt_ssSet_slErrMsg ( void * S , void * diag ) { SimStruct * simStrcut = ( SimStruct * ) S ; if ( ! _ssIsErrorStatusAslErrMsg ( simStrcut ) ) { _ssSet_slLocalErrMsg ( simStrcut , diag ) ; } else { _ssDiscardDiagnostic ( simStrcut , diag ) ; } } void rt_ssReportDiagnosticAsWarning ( void * S , void * diag ) { _ssReportDiagnosticAsWarning ( ( SimStruct * ) S , diag ) ; } void rt_ssReportDiagnosticAsInfo ( void * S , void * diag ) { _ssReportDiagnosticAsInfo ( ( SimStruct * ) S , diag ) ; } const char * rt_CreateFullPathToTop ( const char * toppath , const char * subpath ) { char * fullpath = NULL ; slmrCreateFullPathToTop ( toppath , subpath , & fullpath ) ; return fullpath ; } boolean_T slIsRapidAcceleratorSimulating ( void ) { return false ; } void rt_RAccelReplaceFromFilename ( const char * blockpath , char * fileName ) { ( void ) blockpath ; ( void ) fileName ; } void rt_RAccelReplaceToFilename ( const char * blockpath , char * fileName ) { ( void ) blockpath ; ( void ) fileName ; } void * slsa_malloc ( size_t s ) { return malloc ( s ) ; } void slsa_free ( void * ptr ) { free ( ptr ) ; } void slsaCacheDWorkPointerForSimTargetOP ( void * ss , void * * ptr ) { ( void ) ss ; ( void ) ptr ; } void slsaCacheDWorkDataForSimTargetOP ( void * ss , void * ptr , unsigned int sizeInBytes ) { ( void ) ss ; ( void ) ptr ; ( void ) sizeInBytes ; } void slsaSaveRawMemoryForSimTargetOP ( void * ss , const char * key , void * * ptr , unsigned int sizeInBytes , void * ( * customOPSaveFcn ) ( void * dworkPtr , unsigned int * sizeInBytes ) , void ( * customOPRestoreFcn ) ( void * dworkPtr , const void * data , unsigned int sizeInBytes ) ) { ( void ) ss ; ( void ) key ; ( void ) ptr ; ( void ) sizeInBytes ; ( void ) customOPSaveFcn ; ( void ) customOPRestoreFcn ; }
#define MDL_PROCESS_PARAMETERS
#if defined(MATLAB_MEX_FILE)
static void mdlProcessParameters ( SimStruct * S ) { real_T * GlobalPrm_0 = ( real_T * ) NULL ; real_T * GlobalPrm_1 = ( real_T * ) NULL ; real_T * GlobalPrm_2 = ( real_T * ) NULL ; real_T * GlobalPrm_3 = ( real_T * ) NULL ; real_T * GlobalPrm_4 = ( real_T * ) NULL ; real_T * GlobalPrm_5 = ( real_T * ) NULL ; real_T * GlobalPrm_6 = ( real_T * ) NULL ; real_T * GlobalPrm_7 = ( real_T * ) NULL ; real_T * GlobalPrm_8 = ( real_T * ) NULL ; real_T * GlobalPrm_9 = ( real_T * ) NULL ; real_T * GlobalPrm_10 = ( real_T * ) NULL ; real_T * GlobalPrm_11 = ( real_T * ) NULL ; real_T * GlobalPrm_12 = ( real_T * ) NULL ; real_T * GlobalPrm_13 = ( real_T * ) NULL ; real_T * GlobalPrm_14 = ( real_T * ) NULL ; real_T * GlobalPrm_15 = ( real_T * ) NULL ; real_T * GlobalPrm_16 = ( real_T * ) NULL ; real_T * GlobalPrm_17 = ( real_T * ) NULL ; if ( ! ssGetModelRefGlobalParamData ( S , 0 , ( void * * ) ( & GlobalPrm_0 ) ) ) return ; if ( GlobalPrm_0 != NULL ) { ( void ) memcpy ( & ( rtP__CtrlBcuTrlrBrkGain2f2PassVeh_sldd_ ) , GlobalPrm_0 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 1 , ( void * * ) ( & GlobalPrm_1 ) ) ) return ; if ( GlobalPrm_1 != NULL ) { ( void ) memcpy ( & ( rtP__CtrlVcuEvChrgLmtBpt2f2EvVCU_sldd_ [ 0 ] ) , GlobalPrm_1 , sizeof ( real_T ) * 11 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 2 , ( void * * ) ( & GlobalPrm_2 ) ) ) return ; if ( GlobalPrm_2 != NULL ) { ( void ) memcpy ( & ( rtP__CtrlVcuEvRegenBrkCutOff2f2EvVCU_sldd_ [ 0 ] ) , GlobalPrm_2 , sizeof ( real_T ) * 2 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 3 , ( void * * ) ( & GlobalPrm_3 ) ) ) return ; if ( GlobalPrm_3 != NULL ) { ( void ) memcpy ( & ( rtP__CtrlVcuEvRegenBrkSpdBpt2f2EvVCU_sldd_ [ 0 ] ) , GlobalPrm_3 , sizeof ( real_T ) * 2 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 4 , ( void * * ) ( & GlobalPrm_4 ) ) ) return ; if ( GlobalPrm_4 != NULL ) { ( void ) memcpy ( & ( rtP__CtrlVcuEvSocBpt2f2EvVCU_sldd_ [ 0 ] ) , GlobalPrm_4 , sizeof ( real_T ) * 11 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 5 , ( void * * ) ( & GlobalPrm_5 ) ) ) return ; if ( GlobalPrm_5 != NULL ) { ( void ) memcpy ( & ( rtP__PlntBrkFrntBias2f2PassVeh_sldd_ ) , GlobalPrm_5 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 6 , ( void * * ) ( & GlobalPrm_6 ) ) ) return ; if ( GlobalPrm_6 != NULL ) { ( void ) memcpy ( & ( rtP__PlntBrkKinFricCffFrnt2f2PassVeh_sldd_ ) , GlobalPrm_6 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 7 , ( void * * ) ( & GlobalPrm_7 ) ) ) return ; if ( GlobalPrm_7 != NULL ) { ( void ) memcpy ( & ( rtP__PlntBrkKinFricCffRear2f2PassVeh_sldd_ ) , GlobalPrm_7 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 8 , ( void * * ) ( & GlobalPrm_8 ) ) ) return ; if ( GlobalPrm_8 != NULL ) { ( void ) memcpy ( & ( rtP__PlntBrkNumPadsFrnt2f2PassVeh_sldd_ ) , GlobalPrm_8 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 9 , ( void * * ) ( & GlobalPrm_9 ) ) ) return ; if ( GlobalPrm_9 != NULL ) { ( void ) memcpy ( & ( rtP__PlntBrkNumPadsRear2f2PassVeh_sldd_ ) , GlobalPrm_9 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 10 , ( void * * ) ( & GlobalPrm_10 ) ) ) return ; if ( GlobalPrm_10 != NULL ) { ( void ) memcpy ( & ( rtP__PlntBrkPadMeanRadiusFrnt2f2PassVeh_sldd_ ) , GlobalPrm_10 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 11 , ( void * * ) ( & GlobalPrm_11 ) ) ) return ; if ( GlobalPrm_11 != NULL ) { ( void ) memcpy ( & ( rtP__PlntBrkPadMeanRadiusRear2f2PassVeh_sldd_ ) , GlobalPrm_11 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 12 , ( void * * ) ( & GlobalPrm_12 ) ) ) return ; if ( GlobalPrm_12 != NULL ) { ( void ) memcpy ( & ( rtP__PlntBrkPrsFctr2f2PassVeh_sldd_ ) , GlobalPrm_12 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 13 , ( void * * ) ( & GlobalPrm_13 ) ) ) return ; if ( GlobalPrm_13 != NULL ) { ( void ) memcpy ( & ( rtP__PlntBrkRearBias2f2PassVeh_sldd_ ) , GlobalPrm_13 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 14 , ( void * * ) ( & GlobalPrm_14 ) ) ) return ; if ( GlobalPrm_14 != NULL ) { ( void ) memcpy ( & ( rtP__PlntDiffrntlRatio2f2Drivetrain_sldd_ ) , GlobalPrm_14 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 15 , ( void * * ) ( & GlobalPrm_15 ) ) ) return ; if ( GlobalPrm_15 != NULL ) { ( void ) memcpy ( & ( rtP__PlntEM1EffTbl2f2ElectricMachine1_sldd_ [ 0 ] ) , GlobalPrm_15 , sizeof ( real_T ) * 154 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 16 , ( void * * ) ( & GlobalPrm_16 ) ) ) return ; if ( GlobalPrm_16 != NULL ) { ( void ) memcpy ( & ( rtP__PlntEM1Trq2f2ElectricMachine1_sldd_ [ 0 ] ) , GlobalPrm_16 , sizeof ( real_T ) * 14 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 17 , ( void * * ) ( & GlobalPrm_17 ) ) ) return ; if ( GlobalPrm_17 != NULL ) { ( void ) memcpy ( & ( rtP__PlntEM1TrqEff2f2ElectricMachine1_sldd_ [ 0 ] ) , GlobalPrm_17 , sizeof ( real_T ) * 11 ) ; } }
#endif
static void mdlInitializeConditions ( SimStruct * S ) { ngz2pfeakpt * dw = ( ngz2pfeakpt * ) ssGetDWork ( S , 0 ) ; real_T * o_o_o_B_10_1 = ( real_T * ) ssGetOutputPortSignal ( S , 0 ) ; real_T * o_o_o_B_10_5 = ( real_T * ) ssGetOutputPortSignal ( S , 4 ) ; real_T * o_o_o_B_10_6 = ( real_T * ) ssGetOutputPortSignal ( S , 5 ) ; real_T * o_o_o_B_10_7 = ( real_T * ) ssGetOutputPortSignal ( S , 6 ) ; real_T * o_o_o_B_10_8 = ( real_T * ) ssGetOutputPortSignal ( S , 7 ) ; cdlti3213i ( o_o_o_B_10_1 , o_o_o_B_10_5 , o_o_o_B_10_6 , o_o_o_B_10_7 , o_o_o_B_10_8 ) ; } static void mdlPeriodicOutputUpdate ( SimStruct * S , int_T tid ) { ngz2pfeakpt * dw = ( ngz2pfeakpt * ) ssGetDWork ( S , 0 ) ; real_T const * i_k0qkz30lvp = ( real_T * ) ssGetInputPortSignal ( S , 0 ) ; real_T const * i_jtifdqkawf = ( real_T * ) ssGetInputPortSignal ( S , 1 ) ; real_T const * i_d1n3ldkpku = ( real_T * ) ssGetInputPortSignal ( S , 2 ) ; real_T const * i_cgphvvw5p5 = ( real_T * ) ssGetInputPortSignal ( S , 3 ) ; real_T const * i_ixdt5lrz0n = ( real_T * ) ssGetInputPortSignal ( S , 4 ) ; real_T const * i_k2gzv0qee1 = ( real_T * ) ssGetInputPortSignal ( S , 5 ) ; real_T const * i_kzx0saseph = ( real_T * ) ssGetInputPortSignal ( S , 6 ) ; real_T const * i_oexkfnmire = ( real_T * ) ssGetInputPortSignal ( S , 7 ) ; real_T * o_o_o_B_10_1 = ( real_T * ) ssGetOutputPortSignal ( S , 0 ) ; real_T * o_B_10_2 = ( real_T * ) ssGetOutputPortSignal ( S , 1 ) ; real_T * o_B_10_3 = ( real_T * ) ssGetOutputPortSignal ( S , 2 ) ; real_T * o_B_10_4 = ( real_T * ) ssGetOutputPortSignal ( S , 3 ) ; real_T * o_o_o_B_10_5 = ( real_T * ) ssGetOutputPortSignal ( S , 4 ) ; real_T * o_o_o_B_10_6 = ( real_T * ) ssGetOutputPortSignal ( S , 5 ) ; real_T * o_o_o_B_10_7 = ( real_T * ) ssGetOutputPortSignal ( S , 6 ) ; real_T * o_o_o_B_10_8 = ( real_T * ) ssGetOutputPortSignal ( S , 7 ) ; if ( tid == 0 ) { EvPowertrainControllerBMS ( i_k0qkz30lvp , i_jtifdqkawf , i_d1n3ldkpku , i_cgphvvw5p5 , i_ixdt5lrz0n , i_k2gzv0qee1 , i_kzx0saseph , i_oexkfnmire , o_o_o_B_10_1 , o_B_10_2 , o_B_10_3 , o_B_10_4 , o_o_o_B_10_5 , o_o_o_B_10_6 , o_o_o_B_10_7 , o_o_o_B_10_8 , & ( dw -> rtb ) ) ; } } static void mdlInitializeSizes ( SimStruct * S ) { if ( ( S -> mdlInfo -> genericFcn != ( NULL ) ) && ( ! ( S -> mdlInfo -> genericFcn ) ( S , GEN_FCN_CHK_MODELREF_SFUN_HAS_MODEL_BLOCK , - 1 , ( NULL ) ) ) ) { return ; } ssSetNumSFcnParams ( S , 0 ) ; ssFxpSetU32BitRegionCompliant ( S , 1 ) ; if ( S -> mdlInfo -> genericFcn != ( NULL ) ) { _GenericFcn fcn = S -> mdlInfo -> genericFcn ; } ssSetRTWGeneratedSFcn ( S , 2 ) ; ssSetNumContStates ( S , 0 ) ; ssSetNumDiscStates ( S , 0 ) ; ssSetSymbolicDimsSupport ( S , true ) ; slmrInitializeIOPortDataVectors ( S , 8 , 8 ) ; if ( ! ssSetNumInputPorts ( S , 8 ) ) return ; if ( ! ssSetInputPortVectorDimension ( S , 0 , 1 ) ) return ; ssSetInputPortDimensionsMode ( S , 0 , FIXED_DIMS_MODE ) ; ssSetInputPortFrameData ( S , 0 , FRAME_NO ) ; if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 0 , SS_DOUBLE ) ; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 0 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 0 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 0 , 1 ) ; ssSetInputPortOptimOpts ( S
, 0 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 0 ,
false ) ; ssSetInputPortSampleTime ( S , 0 , 0.01 ) ;
ssSetInputPortOffsetTime ( S , 0 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 1 , 4 ) ) return ;
ssSetInputPortDimensionsMode ( S , 1 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 1 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 1 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 1 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 1 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 1 , 1 ) ; ssSetInputPortOptimOpts ( S
, 1 , SS_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 1 , false )
; ssSetInputPortSampleTime ( S , 1 , 0.01 ) ; ssSetInputPortOffsetTime ( S ,
1 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 2 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 2 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 2 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 2 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 2 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 2 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 2 , 1 ) ; ssSetInputPortOptimOpts ( S
, 2 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 2 ,
false ) ; ssSetInputPortSampleTime ( S , 2 , 0.01 ) ;
ssSetInputPortOffsetTime ( S , 2 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 3 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 3 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 3 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 3 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 3 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 3 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 3 , 1 ) ; ssSetInputPortOptimOpts ( S
, 3 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 3 ,
false ) ; ssSetInputPortSampleTime ( S , 3 , 0.01 ) ;
ssSetInputPortOffsetTime ( S , 3 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 4 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 4 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 4 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 4 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 4 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 4 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 4 , 1 ) ; ssSetInputPortOptimOpts ( S
, 4 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 4 ,
false ) ; ssSetInputPortSampleTime ( S , 4 , 0.01 ) ;
ssSetInputPortOffsetTime ( S , 4 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 5 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 5 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 5 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 5 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 5 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 5 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 5 , 1 ) ; ssSetInputPortOptimOpts ( S
, 5 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 5 ,
false ) ; ssSetInputPortSampleTime ( S , 5 , 0.01 ) ;
ssSetInputPortOffsetTime ( S , 5 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 6 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 6 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 6 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 6 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 6 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 6 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 6 , 1 ) ; ssSetInputPortOptimOpts ( S
, 6 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 6 ,
false ) ; ssSetInputPortSampleTime ( S , 6 , 0.01 ) ;
ssSetInputPortOffsetTime ( S , 6 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 7 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 7 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 7 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 7 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 7 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 7 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 7 , 1 ) ; ssSetInputPortOptimOpts ( S
, 7 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 7 ,
false ) ; ssSetInputPortSampleTime ( S , 7 , 0.01 ) ;
ssSetInputPortOffsetTime ( S , 7 , 0.0 ) ; if ( ! ssSetNumOutputPorts ( S , 8
) ) return ; if ( ! ssSetOutputPortVectorDimension ( S , 0 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 0 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 0 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 0 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 0 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 0 , 0.01 ) ; ssSetOutputPortOffsetTime ( S
, 0 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 0 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 0 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 0 , false , true , false ) ;
ssSetOutputPortOptimOpts ( S , 0 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 1 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 1 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 1 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 1 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 1 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 1 , 0.01 ) ; ssSetOutputPortOffsetTime ( S
, 1 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 1 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 1 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 1 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 1 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 2 , 4 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 2 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 2 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 2 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 2 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 2 , 0.01 ) ; ssSetOutputPortOffsetTime ( S
, 2 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 2 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 2 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 2 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 2 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 3 , 6 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 3 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 3 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 3 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 3 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 3 , 0.01 ) ; ssSetOutputPortOffsetTime ( S
, 3 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 3 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 3 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 3 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 3 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 4 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 4 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 4 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 4 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 4 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 4 , 0.01 ) ; ssSetOutputPortOffsetTime ( S
, 4 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 4 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 4 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 4 , false , true , false ) ;
ssSetOutputPortOptimOpts ( S , 4 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 5 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 5 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 5 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 5 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 5 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 5 , 0.01 ) ; ssSetOutputPortOffsetTime ( S
, 5 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 5 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 5 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 5 , false , true , false ) ;
ssSetOutputPortOptimOpts ( S , 5 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 6 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 6 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 6 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 6 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 6 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 6 , 0.01 ) ; ssSetOutputPortOffsetTime ( S
, 6 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 6 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 6 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 6 , false , true , false ) ;
ssSetOutputPortOptimOpts ( S , 6 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 7 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 7 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 7 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 7 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 7 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 7 , 0.01 ) ; ssSetOutputPortOffsetTime ( S
, 7 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 7 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 7 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 7 , false , true , false ) ;
ssSetOutputPortOptimOpts ( S , 7 , SS_NOT_REUSABLE_AND_GLOBAL ) ;
ssSetSimStateCompliance ( S , USE_CUSTOM_SIM_STATE ) ;
mr_EvPowertrainControllerBMS_RegisterSimStateChecksum ( S ) ;
ssSetNumSampleTimes ( S , 2 ) ; ssSetParameterTuningCompliance ( S , true ) ;
ssSetNumRWork ( S , 0 ) ; ssSetNumIWork ( S , 0 ) ; ssSetNumPWork ( S , 0 ) ;
ssSetNumModes ( S , 0 ) ; { int_T zcsIdx = 0 ; }
ssSetOutputPortIsNonContinuous ( S , 0 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 0 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 1 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 1 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 2 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 2 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 3 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 3 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 4 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 4 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 5 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 5 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 6 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 6 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 7 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 7 , 0 ) ;
ssSetInputPortIsNotDerivPort ( S , 0 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 1 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 2 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 3 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 4 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 5 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 6 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 7 , 1 ) ; ssSetModelReferenceSampleTimeInheritanceRule ( S ,
DISALLOW_SAMPLE_TIME_INHERITANCE ) ; ssSetRuntimeThreadSafetyCompliance ( S ,
RUNTIME_THREAD_SAFETY_COMPLIANCE_TRUE ) ; ssSetAcceptsFcnCallInputs ( S ) ;
ssSetModelReferenceNormalModeSupport ( S ,
MDL_START_AND_MDL_PROCESS_PARAMS_OK ) ; ssSupportsMultipleExecInstances ( S ,
true ) ; ssHasStateInsideForEachSS ( S , false ) ; ssSetOptions ( S ,
SS_OPTION_ALLOW_CONSTANT_PORT_SAMPLE_TIME |
SS_OPTION_PORT_SAMPLE_TIMES_ASSIGNED | SS_OPTION_SUPPORTS_ALIAS_DATA_TYPES |
SS_OPTION_DISALLOW_CONSTANT_SAMPLE_TIME | SS_OPTION_EXCEPTION_FREE_CODE |
SS_OPTION_WORKS_WITH_CODE_REUSE ) ;
#if SS_SFCN_FOR_SIM
if ( S -> mdlInfo -> genericFcn != ( NULL ) && ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { int_T retVal = 1 ;
mr_EvPowertrainControllerBMS_MdlInfoRegFcn ( S , "EvPowertrainControllerBMS"
, & retVal ) ; if ( ! retVal ) return ; }
#endif
#if SS_SFCN_FOR_SIM
if ( ssSetNumDWork ( S , 1 ) ) { int mdlrefDWTypeId ; ssRegMdlRefDWorkType ( S
, & mdlrefDWTypeId ) ; if ( mdlrefDWTypeId == INVALID_DTYPE_ID ) return ; if
( ! ssSetDataTypeSize ( S , mdlrefDWTypeId , sizeof ( ngz2pfeakpt ) ) )
return ; ssSetDWorkDataType ( S , 0 , mdlrefDWTypeId ) ; ssSetDWorkWidth ( S
, 0 , 1 ) ; }
#else
if ( ! ssSetNumDWork ( S , 1 ) ) { return ; }
#endif
slmrRegisterSystemInitializeMethod ( S , mdlInitializeConditions ) ;
slmrRegisterPeriodicOutputUpdateMethod ( S , mdlPeriodicOutputUpdate ) ;
ssSetSimulinkVersionGeneratedIn ( S , "24.2" ) ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { ssSetSampleTime ( S , 0 , 0.01 )
; ssSetOffsetTime ( S , 0 , 0 ) ; ssSetSampleTime ( S , 1 , mxGetInf ( ) ) ;
ssSetOffsetTime ( S , 1 , 0 ) ; return ; }
#define MDL_SET_WORK_WIDTHS
static void mdlSetWorkWidths ( SimStruct * S ) { if ( S -> mdlInfo ->
genericFcn != ( NULL ) ) { _GenericFcn fcn = S -> mdlInfo -> genericFcn ;
ssSetSignalSizesComputeType ( S , SS_VARIABLE_SIZE_FROM_INPUT_VALUE_AND_SIZE
) ; } { static const char * toFileNames [ ] = { "" } ; static const char *
fromFileNames [ ] = { "" } ; if ( ! ssSetModelRefFromFiles ( S , 0 ,
fromFileNames ) ) return ; if ( ! ssSetModelRefToFiles ( S , 0 , toFileNames
) ) return ; } }
#define MDL_SETUP_RUNTIME_RESOURCES
static void mdlSetupRuntimeResources ( SimStruct * S ) { ngz2pfeakpt * dw = ( ngz2pfeakpt * ) ssGetDWork ( S , 0 ) ; real_T * o_o_o_B_10_1 = ( real_T * ) ssGetOutputPortSignal ( S , 0 ) ; real_T * o_o_o_B_10_5 = ( real_T * ) ssGetOutputPortSignal ( S , 4 ) ; real_T * o_o_o_B_10_6 = ( real_T * ) ssGetOutputPortSignal ( S , 5 ) ; real_T * o_o_o_B_10_7 = ( real_T * ) ssGetOutputPortSignal ( S , 6 ) ; real_T * o_o_o_B_10_8 = ( real_T * ) ssGetOutputPortSignal ( S , 7 ) ; void * sysRanPtr = ( NULL ) ; int sysTid = 0 ; ssGetContextSysRanBCPtr ( S , & sysRanPtr ) ; ssGetContextSysTid ( S , & sysTid ) ; if ( sysTid == CONSTANT_TID ) { sysTid = 0 ; } aqzgcfjqsf ( S , slmrGetTopTidFromMdlRefChildTid ( S , 0 , false ) , slmrGetTopTidFromMdlRefChildTid ( S , 1 , true ) , & ( dw -> rtm ) , & ( dw -> rtb ) , sysRanPtr , sysTid , ( ( NULL ) ) , ( ( NULL ) ) , 0 , - 1 ) ; ssSetModelMappingInfoPtr ( S , & ( dw -> rtm . DataMapInfo . mmi ) ) ; if ( S -> mdlInfo -> genericFcn != ( NULL ) ) { _GenericFcn fcn = S -> mdlInfo -> genericFcn ; } }
#define MDL_START
static void mdlStart ( SimStruct * S ) { ngz2pfeakpt * dw = ( ngz2pfeakpt * )
ssGetDWork ( S , 0 ) ; real_T * o_o_o_B_10_1 = ( real_T * )
ssGetOutputPortSignal ( S , 0 ) ; real_T * o_o_o_B_10_5 = ( real_T * )
ssGetOutputPortSignal ( S , 4 ) ; real_T * o_o_o_B_10_6 = ( real_T * )
ssGetOutputPortSignal ( S , 5 ) ; real_T * o_o_o_B_10_7 = ( real_T * )
ssGetOutputPortSignal ( S , 6 ) ; real_T * o_o_o_B_10_8 = ( real_T * )
ssGetOutputPortSignal ( S , 7 ) ; mdlProcessParameters ( S ) ; f5tbg1pt2m ( o_o_o_B_10_1 , o_o_o_B_10_5 , o_o_o_B_10_6 , o_o_o_B_10_7 , o_o_o_B_10_8 , & ( dw -> rtb ) ) ; } static void mdlOutputs ( SimStruct * S , int_T tid ) { ngz2pfeakpt * dw = ( ngz2pfeakpt * ) ssGetDWork ( S , 0 ) ; real_T const * i_k0qkz30lvp = ( real_T * ) ssGetInputPortSignal ( S , 0 ) ; real_T const * i_jtifdqkawf = ( real_T * ) ssGetInputPortSignal ( S , 1 ) ; real_T const * i_d1n3ldkpku = ( real_T * ) ssGetInputPortSignal ( S , 2 ) ; real_T const * i_cgphvvw5p5 = ( real_T * ) ssGetInputPortSignal ( S , 3 ) ; real_T const * i_ixdt5lrz0n = ( real_T * ) ssGetInputPortSignal ( S , 4 ) ; real_T const * i_k2gzv0qee1 = ( real_T * ) ssGetInputPortSignal ( S , 5 ) ; real_T const * i_kzx0saseph = ( real_T * ) ssGetInputPortSignal ( S , 6 ) ; real_T const * i_oexkfnmire = ( real_T * ) ssGetInputPortSignal ( S , 7 ) ; real_T * o_o_o_B_10_1 = ( real_T * ) ssGetOutputPortSignal ( S , 0 ) ; real_T * o_B_10_2 = ( real_T * ) ssGetOutputPortSignal ( S , 1 ) ; real_T * o_B_10_3 = ( real_T * ) ssGetOutputPortSignal ( S , 2 ) ; real_T * o_B_10_4 = ( real_T * ) ssGetOutputPortSignal ( S , 3 ) ; real_T * o_o_o_B_10_5 = ( real_T * ) ssGetOutputPortSignal ( S , 4 ) ; real_T * o_o_o_B_10_6 = ( real_T * ) ssGetOutputPortSignal ( S , 5 ) ; real_T * o_o_o_B_10_7 = ( real_T * ) ssGetOutputPortSignal ( S , 6 ) ; real_T * o_o_o_B_10_8 = ( real_T * ) ssGetOutputPortSignal ( S , 7 ) ; if ( tid == PARAMETER_TUNING_TID ) { EvPowertrainControllerBMSTID1 ( & ( dw -> rtb ) ) ; } if ( tid != CONSTANT_TID && tid != PARAMETER_TUNING_TID ) { EvPowertrainControllerBMS ( i_k0qkz30lvp , i_jtifdqkawf , i_d1n3ldkpku , i_cgphvvw5p5 , i_ixdt5lrz0n , i_k2gzv0qee1 , i_kzx0saseph , i_oexkfnmire , o_o_o_B_10_1 , o_B_10_2 , o_B_10_3 , o_B_10_4 , o_o_o_B_10_5 , o_o_o_B_10_6 , o_o_o_B_10_7 , o_o_o_B_10_8 , & ( dw -> rtb ) ) ; } } static void mdlTerminate ( SimStruct * S ) { ngz2pfeakpt * dw = ( ngz2pfeakpt * ) ssGetDWork ( S , 0 ) ; b3eofzzeql ( & ( dw -> rtm ) ) ; return ; }
#define MDL_CLEANUP_RUNTIME_RESOURCES
static void mdlCleanupRuntimeResources ( SimStruct * S ) { }
#if !defined(MDL_SIM_STATE)
#define MDL_SIM_STATE
#endif
static mxArray * mdlGetSimState ( SimStruct * S ) { static const char *
simStateFieldNames [ 6 ] = { "localX" , "mdlrefDW" , "disallowedStateData" ,
"tNext" , "tNextTid" , "nonContDerivSigInfoPrevVal" , } ; mxArray * ss =
mxCreateStructMatrix ( 1 , 1 , 6 , simStateFieldNames ) ; { mxArray *
mdlrefDW = mr_EvPowertrainControllerBMS_GetDWork ( ssGetDWork ( S , 0 ) ) ;
mxSetFieldByNumber ( ss , 0 , 1 , mdlrefDW ) ; } { mxArray * data =
mr_EvPowertrainControllerBMS_GetSimStateDisallowedBlocks ( ) ;
mxSetFieldByNumber ( ss , 0 , 2 , data ) ; } ; mxSetFieldByNumber ( ss , 0 ,
3 , mxCreateDoubleScalar ( ( double ) ssGetTNext ( S ) ) ) ;
mxSetFieldByNumber ( ss , 0 , 4 , mxCreateDoubleScalar ( ( double )
ssGetTNextTid ( S ) ) ) ; return ss ; }
#if !defined(MDL_SIM_STATE)
#define MDL_SIM_STATE
#endif
static void mdlSetSimState ( SimStruct * S , const mxArray * ss ) {
mr_EvPowertrainControllerBMS_SetDWork ( ssGetDWork ( S , 0 ) ,
mxGetFieldByNumber ( ss , 0 , 1 ) ) ; ssSetTNext ( S , ( time_T ) mxGetScalar
( mxGetFieldByNumber ( ss , 0 , 3 ) ) ) ; ssSetTNextTid ( S , ( int_T )
mxGetScalar ( mxGetFieldByNumber ( ss , 0 , 4 ) ) ) ; }
#ifdef MATLAB_MEX_FILE 
#include "simulink.c"
#include "fixedpoint.c"
#else
#error Assertion failed: file must be compiled as a MEX-file
#endif
