#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/* Общие определения */
//===============================================================
#define PROGRAM_NAME "PersoClient"
#define ORGANIZATION_DOMAIN "powersynt.ru"
#define ORGANIZATION_NAME "PowerSyntez"

#define MASTER_ACCESS_PASSWORD "1995"
//===============================================================

/* Определения менеджера */
//===============================================================
#define FIRMWARE_MANAGER_OPERATION_MAX_DURATION 10000
#define FIRMWARE_MANAGER_OPERATION_QUANT_DURATION 100
//===============================================================

/* Определения клиента */
//===============================================================
#define PERSO_SERVER_CONNECTION_WAITING_TIME 100
#define PERSO_SERVER_DEFAULT_IP "127.0.0.1"
#define PERSO_SERVER_DEFAULT_PORT 6666
#define IP_PORT_MAX_VALUE 65535
#define IP_PORT_MIN_VALUE 0

#define DATA_BLOCK_PART_WAIT_TIME 100
#define ONETIME_TRANSMIT_DATA_SIZE 10240
#define DATA_BLOCK_MAX_SIZE 250000
//===============================================================

/* Определения программатора */
//===============================================================
#define FIRMWARE_TEMP_FILE_NAME "temp.hex"
#define FIRMWARE_FILE_MAX_SIZE 131072

#define DEFAULT_FIRMWARE_FILE_PATH "../Firmware/firmware.hex"
#define DEFAULT_JLINKEXE_FILE_PATH "../JLink/JLink.exe"
#define JLINK_COMMAND_SCRIPT_DEFAULT_NAME "JLinkScript.jlink"

#define USER_DATA_FLASH_START_ADDRESS "0x0801F000"
#define USER_DATA_FLASH_SIZE 0x1000
#define USER_DATA_FLASH_END_ADDRESS "0x0801FFFF"
//===============================================================

/* Определения для данных транспондера */
//===============================================================
#define PAN_CHAR_LENGTH 19
//===============================================================

#endif // DEFINITIONS_H
