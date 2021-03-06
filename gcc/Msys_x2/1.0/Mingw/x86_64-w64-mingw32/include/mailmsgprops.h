/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef __MAILMSGPROPS_H__
#define __MAILMSGPROPS_H__

#define IMMPID_START_LIST(name,start,guid) struct __declspec(uuid(guid)) tagIMMPID_##name##_STRUCT; typedef enum tagIMMPID_##name##_ENUM { IMMPID_##name##_BEFORE__ = (start)-1,
#define IMMPID_END_LIST(name) IMMPID_##name##_AFTER__ } IMMPID_##name##_ENUM;

IMMPID_START_LIST(MP,0x1000,"13384CF0-B3C4-11d1-AA92-00AA006BC80B")
IMMPID_MP_RECIPIENT_LIST,IMMPID_MP_CONTENT_FILE_NAME,IMMPID_MP_SENDER_ADDRESS_SMTP,IMMPID_MP_SENDER_ADDRESS_X500,IMMPID_MP_SENDER_ADDRESS_X400,IMMPID_MP_SENDER_ADDRESS_LEGACY_EX_DN,IMMPID_MP_DOMAIN_LIST,IMMPID_MP_PICKUP_FILE_NAME,IMMPID_MP_AUTHENTICATED_USER_NAME,IMMPID_MP_CONNECTION_IP_ADDRESS,IMMPID_MP_HELO_DOMAIN,IMMPID_MP_EIGHTBIT_MIME_OPTION,IMMPID_MP_CHUNKING_OPTION,IMMPID_MP_BINARYMIME_OPTION,IMMPID_MP_REMOTE_AUTHENTICATION_TYPE,IMMPID_MP_ERROR_CODE,IMMPID_MP_DSN_ENVID_VALUE,IMMPID_MP_DSN_RET_VALUE,IMMPID_MP_REMOTE_SERVER_DSN_CAPABLE,IMMPID_MP_ARRIVAL_TIME,IMMPID_MP_MESSAGE_STATUS,IMMPID_MP_EXPIRE_DELAY,IMMPID_MP_EXPIRE_NDR,IMMPID_MP_LOCAL_EXPIRE_DELAY,IMMPID_MP_LOCAL_EXPIRE_NDR,IMMPID_MP_ARRIVAL_FILETIME,IMMPID_MP_HR_CAT_STATUS,IMMPID_MP_MSG_GUID,IMMPID_MP_SUPERSEDES_MSG_GUID,IMMPID_MP_SCANNED_FOR_CRLF_DOT_CRLF,IMMPID_MP_FOUND_EMBEDDED_CRLF_DOT_CRLF,IMMPID_MP_MSG_SIZE_HINT,IMMPID_MP_RFC822_MSG_ID,IMMPID_MP_RFC822_MSG_SUBJECT,IMMPID_MP_RFC822_FROM_ADDRESS,IMMPID_MP_RFC822_TO_ADDRESS,IMMPID_MP_RFC822_CC_ADDRESS,IMMPID_MP_RFC822_BCC_ADDRESS,IMMPID_MP_CONNECTION_SERVER_IP_ADDRESS,IMMPID_MP_SERVER_NAME,IMMPID_MP_SERVER_VERSION,IMMPID_MP_NUM_RECIPIENTS,IMMPID_MP_X_PRIORITY,IMMPID_MP_FROM_ADDRESS,IMMPID_MP_SENDER_ADDRESS,IMMPID_MP_DEFERRED_DELIVERY_FILETIME,IMMPID_MP_SENDER_ADDRESS_OTHER,IMMPID_MP_ORIGINAL_ARRIVAL_TIME,IMMPID_MP_MSGCLASS,IMMPID_MP_CONTENT_TYPE,IMMPID_MP_ENCRYPTION_TYPE,IMMPID_MP_CONNECTION_SERVER_PORT,IMMPID_MP_CLIENT_AUTH_USER,IMMPID_MP_CLIENT_AUTH_TYPE,IMMPID_MP_CRC_GLOBAL,IMMPID_MP_CRC_RECIPS,IMMPID_MP_INBOUND_MAIL_FROM_AUTH,IMMPID_END_LIST(MP)

#define MP_MSGCLASS_SYSTEM 1
#define MP_MSGCLASS_REPLICATION 2
#define MP_MSGCLASS_DELIVERY_REPORT 3
#define MP_MSGCLASS_NONDELIVERY_REPORT 4

#define MP_STATUS_SUCCESS 0
#define MP_STATUS_RETRY 1
#define MP_STATUS_ABORT_DELIVERY 2
#define MP_STATUS_BAD_MAIL 3
#define MP_STATUS_SUBMITTED 4
#define MP_STATUS_CATEGORIZED 5
#define MP_STATUS_ABANDON_DELIVERY 6

#define RP_RECIP_FLAGS_RESERVED 0x0000000F

#define RP_DSN_NOTIFY_SUCCESS 0x01000000
#define RP_DSN_NOTIFY_FAILURE 0x02000000
#define RP_DSN_NOTIFY_DELAY 0x04000000
#define RP_DSN_NOTIFY_NEVER 0x08000000

#define RP_DSN_NOTIFY_MASK 0x0F000000

#define RP_HANDLED 0x00000010
#define RP_GENERAL_FAILURE 0x00000020
#define RP_DSN_HANDLED 0x00000040

#define RP_DELIVERED 0x00000110
#define RP_DSN_SENT_NDR 0x00000450
#define RP_FAILED 0x00000830
#define RP_UNRESOLVED 0x00001030
#define RP_ENPANDED 0x00002010
#define RP_EXPANDED 0x00002010
#define RP_DSN_SENT_DELAYED 0x00004000
#define RP_DSN_SENT_EXPANDED 0x00008040
#define RP_DSN_SENT_RELAYED 0x00010040
#define RP_DSN_SENT_DELIVERED 0x00020040
#define RP_REMOTE_MTA_NO_DSN 0x00080000
#define RP_ERROR_CONTEXT_STORE 0x00100000
#define RP_ERROR_CONTEXT_CAT 0x00200000
#define RP_ERROR_CONTEXT_MTA 0x00400000

#define RP_VOLATILE_FLAGS_MASK 0xF0000000

#define RP_DSN_NOTIFY_INVALID 0x00000000

IMMPID_START_LIST(RP,0x2000,"79E82048-D320-11d1-9FF4-00C04FA37348")
IMMPID_RP_DSN_NOTIFY_SUCCESS,IMMPID_RP_DSN_NOTIFY_INVALID,IMMPID_RP_ADDRESS_TYPE,IMMPID_RP_ADDRESS,IMMPID_RP_ADDRESS_TYPE_SMTP,IMMPID_RP_ERROR_CODE,IMMPID_RP_ERROR_STRING,IMMPID_RP_DSN_NOTIFY_VALUE,IMMPID_RP_DSN_ORCPT_VALUE,IMMPID_RP_ADDRESS_SMTP,IMMPID_RP_ADDRESS_X400,IMMPID_RP_ADDRESS_X500,IMMPID_RP_LEGACY_EX_DN,IMMPID_RP_RECIPIENT_FLAGS,IMMPID_RP_SMTP_STATUS_STRING,IMMPID_RP_DSN_PRE_CAT_ADDRESS,IMMPID_RP_MDB_GUID,IMMPID_RP_USER_GUID,IMMPID_RP_DOMAIN,IMMPID_RP_ADDRESS_OTHER,IMMPID_RP_DISPLAY_NAME,IMMPID_END_LIST(RP)
IMMPID_START_LIST(MPV,0x3000,"CBE69706-C9BD-11d1-9FF2-00C04FA37348")
IMMPID_MPV_STORE_DRIVER_HANDLE,IMMPID_MPV_MESSAGE_CREATION_FLAGS,IMMPID_MPV_MESSAGE_OPEN_HANDLES,IMMPID_MPV_TOTAL_OPEN_HANDLES,IMMPID_MPV_TOTAL_OPEN_PROPERTY_STREAM_HANDLES,IMMPID_MPV_TOTAL_OPEN_CONTENT_HANDLES,IMMPID_END_LIST(MPV)

typedef struct tagIMMP_MPV_STORE_DRIVER_HANDLE {
  GUID guidSignature;
} IMMP_MPV_STORE_DRIVER_HANDLE;

#define MPV_INBOUND_CUTOFF_EXCEEDED 0x00000001
#define MPV_WRITE_CONTENT 0x00000002

IMMPID_START_LIST(RPV,0x4000,"79E82049-D320-11d1-9FF4-00C04FA37348")
IMMPID_RPV_DONT_DELIVER,IMMPID_RPV_NO_NAME_COLLISIONS,IMMPID_END_LIST(RPV)

#define IMMPID_RP_DONT_DELIVER IMMPID_RPV_DONT_DELIVER

#define IMMPID_RP_NO_NAME_COLLISIONS IMMPID_RPV_NO_NAME_COLLISIONS

IMMPID_START_LIST(NMP,0x6000,"7433a9aa-20e2-11d2-94d6-00c04fa379f1")
IMMPID_NMP_SECONDARY_GROUPS,IMMPID_NMP_SECONDARY_ARTNUM,IMMPID_NMP_PRIMARY_GROUP,IMMPID_NMP_PRIMARY_ARTID,IMMPID_NMP_POST_TOKEN,IMMPID_NMP_NEWSGROUP_LIST,IMMPID_NMP_HEADERS,IMMPID_NMP_NNTP_PROCESSING,IMMPID_NMP_NNTP_APPROVED_HEADER,IMMPID_END_LIST(NMP)

#define NMP_PROCESS_POST 0x00000001
#define NMP_PROCESS_CONTROL 0x00000002
#define NMP_PROCESS_MODERATOR 0x00000004

IMMPID_START_LIST(CPV,0x8000,"A2A76B2A-E52D-11d1-AA64-00C04FA35B82")
IMMPID_CP_START,IMMPID_END_LIST(CPV)

#define IMMPID_DECLARE_ENTRY(name) {&_uuidof(tagIMMPID_##name##_STRUCT),IMMPID_##name##_BEFORE__+1,IMMPID_##name##_AFTER__-1}

extern const __declspec(selectany) struct tagIMMPID_GUIDLIST_ITEM {
  const GUID *pguid;
  DWORD dwStart;
  DWORD dwLast;
} IMMPID_GUIDS[] = {IMMPID_DECLARE_ENTRY(MP),IMMPID_DECLARE_ENTRY(RP),IMMPID_DECLARE_ENTRY(MPV),IMMPID_DECLARE_ENTRY(RPV),{&GUID_NULL,0,0}};
#endif
