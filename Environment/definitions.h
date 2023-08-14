#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define PROGRAM_NAME "PersoClient"
#define ORGANIZATION_DOMAIN "powersynt.ru"
#define ORGANIZATION_NAME "PowerSyntez"

#define MASTER_ACCESS_PASSWORD "1995"

#define DEFAULT_FIRMWARE_FILE_PATH "../Firmware/firmware.hex"
#define DEFAULT_JLINKEXE_FILE_PATH "../JLink/JLink.exe"
#define DEFAULT_JLINK_SCRIPT_FILE_NAME "JLinkScript.jlink"

#define USER_DATA_FLASH_START_ADDRESS "0x0801F000"
#define USER_DATA_FLASH_SIZE 0x1000
#define USER_DATA_FLASH_END_ADDRESS "0x0801FFFF"

#define PERSO_SERVER_DEFAULT_IP "127.0.0.1"
#define PERSO_SERVER_DEFAULT_PORT 6666
#define IP_PORT_MAX_VALUE 65535
#define IP_PORT_MIN_VALUE 0

#define FIRMWARE_TEMP_FILE_NAME "temp.hex"

#endif // DEFINITIONS_H
