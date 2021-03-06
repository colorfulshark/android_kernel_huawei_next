/************************************************************************
*                                                                      *
*                             Vsncp.c                                  *
*                                                                      *
*  Project Code:                                                       *
*  Create Date:        2015/05/25                                      *
*  Author:             TIAN CHENCHEN                                   *
*  Modify Date:                                                        *
*  Document:                                                           *
*  Function:           PPP的VSNCP协议模块                              *
*  Others:                                                             *
*----------------------------------------------------------------------*
*                                                                      *
*   这个文件定义了VSNCP协议模块的全部接口函数和内部处理函数            *
*                                                                      *
************************************************************************/



/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "ppp_inc.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/
#define         THIS_FILE_ID            PS_FILE_ID_PPPC_VSNCP_C

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/

VOID PPP_PDNSendVSNCPReject(ULONG ulRPIndex,
                            UCHAR *pucMsg,
                            UCHAR ucErrCode);
ULONG PPP_VSNCP_CheckOUI(UCHAR *pPacket, ULONG ulLen);

/* PDN上下文 */
PDN_CONTEXT_S g_stPDNContext[A11_MAX_PDN_NUM];

/* VSNCP回调函数数据结构 */
PPPFSMCALLBACK_S g_stVsncpCallbacks =
{
    /* 动作：协商报文处理 */
    PPP_VSNCP_resetci,   /* 根据配置信息设置协商初始值 */
    PPP_VSNCP_cilen,     /* 我要发送的request报文长度 */
    PPP_VSNCP_addci,     /* 组织协商报文 */
    PPP_VSNCP_ackci,     /* 处理对方的ack报文 */
    VOS_NULL_PTR,                /* 处理对方的nak报文 */
    PPP_VSNCP_rejci,     /* 处理对方的reject报文 */
    PPP_VSNCP_reqci,     /* 处理对方的request报文 */
    VOS_NULL_PTR,                /* 处理协议特有的报文，如protocol reject等 */
    /* 事件：协议转入了终止状态 */
    PPP_VSNCP_up,        /* 协议up */
    PPP_VSNCP_down,      /* 协议暂时down,马上要重新协商.
                           与finished的区别是:对于PPPOE、DDR等的情况，
                           不用通知下层拆除链路 */
    PPP_VSNCP_finished,  /* 协议结束，通知底层:拆除链路.对于LCP,只要不是
                           PPPOE、DDR等的情况,则会设置reset定时器,
                           超时后重新启动协商 */
    PPP_VSNCP_starting,  /* tls(This-Layer-Started)动作中，通知下层:我准
                           备好了重新协商,如可能,请发一个up事件.
                           协议对这个动作没有具体规定,可以不实现*/
    /* 子协议信息 */
    "VSNCP"               /* 子协议名 */
};

/*****************************************************************************
  3 函数实现
*****************************************************************************/
/*lint -save -e958 */

VOID PPP_VSNCP_ClearPdnContext(VOS_UINT8 ucPdnId)
{
    if (ucPdnId < A11_MAX_PDN_NUM)
    {
        PS_MEM_SET(&(g_stPDNContext[ucPdnId].stEhrpd.stApn),
            0, sizeof(CNAS_CTTF_EHRPD_APN_STRU));
        PS_MEM_SET(&(g_stPDNContext[ucPdnId].stEhrpd.stPdnAddress),
            0, sizeof(CNAS_CTTF_EHRPD_PDN_ADDRESS_STRU));
        PS_MEM_SET(&(g_stPDNContext[ucPdnId].stEhrpd.stPco),
            0, sizeof(CNAS_CTTF_EHRPD_PCO_STRU));
        PS_MEM_SET(&(g_stPDNContext[ucPdnId].stEhrpd.aucIpv4DefaultRouterAddress),
            0, CNAS_CTTF_EHRPD_IPV4_ADDRESS_LEN);
        PS_MEM_SET(&(g_stPDNContext[ucPdnId].stEhrpd.stApnAmbr),
            0, sizeof(CNAS_CTTF_EHRPD_APN_AMBR_STRU));
        g_stPDNContext[ucPdnId].stEhrpd.enAttachType    = 0;
        g_stPDNContext[ucPdnId].stEhrpd.enPdnType       = CNAS_CTTF_EHRPD_PDN_TYPE_INVALID;
        g_stPDNContext[ucPdnId].stEhrpd.ucAddressAllocCause    = 0;
        g_stPDNContext[ucPdnId].stEhrpd.enErrorCode     = 0;
    }
    else
    {
        PPPC_WARNING_LOG1("Ivalid pdn id!\r\n", ucPdnId);
    }

    return;
}


VOID PPP_CORE_VSNCPFinishProc(PPPINFO_S *pstPppInfo)
{
    PPPVSNCPINFO_S *pstVsncpInfo = VOS_NULL_PTR;

    if ((VOS_NULL_PTR == pstPppInfo->pstLcpInfo) || (VOS_NULL_PTR == pstPppInfo->pstVsncpInfo))
    {
        PPPC_WARNING_LOG("Null pointer!\r\n");
        return;
    }

    pstVsncpInfo = (PPPVSNCPINFO_S *)pstPppInfo->pstVsncpInfo;

    /*添加对于定时器的保护*/
    if (pstVsncpInfo->stFsm.ulTimeOutID)
    {
        (VOID)VOS_StopRelTimer(&(pstVsncpInfo->stFsm.ulTimeOutID));
        pstVsncpInfo->stFsm.ulTimeOutID = 0;
    }

    pstVsncpInfo->ucRenegoFlag = VOS_FALSE;
    pstPppInfo->pstVsncpInfo = VOS_NULL_PTR;


    /* 使用标记删除 */
    pstVsncpInfo->ucUsedFlag = VOS_FALSE;

    return;
}


ULONG PPP_Shell_ReceiveVSNCPUpFromCore(PPPINFO_S *pstPppInfo)
{
    PPPVSNCPINFO_S *pstVSNCPInfo = VOS_NULL_PTR;
    ULONG64 ulNegoTime = 0;

    PPP_DebugOut(MID_PPPC,
                 PPP_DEBUG_INFO,
                 "\r\n [ppp]PPP_Shell_ReceiveVSNCPUpFromCore: Enter");

    pstVSNCPInfo = (PPPVSNCPINFO_S *)pstPppInfo->pstVsncpInfo;

    /* 删除协商定时器 */
    (VOID)PPP_NodeDelete(pstPppInfo->ulRPIndex);
    (VOID)PPP_PDNNodeDelete(pstVSNCPInfo->ulPDNIndex);

    if (pstVSNCPInfo->ucRenegoFlag)
    {
        pstVSNCPInfo->ucRenegoFlag = 0;
        PPP_DBG_OK_CNT(PPP_PHOK_1383);
    }

    /* 性能统计:IPCP协商成功次数 */
    //PPP_PerfInc(g_stPppPerfStatistic.pulIpcpSuccess, PERF_TYPE_PPPC_IPCP_NEGO_SUCC_NUM);
    PPP_DBG_OK_CNT(PPP_PHOK_1384);

    /* 稳态连接标记置成功 */
    //pstPppInfo->bPppStateFlg = PPP_STATE_SUCCESS;
    M_SET_NEWPHASE(pstPppInfo, PPP_SNMP_PERF_PHASE_SUCCESS);
    PPP_SNMP_PERF_SET_SUCCESS(pstPppInfo);

    if (0 == pstPppInfo->ulNegoEndTime)
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1385);
        pstPppInfo->ulNegoEndTime = VOS_GetTick();
        ulNegoTime = pstPppInfo->ulNegoEndTime - pstPppInfo->ulNegoStartTime;
        A11_OM_HSGWPerfStatisticPCFUnit(PERF_TYPE_EHRPD_PCF_PPP_TOTAL_TIME,
                                        pstPppInfo->ulPcfIP,
                                        A11_PERF_OPERATOR_PLUS,
                                        (ULONG)ulNegoTime);
        A11_OM_HSGWPerfStatisticPCFUnit(PERF_TYPE_EHRPD_PCF_PPP_NEGO_NUM,
                                        pstPppInfo->ulPcfIP,
                                        A11_PERF_OPERATOR_PLUS,
                                        1);
    }

    /* PPP链路建立成功 */
    pstPppInfo->bPppSuccess = 1;

    /* 通知备份 */
    /* (VOID)PPP_SendUserInfo(pstPppInfo, VSNCP_INFOR); */


    /* (VOID)PPP_NotifyIpv6SendRA(pstPppInfo->ulRPIndex, pstVSNCPInfo->ucPDNID); */
    return VOS_OK;
}
VOID PPP_VSNCP_FSM_SetRetransmitTimer(PPPFSM_S *pstFsm)
{
    PPPVSNCPINFO_S *pstVsncpInfo = VOS_NULL_PTR;
    PPPINFO_S *pstPppInfo = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    ULONG ulPdpIndex = 0;
    ULONG ulRet = 0;

    pstPppInfo = (PPPINFO_S *)(pstFsm->pPppInfo);
    pstVsncpInfo = (PPPVSNCPINFO_S *)(pstFsm->pProtocolInfo);

    if ((VOS_NULL_PTR == pstPppInfo) || (VOS_NULL_PTR == pstVsncpInfo))
    {
        PPPC_WARNING_LOG("PPP_VSNCP_FSM_SetRetransmitTimer Null pointer!\r\n");
        return;
    }

    ulRet = PPP_GetPDNContextByPDNID(pstVsncpInfo->ucPDNID,
                                     &pstPDNContext);
    if ((VOS_OK != ulRet) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG1("PPP_GetPDNContextByPDNID!\r\n", pstVsncpInfo->ucPDNID);
        return;
    }


    if (0 == pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID)
    {
        pstPppInfo->pulCurrFsm = pstFsm;

        /* ulPdpIndex最高2Byte作为IPCP/CCP/LCP/VSNCP控制位,由于用户最多50000/SC 高两个Byte不使用*/
        ulPdpIndex = pstVsncpInfo->ulPDNIndex;
        ulPdpIndex |= (ULONG)pstFsm->usProtocol << 16;

        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                      "\r\n [ppp]PPP_VSNCP_FSM_SetRetransmitTimer: ulPdpIndex = 0x%x ",
                      ulPdpIndex);

        ulRet = VOS_StartRelTimer((HTIMER)&(pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID),
                                    MID_PPPC,
                                    pstFsm->ulTimeOutTime,
                                    ulPdpIndex,
                                    PPPC_FSM_RETRANSMIT_TIMER,
                                    VOS_TIMER_NOLOOP,
                                    VOS_TIMER_PRECISION_0);

        if (ulRet != VOS_OK)
        {
            PPPC_WARNING_LOG1("VOS_StartRelTimer fail!\r\n", ulRet);
        }

        PPP_DBG_OK_CNT(PPP_PHOK_1386);

        VOS_DBGASSERT(ulRet == 0);
    }

    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n [ppp]PPP_VSNCP_FSM_SetRetransmitTimer :ulPDNIndex[%u]ucPDNID[%u]ulTimeOutID[%u]! ! ",
                 pstVsncpInfo->ulPDNIndex, pstVsncpInfo->ucPDNID,
                 pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID);

    PPP_DBG_OK_CNT(PPP_PHOK_1387);
    return;
}
ULONG PPP_VSNCP_FSM_DelRetransmitTimer(PPPFSM_S *pstFsm)
{
    PPPVSNCPINFO_S *pstVsncpInfo = VOS_NULL_PTR;
    PPPINFO_S *pstPppInfo = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    ULONG ulRet = 0;

    pstPppInfo = (PPPINFO_S *)(pstFsm->pPppInfo);
    pstVsncpInfo = (PPPVSNCPINFO_S *)(pstFsm->pProtocolInfo);

    if ((VOS_NULL_PTR == pstPppInfo) || (VOS_NULL_PTR == pstVsncpInfo))
    {
        PPPC_WARNING_LOG("null pointer!\r\n");
        return VOS_ERR;
    }

    ulRet = PPP_GetPDNContextByPDNID(pstVsncpInfo->ucPDNID,
                                     &pstPDNContext);
    if ((VOS_OK != ulRet) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG1("PPP_GetPDNContextByPDNID fail!\r\n", pstVsncpInfo->ucPDNID);
        return VOS_ERR;
    }

    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n [ppp]PPP_VSNCP_FSM_DelRetransmitTimer :ulPDNIndex[%u]ucPDNID[%u]ulTimeOutID[%u]! ! ",
                 pstVsncpInfo->ulPDNIndex, pstVsncpInfo->ucPDNID,
                 pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID);

    if (pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID)
    {
        (VOID)VOS_StopRelTimer(&(pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID));
        pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID = 0;
        PPP_DBG_OK_CNT(PPP_PHOK_1388);
    }

    PPP_DBG_OK_CNT(PPP_PHOK_1389);

    return VOS_OK;
}



VOID PPP_VSNCP_Clear(PPPVSNCPINFO_S *pstVsncpInfo, PDN_CONTEXT_S *pstPDNContext)
{
    if ((VOS_NULL_PTR == pstVsncpInfo) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG("null pointer!\r\n");

        return;
    }

    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n [ppp]PPP_VSNCP_Clear :ulPDNIndex[%u]ucPDNID[%u]ulTimeOutID[%u]! ",
                 pstVsncpInfo->ulPDNIndex, pstVsncpInfo->ucPDNID,
                 pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID);

    if (pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID)
    {
        (VOID)VOS_StopRelTimer(&pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID);
        pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID = 0;
    }

    pstPDNContext->stEhrpd.stVSNCP.ucGetPCOFlag = VOS_FALSE;

    if (pstPDNContext->stEhrpd.stVSNCP.ulPmipv6UpdTimeOutID)
    {
        PDN_FsmStopTimer(TIMER_TYPE_PDN_IPV6_FSM_TIEMR,
                         pstPDNContext->stEhrpd.stVSNCP.ulPmipv6UpdTimeOutID);

        pstPDNContext->stEhrpd.stVSNCP.ulPmipv6UpdTimeOutID = 0;
    }
    if (pstPDNContext->stEhrpd.stVSNCP.ulReconnectDnsTimeOutID)
    {
        PDN_FsmStopTimer(TIMER_TYPE_PDN_IPV6_FSM_TIEMR,
                         pstPDNContext->stEhrpd.stVSNCP.ulReconnectDnsTimeOutID);

        pstPDNContext->stEhrpd.stVSNCP.ulReconnectDnsTimeOutID = 0;
    }
    PPP_MemSet((UCHAR *)&(pstVsncpInfo->stFsm), 0, sizeof(PPPFSM_S));

    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n [ppp]PPP_VSNCP_Clear :success! ");
    PPP_DBG_OK_CNT(PPP_PHOK_1390);

    return;
}



VOID PPP_VSNCP_ClearALL(PPPINFO_S *pstPppInfo)
{
    PPPVSNCPINFO_S stVSNCPInfo = {0};
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    UCHAR ucPDNId = 0;
    ULONG ulRet;

    if (VOS_NULL_PTR == pstPppInfo)
    {
        PPPC_WARNING_LOG("pstPppInfo is NULL!\r\n");
        return;
    }

    for (ucPDNId = 0; ucPDNId < (VSNCP_OPTION_PDNID_MAX + 1); ucPDNId++)
    {
        pstPDNContext = VOS_NULL_PTR;
        ulRet = PPP_GetPDNContextByPDNID(ucPDNId, &pstPDNContext);
        if ((ulRet == VOS_OK) && (VOS_NULL_PTR != pstPDNContext))
        {
            /* 初始化VSNCP控制块 */
            PPP_VSNCP_Init(pstPppInfo, &stVSNCPInfo, pstPDNContext, ucPDNId);

            PPP_VSNCP_Clear(&stVSNCPInfo, pstPDNContext);
            /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
            PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);

            /* 释放之前的缓存消息 */
            if (pstPDNContext->stEhrpd.pucVsncpReq != VOS_NULL_PTR)
            {
                PPP_Free(pstPDNContext->stEhrpd.pucVsncpReq);
                pstPDNContext->stEhrpd.pucVsncpReq = VOS_NULL_PTR;
            }
            pstPDNContext->stEhrpd.ucbit1L2CReconnectFlg = VOS_FALSE;

            /* 停止IPv6定时器，不再发生RA消息 */
            if (VOS_OK != IPV6_StopTimer(pstPDNContext->ulIpv6Timer))
            {
               /* 增加调试信息，但不退出 */
               PPPC_WARNING_LOG("IPV6_StopTimer fail!\r\n");
            }
            pstPDNContext->ulIpv6Timer = 0;

        }

        PPP_VSNCP_ClearPdnContext(ucPDNId);
    }

    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n [ppp]PPP_VSNCP_ClearALL :success! ");
    PPP_DBG_OK_CNT(PPP_PHOK_1392);
    return;
}
VOID PPP_VSNCP_SetRenegoFlagForAllPDN(PPPINFO_S *pstPppInfo)
{
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    UCHAR ucPDNId = 0;
    ULONG ulRet = 0;

    if (VOS_NULL_PTR == pstPppInfo)
    {
        PPPC_WARNING_LOG("pstPppInfo is NULL!\r\n");

        return;
    }

    for (ucPDNId = 0; ucPDNId < (VSNCP_OPTION_PDNID_MAX + 1); ucPDNId++)
    {
        pstPDNContext = VOS_NULL_PTR;
        ulRet = PPP_GetPDNContextByPDNID(ucPDNId,
                                         &pstPDNContext);
        if ((ulRet == VOS_OK) && (VOS_NULL_PTR != pstPDNContext))
        {
            pstPDNContext->stEhrpd.stVSNCP.ucRenegoFlag = VOS_TRUE;
        }
    }

    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n [ppp]PPP_VSNCP_SetRenegoFlagForAllPDN :success! ");
    PPP_DBG_OK_CNT(PPP_PHOK_1393);
    return;
}


ULONG PPP_GetPDNContextByPDNID(UCHAR ucPDNId, PDN_CONTEXT_S **ppstPDNContext)
{
    PS_BOOL_ENUM_UINT8                  enIsUsing;

    enIsUsing = PPPC_IsPdnIdUsing(ucPDNId);

    if (PS_FALSE == enIsUsing)
    {
        return VOS_ERR;
    }

    *ppstPDNContext = &(g_stPDNContext[ucPDNId]);

    return VOS_OK;
}


PS_BOOL_ENUM_UINT8 PPP_VSNCP_IsPDNOpened(UCHAR ucPDNId)
{
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    ULONG ulRet = 0;

    /* 取得PdnContext */
    ulRet = PPP_GetPDNContextByPDNID(ucPDNId, &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG1("PPP_GetPDNContextByPDNID fail!\r\n", ulRet);

        return PS_FALSE;
    }

    if (PPP_STATE_OPENED == pstPDNContext->stEhrpd.stVSNCP.ucState)
    {
        return PS_TRUE;
    }

    return PS_FALSE;
}

#if 0
ULONG PPP_VSNCP_GetPDNContextByPDNID(ULONG ulRpIdx, UCHAR ucPDNId, PDN_CONTEXT_S **ppstPDNContext)
{
    SDB_RETURNCODE_E enSdbResult = SDB_SUCCESS;
    SDB_PDSN_RP_CONTEXT_S *pstRpContext = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;

    if (VOS_NULL == ppstPDNContext)
    {
        PPPC_WARNING_LOG("ppstPDNContext is null!\r\n");
        return VOS_ERR;
    }

    if (A11_MAX_PDN_NUM <= ucPDNId)
    {
        PPPC_WARNING_LOG1("PPP_VSNCP_GetPDNContextByPDNID: ucPDNId is ERR!\r\n", ucPDNId);
        return VOS_ERR;
    }

    enSdbResult = SDB_GetRpContextByIndex(ulRpIdx, (UCHAR **)&pstRpContext);
    if ((SDB_SUCCESS != enSdbResult) || (VOS_NULL == pstRpContext))
    {
        PPP_DBG_ERR_CNT(PPP_PHERR_682);
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_VSNCP_GetPDNContextByPDNID: ulRpIdx(%d) is ERR!!pstRpContext(%x)",
                     ulRpIdx,
                     pstRpContext);
        return VOS_ERR;
    }

    if (0 != pstRpContext->ausPdnIndexArray[ucPDNId])
    {
        enSdbResult = SDB_GetPdnContextByIndex((ULONG)pstRpContext->ausPdnIndexArray[ucPDNId],
                                               (UCHAR **)&pstPDNContext);
        if ((SDB_SUCCESS != enSdbResult) || (VOS_NULL == pstPDNContext))
        {
            PPP_DBG_ERR_CNT(PPP_PHERR_683);
            PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                         "\r\n PPP_VSNCP_GetPDNContextByPDNID: ulPdnIndex(%d) is ERR! pstPDNContext(%x)",
                         pstRpContext->ausPdnIndexArray[ucPDNId],
                         pstPDNContext);
            return VOS_ERR;
        }

        *ppstPDNContext = pstPDNContext;

        return VOS_OK;
    }
    else
    {
        PPP_DBG_ERR_CNT(PPP_PHERR_684);
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_VSNCP_GetPDNContextByPDNID: ulPdnIndex(%d) is ERR! ucPDNId(%u)",
                     pstRpContext->ausPdnIndexArray[ucPDNId], ucPDNId);
        return VOS_ERR;
    }
}
#endif


VOID PPP_VSNCP_ReceiveEventFromCore(VOID *pstVsncpInfo, ULONG ulCmd, char *pPara)
{
    PPPFSM_S *pstFsm;

    VOS_DBGASSERT(pstVsncpInfo);
    PPP_TRACE("\r\n---Into PPP_VSNCP_ReceiveEventFromCore ulCmd: %d---\r\n", ulCmd);

    if (VOS_NULL_PTR == pstVsncpInfo)
    {
        PPPC_WARNING_LOG("pstVsncpInfo is null!\r\n");
        return;
    }

    /* 性能统计:VSNCP协商尝试次数 */
    if (PPP_EVENT_FSM_OPEN == ulCmd)
    {
    }

    pstFsm = &(((PPPVSNCPINFO_S*)pstVsncpInfo)->stFsm);

    PPP_FSM_ReceiveEvent(pstFsm, ulCmd, pPara);

    return;
}
VOID PPP_VSNCP_ReceivePacket (VOID *pstVsncpInfo, UCHAR* pHead, UCHAR* pPacket, ULONG ulLen)
{
    PPPFSM_S *pstFsm = VOS_NULL_PTR;
    VOS_UINT8 ucPppId;

    VOS_DBGASSERT(pstVsncpInfo);
    VOS_DBGASSERT(pHead);
    VOS_DBGASSERT(pPacket);

    if ((VOS_NULL_PTR == pstVsncpInfo) || (VOS_NULL_PTR == pPacket))
    {
        PPPC_WARNING_LOG("null pointer!\r\n");

        return;
    }

    pstFsm = &(((PPPVSNCPINFO_S*)pstVsncpInfo)->stFsm);

    ucPppId = PPPC_GetCurrentPppId();
    PPP_FSM_ReceivePacket(pstFsm, pHead, pPacket, ulLen, ucPppId);
    return;

}


VOID PPP_VSNCP_UpdateApnAmbr
(
    PDN_CONTEXT_S                      *pstPDNContext,
    PDN_PPP_VSNCP_REQ_S                *pstVsncpReqInfo
)
{
    UCHAR ucAMBRLen = 0;
    UCHAR *pucAMBR = VOS_NULL_PTR;
    CNAS_CTTF_EHRPD_APN_AMBR_STRU *pstApnAmbr = VOS_NULL_PTR;

    ucAMBRLen = pstVsncpReqInfo->ucAMBRLen;
    pucAMBR = pstVsncpReqInfo->aucAMBR;
    pstApnAmbr = &(pstPDNContext->stEhrpd.stApnAmbr);

    switch (ucAMBRLen)
    {
        case VSNCP_AMBR_LEN_3 * 2:
        {
            pstApnAmbr->bitOpDLApnAmbr = PS_TRUE;
            pstApnAmbr->bitOpULApnAmbr = PS_TRUE;
            pstApnAmbr->bitOpDLApnAmbrExt = PS_TRUE;
            pstApnAmbr->bitOpULApnAmbrExt = PS_TRUE;
            pstApnAmbr->bitOpDLApnAmbrExt2 = PS_TRUE;
            pstApnAmbr->bitOpULApnAmbrExt2 = PS_TRUE;
            pstApnAmbr->ucDLApnAmbr = pucAMBR[0];
            pstApnAmbr->ucULApnAmbr = pucAMBR[1];
            pstApnAmbr->ucDLApnAmbrExt = pucAMBR[2];
            pstApnAmbr->ucULApnAmbrExt = pucAMBR[3];
            pstApnAmbr->ucDLApnAmbrExt2 = pucAMBR[4];
            pstApnAmbr->ucULApnAmbrExt2 = pucAMBR[5];

            break;
        }
        case VSNCP_AMBR_LEN_2 * 2:
        {
            pstApnAmbr->bitOpDLApnAmbr = PS_TRUE;
            pstApnAmbr->bitOpULApnAmbr = PS_TRUE;
            pstApnAmbr->bitOpDLApnAmbrExt = PS_TRUE;
            pstApnAmbr->bitOpULApnAmbrExt = PS_TRUE;
            pstApnAmbr->bitOpDLApnAmbrExt2 = PS_FALSE;
            pstApnAmbr->bitOpULApnAmbrExt2 = PS_FALSE;
            pstApnAmbr->ucDLApnAmbr = pucAMBR[0];
            pstApnAmbr->ucULApnAmbr = pucAMBR[1];
            pstApnAmbr->ucDLApnAmbrExt = pucAMBR[2];
            pstApnAmbr->ucULApnAmbrExt = pucAMBR[3];

            break;
        }
        case VSNCP_AMBR_LEN_1 * 2:
        {
            pstApnAmbr->bitOpDLApnAmbr = PS_TRUE;
            pstApnAmbr->bitOpULApnAmbr = PS_TRUE;
            pstApnAmbr->bitOpDLApnAmbrExt = PS_FALSE;
            pstApnAmbr->bitOpULApnAmbrExt = PS_FALSE;
            pstApnAmbr->bitOpDLApnAmbrExt2 = PS_FALSE;
            pstApnAmbr->bitOpULApnAmbrExt2 = PS_FALSE;
            pstApnAmbr->ucDLApnAmbr = pucAMBR[0];
            pstApnAmbr->ucULApnAmbr = pucAMBR[1];

            break;
        }
        case 0:
        {
            pstApnAmbr->bitOpDLApnAmbr = PS_FALSE;
            pstApnAmbr->bitOpULApnAmbr = PS_FALSE;
            pstApnAmbr->bitOpDLApnAmbrExt = PS_FALSE;
            pstApnAmbr->bitOpULApnAmbrExt = PS_FALSE;
            pstApnAmbr->bitOpDLApnAmbrExt2 = PS_FALSE;
            pstApnAmbr->bitOpULApnAmbrExt2 = PS_FALSE;

            break;
        }
        default:
        {
            pstApnAmbr->bitOpDLApnAmbr = PS_FALSE;
            pstApnAmbr->bitOpULApnAmbr = PS_FALSE;
            pstApnAmbr->bitOpDLApnAmbrExt = PS_FALSE;
            pstApnAmbr->bitOpULApnAmbrExt = PS_FALSE;
            pstApnAmbr->bitOpDLApnAmbrExt2 = PS_FALSE;
            pstApnAmbr->bitOpULApnAmbrExt2 = PS_FALSE;

            PPPC_WARNING_LOG1("illegeal ucAMBRLen!\r\n", ucAMBRLen);

            return;
        }
    }
}


VOID PPP_VSNCP_UpdatePdnContext
(
    PDN_CONTEXT_S                      *pstPDNContext,
    PDN_PPP_VSNCP_REQ_S                *pstVsncpReqInfo
)
{
    UCHAR ucAPNLen = 0;
    UCHAR *pucAPN = VOS_NULL_PTR;
    UCHAR *pucPco = VOS_NULL_PTR;
    UCHAR ucPdnType = 0;
    UCHAR *pucPdnAddress = VOS_NULL_PTR;
    UCHAR *pucIpv4DefaultRouterAddress = VOS_NULL_PTR;

    /* PdnId */
    pstPDNContext->stEhrpd.ucPdnId = pstVsncpReqInfo->ucPdnId;

    /* Apn */
    ucAPNLen = (UCHAR)VOS_StrLen(pstVsncpReqInfo->szApn);
    pucAPN = pstPDNContext->stEhrpd.stApn.aucApn;
    pstPDNContext->stEhrpd.stApn.ucApnLen = ucAPNLen;
    if (ucAPNLen > 0)
    {
        PPP_PUTCHARPTR(pstVsncpReqInfo->szApn, pucAPN, ucAPNLen);
    }

    /* PdnType */
    pstPDNContext->stEhrpd.enPdnType = pstVsncpReqInfo->ucPdnType;

    /* AttchType */
    pstPDNContext->stEhrpd.ucAttchType = pstVsncpReqInfo->ucAttchType;

    /* PdnAddress */
    ucPdnType = pstVsncpReqInfo->ucPdnType;
    pucPdnAddress = pstPDNContext->stEhrpd.stPdnAddress.aucAddress;
    if (ucPdnType & VSNCP_OPTION_PDNTYPE_IPV6)
    {
        PPP_PUTCHARPTR((UCHAR*)(pstVsncpReqInfo->aucIpv6Addr + PDN_IPV6PREF_LEN),
                       pucPdnAddress,
                       PDN_IPV6INTF_LEN);
    }

    if (ucPdnType & VSNCP_OPTION_PDNTYPE_IPV4)
    {
        PPP_PUTADDR(pstVsncpReqInfo->ulIpAddr, pucPdnAddress);
    }

    /* Pco */
    pucPco = pstPDNContext->stEhrpd.stPco.aucPco;
    pstPDNContext->stEhrpd.stPco.ucPcoLen = pstVsncpReqInfo->ucPcoLen;
    if (pstVsncpReqInfo->ucPcoLen > 0)
    {
        PPP_PUTCHARPTR(pstVsncpReqInfo->szPco, pucPco, pstVsncpReqInfo->ucPcoLen);
    }

    /* Ipv4DefaultRouterAddress */
    pucIpv4DefaultRouterAddress = pstPDNContext->stEhrpd.aucIpv4DefaultRouterAddress;
    PPP_PUTCHARPTR(pstVsncpReqInfo->aucIpv4DefaultRouterAddress,
                   pucIpv4DefaultRouterAddress,
                   VSNCP_OPTION_IPV4_DEFROUT_LEN - VSNCP_CODE_ID_LEN);

    /* AddressAllocCause */
    pstPDNContext->stEhrpd.ucAddressAllocCause = pstVsncpReqInfo->ucAddressAllocCause;
}


VOID PPP_VSNCP_UpdateErrorCode
(
    PDN_CONTEXT_S                      *pstPDNContext,
    UCHAR                               ucErrorCode
)
{
    pstPDNContext->stEhrpd.enErrorCode = ucErrorCode;
}


/*lint -e550*/
VOID PPP_VSNCP_resetci(PPPFSM_S *pstFsm)
{
    PPPVSNCPINFO_S *pstVsncpInfo = VOS_NULL_PTR;
    PPPINFO_S *pstPppInfo = VOS_NULL_PTR;

    PPP_TRACE("\r\n---Into PPP_VSNCP_resetci---\r\n");

    pstVsncpInfo = (PPPVSNCPINFO_S *)pstFsm->pProtocolInfo;
    pstPppInfo = pstVsncpInfo->pstPppInfo;


    return;
}
/*lint +e550*/


USHORT PPP_VSNCP_cilen(PPPFSM_S *pstFsm)
{
    PPPVSNCPINFO_S *pstVSNCPInfo = (PPPVSNCPINFO_S *)pstFsm->pProtocolInfo;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    USHORT usTotalLen = 0;
    ULONG ulRet = 0;

    /* OUI */
    usTotalLen += VSNCP_3GPP2_OUI_LEN;

    /* 取得pdnContext */
    /* 且根据取来的值构造pstVsncpReqInfo和pstOptSequence */
    ulRet = PPP_GetPDNContextByPDNID(pstVSNCPInfo->ucPDNID, &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");
        return 0;
    }

    /* PdnID */
    usTotalLen += VSNCP_OPTION_PDNID_LEN;
    /* Apn */
    usTotalLen += pstPDNContext->stEhrpd.stApn.ucApnLen + VSNCP_CODE_ID_LEN + 1;
    /* PdnType */
    usTotalLen += VSNCP_OPTION_PDNTYPE_LEN;
    /* PdnAddress */
    usTotalLen += (UCHAR)VOS_StrLen(pstPDNContext->stEhrpd.stPdnAddress.aucAddress)
                          + VSNCP_CODE_ID_LEN + 1;
    /* Pco */
    usTotalLen += pstPDNContext->stEhrpd.stPco.ucPcoLen + VSNCP_CODE_ID_LEN;
    /* AttachType */
    usTotalLen += VSNCP_OPTION_ATTACHTYPE_LEN;
    /* Ipv4DefaultRouterAddress */
    usTotalLen += VSNCP_OPTION_IPV4_DEFROUT_LEN;
    /* AddressAllocCause */
    usTotalLen += VSNCP_OPTION_ADDRESS_ALLOC_CAUSE_LEN;

    return usTotalLen;
}
VOID PPP_VSNCP_addci(PPPFSM_S *pstFsm, UCHAR *pPacket)
{
    PPPVSNCPINFO_S *pstVSNCPInfo = (PPPVSNCPINFO_S *)pstFsm->pProtocolInfo;
    PPPINFO_S *pstPppInfo = (PPPINFO_S *)pstFsm->pPppInfo;
    PPPCONFIGINFO_S *pstConfigInfo = pstPppInfo->pstConfigInfo;

    UCHAR *pucOldPacket = VOS_NULL_PTR;
    VSNCP_OPTION_ENCODE pOptionFunc = VOS_NULL_PTR;
    VSNP_CONIFG_OPTION_SEQUENCE_S stOptSequence;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    PDN_PPP_VSNCP_REQ_S stVsncpReqInfo;
    ULONG ulRet = 0;
    int i = 0;
    USHORT usOffset = 0;
    ULONG ulOptionLen = 0;
    ULONG ulTotalLen = 0;
    UCHAR ucType = 0;

    /* 消息缓存 */
    pucOldPacket = pPacket;

    PPP_PUTCHAR(VSNCP_3GPP2_OUI_BYTE_1, pucOldPacket);
    PPP_PUTCHAR(VSNCP_3GPP2_OUI_BYTE_2, pucOldPacket);
    PPP_PUTCHAR(VSNCP_3GPP2_OUI_BYTE_3, pucOldPacket);

    /* 取得PdnContext */
    /* 根据取来的值构造pstOptSequence */
    ulRet = PPP_GetPDNContextByPDNID(pstVSNCPInfo->ucPDNID, &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");

        return;
    }

    /* 构造OptSequence */
    stOptSequence.aucConfigOptionType[0] = PPP_VSNCP_CO_PDNID;
    stOptSequence.aucConfigOptionType[1] = PPP_VSNCP_CO_APN;
    stOptSequence.aucConfigOptionType[2] = PPP_VSNCP_CO_PDN_TYPE;
    stOptSequence.aucConfigOptionType[3] = PPP_VSNCP_CO_PDN_ADDR;
    stOptSequence.aucConfigOptionType[4] = PPP_VSNCP_CO_PDN_PCO;
    stOptSequence.aucConfigOptionType[5] = PPP_VSNCP_CO_ATTACH_TYPE;
    stOptSequence.aucConfigOptionType[6] = PPP_VSNCP_CO_IPV4_DEFRTADDR;
    stOptSequence.aucConfigOptionType[7] = PPP_VSNCP_CO_ADDRESS_ALLOC_CAUSE;

    for (i = 0; i < PPP_VSNCP_CO_BUTT; i++)
    {
        ucType = stOptSequence.aucConfigOptionType[i];
        if (ucType)
        {
            pOptionFunc = g_pVsncpOptionEncode[ucType];
            if (pOptionFunc != VOS_NULL_PTR)
            {
                ulRet = pOptionFunc(pucOldPacket, pstPDNContext, &ulOptionLen, &stVsncpReqInfo);
                if (VOS_OK != ulRet)
                {
                    PPPC_WARNING_LOG2("PPP_VSNCP_reqci fail!\r\n", ucType, ulRet);
                }
                else
                {
                    /* 定位到中下一个属性位置 */
                    pucOldPacket = (pucOldPacket + ulOptionLen);
                    /* 长度更新 */
                    ulTotalLen = ulTotalLen + ulOptionLen;
                    /* 属性长度清零 */
                    ulOptionLen = 0;
                }
            }

        }
    }

    ulTotalLen += VSNCP_3GPP2_OUI_LEN;

    /* 使用控制块 */
    pstPDNContext->stEhrpd.stVSNCP.ucUsedFlag = VOS_TRUE;

    return;
}



VOID PPP_VSNCP_Termaddci(PPPFSM_S *pstFsm, UCHAR *pPacket)
{
    PPPVSNCPINFO_S *pstVSNCPInfo = (PPPVSNCPINFO_S *)pstFsm->pProtocolInfo;

    PPP_PUTCHAR(VSNCP_3GPP2_OUI_BYTE_1, pPacket);
    PPP_PUTCHAR(VSNCP_3GPP2_OUI_BYTE_2, pPacket);
    PPP_PUTCHAR(VSNCP_3GPP2_OUI_BYTE_3, pPacket);

    ADDCIPDNID(PPP_VSNCP_CO_PDNID,
               pstVSNCPInfo->ucPDNID);

    return;
}



USHORT PPP_VSNCP_ackci(PPPFSM_S *pstFsm, UCHAR *pPacket, ULONG ulLen)
{
    USHORT usOffset = 0;
    PPPVSNCPINFO_S *pstVSNCPInfo = VOS_NULL_PTR;
    PPPINFO_S *pstPppInfo = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    PDN_PPP_VSNCP_REQ_S *pstVsncpReqInfo = VOS_NULL_PTR;
    ULONG ulRet = 0;

    if (ulLen == 0)
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1398);
        PPPC_WARNING_LOG("ulLen is 0!\r\n");
        return VOS_OK;
    }

    if ((VOS_NULL_PTR == pPacket) || (VOS_NULL_PTR == pstFsm))
    {
        PPPC_WARNING_LOG("null pointer!\r\n");

        return VOS_ERROR;
    }

    pstPppInfo = (PPPINFO_S *)pstFsm->pPppInfo;
    pstVSNCPInfo = (PPPVSNCPINFO_S *)pstFsm->pProtocolInfo;
    if ((VOS_NULL_PTR == pstPppInfo) || (VOS_NULL_PTR == pstVSNCPInfo))
    {
        PPPC_WARNING_LOG("null pointer!\r\n");
        return VOS_ERROR;
    }

    ulRet = PPP_GetPDNContextByPDNID(pstVSNCPInfo->ucPDNID,
                                     &pstPDNContext);
    if (VOS_OK != ulRet)
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");
        return VOS_ERROR;
    }

    /* 解析结果 */
    usOffset = FSM_HDRLEN + sizeof(VSNP_CONIFG_OPTION_SEQUENCE_S) + sizeof(PDN_PPP_VSNCP_REQ_S);
    pstVsncpReqInfo = (PDN_PPP_VSNCP_REQ_S *)(pPacket - usOffset);

    /* 将取得值更新到PdnContext */
    PPP_VSNCP_UpdatePdnContext(pstPDNContext, pstVsncpReqInfo);

    return VOS_OK;
}


USHORT PPP_VSNCP_rejci(PPPFSM_S *pstFsm, UCHAR *pPacket, ULONG ulLen)
{
    PPP_DBG_OK_CNT(PPP_PHOK_1399);
    return VOS_ERR;
}


UCHAR PPP_VSNCP_reqci(PPPFSM_S *pstFsm, UCHAR *pPacket, ULONG *pulLen)
{
    PPPVSNCPINFO_S *pstVSNCPInfo = VOS_NULL_PTR;
    PPPINFO_S *pstPppInfo = VOS_NULL_PTR;
    UCHAR *pucOldPacket = VOS_NULL_PTR;
    VSNCP_OPTION_ENCODE pOptionFunc = VOS_NULL_PTR;
    VSNP_CONIFG_OPTION_SEQUENCE_S *pstOptSequence = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    PDN_PPP_VSNCP_REQ_S *pstVsncpReqInfo = VOS_NULL_PTR;
    ULONG ulRet = 0;
    int i = 0;
    USHORT usOffset = 0;
    ULONG ulOptionLen = 0;
    ULONG ulTotalLen = 0;
    UCHAR ucType = 0;

    if ((VOS_NULL_PTR == pPacket) || (VOS_NULL_PTR == pstFsm) || (VOS_NULL_PTR == pulLen))
    {
        PPPC_WARNING_LOG("null pointer!\r\n");

        return CONFACK;
    }

    pstPppInfo = (PPPINFO_S *)pstFsm->pPppInfo;
    pstVSNCPInfo = (PPPVSNCPINFO_S *)pstFsm->pProtocolInfo;
    if ((VOS_NULL_PTR == pstPppInfo) || (VOS_NULL_PTR == pstVSNCPInfo))
    {
        PPPC_WARNING_LOG("null pointer!\r\n");

        return CONFACK;
    }

    ulRet = PPP_GetPDNContextByPDNID(pstVSNCPInfo->ucPDNID,
                                     &pstPDNContext);
    if (VOS_OK != ulRet)
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");
        return CONFACK;
    }

    /* 信元携带标记 */
    usOffset = FSM_HDRLEN + sizeof(VSNP_CONIFG_OPTION_SEQUENCE_S);
    pstOptSequence = (VSNP_CONIFG_OPTION_SEQUENCE_S *)(pPacket - usOffset);
    /* 解析结果 */
    usOffset = FSM_HDRLEN + sizeof(VSNP_CONIFG_OPTION_SEQUENCE_S) + sizeof(PDN_PPP_VSNCP_REQ_S);
    pstVsncpReqInfo = (PDN_PPP_VSNCP_REQ_S *)(pPacket - usOffset);

    /* 消息缓存 */
    pucOldPacket = pPacket;
    PPP_PUTCHAR(VSNCP_3GPP2_OUI_BYTE_1, pucOldPacket);
    PPP_PUTCHAR(VSNCP_3GPP2_OUI_BYTE_2, pucOldPacket);
    PPP_PUTCHAR(VSNCP_3GPP2_OUI_BYTE_3, pucOldPacket);

    for (i = 0; i < PPP_VSNCP_CO_BUTT; i++)
    {
        ucType = pstOptSequence->aucConfigOptionType[i];
        if (ucType)
        {
            pOptionFunc = g_pVsncpOptionEncode[ucType];
            if (pOptionFunc != VOS_NULL_PTR)
            {
                ulRet = pOptionFunc(pucOldPacket, pstPDNContext, &ulOptionLen, pstVsncpReqInfo);
                if (VOS_OK != ulRet)
                {
                    PPPC_WARNING_LOG2("PPP_VSNCP_reqci fail!\r\n", ucType, ulRet);
                }
                else
                {
                    /* 定位到中下一个属性位置 */
                    pucOldPacket = (pucOldPacket + ulOptionLen);
                    /* 长度更新 */
                    ulTotalLen = ulTotalLen + ulOptionLen;
                    /* 属性长度清零 */
                    ulOptionLen = 0;
                }
            }

        }
    }

    *pulLen = ulTotalLen + VSNCP_3GPP2_OUI_LEN;

    /* 更新ApnAmbr */
    PPP_VSNCP_UpdateApnAmbr(pstPDNContext, pstVsncpReqInfo);

    /* Return final code */
    return CONFACK;
}


VOID PPP_VSNCP_up(PPPFSM_S *pstFsm)
{
    ULONG ulAccept = 0;
    PPPINFO_S *pstPppInfo = (PPPINFO_S*)pstFsm->pPppInfo;
    PPPVSNCPINFO_S *pstVSNCPInfo = VOS_NULL_PTR;
    ULONG ulRet = 0;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;

    /* VSNCP协商结果是否可以接收 */
    ulAccept = PPP_VSNCP_UpResetCi(pstPppInfo);
    if (VOS_OK != ulAccept)
    {
        /* 协商结果不可接受,结束协商 */
        PPP_VSNCP_NegotiationDown(pstFsm->pProtocolInfo);
        PPP_DBG_ERR_CNT(PPP_PHERR_689);
        return;
    }

    /* 协商结果可以接受,向内核发送VSNCP Up事件 */
    PPP_Core_ReceiveEventFromProtocol(pstPppInfo,
                                      (LONG)PPP_EVENT_VSNCPUP,
                                      VOS_NULL_PTR);

    pstVSNCPInfo = (PPPVSNCPINFO_S *)pstFsm->pProtocolInfo;
    if ((VOS_NULL_PTR == pstPppInfo) || (VOS_NULL_PTR == pstVSNCPInfo))
    {
        PPPC_WARNING_LOG("null pointer!\r\n");

        return;
    }

    ulRet = PPP_GetPDNContextByPDNID(pstVSNCPInfo->ucPDNID,
                                     &pstPDNContext);
    if (VOS_OK != ulRet)
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");
        return;
    }

    /* 通知LCP回复CNAS */
    PPPC_RespEhsmPdnConnectCmd(pstVSNCPInfo, VOS_OK);

    /* 设置标识位，指示目前已不在尝试建立Pdn连接 */
    pstPDNContext->stEhrpd.stVSNCP.ucStartingFlag = VOS_FALSE;
}
VOID PPP_VSNCP_down(PPPFSM_S *pstFsm)
{

    PPP_Core_ReceiveEventFromProtocol(((PPPINFO_S *)pstFsm->pPppInfo),
                                      (LONG)PPP_EVENT_VSNCPDOWN,
                                      VOS_NULL_PTR);
    return;
}


VOID PPP_VSNCP_finished(PPPFSM_S *pstFsm)
{
    ULONG ulRet = 0;
    PPPINFO_S *pstPppInfo = VOS_NULL_PTR;
    PPPVSNCPINFO_S *pstVSNCPInfo = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;

    pstPppInfo = (PPPINFO_S *)pstFsm->pPppInfo;
    pstVSNCPInfo = (PPPVSNCPINFO_S *)pstFsm->pProtocolInfo;
    if ((VOS_NULL_PTR == pstPppInfo) || (VOS_NULL_PTR == pstVSNCPInfo))
    {
        PPPC_WARNING_LOG("null pointer!\r\n");
        return;
    }

    ulRet = PPP_GetPDNContextByPDNID(pstVSNCPInfo->ucPDNID,
                                     &pstPDNContext);
    if (VOS_OK != ulRet)
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");
        return;
    }

    /* 如果状态机设置的退网原因是超时，则设置PdnContext中的ErrorCode为超时 */
    if (AM_RELCODE_PPP_VSNCP_NEGOTIMEOUT == pstVSNCPInfo->ulPDNRleaseCode)
    {
        PPP_VSNCP_UpdateErrorCode(pstPDNContext, CNAS_CTTF_EHRPD_ERROR_CODE_TIMEROUT);
    }

    PPP_Core_ReceiveEventFromProtocol(((PPPINFO_S *)pstFsm->pPppInfo),
                                      (ULONG)PPP_EVENT_VSNCPFINISHED,
                                      VOS_NULL_PTR);

    /* 通知NAS，失败 */
    if (VOS_TRUE == pstPDNContext->stEhrpd.stVSNCP.ucStartingFlag)
    {
        PPPC_RespEhsmPdnConnectCmd(pstVSNCPInfo, VOS_ERROR);
        /* 设置标识位，指示目前已不在尝试建立Pdn连接 */
        pstPDNContext->stEhrpd.stVSNCP.ucStartingFlag = VOS_FALSE;
    }
    else if (VOS_TRUE == pstPDNContext->stEhrpd.stVSNCP.ucStoppingFlag)
    {
        PPPC_RespEhsmPdnDisconnectCmd(VOS_ERROR);
        /* 设置标识位，指示目前已不在尝试断开Pdn连接 */
        pstPDNContext->stEhrpd.stVSNCP.ucStoppingFlag = VOS_FALSE;
    }
}


VOID PPP_VSNCP_starting(PPPFSM_S *pstFsm)
{

    PPP_Core_ReceiveEventFromProtocol(((PPPINFO_S *)pstFsm->pPppInfo),
                                      (ULONG)PPP_EVENT_VSNCPSTARTING,
                                      VOS_NULL_PTR);
}


VOID PPP_VSNCP_Init(PPPINFO_S *pstPppInfo,
                    PPPVSNCPINFO_S *pstVSNCPInfo,
                    PDN_CONTEXT_S *pstPDNContext,
                    UCHAR ucPDNID)
{
    PPPFSM_S *pstFsm = VOS_NULL_PTR;

    if ((VOS_NULL_PTR == pstPppInfo) || (VOS_NULL_PTR == pstVSNCPInfo) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG("null pointer!\r\n");
        return;
    }

    pstFsm = &(pstVSNCPInfo->stFsm);
    //pstPppInfo->bPppStateFlg = PPP_STATE_NEGOING;
    pstPppInfo->pstVsncpInfo = pstVSNCPInfo;

    /* VSNCP控制块初始化 */
    pstVSNCPInfo->pstPppInfo = pstPppInfo;
    pstVSNCPInfo->ucPDNID = ucPDNID;
    pstVSNCPInfo->ulPDNIndex = pstPDNContext->ulPdnIndex;
    pstVSNCPInfo->ucUsedFlag = pstPDNContext->stEhrpd.stVSNCP.ucUsedFlag;
    pstVSNCPInfo->ucRenegoFlag =  pstPDNContext->stEhrpd.stVSNCP.ucRenegoFlag;
    pstVSNCPInfo->ucDefaultAPNFlag = pstPDNContext->stEhrpd.ucDefAPNFlag;
    pstVSNCPInfo->ulPDNRleaseCode = pstPDNContext->stEhrpd.stVSNCP.ucPdnReleaseCode;
    /* 是否有效,无效不需要携带 */
    if (VOS_TRUE == pstPDNContext->stEhrpd.ucApnAmbrDLFlag)
    {
        pstVSNCPInfo->ucApnAmbrDLFlag = VOS_TRUE;
        pstVSNCPInfo->ulApnAmbrDL = pstPDNContext->stEhrpd.ulApnAmbrDL;
    }
    if (VOS_TRUE == pstPDNContext->stEhrpd.ucApnAmbrULFlag)
    {
        pstVSNCPInfo->ucApnAmbrULFlag = VOS_TRUE;
        pstVSNCPInfo->ulApnAmbrUL = pstPDNContext->stEhrpd.ulApnAmbrUL;
    }

    /* 状态机初始化 */
    pstFsm->usProtocol = PPP_VSNCP;
    pstFsm->pstCallBacks  = &g_stVsncpCallbacks;
    pstFsm->pProtocolInfo = (char*)pstVSNCPInfo;
    pstFsm->pPppInfo = (char*)pstPppInfo;
    pstFsm->ucPktId = pstPDNContext->stEhrpd.stVSNCP.ucPktId;
    pstFsm->sRetransmits = pstPDNContext->stEhrpd.stVSNCP.sRetransmits;

    if (pstPppInfo->pstUsedConfigInfo == VOS_NULL_PTR)
    {
        pstFsm->ulTimeOutTime = PPP_DEFTIMEOUTTIME;
    }
    else
    {
        pstFsm->ulTimeOutTime = pstPppInfo->pstUsedConfigInfo->ulNegTimeOut;
    }

    pstFsm->ulTimeOutID = pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID;

    pstFsm->ucState = pstPDNContext->stEhrpd.stVSNCP.ucState;

    return;
}
VOID PPP_VSNCP_FreshPDNContext(PPPVSNCPINFO_S *pstVSNCPInfo, PDN_CONTEXT_S *pstPDNContext, PPPINFO_S *pstPppInfo)
{
    PPPFSM_S *pstFsm = VOS_NULL_PTR;

    if ((VOS_NULL_PTR == pstVSNCPInfo) || (VOS_NULL_PTR == pstPDNContext) || (VOS_NULL_PTR == pstPppInfo))
    {
        PPPC_WARNING_LOG("null pointer!\r\n");
        return;
    }

    pstFsm = &(pstVSNCPInfo->stFsm);

    pstPDNContext->stEhrpd.stVSNCP.sRetransmits = pstFsm->sRetransmits;
    pstPDNContext->stEhrpd.stVSNCP.ucPktId = pstFsm->ucPktId;
    pstPDNContext->stEhrpd.stVSNCP.ucState = pstFsm->ucState;
    pstPDNContext->stEhrpd.stVSNCP.ucPDNID = pstVSNCPInfo->ucPDNID;
    pstPDNContext->stEhrpd.stVSNCP.ucUsedFlag = pstVSNCPInfo->ucUsedFlag;
    pstPDNContext->stEhrpd.stVSNCP.ucRenegoFlag = pstVSNCPInfo->ucRenegoFlag;
    pstPDNContext->stEhrpd.stVSNCP.ucPdnReleaseCode = (UCHAR)pstVSNCPInfo->ulPDNRleaseCode;
    pstPppInfo->pstVsncpInfo = VOS_NULL_PTR;

    return;
}


ULONG PPP_VSNCP_UpResetCi(PPPINFO_S *pstPppInfo)
{
    return VOS_OK;
}


/*lint -e550*/
VOID PPP_VSNCP_SendReject(PPPINFO_S *pstPppInfo,
                          UCHAR *pucMsg,
                          UCHAR ucErrCode,
                          USHORT usPdnId)
{
    UCHAR *pHead = VOS_NULL_PTR;
    UCHAR *pPacket = VOS_NULL_PTR;
    ULONG ulOffset = 0;
    ULONG ulErrorCode = 0;
    UCHAR *pCurPos = VOS_NULL_PTR;
    UCHAR ucOldCode = 0;
    UCHAR ucOldID = 0;
    USHORT usLen = 0;
    UCHAR *pucLenPos = 0;

    if (VOS_NULL_PTR == pstPppInfo)
    {
        /* 输出debug信息 */
        PPPC_WARNING_LOG("null pointer!\r\n");
        return;
    }


    /* 预留出打PPP报文头的空间 */
    ulOffset = PPP_RESERVED_PACKET_HEADER;

    /* 申请内存 */
    pHead = g_ucPppSendPacketHead;
    if (VOS_NULL_PTR == pHead)
    {
        /*输出错误调试信息*/
        PPPC_WARNING_LOG("pHead is null pointer!\r\n");
        return;
    }

    pPacket = pHead + ulOffset;
    pCurPos = pPacket;

    PPP_GETCHAR(ucOldCode, pucMsg);
    PPP_GETCHAR(ucOldID, pucMsg);

    /* 消息头 */
    PPP_PUTCHAR(CONFREJ, pCurPos);
    PPP_PUTCHAR(ucOldID, pCurPos);
    /* 总长度 */
    pucLenPos = pCurPos;
    PPP_PUTSHORT(0, pCurPos);
    usLen += VSNCP_HEADERLEN;

    /* OUI */
    PPP_PUTCHAR(VSNCP_3GPP2_OUI_BYTE_1, pCurPos);
    PPP_PUTCHAR(VSNCP_3GPP2_OUI_BYTE_2, pCurPos);
    PPP_PUTCHAR(VSNCP_3GPP2_OUI_BYTE_3, pCurPos);
    usLen += VSNCP_3GPP2_OUI_LEN;

    if (VOS_NULL_WORD != usPdnId)
    {
        /* 将PDN ID写入报文 */
        PPP_PUTCHAR(PPP_VSNCP_CO_PDNID, pCurPos);
        PPP_PUTCHAR(VSNCP_OPTION_PDNID_LEN, pCurPos);
        PPP_PUTCHAR(usPdnId, pCurPos);
        usLen += VSNCP_OPTION_PDNID_LEN;
    }

    /* 将ERRCODE写入报文 */
    PPP_PUTCHAR(PPP_VSNCP_CO_ERRCODE, pCurPos);
    PPP_PUTCHAR(VSNCP_OPTION_ERRCODE_LEN, pCurPos);
    PPP_PUTCHAR(ucErrCode, pCurPos);
    usLen += VSNCP_OPTION_ERRCODE_LEN;

    /* 修改总长度 */
    PPP_PUTSHORT(usLen, pucLenPos);

    /* 直接调用外壳的发送函数 */
    ulErrorCode = PPP_Shell_GetPacketFromCore((CHAR *)pstPppInfo,
                                              pHead,
                                              pPacket,
                                              (ULONG)usLen,
                                              PPP_VSNCP);
    if (ulErrorCode != VOS_OK)
    {
        /*输出调试信息*/
        PPPC_WARNING_LOG1("PPP_Shell_GetPacketFromCore fail!\r\n", ulErrorCode);
    }

    PPP_DBG_OK_CNT(PPP_PHOK_1402);
    return;
}
/*lint +e550*/



ULONG PPP_VSNCP_NotifyVSNCPStart(PPPINFO_S *pstPppInfo, UCHAR ucPDNId)
{
    PPPVSNCPINFO_S stVSNCPInfo = {0};
    ULONG ulRet = 0;
    PDN_PPP_VSNCP_REQ_S *pstVsncpReqInfo = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;

    /* 判断是否为网络协商阶段*/
    if (pstPppInfo->usPhase != (USHORT)PPP_PHASE_NETWORK)
    {
        /* 输出debug信息 */
        PPPC_WARNING_LOG1("usPhase is not PPP_PHASE_NETWORK!\r\n", pstPppInfo->usPhase);
        return VOS_OK;
    }

    /* 取得PDNContext */
    ulRet = PPP_GetPDNContextByPDNID(ucPDNId, &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");
        return VOS_ERR;
    }

    /* 初始激活需要将状态机信息清零 */
    pstPDNContext->stEhrpd.stVSNCP.sRetransmits = 0;
    pstPDNContext->stEhrpd.stVSNCP.ucPktId = 0;
    pstPDNContext->stEhrpd.stVSNCP.ucState = 0;
    pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID = 0;
    pstPDNContext->stEhrpd.stVSNCP.ucUsedFlag = VOS_TRUE;

    /* 设置标识位，指示目前在尝试建立Pdn连接 */
    pstPDNContext->stEhrpd.stVSNCP.ucStartingFlag = VOS_TRUE;

    /* 初始化VSNCP控制块 */
    PPP_VSNCP_Init(pstPppInfo, &stVSNCPInfo, pstPDNContext, ucPDNId);
    /* VSNCP状态机open up开始协商 */
    PPP_VSNCP_ReceiveEventFromCore(&stVSNCPInfo, (ULONG)PPP_EVENT_FSM_OPEN, VOS_NULL_PTR);
    PPP_VSNCP_ReceiveEventFromCore(&stVSNCPInfo, (ULONG)PPP_EVENT_FSM_UP, VOS_NULL_PTR);

    /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
    PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);

    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n PPP_VSNCP_NotifyVSNCPStart: Done!");
    PPP_DBG_OK_CNT(PPP_PHOK_1433);
    return VOS_OK;
}


ULONG PPP_VSNCP_NotifyVSNCPStop(PPPINFO_S *pstPppInfo, UCHAR ucPDNId)
{
    PPPVSNCPINFO_S stVSNCPInfo = {0};
    ULONG ulRet = 0;
    UCHAR ucErrCode = 0;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;

    /* 取得PDNContext */
    ulRet = PPP_GetPDNContextByPDNID(ucPDNId, &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");
        return VOS_ERR;
    }

    /* 设置标识位，指示目前在尝试断开Pdn连接 */
    pstPDNContext->stEhrpd.stVSNCP.ucStoppingFlag = VOS_TRUE;

    /* 初始化VSNCP控制块 */
    PPP_VSNCP_Init(pstPppInfo, &stVSNCPInfo, pstPDNContext, ucPDNId);

    /* 发送vsncp term消息 */
    if (PPP_STATE_INITIAL != stVSNCPInfo.stFsm.ucState)
    {
        PPP_VSNCP_NegotiationDown(&stVSNCPInfo);
        PPP_DBG_OK_CNT(PPP_PHOK_1449);
    }
    else
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1450);

        PPP_CORE_VSNCPFinishProc(pstPppInfo);
    }

    /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
    PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);

    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n PPP_VSNCP_NotifyVSNCPStop Done!");
    return VOS_OK;
}

#if 0
ULONG PPP_VSNCP_CheckOptionIsChange(PDN_PPP_VSNCP_REQ_S *pstNewVsncpReqInfo,
                                    PDN_PPP_VSNCP_REQ_S *pstOldVsncpReqInfo,
                                    USHORT usOldAPNIndex,
                                    UCHAR ucPDNType)
{
    USHORT usNewAPNIndex = 0;
    ULONG ulRet = 0;
    UCHAR szApn[HSGW_MAX_APN_NAME_LEN + 1] = {0};

    if (VOS_NULL == pstNewVsncpReqInfo)
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_LOG,
                     "\r\n PPP_VSNCP_CheckOptionIsChange: input is null!");
        return VOS_ERR;
    }

    if (VOS_NULL != pstOldVsncpReqInfo)
    {
        if ((pstNewVsncpReqInfo->ucPdnId != pstOldVsncpReqInfo->ucPdnId)
            || (pstNewVsncpReqInfo->ucPdnType != pstOldVsncpReqInfo->ucPdnType)
            || (0 != VOS_MemCmp((const CHAR *)pstNewVsncpReqInfo->szApn,
                                (const CHAR *)pstOldVsncpReqInfo->szApn,
                                HSGW_MAX_APN_NAME_LEN)))
        {
            PPP_DebugOut(MID_PPPC, PPP_DEBUG_LOG,
                         "\r\n PPP_VSNCP_CheckOptionIsChange: option is change!");
            PPP_DBG_ERR_CNT(PPP_PHERR_695);
            return VOS_ERR;
        }
    }
    else
    {
        /* 根据APN NAME查找APN */
        if ('\0' !=  pstNewVsncpReqInfo->szApn[0])
        {
            VOS_MemCpy(szApn, pstNewVsncpReqInfo->szApn, HSGW_MAX_APN_NAME_LEN);
            ulRet = USM_GetApnIndexByApnName(szApn, &usNewAPNIndex);
            if ((ulRet != VOS_OK) || (usNewAPNIndex != usOldAPNIndex))
            {
                PPP_DebugOut(MID_PPPC, PPP_DEBUG_LOG,
                             "\r\n PPP_VSNCP_CheckOptionIsChange: USM_GetApnIndexByApnName[%s] [old: %u][new: %u]fail!",
                             pstNewVsncpReqInfo->szApn, usOldAPNIndex, usNewAPNIndex);
                PPP_DBG_ERR_CNT(PPP_PHERR_696);
                return VOS_ERR;
            }
        }

        /* 如果请求的PDN type和之前不一致 */
        if (VSNCP_OPTION_PDNTYPE_IPV4V6 != pstNewVsncpReqInfo->ucPdnType)
        {
            if (pstNewVsncpReqInfo->ucPdnType != ucPDNType)
            {
                PPP_DebugOut(MID_PPPC, PPP_DEBUG_LOG,
                             "\r\n PPP_VSNCP_CheckOptionIsChange: ucPDNType[old: %d][new: %d] is change!",
                             ucPDNType, pstNewVsncpReqInfo->ucPdnType);
                PPP_DBG_ERR_CNT(PPP_PHERR_697);
                return VOS_ERR;
            }
        }
    }

    return VOS_OK;
}
#endif


ULONG PPP_VSNCP_ConReqPactketProc(PPPINFO_S *pstPppInfo,
                                  UCHAR *pHead,
                                  UCHAR *pPacket,
                                  ULONG ulLen)
{
    UCHAR *pucNewVsncpReq = VOS_NULL_PTR;
    UCHAR *pucOldVsncpReq = VOS_NULL_PTR;
    USHORT usPdnId = VOS_NULL_WORD;
    ULONG ulRet = 0;
    UCHAR *pucNewPacket = VOS_NULL_PTR;
    PDN_PPP_VSNCP_REQ_S *pstNewVsncpReqInfo = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    PPPVSNCPINFO_S stVSNCPInfo = {0};

    /* Check OUI */
    ulRet = PPP_VSNCP_CheckOUI(pPacket, ulLen);
    if (VOS_OK != ulRet)
    {
        PPPC_WARNING_LOG("PPP_VSNCP_CheckOUI fail!\r\n");

        /* 回复reject消息,errcode为0 */
        PPP_VSNCP_SendReject(pstPppInfo, pPacket, PPP_VSNCP_GENERAL_ERROR, usPdnId);
        PPP_DBG_ERR_CNT(PPP_PHERR_698);
        return VOS_ERR;
    }

    /* 解析vsncp, 并缓存报文 */
    pucNewPacket = PPP_VSNCP_DecodePactket(pPacket, ulLen, &usPdnId, pstPppInfo);
    if (VOS_NULL_PTR == pucNewPacket)
    {
        PPPC_WARNING_LOG("PPP_VSNCP_DecodePactket fail!\r\n");

        /* 回复reject消息,errcode为6 */
        PPP_VSNCP_SendReject(pstPppInfo, pPacket, PPP_VSNCP_INSUFFICIENT_PARAS, usPdnId);
        PPP_DBG_ERR_CNT(PPP_PHERR_698);
        return VOS_ERR;
    }

    pstNewVsncpReqInfo = (PDN_PPP_VSNCP_REQ_S *)(pucNewPacket);
    pucNewVsncpReq = (pucNewPacket + sizeof(PDN_PPP_VSNCP_REQ_S) + sizeof(VSNP_CONIFG_OPTION_SEQUENCE_S));
    /* 根据PDN ID查询PDN上下文 */
    ulRet = PPP_GetPDNContextByPDNID(pstNewVsncpReqInfo->ucPdnId,
                                     &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");

#if 0
        /* 检查必选信元 */
        if (VSNCP_CONFIG_REQ_OPTION_CHECK)
        {
            HSGW_EmsTraceByRpIndex(pstPppInfo->ulRPIndex, HSGW_EMS_MODULE_PPP, EMS_PPP_42);
            /* 回复reject消息,errcode为6 */
            PPP_VSNCP_SendReject(pstPppInfo,
                                 pPacket,
                                 PPP_VSNCP_INSUFFICIENT_PARAS,
                                 (USHORT)pstNewVsncpReqInfo->ucPdnId);
            PPP_Free(pucNewPacket);
            PPP_DBG_ERR_CNT(PPP_PHERR_699);
            return VOS_ERR;
        }

        /* 通知PDN */
        ulRet = PDN_PppStartEhrpdPdn(pstPppInfo->ulRPIndex, (PDN_PPP_VSNCP_REQ_S *)pucNewPacket);
        if (ulRet != VOS_OK)
        {
            PPP_Free(pucNewPacket);
            PPP_DBG_ERR_CNT(PPP_PHERR_700);
        }
#endif
        return VOS_OK;
    }

#if 0
    /* 上下文中的缓存消息不存在 */
    if (VOS_NULL_PTR != pstPDNContext->stEhrpd.pucVsncpReq)
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1404);
        pucOldVsncpReq = (pstPDNContext->stEhrpd.pucVsncpReq + sizeof(PDN_PPP_VSNCP_REQ_S)
                                                     + sizeof(VSNP_CONIFG_OPTION_SEQUENCE_S));
        /* 如果状态为PPP_STATE_INITIAL,则更新消息中的ID,其他的不用更新,我们认为重发消息应该是一样的 */
        if ((PPP_STATE_INITIAL == pstPDNContext->stEhrpd.stVSNCP.ucState)
            || (VOS_TRUE == pstPDNContext->stEhrpd.stVSNCP.ucGetPCOFlag)
            || (VOS_TRUE == pstPDNContext->stEhrpd.ucbit1L2CReconnectFlg))
        {
            VSNCP_REFRESH_PACKET_ID(pucOldVsncpReq, pucNewVsncpReq);
            PPP_Free(pucNewPacket);
            PPP_DBG_OK_CNT(PPP_PHOK_1405);
            return VOS_OK;
        }
        else
        {
            PPP_Free(pucNewPacket);
        }
    }
    else
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1406);

        /* 如果此时控制没有使用,需要将控制块设置为使用 */
        if (VOS_FALSE == pstPDNContext->stEhrpd.stVSNCP.ucUsedFlag)
        {
            pstPDNContext->stEhrpd.stVSNCP.ucUsedFlag = VOS_TRUE;
            PPP_DBG_OK_CNT(PPP_PHOK_1407);
        }

        /* 检查必选信元 */
        if (VSNCP_CONFIG_REQ_OPTION_CHECK)
        {
            HSGW_EmsTraceByPdnIndex(pstPDNContext->ulPdnIndex, HSGW_EMS_MODULE_PPP, EMS_PPP_43);
            /* 回复reject消息,errcode为6 */
            PPP_VSNCP_SendReject(pstPppInfo,
                                 pPacket,
                                 PPP_VSNCP_INSUFFICIENT_PARAS,
                                 (USHORT)pstNewVsncpReqInfo->ucPdnId);
            PPP_Free(pucNewPacket);
            PPP_DBG_ERR_CNT(PPP_PHERR_701);
            return VOS_ERR;
        }

        /* 如果是切换消息则缓存该消息,释放原PDN连接 */
        if (VSNCP_OPTION_ATTACHTYPE_HANDOFF == pstNewVsncpReqInfo->ucAttchType)
        {
            PPP_DBG_OK_CNT(PPP_PHOK_1408_L2C_RECONNECT);

            /* 缓存消息 */
            if (pstPDNContext->stEhrpd.pucVsncpReq != VOS_NULL_PTR)
            {
                PPP_Free(pstPDNContext->stEhrpd.pucVsncpReq);
                pstPDNContext->stEhrpd.pucVsncpReq = VOS_NULL_PTR;
            }
            pstPDNContext->stEhrpd.pucVsncpReq = pucNewPacket;
            pstPDNContext->stEhrpd.ucbit1L2CReconnectFlg = VOS_TRUE;

            /* 重建PDN连接 */
            ulRet = PDN_VsncpNotifyReconnect(pstPDNContext, (PDN_PPP_VSNCP_REQ_S *)pucNewPacket);
            if (ulRet != VOS_OK)
            {
                PPP_DBG_ERR_CNT(PPP_PHERR_702);
                return VOS_ERR;
            }
            return VOS_OK;
        }

        /* 重协商 */
        /* 检查必选信元是否改变 */
        ulRet = PPP_VSNCP_CheckOptionIsChange(pstNewVsncpReqInfo,
                                              VOS_NULL_PTR,
                                              pstPDNContext->stEhrpd.usApnIndex,
                                              pstPDNContext->ucPdnCapability);
        if (ulRet != VOS_OK)
        {
            HSGW_EmsTraceByPdnIndex(pstPDNContext->ulPdnIndex, HSGW_EMS_MODULE_PPP, EMS_PPP_44);
            /* 去活PDN */
            PPP_Free(pucNewPacket);
            PPP_DBG_OK_CNT(PPP_PHOK_1409);
            /* 初始化VSNCP控制块 */
            PPP_VSNCP_Init(pstPppInfo, &stVSNCPInfo, pstPDNContext, pstPDNContext->stEhrpd.ucPdnId);
            if (PPP_STATE_INITIAL != stVSNCPInfo.stFsm.ucState)
            {
                PPP_VSNCP_NegotiationDown(&stVSNCPInfo);
                PPP_DBG_OK_CNT(PPP_PHOK_1410);
            }
            else
            {
                PPP_DBG_OK_CNT(PPP_PHOK_1411);
                PPP_CORE_VSNCPFinishProc(pstPppInfo);
            }
            /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
            PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);
            return VOS_ERR;
        }

        /* 如果状态为PPP_STATE_INITIAL, 则需要挂到pdn上下文中 */
        if (PPP_STATE_INITIAL == pstPDNContext->stEhrpd.stVSNCP.ucState)
        {
            if (pstPDNContext->stEhrpd.pucVsncpReq != VOS_NULL_PTR)
            {
                PPP_Free(pstPDNContext->stEhrpd.pucVsncpReq);
                pstPDNContext->stEhrpd.pucVsncpReq = VOS_NULL_PTR;
            }
            pstPDNContext->stEhrpd.pucVsncpReq = pucNewPacket;
            PPP_DBG_OK_CNT(PPP_PHOK_1412);
            return VOS_OK;
        }

#if 0
        /* 如果携带PCO, 则需要去PGW获取PCO */
        if ((VSNCP_CONFIG_REQ_PCO_OPTION_FLAG)
            && (VOS_FALSE == pstPDNContext->stEhrpd.stVSNCP.ucGetPCOFlag)
            && (!(SOFTPARA_BIT1455)))
        {
            ulRet = PDN_VsncpNotifyGetPCO(pstPDNContext, (PDN_PPP_VSNCP_REQ_S *)pucNewPacket);
            if (ulRet != VOS_OK)
            {
                PPP_DBG_ERR_CNT(PPP_PHERR_703);
                return VOS_ERR;
            }
            pstPDNContext->stEhrpd.stVSNCP.ucGetPCOFlag = VOS_TRUE;
            return VOS_OK;
        }
        /* 如果正在获取PCO，则缓存 */
        else if (VOS_TRUE == pstPDNContext->stEhrpd.stVSNCP.ucGetPCOFlag)
        {
            if (pstPDNContext->stEhrpd.pucVsncpReq != VOS_NULL_PTR)
            {
                PPP_Free(pstPDNContext->stEhrpd.pucVsncpReq);
                pstPDNContext->stEhrpd.pucVsncpReq = VOS_NULL_PTR;
            }
            pstPDNContext->stEhrpd.pucVsncpReq = pucNewPacket;
            PPP_DBG_OK_CNT(PPP_PHOK_1413);
            return VOS_OK;
        }
#endif
#endif

        /* 初始化VSNCP控制块 */
        PPP_VSNCP_Init(pstPppInfo,
                       &stVSNCPInfo,
                       pstPDNContext,
                       pstNewVsncpReqInfo->ucPdnId);

        /* 处理终端的VSNCP请求 */
        PPP_VSNCP_ReceivePacket(&stVSNCPInfo,
                                pHead,
                                pucNewVsncpReq,
                                ulLen);
        /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
        PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);
        PPP_Free(pucNewPacket);
    //}

    return VOS_OK;
}
ULONG PPP_VSNCP_CheckOUI(UCHAR *pPacket, ULONG ulLen)
{
    UCHAR *pOuiPos = VOS_NULL_PTR;
    UCHAR *pOuiByte1;
    UCHAR *pOuiByte2;
    UCHAR *pOuiByte3;

    if (ulLen < (VSNCP_HEADERLEN + VSNCP_3GPP2_OUI_LEN))
    {
        PPPC_WARNING_LOG1("PPP_VSNCP_CheckOUI pPacket Len error!\r\n", ulLen);
        return VOS_ERR;
    }

    pOuiPos = pPacket + VSNCP_HEADERLEN;

    pOuiByte1 = *pOuiPos;
    pOuiByte2 = *(pOuiPos + 1);
    pOuiByte3 = *(pOuiPos + 2);

    /* Check OUI */
    if ((VSNCP_3GPP2_OUI_BYTE_1!= pOuiByte1) || (VSNCP_3GPP2_OUI_BYTE_2!= pOuiByte2)
        || (VSNCP_3GPP2_OUI_BYTE_3!= pOuiByte3))
    {
        PPPC_WARNING_LOG3("PPP_VSNCP_CheckOUI error!\r\n",
                     pOuiByte1, pOuiByte2, pOuiByte3);
        return VOS_ERR;
    }

    return VOS_OK;
}
ULONG PPP_VSNCP_ConAckPactketProc(PPPINFO_S *pstPppInfo,
                                  UCHAR *pHead,
                                  UCHAR *pPacket,
                                  ULONG ulLen)
{
    ULONG ulRet = 0;
    UCHAR *pucNewVsncpReq = VOS_NULL_PTR;
    UCHAR *pucNewPacket = VOS_NULL_PTR;
    PDN_PPP_VSNCP_REQ_S *pstNewVsncpReqInfo = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    PPPVSNCPINFO_S stVSNCPInfo = {0};
    USHORT usPdnId = VOS_NULL_WORD;

    /* Check OUI */
    ulRet = PPP_VSNCP_CheckOUI(pPacket, ulLen);
    if (VOS_OK != ulRet)
    {
        /* 回复reject消息,errcode为0 */
        PPP_VSNCP_SendReject(pstPppInfo, pPacket, PPP_VSNCP_GENERAL_ERROR, usPdnId);

        PPPC_WARNING_LOG("PPP_VSNCP_CheckOUI error!\r\n");
        return VOS_ERR;
    }

    /* 解析vsncp, 并缓存报文 */
    pucNewPacket = PPP_VSNCP_DecodePactket(pPacket, ulLen, &usPdnId, pstPppInfo);
    if (VOS_NULL_PTR == pucNewPacket)
    {
        /* 丢弃 */
        PPPC_WARNING_LOG("PPP_VSNCP_DecodePactket error!\r\n");
        return VOS_ERR;
    }

    if (VOS_TRUE != g_stVsncpOptionFlag.ucPDNIDFlag)
    {
        /* 丢弃 */
        PPP_Free(pucNewPacket);
        PPPC_WARNING_LOG("PDNID error!\r\n");
        return VOS_ERR;
    }

    pstNewVsncpReqInfo = (PDN_PPP_VSNCP_REQ_S *)(pucNewPacket);
    pucNewVsncpReq = (pucNewPacket + sizeof(PDN_PPP_VSNCP_REQ_S) + sizeof(VSNP_CONIFG_OPTION_SEQUENCE_S));
    /* 根据PDN ID查询PDN上下文 */
    ulRet = PPP_GetPDNContextByPDNID(pstNewVsncpReqInfo->ucPdnId,
                                     &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");
        PPP_Free(pucNewPacket);
        return VOS_ERR;
    }

    /* 如果此时控制没有使用,直接返回丢弃 */
    if (VOS_FALSE == pstPDNContext->stEhrpd.stVSNCP.ucUsedFlag)
    {
        PPP_Free(pucNewPacket);
        PPPC_WARNING_LOG("stVSNCP ucUsedFlag fail!\r\n");
        return VOS_ERR;
    }

    /* 初始化VSNCP控制块 */
    PPP_VSNCP_Init(pstPppInfo,
                   &stVSNCPInfo,
                   pstPDNContext,
                   pstNewVsncpReqInfo->ucPdnId);

    /* 处理终端的VSNCP ACK */
    PPP_VSNCP_ReceivePacket(&stVSNCPInfo,
                             pHead,
                             pucNewVsncpReq,
                             ulLen);

    /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
    PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);

    PPP_Free(pucNewPacket);

    PPP_DBG_OK_CNT(PPP_PHOK_1414);
    return VOS_OK;
}




ULONG PPP_VSNCP_ConRejPactketProc(PPPINFO_S *pstPppInfo,
                                  UCHAR *pHead,
                                  UCHAR *pPacket,
                                  ULONG ulLen)
{
    ULONG ulRet = 0;
    UCHAR *pucNewPacket = VOS_NULL_PTR;
    PDN_PPP_VSNCP_REQ_S *pstNewVsncpReqInfo = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    PPPVSNCPINFO_S stVSNCPInfo = {0};
    USHORT usPdnId = VOS_NULL_WORD;

    /* 解析vsncp, 并缓存报文 */
    pucNewPacket = PPP_VSNCP_DecodePactket(pPacket, ulLen, &usPdnId, pstPppInfo);
    if (VOS_NULL_PTR == pucNewPacket)
    {
        /* 丢弃 */
        PPPC_WARNING_LOG("PPP_VSNCP_DecodePactket fail!\r\n");
        return VOS_ERR;
    }

    if (VOS_TRUE != g_stVsncpOptionFlag.ucPDNIDFlag)
    {
        /* 丢弃 */
        PPPC_WARNING_LOG("PDNID error!\r\n");
        PPP_Free(pucNewPacket);
        return VOS_ERR;
    }

    pstNewVsncpReqInfo = (PDN_PPP_VSNCP_REQ_S *)(pucNewPacket);
    /* 根据PDN ID查询PDN上下文 */
    ulRet = PPP_GetPDNContextByPDNID(pstNewVsncpReqInfo->ucPdnId,
                                     &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");
        PPP_Free(pucNewPacket);
        return VOS_ERR;
    }

    /* 更新ErrorCode */
    PPP_VSNCP_UpdateErrorCode(pstPDNContext, pstNewVsncpReqInfo->ucErrorCode);

    /* 初始化VSNCP控制块 */
    PPP_VSNCP_Init(pstPppInfo,
                   &stVSNCPInfo,
                   pstPDNContext,
                   pstNewVsncpReqInfo->ucPdnId);

    /* 处理终端的VSNCP REJECT */
    if (PPP_STATE_INITIAL != stVSNCPInfo.stFsm.ucState)
    {
        PPP_VSNCP_NegotiationDown(&stVSNCPInfo);
        PPP_DBG_OK_CNT(PPP_PHOK_1415);
    }
    else
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1416);
        PPP_CORE_VSNCPFinishProc(pstPppInfo);
    }

    /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
    PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);

    PPP_Free(pucNewPacket);

    /* 通知LCP回复CNAS */
    PPPC_RespEhsmPdnConnectCmd(&stVSNCPInfo, VOS_ERROR);

    /* 设置标识位，指示目前已不在尝试建立Pdn连接 */
    pstPDNContext->stEhrpd.stVSNCP.ucStartingFlag = VOS_FALSE;

    PPP_DBG_OK_CNT(PPP_PHOK_1417);
    return VOS_OK;
}



ULONG PPP_VSNCP_TermReqPactketProc(PPPINFO_S *pstPppInfo,
                                   UCHAR *pHead,
                                   UCHAR *pPacket,
                                   ULONG ulLen)
{
    ULONG ulRet = 0;
    UCHAR *pucNewPacket = VOS_NULL_PTR;
    PDN_PPP_VSNCP_REQ_S *pstNewVsncpReqInfo = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    PPPVSNCPINFO_S stVSNCPInfo = {0};
    USHORT usPdnId = VOS_NULL_WORD;

    /* 解析vsncp, 并缓存报文 */
    pucNewPacket = PPP_VSNCP_DecodePactket(pPacket, ulLen, &usPdnId, pstPppInfo);
    if (VOS_NULL_PTR == pucNewPacket)
    {
        /* 丢弃 */
        PPPC_WARNING_LOG("PPP_VSNCP_DecodePactket fail!\r\n");
        return VOS_ERR;
    }

    if (VOS_TRUE != g_stVsncpOptionFlag.ucPDNIDFlag)
    {
        /* 丢弃 */
        PPPC_WARNING_LOG("PDNID error!\r\n");
        PPP_Free(pucNewPacket);
        return VOS_ERR;
    }

    pstNewVsncpReqInfo = (PDN_PPP_VSNCP_REQ_S *)(pucNewPacket);
    /* 根据PDN ID查询PDN上下文 */
    ulRet = PPP_GetPDNContextByPDNID(pstNewVsncpReqInfo->ucPdnId,
                                     &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");
        PPP_Free(pucNewPacket);
        return VOS_ERR;
    }

    /* 初始化VSNCP控制块 */
    PPP_VSNCP_Init(pstPppInfo,
                   &stVSNCPInfo,
                   pstPDNContext,
                   pstNewVsncpReqInfo->ucPdnId);


    /* 处理终端的VSNCP TERM REQ */
    PPP_VSNCP_ReceivePacket(&stVSNCPInfo,
                            pHead,
                            pPacket,
                            ulLen);

    /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
    PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);

    PPP_Free(pucNewPacket);

    /* 通知LCP回复NAS */
    PPPC_SendEhsmPdnDisconnectNotify(usPdnId);

    PPP_DBG_OK_CNT(PPP_PHOK_1417);
    return VOS_OK;
}



ULONG PPP_VSNCP_TermAckPactketProc(PPPINFO_S *pstPppInfo,
                                   UCHAR *pHead,
                                   UCHAR *pPacket,
                                   ULONG ulLen)
{
    ULONG ulRet = 0;
    UCHAR *pucNewPacket = VOS_NULL_PTR;
    PDN_PPP_VSNCP_REQ_S *pstNewVsncpReqInfo = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    PPPVSNCPINFO_S stVSNCPInfo = {0};
    USHORT usPdnId = VOS_NULL_WORD;

    /* 解析vsncp, 并缓存报文 */
    pucNewPacket = PPP_VSNCP_DecodePactket(pPacket, ulLen, &usPdnId, pstPppInfo);
    if (VOS_NULL_PTR == pucNewPacket)
    {
        /* 丢弃 */
        PPPC_WARNING_LOG("PPP_VSNCP_DecodePactket fail!\r\n");
        return VOS_ERR;
    }

    if (VOS_TRUE != g_stVsncpOptionFlag.ucPDNIDFlag)
    {
        /* 丢弃 */
        PPPC_WARNING_LOG("PDNID error!\r\n");
        PPP_Free(pucNewPacket);
        return VOS_ERR;
    }

    pstNewVsncpReqInfo = (PDN_PPP_VSNCP_REQ_S *)(pucNewPacket);
    /* 根据PDN ID查询PDN上下文 */
    ulRet = PPP_GetPDNContextByPDNID(pstNewVsncpReqInfo->ucPdnId,
                                     &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPPC_WARNING_LOG("PPP_GetPDNContextByPDNID fail!\r\n");
        PPP_Free(pucNewPacket);
        return VOS_ERR;
    }

    /* 如果此时控制没有使用,直接返回丢弃 */
    if (VOS_FALSE == pstPDNContext->stEhrpd.stVSNCP.ucUsedFlag)
    {
        PPP_Free(pucNewPacket);
        PPP_DBG_ERR_CNT(PPP_PHERR_717);
        return VOS_ERR;
    }


    /* 设置标识位，指示目前已不在尝试断开Pdn连接 */
    pstPDNContext->stEhrpd.stVSNCP.ucStoppingFlag = VOS_FALSE;


    /* 初始化VSNCP控制块 */
    PPP_VSNCP_Init(pstPppInfo,
                   &stVSNCPInfo,
                   pstPDNContext,
                   pstNewVsncpReqInfo->ucPdnId);

    /* 处理终端的VSNCP TERM ACK */
    PPP_VSNCP_ReceivePacket(&stVSNCPInfo,
                            pHead,
                            pPacket,
                            ulLen);

    /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
    PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);

    PPP_Free(pucNewPacket);

    PPP_DBG_OK_CNT(PPP_PHOK_1418);

    /* 通知LCP发送回复给CNAS */
    PPPC_RespEhsmPdnDisconnectCmd(VOS_OK);

    /* 清除上下文信息 */
    PPP_VSNCP_ClearPdnContext(pstNewVsncpReqInfo->ucPdnId);

    return VOS_OK;
}
VOID PPP_VSNCP_PactketProc(PPPINFO_S *pstPppInfo,
                           UCHAR *pHead,
                           UCHAR *pPacket,
                           ULONG ulLen,
                           UCHAR ucCode)
{
    /* 判断阶段状态是否正确 */
    if (pstPppInfo->usPhase != (USHORT)PPP_PHASE_NETWORK)
    {
        /* 输出debug信息 */
        PPPC_WARNING_LOG1("Phase error!\r\n", pstPppInfo->usPhase);
        return;
    }

    if (CONFREQ == ucCode)
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1419);
        (VOS_VOID)PPP_VSNCP_ConReqPactketProc(pstPppInfo, pHead, pPacket, ulLen);
    }
    else if (CONFREJ == ucCode)
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1420);
        (VOS_VOID)PPP_VSNCP_ConRejPactketProc(pstPppInfo, pHead, pPacket, ulLen);
    }
    else if (CONFACK == ucCode)
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1421);
        (VOS_VOID)PPP_VSNCP_ConAckPactketProc(pstPppInfo, pHead, pPacket, ulLen);
    }
    else if (TERMACK == ucCode)
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1422);
        (VOS_VOID)PPP_VSNCP_TermAckPactketProc(pstPppInfo, pHead, pPacket, ulLen);
    }
    else if (TERMREQ == ucCode)
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1423);
        (VOS_VOID)PPP_VSNCP_TermReqPactketProc(pstPppInfo, pHead, pPacket, ulLen);
    }
    else
    {
        PPPC_WARNING_LOG1("ucCode error!\r\n", ucCode);
    }

    return;
}

#if 0
#define __VSNCP_PDN_INTERFACE__


ULONG PPP_SubPDNNotifyVSNCPGetPCOSucc(ULONG ulRpIdx, UCHAR ucPDNId, UCHAR *pucPacket)
{
    PPPINFO_S *pstPppInfo = VOS_NULL_PTR;
    PPPVSNCPINFO_S stVSNCPInfo = {0};
    UCHAR *pHead = VOS_NULL_PTR;
    UCHAR *pPacket = VOS_NULL_PTR;
    ULONG ulRet = 0;
    PDN_PPP_VSNCP_REQ_S *pstVsncpReqInfo = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;

    if (VOS_NULL == pucPacket)
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_SubPDNNotifyVSNCPGetPCOSucc: pucPacket is null!",
                     ulRpIdx);
        PPP_DBG_ERR_CNT(PPP_PHERR_720);
        return VOS_OK;
    }

    GETPPPINFOPTRBYRP(pstPppInfo, ulRpIdx);
    if (VOS_NULL_PTR == pstPppInfo)
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_SubPDNNotifyVSNCPGetPCOSucc: User %d isn't exist!",
                     ulRpIdx);
        PPP_DBG_ERR_CNT(PPP_PHERR_721);
        return VOS_ERR;
    }

    /* 判断是否为网络协商阶段*/
    if (pstPppInfo->usPhase != (USHORT)PPP_PHASE_NETWORK)
    {
        /* 输出debug信息 */
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_SubPDNNotifyVSNCPGetPCOSucc: usPhase[%lu] is not PPP_PHASE_NETWORK!",
                     pstPppInfo->usPhase);
        PPP_DBG_OK_CNT(PPP_PHOK_1424);
        return VOS_OK;
    }

    ulRet = PPP_VSNCP_GetPDNContextByPDNID(ulRpIdx, ucPDNId, &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_LOG,
                     "\r\n PPP_SubPDNNotifyVSNCPGetPCOSucc: PPP_VSNCP_GetPDNContextByPDNID fail pdnid[%d]RPindex[%lu]!",
                     ucPDNId, ulRpIdx);
        PPP_DBG_ERR_CNT(PPP_PHERR_722);
        return VOS_ERR;
    }

    if ((VOS_FALSE == pstPDNContext->stEhrpd.stVSNCP.ucGetPCOFlag)
        && (VOS_FALSE == pstPDNContext->stEhrpd.ucbit1L2CReconnectFlg))
    {
        /* 输出debug信息 */
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_SubPDNNotifyVSNCPGetPCOSucc: ucGetPCOFlag is false!",
                     pstPDNContext->stEhrpd.stVSNCP.ucGetPCOFlag);
        PPP_DBG_OK_CNT(PPP_PHOK_1425);
        return VOS_OK;
    }
    /* 初始化VSNCP控制块 */
    PPP_VSNCP_Init(pstPppInfo, &stVSNCPInfo, pstPDNContext, ucPDNId);

    /* 处理缓存消息 */
    pstVsncpReqInfo = (PDN_PPP_VSNCP_REQ_S *)pucPacket;
    pPacket = pucPacket + sizeof(PDN_PPP_VSNCP_REQ_S) + sizeof(VSNP_CONIFG_OPTION_SEQUENCE_S);
    pHead = pPacket;

    /* 处理终端的VSNCP请求 */
    PPP_VSNCP_ReceivePacket(&stVSNCPInfo,
                            pHead,
                            pPacket,
                            (ULONG)(pstVsncpReqInfo->usVsncpMsgBodyLen));

    /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
    PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);

    pstPDNContext->stEhrpd.stVSNCP.ucGetPCOFlag = VOS_FALSE;
    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n PPP_SubPDNNotifyVSNCPGetPCOSucc: Done!");
    PPP_DBG_OK_CNT(PPP_PHOK_1426);
    return VOS_OK;
}
ULONG PPP_SubPDNNotifyVSNCPRenegoStart(ULONG ulRpIdx, UCHAR ucPDNId)
{
    PPPINFO_S *pstPppInfo = VOS_NULL_PTR;
    PPPVSNCPINFO_S stVSNCPInfo = {0};
    ULONG ulRet = 0;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    UCHAR *pHead = VOS_NULL_PTR;
    UCHAR *pPacket = VOS_NULL_PTR;
    PDN_PPP_VSNCP_REQ_S *pstVsncpReqInfo = VOS_NULL_PTR;

    GETPPPINFOPTRBYRP(pstPppInfo, ulRpIdx);
    if (VOS_NULL_PTR == pstPppInfo)
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_SubPDNNotifyVSNCPRenegoStart: User %d isn't exist!",
                     ulRpIdx);
        PPP_DBG_ERR_CNT(PPP_PHERR_723);
        return VOS_ERR;
    }

    /* 判断是否为网络协商阶段*/
    if (pstPppInfo->usPhase != (USHORT)PPP_PHASE_NETWORK)
    {
        /* 输出debug信息 */
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_SubPDNNotifyVSNCPRenegoStart: usPhase[%lu] is not PPP_PHASE_NETWORK!",
                     pstPppInfo->usPhase);
        PPP_DBG_OK_CNT(PPP_PHOK_1427);
        return VOS_OK;
    }

    ulRet = PPP_VSNCP_GetPDNContextByPDNID(ulRpIdx, ucPDNId, &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_LOG,
                     "\r\n PPP_SubPDNNotifyVSNCPRenegoStart: PPP_VSNCP_GetPDNContextByPDNID fail!");
        PPP_DBG_OK_CNT(PPP_PHOK_1428);
        return VOS_OK;
    }

    if (AM_RELCODE_PDN_HSGW_IDLETIMEOUT == pstPDNContext->usReleaseCode)
    {
        (VOID)PPP_PDNNodeAdd(pstPppInfo->ulRPIndex, pstPDNContext->ulPdnIndex, PPP_NEGO_LIST_NODE_VSNCP_RENEGO_START);
    }

    /* 如果此时vsncp处于以下几个状态说明还没有收到用户的VSNCP请求，所以不需要再发起重协商 */
    if ((PPP_STATE_REQSENT == pstPDNContext->stEhrpd.stVSNCP.ucState)
        || (PPP_STATE_ACKRCVD == pstPDNContext->stEhrpd.stVSNCP.ucState))
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_SubPDNNotifyVSNCPRenegoStart: state is not initial!",
                     pstPDNContext->stEhrpd.stVSNCP.ucState);
        PPP_DBG_OK_CNT(PPP_PHOK_1429);
        return VOS_OK;
    }

    /* 初始激活需要将状态机信息清零 */
    pstPDNContext->stEhrpd.stVSNCP.sRetransmits = 0;
    pstPDNContext->stEhrpd.stVSNCP.ucPktId = 0;
    pstPDNContext->stEhrpd.stVSNCP.ucState = 0;
    if (pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID)
    {
        (VOID)VOS_StopRelTimer(&(pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID));
        pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID = 0;
    }
    pstPDNContext->stEhrpd.stVSNCP.ucUsedFlag = VOS_TRUE;

    /* 初始化VSNCP控制块 */
    PPP_VSNCP_Init(pstPppInfo, &stVSNCPInfo, pstPDNContext, ucPDNId);
    /* VSNCP状态机open up开始协商 */
    PPP_VSNCP_ReceiveEventFromCore(&stVSNCPInfo, (ULONG)PPP_EVENT_FSM_OPEN, VOS_NULL_PTR);
    PPP_VSNCP_ReceiveEventFromCore(&stVSNCPInfo, (ULONG)PPP_EVENT_FSM_UP, VOS_NULL_PTR);

    /* 如果此时还存在缓存 */
    if (pstPDNContext->stEhrpd.pucVsncpReq != VOS_NULL_PTR)
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1430);
        pstVsncpReqInfo = (PDN_PPP_VSNCP_REQ_S *)pstPDNContext->stEhrpd.pucVsncpReq;
        pPacket = pstPDNContext->stEhrpd.pucVsncpReq + sizeof(PDN_PPP_VSNCP_REQ_S) + sizeof(VSNP_CONIFG_OPTION_SEQUENCE_S);
        pHead = pPacket;

        /* 处理终端的VSNCP请求 */
        PPP_VSNCP_ReceivePacket(&stVSNCPInfo,
                                pHead,
                                pPacket,
                                (ULONG)(pstVsncpReqInfo->usVsncpMsgBodyLen));
        PPP_Free(pstPDNContext->stEhrpd.pucVsncpReq);
        pstPDNContext->stEhrpd.pucVsncpReq = VOS_NULL_PTR;
    }

    /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
    PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);


    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n PPP_SubPDNNotifyVSNCPRenegoStart: Done!");
    PPP_DBG_OK_CNT(PPP_PHOK_1431);

    return VOS_OK;
}
ULONG PPP_SubPDNNotifyVSNCPStart(ULONG ulRpIdx, UCHAR ucPDNId, UCHAR *pucPacket)
{
    PPPINFO_S *pstPppInfo = VOS_NULL_PTR;
    PPPVSNCPINFO_S stVSNCPInfo = {0};
    UCHAR *pHead = VOS_NULL_PTR;
    UCHAR *pPacket = VOS_NULL_PTR;
    ULONG ulRet = 0;
    PDN_PPP_VSNCP_REQ_S *pstVsncpReqInfo = VOS_NULL_PTR;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;


    GETPPPINFOPTRBYRP(pstPppInfo, ulRpIdx);
    if (VOS_NULL_PTR == pstPppInfo)
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_SubPDNNotifyVSNCPStart: User %d isn't exist!",
                     ulRpIdx);
        PPP_DBG_ERR_CNT(PPP_PHERR_724);
        return VOS_ERR;
    }

    /* 判断是否为网络协商阶段*/
    if (pstPppInfo->usPhase != (USHORT)PPP_PHASE_NETWORK)
    {
        /* 输出debug信息 */
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_SubPDNNotifyVSNCPStart: usPhase[%lu] is not PPP_PHASE_NETWORK!",
                     pstPppInfo->usPhase);
        PPP_DBG_OK_CNT(PPP_PHOK_1432);
        return VOS_OK;
    }

    ulRet = PPP_VSNCP_GetPDNContextByPDNID(ulRpIdx, ucPDNId, &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_LOG,
                     "\r\n PPP_SubPDNNotifyVSNCPStart: PPP_VSNCP_GetPDNContextByPDNID fail pdnid[%d]RPindex[%lu]!",
                     ucPDNId, ulRpIdx);
        PPP_DBG_ERR_CNT(PPP_PHERR_725);
        return VOS_ERR;
    }


    /* 初始激活需要将状态机信息清零 */
    pstPDNContext->stEhrpd.stVSNCP.sRetransmits = 0;
    pstPDNContext->stEhrpd.stVSNCP.ucPktId = 0;
    pstPDNContext->stEhrpd.stVSNCP.ucState = 0;
    pstPDNContext->stEhrpd.stVSNCP.ulTimeOutID = 0;
    pstPDNContext->stEhrpd.stVSNCP.ucUsedFlag = VOS_TRUE;

    /* 初始化VSNCP控制块 */
    PPP_VSNCP_Init(pstPppInfo, &stVSNCPInfo, pstPDNContext, ucPDNId);
    /* VSNCP状态机open up开始协商 */
    PPP_VSNCP_ReceiveEventFromCore(&stVSNCPInfo, (ULONG)PPP_EVENT_FSM_OPEN, VOS_NULL_PTR);
    PPP_VSNCP_ReceiveEventFromCore(&stVSNCPInfo, (ULONG)PPP_EVENT_FSM_UP, VOS_NULL_PTR);

    /* 处理缓存消息 */
    if (pucPacket != VOS_NULL_PTR)
    {
        pstVsncpReqInfo = (PDN_PPP_VSNCP_REQ_S *)pucPacket;
        pPacket = pucPacket + sizeof(PDN_PPP_VSNCP_REQ_S) + sizeof(VSNP_CONIFG_OPTION_SEQUENCE_S);
        pHead = pPacket;

        /* 处理终端的VSNCP请求 */
        PPP_VSNCP_ReceivePacket(&stVSNCPInfo,
                                pHead,
                                pPacket,
                                (ULONG)(pstVsncpReqInfo->usVsncpMsgBodyLen));

    }

    /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
    PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);

    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n PPP_SubPDNNotifyVSNCPStart: Done!");
    PPP_DBG_OK_CNT(PPP_PHOK_1433);
    return VOS_OK;
}


UCHAR PPP_VSNCP_SetRejectCodeByReleaseCode(ULONG ulReleaseCode)
{
    UCHAR ucErrCode = 0;

    if ((ulReleaseCode >= AM_RELCODE_DNS_ERR) && (ulReleaseCode <= AM_RELCODE_DNS_SORT_ERR))
    {
		return PPP_VSNCP_NO_PGW;
    }

    switch (ulReleaseCode)
    {
        case AM_RELCODE_PMIPV6_LMA_TOUT :
            ucErrCode = PPP_VSNCP_PGW_UNREACHABLE;
            PPP_DBG_OK_CNT(PPP_PHOK_1434);
            break;
        case AM_RELCODE_PMIPV6_INNER_ERR :
            ucErrCode = PPP_VSNCP_ADMIN_PROHIBITED;
            PPP_DBG_OK_CNT(PPP_PHOK_1435);
            break;
        case AM_RELCODE_PMIPV6_LMA_FAIL :                     /* lma返回失败 */
            ucErrCode = PPP_VSNCP_PGW_REJECT;
            PPP_DBG_OK_CNT(PPP_PHOK_1436);
            break;
        case AM_RELCODE_PMIPV6_LMA_CHECK_ERR :               /* lma消息检查失败 */
            ucErrCode = PPP_VSNCP_PGW_REJECT;
            PPP_DBG_OK_CNT(PPP_PHOK_1437);
            break;
        case AM_RELCODE_PMIPV6_LIFE_TOUT :                   /* lifetime超时 */
            ucErrCode = PPP_VSNCP_PGW_REJECT;
            PPP_DBG_OK_CNT(PPP_PHOK_1438);
            break;
        case AM_RELCODE_PMIPV6_PGW_REL_NORMAL :               /* PGW发起正常去活 */
            ucErrCode = PPP_VSNCP_PGW_REJECT;
            PPP_DBG_OK_CNT(PPP_PHOK_1439);
            break;
        case AM_RELCODE_PMIPV6_PGW_REL_SAME_HAND :            /* PGW发起去活 - HSGW间切换*/
            ucErrCode = PPP_VSNCP_PGW_REJECT;
            PPP_DBG_OK_CNT(PPP_PHOK_1440);
            break;
        case AM_RELCODE_PMIPV6_PGW_REL_DIFF_HAND :           /* PGW发起去活 - 网间切换*/
            ucErrCode = PPP_VSNCP_PGW_REJECT;
            PPP_DBG_OK_CNT(PPP_PHOK_1441);
            break;
        case AM_RELCODE_PMIPV6_PGW_REL_UNKNOWN_HAND :         /* PGW发起去活 - 未知切换*/
            ucErrCode = PPP_VSNCP_PGW_REJECT;
            PPP_DBG_OK_CNT(PPP_PHOK_1442);
            break;
        case AM_RELCODE_PDN_COMMAND :
            PPP_DBG_OK_CNT(PPP_PHOK_1443);
            ucErrCode = PPP_VSNCP_ADMIN_PROHIBITED;
            break;
        case AM_RELCODE_PDN_DUALSTACK_LICENSE_FAIL:
            PPP_DBG_OK_CNT(PPP_PHOK_1444);
            ucErrCode = PPP_VSNCP_RES_UNAVAILABLE;
            break;
        case AM_RELCODE_SUBNET_INVAILD_PGW:
            PPP_DBG_OK_CNT(PPP_PHOK_1445);
            ucErrCode = PPP_VSNCP_NO_PGW;
            break;

        case AM_RELCODE_SUBNET_HSGWPCC_LICE:
            PPP_DBG_OK_CNT(PPP_PHOK_1446);
            ucErrCode = PPP_VSNCP_RES_UNAVAILABLE;       /*HSGW PCC license资源不足*/
            break;

        default:
            PPP_DBG_OK_CNT(PPP_PHOK_1447);
            ucErrCode = PPP_VSNCP_ADMIN_PROHIBITED;
            break;
    }

    return ucErrCode;
}


ULONG PPP_SubPDNNotifyVSNCPStop(ULONG ulRpIdx, UCHAR ucPDNId, ULONG ulReleaseCode)
{
    PPPINFO_S *pstPppInfo = VOS_NULL_PTR;
    PPPVSNCPINFO_S stVSNCPInfo = {0};
    ULONG ulRet = 0;
    UCHAR ucErrCode = 0;
    PDN_CONTEXT_S *pstPDNContext = VOS_NULL_PTR;
    UCHAR eAlarmCode = FSM_ALARM_NULL;

    GETPPPINFOPTRBYRP(pstPppInfo, ulRpIdx);
    if (VOS_NULL_PTR == pstPppInfo)
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_SubPDNNotifyVSNCPStop: User %d isn't exist!",
                     ulRpIdx);
        PPP_DBG_ERR_CNT(PPP_PHERR_726);
        return VOS_ERR;
    }

    ulRet = PPP_VSNCP_GetPDNContextByPDNID(ulRpIdx, ucPDNId, &pstPDNContext);
    if ((ulRet != VOS_OK) || (VOS_NULL_PTR == pstPDNContext))
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_LOG,
                     "\r\n PPP_SubPDNNotifyVSNCPStop: PPP_VSNCP_GetPDNContextByPDNID fail!");
        PPP_DBG_ERR_CNT(PPP_PHERR_727);
        return VOS_ERR;
    }

    /* 判断是否为网络协商阶段*/
    if (pstPppInfo->usPhase != (USHORT)PPP_PHASE_NETWORK)
    {
        /* 输出debug信息 */
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_SubPDNNotifyVSNCPStop: usPhase[%lu] is not PPP_PHASE_NETWORK!",
                     pstPppInfo->usPhase);
        PPP_DBG_OK_CNT(PPP_PHOK_1448);
        return VOS_ERR;
    }

    /* 按照去活原因值获取 告警原因值 */
    SNMP_GetFailReasonByReleaseCode(ulReleaseCode, &eAlarmCode);
    /* 设置性能统计去活原因 */
    if ( FSM_ALARM_NULL != eAlarmCode )
    {
        SNMP_SetFailReason(pstPppInfo, (FSM_ALARM_CODE_E)eAlarmCode);
    }

    /* 初始化VSNCP控制块 */
    PPP_VSNCP_Init(pstPppInfo, &stVSNCPInfo, pstPDNContext, ucPDNId);

    /* 发送vsncp term消息 */
    if (PPP_STATE_INITIAL != stVSNCPInfo.stFsm.ucState)
    {
        PPP_VSNCP_NegotiationDown(&stVSNCPInfo);
        PPP_DBG_OK_CNT(PPP_PHOK_1449);
    }
    else
    {
        PPP_DBG_OK_CNT(PPP_PHOK_1450);

        ucErrCode = PPP_VSNCP_SetRejectCodeByReleaseCode(ulReleaseCode);
        PPP_PDNSendVSNCPReject(pstPDNContext->ulRpIndex,
                               pstPDNContext->stEhrpd.pucVsncpReq,
                               ucErrCode);

        PPP_CORE_VSNCPFinishProc(pstPppInfo);
    }

    /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
    PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);

    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n PPP_SubPDNNotifyVSNCPStop Done!");
    return VOS_OK;
}
VOID PPP_PDNSendVSNCPReject(ULONG ulRPIndex,
                            UCHAR *pucMsg,
                            UCHAR ucErrCode)
{
    PPPINFO_S *pstPppInfo = VOS_NULL_PTR;
    UCHAR *pucVsncpReq = VOS_NULL_PTR;
    PDN_PPP_VSNCP_REQ_S *pstVsncpReqInfo = VOS_NULL_PTR;

    if ((ulRPIndex > PPP_MAX_USER_NUM)
        || (VOS_NULL_PTR == pucMsg))
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n [ppp]PPP_PDNSendVSNCPReject :ulRPIndex is invalid! error, %lu, %x",
                     ulRPIndex, pucMsg);
        PPP_DBG_ERR_CNT(PPP_PHERR_728);
        return;
    }

    GETPPPINFOPTRBYRP(pstPppInfo, (ULONG)ulRPIndex);
    if (VOS_NULL_PTR == pstPppInfo)
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                      "\r\n[ppp]: PPP_PDNSendVSNCPReject: rpindex[%lu] pstPppInfo is NULL!",
                      ulRPIndex);
        PPP_DBG_ERR_CNT(PPP_PHERR_729);
        return;
    }
    pstVsncpReqInfo = (PDN_PPP_VSNCP_REQ_S *)pucMsg;
    pucVsncpReq = (pucMsg + sizeof(PDN_PPP_VSNCP_REQ_S) + sizeof(VSNP_CONIFG_OPTION_SEQUENCE_S));

    PPP_VSNCP_SendReject(pstPppInfo, pucVsncpReq, ucErrCode, (USHORT)pstVsncpReqInfo->ucPdnId);

    PPP_DBG_OK_CNT(PPP_PHOK_1451);
    return;
}



VOID PPP_PDNClearVSNCP(PDN_CONTEXT_S *pstPDNContext)
{
    PPPINFO_S *pstPppInfo = VOS_NULL_PTR;
    PPPVSNCPINFO_S stVSNCPInfo = {0};

    if (VOS_NULL_PTR == pstPDNContext)
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_LOG,
                     "\r\n [ppp]PPP_PDNClearVSNCP :pstPDNContext is NULL! ");
        PPP_DBG_ERR_CNT(PPP_PHERR_730);
        return;
    }

    GETPPPINFOPTRBYRP(pstPppInfo, pstPDNContext->ulRpIndex);
    if (VOS_NULL_PTR == pstPppInfo)
    {
        PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                     "\r\n PPP_PDNClearVSNCP: User %d isn't exist!",
                     pstPDNContext->ulRpIndex);
        PPP_DBG_ERR_CNT(PPP_PHERR_731);
        return ;
    }
    /* 初始化VSNCP控制块 */
    PPP_VSNCP_Init(pstPppInfo, &stVSNCPInfo, pstPDNContext, pstPDNContext->stEhrpd.ucPdnId);

    PPP_VSNCP_Clear(&stVSNCPInfo, pstPDNContext);

    /* vsncp状态机处理完毕后刷新pdn上下文中的状态信息 */
    PPP_VSNCP_FreshPDNContext(&stVSNCPInfo, pstPDNContext, pstPppInfo);

    PPP_DebugOut(MID_PPPC, PPP_DEBUG_INFO,
                 "\r\n [ppp]PPP_PDNClearVSNCP :success! ");
    PPP_DBG_OK_CNT(PPP_PHOK_1452);
    return;
}
#endif


/*lint -restore */


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
