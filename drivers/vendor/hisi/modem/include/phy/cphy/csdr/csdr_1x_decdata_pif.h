/** ****************************************************************************

                    Huawei Technologies Sweden AB (C), 2001-2015

 ********************************************************************************
 * @author    Automatically generated by DAISY
 * @version
 * @date      2015-06-15
 * @file
 * @brief
 * This interface provides the forward physical channel interface to 1x_Layer_2.
 * 
 * Output: Decoded bits. Frequency: Once per received frame.
 * @copyright Huawei Technologies Sweden AB
 *******************************************************************************/
#ifndef CSDR_1X_DECDATA_PIF_H
#define CSDR_1X_DECDATA_PIF_H

/*******************************************************************************
 1. Other files included
*******************************************************************************/

#include "vos.h"
#include "csdr_1x_data_public_pif.h"
#include "csdr1xcttfcproc.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#pragma pack(4)

/*******************************************************************************
 2. Macro definitions
*******************************************************************************/

/**
 * maximum frame size in 16-bit on F-DCCH
 * 32-bit align(267bit) = 288bit, 16-bit-occupied(288bit) = 18*uint16
 */
#define CSDR_1X_FDCCH_BLK_MAX_SIZE                          ( 18 )
/**
 * maximum frame size in octet on F-FCH
 * 32-bit align(267bit) = 288bit, 16-bit-occupied(288bit) = 18*uint16
 */
#define CSDR_1X_FFCH_BLK_MAX_SIZE                           ( 18 )
/**
 * maximum frame size in octet on F-SCCH
 * 32-bit align(267bit) = 288bit, 16-bit-occupied(288bit) = 18*uint16
 */
#define CSDR_1X_FSCCH_BLK_MAX_SIZE                          ( 18 )
/**
 * maximum frame size in octet on F-SCH
 * 32-bit align(6120bit) = 6144bit, 16-bit-occupied(6144bit) = 384*uint16
 */
#define CSDR_1X_FSCH_BLK_MAX_SIZE                           ( 384 )
/**
 * Now it is defined that every ring buffer contains 3 blocks, as using head
 * and tail pointer to implement it, there is 2 blocks data stored at most.
 */
#define CSDR_1X_MAX_BLK_NUM_PER_RING_BUFFER                 ( 3 )
/**
 * maximum frame size in 16-bit on BCCH, it's BCCH_FRAME_SIZE in C.S0003-A
 * v6.0, section 2.2.1.1.2.3.6
 * 32-bit align(744bit) = 744bit, 16-bit-occupied(744bit) = 47*unit16
 */
#define CSDR_1X_BCCH_BLK_MAX_SIZE                           ( 47 )
/**
 * maximum frame size in 16-bit on FCCCH, it's FCCCH_FRAME_SIZE in C.S0003-A
 * v6.0, section 2.2.1.1.2.3.7 and Table 2-29
 * 32-bit align(744bit) = 744bit, 16-bit-occupied(744bit) = 47*unit16
 */
#define CSDR_1X_FCCCH_BLK_MAX_SIZE                          ( 47 )
/**
 * maximum frame size in octet on PCH, it's PCH_FRAME_SIZE in C.S0003-A v6.0,
 * section 2.2.1.1.2.3.2
 * 32-bit align(192) = 192bit, 16-bit-occupied(192) = 12*uint16
 */
#define CSDR_1X_PCH_BLK_MAX_SIZE                            ( 12 )
/**
 * maximum frame size in 16-bit on Synch, it's SYNC_FRAME_SIZE in C.S0003-A
 * v6.0, section 2.2.1.1.2.3.1
 * 32-bit align(32bit) = 32bit, 16-bit-occupied(32bit) = 2*unit16
 */
#define CSDR_1X_SYNC_BLK_MAX_SIZE                           ( 2 )

/*******************************************************************************
 3. Enumerations declarations
*******************************************************************************/

/** ****************************************************************************
 * Name        : CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM_UINT16
 *
 *******************************************************************************/
enum CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM
{
    ID_CSDR_CTTF_1X_SYNCH_DATA_IND                          = 0x0000, /**< @sa CSDR_CTTF_1X_SYNCH_DATA_IND_STRU */
    ID_CSDR_CTTF_1X_PCH_DATA_IND                            = 0x0001, /**< @sa CSDR_CTTF_1X_PCH_DATA_IND_STRU */
    ID_CSDR_CTTF_1X_BCCH_DATA_IND                           = 0x0002, /**< @sa CSDR_CTTF_1X_BCCH_DATA_IND_STRU */
    ID_CSDR_CTTF_1X_FCCCH_DATA_IND                          = 0x0003, /**< @sa CSDR_CTTF_1X_FCCCH_DATA_IND_STRU */
    ID_CSDR_CTTF_1X_DCCH_TRACE_DATA_IND                     = 0x0004, /**< @sa CSDR_CTTF_1X_DCCH_TRACE_DATA_IND_STRU */
    ID_CSDR_CTTF_1X_FCH_TRACE_DATA_IND                      = 0x0005, /**< @sa CSDR_CTTF_1X_FCH_TRACE_DATA_IND_STRU */
    ID_CSDR_CTTF_1X_SCCH_TRACE_DATA_IND                     = 0x0006, /**< @sa CSDR_CTTF_1X_SCCH_TRACE_DATA_IND_STRU */
    ID_CSDR_CTTF_1X_SCH_TRACE_DATA_IND                      = 0x0007, /**< @sa CSDR_CTTF_1X_SCH_TRACE_DATA_IND_STRU */
    ID_CSDR_CTTF_1X_PCH_START_MONITORING_IND                = 0x0008, /**< @sa CSDR_CTTF_1X_PCH_START_MONITORING_IND_STRU */
    ID_CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM_BUTT               = 0xFFFF
};
typedef VOS_UINT16 CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM_UINT16;

/** ****************************************************************************
 * Name        : CSDR_1X_FCCCH_FRAME_TYPE_ENUM_UINT16
 *
 * Description : MAC will do statistics depends on FCCCH frame type
 *******************************************************************************/
enum CSDR_1X_FCCCH_FRAME_TYPE_ENUM
{
    CSDR_1X_FCCCH_FRAME_TYPE_38400BPS_5MS                   = 0x0000,
    CSDR_1X_FCCCH_FRAME_TYPE_19200BPS_10MS                  = 0x0001,
    CSDR_1X_FCCCH_FRAME_TYPE_38400BPS_10MS                  = 0x0002,
    CSDR_1X_FCCCH_FRAME_TYPE_9600BPS_20MS                   = 0x0003,
    CSDR_1X_FCCCH_FRAME_TYPE_19200BPS_20MS                  = 0x0004,
    CSDR_1X_FCCCH_FRAME_TYPE_38400BPS_20MS                  = 0x0005,
    CSDR_1X_FCCCH_FRAME_TYPE_BUTT                           = 0x0006
};
typedef VOS_UINT16 CSDR_1X_FCCCH_FRAME_TYPE_ENUM_UINT16;

/** ****************************************************************************
 * Name        : CSDR_1X_FRAME_QUALITY_ENUM_UINT16
 *
 * Description : good quality; insufficient quality; insufficient but know bit
 * rate
 * for fix rate: only "good quality", or "insufficient but know bit rate";
 * for variable rate: any of three
 *******************************************************************************/
enum CSDR_1X_FRAME_QUALITY_ENUM
{
    CSDR_1X_FRAME_QUALITY_GOOD                              = 0x0000,
    CSDR_1X_FRAME_QUALITY_INSUFFICIENT                      = 0x0001,
    CSDR_1X_FRAME_QUALITY_INSUFFICIENT_KNOWN_RATE           = 0x0002,
    CSDR_1X_FRAME_QUALITY_BUTT                              = 0x0003
};
typedef VOS_UINT16 CSDR_1X_FRAME_QUALITY_ENUM_UINT16;

/*******************************************************************************
 4. Message Header declaration
*******************************************************************************/

/*******************************************************************************
 5. Message declaration
*******************************************************************************/

/*******************************************************************************
 6. STRUCT and UNION declaration
*******************************************************************************/

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_F_DATA_BLK_STRU
 *
 * Description : All elements defined in this structure is always written by
 * PHY, even met poor quality or did not receive any data.
 * 
 * Every @pulDataPtr point to one of data block on that channel. Its value is
 * always fixed, it won't change except MS goes to any other mode, e.g, EVDO,
 * or WCDMA. It will be initialed by SDR when power on.
 * 
 * Only for good quality, SDR will put actual data. But it always fill all
 * elements in this structure by SDR.
 * 
 * @usNumBit: If it's good quality, it's actual frame number bit; If it's
 * insufficient but know bit rate, it's frame number bit from bit rate; If it's
 * insufficient quality but unknown bit rate, it's zero.
 *******************************************************************************/
typedef struct
{
    VOS_UINT16                          usNumBit;
    CSDR_1X_FRAME_QUALITY_ENUM_UINT16   enFrameQuality;            /**< good quality; insufficient quality; insufficient but know bit rate */
    CSDR_1X_FRAME_DURATION_ENUM_UINT16  enFrameDuration;           /**< 5ms, 10ms, 20ms, 40ms, 80ms */
    VOS_UINT16                          usCfgId;                   /**< config id from L3 config per channel */
    VOS_UINT32                          aulFrameRxedSystemTime[2]; /**< CDMA System Time(in BBP register format) corresponding to the frame in which the first bit was received, in uint of 80ms; [0] is low part(in CBBP_SYS_CNT_1X_RPT2_UNION format), [1] is high part */
    CSDR_1X_CODE_TYPE_ENUM_UINT16       enDecodeType;
    VOS_UINT16                          ausReserve[3];             /**< reserve for future */
    VOS_UINT32                          ulDataAddr;
} CSDR_CTTF_1X_F_DATA_BLK_STRU;

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_F_DATA_BUFFER_STRU
 *
 * Description : It's used to corporate with per channel data management to
 * indicate where and how many bits has been received on certain type channel.
 * Each channel can store 3 data blocks at most.
 * It's suggested put in memory as the occupied size.
 *******************************************************************************/
typedef struct
{
    VOS_UINT16                          ausDcchData[CSDR_1X_MAX_BLK_NUM_PER_RING_BUFFER][CSDR_1X_FDCCH_BLK_MAX_SIZE];
    VOS_UINT16                          ausFchData[CSDR_1X_MAX_BLK_NUM_PER_RING_BUFFER][CSDR_1X_FFCH_BLK_MAX_SIZE];
    VOS_UINT16                          ausScchData[CSDR_1X_MAX_BLK_NUM_PER_RING_BUFFER][CSDR_1X_FSCCH_BLK_MAX_SIZE];
    VOS_UINT16                          ausSchData[CSDR_1X_MAX_BLK_NUM_PER_RING_BUFFER][CSDR_1X_FSCH_BLK_MAX_SIZE];
} CSDR_CTTF_1X_F_DATA_BUFFER_STRU;

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_F_DATA_INFO_STRU
 *
 * Description : @usReceivedInd: it's used to indicate whether SDR received
 * data or not. 1 means received, 0 means not.
 * Even @usReceivedInd is 0, SDR still needs to fill elements defined in
 * CSDR_CTTF_1X_F_DATA_BLK_STRU.
 *******************************************************************************/
typedef struct
{
    VOS_UINT16                          usReceivedInd; /**< indicate received data or not on this channel */
    VOS_UINT16                          usReserve;
    CSDR_CTTF_1X_F_DATA_BLK_STRU        stDataBlock;
} CSDR_CTTF_1X_F_DATA_INFO_STRU;

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_GENERIC_DATA_IND_STRU
 *
 * Description : PHY use head index, MAC use tail index. Both are initialed to
 * zero by DSP when power up.
 * if head index == tail index, it means empty; if head index + 1 mod
 * CSDR_1X_MAX_BLK_NUM_PER_RING_BUFFER == tail index, it means full
 * When buffer is full, SDR drops new data. Otherwise, SDR puts data into the
 * place of head index, then plus one to head index.
 *******************************************************************************/
typedef struct
{
    VOS_UINT16                          usHeadIdx;                                          /**< PHY use head index */
    VOS_UINT16                          usTailIdx;                                          /**< MAC use tail index */
    CSDR_CTTF_1X_F_DATA_INFO_STRU       ast1xDataInfo[CSDR_1X_MAX_BLK_NUM_PER_RING_BUFFER];
} CSDR_CTTF_1X_GENERIC_DATA_IND_STRU;

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_F_CTRL_MGMT_STRU
 *
 * Description : As using share memory between 1x_Layer_1 and CTTF1XFMAC to
 * implement dedicate channel data indication. We decide using ring buffer to
 * maintain data in the memory. The structure is the ring buffer ctrl
 * information. Per channel has a 3 blocks in ring buffer. PHY use head index,
 * MAC use tail index.
 * We call this structure "forward control management structure". It's stored
 * in SDR TCM non-power-down area.
 * When power-on or RAT turns back to CDMA, SDR will initial this structure and
 * link data pointers.
 * When release a channel, MAC need to set its tail index to head index.
 * 
 * When releasing a channel, CTTF will empty the buffer. There is however a
 * possibility that CSDR writes another frame of data into the buffer after
 * CTTF has released the channel.
 * -  Any data sent after CTTF has released will be lost.
 * -  When the NW/CAS orders the release, it cannot be a problem.
 * -  For F-SCH this means that we lose the last frame of an assignment. This
 *    is not judged to be a problem, as the NW will most likely not schedule
 *    any data in the last frame.
 * -  CSDR will write at most one more entry into the buffer. If the buffer is
 *    full (which we assume shall never happen, since CTTF empties the buffer,
 *    and CSDR writes at most one more entry), CSDR shall log it.
 * -  At activation, CSDR will start writing to the buffer again. At most one
 *    position of the buffer can be occupied at start.
 * -  At activation CTTF will start reading the buffer, and can discard any
 *    data with old CfgId (CTTF shall log this)
 *******************************************************************************/
typedef struct
{
    CSDR_CTTF_1X_GENERIC_DATA_IND_STRU  stDcchDataInd;
    CSDR_CTTF_1X_GENERIC_DATA_IND_STRU  stFchDataInd;
    CSDR_CTTF_1X_GENERIC_DATA_IND_STRU  stScchDataInd;
    CSDR_CTTF_1X_GENERIC_DATA_IND_STRU  stSchDataInd;
} CSDR_CTTF_1X_F_CTRL_MGMT_STRU;

/*******************************************************************************
 7. OTHER declarations
*******************************************************************************/

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_PCH_START_MONITORING_IND_STRU
 *
 * Description : SDR will use CSDR_CTTF_1X_PCH_START_MONITORING_IND to notify
 * CTTF1XFLAC start monitoring PCH channel. CTTF1XFLAC will trigger start timer
 * to monitor PCH channel.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM_UINT16                enMsgId;
    VOS_UINT16                                              usOpId;            /* add for adapting EA,no use */
    VOS_UINT16                                              usCfgId;           /**< each time PCH channel established, make it unique */
    VOS_UINT16                                              ausReserveData[1];
} CSDR_CTTF_1X_PCH_START_MONITORING_IND_STRU;

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_DCCH_TRACE_DATA_IND_STRU
 *
 * Description : This signal is used to trace PHY-DCCH.Indication, including
 * parameters are defined in section 2.2.2.4.8 in 3GPP2 C.S0002-A v6.0:
 * "Physical Layer Standard for cdma2000 Spread Spectrum Systems" and
 * 2.2.1.1.1.7.6 in 3GPP2 C.S0003-A v6.0: "Medium Access Control (MAC) Standard
 * for cdma2000 Spread Spectrum Systems".
 * 
 * It will use share memory between 1x_Layer_1 and CTTF1XFMAC to implement. See
 * more detail in CSDR_CTTF_1X_F_CTRL_MGMT_STRU and
 * CSDR_CTTF_1X_F_DATA_BUFFER_STRU.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM_UINT16                enMsgId;
    VOS_UINT16                                              usOpId;
} CSDR_CTTF_1X_DCCH_TRACE_DATA_IND_STRU;

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_FCH_TRACE_DATA_IND_STRU
 *
 * Description : This signal is used to trace PHY-FCH.Indication, including
 * parameters are defined in section 2.2.2.4.9 in 3GPP2 C.S0002-A v6.0:
 * "Physical Layer Standard for cdma2000 Spread Spectrum Systems" and
 * 2.2.1.1.1.7.5 in 3GPP2 C.S0003-A v6.0: "Medium Access Control (MAC) Standard
 * for cdma2000 Spread Spectrum Systems".
 * 
 * It will use share memory between 1x_Layer_1 and CTTF1XFMAC to implement. See
 * more detail in CSDR_CTTF_1X_F_CTRL_MGMT_STRU and
 * CSDR_CTTF_1X_F_DATA_BUFFER_STRU.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM_UINT16                enMsgId;
    VOS_UINT16                                              usOpId;
} CSDR_CTTF_1X_FCH_TRACE_DATA_IND_STRU;

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_SCCH_TRACE_DATA_IND_STRU
 *
 * Description : This signal is used to trace PHY-SCCH.Indication, including
 * parameters are defined in section 2.2.2.4.11 in 3GPP2 C.S0002-A v6.0:
 * "Physical Layer Standard for cdma2000 Spread Spectrum Systems" and
 * 2.2.1.1.1.7.7 in 3GPP2 C.S0003-A v6.0: "Medium Access Control (MAC) Standard
 * for cdma2000 Spread Spectrum Systems".
 * 
 * It will use share memory between 1x_Layer_1 and CTTF1XFMAC to implement. See
 * more detail in CSDR_CTTF_1X_F_CTRL_MGMT_STRU and
 * CSDR_CTTF_1X_F_DATA_BUFFER_STRU.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM_UINT16                enMsgId;
    VOS_UINT16                                              usOpId;
} CSDR_CTTF_1X_SCCH_TRACE_DATA_IND_STRU;

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_SCH_TRACE_DATA_IND_STRU
 *
 * Description : This signal is used to trace PHY-SCH.Indication, including
 * parameters are defined in section 2.2.2.4.10 in 3GPP2 C.S0002-A v6.0:
 * "Physical Layer Standard for cdma2000 Spread Spectrum Systems" and
 * 2.2.1.1.1.7.8 in 3GPP2 C.S0003-A v6.0: "Medium Access Control (MAC) Standard
 * for cdma2000 Spread Spectrum Systems".
 * 
 * It will use share memory between 1x_Layer_1 and CTTF1XFMAC to implement. See
 * more detail in CSDR_CTTF_1X_F_CTRL_MGMT_STRU and
 * CSDR_CTTF_1X_F_DATA_BUFFER_STRU.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM_UINT16                enMsgId;
    VOS_UINT16                                              usOpId;
} CSDR_CTTF_1X_SCH_TRACE_DATA_IND_STRU;

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_BCCH_DATA_IND_STRU
 *
 * Description : SDR will use CSDR_CTTF_1X_BCCH_DATA_IND to transfer BCCH data
 * to CTTF1XFMAC.
 * The CfgId will be used as channel_id. Its values included in any two
 * MAC-Data.Indication primitives shall be the same if all of the following are
 * true, refer to section 2.2.1.1.2.3.9 in 3GPP2 C.S0003-A v6.0: "Medium Access
 * Control (MAC) Standard for cdma2000 Spread Spectrum Systems":
 * ?   the received data were transmitted by the same base station,
 * ?   the received data were transmitted on the same code channel,
 * ?   the received data were transmitted on the same CDMA Channel.
 * Otherwise, the channel_id values included in the MAC-Data.Indication
 * primitives shall have distinct values.
 * enFrameDuration: SDR needs to indicate frame duration, MAC does statistics
 * depended on frame duration. It could be 40ms, 80ms or 160ms, but @usNumBit
 * is always the same.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM_UINT16                enMsgId;
    VOS_UINT16                                              usOpId;
    VOS_UINT16                                              usNumBit;                           /**< data size in bit */
    CSDR_1X_FRAME_QUALITY_ENUM_UINT16                       enFrameQuality;
    VOS_UINT16                                              ausData[CSDR_1X_BCCH_BLK_MAX_SIZE];
    VOS_UINT16                                              usCfgId;                            /**< each time BCCH channel established, make it unique */
    CSDR_1X_FRAME_DURATION_ENUM_UINT16                      enFrameDuration;
    VOS_UINT16                                              ausReserve1[1];
    VOS_UINT32                                              aulSystemTime[2];                   /**< CDMA System Time(in BBP register format) corresponding to the frame in which the first bit was received; [0] is low part(in CBBP_SYS_CNT_1X_RPT2_UNION format), [1] is high part */
} CSDR_CTTF_1X_BCCH_DATA_IND_STRU;

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_FCCCH_DATA_IND_STRU
 *
 * Description : SDR will use CSDR_CTTF_1X_FCCCH_DATA_IND to deliver F-CCCH
 * data to CTTF1XFMAC.
 * The CfgId will be used as channel_id. Its values included in any two
 * MAC-Data.Indication primitives shall be the same if all of the following are
 * true, refer to section 2.2.1.1.2.3.9 in 3GPP2 C.S0003-A v6.0: "Medium Access
 * Control (MAC) Standard for cdma2000 Spread Spectrum Systems":
 * ?   the received data were transmitted by the same base station,
 * ?   the received data were transmitted on the same code channel,
 * ?   the received data were transmitted on the same CDMA Channel.
 * Otherwise, the channel_id values included in the MAC-Data.Indication
 * primitives shall have distinct values.
 * enFrameDuration: SDR needs to indicate frame duration, MAC does statistics
 * depended on frame duration. It could be 5ms, 10ms or 20ms
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM_UINT16                enMsgId;
    VOS_UINT16                                              usOpId;
    VOS_UINT16                                              usNumBit;                            /**< data size in bit */
    CSDR_1X_FRAME_QUALITY_ENUM_UINT16                       enFrameQuality;
    VOS_UINT16                                              ausData[CSDR_1X_FCCCH_BLK_MAX_SIZE];
    VOS_UINT16                                              usCfgId;                             /**< each time F-CCCH channel established, make it unique */
    CSDR_1X_FCCCH_FRAME_TYPE_ENUM_UINT16                    enFrameType;
    VOS_UINT16                                              ausReserve1[1];
    VOS_UINT32                                              aulSystemTime[2];                    /**< CDMA System Time(in BBP register format) corresponding to the frame in which the first bit was received; [0] is low part(in CBBP_SYS_CNT_1X_RPT2_UNION format), [1] is high part */
} CSDR_CTTF_1X_FCCCH_DATA_IND_STRU;

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_PCH_DATA_IND_STRU
 *
 * Description : SDR will use CSDR_CTTF_1X_PCH_DATA_IND to deliver PCH data to
 * CTTF1XFMAC.
 * The CfgId will be used as channel_id. Its values included in any two
 * MAC-Data.Indication primitives shall be the same if all of the following are
 * true, refer to section 2.2.1.1.2.3.9 in 3GPP2 C.S0003-A v6.0: "Medium Access
 * Control (MAC) Standard for cdma2000 Spread Spectrum Systems":
 * ?   the received data were transmitted by the same base station,
 * ?   the received data were transmitted on the same code channel,
 * ?   the received data were transmitted on the same CDMA Channel.
 * Otherwise, the channel_id values included in the MAC-Data.Indication
 * primitives shall have distinct values.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM_UINT16                enMsgId;
    VOS_UINT16                                              usOpId;
    VOS_UINT16                                              usNumBit;                          /**< data size in bit */
    VOS_UINT16                                              usCfgId;                           /**< each time PCH channel established, make it unique */
    VOS_UINT16                                              ausData[CSDR_1X_PCH_BLK_MAX_SIZE];
    VOS_UINT32                                              aulSystemTime[2];                  /**< CDMA System Time(in BBP register format) corresponding to the frame in which the first bit was received; [0] is low part(in CBBP_SYS_CNT_1X_RPT2_UNION format), [1] is high part */
} CSDR_CTTF_1X_PCH_DATA_IND_STRU;

/** ****************************************************************************
 * Name        : CSDR_CTTF_1X_SYNCH_DATA_IND_STRU
 *
 * Description : It's used to transfer SYNCH data from 1x_Layer_1 to
 * CTTF1XFMAC. SDR will use CSDR_CTTF_1X_SYNCH_DATA_IND to deliver synch data
 * to CTTF1XFMAC. As in section 2.2.1.1.2.3.9 in 3GPP2 C.S0003-A v6.0: "Medium
 * Access Control (MAC) Standard for cdma2000 Spread Spectrum Systems", SRBP
 * needs to know data from the same CDMA channel, the same code channel, and
 * same base station, so need PROC to carry a unique channel id.
 * 
 * The CfgId will be used as channel_id. Its values included in any two
 * MAC-Data.Indication primitives shall be the same if all of the following are
 * true:
 * -  the received data were transmitted by the same base station,
 * -  the received data were transmitted on the same code channel,
 * -  the received data were transmitted on the same CDMA Channel.
 * Otherwise, the channel_id values included in the MAC-Data.Indication
 * primitives shall have distinct values.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CSDR_1X_DECDATA_PIF_MSG_TYPE_ENUM_UINT16                enMsgId;
    VOS_UINT16                                              usOpId;
    VOS_UINT16                                              usNumBit;                           /**< data size in bit */
    VOS_UINT16                                              usCfgId;                            /**< each time SYNC channel established, make it unique */
    VOS_UINT16                                              ausData[CSDR_1X_SYNC_BLK_MAX_SIZE];
    VOS_UINT32                                              ulBaseTime;
} CSDR_CTTF_1X_SYNCH_DATA_IND_STRU;

/*******************************************************************************
 8. Global  declaration
*******************************************************************************/

/*******************************************************************************
 9. Function declarations
*******************************************************************************/

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
