#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "TransmissionController_capi_host.h"
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
#include "TransmissionController.h"
#include "TransmissionController_capi.h"
#include "TransmissionController_private.h"
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
} ; static int_T rtContextSystems [ 6 ] ; static rtwCAPI_LoggingMetaInfo
loggingMetaInfo [ ] = { { 0 , 0 , "" , 0 } } ; static
rtwCAPI_ModelMapLoggingStaticInfo mmiStaticInfoLogging = { 6 ,
rtContextSystems , loggingMetaInfo , 0 , ( NULL ) , { 0 , ( NULL ) , ( NULL )
} , 0 , ( NULL ) } ; static rtwCAPI_ModelMappingStaticInfo mmiStatic = { {
rtBlockSignals , 0 , ( NULL ) , 0 , ( NULL ) , 0 } , { ( NULL ) , 0 , ( NULL
) , 0 } , { rtBlockStates , 0 } , { rtDataTypeMap , rtDimensionMap ,
rtFixPtMap , rtElementMap , rtSampleTimeMap , rtDimensionArray } , "float" ,
{ 3928164161U , 3337137540U , 1765666061U , 530253588U } , &
mmiStaticInfoLogging , 0 , ( boolean_T ) 0 , rt_LoggedStateIdxList } ; const
rtwCAPI_ModelMappingStaticInfo * TransmissionController_GetCAPIStaticMap ( void
) { return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
static void TransmissionController_InitializeSystemRan ( prjutdmklf * const
msg2oexmhm , sysRanDType * systemRan [ ] , ikk0kw0e3s * localDW , int_T
systemTid [ ] , void * rootSysRanPtr , int rootTid ) { UNUSED_PARAMETER ( msg2oexmhm ) ; UNUSED_PARAMETER ( localDW ) ; systemRan [ 0 ] = ( sysRanDType * ) rootSysRanPtr ; systemRan [ 1 ] = ( sysRanDType * ) & localDW -> c1rvhddg4tu . czt2emm2gr ; systemRan [ 2 ] = ( sysRanDType * ) & localDW -> fe5mmldjmq ; systemRan [ 3 ] = ( sysRanDType * ) & localDW -> lyaq3qnmfw ; systemRan [ 4 ] = ( NULL ) ; systemRan [ 5 ] = ( NULL ) ; systemTid [ 3 ] = msg2oexmhm -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 2 ] = msg2oexmhm -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 1 ] = msg2oexmhm -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 4 ] = msg2oexmhm -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 5 ] = msg2oexmhm -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 0 ] = rootTid ; rtContextSystems [ 0 ] = 0 ; rtContextSystems [ 1 ] = 1 ; rtContextSystems [ 2 ] = 2 ; rtContextSystems [ 3 ] = 3 ; rtContextSystems [ 4 ] = 0 ; rtContextSystems [ 5 ] = 0 ; }
#endif
#ifndef HOST_CAPI_BUILD
void TransmissionController_InitializeDataMapInfo ( prjutdmklf * const
msg2oexmhm , ikk0kw0e3s * localDW , void * sysRanPtr , int contextTid ) {
rtwCAPI_SetVersion ( msg2oexmhm -> DataMapInfo . mmi , 1 ) ;
rtwCAPI_SetStaticMap ( msg2oexmhm -> DataMapInfo . mmi , & mmiStatic ) ;
rtwCAPI_SetLoggingStaticMap ( msg2oexmhm -> DataMapInfo . mmi , &
mmiStaticInfoLogging ) ; rtwCAPI_SetPath ( msg2oexmhm -> DataMapInfo . mmi ,
( NULL ) ) ; rtwCAPI_SetFullPath ( msg2oexmhm -> DataMapInfo . mmi , ( NULL )
) ; rtwCAPI_SetInstanceLoggingInfo ( msg2oexmhm -> DataMapInfo . mmi , &
msg2oexmhm -> DataMapInfo . mmiLogInstanceInfo ) ; rtwCAPI_SetChildMMIArray ( msg2oexmhm -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArrayLen ( msg2oexmhm -> DataMapInfo . mmi , 0 ) ; TransmissionController_InitializeSystemRan ( msg2oexmhm , msg2oexmhm -> DataMapInfo . systemRan , localDW , msg2oexmhm -> DataMapInfo . systemTid , sysRanPtr , contextTid ) ; rtwCAPI_SetSystemRan ( msg2oexmhm -> DataMapInfo . mmi , msg2oexmhm -> DataMapInfo . systemRan ) ; rtwCAPI_SetSystemTid ( msg2oexmhm -> DataMapInfo . mmi , msg2oexmhm -> DataMapInfo . systemTid ) ; rtwCAPI_SetGlobalTIDMap ( msg2oexmhm -> DataMapInfo . mmi , & msg2oexmhm -> Timing . mdlref_GlobalTID [ 0 ] ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void TransmissionController_host_InitializeDataMapInfo ( TransmissionController_host_DataMapInfo_T * dataMap , const char * path ) { rtwCAPI_SetVersion ( dataMap -> mmi , 1 ) ; rtwCAPI_SetStaticMap ( dataMap -> mmi , & mmiStatic ) ; rtwCAPI_SetDataAddressMap ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetVarDimsAddressMap ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetPath ( dataMap -> mmi , path ) ; rtwCAPI_SetFullPath ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArray ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArrayLen ( dataMap -> mmi , 0 ) ; }
#ifdef __cplusplus
}
#endif
#endif
