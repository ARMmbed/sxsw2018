#ifndef _TTN_CONFIG_H_
#define _TTN_CONFIG_H_

#include "mbed.h"
#include "dot_util.h"
#include "RadioEvent.h"

// Replace this with your AppEUI and AppKey
static uint8_t appEUI[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x00, 0xAA, 0xDF };
static uint8_t appKey[] = { 0xC4, 0x18, 0xF2, 0x99, 0xCC, 0x33, 0x1B, 0xDA, 0x4E, 0x0A, 0xAA, 0x81, 0xB1, 0x7F, 0x29, 0xFE };






#define CHANNEL_PLAN CP_US915






// Other configuration options
static uint8_t frequency_sub_band = 2;  // frequency sub band
static bool public_network = true;      // public or private network
static uint8_t ack = 0;                 // acknowledgements
static bool adr = false;                // adaptive data rating


// this is dirty
extern mDot* dot;

static void connect_to_ttn(RadioEvent *events) {
    lora::ChannelPlan* plan = NULL;

    mts::MTSLog::setLogLevel(mts::MTSLog::ERROR_LEVEL);

#if CHANNEL_PLAN == CP_US915
    plan = new lora::ChannelPlan_US915();
#elif CHANNEL_PLAN == CP_AU915
    plan = new lora::ChannelPlan_AU915();
#elif CHANNEL_PLAN == CP_EU868
    plan = new lora::ChannelPlan_EU868();
#elif CHANNEL_PLAN == CP_KR920
    plan = new lora::ChannelPlan_KR920();
#elif CHANNEL_PLAN == CP_AS923
    plan = new lora::ChannelPlan_AS923();
#elif CHANNEL_PLAN == CP_AS923_JAPAN
    plan = new lora::ChannelPlan_AS923_Japan();
#elif CHANNEL_PLAN == CP_IN865
    plan = new lora::ChannelPlan_IN865();
#endif
    assert(plan);

    dot = mDot::getInstance(plan);
    assert(dot);

    // attach the custom events handler
    dot->setEvents(events);

    // logInfo("mbed-os library version: %d", MBED_LIBRARY_VERSION);

    // start from a well-known state
    // logInfo("defaulting Dot configuration");
    dot->resetConfig();
    dot->resetNetworkSession();

    // update configuration if necessary
    if (dot->getJoinMode() != mDot::OTA) {
        // logInfo("changing network join mode to OTA");
        if (dot->setJoinMode(mDot::OTA) != mDot::MDOT_OK) {
            logError("failed to set network join mode to OTA");
        }
    }
    update_ota_config_id_key(appEUI, appKey, frequency_sub_band, public_network, ack);

    // enable or disable Adaptive Data Rate
    dot->setAdr(adr);

    dot->setLogLevel(mts::MTSLog::INFO_LEVEL);

    // display configuration
    display_config();

    mts::MTSLog::setLogLevel(mts::MTSLog::ERROR_LEVEL);
}


#endif // _TTN_CONFIG_H_
