#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/* Общие определения */
//===============================================================
#define PROGRAM_NAME "PersoClient"
#define ORGANIZATION_DOMAIN "powersynt.ru"
#define ORGANIZATION_NAME "PowerSyntez"

#define MASTER_ACCESS_PASSWORD "1995"
//===============================================================

/* Определения для системы логгирования */
//===============================================================
#define LOG_FILE_MAX_NUMBER 8
#define LOG_MESSAGE_MAX_SIZE 250
//===============================================================

/* Определения менеджера */
//===============================================================
#define CLIENT_MANAGER_OPERATION_MAX_DURATION 20000
//===============================================================

/* Определения клиента */
//===============================================================
#define PERSO_SERVER_CONNECTION_WAITING_TIME 1000
#define PERSO_SERVER_DEFAULT_IP "127.0.0.1"
#define PERSO_SERVER_DEFAULT_PORT 6666
#define IP_PORT_MAX_VALUE 65535
#define IP_PORT_MIN_VALUE 0

#define DATA_BLOCK_PART_WAIT_TIME 1000
#define ONETIME_TRANSMIT_DATA_SIZE 10240
#define DATA_BLOCK_MAX_SIZE (FIRMWARE_SIZE * 2)

#define PRODUCTION_LINE_DEFAULT_LOGIN "1"
#define PRODUCTION_LINE_DEFAULT_PASSWORD "1"
//===============================================================

/* Определения программатора */
//===============================================================
#define FIRMWARE_TEMP_FILE_NAME \
  "data.bin"  // Расширение критически важно, оно обязательно должно быть .bin
#define FIRMWARE_SIZE 131072

#define JLINK_COMMAND_SCRIPT_DEFAULT_NAME "JLinkScript.jlink"

#define USER_DATA_FLASH_START_ADDRESS "0x0801F000"
#define USER_DATA_FLASH_SIZE 0x1000
#define USER_DATA_FLASH_END_ADDRESS "0x0801FFFF"

#define UCID_MEMORY_ADDRESS "0x1FFFF7C0"
#define UCID_CHAR_LENGTH 32
#define UCID_SIZE 16
//===============================================================

/* Определения для данных транспондера */
//===============================================================
#define PAN_CHAR_LENGTH 19

#define MANUFACTURER_ID_DS_CHAR_LENGTH 2
#define BATTERY_INSERTATION_DATE_DS_CHAR_LENGTH 4
#define SERIAL_NUMBER_DS_CHAR_LENGTH 16
//===============================================================

/* Определения для команд клиента */
//===============================================================
#define COMMAND_ECHO_RESPONSE_SIZE 3
#define COMMAND_LAUNCHPRODUCTIONLINE_RESPONSE_SIZE 2
#define COMMAND_SHUTDOWNPRODUCTIONLINE_RESPONSE_SIZE 2
#define COMMAND_GETNPRODUCTIONLINEDATA_RESPONSE_SIZE 7

#define COMMAND_REQUESTBOX_RESPONSE_SIZE 2
#define COMMAND_GETCURRENTBOXDATA_RESPONSE_SIZE 12
#define COMMAND_REFUNDBOX_RESPONSE_SIZE 2
#define COMMAND_COMPLETECURRENTBOX_RESPONSE_SIZE 2

#define COMMAND_RELEASETRANSPONDER_RESPONSE_SIZE 3
#define COMMAND_CONFIRMTRANSPONDERRELEASE_RESPONSE_SIZE 2
#define COMMAND_RERELEASETRANSPONDER_RESPONSE_SIZE 3
#define COMMAND_CONFIRMTRANSPONDERRERELEASE_RESPONSE_SIZE 2
#define COMMAND_ROLLBACKTRANSPONDER_RESPONSE_SIZE 2

#define COMMAND_GETCURRENTTRANSPONDERDATA_RESPONSE_SIZE 8
#define COMMAND_GETTRANSPONDERDATA_RESPONSE_SIZE 8

#define COMMAND_PRINTBOXSTICKER_RESPONSE_SIZE 2
#define COMMAND_PRINTLASTBOXSTICKER_RESPONSE_SIZE 2
#define COMMAND_PRINTPALLETSTICKER_RESPONSE_SIZE 2
#define COMMAND_PRINTLASTPALLETSTICKER_RESPONSE_SIZE 2

#define COMMAND_ECHO_NAME "echo"
#define COMMAND_LAUNCHPRODUCTIONLINE_NAME "launch_production_line"
#define COMMAND_SHUTDOWNPRODUCTIONLINE_NAME "shutdown_production_line"
#define COMMAND_GETPRODUCTIONLINEDATA_NAME "get_production_line_data"

#define COMMAND_REQUESTBOX_NAME "request_box"
#define COMMAND_GETCURRENTBOXDATA_NAME "get_current_box_data"
#define COMMAND_REFUNDCURRENTBOX_NAME "refund_current_box"
#define COMMAND_COMPLETECURRENTBOX_NAME "complete_current_box"

#define COMMAND_RELEASETRANSPONDER_NAME "release_transponder"
#define COMMAND_CONFIRMTRANSPONDERRELEASE_NAME "confirm_transponder_release"
#define COMMAND_RERELEASETRANSPONDER_NAME "rerelease_transponder"
#define COMMAND_CONFIRMTRANSPONDERRERELEASE_NAME "confirm_transponder_rerelease"
#define COMMAND_ROLLBACKTRANSPONDER_NAME "rollback_transponder"

#define COMMAND_GETCURRENTTRANSPONDERDATA_NAME "get_current_transponder_data"
#define COMMAND_GETTRANSPONDERDATA_NAME "get_transponder_data"

#define COMMAND_PRINTBOXSTICKER_NAME "print_box_sticker"
#define COMMAND_PRINTLASTBOXSTICKER_NAME "print_last_box_sticker"
#define COMMAND_PRINTPALLETSTICKER_NAME "print_pallet_sticker"
#define COMMAND_PRINTLASTPALLETSTICKER_NAME "print_last_pallet_sticker"
//===============================================================
#endif  // DEFINITIONS_H
