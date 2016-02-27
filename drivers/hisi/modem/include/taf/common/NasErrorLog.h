/*
* Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
* foss@huawei.com
*
* If distributed as part of the Linux kernel, the following license terms
* apply:
*
* * This program is free software; you can redistribute it and/or modify
* * it under the terms of the GNU General Public License version 2 and
* * only version 2 as published by the Free Software Foundation.
* *
* * This program is distributed in the hope that it will be useful,
* * but WITHOUT ANY WARRANTY; without even the implied warranty of
* * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* * GNU General Public License for more details.
* *
* * You should have received a copy of the GNU General Public License
* * along with this program; if not, write to the Free Software
* * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
*
* Otherwise, the following license terms apply:
*
* * Redistribution and use in source and binary forms, with or without
* * modification, are permitted provided that the following conditions
* * are met:
* * 1) Redistributions of source code must retain the above copyright
* *    notice, this list of conditions and the following disclaimer.
* * 2) Redistributions in binary form must reproduce the above copyright
* *    notice, this list of conditions and the following disclaimer in the
* *    documentation and/or other materials provided with the distribution.
* * 3) Neither the name of Huawei nor the names of its contributors may
* *    be used to endorse or promote products derived from this software
* *    without specific prior written permission.
*
* * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef __NAS_ERRORLOG_H__
#define __NAS_ERRORLOG_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "errorlog.h"
#include "omerrorlog.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/

#define NAS_FTM_MAX_PTMSI_LEN                           (4)                     /* ����PTMSI�ĳ��� */
#define NAS_FTM_MAX_TMSI_LEN                            (4)                     /* ����TMSI�ĳ��� */

#define TAF_SDC_RING_BUFFER_SIZE                        (1024 * 2)              /* TAF��Ļ��ι��������С */
#define NAS_MML_RING_BUFFER_SIZE                        (1024 * 6)              /* MM��Ļ��ι��������С */
#define NAS_CC_RING_BUFFER_SIZE                         (1024 * 2)              /* CC��Ļ��ι��������С */

#if (FEATURE_ON == FEATURE_LTE)
#define NAS_ERR_LOG_MML_MAX_RAT_NUM                     (3)                     /* ��ǰ֧�ֵĽ��뼼������ */
#else
#define NAS_ERR_LOG_MML_MAX_RAT_NUM                     (2)                     /* ��ǰ֧�ֵ�GU���뼼������ */
#endif

#define NAS_ERR_LOG_CTRL_LEVEL_NULL                     (0)                     /* ErrLog�ȼ�δ����*/
#define NAS_ERR_LOG_CTRL_LEVEL_CRITICAL                 (1)                     /* ErrLog�ȼ�Ϊ���� */
#define NAS_ERR_LOG_CTRL_LEVEL_MAJOR                    (2)                     /* ErrLog�ȼ�Ϊ��Ҫ */
#define NAS_ERR_LOG_CTRL_LEVEL_MINOR                    (3)                     /* ErrLog�ȼ�Ϊ��Ҫ */
#define NAS_ERR_LOG_CTRL_LEVEL_WARNING                  (4)                     /* ErrLog�ȼ�Ϊ��ʾ */

/* Added by zwx247453 for CHR optimize, 2015-3-13 Begin */
#define NAS_ERR_LOG_MAX_RAT_SWITCH_RECORD_MUN           (20)                    /* ָ��ʱ��GUTLģʽ�л������� */
#define NAS_ERR_LOG_MAX_RAT_SWITCH_STATISTIC_TIME       (172800)                /* GUTLģʽ���ͳ��ʱ�䣬��λ�� */

#define NAS_ERR_LOG_ACTIVE_RPT_FLAG_OFFSET              (7)
#define NAS_ERR_LOG_RAT_SWITCH_RPT_FLAG_OFFSET          (2)
/* Added by zwx247453 for CHR optimize, 2015-3-13 End */

#define NAS_ERR_LOG_MAX_POSITION_RECORD_NUM             (8)                     /* λ������¼�������� */

#define NAS_ERR_LOG_MAX_CSFB_MT_STATE_RECORD_NUM        (32)                    /* CSFB MT��¼��״̬������ */

#define NAS_INFO_CLT_MAX_PLMN_NUM                       (32)                    /* PLMN�������� */

#define NAS_INFO_CLT_MAX_GSM_FREQ_NUM                   (60)                    /* GSMƵ��������� */

#define NAS_INFO_CLT_MAX_WCDMA_FREQ_NUM                 (16)                    /* WCDMAƵ��������� */

#define NAS_INFO_CLT_MAX_LTE_FREQ_NUM                   (16)                    /* LTEƵ��������� */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


enum NAS_ERR_LOG_NET_RAT_TYPE_ENUM
{
    NAS_ERR_LOG_NET_RAT_TYPE_GSM,           /* GSM���뼼�� */
    NAS_ERR_LOG_NET_RAT_TYPE_WCDMA,         /* WCDMA���뼼�� */
    NAS_ERR_LOG_NET_RAT_TYPE_LTE,           /* LTE���뼼�� */
    NAS_ERR_LOG_NET_RAT_TYPE_BUTT           /* ��Ч�Ľ��뼼�� */
};
typedef VOS_UINT8 NAS_ERR_LOG_NET_RAT_TYPE_ENUM_UINT8;


enum NAS_ERR_LOG_PS_ACTION_TYPE_ENUM
{
    NAS_ERR_LOG_GMM_ACTION_RAU                  = 0,
    NAS_ERR_LOG_GMM_ACTION_ATTACH               = 1,
    NAS_ERR_LOG_GMM_ACTION_COMBINED_RAU         = 2,
    NAS_ERR_LOG_GMM_ACTION_COMBINED_ATTACH      = 3,
    NAS_ERR_LOG_GMM_ACTION_PERIODC_RAU          = 4,
    NAS_ERR_LOG_GMM_ACTION_BUTT
};
typedef VOS_UINT32 NAS_ERR_LOG_GMM_ACTION_TYPE_ENUM_U32;


enum NAS_ERR_LOG_REG_DOMAIN_ENUM
{
    NAS_ERR_LOG_REG_DOMAIN_PS    = 0x00,                                        /* ע������ΪPS�� */
    NAS_ERR_LOG_REG_DOMAIN_PS_CS = 0x01,                                        /* ע������ΪCS+PS�� */
    NAS_ERR_LOG_REG_DOMAIN_BUTT
};
typedef VOS_UINT32 NAS_ERR_LOG_REG_DOMAIN_ENUM_UINT32;


enum NAS_ERR_LOG_CALL_EX_STATE_ENUM
{
    NAS_ERR_LOG_CALL_S_ACTIVE,                                                  /* active */
    NAS_ERR_LOG_CALL_S_HELD,                                                    /* held */
    NAS_ERR_LOG_CALL_S_DIALING,                                                 /* dialing (MO call) */
    NAS_ERR_LOG_CALL_S_ALERTING,                                                /* alerting (MO call) */
    NAS_ERR_LOG_CALL_S_INCOMING,                                                /* incoming (MT call) */
    NAS_ERR_LOG_CALL_S_WAITING,                                                 /* waiting (MT call) */
    NAS_ERR_LOG_CALL_S_IDLE,                                                    /* idle */
    NAS_ERR_LOG_CALL_S_CCBS_WAITING_ACTIVE,                                     /* CCBS�ȴ�����̬ */
    NAS_ERR_LOG_CALL_S_CCBS_WAITING_RECALL,                                     /* CCBS�ȴ��غ�̬ */
    NAS_ERR_LOG_CALL_S_UNKNOWN,                                                 /* unknown state */
    NAS_ERR_LOG_CALL_S_MO_FDN_CHECK,                                            /* FDN check */
    NAS_ERR_LOG_CALL_S_MO_CALL_CONTROL_CHECK,                                   /* CALL CONTROL check */
    NAS_ERR_LOG_CALL_S_BUTT
};
typedef VOS_UINT8  NAS_ERR_LOG_CALL_STATE_ENUM_U8;


enum NAS_ERR_LOG_ALM_ID_ENUM
{
    NAS_ERR_LOG_ALM_CS_REG_FAIL                             = 0x01,             /* CS��ע��ʧ�� */
    NAS_ERR_LOG_ALM_PS_REG_FAIL                             = 0x02,             /* PS��ע��ʧ�� */
    NAS_ERR_LOG_ALM_OOS_NW_SRCH_PROC_INFO                   = 0x03,             /* ��������������������Ϣ */
    NAS_ERR_LOG_ALM_CS_CALL_FAIL                            = 0x04,             /* CS����ʧ�ܼ��쳣�Ҷ� */
    NAS_ERR_LOG_ALM_PS_CALL_FAIL                            = 0x05,             /* PS����ʧ�ܼ��쳣�Ҷ� */
    NAS_ERR_LOG_ALM_SMS_FAIL                                = 0x06,             /* ����ʧ�� */
    NAS_ERR_LOG_ALM_VC_OPT_FAIL                             = 0x07,             /* VC����ʧ�� */
    NAS_ERR_LOG_ALM_CS_PAGING_FAIL                          = 0x08,             /* CS PAGING fail */
    NAS_ERR_LOG_ALM_CS_MT_CALL_FAIL                         = 0x09,             /* CS MT fail */
    NAS_ERR_LOG_ALM_CSFB_MT_CALL_FAIL                       = 0x0a,             /* CSFB MT fail */
    NAS_ERR_LOG_ALM_MNTN                                    = 0x0b,             /* ���ϸ澯�Ŀ�ά�ɲ� */
    NAS_ERR_LOG_ALM_NW_DETACH_IND                           = 0x0c,             /* ���緢���DETACHָʾ */

    NAS_ERR_LOG_ALM_PS_SRV_REG_FAIL                         = 0x0d,             /* PS SERVICE���� */
    NAS_ERR_LOG_ALM_CM_SRV_REJ_IND                          = 0x0e,             /* CS SERVICE�ܾ� */
    NAS_ERR_LOG_ALM_MO_DETACH_IND                           = 0x0f,             /* ���ط����DETACH */
    NAS_ERR_LOG_ALM_RAT_FREQUENTLY_SWITCH                   = 0x10,             /* 4G��2/3GƵ���л� */

    NAS_ERR_LOG_ALM_OOS_BEGIN                               = 0x11,             /* �޷���ʼ */
    NAS_ERR_LOG_ALM_OOS_END                                 = 0x12,             /* �޷������ */

    NAS_ERR_LOG_ALM_PORCESS_INFO_IND                        = 0x13,             /* ������Ϣָʾ */

    NAS_ERR_LOG_ALM_FFT_SRCH_MCC_INFO                        = 0x14,             /* FFT����MCC�Ľ�� */

    NAS_ERR_LOG_ALM_ID_BUTT
};
typedef VOS_UINT16  NAS_ERR_LOG_ALM_ID_ENUM_U16;


enum NAS_ERR_LOG_UNSOLI_REPORT_TYPE_ENUM
{
    NAS_ERR_LOG_FAULT_REPORT                                = 0x01,             /* �����ϱ� */
    NAS_ERR_LOG_ALARM_REPORT                                = 0x02,             /* �澯�ϱ� */

    NAS_ERR_LOG_REPORT_TYPE_BUTT
};
typedef VOS_UINT16  NAS_ERR_LOG_UNSOLI_REPORT_TYPE_ENUM_U16;


enum NAS_ERR_LOG_ALM_TYPE_ENUM
{
    NAS_ERR_LOG_ALM_TYPE_COMMUNICATION    = 0x00,                               /* ͨ�� */
    NAS_ERR_LOG_ALM_TYPE_TRAFFIC_QUALITY  = 0x01,                               /* ҵ������ */
    NAS_ERR_LOG_ALM_TYPE_DEAL_WRONG       = 0x02,                               /* �������� */
    NAS_ERR_LOG_ALM_TYPE_DEVICE_FAILURE   = 0x03,                               /* �豸���� */
    NAS_ERR_LOG_ALM_TYPE_ENV_FAILURE      = 0x04,                               /* �������� */
    NAS_ERR_LOG_ALM_TYPE_BUTT
};
typedef VOS_UINT16  NAS_ERR_LOG_ALM_TYPE_ENUM_U16;
enum NAS_ERR_LOG_FTM_PROJECT_ID_ENUM
{
    NAS_FTM_PROJECT_TMSI_RPT,
    NAS_FTM_PROJECT_PTMSI_RPT,

    NAS_FTM_PROJECT_ID_BUTT
};
typedef VOS_UINT16  NAS_ERR_LOG_FTM_PROJECT_ID_ENUM_U8;


enum NAS_ERR_LOG_CS_PAGING_CAUSE_ENUM
{
    NAS_ERR_LOG_CS_PAGING_CAUSE_MM_STATE_ERR       = 1,                         /* MM״̬����ȷ */
    NAS_ERR_LOG_CS_PAGING_CAUSE_MM_PAGING_BAR      = 2,                         /* Ѱ������ */
    NAS_ERR_LOG_CS_PAGING_CAUSE_EST_FAIL           = 3,                         /* ���ӽ���ʧ�� */
    NAS_ERR_LOG_CS_PAGING_CAUSE_EST_TIMEOUT        = 4,                         /* ���ӽ�����ʱ */
    NAS_ERR_LOG_CS_PAGING_CAUSE_EST_RELEASED       = 5,                         /* ���ӽ������ͷ� */
    NAS_ERR_LOG_CS_PAGING_CAUSE_CONN_RELEASED      = 6,                         /* ���ӱ��ͷ� */
    NAS_ERR_LOG_CS_PAGING_CAUSE_CONN_ABORT         = 7,                         /* ҵ��ABORT */

    NAS_ERR_LOG_CS_PAGING_CAUSE_BUTT
};
typedef VOS_UINT32  NAS_ERR_LOG_CS_PAGING_CAUSE_ENUM_U32;
enum NAS_ERR_LOG_CS_MT_CALL_CAUSE_ENUM
{
    /* CC���쳣ԭ��ֵ */
    NAS_ERR_LOG_CS_MT_CALL_CAUSE_TI_CODED_AS_111        = 1,                    /* TIֵΪ7 */
    NAS_ERR_LOG_CS_MT_CALL_CAUSE_TI_INC_SET_TO_1        = 2,                    /* TIֵ������Ϊ1 */
    NAS_ERR_LOG_CS_MT_CALL_CAUSE_TI_IS_USED             = 3,                    /* TI��ʹ�� */
    NAS_ERR_LOG_CS_MT_CALL_CAUSE_CC_CREATE_ENTYTY_FAIL  = 4,                    /* CC����ʵ��ʧ�� */
    NAS_ERR_LOG_CS_MT_CALL_CAUSE_NEW_CALL_NOT_ALLOW     = 5,                    /* �����ٷ����µĺ��� */
    NAS_ERR_LOG_CS_MT_CALL_CAUSE_OPTIONAL_IE_ERR        = 6,                    /* SETUP������IE���ʧ�� */
    NAS_ERR_LOG_CS_MT_CALL_CAUSE_DECODE_FAIL            = 7,                    /* CC����ʧ�� */

    /* CALL���쳣ԭ��ֵ */
    NAS_ERR_LOG_CS_MT_CALL_CAUSE_CALL_STATE_CTRL_NOT_SUPPORT  = 8,              /* �������в�֧�� */
    NAS_ERR_LOG_CS_MT_CALL_CAUSE_BC_CHECK_FAIL          = 9,                    /* BC���ʧ�� */
    NAS_ERR_LOG_CS_MT_CALL_CAUSE_MT_CALL_NOT_ALLOW      = 10,                   /* ����������MT���� */
    NAS_ERR_LOG_CS_MT_CALL_CAUSE_NE_GET_BC_FAIL         = 11,                   /* BCЭ��ʧ�� */

    NAS_ERR_LOG_CS_MT_CALL_CAUSE_BUTT
};
typedef VOS_UINT32  NAS_ERR_LOG_CS_MT_CALL_CAUSE_ENUM_U32;
enum NAS_ERR_BASE_LOG_STATE_ENUM
{
    NAS_ERR_LOG_BASE_STATE_NULL                                = 0x00,          /* ��Чֵ */
    NAS_ERR_LOG_BASE_STATE_LMM_CS_PAGING_RECEIVED              = 0x01,          /* LMM��CSFB PAGING�Ѿ����� */
    NAS_ERR_LOG_BASE_STATE_TRAG_NW_SEARCH_FOR_RCV_LMM_SRV_RST  = 0x02,          /* ���յ�LMM��Service Resultָʾ,�������� */
    NAS_ERR_LOG_BASE_STATE_END_IND_RECEIVED                    = 0x03,          /* ���յ�LMM��Service End Indָʾ */
    NAS_ERR_LOG_BASE_STATE_SUSPEND_IND_RECEIVED                = 0x04,          /* ���յ���ϵͳ��ʼ����ָʾ */
    NAS_ERR_LOG_BASE_STATE_LTE_RESUME_IND_RECEIVED             = 0x05,          /* ���յ�LTE�Ļָ�ָʾ */
    NAS_ERR_LOG_BASE_STATE_GU_RESUME_IND_RECEIVED              = 0x06,          /* ���յ���ϵͳ�����ָ�ָʾ */
    NAS_ERR_LOG_BASE_STATE_RESUME_TO_GU_FAIL                   = 0x07,          /* ��ϵͳ�ָ�ʧ�� */
    NAS_ERR_LOG_BASE_STATE_LTE_SYS_INFO_RECEIVED               = 0x08,          /* ���յ�LTE��ϵͳ��Ϣ */
    NAS_ERR_LOG_BASE_STATE_GU_SYS_INFO_RECEIVED                = 0x09,          /* ���յ�GU��ϵͳ��Ϣ */
    NAS_ERR_LOG_BASE_STATE_PAGING_RSP_SEND                     = 0x0a,          /* LAIδ�����ı䣬����paging response */
    NAS_ERR_LOG_BASE_STATE_TRAG_LAU                            = 0x0b,          /* LAI�����ı�,����LAU */
    NAS_ERR_LOG_BASE_STATE_EST_FAILED                          = 0x0c,          /* ҵ����ʧ�ܣ���LAU���� */
    NAS_ERR_LOG_BASE_STATE_EST_SUCCESSED                       = 0x0d,          /* ҵ�����ɹ�����LAU���� */
    NAS_ERR_LOG_BASE_STATE_AUTH_REJ                            = 0x0e,          /* LAU��Ȩ���� */
    NAS_ERR_LOG_BASE_STATE_AUTH_FAIL                           = 0x0f,          /* LAU��Ȩʧ�� */
    NAS_ERR_LOG_BASE_STATE_T3240_TIMEOUT                       = 0x10,          /* T3240��ʱ����ʱ */
    NAS_ERR_LOG_BASE_STATE_LAU_REJECT                          = 0x11,          /* LAU���� */
    NAS_ERR_LOG_BASE_STATE_LAU_ACCEPT                          = 0x12,          /* LAU�ɹ� */
    NAS_ERR_LOG_BASE_STATE_RRC_REL_IND_RECEIVED                = 0x13,          /* ���յ�RRC�����ͷ�ָʾ */
    NAS_ERR_LOG_BASE_STATE_CSFB_MT_PROTECT_TI_TIME_OUT         = 0x14,          /* CSFB MT������ʱ����ʱ */
    NAS_ERR_LOG_BASE_STATE_CSFB_MT_TI_TIME_OUT_RCV_REL_IND     = 0x15,          /* CSFB MT������ʱ����ʱ״̬���յ������REL IND */
    NAS_ERR_LOG_BASE_STATE_CM_SERVICE_MSG_RECEIVED             = 0x16,          /* CSFB MTҵ����Ϣ�Ѿ����� */

    NAS_ERR_LOG_BASE_STATE_BUTT
};
typedef VOS_UINT32  NAS_ERR_LOG_BASE_STATE_ENUM_U32;
enum NAS_ERR_LOG_CSFB_MT_STATE_ENUM
{
    NAS_ERR_LOG_CSFB_MT_STATE_NULL                                = 0x00,       /* ��Чֵ */
    NAS_ERR_LOG_CSFB_MT_STATE_LMM_CS_PAGING_RECEIVED              = 0x01,       /* LMM��CSFB PAGING�Ѿ����� */
    NAS_ERR_LOG_CSFB_MT_STATE_TRAG_NW_SEARCH_FOR_RCV_LMM_SRV_RST  = 0x02,       /* ���յ�LMM��Service Resultָʾ,�������� */
    NAS_ERR_LOG_CSFB_MT_STATE_END_IND_RECEIVED                    = 0x03,       /* ���յ�LMM��Service End Indָʾ */
    NAS_ERR_LOG_CSFB_MT_STATE_SUSPEND_IND_RECEIVED                = 0x04,       /* ���յ���ϵͳ��ʼ����ָʾ */
    NAS_ERR_LOG_CSFB_MT_STATE_LTE_RESUME_IND_RECEIVED             = 0x05,       /* ���յ�LTE�Ļָ�ָʾ */
    NAS_ERR_LOG_CSFB_MT_STATE_GU_RESUME_IND_RECEIVED              = 0x06,       /* ���յ���ϵͳ�����ָ�ָʾ */
    NAS_ERR_LOG_CSFB_MT_STATE_RESUME_TO_GU_FAIL                   = 0x07,       /* ��ϵͳ�ָ�ʧ�� */
    NAS_ERR_LOG_CSFB_MT_STATE_LTE_SYS_INFO_RECEIVED               = 0x08,       /* ���յ�LTE��ϵͳ��Ϣ */
    NAS_ERR_LOG_CSFB_MT_STATE_GU_SYS_INFO_RECEIVED                = 0x09,       /* ���յ�GU��ϵͳ��Ϣ */
    NAS_ERR_LOG_CSFB_MT_STATE_PAGING_RSP_SEND                     = 0x0a,       /* LAIδ�����ı䣬����paging response */
    NAS_ERR_LOG_CSFB_MT_STATE_TRAG_LAU                            = 0x0b,       /* LAI�����ı�,����LAU */
    NAS_ERR_LOG_CSFB_MT_STATE_EST_FAILED                          = 0x0c,       /* ҵ����ʧ�ܣ���LAU���� */
    NAS_ERR_LOG_CSFB_MT_STATE_EST_SUCCESSED                       = 0x0d,       /* ҵ�����ɹ�����LAU���� */
    NAS_ERR_LOG_CSFB_MT_STATE_AUTH_REJ                            = 0x0e,       /* LAU��Ȩ���� */
    NAS_ERR_LOG_CSFB_MT_STATE_AUTH_FAIL                           = 0x0f,       /* LAU��Ȩʧ�� */
    NAS_ERR_LOG_CSFB_MT_STATE_T3240_TIMEOUT                       = 0x10,       /* T3240��ʱ����ʱ */
    NAS_ERR_LOG_CSFB_MT_STATE_LAU_REJECT                          = 0x11,       /* LAU���� */
    NAS_ERR_LOG_CSFB_MT_STATE_LAU_ACCEPT                          = 0x12,       /* LAU�ɹ� */
    NAS_ERR_LOG_CSFB_MT_STATE_RRC_REL_IND_RECEIVED                = 0x13,       /* ���յ�RRC�����ͷ�ָʾ */
    NAS_ERR_LOG_CSFB_MT_STATE_CSFB_MT_PROTECT_TI_TIME_OUT         = 0x14,       /* CSFB MT������ʱ����ʱ */
    NAS_ERR_LOG_CSFB_MT_STATE_CSFB_MT_TI_TIME_OUT_RCV_REL_IND     = 0x15,       /* CSFB MT������ʱ����ʱ״̬���յ������REL IND */
    NAS_ERR_LOG_CSFB_MT_STATE_CM_SERVICE_MSG_RECEIVED             = 0x16,       /* CSFB MTҵ����Ϣ�Ѿ����� */

    NAS_ERR_LOG_CSFB_MT_CALL_CAUSE_BUTT
};
typedef VOS_UINT32  NAS_ERR_LOG_CSFB_MT_STATE_ENUM_U32;

/* Added by zwx247453 for CHR optimize, 2015-03-10 begin */

enum NAS_ERR_LOG_MO_DETACH_TYPE_ENUM
{
    NAS_ERR_LOG_MO_DETACH_NULL          = 0,
    NAS_ERR_LOG_MO_DETACH_PS            = 1,
    NAS_ERR_LOG_MO_DETACH_CS            = 2,
    NAS_ERR_LOG_MO_DETACH_CS_PS         = 3,

    NAS_ERR_LOG_MO_DETACH_BUTT
};

typedef VOS_UINT32 NAS_ERR_LOG_MO_DETACH_TYPE_ENUM_UINT32;
/* Added by zwx247453 for CHR optimize, 2015-03-10 begin */

enum NAS_INFO_CLT_BAND_IND_ENUM
{
    NAS_INFO_CLT_BAND_IND_NONE               = 0,
    NAS_INFO_CLT_BAND_IND_1                  = 1,
    NAS_INFO_CLT_BAND_IND_2                  = 2,
    NAS_INFO_CLT_BAND_IND_3                  = 3,
    NAS_INFO_CLT_BAND_IND_4                  = 4,
    NAS_INFO_CLT_BAND_IND_5                  = 5,
    NAS_INFO_CLT_BAND_IND_6                  = 6,
    NAS_INFO_CLT_BAND_IND_7                  = 7,
    NAS_INFO_CLT_BAND_IND_8                  = 8,
    NAS_INFO_CLT_BAND_IND_9                  = 9,
    NAS_INFO_CLT_BAND_IND_10                 = 10,
    NAS_INFO_CLT_BAND_IND_11                 = 11,
    NAS_INFO_CLT_BAND_IND_12                 = 12,
    NAS_INFO_CLT_BAND_IND_13                 = 13,
    NAS_INFO_CLT_BAND_IND_14                 = 14,
    NAS_INFO_CLT_BAND_IND_15                 = 15,
    NAS_INFO_CLT_BAND_IND_16                 = 16,
    NAS_INFO_CLT_BAND_IND_17                 = 17,
    NAS_INFO_CLT_BAND_IND_18                 = 18,
    NAS_INFO_CLT_BAND_IND_19                 = 19,
    NAS_INFO_CLT_BAND_IND_20                 = 20,
    NAS_INFO_CLT_BAND_IND_21                 = 21,
    NAS_INFO_CLT_BAND_IND_22                 = 22,
    NAS_INFO_CLT_BAND_IND_23                 = 23,
    NAS_INFO_CLT_BAND_IND_24                 = 24,
    NAS_INFO_CLT_BAND_IND_25                 = 25,
    NAS_INFO_CLT_BAND_IND_26                 = 26,
    NAS_INFO_CLT_BAND_IND_27                 = 27,
    NAS_INFO_CLT_BAND_IND_28                 = 28,
    NAS_INFO_CLT_BAND_IND_29                 = 29,
    NAS_INFO_CLT_BAND_IND_30                 = 30,
    NAS_INFO_CLT_BAND_IND_31                 = 31,
    NAS_INFO_CLT_BAND_IND_32                 = 32,
    NAS_INFO_CLT_BAND_IND_33                 = 33,
    NAS_INFO_CLT_BAND_IND_34                 = 34,
    NAS_INFO_CLT_BAND_IND_35                 = 35,
    NAS_INFO_CLT_BAND_IND_36                 = 36,
    NAS_INFO_CLT_BAND_IND_37                 = 37,
    NAS_INFO_CLT_BAND_IND_38                 = 38,
    NAS_INFO_CLT_BAND_IND_39                 = 39,
    NAS_INFO_CLT_BAND_IND_40                 = 40,
    NAS_INFO_CLT_BAND_IND_41                 = 41,
    NAS_INFO_CLT_BAND_IND_42                 = 42,
    NAS_INFO_CLT_BAND_IND_43                 = 43,
    NAS_INFO_CLT_BAND_IND_44                 = 44,

    NAS_INFO_CLT_BAND_IND_BUTT
};
typedef VOS_UINT16 NAS_INFO_CLT_BAND_IND_ENUM_UINT16;


enum NAS_INFO_CLT_INFO_ID_ENUM
{
    NAS_INFO_CLT_SCENE_TYPE_FREQ_BAND            = 1,                           /* �ƶ�Ƶ��Ƶ����Ϣ�ռ� */
    NAS_INFO_CLT_SCENE_TYPE_BUTT
};
typedef VOS_UINT32 NAS_INFO_CLT_SCENE_TYPE_ENUM_UINT32;
enum NAS_INFO_CLT_RATMODE_ENUM
{
    NAS_INFO_CLT_RATMODE_GSM                         = 0,
    NAS_INFO_CLT_RATMODE_WCDMA                       = 1,
    NAS_INFO_CLT_RATMODE_LTE                         = 2,
    NAS_INFO_CLT_RATMODE_TDS                         = 3,
    NAS_INFO_CLT_RATMODE_BUTT
};
typedef VOS_UINT8 NAS_INFO_CLT_RATMODE_ENUM_UINT8;


enum NAS_ERR_LOG_OOS_EVENT_ENUM
{
    NAS_ERR_LOG_OOS_EVENT_NULL                                           = 0x0,

    /* �û����õĳ��� */
    NAS_ERR_LOG_OOS_EVENT_USER_SYSCFG                                    = 0x001,
    NAS_ERR_LOG_OOS_EVENT_USER_LIST_SRCH                                 = 0x002,
    NAS_ERR_LOG_OOS_EVENT_USER_SPEC_SRCH                                 = 0x003,
    NAS_ERR_LOG_OOS_EVENT_USER_AUTO_SEL                                  = 0x004,
    NAS_ERR_LOG_OOS_EVENT_USER_POWER_ON                                  = 0x005,
    NAS_ERR_LOG_OOS_EVENT_USER_POWER_OFF                                 = 0x006,
    NAS_ERR_LOG_OOS_EVENT_USER_DETACH                                    = 0x007,

    /* ����㶪���ĳ��� */
    NAS_ERR_LOG_OOS_EVENT_GAS_NORMAL_AREA_LOST                           = 0x100,
    NAS_ERR_LOG_OOS_EVENT_WAS_NORMAL_AREA_LOST                           = 0x101,
    NAS_ERR_LOG_OOS_EVENT_TAS_NORMAL_AREA_LOST                           = 0x102,
    NAS_ERR_LOG_OOS_EVENT_LMM_NORMAL_AREA_LOST                           = 0x103,
    NAS_ERR_LOG_OOS_EVENT_GAS_NO_RF_AREA_LOST                            = 0x104,
    NAS_ERR_LOG_OOS_EVENT_WAS_NO_RF_AREA_LOST                            = 0x105,
    NAS_ERR_LOG_OOS_EVENT_TAS_NO_RF_AREA_LOST                            = 0x106,
    NAS_ERR_LOG_OOS_EVENT_LMM_NO_RF_AREA_LOST                            = 0x107,
    NAS_ERR_LOG_OOS_EVENT_WAS_LIMITED_CAMP                               = 0x108,
    NAS_ERR_LOG_OOS_EVENT_TAS_LIMITED_CAMP                               = 0x109,
    NAS_ERR_LOG_OOS_EVENT_GAS_AC_INFO_CHANGE                             = 0x10a,
    NAS_ERR_LOG_OOS_EVENT_WAS_AC_INFO_CHANGE                             = 0x10b,
    NAS_ERR_LOG_OOS_EVENT_TAS_AC_INFO_CHANGE                             = 0x10c,

    /* ע��ʧ�ܵĳ��� */
    NAS_ERR_LOG_OOS_EVENT_CS_REG_FAIL                                    = 0x200,
    NAS_ERR_LOG_OOS_EVENT_PS_REG_FAIL                                    = 0x201,
    NAS_ERR_LOG_OOS_EVENT_COMBINE_EPS_REG_ATTACH_FAIL                    = 0x202,
    NAS_ERR_LOG_OOS_EVENT_COMBINE_EPS_REG_TAU_FAIL                       = 0x203,
    NAS_ERR_LOG_OOS_EVENT_COMBINE_EPS_REG_ONLY_EPS_SUCC                  = 0x204,
    NAS_ERR_LOG_OOS_EVENT_EPS_REG_ATTACH_FAIL                            = 0x205,
    NAS_ERR_LOG_OOS_EVENT_EPS_REG_TAU_FAIL                               = 0x206,
    NAS_ERR_LOG_OOS_EVENT_EPS_ONLY_ATTACH                                = 0x207,
    NAS_ERR_LOG_OOS_EVENT_ATTACH_AUTH_REJ                                = 0x208,

    /* ���񱻾ܵĳ��� */
    NAS_ERR_LOG_OOS_EVENT_CM_SERVICE_REJ                                 = 0x300,
    NAS_ERR_LOG_OOS_EVENT_PS_SERVICE_REG_FAIL                            = 0x301,
    NAS_ERR_LOG_OOS_EVENT_EPS_SERVICE_REQ_FAIL                           = 0x302,

    /* ����DETACH�ĳ��� */
    NAS_ERR_LOG_OOS_EVENT_GUT_NW_DETACH                                  = 0x400,
    NAS_ERR_LOG_OOS_EVENT_LTE_NW_DETACH                                  = 0x401,

    /* ����DETACH�ĳ��� */
    NAS_ERR_LOG_OOS_EVENT_GUT_LOCAL_DETACH                               = 0x500,
    NAS_ERR_LOG_OOS_EVENT_LTE_LOCAL_DETACH                               = 0x501,

    /* �ڲ������ĳ��� */
    NAS_ERR_LOG_OOS_EVENT_INTER_SYS_CELL_RESEL_TI_WAIT_GAS_SYS_INFO_EXP  = 0x600,
    NAS_ERR_LOG_OOS_EVENT_INTER_SYS_CELL_RESEL_TI_WAIT_WAS_SYS_INFO_EXP  = 0x601,
    NAS_ERR_LOG_OOS_EVENT_INTER_SYS_CELL_RESEL_TI_WAIT_TAS_SYS_INFO_EXP  = 0x602,
    NAS_ERR_LOG_OOS_EVENT_INTER_SYS_CELL_RESEL_TI_WAIT_LNAS_SYS_INFO_EXP = 0x603,
    NAS_ERR_LOG_OOS_EVENT_INTER_SYS_CCO_TI_WAIT_GAS_SYS_INFO_EXP         = 0x604,
    NAS_ERR_LOG_OOS_EVENT_INTER_SYS_CCO_TI_WAIT_WAS_SYS_INFO_EXP         = 0x605,
    NAS_ERR_LOG_OOS_EVENT_INTER_SYS_CCO_TI_WAIT_TAS_SYS_INFO_EXP         = 0x606,
    NAS_ERR_LOG_OOS_EVENT_INTER_SYS_CCO_TI_WAIT_LNAS_SYS_INFO_EXP        = 0x607,
    NAS_ERR_LOG_OOS_EVENT_HIGH_PRO_PLMN_SRCH_BACK_TO_ORIG_PLMN_FAIL      = 0x608,
    NAS_ERR_LOG_OOS_EVENT_DISABLE_LTE                                    = 0x609,
    NAS_ERR_LOG_OOS_EVENT_ENABLE_LTE                                     = 0x60a,

    NAS_ERR_LOG_OOS_EVENT_BUTT                                           = 0xFFFFFFFF
};
typedef VOS_UINT32 NAS_ERR_LOG_OOS_EVENT_ENUM_UINT32;
enum NAS_ERR_LOG_OOS_CAUSE_ENUM
{
    NAS_ERR_LOG_OOS_CAUSE_NULL                                           = 0x0,

    /* �û����õĳ��� */
    NAS_ERR_LOG_OOS_CAUSE_USER_SYSCFG                                    = 0x001,
    NAS_ERR_LOG_OOS_CAUSE_USER_LIST_SRCH                                 = 0x002,
    NAS_ERR_LOG_OOS_CAUSE_USER_SPEC_SRCH                                 = 0x003,
    NAS_ERR_LOG_OOS_CAUSE_USER_AUTO_SEL                                  = 0x004,
    NAS_ERR_LOG_OOS_CAUSE_USER_POWER_ON                                  = 0x005,
    NAS_ERR_LOG_OOS_CAUSE_USER_POWER_OFF                                 = 0x006,
    NAS_ERR_LOG_OOS_CAUSE_USER_DETACH                                    = 0x007,

    /* ����㶪���ĳ��� */
    NAS_ERR_LOG_OOS_CAUSE_GAS_NORMAL_AREA_LOST                           = 0x100,
    NAS_ERR_LOG_OOS_CAUSE_WAS_NORMAL_AREA_LOST                           = 0x101,
    NAS_ERR_LOG_OOS_CAUSE_TAS_NORMAL_AREA_LOST                           = 0x102,
    NAS_ERR_LOG_OOS_CAUSE_LMM_NORMAL_AREA_LOST                           = 0x103,
    NAS_ERR_LOG_OOS_CAUSE_GAS_NO_RF_AREA_LOST                            = 0x104,
    NAS_ERR_LOG_OOS_CAUSE_WAS_NO_RF_AREA_LOST                            = 0x105,
    NAS_ERR_LOG_OOS_CAUSE_TAS_NO_RF_AREA_LOST                            = 0x106,
    NAS_ERR_LOG_OOS_CAUSE_LMM_NO_RF_AREA_LOST                            = 0x107,
    NAS_ERR_LOG_OOS_CAUSE_WAS_LIMITED_CAMP                               = 0x108,
    NAS_ERR_LOG_OOS_CAUSE_TAS_LIMITED_CAMP                               = 0x109,
    NAS_ERR_LOG_OOS_CAUSE_GAS_AC_INFO_CHANGE                             = 0x10a,
    NAS_ERR_LOG_OOS_CAUSE_WAS_AC_INFO_CHANGE                             = 0x10b,
    NAS_ERR_LOG_OOS_CAUSE_TAS_AC_INFO_CHANGE                             = 0x10c,

    /* ע��ʧ�ܵĳ��� */
    NAS_ERR_LOG_OOS_CAUSE_CS_REG_FAIL                                    = 0x200,
    NAS_ERR_LOG_OOS_CAUSE_PS_REG_FAIL                                    = 0x201,
    NAS_ERR_LOG_OOS_CAUSE_COMBINE_EPS_REG_ATTACH_FAIL                    = 0x202,
    NAS_ERR_LOG_OOS_CAUSE_COMBINE_EPS_REG_TAU_FAIL                       = 0x203,
    NAS_ERR_LOG_OOS_CAUSE_COMBINE_EPS_REG_ONLY_EPS_SUCC                  = 0x204,
    NAS_ERR_LOG_OOS_CAUSE_EPS_REG_ATTACH_FAIL                            = 0x205,
    NAS_ERR_LOG_OOS_CAUSE_EPS_REG_TAU_FAIL                               = 0x206,
    NAS_ERR_LOG_OOS_CAUSE_EPS_ONLY_ATTACH                                = 0x207,
    NAS_ERR_LOG_OOS_CAUSE_ATTACH_AUTH_REJ                                = 0x208,

    /* ���񱻾ܵĳ��� */
    NAS_ERR_LOG_OOS_CAUSE_CM_SERVICE_REJ                                 = 0x300,
    NAS_ERR_LOG_OOS_CAUSE_PS_SERVICE_REG_FAIL                            = 0x301,
    NAS_ERR_LOG_OOS_CAUSE_EPS_SERVICE_REQ_FAIL                           = 0x302,

    /* ����DETACH�ĳ��� */
    NAS_ERR_LOG_OOS_CAUSE_GUT_NW_DETACH                                  = 0x400,
    NAS_ERR_LOG_OOS_CAUSE_LTE_NW_DETACH                                  = 0x401,

    /* ����DETACH�ĳ��� */
    NAS_ERR_LOG_OOS_CAUSE_GUT_LOCAL_DETACH                               = 0x500,
    NAS_ERR_LOG_OOS_CAUSE_LTE_LOCAL_DETACH                               = 0x501,

    /* �ڲ������ĳ��� */
    NAS_ERR_LOG_OOS_CAUSE_INTER_SYS_CELL_RESEL_TI_WAIT_GAS_SYS_INFO_EXP  = 0x600,
    NAS_ERR_LOG_OOS_CAUSE_INTER_SYS_CELL_RESEL_TI_WAIT_WAS_SYS_INFO_EXP  = 0x601,
    NAS_ERR_LOG_OOS_CAUSE_INTER_SYS_CELL_RESEL_TI_WAIT_TAS_SYS_INFO_EXP  = 0x602,
    NAS_ERR_LOG_OOS_CAUSE_INTER_SYS_CELL_RESEL_TI_WAIT_LNAS_SYS_INFO_EXP = 0x603,
    NAS_ERR_LOG_OOS_CAUSE_INTER_SYS_CCO_TI_WAIT_GAS_SYS_INFO_EXP         = 0x604,
    NAS_ERR_LOG_OOS_CAUSE_INTER_SYS_CCO_TI_WAIT_WAS_SYS_INFO_EXP         = 0x605,
    NAS_ERR_LOG_OOS_CAUSE_INTER_SYS_CCO_TI_WAIT_TAS_SYS_INFO_EXP         = 0x606,
    NAS_ERR_LOG_OOS_CAUSE_INTER_SYS_CCO_TI_WAIT_LNAS_SYS_INFO_EXP        = 0x607,
    NAS_ERR_LOG_OOS_CAUSE_HIGH_PRO_PLMN_SRCH_BACK_TO_ORIG_PLMN_FAIL      = 0x608,
    NAS_ERR_LOG_OOS_CAUSE_DISABLE_LTE                                    = 0x609,
    NAS_ERR_LOG_OOS_CAUSE_ENABLE_LTE                                     = 0x60a,

    NAS_ERR_LOG_OOS_CAUSE_BUTT                                           = 0xFFFFFFFF
};
typedef VOS_UINT32 NAS_ERR_LOG_OOS_CAUSE_ENUM_UINT32;
enum NAS_ERR_LOG_RATMODE_ENUM
{
    NAS_ERR_LOG_RATMODE_GSM                         = 0,
    NAS_ERR_LOG_RATMODE_WCDMA                       = 1,
    NAS_ERR_LOG_RATMODE_LTE                         = 2,
    NAS_ERR_LOG_RATMODE_TDS                         = 3,
    NAS_ERR_LOG_RATMODE_BUTT
};
typedef VOS_UINT8 NAS_ERR_LOG_RATMODE_ENUM_UINT8;


enum NAS_ERR_LOG_OOS_DOMAIN_ENUM
{
    NAS_ERR_LOG_OOS_NO_DOMAIN                       = 0,
    NAS_ERR_LOG_OOS_DOMAIN_CS                       = 1,
    NAS_ERR_LOG_OOS_DOMAIN_PS                       = 2,
    NAS_ERR_LOG_OOS_DOMAIN_BUTT
};
typedef VOS_UINT32 NAS_ERR_LOG_OOS_DOMAIN_ENUM_UINT32;


enum NAS_ERR_LOG_PROCESS_INFO_TYPE_ENUM
{
    NAS_ERR_LOG_PROCESS_INFO_TYPE_NULL              = 0,
    NAS_ERR_LOG_PROCESS_INFO_TYPE_CS_CALL_DISC      = 1,
    NAS_ERR_LOG_PROCESS_INFO_TYPE_ID_RES            = 2,
    NAS_ERR_LOG_PROCESS_INFO_TYPE_BUTT
};
typedef VOS_UINT32 NAS_ERR_LOG_PROCESS_INFO_TYPE_ENUM_UINT32;


/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/

typedef struct
{
    MNTN_PLMN_ID_STRU                   stPlmn;
    VOS_UINT32                          ulLac;
}NAS_ERR_LOG_LAI_INFO_STRU;


typedef struct
{
    OM_ERR_LOG_HEADER_STRU              stHeader;
    VOS_UINT32                          ulLuResult;                             /* LU���            */
    NAS_ERR_LOG_NET_RAT_TYPE_ENUM_UINT8 enNetType;                              /* ��ǰע����������� */
    VOS_UINT8                           ucIsComBined;                           /* �Ƿ�Ϊ����ע��   */
    VOS_UINT16                          usRegFailCause;                         /* ����ʧ��ԭ��     */
    VOS_UINT32                          ulServiceStatus;                        /* ����״̬          */
    VOS_UINT32                          ulCsUpdateStatus;                       /* CS �����״̬     */
    VOS_UINT32                          ulLuAttemptCnt;                         /* LU Attempt counter */
    NAS_ERR_LOG_LAI_INFO_STRU           stOldLai;                               /* ��LAI              */
    NAS_MNTN_POSITION_INFO_STRU         stPositionInfo;                         /* λ����Ϣ */
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, begin */
    VOS_UINT8                           ucRegFailOriginalCause;                 /* ����ʧ��ԭ��     */
    VOS_UINT8                           aucReserved[3];                         /* ������ */
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, end */
}NAS_ERR_LOG_CS_REG_RESULT_EVENT_STRU;
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                    stHeader;
    NAS_ERR_LOG_GMM_ACTION_TYPE_ENUM_U32      enGmmActionType;                  /* GMM�Ĳ������� */
    VOS_UINT32                                ulActionResult;                   /* ע���� */
    NAS_ERR_LOG_REG_DOMAIN_ENUM_UINT32        enReqDomain;                      /* ��ǰע��Ľ����Ps Only��CS_PS */
    NAS_ERR_LOG_REG_DOMAIN_ENUM_UINT32        enRsltDomain;                     /* ��ǰע��Ľ����Ps Only��CS_PS */
    VOS_UINT16                                usRegFailCause;                   /* ����ʧ��ԭ�� */
    NAS_ERR_LOG_NET_RAT_TYPE_ENUM_UINT8       enNetType;                        /* ��ǰע����������� */
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, begin */
    VOS_UINT8                                 ucRegFailOriginalCause;                 /* �յ�����ԭʼ�ܾ�ԭ�� */
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, end */
    VOS_UINT32                                ulRegCounter;                     /* Attach attempt counter */
    VOS_UINT32                                ulServiceStatus;                  /* ����״̬ */
    NAS_MNTN_POSITION_INFO_STRU               stPositionInfo;                   /* λ����Ϣ */
}NAS_ERR_LOG_PS_REG_RESULT_EVENT_STRU;


typedef struct
{
    OM_ERR_LOG_HEADER_STRU                    stHeader;
    VOS_UINT32                                enActionResult;                   /* �����Ľ�� */
    VOS_UINT16                                enRegFailCause;                   /* ����ʧ��ԭ�� */
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, begin */
    VOS_UINT8                                 ucRegFailOriginalCause;           /* �յ�����ԭʼ�ܾ�ԭ�� */
    VOS_UINT8                                 ucReserved;
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, end */
    VOS_UINT32                                ulServiceSts;                     /* ����״̬ */
    NAS_MNTN_POSITION_INFO_STRU               stPositionInfo;                   /* λ����Ϣ */
}NAS_ERR_LOG_PS_SRV_REG_RESULT_EVENT_STRU;
typedef struct
{
    VOS_UINT8                           ucIsUser;
    VOS_UINT8                           ucDiscCallFlag;
    VOS_UINT8                           aucReserved[2];
}NAS_ERR_LOG_MN_CALL_DISC_DIR_STRU;


typedef struct
{
    OM_ERR_LOG_HEADER_STRU              stHeader;
    VOS_UINT8                           ucCallId;                               /* call id */
    NAS_ERR_LOG_CALL_STATE_ENUM_U8      enCallState;                            /* Call State */
    VOS_UINT8                           enRat;
    VOS_UINT8                           aucReserved[1];                         /* ������ */
    NAS_ERR_LOG_MN_CALL_DISC_DIR_STRU   stDiscDir;                              /* �Ҷϵ绰�ķ��� */
    VOS_UINT32                          ulCcCause;                              /* ʧ��ԭ��ֵ */
    NAS_MNTN_USIM_INFO_STRU             stUsimInfo;                             /* ����Ϣ */
    NAS_MNTN_POSITION_INFO_STRU         stPositionInfo;                         /* λ����Ϣ */
    VOS_UINT16                          usCellDlFreq;                           /*С��Ƶ��(����)*/
    VOS_UINT16                          usCellUlFreq;                           /*��ǰƵ��(����)*/
}NAS_ERR_LOG_CS_CALL_FAIL_EVENT_STRU;
typedef struct
{
    VOS_UINT32                          ulFsmId;                                /* ��ǰ״̬����ʶ */
    VOS_UINT32                          ulState;                                /* ��ǰ״̬ */
}NAS_ERR_LOG_APS_FSM_CTX_STRU;


typedef struct
{
    NAS_ERR_LOG_APS_FSM_CTX_STRU        stMainFsm;                              /* ��״̬�� */
    NAS_ERR_LOG_APS_FSM_CTX_STRU        stSubFsm;                               /* ��״̬�� */
    VOS_UINT8                           ucPdpId;                                /* ��ǰ״̬������Ӧ��PDP ID */
    VOS_UINT8                           aucReserve[3];
}NAS_ERR_LOG_APS_PDP_ENTITY_FSM_INFO_STRU;
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                      stHeader;
    NAS_MNTN_USIM_INFO_STRU                     stUsimInfo;                     /* ����Ϣ */
    NAS_MNTN_POSITION_INFO_STRU                 stPositionInfo;                 /* λ����Ϣ */
    VOS_UINT32                                  ulPsRegState;                   /* PSע��״̬ */
    NAS_ERR_LOG_APS_PDP_ENTITY_FSM_INFO_STRU    stApsFsmInfo;                   /* ״̬����Ϣ */
    VOS_UINT32                                  ulPsCause;                      /* ʧ��ԭ�� */
}NAS_ERR_LOG_PS_CALL_FAIL_EVENT_STRU;
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                                  stHeader;
    NAS_MNTN_USIM_INFO_STRU                                 stUsimInfo;
    NAS_MNTN_POSITION_INFO_STRU                             stPositionInfo;
    NAS_MNTN_SMS_MO_INFO_STRU                               stMoFail;
    NAS_MNTN_SEND_DOMAIN_STATUS_ENUM_UINT32                 enCsStatus;
    NAS_MNTN_SEND_DOMAIN_STATUS_ENUM_UINT32                 enPsStatus;
}NAS_ERR_LOG_SMS_MO_FAIL_EVENT_STRU;


typedef struct
{
    OM_ERR_LOG_HEADER_STRU                  stHeader;
    VOS_UINT32                              ulOptState;                         /* �������� */
    VOS_UINT32                              ulCasue;                            /* ������� */
    VOS_UINT32                              ulName;                             /* ��Ϣ���� */
}NAS_ERR_LOG_VC_FAILURE_EVENT_STRU;
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                  stHeader;
    NAS_MNTN_POSITION_INFO_STRU             stPositionInfo;                     /* λ����Ϣ */
    NAS_ERR_LOG_CS_PAGING_CAUSE_ENUM_U32    enCause;

    VOS_UINT8                               ucGMsIdType;                        /* TMSI or IMSI or PTMSI */
    VOS_UINT8                               ucGPagingType;                      /* CS or PS */
    VOS_UINT8                               aucReserve1[2];
    VOS_UINT32                              ulWCnDomainId;                      /* CS DOMAIN��PS DOMAIN��INVALID DOMAIN */
    VOS_UINT32                              ulWPagingType;                      /* Ѱ����Ϣ����:paging type1��paging type2 */
    VOS_UINT32                              ulWPagingCause;                     /* pagingԭ��ֵ */
    VOS_UINT32                              ulWPagingUeId;                      /* Paging Recorder Id */

    /* ������ʹ��˵��:
       1. �����ǰ����λ����������ͨ��ucCurrProcType���ж�
       2. ������н�����������ͨ��ucCurrProcType,ucLastEstType���ж�
       3. ������н�����������ͨ��ucCurrProcType,ucLastEstType,ucLastPagingCause���ж�
    */
    VOS_UINT8                               ucCurrMmState;                      /* MM�ĵ�ǰ״̬ */
    VOS_UINT8                               ucCurrProcType;                     /* MM�ĵ�ǰ�������� */
    VOS_UINT8                               ucLastEstType;                      /* ���һ�εĽ������ͣ�MO OR MT */
    VOS_UINT8                               ucLastPagingCause;                  /* ���һ�ε�pagingԭ��ֵ */
}NAS_ERR_LOG_CS_PAGING_FAIL_EVENT_STRU;


typedef struct
{
    OM_ERR_LOG_HEADER_STRU                  stHeader;
    NAS_MNTN_POSITION_INFO_STRU             stPositionInfo;                     /* λ����Ϣ */
    NAS_ERR_LOG_CS_MT_CALL_CAUSE_ENUM_U32   enCause;
}NAS_ERR_LOG_CS_MT_CALL_FAIL_EVENT_STRU;
typedef struct
{
    NAS_ERR_LOG_BASE_STATE_ENUM_U32     enBaseState;                            /* ��������״̬ */
    NAS_ERR_LOG_CSFB_MT_STATE_ENUM_U32  enCsfbMtState;                          /* CSFB MT�Ĺ���״̬ */
}NAS_ERR_LOG_BAST_STATE_TO_CSFB_MT_STATE_STRU;


typedef struct
{
    NAS_ERR_LOG_CSFB_MT_STATE_ENUM_U32  enCsfbMtState;                          /* ����״̬ */
    VOS_UINT32                          ulMmState;
    VOS_UINT32                          ulCause;                                /* ԭ��ֵ */
    VOS_UINT32                          ulTick;                                 /* ״̬��ӦTick */
}NAS_ERR_LOG_CSFB_MT_STATE_INFO_STRU;
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                  stHeader;
    NAS_ERR_LOG_CSFB_MT_STATE_ENUM_U32      enCurrState;                        /* ��ǰ�Ĺ���״̬ */
    VOS_UINT32                              ulPositionNum;
    NAS_MNTN_POSITION_INFO_STRU             stPositionInfo[NAS_ERR_LOG_MAX_POSITION_RECORD_NUM];
    VOS_UINT32                              ulStateNum;
    NAS_ERR_LOG_CSFB_MT_STATE_INFO_STRU     astStateInfo[NAS_ERR_LOG_MAX_CSFB_MT_STATE_RECORD_NUM];
}NAS_ERR_LOG_CSFB_MT_CALL_FAIL_EVENT_STRU;
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                  stHeader;
    VOS_UINT32                              ulCount;                            /* ������������� */
}NAS_ERR_LOG_MNTN_EVENT_STRU;
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                  stHeader;
    NAS_MNTN_POSITION_INFO_STRU             stPositionInfo;                     /* λ����Ϣ */
    VOS_UINT8                               ucDetachType;                       /* detach���� */
    VOS_UINT8                               ucGmmCause;                         /* detachԭ�� */
    VOS_UINT8                               ucForceToStandby;
    VOS_UINT8                               ucCurrNetRat;
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, begin */
    VOS_UINT8                               ucOriginalGmmCause ;               /* �յ�����ԭʼ�ܾ�ԭ�� */
    VOS_UINT8                               aucReserved[3];
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, end */
}NAS_ERR_LOG_NW_DETACH_IND_EVENT_STRU;


typedef struct
{
    OM_ERR_LOG_HEADER_STRU              stHeader;
    NAS_MNTN_POSITION_INFO_STRU         stPositionInfo;                         /* λ����Ϣ */
    VOS_UINT32                          ulCause;                                /* ʧ��ԭ��                                 */
    VOS_UINT32                          ulServiceStatus ;                       /* ����״̬ */
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, begin */
    VOS_UINT8                           ucOriginalCause;                        /* �յ�����ԭʼ�ܾ�ԭ�� */
    VOS_UINT8                           aucReserved[3];
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, begin */
}NAS_ERR_LOG_CM_SRV_REJ_IND_EVENT_STRU;


typedef struct
{
    OM_ERR_LOG_HEADER_STRU                  stHeader;
    NAS_MNTN_POSITION_INFO_STRU             stPositionInfo;                     /* λ����Ϣ */
    NAS_ERR_LOG_MO_DETACH_TYPE_ENUM_UINT32  enDetachType;                       /* detach���� */
}NAS_ERR_LOG_MO_DETACH_IND_EVENT_STRU;

/*****************************************************************************
 �ṹ��    : NAS_ERR_LOG_RAT_SWITCH_RECORD_STRU
 �ṹ˵��  : RAT�л�ʱ��Ҫ��¼����Ϣ
 1.��    ��   : 2015��03��13��
   ��    ��   : zwx247453
   �޸�����   : �½�
*****************************************************************************/
typedef struct
{
    NAS_MNTN_POSITION_INFO_STRU         stPositionInfo;
    VOS_UINT8                           ucRatType;
    VOS_UINT8                           aucReserved[3];
    VOS_UINT32                          ulSystemTick;
}NAS_ERR_LOG_RAT_SWITCH_RECORD_STRU;


typedef struct
{
    OM_ERR_LOG_HEADER_STRU              stHeader;
    NAS_ERR_LOG_RAT_SWITCH_RECORD_STRU  astRatSwitchInfo[NAS_ERR_LOG_MAX_RAT_SWITCH_RECORD_MUN];/* RAT�л���Ϣ */
    VOS_UINT32                          ulStatisticTime;                        /* ͳ��ʱ�� */
    VOS_UINT32                          ulSwitchNum;                            /* �л����� */
}NAS_ERR_LOG_RAT_FREQUENTLY_SWITCH_EVENT_STRU;
typedef struct
{
    NAS_ERR_LOG_OOS_EVENT_ENUM_UINT32   enOosEvent;                             /* �����¼� */
    NAS_ERR_LOG_OOS_CAUSE_ENUM_UINT32   enOosCause;                             /* ����ԭ�� */
}NAS_ERR_LOG_OOS_EVENT_TO_OOS_CAUSE_STRU;


typedef struct
{
    OM_ERR_LOG_HEADER_STRU              stHeader;
    NAS_ERR_LOG_OOS_CAUSE_ENUM_UINT32   enOosCause;                             /* ����ԭ�� */
    NAS_MNTN_POSITION_INFO_STRU         stPositionInfo;                         /* λ����Ϣ */
    MNTN_PLMN_ID_STRU                   stHomePlmnId;                           /* HPLMN */
    NAS_ERR_LOG_OOS_DOMAIN_ENUM_UINT32  enLostDomain;                           /* ������ */
    VOS_UINT16                          usGUTCellDlFreq;                        /* GUT��С��Ƶ��(����) */
    VOS_UINT16                          usGUTCellUlFreq;                        /* GUT�ĵ�ǰƵ��(����) */
    VOS_UINT32                          ulLteArfcn;                             /* LTEפ��Ƶ����Ϣ */
    VOS_UINT8                           ucRssiValue;                            /* �ź�ǿ�� */
    VOS_UINT8                           aucReserved[3];                         /* ������ */
}NAS_ERR_LOG_OOS_BEGIN_STRU;
typedef struct
{
    OM_ERR_LOG_HEADER_STRU              stHeader;
    MNTN_PLMN_ID_STRU                   stPlmnId;                               /* ������PLMN */
    NAS_ERR_LOG_RATMODE_ENUM_UINT8      enSrchRat;                              /* �����Ľ��뼼�� */
    VOS_UINT8                           ucSearchType;                           /* ���������� */
    VOS_UINT8                           ucResult;                               /* �����Ľ�� */
    VOS_UINT8                           ucCoverageType;                         /* �������� */
    VOS_UINT32                          ulPlmnSrchReqSlice;                     /* ����ʱ��� */
}NAS_ERR_LOG_OOS_NW_SRCH_PROC_INFO_STRU;


typedef struct
{
    OM_ERR_LOG_HEADER_STRU              stHeader;
    NAS_MNTN_POSITION_INFO_STRU         stPositionInfo;                         /* λ����Ϣ */
    NAS_ERR_LOG_OOS_DOMAIN_ENUM_UINT32  enResumeDomain;                         /* �ָ��� */
    VOS_UINT16                          usGUTCellDlFreq;                        /* GUT��С��Ƶ��(����) */
    VOS_UINT16                          usGUTCellUlFreq;                        /* GUT�ĵ�ǰƵ��(����) */
    VOS_UINT32                          ulLteArfcn;                             /* LTEפ��Ƶ����Ϣ */
    VOS_UINT8                           ucRssiValue;                            /* �ź�ǿ�� */
    VOS_UINT8                           ucReportFlag;                           /* �Ƿ��ϱ���������� */
    VOS_UINT8                           aucReserved[2];                         /* ������ */
}NAS_ERR_LOG_OOS_END_STRU;



typedef struct
{
    VOS_UINT8                           ucCallId;                               /* call id */
    VOS_UINT8                           aucReserved[3];                         /* ������ */
    VOS_UINT32                          ulCause;                                /* ʧ��ԭ��ֵ */
}NAS_ERR_LOG_CS_CALL_DISC_INFO_STRU;
typedef struct
{
    VOS_UINT8                           ucIDReqType;
    VOS_UINT8                           aucReserved[3];                         /* ������ */
}NAS_ERR_LOG_ID_RES_INFO_STRU;
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                      stHeader;
    NAS_ERR_LOG_PROCESS_INFO_TYPE_ENUM_UINT32   enInfoType;
    NAS_ERR_LOG_CS_CALL_DISC_INFO_STRU          stDiscInfo;
    NAS_ERR_LOG_ID_RES_INFO_STRU                stIDResInfo;
}NAS_ERR_LOG_PROCESS_INFO_IND_EVENT_STRU;


typedef struct
{
    VOS_UINT32                   ulBandScanStartSlice;   /* CSS Multi-Band SCAN�Ŀ�ʼʱ�� */
    VOS_UINT32                   ulBandScanFinishSlice;  /* CSS Multi-Band SCAN�Ľ���ʱ�� */
    VOS_UINT8                    ucResult;                /* ɨ���������û��ɨ�������������쳣ʱ��Я���Ľ��ʱʧ�� */
    VOS_UINT8                    ucGsmCoverageType;      /* ���ɨƵ������ָʾ���Ǹ������������ϱ��Ľ��ֻ�и��������ǻ�����ɨƵ��� */
    VOS_UINT8                    ucUmtsFddCoverageType;  /* ͬ��*/
    VOS_UINT8                    ucLteCoverageType;      /* ͬ��*/
}NAS_ERR_LOG_MULTI_BAND_SCAN_PROC_INFO_STRU;


typedef struct
{
    OM_ERR_LOG_HEADER_STRU                      stHeader;
    VOS_UINT32                                  ulSrchStartSlice;       /* GET_GEO����MCC�Ŀ�ʼʱ��� */
    VOS_UINT32                                  ulSrchFinishSlice;      /* GET_GEO����MCC�Ľ���ʱ��� */
    VOS_UINT8                                   ucSrchRslt;         /* Get_GEO��MCC�Ľ��: VOS_TRUE-�ҵ�MCC; VOS_FALSE -û�ҵ� */
    VOS_UINT8                                   ucFailCause;        /* Get_Geoû���ҵ�MCC��ԭ��.����ҵ�������ֵΪ0 */
    NAS_ERR_LOG_RATMODE_ENUM_UINT8              enMccRatType;       /* ������MCC�Ľ��뼼�� */
    VOS_UINT8                                   ucRsv;
    NAS_ERR_LOG_MULTI_BAND_SCAN_PROC_INFO_STRU  stMultiBandScanInfo;
}NAS_ERR_LOG_FFT_SRCH_MCC_PROC_INFO_STRU;
typedef struct
{
    OM_FTM_HEADER_STRU                      stHeader;
    VOS_UINT8                               aucTmsi[NAS_FTM_MAX_TMSI_LEN];      /* tmsi������ */
}NAS_FTM_TMSI_RPT_EVENT_STRU;
typedef struct
{
    OM_FTM_HEADER_STRU                      stHeader;
    VOS_UINT8                               aucPtmsi[NAS_FTM_MAX_PTMSI_LEN];    /* ptmsi������ */
}NAS_FTM_PTMSI_RPT_EVENT_STRU;
typedef struct
{
    NAS_ERR_LOG_ALM_ID_ENUM_U16             enAlmID;        /* �쳣ģ��ID */
    VOS_UINT16                              usLogLevel;     /* �ϱ�log�ȼ� */
}NAS_ERR_LOG_ALM_LEVEL_STRU;


typedef struct
{
    NAS_ERR_LOG_ALM_ID_ENUM_U16                             enAlmID;            /* �쳣ģ��ID */
    NAS_ERR_LOG_UNSOLI_REPORT_TYPE_ENUM_U16                 enReportType;       /* �ϱ�log�ȼ� */
}NAS_ERR_LOG_ALM_REPORT_TYPE_STRU;


typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;
    VOS_UINT8                           aucContent[4];                          /* ��Ϣ���� */
}NAS_ERR_LOG_MNTN_PUT_BUF_INFO_STRU;
typedef struct
{
    NAS_INFO_CLT_BAND_IND_ENUM_UINT16   enBandInd;                              /* Ƶ��ָʾ */
    VOS_UINT16                          usFreq;                                 /* Ƶ����Ϣ */
}NAS_INFO_CLT_GU_FREQ_INFO_STRU;


typedef struct
{
    NAS_INFO_CLT_BAND_IND_ENUM_UINT16   enBandInd;                              /* Ƶ��ָʾ */
    VOS_UINT8                           aucRsv[2];                              /* ����λ */
    VOS_UINT32                          ulFreq;                                 /* Ƶ����Ϣ */
}NAS_INFO_CLT_LTE_FREQ_INFO_STRU;
typedef struct
{
    VOS_UINT16                          usFreqNum;                              /* Ƶ����Ŀ */
    VOS_UINT8                           aucRsv[2];                              /* ����λ */
    MNTN_PLMN_ID_STRU                   stPlmnId;
    NAS_INFO_CLT_GU_FREQ_INFO_STRU      astFreqInfo[NAS_INFO_CLT_MAX_GSM_FREQ_NUM];  /* Ƶ����Ϣ�б� */
}NAS_INFO_CLT_GSM_FREQ_LIST_STRU;
typedef struct
{
    VOS_UINT16                          usFreqNum;                              /* Ƶ����Ŀ */
    VOS_UINT8                           aucRsv[2];                              /* ����λ */
    MNTN_PLMN_ID_STRU                   stPlmnId;
    NAS_INFO_CLT_GU_FREQ_INFO_STRU      astFreqInfo[NAS_INFO_CLT_MAX_WCDMA_FREQ_NUM]; /* Ƶ����Ϣ�б� */
}NAS_INFO_CLT_WCDMA_FREQ_LIST_STRU;
typedef struct
{
    VOS_UINT16                          usFreqNum;                              /* Ƶ����Ŀ */
    VOS_UINT8                           aucRsv[2];                              /* ����λ */
    MNTN_PLMN_ID_STRU                   stPlmnId;
    NAS_INFO_CLT_LTE_FREQ_INFO_STRU     astFreqInfo[NAS_INFO_CLT_MAX_LTE_FREQ_NUM];  /* Ƶ����Ϣ�б� */
}NAS_INFO_CLT_LTE_FREQ_LIST_STRU;
typedef struct
{
    OM_INFO_CLT_HEADER_STRU             stHeader;
    MNTN_PLMN_ID_STRU                   stPlmn;                                 /* ��ǰפ����PLMN */
    NAS_INFO_CLT_RATMODE_ENUM_UINT8     enNetMode;                              /* ��ǰע��Ľ���ģʽ */
    VOS_UINT8                           ucRac;                                  /* ·���� */
    VOS_UINT16                          usLac;                                  /* �����ǰפ����L��ΪTAC�� �����ǰפ����GU��ΪLAC*/
    VOS_UINT32                          ulCellId;                               /* ��ǰפ����С��ID */
    NAS_INFO_CLT_GSM_FREQ_LIST_STRU     stGFreqList;
    NAS_INFO_CLT_WCDMA_FREQ_LIST_STRU   stWFreqList;
    NAS_INFO_CLT_LTE_FREQ_LIST_STRU     stLFreqList;
}NAS_INFO_CLT_CLOUD_FREQ_BAND_EVENT_STRU;



/* Added by z00328328 for TX_POWER_CHR, 2015-10-20, begin */
/*****************************************************************************
 �ṹ��    : NAS_INFO_CLT_TX_POWER_EVENT_STRU
 �ṹ˵��  : PHY TX POWER�ɼ���Ϣ

  1.��    ��   : 2015��10��20��
    ��    ��   : ���巼 z00328328
    �޸�����   : ����, for ���з��书���ռ�
*****************************************************************************/
typedef struct
{
    OM_INFO_CLT_HEADER_STRU                 stHeader;
    VOS_INT32                               lTxPower;                           /* ���з��书��*/
    VOS_UINT32                              ulSavedSlice;                       /* ��ǰ�ϱ�ʱ�� */
} NAS_INFO_CLT_TX_POWER_EVENT_STRU;
/* Added by z00328328 for TX_POWER_CHR, 2015-10-20, end */

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/


#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of NasErrorLog.h */

