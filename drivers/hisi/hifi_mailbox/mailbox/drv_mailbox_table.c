

/*****************************************************************************
1 ͷ�ļ�����
*****************************************************************************/
#include "hifi_drv_mailbox_cfg.h"
#include "drv_mailbox_gut.h"

#if defined(_DRV_LLT_)
#include "drv_mailbox_stub.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
	��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e750*/
#undef	_MAILBOX_FILE_
#define _MAILBOX_FILE_	 "table"
/*lint -e785*/
/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/*���������Ѵ��ڵĹ����ڴ�ͨ���������ڴ漰Ӳ����Դ���ã�ȫ�ֶ���*/
MAILBOX_EXTERN struct mb_cfg g_mailbox_global_cfg_tbl[MAILBOX_GLOBAL_CHANNEL_NUM+1] = {
	/*CCPU�������˵�ͨ��*/
	MAILBOX_CHANNEL_COMPOSE(CCPU, HIFI, MSG),

	/*ACPU�������˵�ͨ��*/
	MAILBOX_CHANNEL_COMPOSE(ACPU, HIFI, MSG),

	/*HIFI�������˵�ͨ��*/
	MAILBOX_CHANNEL_COMPOSE(HIFI, CCPU, MSG),
	MAILBOX_CHANNEL_COMPOSE(HIFI, ACPU, MSG),

	/*������־*/
	{MAILBOX_MAILCODE_INVALID,	0,	0, 0}

};
/*lint +e785*/
/*ƽ̨����ͨ��������ڴ�ؿռ�*/
MAILBOX_EXTERN struct mb_buff	  g_mailbox_channel_handle_pool[MAILBOX_CHANNEL_NUM];

/*ƽ̨�����û��ص�������ڴ�ؿռ�*/
MAILBOX_EXTERN struct mb_cb    g_mailbox_user_cb_pool[MAILBOX_USER_NUM];

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
