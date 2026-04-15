#include "SimpleECU.h"
#include "rtwtypes.h"
#include "SimpleECU_private.h"
#include "SimpleECU_capi.h"
#include "look1_binlcpw.h"
static RegMdlInfo rtMdlInfo_SimpleECU [ 40 ] = { { "gdwzb1w12tj" ,
MDL_INFO_NAME_MDLREF_DWORK , 0 , - 1 , ( void * ) "SimpleECU" } , {
"ozcrl5lx51" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"SimpleECU" } , { "ous15bpw34" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 ,
( void * ) "SimpleECU" } , { "bb2mvg52pz" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT
, 0 , - 1 , ( void * ) "SimpleECU" } , { "kojkkh5ttb" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "SimpleECU" } , {
"lhagwna21c" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"SimpleECU" } , { "hvxdsols0i" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 ,
( void * ) "SimpleECU" } , { "dsfarfpj14" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT
, 0 , - 1 , ( void * ) "SimpleECU" } , { "a5byii4gq0" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "SimpleECU" } , {
"ec4vvgkxef" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"SimpleECU" } , { "d4qiw13n1s" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 ,
( void * ) "SimpleECU" } , { "akvak34ed4" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT
, 0 , - 1 , ( void * ) "SimpleECU" } , { "lnfrwp3yuw" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "SimpleECU" } , {
"gjojfccbqg" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"SimpleECU" } , { "eauk5datlm" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 ,
( void * ) "SimpleECU" } , { "npdtakcntc" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT
, 0 , - 1 , ( void * ) "SimpleECU" } , { "eybitwv3ad" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "SimpleECU" } , {
"hpdbk3e53r" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"SimpleECU" } , { "ls4iwfjhmk" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 ,
( void * ) "SimpleECU" } , { "fgxsnqfu34" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT
, 0 , - 1 , ( void * ) "SimpleECU" } , { "e2ixfdkusu" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "SimpleECU" } , {
"SimpleECU" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , 0 , ( NULL ) } , {
"kn3ldh3fze" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"SimpleECU" } , { "odvzwogg5bn" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1
, ( void * ) "SimpleECU" } , { "oat5daxizt" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "SimpleECU" } , {
"inrj4312kz" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"SimpleECU" } , { "mr_SimpleECU_GetSimStateDisallowedBlocks" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "SimpleECU" } , {
"mr_SimpleECU_extractBitFieldFromCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "SimpleECU" } , {
"mr_SimpleECU_cacheBitFieldToCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "SimpleECU" } , {
"mr_SimpleECU_restoreDataFromMxArrayWithOffset" , MDL_INFO_ID_MODEL_FCN_NAME
, 0 , - 1 , ( void * ) "SimpleECU" } , {
"mr_SimpleECU_cacheDataToMxArrayWithOffset" , MDL_INFO_ID_MODEL_FCN_NAME , 0
, - 1 , ( void * ) "SimpleECU" } , {
"mr_SimpleECU_extractBitFieldFromMxArray" , MDL_INFO_ID_MODEL_FCN_NAME , 0 ,
- 1 , ( void * ) "SimpleECU" } , { "mr_SimpleECU_cacheBitFieldToMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "SimpleECU" } , {
"mr_SimpleECU_restoreDataFromMxArray" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1
, ( void * ) "SimpleECU" } , { "mr_SimpleECU_cacheDataAsMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "SimpleECU" } , {
"mr_SimpleECU_RegisterSimStateChecksum" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , -
1 , ( void * ) "SimpleECU" } , { "mr_SimpleECU_SetDWork" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "SimpleECU" } , {
"mr_SimpleECU_GetDWork" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * )
"SimpleECU" } , { "SimpleECU.h" , MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( NULL
) } , { "SimpleECU.c" , MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( void * )
"SimpleECU" } } ; odvzwogg5bn odvzwogg5b = { 0.0 , { 1.0 , 0.0 } , { 1.0 ,
1.01 } , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 ,
0.0 , 1.0 , 0.0 , 0.0 , 0.0 } ; void hpdbk3e53r ( real_T * h0uquagyg0 ,
real_T * m1h2iw23s1 , real_T * hjbjee1qf3 , real_T * p1qwr4kg4w , real_T *
djdsbxjjcs , real_T * aakgidicwv , real_T * m0ncuwdtjo , real_T * jcvlomlnhi
, real_T * l45dkyhzbm , real_T * hvpmgazb03 , real_T * njjcdejhvz , real_T *
j35keouu1s , real_T * jn0muy2z31 , real_T * j4qz2y4at2 , real_T * anrpwz2hpc
, real_T * eac1s300pn ) { * h0uquagyg0 = odvzwogg5b . P_4 ; * m1h2iw23s1 =
odvzwogg5b . P_5 ; * hjbjee1qf3 = odvzwogg5b . P_6 ; * p1qwr4kg4w =
odvzwogg5b . P_7 ; * djdsbxjjcs = odvzwogg5b . P_8 ; * aakgidicwv =
odvzwogg5b . P_9 ; * m0ncuwdtjo = odvzwogg5b . P_10 ; * jcvlomlnhi =
odvzwogg5b . P_11 ; * l45dkyhzbm = odvzwogg5b . P_12 ; * hvpmgazb03 =
odvzwogg5b . P_13 ; * njjcdejhvz = odvzwogg5b . P_14 ; * j35keouu1s =
odvzwogg5b . P_15 ; * jn0muy2z31 = odvzwogg5b . P_16 ; * j4qz2y4at2 =
odvzwogg5b . P_17 ; * anrpwz2hpc = odvzwogg5b . P_18 ; * eac1s300pn =
odvzwogg5b . P_19 ; } void fgxsnqfu34 ( real_T * h0uquagyg0 , real_T *
m1h2iw23s1 , real_T * hjbjee1qf3 , real_T * p1qwr4kg4w , real_T * djdsbxjjcs
, real_T * aakgidicwv , real_T * m0ncuwdtjo , real_T * jcvlomlnhi , real_T *
l45dkyhzbm , real_T * hvpmgazb03 , real_T * njjcdejhvz , real_T * j35keouu1s
, real_T * jn0muy2z31 , real_T * j4qz2y4at2 , real_T * anrpwz2hpc , real_T *
eac1s300pn ) { * h0uquagyg0 = odvzwogg5b . P_4 ; * m1h2iw23s1 = odvzwogg5b .
P_5 ; * hjbjee1qf3 = odvzwogg5b . P_6 ; * p1qwr4kg4w = odvzwogg5b . P_7 ; *
djdsbxjjcs = odvzwogg5b . P_8 ; * aakgidicwv = odvzwogg5b . P_9 ; *
m0ncuwdtjo = odvzwogg5b . P_10 ; * jcvlomlnhi = odvzwogg5b . P_11 ; *
l45dkyhzbm = odvzwogg5b . P_12 ; * hvpmgazb03 = odvzwogg5b . P_13 ; *
njjcdejhvz = odvzwogg5b . P_14 ; * j35keouu1s = odvzwogg5b . P_15 ; *
jn0muy2z31 = odvzwogg5b . P_16 ; * j4qz2y4at2 = odvzwogg5b . P_17 ; *
anrpwz2hpc = odvzwogg5b . P_18 ; * eac1s300pn = odvzwogg5b . P_19 ; } void
SimpleECU ( const real_T * ipvvddyov3 , const real_T * av2xyxaxr3 , const
boolean_T * dewtouimsp , real_T * h0uquagyg0 , real_T * m1h2iw23s1 , real_T *
hjbjee1qf3 , real_T * p1qwr4kg4w , real_T * djdsbxjjcs , real_T * aakgidicwv
, real_T * m0ncuwdtjo , real_T * jcvlomlnhi , real_T * l45dkyhzbm , real_T *
hvpmgazb03 , real_T * njjcdejhvz , real_T * j35keouu1s , real_T * jn0muy2z31
, real_T * j4qz2y4at2 , real_T * ouv21new2s , real_T * anrpwz2hpc , real_T *
eac1s300pn ) { * h0uquagyg0 = odvzwogg5b . P_4 ; * m1h2iw23s1 = odvzwogg5b .
P_5 ; * hjbjee1qf3 = odvzwogg5b . P_6 ; * p1qwr4kg4w = odvzwogg5b . P_7 ; *
djdsbxjjcs = odvzwogg5b . P_8 ; * aakgidicwv = odvzwogg5b . P_9 ; *
m0ncuwdtjo = odvzwogg5b . P_10 ; * jcvlomlnhi = odvzwogg5b . P_11 ; *
l45dkyhzbm = odvzwogg5b . P_12 ; * hvpmgazb03 = odvzwogg5b . P_13 ; *
njjcdejhvz = odvzwogg5b . P_14 ; * j35keouu1s = odvzwogg5b . P_15 ; *
jn0muy2z31 = odvzwogg5b . P_16 ; * j4qz2y4at2 = odvzwogg5b . P_17 ; if ( *
dewtouimsp ) { * ouv21new2s = odvzwogg5b . P_1 ; } else { * ouv21new2s =
look1_binlcpw ( * av2xyxaxr3 / rtP__CtrlEcuRevLim2f2SiEngineController_sldd_
, odvzwogg5b . P_3 , odvzwogg5b . P_2 , 1U ) * * ipvvddyov3 ; } * anrpwz2hpc
= odvzwogg5b . P_18 ; * eac1s300pn = odvzwogg5b . P_19 ; } void npdtakcntc ( inrj4312kz * const e44s5qfgut ) { if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( e44s5qfgut -> _mdlRefSfcnS , "SimpleECU" , "SIMSTATUS_TERMINATING_MODELREF_ACCEL_EVENT" ) ; } } void ls4iwfjhmk ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , inrj4312kz * const e44s5qfgut , void * sysRanPtr , int contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) { ( void ) memset ( ( void * ) e44s5qfgut , 0 , sizeof ( inrj4312kz ) ) ; e44s5qfgut -> Timing . mdlref_GlobalTID [ 0 ] = mdlref_TID0 ; e44s5qfgut -> _mdlRefSfcnS = ( _mdlRefSfcnS ) ; if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( e44s5qfgut -> _mdlRefSfcnS , "SimpleECU" , "START_OF_SIM_MODEL_MODELREF_ACCEL_EVENT" ) ; } SimpleECU_InitializeDataMapInfo ( e44s5qfgut , sysRanPtr , contextTid ) ; if ( ( rt_ParentMMI != ( NULL ) ) && ( rt_ChildPath != ( NULL ) ) ) { rtwCAPI_SetChildMMI ( * rt_ParentMMI , rt_ChildMMIIdx , & ( e44s5qfgut -> DataMapInfo . mmi ) ) ; rtwCAPI_SetPath ( e44s5qfgut -> DataMapInfo . mmi , rt_ChildPath ) ; rtwCAPI_MMISetContStateStartIndex ( e44s5qfgut -> DataMapInfo . mmi , rt_CSTATEIdx ) ; } } void mr_SimpleECU_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) { * retVal = 0 ; { boolean_T regSubmodelsMdlinfo = false ; ssGetRegSubmodelsMdlinfo ( mdlRefSfcnS , & regSubmodelsMdlinfo ) ; if ( regSubmodelsMdlinfo ) { } } * retVal = 0 ; ssRegModelRefMdlInfo ( mdlRefSfcnS , modelName , rtMdlInfo_SimpleECU , 40 ) ; * retVal = 1 ; } static void mr_SimpleECU_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) ; static void mr_SimpleECU_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) { mxArray * newArray = mxCreateUninitNumericMatrix ( ( size_t ) 1 , numBytes , mxUINT8_CLASS , mxREAL ) ; memcpy ( ( uint8_T * ) mxGetData ( newArray ) , ( const uint8_T * ) srcData , numBytes ) ; mxSetFieldByNumber ( destArray , i , j , newArray ) ; } static void mr_SimpleECU_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) ; static void mr_SimpleECU_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) { memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) , numBytes ) ; } static void mr_SimpleECU_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) ; static void mr_SimpleECU_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) { mxSetFieldByNumber ( destArray , i , j , mxCreateDoubleScalar ( ( real_T ) bitVal ) ) ; } static uint_T mr_SimpleECU_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) ; static uint_T mr_SimpleECU_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) { const uint_T varVal = ( uint_T ) mxGetScalar ( mxGetFieldByNumber ( srcArray , i , j ) ) ; return varVal & ( ( 1u << numBits ) - 1u ) ; } static void mr_SimpleECU_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) ; static void mr_SimpleECU_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) { uint8_T * varData = ( uint8_T * ) mxGetData ( mxGetFieldByNumber ( destArray , i , j ) ) ; memcpy ( ( uint8_T * ) & varData [ offset * numBytes ] , ( const uint8_T * ) srcData , numBytes ) ; } static void mr_SimpleECU_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) ; static void mr_SimpleECU_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) { const uint8_T * varData = ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) ; memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) & varData [ offset * numBytes ] , numBytes ) ; } static void mr_SimpleECU_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) ; static void mr_SimpleECU_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) { mxSetCell ( mxGetFieldByNumber ( destArray , i , j ) , offset , mxCreateDoubleScalar ( ( real_T ) fieldVal ) ) ; } static uint_T mr_SimpleECU_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) ; static uint_T mr_SimpleECU_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) { const uint_T fieldVal = ( uint_T ) mxGetScalar ( mxGetCell ( mxGetFieldByNumber ( srcArray , i , j ) , offset ) ) ; return fieldVal & ( ( 1u << numBits ) - 1u ) ; } mxArray * mr_SimpleECU_GetDWork ( const gdwzb1w12tj * mdlrefDW ) { ( void ) mdlrefDW ; return ( NULL ) ; } void mr_SimpleECU_SetDWork ( gdwzb1w12tj * mdlrefDW , const mxArray * ssDW ) { ( void ) ssDW ; ( void ) mdlrefDW ; } void mr_SimpleECU_RegisterSimStateChecksum ( SimStruct * S ) { const uint32_T chksum [ 4 ] = { 4111111339U , 2630141003U , 1174318116U , 3657840603U , } ; slmrModelRefRegisterSimStateChecksum ( S , "SimpleECU" , & chksum [ 0 ] ) ; } mxArray * mr_SimpleECU_GetSimStateDisallowedBlocks ( ) { return ( NULL ) ; }
#if defined(_MSC_VER)
#pragma warning(disable: 4505) //unreferenced local function has been removed
#endif
