#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "EvPowertrainControllerBMS_capi_host.h"
#define sizeof(...) ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el) ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s) (s)
#ifndef SS_UINT64
#define SS_UINT64 17
#endif
#ifndef SS_INT64
#define SS_INT64 18
#endif
#else
#include "builtin_typeid_types.h"
#include "EvPowertrainControllerBMS.h"
#include "EvPowertrainControllerBMS_capi.h"
#include "EvPowertrainControllerBMS_private.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST
#define TARGET_STRING(s)               ((NULL))
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif
static rtwCAPI_Signals rtBlockSignals [ ] = { { 0 , 0 , ( NULL ) , ( NULL ) ,
0 , 0 , 0 , 0 , 0 } } ; static rtwCAPI_States rtBlockStates [ ] = { { 0 , - 1
, ( NULL ) , ( NULL ) , ( NULL ) , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } } ;
static int_T rt_LoggedStateIdxList [ ] = { - 1 } ; static TARGET_CONST
rtwCAPI_DataTypeMap rtDataTypeMap [ ] = { { "" , "" , 0 , 0 , 0 , 0 , 0 , 0 ,
0 } } ;
#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif
static TARGET_CONST rtwCAPI_ElementMap rtElementMap [ ] = { { ( NULL ) , 0 ,
0 , 0 , 0 } , } ; static rtwCAPI_DimensionMap rtDimensionMap [ ] = { {
rtwCAPI_SCALAR , 0 , 0 , 0 } } ; static uint_T rtDimensionArray [ ] = { 0 } ;
static rtwCAPI_FixPtMap rtFixPtMap [ ] = { { ( NULL ) , ( NULL ) ,
rtwCAPI_FIX_RESERVED , 0 , 0 , ( boolean_T ) 0 } , } ; static
rtwCAPI_SampleTimeMap rtSampleTimeMap [ ] = { { ( NULL ) , ( NULL ) , 0 , 0 }
} ; static int_T rtContextSystems [ 11 ] ; static rtwCAPI_LoggingMetaInfo
loggingMetaInfo [ ] = { { 0 , 0 , "" , 0 } } ; static
rtwCAPI_ModelMapLoggingStaticInfo mmiStaticInfoLogging = { 11 ,
rtContextSystems , loggingMetaInfo , 0 , ( NULL ) , { 0 , ( NULL ) , ( NULL )
} , 0 , ( NULL ) } ; static rtwCAPI_ModelMappingStaticInfo mmiStatic = { {
rtBlockSignals , 0 , ( NULL ) , 0 , ( NULL ) , 0 } , { ( NULL ) , 0 , ( NULL
) , 0 } , { rtBlockStates , 0 } , { rtDataTypeMap , rtDimensionMap ,
rtFixPtMap , rtElementMap , rtSampleTimeMap , rtDimensionArray } , "float" ,
{ 755973996U , 3448584586U , 364184256U , 4082232055U } , &
mmiStaticInfoLogging , 0 , ( boolean_T ) 0 , rt_LoggedStateIdxList } ; const
rtwCAPI_ModelMappingStaticInfo * EvPowertrainControllerBMS_GetCAPIStaticMap ( void ) { return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
static void EvPowertrainControllerBMS_InitializeSystemRan ( obv4iumo0b *
const ftj5mqhdtx , sysRanDType * systemRan [ ] , int_T systemTid [ ] , void *
rootSysRanPtr , int rootTid ) { UNUSED_PARAMETER ( ftj5mqhdtx ) ; systemRan [
0 ] = ( sysRanDType * ) rootSysRanPtr ; systemRan [ 1 ] = ( NULL ) ;
systemRan [ 2 ] = ( NULL ) ; systemRan [ 3 ] = ( NULL ) ; systemRan [ 4 ] = ( NULL ) ; systemRan [ 5 ] = ( NULL ) ; systemRan [ 6 ] = ( NULL ) ; systemRan [ 7 ] = ( NULL ) ; systemRan [ 8 ] = ( NULL ) ; systemRan [ 9 ] = ( NULL ) ; systemRan [ 10 ] = ( NULL ) ; systemTid [ 1 ] = ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 2 ] = ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 3 ] = ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 4 ] = ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 5 ] = ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 6 ] = ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 7 ] = ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 8 ] = ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 9 ] = ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 10 ] = ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 0 ] = rootTid ; rtContextSystems [ 0 ] = 0 ; rtContextSystems [ 1 ] = 0 ; rtContextSystems [ 2 ] = 0 ; rtContextSystems [ 3 ] = 0 ; rtContextSystems [ 4 ] = 0 ; rtContextSystems [ 5 ] = 0 ; rtContextSystems [ 6 ] = 0 ; rtContextSystems [ 7 ] = 0 ; rtContextSystems [ 8 ] = 0 ; rtContextSystems [ 9 ] = 0 ; rtContextSystems [ 10 ] = 0 ; }
#endif
#ifndef HOST_CAPI_BUILD
void EvPowertrainControllerBMS_InitializeDataMapInfo ( obv4iumo0b * const
ftj5mqhdtx , void * sysRanPtr , int contextTid ) { rtwCAPI_SetVersion ( ftj5mqhdtx -> DataMapInfo . mmi , 1 ) ; rtwCAPI_SetStaticMap ( ftj5mqhdtx -> DataMapInfo . mmi , & mmiStatic ) ; rtwCAPI_SetLoggingStaticMap ( ftj5mqhdtx -> DataMapInfo . mmi , & mmiStaticInfoLogging ) ; rtwCAPI_SetPath ( ftj5mqhdtx -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetFullPath ( ftj5mqhdtx -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetInstanceLoggingInfo ( ftj5mqhdtx -> DataMapInfo . mmi , & ftj5mqhdtx -> DataMapInfo . mmiLogInstanceInfo ) ; rtwCAPI_SetChildMMIArray ( ftj5mqhdtx -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArrayLen ( ftj5mqhdtx -> DataMapInfo . mmi , 0 ) ; EvPowertrainControllerBMS_InitializeSystemRan ( ftj5mqhdtx , ftj5mqhdtx -> DataMapInfo . systemRan , ftj5mqhdtx -> DataMapInfo . systemTid , sysRanPtr , contextTid ) ; rtwCAPI_SetSystemRan ( ftj5mqhdtx -> DataMapInfo . mmi , ftj5mqhdtx -> DataMapInfo . systemRan ) ; rtwCAPI_SetSystemTid ( ftj5mqhdtx -> DataMapInfo . mmi , ftj5mqhdtx -> DataMapInfo . systemTid ) ; rtwCAPI_SetGlobalTIDMap ( ftj5mqhdtx -> DataMapInfo . mmi , & ftj5mqhdtx -> Timing . mdlref_GlobalTID [ 0 ] ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void EvPowertrainControllerBMS_host_InitializeDataMapInfo ( EvPowertrainControllerBMS_host_DataMapInfo_T * dataMap , const char * path ) { rtwCAPI_SetVersion ( dataMap -> mmi , 1 ) ; rtwCAPI_SetStaticMap ( dataMap -> mmi , & mmiStatic ) ; rtwCAPI_SetDataAddressMap ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetVarDimsAddressMap ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetPath ( dataMap -> mmi , path ) ; rtwCAPI_SetFullPath ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArray ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArrayLen ( dataMap -> mmi , 0 ) ; }
#ifdef __cplusplus
}
#endif
#endif
