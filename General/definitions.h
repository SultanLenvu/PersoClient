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
//===============================================================

/* Определения менеджера */
//===============================================================
#define CLIENT_MANAGER_OPERATION_MAX_DURATION 10000
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
#define FIRMWARE_TEMP_FILE_NAME "data.bin"
#define FIRMWARE_SIZE 131072

#define JLINK_COMMAND_SCRIPT_DEFAULT_NAME "JLinkScript.jlink"

#define USER_DATA_FLASH_START_ADDRESS "0x0801F000"
#define USER_DATA_FLASH_SIZE 0x1000
#define USER_DATA_FLASH_END_ADDRESS "0x0801FFFF"

#define UCID_START_ADDRESS "0x1FFFF7C0"
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

#endif // DEFINITIONS_H
