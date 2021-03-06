

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "CssProcTimerMsg.h"
#include    "CssProcLphyMsg.h"
#include    "CssProcCommMsg.h"

#define    THIS_FILE_ID        PS_FILE_ID_CSSPROCTIMERMSG_C

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

VOS_VOID CSS_ProcSetInactiveModeTimerExp
(
    VOS_VOID
)
{
    #if (VOS_OS_VER != VOS_WIN32)
    NAS_TRACE_HIGH(" CSS_ProcSetInactiveModeTimerExp");
    TLPS_PRINT2LAYER_INFO(CSS_ProcSetInactiveModeTimerExp_ENUM, LNAS_ENTRY);
    #endif

    CSS_ClearLocalResource();

    /* 设置从模失败，调用底软接口，直接复位 */
    (VOS_VOID)LPS_SoftReBoot_WithLineNoAndFileID( CSS_REBOOT_TYPE_SET_INACTIVE_MODE_TIMEOUT, (__LINE__), (THIS_FILE_ID) );
    return ;
}
VOS_VOID CSS_ProcSetActiveModeTimerExp
(
    VOS_VOID
)
{
    CSS_CONTROL_STRU                        *pstCssControl;

    #if (VOS_OS_VER != VOS_WIN32)
    NAS_TRACE_HIGH(" CSS_ProcSetActiveModeTimerExp");
    TLPS_PRINT2LAYER_INFO(CSS_ProcSetActiveModeTimerExp_ENUM, LNAS_TIMER_EXPIRED);
    #endif

    pstCssControl = CSS_GetControlAddr();

    /* 将状态迁移至等待停止扫频扫频状态 */
    CSS_FSMTranState(CSS_MS_STOP_SCAN, CSS_SS_WAIT_SET_WORK_INACTIVE_MODE_CNF);

    /* 启动定时器 */
    CSS_StartTimer(&pstCssControl->stSetInactiveModeTimer);

     /* 需要通知LPHY设置从模，收到回复后，回复扫频失败 */
    CSS_SndLphyMsgSetWorkModeReq(CSS_LPHY_WORK_MODE_TYPE_INACTIVE);

    return ;
}
VOS_VOID CSS_ProcBandScanTimerExp
(
    VOS_VOID
)
{
    CSS_CONTROL_STRU                        *pstCssControl;

    #if (VOS_OS_VER != VOS_WIN32)
    NAS_TRACE_HIGH(" CSS_ProcBandScanTimerExp");
    TLPS_PRINT2LAYER_INFO(CSS_ProcBandScanTimerExp_ENUM, LNAS_TIMER_EXPIRED);
    #endif

    pstCssControl = CSS_GetControlAddr();

    /* 将状态迁移至等待停止扫频扫频状态 */
    CSS_FSMTranState(CSS_MS_STOP_SCAN, CSS_SS_WAIT_STOP_SCAN_CNF);

    /* 启动定时器 */
    CSS_StartTimer(&pstCssControl->stStopBandScanTimer);

    /* 需要通知LPHY停止扫频，并设置从模，收到回复后，回复扫频失败 */
    CSS_SndLphyMsgStopBandScanReq();

    return ;
}
VOS_VOID CSS_ProcStopBandScanTimerExp
(
    VOS_VOID
)
{
    #if (VOS_OS_VER != VOS_WIN32)
    NAS_TRACE_HIGH(" CSS_ProcStopBandScanTimerExp");
    TLPS_PRINT2LAYER_INFO(CSS_ProcStopBandScanTimerExp_ENUM, LNAS_TIMER_EXPIRED);
    #endif

    CSS_ClearLocalResource();

    /* 停止扫频失败，调用底软接口，直接复位 */
    (VOS_VOID)LPS_SoftReBoot_WithLineNoAndFileID( CSS_REBOOT_TYPE_SET_INACTIVE_MODE_TIMEOUT, (__LINE__), (THIS_FILE_ID) );

    return ;
}
VOS_VOID CSS_ProcTimerMsg(const REL_TIMER_MSG *pRcvMsg)
{
    CSS_TIMER_ID_ENUM_UINT16            enTimerName;

    enTimerName = (VOS_UINT16)PS_GET_REL_TIMER_NAME(pRcvMsg);

    switch(enTimerName)
    {
        case TI_CSS_WAIT_FOR_SET_INACTIVE_MODE_CNF:
            CSS_ProcSetInactiveModeTimerExp();
            break;

        case TI_CSS_WAIT_FOR_SET_ACTIVE_MODE_CNF:
            CSS_ProcSetActiveModeTimerExp();
            break;

        case TI_CSS_WAIT_FOR_SCAN_IND:
            CSS_ProcBandScanTimerExp();
            break;

        case TI_CSS_WAIT_FOR_STOP_SCAN_CNF:
            CSS_ProcStopBandScanTimerExp();
            break;
        default:
            break;
    }
}

#ifdef __cplusplus
    #if __cplusplus
            }
    #endif
#endif





