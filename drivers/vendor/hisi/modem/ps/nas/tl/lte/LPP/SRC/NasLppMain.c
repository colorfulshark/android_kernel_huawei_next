

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/

#include    "NasLppPublic.h"
#include    "LPsOm.h"
#include    "TlPsDrv.h"
#include    "NasLppLrrcMsgProc.h"
#include    "NasLppMtaMsgProc.h"
#include    "NasLppTimerMsgProc.h"
#include    "NasLppEmmMsgProc.h"
#include    "NasLppMain.h"

#include    "NasLppEtcMsgProc.h"


#define    THIS_FILE_ID            PS_FILE_ID_NASLPPMAIN_C
#define    THIS_NAS_FILE_ID        NAS_FILE_ID_NASLPPMAIN_C

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



/*****************************************************************************
  3 Function
*****************************************************************************/

VOS_VOID  NAS_LPP_TaskEntry(MsgBlock * pMsg)
{
    /* 定义消息头指针*/
#if (FEATURE_LPP == FEATURE_ON)
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;

    NAS_LPP_NORM_LOG("NAS_LPP_TaskEntry is entered!");
    TLPS_PRINT2LAYER_INFO(NAS_LPP_TaskEntry_ENUM, LNAS_LPP_TaskEntryEnter);

    /* 判断入口参数是否合法*/
    if( VOS_NULL_PTR == pMsg)
    {
        NAS_LPP_ERR_LOG("NAS_LPP_TaskEntry:ERROR: No Msg!");
        TLPS_PRINT2LAYER_ERROR(NAS_LPP_TaskEntry_ENUM, LNAS_LPP_TaskEntryNullMsg);
        return;
    }

    /* 获取消息头指针*/
    pHeader = (PS_MSG_HEADER_STRU *) pMsg;

    /*打印收到的消息*/
    NAS_LPP_PrintLppRevMsg(pHeader,NAS_COMM_GET_LPP_PRINT_BUF());

    /*打印消息码流*/
    NAS_LPP_NORM_LOG("====================IN MSG CONTENT is : ============");
    NAS_COMM_PrintArray(             NAS_COMM_GET_LPP_PRINT_BUF(),
                                     (VOS_UINT8 *)pMsg,
                                     (pMsg->ulLength + NAS_LPP_MSG_HEADER_LENGTH));
    NAS_LPP_NORM_LOG("====================================================");

    switch(pHeader->ulSenderPid)
    {
        /*如果是EMM消息，调用EMM处理函数*/
        case PS_PID_MM:
            NAS_LPP_EmmMsgDistr(pMsg);
            break;

        /*如果是LRRC的消息，调用LRRC的处理函数*/
        case PS_PID_ERRC:
            NAS_LPP_LrrcMsgDistr(pMsg);
            break;

        /*如果是Timer消息，调用Timer溢出处理函数*/
        case VOS_PID_TIMER:
            NAS_LPP_TimerMsgDistr(pMsg);
            break;

        /*如果是MTA的消息，调用MTA的处理函数*/
        case UEPS_PID_MTA:
            NAS_LPP_MtaMsgDistr(pMsg);
            break;

        /*如果是LPP的的内部消息，调用LPP的处理函数*/
        case PS_PID_LPP:
            NAS_LPP_LppMsgDistr(pMsg);
            break;

        case PS_PID_TC:
            NAS_LPP_EtcMsgDistr(pMsg);
            break;

        default:
            NAS_LPP_WARN_LOG("NAS_LPP_TaskEntry:NORM: Error SenderPid!");
            TLPS_PRINT2LAYER_ERROR(NAS_LPP_TaskEntry_ENUM, LNAS_LPP_TaskEntryNoThisMsg);
            break;
    }
#endif
    return;
}

VOS_UINT32 NAS_LPP_PidInit ( enum VOS_INIT_PHASE_DEFINE ip )
{
#if (FEATURE_LPP == FEATURE_ON)

    switch( ip )
    {
        case    VOS_IP_LOAD_CONFIG:
                {
                    NAS_LPP_LppCtxInit();
                }
                break;

        case    VOS_IP_FARMALLOC:
        case    VOS_IP_INITIAL:
        case    VOS_IP_ENROLLMENT:
        case    VOS_IP_LOAD_DATA:
        case    VOS_IP_FETCH_DATA:
        case    VOS_IP_STARTUP:
        case    VOS_IP_RIVAL:
        case    VOS_IP_KICKOFF:
        case    VOS_IP_STANDBY:
        case    VOS_IP_BROADCAST_STATE:
        case    VOS_IP_RESTART:
        case    VOS_IP_BUTT:
                break;

        default:
                break;
    }
#endif
    return VOS_OK;
}



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of NasLppMain.c */

