

#ifndef __HMAC_ROAM_MAIN_H__
#define __HMAC_ROAM_MAIN_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_ROAM

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/


#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ROAM_MAIN_H
/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define ROAM_SCAN_TIME_MAX        (5 * 1000)       /* ɨ�賬ʱʱ�� ��λms*/
#define ROAM_CONNECT_TIME_MAX     (5 * 1000)       /* ������ʱʱ�� ��λms*/

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/* ������״̬��״̬ */
typedef enum
{
    ROAM_MAIN_STATE_INIT               = 0,
    ROAM_MAIN_STATE_FAIL               = ROAM_MAIN_STATE_INIT,
    ROAM_MAIN_STATE_SCANING            = 1,
    ROAM_MAIN_STATE_CONNECTING         = 2,
    ROAM_MAIN_STATE_UP                 = 3,

    ROAM_MAIN_STATE_BUTT
}roam_main_state_enum;
typedef oal_uint8  roam_main_state_enum_uint8;

/* ������״̬���¼����� */
typedef enum
{
    ROAM_MAIN_FSM_EVENT_START          = 0,
    ROAM_MAIN_FSM_EVENT_SCAN_RESULT    = 1,
    ROAM_MAIN_FSM_EVENT_START_CONNECT  = 2,
    ROAM_MAIN_FSM_EVENT_CONNECT_FAIL   = 3,
    ROAM_MAIN_FSM_EVENT_CONNECT_SUCC   = 4,
    ROAM_MAIN_FSM_EVENT_TIMEOUT        = 5,
    ROAM_MAIN_FSM_EVENT_TYPE_BUTT
}roam_main_fsm_event_type_enum;

#define ROAM_BAND_2G_BIT BIT0
#define ROAM_BAND_5G_BIT BIT1

/* ����ɨ���ŵ��������Բ���,�����д���*/
typedef enum
{
    ROAM_SCAN_CHANNEL_ORG_1       = 1, //2.4G channel 1\6\11
    ROAM_SCAN_CHANNEL_ORG_3       = 3, //2.4G channel 1\5\7\11
    ROAM_SCAN_CHANNEL_ORG_BUTT
}roam_channel_org_enum;
typedef oal_uint8  roam_channel_org_enum_uint8;
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
/* �������ýṹ�� */
typedef struct
{
    oal_uint8                       uc_scan_band;                /* ɨ��Ƶ�� */
    roam_channel_org_enum_uint8     uc_scan_orthogonal;          /* ɨ���ŵ��������� */
    oal_uint8                       auc_resv[2];
    oal_uint32                      ul_trigger_rssi_threshold;
    oal_uint32                      ul_blacklist_expire_sec;
    oal_uint32                      ul_buffer_max;
}hmac_roam_config_stru;

/* ����ͳ�ƽṹ�� */
typedef struct
{
    oal_uint32                 ul_trigger_rssi_cnt;
    oal_uint32                 ul_trigger_linkloss_cnt;
    oal_uint32                 ul_scan_cnt;
    oal_uint32                 ul_scan_result_cnt;
    oal_uint32                 ul_connect_cnt;
    oal_uint32                 ul_roam_old_cnt;
    oal_uint32                 ul_roam_new_cnt;
}hmac_roam_static_stru;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
oal_uint32 hmac_roam_enable(hmac_vap_stru *pst_hmac_vap, oal_uint8 uc_enable);
oal_uint32 hmac_roam_band(hmac_vap_stru *pst_hmac_vap, oal_uint8 uc_scan_band);
oal_uint32 hmac_roam_org(hmac_vap_stru *pst_hmac_vap, oal_uint8 uc_scan_orthogonal);
oal_uint32 hmac_roam_start(hmac_vap_stru *pst_hmac_vap);
oal_uint32 hmac_roam_show(hmac_vap_stru *pst_hmac_vap);
oal_uint32 hmac_roam_init(hmac_vap_stru *pst_hmac_vap);
oal_uint32 hmac_roam_exit(hmac_vap_stru *pst_hmac_vap);
oal_uint32 hmac_roam_test(hmac_vap_stru *pst_hmac_vap);
oal_uint32 hmac_roam_resume_user(hmac_vap_stru *pst_hmac_vap, oal_void *p_param);
oal_uint32 hmac_roam_pause_user(hmac_vap_stru *pst_hmac_vap, oal_void *p_param);
oal_uint32 hmac_sta_roam_rx_mgmt(hmac_vap_stru *pst_hmac_vap, oal_void *p_param);
oal_uint32 hmac_roam_trigger_handle(hmac_vap_stru *pst_hmac_vap, oal_int8 c_rssi);
oal_void hmac_roam_tbtt_handle(hmac_vap_stru *pst_hmac_vap);
oal_uint32 hmac_roam_scan_complete(hmac_vap_stru *pst_hmac_vap);
oal_void hmac_roam_connect_complete(hmac_vap_stru *pst_hmac_vap, oal_uint32 ul_reslut);
oal_void  hmac_roam_add_key_done(hmac_vap_stru *pst_hmac_vap);
#ifdef _PRE_WLAN_FEATURE_11R
oal_uint32 hmac_roam_reassoc(hmac_vap_stru *pst_hmac_vap);
oal_uint32 hmac_roam_rx_ft_action(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf);
#endif //_PRE_WLAN_FEATURE_11R
oal_void hmac_roam_timeout_test(hmac_vap_stru *pst_hmac_vap);

#endif //_PRE_WLAN_FEATURE_ROAM

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hmac_roam_main.h */