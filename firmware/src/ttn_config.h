#ifndef __TTN_CONFIG_DATA__
#define __TTN_CONFIG_DATA__

#include "mbed.h"
#include "xdot_eeprom.h"

// Replace this with your AppEUI and AppKey
static uint8_t appEUI[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xF0, 0x00, 0x6A, 0x73 };
static uint8_t appKey[] = { 0xB8, 0xB4, 0x33, 0x0D, 0xFD, 0xD5, 0xD8, 0x61, 0xE7, 0x37, 0xA6, 0xC9, 0x5E, 0x5F, 0xD3, 0xF0 };



static void get_connection_params(lorawan_connect_t* connection_params) {
    uint8_t devEUI[8] = { 0 };

    int v = xdot_eeprom_read_buf(0x401, devEUI, 8);
    if (v != 0) {
        printf("READING DEVEUI FROM EEPROM FAILED %d\n", v);
    }

    connection_params->connect_type = LORAWAN_CONNECTION_OTAA;
    connection_params->connection_u.otaa.app_eui = appEUI;
    connection_params->connection_u.otaa.dev_eui = devEUI;
    connection_params->connection_u.otaa.app_key = appKey;

    printf("Connecting over OTAA\n");
    printf("Device EUI:         ");
    for (size_t ix = 0; ix < 8; ix++) {
        printf("%02x", devEUI[ix]);
    }
    printf("\n");
    printf("Application EUI:    ");
    for (size_t ix = 0; ix < 8; ix++) {
        printf("%02x", appEUI[ix]);
    }
    printf("\n");
    printf("Application Key:    ");
    for (size_t ix = 0; ix < 16; ix++) {
        printf("%02x", appKey[ix]);
    }
    printf("\n");
}

#endif
