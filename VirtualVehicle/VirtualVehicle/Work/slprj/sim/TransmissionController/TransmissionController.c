#include "TransmissionController.h"
#include "rtwtypes.h"
#include "TransmissionController_private.h"
#include "mwmathutil.h"
#include "TransmissionController_capi.h"
#include "look1_binlcpw.h"
#include "plook_binc.h"
#include "intrp3d_l_pw.h"
#include "look1_binlcapw.h"
#include <string.h>
#define cxpbakdhrp ((uint8_T)3U)
#define ehvo05zd12 ((uint8_T)2U)
#define fl10bhbe4i ((uint8_T)6U)
#define fv5jrookmk ((uint8_T)4U)
#define fxdnyiu0qi ((uint8_T)2U)
#define gfsyqyahmv ((uint8_T)1U)
#define hv1ngpatlt ((uint8_T)4U)
#define ibr2p5f2yw ((uint8_T)1U)
#define kry4jbkzwq ((uint8_T)5U)
#define mlcdkbx3rv ((uint8_T)3U)
#define ni5p0qit5r ((uint8_T)0U)
static RegMdlInfo rtMdlInfo_TransmissionController [ 64 ] = { { "ocqtzhujgh"
, MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "j35homo4p4" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "o4uw4qs4iz" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "glzig3tuqj" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "ardt3kkxyv" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "eqrgndpenl" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "bi30vrmcoh" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "ohln4knokr" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "guy1iy0lgm" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "k04tyibnqj" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "g2mn4pykba" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "o4d1nv4zcx" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "oromh3n2ow" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "dosld0bzlxb" , MDL_INFO_NAME_MDLREF_DWORK , 0
, - 1 , ( void * ) "TransmissionController" } , { "bfqaszcaxh" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "e4gvjuqqkn" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "gooic04uzd" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "hybes53t2z" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "afvbs1mlwh" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "e03oz4jfst" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "fq3fkk2t4v" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "ibapq0aaxv" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "pzip0b4x1b" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "f0ysncsa3z" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "o3ciyeksc1" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "ikk0kw0e3s" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "esksn11ftz" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "beezh30zb2" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "g2lk0g2ith" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "lqm01f5rw2" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "gnmf0hpv3l" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "bbtamnazst" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "asexx13y03" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "bi3mvnnhxb" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "pffr22s23c" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "j04qfjz3hc" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "hqni12ozpk" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "n0wbauu0k0" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "h50zxp153c" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "cfiknrekqn" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "nsopsaho5o" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "gtobkiuk51" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "h35o3lx45o" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "ociqxrok4u" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "c1rvhddg4t" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , 0 , ( void * ) "LugCheck" } , {
"TransmissionController" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , 0 , ( NULL
) } , { "iahfs2nsni" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void *
) "TransmissionController" } , { "luigmi4qrbs" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "huwnotjefz" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , { "prjutdmklf" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"TransmissionController" } , {
"mr_TransmissionController_GetSimStateDisallowedBlocks" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "TransmissionController" }
, { "mr_TransmissionController_extractBitFieldFromCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "TransmissionController" }
, { "mr_TransmissionController_cacheBitFieldToCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "TransmissionController" }
, { "mr_TransmissionController_restoreDataFromMxArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "TransmissionController" }
, { "mr_TransmissionController_cacheDataToMxArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "TransmissionController" }
, { "mr_TransmissionController_extractBitFieldFromMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "TransmissionController" }
, { "mr_TransmissionController_cacheBitFieldToMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "TransmissionController" }
, { "mr_TransmissionController_restoreDataFromMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "TransmissionController" }
, { "mr_TransmissionController_cacheDataAsMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "TransmissionController" }
, { "mr_TransmissionController_RegisterSimStateChecksum" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "TransmissionController" }
, { "mr_TransmissionController_SetDWork" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , -
1 , ( void * ) "TransmissionController" } , {
"mr_TransmissionController_GetDWork" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 ,
( void * ) "TransmissionController" } , { "TransmissionController.h" ,
MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( NULL ) } , { "TransmissionController.c"
, MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( void * ) "TransmissionController" } }
; luigmi4qrbs luigmi4qrb = { { 1.0 , 2.0 } , { 2.0 , 3.0 , 4.0 , 5.0 , 6.0 ,
7.0 , 8.0 } , { 1.0 , 2.0 } , { 1.0 , 2.0 , 3.0 , 4.0 , 5.0 , 6.0 , 7.0 } ,
0.0 , { 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 } , { 0.0 , 0.0 , 0.0 , 0.0 }
, 0.0 , { 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 } , { 0.0 , 0.0 , 0.0 , 0.0
} , 8.0 , 120.0 , 1.0 , { 3U , 6U , 1U } , { 1U , 4U , 28U } , { 3U , 6U , 1U
} , { 1U , 4U , 28U } , { 0.0 , { 0.0 , 1.0 } , { 0.0 , 200.0 } , 0.75 , 0.25
, 1.0 , 0.0 } } ; void nsopsaho5o ( oromh3n2ow * localB , g2mn4pykba * localP
) { localB -> ctbloljy0k = localP -> P_0 ; } void c1rvhddg4t ( real_T
h5siubfvdv , real_T ibnrla4v0g , oromh3n2ow * localB , o4d1nv4zcx * localDW ,
g2mn4pykba * localP ) { real_T drljq1pl01 ; drljq1pl01 = look1_binlcpw ( ibnrla4v0g - h5siubfvdv , localP -> P_2 , localP -> P_1 , 1U ) ; localDW -> jdn5p4hqsj = ( ( drljq1pl01 >= localP -> P_3 ) || ( ( ! ( drljq1pl01 <= localP -> P_4 ) ) && localDW -> jdn5p4hqsj ) ) ; if ( localDW -> jdn5p4hqsj ) { localB -> ctbloljy0k = localP -> P_5 ; } else { localB -> ctbloljy0k = localP -> P_6 ; } localDW -> czt2emm2gr = 4 ; } void bi3mvnnhxb ( real_T * bmkiryd24s , esksn11ftz * localB , ikk0kw0e3s * localDW ) { localDW -> b5yfuhwx0t = 0U ; localDW -> av2wa1pd3n = 0U ; * bmkiryd24s = 0.0 ; localDW -> drgyfxwvr2 = 0.0 ; localDW -> icidstynwv = 0.0 ; localDW -> dv1nobpvdc = 0U ; localDW -> a05tnifdjv = ni5p0qit5r ; localDW -> oceatsapa5 = ni5p0qit5r ; nsopsaho5o ( & localB -> c1rvhddg4tu , & luigmi4qrb . c1rvhddg4tu ) ; } void asexx13y03 ( real_T * bmkiryd24s , ikk0kw0e3s * localDW ) { localDW -> b5yfuhwx0t = 0U ; localDW -> av2wa1pd3n = 0U ; * bmkiryd24s = 0.0 ; localDW -> drgyfxwvr2 = 0.0 ; localDW -> icidstynwv = 0.0 ; localDW -> dv1nobpvdc = 0U ; localDW -> a05tnifdjv = ni5p0qit5r ; localDW -> oceatsapa5 = ni5p0qit5r ; } void TransmissionController ( const real_T * ivuhyrz123 , const real_T * njl0yujf4t , const real_T * prevjjccrx , const real_T * arho0tmwfk , const real_T * lqpwdnlioa , const boolean_T * hazcyglkvp , real_T * bmkiryd24s , esksn11ftz * localB , ikk0kw0e3s * localDW ) { real_T fractions [ 3 ] ; real_T fractions_p [ 3 ] ; real_T i33oh4ttuj_p ; real_T o0bsmza42m_p ; real_T o3aycuewqj ; real_T pecqkzechg ; uint32_T bpIndices [ 3 ] ; uint32_T bpIndices_p [ 3 ] ; boolean_T tmp ; o3aycuewqj = 3.5999999999999996 * * prevjjccrx ; pecqkzechg = 9.5492965855137211 * * arho0tmwfk ; if ( localDW -> b5yfuhwx0t < MAX_uint32_T ) { localDW -> b5yfuhwx0t ++ ; } if ( localDW -> av2wa1pd3n < MAX_uint32_T ) { localDW -> av2wa1pd3n ++ ; } if ( localDW -> dv1nobpvdc == 0 ) { localDW -> dv1nobpvdc = 1U ; * bmkiryd24s = rtP__CtrlTcuInitGear2f2Drivetrain_sldd_ ; if ( rtP__CtrlTcuInitGear2f2Drivetrain_sldd_ == 80.0 ) { localDW -> a05tnifdjv = mlcdkbx3rv ; * bmkiryd24s = 80.0 ; localDW -> icidstynwv = 0.0 ; } else if ( rtP__CtrlTcuInitGear2f2Drivetrain_sldd_ < 0.0 ) { localDW -> a05tnifdjv = fv5jrookmk ; * bmkiryd24s = - 1.0 ; localDW -> icidstynwv = 0.0 ; } else if ( rtP__CtrlTcuInitGear2f2Drivetrain_sldd_ == 0.0 ) { localDW -> b5yfuhwx0t = 0U ; localDW -> a05tnifdjv = ehvo05zd12 ; * bmkiryd24s = 0.0 ; localDW -> icidstynwv = 0.0 ; } else { localDW -> a05tnifdjv = gfsyqyahmv ; } if ( localDW -> icidstynwv == 1.0 ) { localDW -> oceatsapa5 = cxpbakdhrp ; } else { localDW -> oceatsapa5 = fxdnyiu0qi ; } } else { switch ( localDW -> a05tnifdjv ) { case gfsyqyahmv : if ( ( ( localDW -> drgyfxwvr2 != 0.0 ) && ( * ivuhyrz123 < 0.01 ) && ( ( * bmkiryd24s == 1.0 ) || ( * bmkiryd24s == 2.0 ) ) ) || ( * hazcyglkvp ) ) { localDW -> b5yfuhwx0t = 0U ; localDW -> a05tnifdjv = ehvo05zd12 ; * bmkiryd24s = 0.0 ; localDW -> icidstynwv = 0.0 ; } else { c1rvhddg4t ( pecqkzechg , rtP__CtrlTcuLugSpd2f2Drivetrain_sldd_ , & localB -> c1rvhddg4tu , & localDW -> c1rvhddg4tu , & luigmi4qrb . c1rvhddg4tu ) ; localDW -> drgyfxwvr2 = localB -> c1rvhddg4tu . ctbloljy0k ; localDW -> icidstynwv = 1.0 ; } break ; case ehvo05zd12 : tmp = ! * hazcyglkvp ; if ( ( * ivuhyrz123 >= 0.1 ) && tmp && ( pecqkzechg >= rtP__CtrlTcuMinUpSpd2f2Drivetrain_sldd_ ) ) { * bmkiryd24s = 1.0 ; localDW -> a05tnifdjv = gfsyqyahmv ; } else if ( ( * lqpwdnlioa == - 1.0 ) && tmp ) { localDW -> a05tnifdjv = fv5jrookmk ; * bmkiryd24s = - 1.0 ; localDW -> icidstynwv = 0.0 ; } else if ( ( localDW -> b5yfuhwx0t >= ( uint32_T ) muDoubleScalarCeil ( luigmi4qrb . P_20 * 100.0 ) ) && ( ( * lqpwdnlioa == 80.0 ) && ( * njl0yujf4t > 0.5 ) && ( muDoubleScalarAbs ( o3aycuewqj ) <= 1.0 ) && ( * ivuhyrz123 > 0.0 ) && ( * ivuhyrz123 <= 0.05 ) ) ) { localDW -> a05tnifdjv = mlcdkbx3rv ; * bmkiryd24s = 80.0 ; localDW -> icidstynwv = 0.0 ; } else { c1rvhddg4t ( pecqkzechg , rtP__CtrlTcuLugSpd2f2Drivetrain_sldd_ , & localB -> c1rvhddg4tu , & localDW -> c1rvhddg4tu , & luigmi4qrb . c1rvhddg4tu ) ; localDW -> drgyfxwvr2 = localB -> c1rvhddg4tu . ctbloljy0k ; } break ; case mlcdkbx3rv : if ( ( muDoubleScalarAbs ( o3aycuewqj ) <= 1.0 ) && ( * ivuhyrz123 > 0.0 ) ) { localDW -> b5yfuhwx0t = 0U ; localDW -> a05tnifdjv = ehvo05zd12 ; * bmkiryd24s = 0.0 ; localDW -> icidstynwv = 0.0 ; } break ; default : if ( ( * lqpwdnlioa == 1.0 ) || ( * hazcyglkvp ) ) { localDW -> b5yfuhwx0t = 0U ; localDW -> a05tnifdjv = ehvo05zd12 ; * bmkiryd24s = 0.0 ; localDW -> icidstynwv = 0.0 ; } break ; } bpIndices [ 0U ] = plook_binc ( * ivuhyrz123 , rtP__CtrlTcuUpLdBpt2f2Drivetrain_sldd_ , 3U , & o0bsmza42m_p ) ; fractions [ 0U ] = o0bsmza42m_p ; bpIndices [ 1U ] = plook_binc ( * bmkiryd24s , luigmi4qrb . P_12 , 6U , & o0bsmza42m_p ) ; fractions [ 1U ] = o0bsmza42m_p ; bpIndices [ 2U ] = plook_binc ( luigmi4qrb . P_21 , luigmi4qrb . P_11 , 1U , & o0bsmza42m_p ) ; fractions [ 2U ] = o0bsmza42m_p ; i33oh4ttuj_p = ( intrp3d_l_pw ( bpIndices , fractions , rtP__CtrlTcuUpShSpd2f2Drivetrain_sldd_ , luigmi4qrb . P_23 ) + look1_binlcapw ( * bmkiryd24s , luigmi4qrb . P_12 , luigmi4qrb . P_14 , 6U ) ) + look1_binlcapw ( * ivuhyrz123 , rtP__CtrlTcuUpLdBpt2f2Drivetrain_sldd_ , luigmi4qrb . P_15 , 3U ) ; localDW -> lyaq3qnmfw = 4 ; bpIndices_p [ 0U ] = plook_binc ( * ivuhyrz123 , rtP__CtrlTcuDnLdBpt2f2Drivetrain_sldd_ , 3U , & o0bsmza42m_p ) ; fractions_p [ 0U ] = o0bsmza42m_p ; bpIndices_p [ 1U ] = plook_binc ( * bmkiryd24s , luigmi4qrb . P_10 , 6U , & o0bsmza42m_p ) ; fractions_p [ 1U ] = o0bsmza42m_p ; bpIndices_p [ 2U ] = plook_binc ( luigmi4qrb . P_21 , luigmi4qrb . P_9 , 1U , & o0bsmza42m_p ) ; fractions_p [ 2U ] = o0bsmza42m_p ; o0bsmza42m_p = ( intrp3d_l_pw ( bpIndices_p , fractions_p , rtP__CtrlTcuDnShSpd2f2Drivetrain_sldd_ , luigmi4qrb . P_25 ) + look1_binlcapw ( * bmkiryd24s , luigmi4qrb . P_10 , luigmi4qrb . P_17 , 6U ) ) + look1_binlcapw ( * ivuhyrz123 , rtP__CtrlTcuDnLdBpt2f2Drivetrain_sldd_ , luigmi4qrb . P_18 , 3U ) ; localDW -> fe5mmldjmq = 4 ; switch ( localDW -> oceatsapa5 ) { case ibr2p5f2yw : if ( ( ( o3aycuewqj >= o0bsmza42m_p ) && ( ! ( localDW -> drgyfxwvr2 != 0.0 ) ) ) || ( * bmkiryd24s < 1.0 ) ) { localDW -> oceatsapa5 = cxpbakdhrp ; } else if ( * bmkiryd24s > 1.0 ) { ( * bmkiryd24s ) -- ; localDW -> oceatsapa5 = cxpbakdhrp ; } break ; case fxdnyiu0qi : if ( localDW -> icidstynwv == 1.0 ) { localDW -> oceatsapa5 = cxpbakdhrp ; } break ; case cxpbakdhrp : if ( ( ( o3aycuewqj < o0bsmza42m_p ) || ( localDW -> drgyfxwvr2 != 0.0 ) ) && ( * bmkiryd24s > 1.0 ) ) { localDW -> av2wa1pd3n = 0U ; localDW -> oceatsapa5 = kry4jbkzwq ; } else if ( ( o3aycuewqj > i33oh4ttuj_p ) && ( * bmkiryd24s <= luigmi4qrb . P_19 - 1.0 ) && ( pecqkzechg >= rtP__CtrlTcuMinUpSpd2f2Drivetrain_sldd_ ) ) { localDW -> av2wa1pd3n = 0U ; localDW -> oceatsapa5 = fl10bhbe4i ; } else if ( localDW -> icidstynwv != 1.0 ) { localDW -> oceatsapa5 = fxdnyiu0qi ; } break ; case hv1ngpatlt : if ( ( ! ( o3aycuewqj <= i33oh4ttuj_p ) ) && ( ! ( localDW -> drgyfxwvr2 != 0.0 ) ) ) { ( * bmkiryd24s ) ++ ; } localDW -> oceatsapa5 = cxpbakdhrp ; break ; case kry4jbkzwq : if ( ( localDW -> av2wa1pd3n >= ( uint32_T ) muDoubleScalarCeil ( rtP__CtrlTcuTmDnSh2f2Drivetrain_sldd_ * 100.0 ) ) && ( ( o3aycuewqj < o0bsmza42m_p - 2.0 ) || ( localDW -> drgyfxwvr2 != 0.0 ) ) ) { localDW -> oceatsapa5 = ibr2p5f2yw ; } else if ( ( o3aycuewqj >= o0bsmza42m_p ) && ( ! ( localDW -> drgyfxwvr2 != 0.0 ) ) ) { localDW -> oceatsapa5 = cxpbakdhrp ; } break ; default : if ( ( localDW -> av2wa1pd3n >= ( uint32_T ) muDoubleScalarCeil ( rtP__CtrlTcuTmUpSh2f2Drivetrain_sldd_ * 100.0 ) ) && ( o3aycuewqj > i33oh4ttuj_p ) ) { localDW -> oceatsapa5 = hv1ngpatlt ; } else if ( ( o3aycuewqj <= i33oh4ttuj_p ) || ( localDW -> drgyfxwvr2 != 0.0 ) ) { localDW -> oceatsapa5 = cxpbakdhrp ; } break ; } } } void g2lk0g2ith ( prjutdmklf * const msg2oexmhm ) { if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( msg2oexmhm -> _mdlRefSfcnS , "TransmissionController" , "SIMSTATUS_TERMINATING_MODELREF_ACCEL_EVENT" ) ; } } void pffr22s23c ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , prjutdmklf * const msg2oexmhm , esksn11ftz * localB , ikk0kw0e3s * localDW , void * sysRanPtr , int contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) { ( void ) memset ( ( void * ) msg2oexmhm , 0 , sizeof ( prjutdmklf ) ) ; msg2oexmhm -> Timing . mdlref_GlobalTID [ 0 ] = mdlref_TID0 ; msg2oexmhm -> Timing . mdlref_GlobalTID [ 1 ] = mdlref_TID1 ; msg2oexmhm -> _mdlRefSfcnS = ( _mdlRefSfcnS ) ; if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( msg2oexmhm -> _mdlRefSfcnS , "TransmissionController" , "START_OF_SIM_MODEL_MODELREF_ACCEL_EVENT" ) ; } { localB -> c1rvhddg4tu . ctbloljy0k = 0.0 ; } ( void ) memset ( ( void * ) localDW , 0 , sizeof ( ikk0kw0e3s ) ) ; localDW -> drgyfxwvr2 = 0.0 ; localDW -> icidstynwv = 0.0 ; TransmissionController_InitializeDataMapInfo ( msg2oexmhm , localDW , sysRanPtr , contextTid ) ; if ( ( rt_ParentMMI != ( NULL ) ) && ( rt_ChildPath != ( NULL ) ) ) { rtwCAPI_SetChildMMI ( * rt_ParentMMI , rt_ChildMMIIdx , & ( msg2oexmhm -> DataMapInfo . mmi ) ) ; rtwCAPI_SetPath ( msg2oexmhm -> DataMapInfo . mmi , rt_ChildPath ) ; rtwCAPI_MMISetContStateStartIndex ( msg2oexmhm -> DataMapInfo . mmi , rt_CSTATEIdx ) ; } } void mr_TransmissionController_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) { * retVal = 0 ; { boolean_T regSubmodelsMdlinfo = false ; ssGetRegSubmodelsMdlinfo ( mdlRefSfcnS , & regSubmodelsMdlinfo ) ; if ( regSubmodelsMdlinfo ) { } } * retVal = 0 ; ssRegModelRefMdlInfo ( mdlRefSfcnS , modelName , rtMdlInfo_TransmissionController , 64 ) ; * retVal = 1 ; } static void mr_TransmissionController_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) ; static void mr_TransmissionController_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) { mxArray * newArray = mxCreateUninitNumericMatrix ( ( size_t ) 1 , numBytes , mxUINT8_CLASS , mxREAL ) ; memcpy ( ( uint8_T * ) mxGetData ( newArray ) , ( const uint8_T * ) srcData , numBytes ) ; mxSetFieldByNumber ( destArray , i , j , newArray ) ; } static void mr_TransmissionController_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) ; static void mr_TransmissionController_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) { memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) , numBytes ) ; } static void mr_TransmissionController_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) ; static void mr_TransmissionController_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) { mxSetFieldByNumber ( destArray , i , j , mxCreateDoubleScalar ( ( real_T ) bitVal ) ) ; } static uint_T mr_TransmissionController_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) ; static uint_T mr_TransmissionController_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) { const uint_T varVal = ( uint_T ) mxGetScalar ( mxGetFieldByNumber ( srcArray , i , j ) ) ; return varVal & ( ( 1u << numBits ) - 1u ) ; } static void mr_TransmissionController_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) ; static void mr_TransmissionController_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) { uint8_T * varData = ( uint8_T * ) mxGetData ( mxGetFieldByNumber ( destArray , i , j ) ) ; memcpy ( ( uint8_T * ) & varData [ offset * numBytes ] , ( const uint8_T * ) srcData , numBytes ) ; } static void mr_TransmissionController_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) ; static void mr_TransmissionController_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) { const uint8_T * varData = ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) ; memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) & varData [ offset * numBytes ] , numBytes ) ; } static void mr_TransmissionController_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) ; static void mr_TransmissionController_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) { mxSetCell ( mxGetFieldByNumber ( destArray , i , j ) , offset , mxCreateDoubleScalar ( ( real_T ) fieldVal ) ) ; } static uint_T mr_TransmissionController_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) ; static uint_T mr_TransmissionController_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) { const uint_T fieldVal = ( uint_T ) mxGetScalar ( mxGetCell ( mxGetFieldByNumber ( srcArray , i , j ) , offset ) ) ; return fieldVal & ( ( 1u << numBits ) - 1u ) ; } mxArray * mr_TransmissionController_GetDWork ( const dosld0bzlxb * mdlrefDW ) { static const char_T * ssDWFieldNames [ 3 ] = { "rtb" , "rtdw" , "NULL->rtzce" , } ; mxArray * ssDW = mxCreateStructMatrix ( 1 , 1 , 3 , ssDWFieldNames ) ; mr_TransmissionController_cacheDataAsMxArray ( ssDW , 0 , 0 , ( const void * ) & ( mdlrefDW -> rtb ) , sizeof ( mdlrefDW -> rtb ) ) ; { static const char_T * rtdwDataFieldNames [ 11 ] = { "mdlrefDW->rtdw.drgyfxwvr2" , "mdlrefDW->rtdw.icidstynwv" , "mdlrefDW->rtdw.b5yfuhwx0t" , "mdlrefDW->rtdw.av2wa1pd3n" , "mdlrefDW->rtdw.fe5mmldjmq" , "mdlrefDW->rtdw.lyaq3qnmfw" , "mdlrefDW->rtdw.dv1nobpvdc" , "mdlrefDW->rtdw.a05tnifdjv" , "mdlrefDW->rtdw.oceatsapa5" , "mdlrefDW->rtdw.c1rvhddg4tu.czt2emm2gr" , "mdlrefDW->rtdw.c1rvhddg4tu.jdn5p4hqsj" , } ; mxArray * rtdwData = mxCreateStructMatrix ( 1 , 1 , 11 , rtdwDataFieldNames ) ; mr_TransmissionController_cacheDataAsMxArray ( rtdwData , 0 , 0 , ( const void * ) & ( mdlrefDW -> rtdw . drgyfxwvr2 ) , sizeof ( mdlrefDW -> rtdw . drgyfxwvr2 ) ) ; mr_TransmissionController_cacheDataAsMxArray ( rtdwData , 0 , 1 , ( const void * ) & ( mdlrefDW -> rtdw . icidstynwv ) , sizeof ( mdlrefDW -> rtdw . icidstynwv ) ) ; mr_TransmissionController_cacheDataAsMxArray ( rtdwData , 0 , 2 , ( const void * ) & ( mdlrefDW -> rtdw . b5yfuhwx0t ) , sizeof ( mdlrefDW -> rtdw . b5yfuhwx0t ) ) ; mr_TransmissionController_cacheDataAsMxArray ( rtdwData , 0 , 3 , ( const void * ) & ( mdlrefDW -> rtdw . av2wa1pd3n ) , sizeof ( mdlrefDW -> rtdw . av2wa1pd3n ) ) ; mr_TransmissionController_cacheDataAsMxArray ( rtdwData , 0 , 4 , ( const void * ) & ( mdlrefDW -> rtdw . fe5mmldjmq ) , sizeof ( mdlrefDW -> rtdw . fe5mmldjmq ) ) ; mr_TransmissionController_cacheDataAsMxArray ( rtdwData , 0 , 5 , ( const void * ) & ( mdlrefDW -> rtdw . lyaq3qnmfw ) , sizeof ( mdlrefDW -> rtdw . lyaq3qnmfw ) ) ; mr_TransmissionController_cacheDataAsMxArray ( rtdwData , 0 , 6 , ( const void * ) & ( mdlrefDW -> rtdw . dv1nobpvdc ) , sizeof ( mdlrefDW -> rtdw . dv1nobpvdc ) ) ; mr_TransmissionController_cacheDataAsMxArray ( rtdwData , 0 , 7 , ( const void * ) & ( mdlrefDW -> rtdw . a05tnifdjv ) , sizeof ( mdlrefDW -> rtdw . a05tnifdjv ) ) ; mr_TransmissionController_cacheDataAsMxArray ( rtdwData , 0 , 8 , ( const void * ) & ( mdlrefDW -> rtdw . oceatsapa5 ) , sizeof ( mdlrefDW -> rtdw . oceatsapa5 ) ) ; mr_TransmissionController_cacheDataAsMxArray ( rtdwData , 0 , 9 , ( const void * ) & ( mdlrefDW -> rtdw . c1rvhddg4tu . czt2emm2gr ) , sizeof ( mdlrefDW -> rtdw . c1rvhddg4tu . czt2emm2gr ) ) ; mr_TransmissionController_cacheDataAsMxArray ( rtdwData , 0 , 10 , ( const void * ) & ( mdlrefDW -> rtdw . c1rvhddg4tu . jdn5p4hqsj ) , sizeof ( mdlrefDW -> rtdw . c1rvhddg4tu . jdn5p4hqsj ) ) ; mxSetFieldByNumber ( ssDW , 0 , 1 , rtdwData ) ; } ( void ) mdlrefDW ; return ssDW ; } void mr_TransmissionController_SetDWork ( dosld0bzlxb * mdlrefDW , const mxArray * ssDW ) { ( void ) ssDW ; ( void ) mdlrefDW ; mr_TransmissionController_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtb ) , ssDW , 0 , 0 , sizeof ( mdlrefDW -> rtb ) ) ; { const mxArray * rtdwData = mxGetFieldByNumber ( ssDW , 0 , 1 ) ; mr_TransmissionController_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtdw . drgyfxwvr2 ) , rtdwData , 0 , 0 , sizeof ( mdlrefDW -> rtdw . drgyfxwvr2 ) ) ; mr_TransmissionController_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtdw . icidstynwv ) , rtdwData , 0 , 1 , sizeof ( mdlrefDW -> rtdw . icidstynwv ) ) ; mr_TransmissionController_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtdw . b5yfuhwx0t ) , rtdwData , 0 , 2 , sizeof ( mdlrefDW -> rtdw . b5yfuhwx0t ) ) ; mr_TransmissionController_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtdw . av2wa1pd3n ) , rtdwData , 0 , 3 , sizeof ( mdlrefDW -> rtdw . av2wa1pd3n ) ) ; mr_TransmissionController_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtdw . fe5mmldjmq ) , rtdwData , 0 , 4 , sizeof ( mdlrefDW -> rtdw . fe5mmldjmq ) ) ; mr_TransmissionController_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtdw . lyaq3qnmfw ) , rtdwData , 0 , 5 , sizeof ( mdlrefDW -> rtdw . lyaq3qnmfw ) ) ; mr_TransmissionController_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtdw . dv1nobpvdc ) , rtdwData , 0 , 6 , sizeof ( mdlrefDW -> rtdw . dv1nobpvdc ) ) ; mr_TransmissionController_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtdw . a05tnifdjv ) , rtdwData , 0 , 7 , sizeof ( mdlrefDW -> rtdw . a05tnifdjv ) ) ; mr_TransmissionController_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtdw . oceatsapa5 ) , rtdwData , 0 , 8 , sizeof ( mdlrefDW -> rtdw . oceatsapa5 ) ) ; mr_TransmissionController_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtdw . c1rvhddg4tu . czt2emm2gr ) , rtdwData , 0 , 9 , sizeof ( mdlrefDW -> rtdw . c1rvhddg4tu . czt2emm2gr ) ) ; mr_TransmissionController_restoreDataFromMxArray ( ( void * ) & ( mdlrefDW -> rtdw . c1rvhddg4tu . jdn5p4hqsj ) , rtdwData , 0 , 10 , sizeof ( mdlrefDW -> rtdw . c1rvhddg4tu . jdn5p4hqsj ) ) ; } } void mr_TransmissionController_RegisterSimStateChecksum ( SimStruct * S ) { const uint32_T chksum [ 4 ] = { 2412753479U , 1620018923U , 1681422967U , 1714061958U , } ; slmrModelRefRegisterSimStateChecksum ( S , "TransmissionController" , & chksum [ 0 ] ) ; } mxArray * mr_TransmissionController_GetSimStateDisallowedBlocks ( ) { return ( NULL ) ; }
#if defined(_MSC_VER)
#pragma warning(disable: 4505) //unreferenced local function has been removed
#endif
