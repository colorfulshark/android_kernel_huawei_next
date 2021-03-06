


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasEmmcSendMsg.h"
#include  "NasEmmcPublic.h"
#include  "NasEmmPubUGlobal.h"

#include "TlPsDrv.h"
/*end*/
#include  "NasEmmAttDetInclude.h"

/*lint -e767*/
#define    THIS_FILE_ID            PS_FILE_ID_NASEMMCSENDMSG_C
#define    THIS_NAS_FILE_ID        NAS_FILE_ID_NASEMMCSENDMSG_C
/*lint +e767*/


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/

/*lint -e72*/
VOS_UINT32  NAS_LMM_MmcPlmnReqTypeCheck
(
    MMC_LMM_PLMN_SRCH_TYPE_ENUM_UINT32 ulMmcPlmnType
)
{
    LRRC_LNAS_PLMN_SEARCH_TYPE_ENUM_UINT32   ulPlmnType;

    if (MMC_LMM_PLMN_SRCH_SPEC == ulMmcPlmnType)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_MmcPlmnReqTypeCheck: SPEC PLMN! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_MmcPlmnReqTypeCheck_ENUM
,LNAS_FUNCTION_LABEL1);
        ulPlmnType = LRRC_LNAS_PLMN_SEARCH_SPEC;
    }
    else if (MMC_LMM_PLMN_SRCH_LIST == ulMmcPlmnType)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_MmcPlmnReqTypeCheck: LIST PLMN! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_MmcPlmnReqTypeCheck_ENUM
,LNAS_FUNCTION_LABEL2);
        ulPlmnType = LRRC_LNAS_PLMN_SEARCH_LIST;
    }
    else if (MMC_LMM_PLMN_SRCH_ANY == ulMmcPlmnType)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_MmcPlmnReqTypeCheck: ANY PLMN! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_MmcPlmnReqTypeCheck_ENUM
,LNAS_FUNCTION_LABEL3);
        ulPlmnType = LRRC_LNAS_PLMN_SEARCH_ANY;
    }
    else if (MMC_LMM_PLMN_SRCH_USER_SPEC == ulMmcPlmnType)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_MmcPlmnReqTypeCheck: USER SPEC PLMN! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_MmcPlmnReqTypeCheck_ENUM
,LNAS_FUNCTION_LABEL4);
        ulPlmnType = LRRC_LNAS_PLMN_SEARCH_SPEC;
    }
    else if (MMC_LMM_PLMN_SRCH_FAST_SPEC == ulMmcPlmnType)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_MmcPlmnReqTypeCheck: FAST SPEC PLMN! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_MmcPlmnReqTypeCheck_ENUM
,LNAS_FUNCTION_LABEL5);
        ulPlmnType = LRRC_LNAS_PLMN_SEARCH_FAST_SPEC;
    }
    else if (MMC_LMM_PLMN_SRCH_NCELL_SPEC == ulMmcPlmnType)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_MmcPlmnReqTypeCheck: NCELL SPEC PLMN! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_MmcPlmnReqTypeCheck_ENUM
,LNAS_FUNCTION_LABEL6);
        ulPlmnType = LRRC_LNAS_PLMN_SEARCH_NCELL_SPEC;
    }
    else if (MMC_LMM_PLMN_SRCH_HISTORY == ulMmcPlmnType)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_MmcPlmnReqTypeCheck: HISTORY PLMN! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_MmcPlmnReqTypeCheck_ENUM
,LNAS_FUNCTION_LABEL7);
        ulPlmnType = LRRC_LNAS_PLMN_SRCH_HISTORY;
    }
    else if (MMC_LMM_PLMN_SRCH_GET_GEO == ulMmcPlmnType)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_MmcPlmnReqTypeCheck: GET_GEO PLMN! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_MmcPlmnReqTypeCheck_ENUM
,LNAS_FUNCTION_LABEL8);
        ulPlmnType = LRRC_LNAS_PLMN_GET_GEO;
    }
    else
    {
        NAS_LMM_EMMC_LOG_ERR(" NAS_LMM_MmcPlmnReqTypeCheck: Input Plmn Type is Err! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_MmcPlmnReqTypeCheck_ENUM
,LNAS_FUNCTION_LABEL9);
        ulPlmnType = LRRC_LNAS_PLMN_SEARCH_TYPE_BUTT;
    }
    return ulPlmnType;
}



VOS_UINT32  NAS_LMM_RrcPlmnCnfTypeCheck
(
    LRRC_LNAS_PLMN_SEARCH_RLT_ENUM_UINT32   ulPlmnRslt
)
{
    MMC_LMM_PLMN_SRCH_RLT_ENUM_UINT32   ulMmcPlmnRslt;

    if (LRRC_LNAS_PLMN_SEARCH_RLT_SPEC_SUCC == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: SPEC SUCC! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL1);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_SPEC_SUCC;
    }
    else if(LRRC_LNAS_PLMN_SEARCH_RLT_LIST_SUCC == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: LIST SUCC! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL2);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_LIST_SUCC;
    }
    else if (LRRC_LNAS_PLMN_SEARCH_RLT_SPEC_FAIL == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: SPEC FAIL! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL3);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_SPEC_FAIL;
    }
    else if (LRRC_LNAS_PLMN_SEARCH_RLT_LIST_FAIL == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: LIST FAIL! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL4);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_LIST_SUCC;
    }
    else if (LRRC_LNAS_PLMN_SRCH_RLT_LIST_REJ == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: LIST REJ! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL5);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_LIST_REJ;
    }
    else if (LRRC_LNAS_PLMN_SRCH_RLT_HISTORY_FAIL == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: HISTORY FAIL! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL6);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_HISTORY_FAIL;
    }
    else if (LRRC_LNAS_PLMN_SRCH_RLT_HISTORY_SUCC == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: HISTORY SUCC! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL7);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_HISTORY_SUCC;
    }
    else if (LRRC_LNAS_PLMN_SRCH_RLT_HISTORY_NO_RF == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: HISTORY NO RF! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL8);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_HISTORY_NO_RF;
    }
    else if (LRRC_LNAS_PLMN_SRCH_RLT_GET_GEO_FAIL == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: GET GEO FAIL! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL9);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_GET_GEO_FAIL;
    }
    else if (LRRC_LNAS_PLMN_SRCH_RLT_GET_GEO_SUCC == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: GET GEO SUCC! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL10);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_GET_GEO_SUCC;
    }
    else if (LRRC_LNAS_PLMN_SRCH_RLT_GET_GEO_NO_RF == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: GET GEO NO RF! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL11);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_GET_GEO_NO_RF;
    }
    #if (FEATURE_ON == FEATURE_DSDS)
    else if (LRRC_LNAS_PLMN_SEARCH_RLT_SPEC_NO_RF == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: SPEC NO RF! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL12);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_SPEC_NO_RF;
    }
    else if (LRRC_LNAS_PLMN_SEARCH_RLT_LIST_NO_RF == ulPlmnRslt)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_LMM_RrcPlmnCnfTypeCheck: LIST NO RF! ");
        TLPS_PRINT2LAYER_INFO(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL13);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_LIST_NO_RF;
    }
    #endif
    else
    {
        NAS_LMM_EMMC_LOG_ERR(" NAS_LMM_MmcPlmnReqTypeCheck: Input Plmn Rslt is Err! ");
        TLPS_PRINT2LAYER_ERROR(NAS_LMM_RrcPlmnCnfTypeCheck_ENUM
,LNAS_FUNCTION_LABEL14);
        ulMmcPlmnRslt = MMC_LMM_PLMN_SRCH_RLT_BUTT;
    }

    return ulMmcPlmnRslt;
}



/*lint -e960*/
/*lint -e961*/
VOS_VOID  NAS_EMMC_SendEmmStartCnf(VOS_VOID)
{

    EMMC_EMM_START_CNF_STRU             *pstStartCnf;

    /* 获取内部消息地址 */
    pstStartCnf = (VOS_VOID *)NAS_LMM_GetSendIntraMsgBuffAddr(sizeof(EMMC_EMM_START_CNF_STRU));

    if (NAS_EMMC_NULL_PTR == pstStartCnf)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendEmmStartCnf:MSG ALLOC ERR! ");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendEmmStartCnf_ENUM
,LNAS_NULL_PTR);
        return;
    }

    NAS_LMM_MEM_SET(pstStartCnf, 0, sizeof(EMMC_EMM_START_CNF_STRU));


    /* 打包内部消息头 */
    NAS_EMMC_COMP_MM_INTRAMSG_HEADER(   pstStartCnf,
                                        (sizeof(EMMC_EMM_START_CNF_STRU)
                                        - NAS_EMMC_LEN_VOS_MSG_HEADER));

    pstStartCnf->enMsgId                = ID_EMMC_EMM_START_CNF;

    /*向EMM发送ID_EMMC_EMM_START_CNF消息*/
    NAS_EMM_SEND_INTRA_MSG(             pstStartCnf);

}




VOS_VOID  NAS_EMMC_SendEmmStopCnf(VOS_VOID)
{
    EMMC_EMM_STOP_CNF_STRU             *pstStopCnf;

    /* 获取内部消息地址 */
    pstStopCnf = (VOS_VOID *)NAS_LMM_GetSendIntraMsgBuffAddr(sizeof(EMMC_EMM_STOP_CNF_STRU));

    if (NAS_EMMC_NULL_PTR == pstStopCnf)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendEmmStopCnf:MSG ALLOC ERR! ");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendEmmStopCnf_ENUM
,LNAS_NULL_PTR);
        return;
    }

    NAS_LMM_MEM_SET(pstStopCnf, 0, sizeof(EMMC_EMM_STOP_CNF_STRU));

    /* 打包内部消息头 */
    NAS_EMMC_COMP_MM_INTRAMSG_HEADER(   pstStopCnf,
                                        (sizeof(EMMC_EMM_STOP_CNF_STRU)
                                        - NAS_EMMC_LEN_VOS_MSG_HEADER));

    pstStopCnf->enMsgId                 = ID_EMMC_EMM_STOP_CNF;

    /*向EMM发送ID_EMMC_EMM_STOP_CNF消息*/
    NAS_EMM_SEND_INTRA_MSG(             pstStopCnf);

    return;
}



VOS_VOID  NAS_EMMC_SendEmmPlmnInd(VOS_VOID)
{
    EMMC_EMM_PLMN_IND_STRU             *pstPlmnInd;

    /* 获取内部消息地址 */
    pstPlmnInd = (VOS_VOID *)NAS_LMM_GetSendIntraMsgBuffAddr(sizeof(EMMC_EMM_PLMN_IND_STRU));

    if (NAS_EMMC_NULL_PTR == pstPlmnInd)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendEmmPlmnInd:MSG ALLOC ERR! ");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendEmmPlmnInd_ENUM
,LNAS_NULL_PTR);
        return;
    }

    NAS_LMM_MEM_SET(pstPlmnInd, 0, sizeof(EMMC_EMM_PLMN_IND_STRU));

    /* 打包内部消息头 */
    NAS_EMMC_COMP_MM_INTRAMSG_HEADER(   pstPlmnInd,
                                        (sizeof(EMMC_EMM_PLMN_IND_STRU)
                                        - NAS_EMMC_LEN_VOS_MSG_HEADER));

    pstPlmnInd->enMsgId                 = ID_EMMC_EMM_PLMN_IND;


    /*向EMM发送ID_EMMC_EMM_PLMN_IND消息*/
    NAS_EMM_SEND_INTRA_MSG(             pstPlmnInd);

    return;
}



/*lint -specific(-e433)*/
VOS_VOID NAS_EMMC_SendRrcPlmnSrchReq(const MMC_LMM_PLMN_SRCH_REQ_STRU *pstRcvMsg)
{
    LRRC_LMM_PLMN_SEARCH_REQ_STRU      *pstPlmnReqMsg = NAS_EMMC_NULL_PTR;
    NAS_MM_TA_LIST_STRU                *pstForbTaListForRoaming = NAS_EMMC_NULL_PTR;
    LRRC_LNAS_PLMN_SEARCH_TYPE_ENUM_UINT32   ulPlmnType;
    NAS_MM_TA_LIST_STRU                 stTaList = {0};
    if(NAS_EMMC_NULL_PTR == pstRcvMsg)
    {
        /*打印错误*/
        NAS_LMM_EMMC_LOG_ERR(" NAS_EMMC_SendRrcPlmnSrchReq: Input Null!");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendRrcPlmnSrchReq_ENUM
,LNAS_NULL_PTR);

        return;
    }

    /*申请消息内存*/
    /*lint -e433 -e826*/
    pstPlmnReqMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(LRRC_LMM_PLMN_SEARCH_REQ_STRU));
    /*lint +e433 +e826*/

    /*判断申请结果，若失败打印错误并退出*/
    if (NAS_EMMC_NULL_PTR == pstPlmnReqMsg)
    {
        /*打印错误*/
        NAS_LMM_EMMC_LOG_ERR(" NAS_EMMC_SendRrcPlmnSrchReq: MSG ALLOC NAS_EMMC_NULL_PTR  Null!");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendRrcPlmnSrchReq_ENUM
,LNAS_FUNCTION_LABEL1);
        return;
    }

    NAS_LMM_MEM_SET(pstPlmnReqMsg,0,sizeof(LRRC_LMM_PLMN_SEARCH_REQ_STRU));

    /*构造ID_RRC_MM_PLMN_SEARCH_REQ消息*/
    /*填充消息头*/
    NAS_EMMC_COMP_RRC_MSG_HEADER(       pstPlmnReqMsg,
                                        (sizeof(LRRC_LMM_PLMN_SEARCH_REQ_STRU)-
                                        NAS_EMMC_LEN_VOS_MSG_HEADER));

    /*填充消息ID*/
    pstPlmnReqMsg->enMsgId              = ID_LRRC_LMM_PLMN_SEARCH_REQ;

    /*填充消息内容*/
    ulPlmnType = NAS_LMM_MmcPlmnReqTypeCheck(pstRcvMsg->enSrchType);

    pstPlmnReqMsg->enSearchType         = ulPlmnType;

    pstPlmnReqMsg->bitOpPlmnInfoCampFlg = pstRcvMsg->bitOpPlmnInfoCampFlg;
    pstPlmnReqMsg->enFftScanType        = (LRRC_LNAS_FFT_SCAN_TYPE_ENUM_UINT8)pstRcvMsg->enFftScanType;

    if(NAS_EMMC_BIT_SLCT == pstRcvMsg->bitOpSpecPlmn)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_EMMC_SendRrcPlmnSrchReq: Spec PLMN");
        TLPS_PRINT2LAYER_INFO(NAS_EMMC_SendRrcPlmnSrchReq_ENUM
,LNAS_FUNCTION_LABEL2);

        pstPlmnReqMsg->bitOpSpecPlmn    = NAS_EMMC_BIT_SLCT;
        NAS_LMM_MEM_CPY(                 &pstPlmnReqMsg->stSpecPlmnId,
                                        &pstRcvMsg->stSpecPlmnId,
                                        sizeof(LRRC_LNAS_PLMN_ID_STRU));
    }

    if(NAS_EMMC_BIT_SLCT == pstRcvMsg->bitOpNcellInfo)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_EMMC_SendRrcPlmnSrchReq: LTE ARFCN INFO");
        TLPS_PRINT2LAYER_INFO(NAS_EMMC_SendRrcPlmnSrchReq_ENUM
,LNAS_FUNCTION_LABEL3);

        pstPlmnReqMsg->bitOpNcellInfo    = NAS_EMMC_BIT_SLCT;
        NAS_LMM_MEM_CPY(                 &pstPlmnReqMsg->stNcellInfo,
                                        &pstRcvMsg->stNcellInfo,
                                        sizeof(LRRC_LNAS_NCELL_INFO_STRU));
    }

    if ( NAS_EMMC_BIT_SLCT == pstRcvMsg->bitOpEqlPlmn)
    {
        NAS_LMM_EMMC_LOG1_NORM(" NAS_EMMC_SendRrcPlmnSrchReq: Eplmn num = ",
                             pstRcvMsg->stEplmnList.ulPlmnNum);
        TLPS_PRINT2LAYER_INFO1(NAS_EMMC_SendRrcPlmnSrchReq_ENUM
,LNAS_FUNCTION_LABEL4,
                             pstRcvMsg->stEplmnList.ulPlmnNum);
        pstPlmnReqMsg->bitOpEqlPlmn     = NAS_EMMC_BIT_SLCT;
        NAS_LMM_MEM_CPY(                 &pstPlmnReqMsg->stEplmn,
                                        &pstRcvMsg->stEplmnList,
                                        sizeof(MMC_LMM_EPLMN_STRU));
    }

    pstPlmnReqMsg->bitOpForbLa          = NAS_EMMC_BIT_NO_SLCT;

    pstForbTaListForRoaming = (VOS_VOID *)NAS_EMM_GetEmmGlobleAddr(NAS_LMM_GLOBLE_FORB_TA_FOR_ROAMING);

    if (MMC_LMM_PLMN_SRCH_USER_SPEC == pstRcvMsg->enSrchType)
    {
        NAS_EMMC_FillUserSpecRrcNasForbTaList((NAS_MM_PLMN_ID_STRU *)&pstRcvMsg->stSpecPlmnId,&stTaList);

        if (NAS_LMM_NULL < stTaList.ulTaNum)
        {
            NAS_LMM_EMMC_LOG1_NORM(" NAS_EMMC_SendPlmnSrchReq: USER PLMN Forb TA num = ",
                                             stTaList.ulTaNum);
           TLPS_PRINT2LAYER_INFO1(NAS_EMMC_SendRrcPlmnSrchReq_ENUM
,LNAS_FUNCTION_LABEL4,
                             pstRcvMsg->stEplmnList.ulPlmnNum);

            pstPlmnReqMsg->bitOpForbTa      = NAS_EMMC_BIT_SLCT;
            NAS_LMM_MEM_CPY(        &pstPlmnReqMsg->stForbTaList,
                                    &stTaList,
                                    sizeof(NAS_MM_TA_LIST_STRU));
        }
    }
    else
    {
        /*获取禁止TA信息*/
        /* 增加Gradual Forb临时表中是否存在TA被禁判断 */
        if ((0 < pstForbTaListForRoaming->ulTaNum) || (NAS_EMM_YES == NAS_LMM_ExistTaInTempTaListWithForbTimerRunning()))
        {
            NAS_LMM_EMMC_LOG1_NORM(" NAS_EMMC_SendPlmnSrchReq: Forb TA num = ",
                                 pstForbTaListForRoaming->ulTaNum);
            TLPS_PRINT2LAYER_INFO1(NAS_EMMC_SendRrcPlmnSrchReq_ENUM
,LNAS_FUNCTION_LABEL5,
                                pstForbTaListForRoaming->ulTaNum);

            pstPlmnReqMsg->bitOpForbTa      = NAS_EMMC_BIT_SLCT;

            NAS_EMMC_FillRrcNasForbTaList(&(pstPlmnReqMsg->stForbTaList));

        }
    }
    /*向RRC发送ID_RRC_MM_PLMN_SEARCH_REQ消息*/
    NAS_LMM_SEND_MSG(                    pstPlmnReqMsg);

    return;

}
/*lint -specific(+e433)*/

/*lint -specific(-e433)*/
VOS_VOID  NAS_EMMC_SendMmcPlmnSrchCnf(const LRRC_LMM_PLMN_SEARCH_CNF_STRU *pstRcvMsg )
{
    LMM_MMC_PLMN_SRCH_CNF_STRU        *pstPlmnCnfMsg = NAS_EMMC_NULL_PTR;
    MMC_LMM_PLMN_SRCH_RLT_ENUM_UINT32   ulMmcPlmnRslt;
    VOS_UINT32                         ulLoop;
#if (VOS_OS_VER != VOS_WIN32)
    static VOS_UINT32  s_ulPlmnSrchCnfCnt = 1;
#endif

    NAS_LMM_EMMC_LOG_NORM(" NAS_EMMC_SendMmcPlmnSrchCnf is entered!");
    TLPS_PRINT2LAYER_INFO(NAS_EMMC_SendMmcPlmnSrchCnf_ENUM
,LNAS_ENTRY);
    if(NAS_EMMC_NULL_PTR == pstRcvMsg)
    {
        /*打印错误*/
        NAS_LMM_EMMC_LOG_ERR(" NAS_EMMC_SendMmcPlmnSrchCnf: Input Ptr Null!");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcPlmnSrchCnf_ENUM
,LNAS_NULL_PTR);

        return;
    }

    /*申请MMC内部消息 */
    pstPlmnCnfMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_PLMN_SRCH_CNF_STRU));

    /*判断申请结果，若失败打印错误并退出*/
    if (NAS_EMMC_NULL_PTR == pstPlmnCnfMsg)
    {
        /*打印错误*/
        NAS_LMM_EMMC_LOG_ERR(" NAS_EMMC_SendMmcPlmnSrchCnf: MSG ALLOC ERR or Input Ptr Null!");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcPlmnSrchCnf_ENUM
,LNAS_FUNCTION_LABEL1);
        return;
    }

    NAS_LMM_MEM_SET(pstPlmnCnfMsg, 0, sizeof(LMM_MMC_PLMN_SRCH_CNF_STRU));

    /*构造ID_LMM_MMC_PLMN_SRCH_CNF消息*/
    /*填充消息头*/
    NAS_EMMC_COMP_MMC_MSG_HEADER(       pstPlmnCnfMsg,
                                        (sizeof(LMM_MMC_PLMN_SRCH_CNF_STRU)-
                                        NAS_EMMC_LEN_VOS_MSG_HEADER));

    /*填充消息ID*/
    pstPlmnCnfMsg->ulMsgId              = ID_LMM_MMC_PLMN_SRCH_CNF;


    /*填充消息内容*/
    ulMmcPlmnRslt = NAS_LMM_RrcPlmnCnfTypeCheck(pstRcvMsg->enRlst);

    if (LRRC_LNAS_PLMN_SEARCH_RLT_LIST_FAIL == pstRcvMsg->enRlst)
    {
        NAS_LMM_EMMC_LOG_NORM(" NAS_EMMC_SendMmcPlmnSrchCnf:LIST FAIL!");
        TLPS_PRINT2LAYER_INFO(NAS_EMMC_SendMmcPlmnSrchCnf_ENUM
,LNAS_FUNCTION_LABEL2);
        pstPlmnCnfMsg->enRlst           = MMC_LMM_PLMN_SRCH_RLT_LIST_SUCC;

        NAS_LMM_MEM_SET(                 &pstPlmnCnfMsg->stSpecPlmnIdList,
                                        0,
                                        sizeof(MMC_LMM_SUIT_PLMN_ID_LIST_STRU));
        NAS_LMM_MEM_SET(                 &pstPlmnCnfMsg->stPlmnIdList,
                                        0,
                                        sizeof(MMC_LMM_PLMN_ID_LIST_STRU));
    }
    else
    {
        pstPlmnCnfMsg->enRlst           = ulMmcPlmnRslt;
        NAS_LMM_MEM_CPY(                 &pstPlmnCnfMsg->stSpecPlmnIdList,
                                        &pstRcvMsg->stSpecPlmnIdList,
                                        sizeof(MMC_LMM_SUIT_PLMN_ID_LIST_STRU));

        NAS_LMM_MEM_CPY(                 &pstPlmnCnfMsg->stPlmnIdList,
                                        &pstRcvMsg->stPlmnIdList,
                                        sizeof(MMC_LMM_PLMN_ID_LIST_STRU));
    }


    if (LRRC_LNAS_COVERAGE_TYPE_NONE == pstRcvMsg->enCoverageType)
    {
        pstPlmnCnfMsg->enCoverageType = MMC_LMM_COVERAGE_TYPE_NONE;
    }
    else if (LRRC_LNAS_COVERAGE_TYPE_LOW == pstRcvMsg->enCoverageType)
    {
        pstPlmnCnfMsg->enCoverageType = MMC_LMM_COVERAGE_TYPE_LOW;
    }
    else if (LRRC_LNAS_COVERAGE_TYPE_HIGH == pstRcvMsg->enCoverageType)
    {
        pstPlmnCnfMsg->enCoverageType = MMC_LMM_COVERAGE_TYPE_HIGH;
    }
    else
    {
        pstPlmnCnfMsg->enCoverageType = MMC_LMM_COVERAGE_TYPE_BUTT;
    }


    NAS_LMM_MEM_CPY(                 pstPlmnCnfMsg->astSearchRatInfo,
                                    pstRcvMsg->astSearchRatInfo,
                                    sizeof(MMC_LMM_LIST_SEARCH_RAT_INFO_STRU)*MMC_LMM_MAX_RAT_NUM);
    for(ulLoop = 0; ulLoop < MMC_LMM_MAX_RAT_NUM; ulLoop++)
    {
        /* 防止enRat是非法枚举值 */
        if(MMC_LMM_RAT_LIST_SEARCH_BUTT < pstPlmnCnfMsg->astSearchRatInfo[ulLoop].enRat)
        {
            pstPlmnCnfMsg->astSearchRatInfo[ulLoop].enRat = MMC_LMM_RAT_LIST_SEARCH_BUTT;
        }
    }


#if (VOS_OS_VER != VOS_WIN32)
    if(ulMmcPlmnRslt < LRRC_LNAS_PLMN_SEARCH_RLT_SPEC_FAIL)
    {
        if(1 == s_ulPlmnSrchCnfCnt)
        {
            (VOS_VOID)LDRV_SYSBOOT_STAMPADD("LTE Cell search success", __LINE__);
            s_ulPlmnSrchCnfCnt++;
        }
    }
#endif
    /*end*/

    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstPlmnCnfMsg);

    return;
}
/*lint -specific(+e433)*/

VOS_VOID  NAS_EMMC_SendMmcPlmnListSrchRej(VOS_VOID )
{
    LMM_MMC_PLMN_SRCH_CNF_STRU        *pstPlmnCnfMsg = NAS_EMMC_NULL_PTR;

    NAS_LMM_EMMC_LOG_NORM(" NAS_EMMC_SendMmcPlmnListSrchRej is entered!");
    TLPS_PRINT2LAYER_INFO(NAS_EMMC_SendMmcPlmnListSrchRej_ENUM
,LNAS_ENTRY);

    /*申请MMC内部消息 */
    pstPlmnCnfMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_PLMN_SRCH_CNF_STRU));

    /*判断申请结果，若失败打印错误并退出*/
    if (NAS_EMMC_NULL_PTR == pstPlmnCnfMsg)
    {
        /*打印错误*/
        NAS_LMM_EMMC_LOG_ERR(" NAS_EMMC_SendMmcPlmnListSrchRej: MSG ALLOC ERR or Input Ptr Null!");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcPlmnListSrchRej_ENUM
,LNAS_NULL_PTR);
        return;
    }

    NAS_LMM_MEM_SET(pstPlmnCnfMsg, 0, sizeof(LMM_MMC_PLMN_SRCH_CNF_STRU));

    /*构造ID_LMM_MMC_PLMN_SRCH_CNF消息*/
    /*填充消息头*/
    NAS_EMMC_COMP_MMC_MSG_HEADER(       pstPlmnCnfMsg,
                                        (sizeof(LMM_MMC_PLMN_SRCH_CNF_STRU)-
                                        NAS_EMMC_LEN_VOS_MSG_HEADER));

    /*填充消息ID*/
    pstPlmnCnfMsg->ulMsgId              = ID_LMM_MMC_PLMN_SRCH_CNF;

    pstPlmnCnfMsg->enRlst               = MMC_LMM_PLMN_SRCH_RLT_LIST_REJ;


    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstPlmnCnfMsg);

    return;
}
VOS_VOID    NAS_EMMC_SendEmmSysInfoInd
(/* lihong00150010 emergency tau&service begin */
    EMMC_EMM_CHANGE_INFO_ENUM_UINT32 ulChangeInfo,
    EMMC_EMM_CELL_STATUS_ENUM_UINT32 ulCellStatus,
    EMMC_EMM_FORBIDDEN_INFO_ENUM_UINT32  ulForbdInfo
)/* lihong00150010 emergency tau&service end */
{

    EMMC_EMM_SYS_INFO_IND_STRU          *pstEmmSysInfoIndMsg = NAS_EMMC_NULL_PTR;
    NAS_MM_NETWORK_ID_STRU               *pstAreaInfo = NAS_EMMC_NULL_PTR;

    /*申请消息内存*/
    pstEmmSysInfoIndMsg = (VOS_VOID *)NAS_LMM_GetSendIntraMsgBuffAddr(sizeof(EMMC_EMM_SYS_INFO_IND_STRU));

    /*判断申请结果，若失败打印错误并退出*/
    if (NAS_EMMC_NULL_PTR == pstEmmSysInfoIndMsg)
    {
        /*打印错误*/
        NAS_LMM_EMMC_LOG_ERR(" NAS_EMMC_SendEmmSysInfoInd: MSG ALLOC ERR!");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendEmmSysInfoInd_ENUM
,LNAS_NULL_PTR);
        return;

    }

    NAS_LMM_MEM_SET(pstEmmSysInfoIndMsg, 0, sizeof(EMMC_EMM_SYS_INFO_IND_STRU));

    pstAreaInfo = (VOS_VOID *)NAS_EMM_GetEmmGlobleAddr(NAS_LMM_GLOBLE_PRESENT_NET_ID);

    /*构造ID_EMMC_EMM_SYS_INFO_IND消息*/
    /*填充消息头*/
    NAS_EMMC_COMP_MM_INTRAMSG_HEADER(   pstEmmSysInfoIndMsg,
                                        (sizeof(EMMC_EMM_SYS_INFO_IND_STRU)-
                                        NAS_EMMC_LEN_VOS_MSG_HEADER));

    /*填充消息ID*/
    pstEmmSysInfoIndMsg->enMsgId        = ID_EMMC_EMM_SYS_INFO_IND;


    /*填充消息内容*/
    NAS_LMM_MEM_CPY(                     &pstEmmSysInfoIndMsg->stPlmnId,
                                        &pstAreaInfo->stPlmnId,
                                        sizeof(NAS_MM_PLMN_ID_STRU));

    pstEmmSysInfoIndMsg->stTac.ucTac    = pstAreaInfo->stTac.ucTac;
    pstEmmSysInfoIndMsg->stTac.ucTacCnt = pstAreaInfo->stTac.ucTacCnt;

    pstEmmSysInfoIndMsg->ulCellId       = pstAreaInfo->ulCellId;

    /* lihong00150010 emergency tau&service delete */
    pstEmmSysInfoIndMsg->ulForbiddenInfo= ulForbdInfo;

    /*获取改变信息*/
    pstEmmSysInfoIndMsg->ulChangeInfo   = ulChangeInfo;

    pstEmmSysInfoIndMsg->ulCellStatus   = ulCellStatus;

    /*向MMC发送ID_EMMC_EMM_SYS_INFO_IND消息*/
    NAS_EMM_SEND_INTRA_MSG(                    pstEmmSysInfoIndMsg);

    return;
}




VOS_VOID  NAS_EMMC_SendMmcSysInfo(const LRRC_LMM_SYS_INFO_IND_STRU  *pstRcvMsg  )
{
    LMM_MMC_SYS_INFO_IND_STRU          *pstSysInfoIndMsg = NAS_EMMC_NULL_PTR;
    LRRC_LNAS_CELL_STATUS_ENUM_UINT32   enCellStatus;
    NAS_MM_TA_STRU                      stTa;

    if(NAS_EMMC_NULL_PTR == pstRcvMsg)
    {
        /*打印错误*/
        NAS_LMM_EMMC_LOG_ERR(" NAS_EMMC_SendMmcSysInfo: Input Ptr Null!");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcSysInfo_ENUM
,LNAS_NULL_PTR);
        return;
    }

    /*申请MMC内部消息 */
    pstSysInfoIndMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_SYS_INFO_IND_STRU));

    /*判断申请结果，若失败打印错误并退出*/
    if (NAS_EMMC_NULL_PTR == pstSysInfoIndMsg)
    {
        /*打印错误*/
        NAS_LMM_EMMC_LOG_ERR(" NAS_EMMC_SendMmcSysInfo: MSG ALLOC ERR or Input Ptr Null!");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcSysInfo_ENUM
,LNAS_FUNCTION_LABEL1);
        return;

    }

    NAS_LMM_MEM_SET(pstSysInfoIndMsg, 0, sizeof(LMM_MMC_SYS_INFO_IND_STRU));


    /*构造ID_LMM_MMC_SYS_INFO_IND消息*/
    /*填充消息头*/
    NAS_EMMC_COMP_MMC_MSG_HEADER(       pstSysInfoIndMsg,
                                        (sizeof(LMM_MMC_SYS_INFO_IND_STRU)-
                                        NAS_EMMC_LEN_VOS_MSG_HEADER));

    /*填充消息ID*/
    pstSysInfoIndMsg->ulMsgId           = ID_LMM_MMC_SYS_INFO_IND;


    /*填充消息内容*/
    NAS_LMM_MEM_CPY(&pstSysInfoIndMsg->stLteSysInfo.stSpecPlmnIdList,
                   &pstRcvMsg->stSpecPlmnIdList,
                   sizeof(LRRC_LNAS_SUIT_PLMN_ID_LIST_STRU));

    pstSysInfoIndMsg->stLteSysInfo.stTac.ucTac = pstRcvMsg->stTac.ucTac;
    pstSysInfoIndMsg->stLteSysInfo.stTac.ucTacCnt = pstRcvMsg->stTac.ucTacCont;

    pstSysInfoIndMsg->stLteSysInfo.ulCellId       = pstRcvMsg->ulCellId;

    /* GU NAS让LNAS在收到RRC系统消息时先判断小区状态是否是NORMAL上报给MMC，以便GU NAS判断是否停止3212等定时器 */

    /* 初始化小区状态为NORMAL */
    enCellStatus = EMMC_EMM_CELL_STATUS_NORMAL;

    stTa.stPlmnId.aucPlmnId[0] = pstRcvMsg->stSpecPlmnIdList.astSuitPlmnList[0].aucPlmnId[0];
    stTa.stPlmnId.aucPlmnId[1] = pstRcvMsg->stSpecPlmnIdList.astSuitPlmnList[0].aucPlmnId[1];
    stTa.stPlmnId.aucPlmnId[2] = pstRcvMsg->stSpecPlmnIdList.astSuitPlmnList[0].aucPlmnId[2];
    stTa.stPlmnId.ucRsv        = 0x00;
    stTa.stTac.ucTac           = pstRcvMsg->stTac.ucTac;
    stTa.stTac.ucTacCnt        = pstRcvMsg->stTac.ucTacCont;
    stTa.stTac.aucRsv[0]       = 0x00;
    stTa.stTac.aucRsv[1]       = 0x00;

    /* 如果收到RRC上报小区状态为ANY CELL，判断是否真的是ANY CELL，是则修改小区状态为ANY,否则不修改 */
    if (LRRC_LNAS_CELL_STATUS_ANYCELL == pstRcvMsg->enCellStatusInd)
    {
        if (VOS_FALSE == NAS_LMM_IsCellStatusNormal( &pstSysInfoIndMsg->stLteSysInfo.stSpecPlmnIdList.astSuitPlmnList[0],
                                                    &stTa))
        {
            NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_ProcessMmcLteSysInfoInd: anycell search find avaiable cell,cellStatus: ANYCELL");
            TLPS_PRINT2LAYER_INFO(NAS_EMMC_SendMmcSysInfo_ENUM
,LNAS_FUNCTION_LABEL2);
            enCellStatus = EMMC_EMM_CELL_STATUS_ANYCELL;
        }
    }
    else if(LRRC_LNAS_CELL_STATUS_NORMAL == pstRcvMsg->enCellStatusInd)
    {
        /* 本身NORMAL无需修改 */
        NAS_LMM_EMMC_LOG_NORM("NAS_LMM_PreProcMmcLteSysInfoInd:cellstauts is NORMAL! ");
        TLPS_PRINT2LAYER_INFO(NAS_EMMC_SendMmcSysInfo_ENUM
,LNAS_FUNCTION_LABEL3);
    }
    else
    {
        /* 不是ANY CELL，但也不是NORMAL，说明RRC上报小区状态非法，修改为BUTT */
        NAS_LMM_EMMC_LOG_WARN("NAS_LMM_PreProcMmcLteSysInfoInd:cellstauts is err! ");
        TLPS_PRINT2LAYER_WARNING(NAS_EMMC_SendMmcSysInfo_ENUM
,LNAS_FUNCTION_LABEL4);
        enCellStatus = EMMC_EMM_CELL_STATUS_BUTT;
    }

    pstSysInfoIndMsg->stLteSysInfo.enCellStatusInd = enCellStatus;

    pstSysInfoIndMsg->stLteSysInfo.ulArfcn        = pstRcvMsg->ulArfcn;
    /* 添加bandwidth信息 */
    pstSysInfoIndMsg->stLteSysInfo.ucBandWidth         = pstRcvMsg->ucBandWidth;

    pstSysInfoIndMsg->stLteSysInfo.enAccessType = pstRcvMsg->enAccessType;

    /* 增加BAND信息 */
    NAS_LMM_MEM_CPY(pstSysInfoIndMsg->stLteSysInfo.stLteBand.aulLteBand,
                    pstRcvMsg->aulLteBand,
                    sizeof(LTE_BAND_STRU));

    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstSysInfoIndMsg);

    return;
}



VOS_VOID  NAS_EMMC_SendRrcEplmnNotifyReq(const MMC_LMM_EPLMN_NOTIFY_REQ_STRU *pstRcvMsg)
{
    LRRC_LMM_EQU_PLMN_NOTIFY_REQ_STRU   *pstEplmnReq;
    VOS_UINT32                          ulPlmnNum;

    if(NAS_EMMC_NULL_PTR == pstRcvMsg)
    {
        /*打印错误*/
        NAS_LMM_EMMC_LOG_ERR(" NAS_EMMC_SendRrcEplmnNotifyReq: INPUT NULL!");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendRrcEplmnNotifyReq_ENUM
,LNAS_NULL_PTR);
        return;
    }

    /*申请消息内存*/
    pstEplmnReq = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(LRRC_LMM_EQU_PLMN_NOTIFY_REQ_STRU));

    /*判断申请结果，若失败退出*/
    if (NAS_EMMC_NULL_PTR == pstEplmnReq)
    {
        return;
    }

    NAS_LMM_MEM_SET(pstEplmnReq,0,sizeof(LRRC_LMM_EQU_PLMN_NOTIFY_REQ_STRU));

    /*构造ID_RRC_MM_EQU_PLMN_NOTIFY_REQ消息*/
    /*填充消息头*/
    NAS_EMMC_COMP_RRC_MSG_HEADER(       pstEplmnReq,
                                        (sizeof(LRRC_LMM_EQU_PLMN_NOTIFY_REQ_STRU)-
                                        NAS_EMMC_LEN_VOS_MSG_HEADER));

    /*填充消息ID*/
    pstEplmnReq->enMsgId                = ID_LRRC_LMM_EQU_PLMN_NOTIFY_REQ;


    /*填充消息内容*/

    if (MMC_LMM_MAX_EQUPLMN_NUM < pstRcvMsg->ulEplmnNum)
    {
        pstEplmnReq->ulEquPlmnNum = LRRC_LNAS_MAX_EQUPLMN_NUM;
    }
    else
    {
        pstEplmnReq->ulEquPlmnNum           = pstRcvMsg->ulEplmnNum;
    }

    /*把当前RPLMN位置移动到最后一位，以匹配与RRC的接口*/
    ulPlmnNum                       = pstEplmnReq->ulEquPlmnNum;

    if (NAS_EMMC_NULL != ulPlmnNum)
    {
        NAS_LMM_MEM_CPY(                 pstEplmnReq->aEquPlmnIdList,
                                        &pstRcvMsg->astEplmnList[1],
                                        sizeof(MMC_LMM_PLMN_ID_STRU) * (ulPlmnNum -1));
        NAS_LMM_MEM_CPY(                 &pstEplmnReq->aEquPlmnIdList[ulPlmnNum -1],
                                        &pstRcvMsg->astEplmnList[0],
                                        sizeof(LRRC_LNAS_PLMN_ID_STRU));
    }

    /*向RRC发送ID_RRC_MM_EQU_PLMN_NOTIFY_REQ消息*/
    NAS_LMM_SEND_MSG(                    pstEplmnReq);
}
VOS_VOID  NAS_EMMC_SendRrcPlmnSrchStopReq(VOS_VOID)
{
    LRRC_LMM_PLMN_SEARCH_STOP_REQ_STRU    *pstPlmnSrchStopReq;

    /*申请消息内存*/
    pstPlmnSrchStopReq = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(LRRC_LMM_PLMN_SEARCH_STOP_REQ_STRU));

    /*判断申请结果，若失败退出*/
    if (NAS_EMMC_NULL_PTR == pstPlmnSrchStopReq)
    {
        return;
    }

    NAS_LMM_MEM_SET(pstPlmnSrchStopReq,0,sizeof(LRRC_LMM_PLMN_SEARCH_STOP_REQ_STRU));

    /*构造ID_RRC_MM_PLMN_SEARCH_STOP_REQ消息*/
    /*填充消息头*/
    NAS_EMMC_COMP_RRC_MSG_HEADER(       pstPlmnSrchStopReq,
                                        (sizeof(LRRC_LMM_PLMN_SEARCH_STOP_REQ_STRU)-
                                        NAS_EMMC_LEN_VOS_MSG_HEADER));

    /*填充消息ID*/
    pstPlmnSrchStopReq->enMsgId         = ID_LRRC_LMM_PLMN_SEARCH_STOP_REQ;


    /*向RRC发送ID_RRC_MM_PLMN_SEARCH_STOP_REQ消息*/
    NAS_LMM_SEND_MSG(                    pstPlmnSrchStopReq);
}


VOS_VOID  NAS_EMMC_SendMmcStopSrchCnf(VOS_VOID )
{
    LMM_MMC_STOP_PLMN_SRCH_CNF_STRU    *pstStopSrchMsg = NAS_EMMC_NULL_PTR;

    /*申请MMC内部消息 */
    pstStopSrchMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_STOP_PLMN_SRCH_CNF_STRU));

    /*判断申请结果，若失败打印错误并退出*/
    if (NAS_EMMC_NULL_PTR == pstStopSrchMsg)
    {
        /*打印错误*/
        NAS_LMM_EMMC_LOG_ERR(" NAS_EMMC_SendMmcStopSrchCnf: MSG ALLOC ERR!");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcStopSrchCnf_ENUM
,LNAS_NULL_PTR);
        return;

    }

    NAS_LMM_MEM_SET(pstStopSrchMsg, 0, sizeof(LMM_MMC_STOP_PLMN_SRCH_CNF_STRU));


    /*构造LMM_MMC_STOP_SRCH_CNF消息*/
    /*填充消息头*/
    NAS_EMMC_COMP_MMC_MSG_HEADER(       pstStopSrchMsg,
                                        (sizeof(LMM_MMC_STOP_PLMN_SRCH_CNF_STRU)-
                                        NAS_EMMC_LEN_VOS_MSG_HEADER));

    /*填充消息ID*/
    pstStopSrchMsg->ulMsgId           = ID_LMM_MMC_STOP_PLMN_SRCH_CNF;


    /*填充消息内容*/

    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstStopSrchMsg);

}


VOS_VOID  NAS_EMMC_SendEmmCoverageLostInd( VOS_VOID )
{
    EMMC_EMM_COVERAGE_LOST_IND_STRU     *pstCoverageLostInd;

    /* 获取内部消息地址 */
    pstCoverageLostInd = (VOS_VOID *)NAS_LMM_GetSendIntraMsgBuffAddr(sizeof(EMMC_EMM_COVERAGE_LOST_IND_STRU));

    if (NAS_EMMC_NULL_PTR == pstCoverageLostInd)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendEmmCoverageLostInd:MSG ALLOC ERR! ");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendEmmCoverageLostInd_ENUM
,LNAS_NULL_PTR);
        return;
    }

    NAS_LMM_MEM_SET(pstCoverageLostInd, 0, sizeof(EMMC_EMM_COVERAGE_LOST_IND_STRU));


    /* 打包内部消息头 */
    NAS_EMMC_COMP_MM_INTRAMSG_HEADER(   pstCoverageLostInd,
                                        (sizeof(EMMC_EMM_COVERAGE_LOST_IND_STRU) -
                                        NAS_EMMC_LEN_VOS_MSG_HEADER));

    pstCoverageLostInd->enMsgId         = ID_EMMC_EMM_COVERAGE_LOST_IND;


    /*向EMM发送EMMC_EMM_COVERAGE_LOST_IND消息*/
    NAS_EMM_SEND_INTRA_MSG(             pstCoverageLostInd);

    return;
}



/*lint -specific(-e433)*/
VOS_VOID  NAS_EMMC_SendMmcAreaLostInd(MMC_LMM_AREA_LOST_REASON_UINT32  enAreaLostReason  )
{
    LMM_MMC_AREA_LOST_IND_STRU         *pstAreaLostIndMsg = NAS_EMMC_NULL_PTR;

    /*申请MMC内部消息 */
    pstAreaLostIndMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_AREA_LOST_IND_STRU));

    /*判断申请结果，若失败打印错误并退出*/
    if (NAS_EMMC_NULL_PTR == pstAreaLostIndMsg)
    {
        /*打印错误*/
        NAS_LMM_EMMC_LOG_ERR(" NAS_EMMC_SendMmcStopSrchCnf: MSG ALLOC ERR!");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcAreaLostInd_ENUM
,LNAS_NULL_PTR);
        return;

    }

    NAS_LMM_MEM_SET(pstAreaLostIndMsg, 0, sizeof(LMM_MMC_AREA_LOST_IND_STRU));


    /*构造LMM_MMC_AREA_LOST_IND消息*/
    /*填充消息头*/
    NAS_EMMC_COMP_MMC_MSG_HEADER(       pstAreaLostIndMsg,
                                        (sizeof(LMM_MMC_AREA_LOST_IND_STRU)-
                                        NAS_EMMC_LEN_VOS_MSG_HEADER));

    /*填充消息ID*/
    pstAreaLostIndMsg->ulMsgId           = ID_LMM_MMC_AREA_LOST_IND;


    /*填充消息内容*/
    pstAreaLostIndMsg->enAreaLostReason = enAreaLostReason;

    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstAreaLostIndMsg);

}
/*lint -specific(+e433)*/


/*lint -specific(-e433)*/
VOS_VOID  NAS_EMMC_SendRrcCellSelectionReq
(
    LRRC_LNAS_RESEL_TYPE_ENUM_UINT32 ulReselType
)
{
    LRRC_LMM_CELL_SELECTION_CTRL_REQ_STRU *pstCellSelReqMsg = NAS_EMMC_NULL_PTR;

    /*申请消息内存*/
    /*lint -e433 -e826*/
    pstCellSelReqMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(LRRC_LMM_CELL_SELECTION_CTRL_REQ_STRU));
    /*lint +e433 +e826*/
    /*判断申请结果，若失败退出*/
    if (NAS_EMMC_NULL_PTR == pstCellSelReqMsg)
    {
        return;
    }

    NAS_LMM_MEM_SET(pstCellSelReqMsg,0,sizeof(LRRC_LMM_CELL_SELECTION_CTRL_REQ_STRU));

    /*构造RRC_MM_CELL_SELECTION_CTRL_REQ消息*/
    /*填充消息头*/
    NAS_EMMC_COMP_RRC_MSG_HEADER(       pstCellSelReqMsg,
                                        (sizeof(LRRC_LMM_CELL_SELECTION_CTRL_REQ_STRU)-
                                        NAS_EMMC_LEN_VOS_MSG_HEADER));

    /*填充消息ID*/
    pstCellSelReqMsg->enMsgId           = ID_LRRC_LMM_CELL_SELECTION_CTRL_REQ;


    /*填充消息内容*/
    pstCellSelReqMsg->enReselectInd     = ulReselType;


    /*获取禁止TA信息*/
    NAS_EMMC_FillRrcNasForbTaList(&(pstCellSelReqMsg->stForbTaList));

    /*向RRC发送RRC_MM_CELL_SELECTION_CTRL_REQ消息*/
    NAS_LMM_SEND_MSG(                    pstCellSelReqMsg);

    return;
}
/*lint -specific(+e433)*/


VOS_VOID NAS_EMMC_SendRrcBgPlmnSearchReq(const MMC_LMM_BG_PLMN_SEARCH_REQ_STRU *pstMmcMsg)
{
    LRRC_LMM_BG_PLMN_SEARCH_REQ_STRU  *pstLrrcMsg = NAS_EMMC_NULL_PTR;

    /* 入口参数检查 */
    if (NAS_EMMC_NULL_PTR == pstMmcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendRrcBgPlmnSearchReq: input null");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendRrcBgPlmnSearchReq_ENUM
,LNAS_NULL_PTR);
        return;
    }

    /* 申请内存 */
    pstLrrcMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(LRRC_LMM_BG_PLMN_SEARCH_REQ_STRU));
    if (NAS_LMM_NULL_PTR == pstLrrcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendRrcBgPlmnSearchReq: alloc memory failure");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendRrcBgPlmnSearchReq_ENUM
,LNAS_EMM_ALLOC_FAIL);
        return;
    }

    /* 初始化 */
    NAS_LMM_MEM_SET(pstLrrcMsg, 0, sizeof(LRRC_LMM_BG_PLMN_SEARCH_REQ_STRU));

    /* 构造消息头 */
    NAS_EMMC_COMP_ERMM_MSG_HEADER(pstLrrcMsg, (sizeof(LRRC_LMM_BG_PLMN_SEARCH_REQ_STRU) - NAS_EMMC_LEN_VOS_MSG_HEADER));

    /* 填充消息ID */
    pstLrrcMsg->enMsgId = ID_LRRC_LMM_BG_PLMN_SEARCH_REQ;

    /* 填充消息内容 */
    NAS_LMM_MEM_CPY(&pstLrrcMsg->stSpecPlmnWithRat,
                    &pstMmcMsg->stSpecPlmnWithRat,
                    sizeof(LRRC_LMM_PLMN_ID_WITH_RAT_STRU));

    if (pstMmcMsg->ulForbRoamLaiNum > LRRC_LNAS_BG_MAX_FORBLA_NUM)
    {
        pstLrrcMsg->ulForbRoamLaiNum = LRRC_LNAS_BG_MAX_FORBLA_NUM;
    }
    else
    {
        pstLrrcMsg->ulForbRoamLaiNum = pstMmcMsg->ulForbRoamLaiNum;
    }

    NAS_LMM_MEM_CPY(pstLrrcMsg->astForbRoamLaiList,
                    pstMmcMsg->astForbRoamLaiList,
                    pstLrrcMsg->ulForbRoamLaiNum*sizeof(LRRC_LNAS_LA_STRU));

    /* 发送消息 */
    NAS_LMM_SEND_MSG(pstLrrcMsg);

    return;
}
VOS_VOID NAS_EMMC_SendRrcBgPlmnSearchStopReq(const MMC_LMM_STOP_BG_PLMN_SEARCH_REQ_STRU *pstMmcMsg)
{
    LRRC_LMM_BG_PLMN_SEARCH_REQ_STOP_STRU *pLrrcMsg = NAS_EMMC_NULL_PTR;

    /* 入口参数检查 */
    if (NAS_LMM_NULL_PTR == pstMmcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendRrcBgPlmnSearchStopReq: input null");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendRrcBgPlmnSearchStopReq_ENUM
,LNAS_NULL_PTR);
        return;
    }

    /* 申请内存 */
    pLrrcMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(LRRC_LMM_BG_PLMN_SEARCH_REQ_STOP_STRU));
    if (NAS_LMM_NULL_PTR == pLrrcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendRrcBgPlmnSearchStopReq: alloc memory failure");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendRrcBgPlmnSearchStopReq_ENUM
,LNAS_EMM_ALLOC_FAIL);
        return;
    }

    /* 初始化 */
    NAS_LMM_MEM_SET(pLrrcMsg, 0, sizeof(LRRC_LMM_BG_PLMN_SEARCH_REQ_STOP_STRU));

    /* 构造消息头 */
    NAS_EMMC_COMP_ERMM_MSG_HEADER(pLrrcMsg, (sizeof(LRRC_LMM_BG_PLMN_SEARCH_REQ_STOP_STRU) - NAS_EMMC_LEN_VOS_MSG_HEADER));

    /* 填充消息ID */
    pLrrcMsg->enMsgId = ID_LRRC_LMM_BG_PLMN_SEARCH_STOP_REQ;

    /* 填充消息内容 */

    /* 发送消息 */
    NAS_LMM_SEND_MSG(pLrrcMsg);

    return;
}

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
VOS_VOID NAS_EMMC_SendRrcBgSearchHrpdReq(VOS_VOID)
{
    LRRC_LMM_BG_SEARCH_HRPD_REQ_STRU   *pLrrcMsg = NAS_EMMC_NULL_PTR;

    /* 申请内存 */
    pLrrcMsg = (LRRC_LMM_BG_SEARCH_HRPD_REQ_STRU*)NAS_LMM_ALLOC_MSG(sizeof(LRRC_LMM_BG_SEARCH_HRPD_REQ_STRU));
    if (NAS_LMM_NULL_PTR == pLrrcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendRrcBgSearchHrpdReq: alloc memory fail.");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendRrcBgSearchHrpdReq_ENUM,LNAS_EMM_ALLOC_FAIL);
        return;
    }

    /* 初始化 */
    NAS_LMM_MEM_SET(pLrrcMsg, 0, sizeof(LRRC_LMM_BG_SEARCH_HRPD_REQ_STRU));

    /* 构造消息头 */
    NAS_EMMC_COMP_ERMM_MSG_HEADER(pLrrcMsg, (sizeof(LRRC_LMM_BG_SEARCH_HRPD_REQ_STRU) - NAS_EMMC_LEN_VOS_MSG_HEADER));

    /* 填充消息ID */
    pLrrcMsg->enMsgId = ID_LRRC_LMM_BG_SEARCH_HRPD_REQ;

    /* 填充消息内容 */

    /* 发送消息 */
    NAS_LMM_SEND_MSG(pLrrcMsg);

    return;
}


VOS_VOID NAS_EMMC_SendRrcBgSearchHrpdStopReq(VOS_VOID)
{
    LRRC_LMM_BG_SEARCH_HRPD_STOP_REQ_STRU *pLrrcMsg = NAS_EMMC_NULL_PTR;

    /* 申请内存 */
    pLrrcMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(LRRC_LMM_BG_SEARCH_HRPD_STOP_REQ_STRU));
    if (NAS_LMM_NULL_PTR == pLrrcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendRrcBgSearchHrpdStopReq: alloc memory failure");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendRrcBgSearchHrpdStopReq_ENUM,LNAS_EMM_ALLOC_FAIL);
        return;
    }

    /* 初始化 */
    NAS_LMM_MEM_SET(pLrrcMsg, 0, sizeof(LRRC_LMM_BG_SEARCH_HRPD_STOP_REQ_STRU));

    /* 构造消息头 */
    NAS_EMMC_COMP_ERMM_MSG_HEADER(pLrrcMsg, (sizeof(LRRC_LMM_BG_SEARCH_HRPD_STOP_REQ_STRU) - NAS_EMMC_LEN_VOS_MSG_HEADER));

    /* 填充消息ID */
    pLrrcMsg->enMsgId = ID_LRRC_LMM_BG_SEARCH_HRPD_STOP_REQ;

    /* 填充消息内容 */

    /* 发送消息 */
    NAS_LMM_SEND_MSG(pLrrcMsg);

    return;
}
#endif


VOS_VOID NAS_EMMC_SendMmcBgPlmnSearchCnf(const LRRC_LMM_BG_PLMN_SEARCH_CNF_STRU *pstLrrcMsg)
{
    LMM_MMC_BG_PLMN_SEARCH_CNF_STRU *pstMmcMsg = NAS_LMM_NULL_PTR;

    /* 入口参数检查 */
    if (NAS_LMM_NULL_PTR == pstLrrcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendMmcBgPlmnSearchCnf: input null");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcBgPlmnSearchCnf_ENUM
,LNAS_NULL_PTR);
        return;
    }

    /* 申请MMC内部消息 */
    pstMmcMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_BG_PLMN_SEARCH_CNF_STRU));
    if (NAS_LMM_NULL_PTR == pstMmcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendMmcBgPlmnSearchCnf: alloc memory failure");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcBgPlmnSearchCnf_ENUM
,LNAS_EMM_ALLOC_FAIL);
        return;
    }

    /* 初始化 */
    NAS_LMM_MEM_SET(pstMmcMsg, 0, sizeof(LMM_MMC_BG_PLMN_SEARCH_CNF_STRU));

    /* 构造消息头 */
    NAS_EMMC_COMP_MMC_MSG_HEADER(pstMmcMsg, (sizeof(LMM_MMC_BG_PLMN_SEARCH_CNF_STRU) - NAS_EMMC_LEN_VOS_MSG_HEADER));

    /* 填充消息ID */
    pstMmcMsg->ulMsgId = ID_LMM_MMC_BG_PLMN_SEARCH_CNF;

    /* 填充消息内容 */
    switch(pstLrrcMsg->enRlst)
    {
    case LRRC_LMM_BG_SRCH_RLT_SUCC:
        pstMmcMsg->enRlst = MMC_LMM_BG_SRCH_RLT_SUCCESS;
        break;
    case LRRC_LMM_BG_SRCH_RLT_FAIL:
        pstMmcMsg->enRlst = MMC_LMM_BG_SRCH_RLT_FAIL;
        break;
    case LRRC_LMM_BG_SRCH_RLT_ABORT:
        pstMmcMsg->enRlst = MMC_LMM_BG_SRCH_RLT_ABORT;
        break;
    default:
        pstMmcMsg->enRlst = MMC_LMM_BG_SRCH_RLT_BUTT;
    }

    if (pstLrrcMsg->ulPlmnNum > MMC_LMM_MAX_BG_SRCH_PLMN_NUM)
    {
        pstMmcMsg->ulPlmnNum = MMC_LMM_MAX_BG_SRCH_PLMN_NUM;
    }
    else
    {
        pstMmcMsg->ulPlmnNum = pstLrrcMsg->ulPlmnNum;
    }

    NAS_LMM_MEM_CPY(pstMmcMsg->astPlmnIdWithRatList,
                    pstLrrcMsg->astPlmnIdWithRatList,
                    sizeof(MMC_LMM_PLMN_ID_WITH_RAT_STRU)*pstMmcMsg->ulPlmnNum);

    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID *)pstMmcMsg);

    return;
}
VOS_VOID NAS_EMMC_SendMmcBgPlmnSearchStopCnf(const LRRC_LMM_BG_PLMN_SEARCH_STOP_CNF_STRU *pstLrrcMsg)
{
    LMM_MMC_STOP_BG_PLMN_SEARCH_CNF_STRU *pstMmcMsg = NAS_LMM_NULL_PTR;

    /* 入口参数检查 */
    if (NAS_EMMC_NULL_PTR == pstLrrcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendMmcBgPlmnSearchStopCnf: input null");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcBgPlmnSearchStopCnf_ENUM
,LNAS_NULL_PTR);
        return;
    }

    /* 申请MMC内部消息 */
    pstMmcMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_STOP_BG_PLMN_SEARCH_CNF_STRU));
    if (NAS_LMM_NULL_PTR == pstMmcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendMmcBgPlmnSearchStopCnf: alloc memory failure");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcBgPlmnSearchStopCnf_ENUM
,LNAS_EMM_ALLOC_FAIL);
        return;
    }

    (VOS_VOID)pstLrrcMsg;

    /* 初始化 */
    NAS_LMM_MEM_SET(pstMmcMsg, 0, sizeof(LMM_MMC_STOP_BG_PLMN_SEARCH_CNF_STRU));

    /* 构造消息头 */
    NAS_EMMC_COMP_MMC_MSG_HEADER(pstMmcMsg, (sizeof(LMM_MMC_STOP_BG_PLMN_SEARCH_CNF_STRU) - NAS_EMMC_LEN_VOS_MSG_HEADER));

    /* 填充消息ID */
    pstMmcMsg->ulMsgId = ID_LMM_MMC_STOP_BG_PLMN_SEARCH_CNF;

    /* 填充消息内容 */

    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID *)pstMmcMsg);

    return;
}
VOS_VOID NAS_EMMC_SendMmcNotCampOnInd(VOS_VOID)
{
    LMM_MMC_NOT_CAMP_ON_IND_STRU *pstMmcMsg = NAS_LMM_NULL_PTR;

    /* 申请MMC内部消息 */
    pstMmcMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_NOT_CAMP_ON_IND_STRU));
    if (NAS_LMM_NULL_PTR == pstMmcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendMmcNotCampOnInd: alloc memory failure");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcNotCampOnInd_ENUM
,LNAS_EMM_ALLOC_FAIL);
        return;
    }

    /* 初始化 */
    NAS_LMM_MEM_SET(pstMmcMsg, 0, sizeof(LMM_MMC_NOT_CAMP_ON_IND_STRU));

    /* 构造消息头 */
    NAS_EMMC_COMP_MMC_MSG_HEADER(pstMmcMsg, (sizeof(LMM_MMC_NOT_CAMP_ON_IND_STRU) - NAS_EMMC_LEN_VOS_MSG_HEADER));

    /* 填充消息ID */
    pstMmcMsg->ulMsgId = ID_LMM_MMC_NOT_CAMP_ON_IND;

    /* 填充消息内容 */

    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID *)pstMmcMsg);

    return;
}



VOS_VOID NAS_EMMC_SendMmcSearchPlmnInfoInd
(
    const LRRC_LMM_SEARCHED_PLMN_INFO_IND_STRU *pstLrrcMsg
)
{
    LMM_MMC_SEARCHED_PLMN_INFO_IND_STRU  *pstMmcMsg = NAS_LMM_NULL_PTR;

    /* 申请MMC内部消息 */
    pstMmcMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_SEARCHED_PLMN_INFO_IND_STRU));

    if (NAS_LMM_NULL_PTR == pstMmcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendMmcSearchPlmnInfoInd: alloc memory failure");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcSearchPlmnInfoInd_ENUM
,LNAS_EMM_ALLOC_FAIL);
        return;
    }

    /* 初始化 */
    NAS_LMM_MEM_SET(pstMmcMsg, 0, sizeof(LMM_MMC_SEARCHED_PLMN_INFO_IND_STRU));

    /* 构造消息头 */
    NAS_EMMC_COMP_MMC_MSG_HEADER(pstMmcMsg, (sizeof(LMM_MMC_SEARCHED_PLMN_INFO_IND_STRU) - NAS_EMMC_LEN_VOS_MSG_HEADER));

    /* 填充消息ID */
    pstMmcMsg->ulMsgId = ID_LMM_MMC_SEARCHED_PLMN_INFO_IND;

    /* 填充消息内容 */
    pstMmcMsg->ulTaiNum = pstLrrcMsg->ulTaiNum;

    NAS_LMM_MEM_CPY(pstMmcMsg->stTaiList, pstLrrcMsg->stTaiList,
                    (LRRC_LMM_MAX_SEARCHED_TAI_NUM)*sizeof(LRRC_LNAS_TA_STRU));

    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID *)pstMmcMsg);

    return;

}

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

VOS_VOID NAS_EMMC_SendMmcBgSearchHrpdCnf
(
    const LRRC_LMM_BG_SEARCH_HRPD_CNF_STRU       *pstLrrcMsg
)
{
    LMM_MMC_BG_SEARCH_HRPD_CNF_STRU    *pstMmcMsg = NAS_LMM_NULL_PTR;

    /* 申请MMC内部消息 */
    pstMmcMsg = (LMM_MMC_BG_SEARCH_HRPD_CNF_STRU*)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_BG_SEARCH_HRPD_CNF_STRU));

    if (NAS_LMM_NULL_PTR == pstMmcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendMmcBgSearchHrpdCnf: alloc memory failure");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcBgSearchHrpdCnf_ENUM,LNAS_EMM_ALLOC_FAIL);
        return;
    }

    /* 初始化 */
    NAS_LMM_MEM_SET(pstMmcMsg, 0, sizeof(LMM_MMC_BG_SEARCH_HRPD_CNF_STRU));

    /* 构造消息头 */
    NAS_EMMC_COMP_MMC_MSG_HEADER(pstMmcMsg, (sizeof(LMM_MMC_BG_SEARCH_HRPD_CNF_STRU) - NAS_EMMC_LEN_VOS_MSG_HEADER));

    /* 填充消息ID和Opid */
    pstMmcMsg->ulMsgId                  = ID_LMM_MMC_BG_SEARCH_HRPD_CNF;
    pstMmcMsg->ulOpId                   = 0;

    /* 填充消息内容 */
    pstMmcMsg->enRlst                   = pstLrrcMsg->enRslt;
    pstMmcMsg->ucHrpdSysNum             = pstLrrcMsg->ucHrpdSysNum;

    NAS_LMM_MEM_CPY(pstMmcMsg->astHrpdSysList,
                    pstLrrcMsg->astHrpdSysList,
                    (LRRC_LMM_BG_HRPD_MAX_SUBNET_NUM)*sizeof(LMM_MMC_HRPD_SYS_STRU));

    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID *)pstMmcMsg);

    return;

}


VOS_VOID NAS_EMMC_SendMmcStopBgSearchHrpdCnf(VOS_VOID)
{
    LMM_MMC_STOP_BG_SEARCH_HRPD_CNF_STRU         *pstMmcMsg = NAS_LMM_NULL_PTR;

    /* 申请MMC内部消息 */
    pstMmcMsg = (LMM_MMC_STOP_BG_SEARCH_HRPD_CNF_STRU*)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_STOP_BG_SEARCH_HRPD_CNF_STRU));

    if (NAS_LMM_NULL_PTR == pstMmcMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_SendMmcStopBgSearchHrpdCnf: alloc memory failure");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendMmcStopBgSearchHrpdCnf_ENUM,LNAS_EMM_ALLOC_FAIL);
        return;
    }

    /* 初始化 */
    NAS_LMM_MEM_SET(pstMmcMsg, 0, sizeof(LMM_MMC_STOP_BG_SEARCH_HRPD_CNF_STRU));

    /* 构造消息头 */
    NAS_EMMC_COMP_MMC_MSG_HEADER(pstMmcMsg, (sizeof(LMM_MMC_STOP_BG_SEARCH_HRPD_CNF_STRU) - NAS_EMMC_LEN_VOS_MSG_HEADER));

    /* 填充消息ID和Opid */
    pstMmcMsg->ulMsgId                  = ID_LMM_MMC_STOP_BG_SEARCH_HRPD_CNF;
    pstMmcMsg->ulOpId                   = 0;

    /* 填充消息内容 */

    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID *)pstMmcMsg);

    return;

}
#endif


VOS_VOID    NAS_EMMC_SendEsmPlmnChangeInd (VOS_VOID)
{
    EMM_ESM_PLMN_CHANGE_IND_STRU     *pEmmEsmPlmnChangeMsg;

    pEmmEsmPlmnChangeMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(EMM_ESM_PLMN_CHANGE_IND_STRU));

    /*判断申请结果，若失败打印错误并退出*/
    if (NAS_EMM_NULL_PTR == pEmmEsmPlmnChangeMsg)
    {
        /*打印错误*/
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMMC_SendEsmPlmnChangeInd: MSG ALLOC ERR!");
        TLPS_PRINT2LAYER_ERROR(NAS_EMMC_SendEsmPlmnChangeInd_ENUM
,LNAS_NULL_PTR);
        return;
    }

    /*填充消息头*/
    NAS_EMM_COMP_AD_ESM_MSG_HEADER(pEmmEsmPlmnChangeMsg,
                                    (sizeof(EMM_ESM_PLMN_CHANGE_IND_STRU)-
                                    NAS_EMM_LEN_VOS_MSG_HEADER));

    /*填充消息ID*/
    pEmmEsmPlmnChangeMsg->ulMsgId  = ID_EMM_ESM_PLMN_CHANGE_IND;

    /*向ESM发送ID_EMM_ESM_STOP_NOTIFY_IND消息*/
    NAS_LMM_SEND_MSG(pEmmEsmPlmnChangeMsg);

    return;

}
/*lint +e961*/
/*lint +e960*/
/*lint +e72*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif










