#include "dot_util.h"
#if defined(TARGET_XDOT_L151CC)
#include "xdot_low_power.h"
#endif

#if defined(TARGET_MTS_MDOT_F411RE)
uint32_t portA[6];
uint32_t portB[6];
uint32_t portC[6];
uint32_t portD[6];
uint32_t portH[6];
#endif

#define WAKE_PIN                WAKE

// void sleep_wake_rtc_only(bool deepsleep) {
//     // in some frequency bands we need to wait until another channel is available before transmitting again
//     // wait at least 10s between transmissions
//     uint32_t delay_s = dot->getNextTxMs() / 1000;
//     if (delay_s < 10) {
//         delay_s = 10;
//     }

//     logInfo("%ssleeping %lus", deepsleep ? "deep" : "", delay_s);
//     logInfo("application will %s after waking up", deepsleep ? "execute from beginning" : "resume");

//     // lowest current consumption in sleep mode can only be achieved by configuring IOs as analog inputs with no pull resistors
//     // the library handles all internal IOs automatically, but the external IOs are the application's responsibility
//     // certain IOs may require internal pullup or pulldown resistors because leaving them floating would cause extra current consumption
//     // for xDot: UART_*, I2C_*, SPI_*, GPIO*, WAKE
//     // for mDot: XBEE_*, USBTX, USBRX, PB_0, PB_1
//     // steps are:
//     //   * save IO configuration
//     //   * configure IOs to reduce current consumption
//     //   * sleep
//     //   * restore IO configuration
//     if (! deepsleep) {
// 	// save the GPIO state.
// 	sleep_save_io();

// 	// configure GPIOs for lowest current
// 	sleep_configure_io();
//     }

//     // go to sleep/deepsleep for delay_s seconds and wake using the RTC alarm
//     dot->sleep(delay_s, mDot::RTC_ALARM, deepsleep);

//     if (! deepsleep) {
// 	// restore the GPIO state.
// 	sleep_restore_io();
//     }
// }

// void sleep_wake_interrupt_only(bool deepsleep) {
// #if defined (TARGET_XDOT_L151CC)
//     if (deepsleep) {
//         // for xDot, WAKE pin (connected to S2 on xDot-DK) is the only pin that can wake the processor from deepsleep
//         // it is automatically configured when INTERRUPT or RTC_ALARM_OR_INTERRUPT is the wakeup source and deepsleep is true in the mDot::sleep call
//     } else {
//         // configure WAKE pin (connected to S2 on xDot-DK) as the pin that will wake the xDot from low power modes
//         //      other pins can be confgured instead: GPIO0-3 or UART_RX
//         dot->setWakePin(WAKE);
//     }

//     logInfo("%ssleeping until interrupt on %s pin", deepsleep ? "deep" : "", deepsleep ? "WAKE" : mDot::pinName2Str(WAKE_PIN).c_str());
// #else

//     if (deepsleep) {
//         // for mDot, XBEE_DIO7 pin is the only pin that can wake the processor from deepsleep
//         // it is automatically configured when INTERRUPT or RTC_ALARM_OR_INTERRUPT is the wakeup source and deepsleep is true in the mDot::sleep call
//     } else {
//         // configure XBEE_DIO7 pin as the pin that will wake the mDot from low power modes
//         //      other pins can be confgured instead: XBEE_DIO2-6, XBEE_DI8, XBEE_DIN
//         dot->setWakePin(XBEE_DIO7);
//     }

//     logInfo("%ssleeping until interrupt on %s pin", deepsleep ? "deep" : "", deepsleep ? "DIO7" : mDot::pinName2Str(WAKE_PIN).c_str());
// #endif

//     logInfo("application will %s after waking up", deepsleep ? "execute from beginning" : "resume");

//     // lowest current consumption in sleep mode can only be achieved by configuring IOs as analog inputs with no pull resistors
//     // the library handles all internal IOs automatically, but the external IOs are the application's responsibility
//     // certain IOs may require internal pullup or pulldown resistors because leaving them floating would cause extra current consumption
//     // for xDot: UART_*, I2C_*, SPI_*, GPIO*, WAKE
//     // for mDot: XBEE_*, USBTX, USBRX, PB_0, PB_1
//     // steps are:
//     //   * save IO configuration
//     //   * configure IOs to reduce current consumption
//     //   * sleep
//     //   * restore IO configuration
//     if (! deepsleep) {
// 	// save the GPIO state.
// 	sleep_save_io();

// 	// configure GPIOs for lowest current
// 	sleep_configure_io();
//     }

//     // go to sleep/deepsleep and wake on rising edge of configured wake pin (only the WAKE pin in deepsleep)
//     // since we're not waking on the RTC alarm, the interval is ignored
//     dot->sleep(0, mDot::INTERRUPT, deepsleep);

//     if (! deepsleep) {
// 	// restore the GPIO state.
// 	sleep_restore_io();
//     }
// }

// void sleep_wake_rtc_or_interrupt(bool deepsleep) {
//     // in some frequency bands we need to wait until another channel is available before transmitting again
//     // wait at least 5s between transmissions
//     uint32_t delay_s = dot->getNextTxMs() / 1000;
//     if (delay_s < 5) {
//         delay_s = 5;
//     }

// #if defined (TARGET_XDOT_L151CC)
//     if (deepsleep) {
//         // for xDot, WAKE pin (connected to S2 on xDot-DK) is the only pin that can wake the processor from deepsleep
//         // it is automatically configured when INTERRUPT or RTC_ALARM_OR_INTERRUPT is the wakeup source and deepsleep is true in the mDot::sleep call
//     } else {
//         // configure WAKE pin (connected to S2 on xDot-DK) as the pin that will wake the xDot from low power modes
//         //      other pins can be confgured instead: GPIO0-3 or UART_RX
//         dot->setWakePin(WAKE);
//     }

//     logInfo("%ssleeping %lus or until interrupt on %s pin", deepsleep ? "deep" : "", delay_s, deepsleep ? "WAKE" : mDot::pinName2Str(WAKE_PIN).c_str());
// #else
//     if (deepsleep) {
//         // for mDot, XBEE_DIO7 pin is the only pin that can wake the processor from deepsleep
//         // it is automatically configured when INTERRUPT or RTC_ALARM_OR_INTERRUPT is the wakeup source and deepsleep is true in the mDot::sleep call
//     } else {
//         // configure XBEE_DIO7 pin as the pin that will wake the mDot from low power modes
//         //      other pins can be confgured instead: XBEE_DIO2-6, XBEE_DI8, XBEE_DIN
//         dot->setWakePin(XBEE_DIO7);
//     }

//     logInfo("%ssleeping %lus or until interrupt on %s pin", deepsleep ? "deep" : "", delay_s, deepsleep ? "DIO7" : mDot::pinName2Str(WAKE_PIN).c_str());
// #endif

//     logInfo("application will %s after waking up", deepsleep ? "execute from beginning" : "resume");

//     // lowest current consumption in sleep mode can only be achieved by configuring IOs as analog inputs with no pull resistors
//     // the library handles all internal IOs automatically, but the external IOs are the application's responsibility
//     // certain IOs may require internal pullup or pulldown resistors because leaving them floating would cause extra current consumption
//     // for xDot: UART_*, I2C_*, SPI_*, GPIO*, WAKE
//     // for mDot: XBEE_*, USBTX, USBRX, PB_0, PB_1
//     // steps are:
//     //   * save IO configuration
//     //   * configure IOs to reduce current consumption
//     //   * sleep
//     //   * restore IO configuration
//     if (! deepsleep) {
// 	// save the GPIO state.
// 	sleep_save_io();

// 	// configure GPIOs for lowest current
// 	sleep_configure_io();
//     }

//     // go to sleep/deepsleep and wake using the RTC alarm after delay_s seconds or rising edge of configured wake pin (only the WAKE pin in deepsleep)
//     // whichever comes first will wake the xDot
//     dot->sleep(delay_s, mDot::RTC_ALARM_OR_INTERRUPT, deepsleep);

//     if (! deepsleep) {
// 	// restore the GPIO state.
// 	sleep_restore_io();
//     }
// }

// void sleep_save_io() {
// #if defined(TARGET_XDOT_L151CC)
// 	xdot_save_gpio_state();
// #else
// 	portA[0] = GPIOA->MODER;
// 	portA[1] = GPIOA->OTYPER;
// 	portA[2] = GPIOA->OSPEEDR;
// 	portA[3] = GPIOA->PUPDR;
// 	portA[4] = GPIOA->AFR[0];
// 	portA[5] = GPIOA->AFR[1];

// 	portB[0] = GPIOB->MODER;
// 	portB[1] = GPIOB->OTYPER;
// 	portB[2] = GPIOB->OSPEEDR;
// 	portB[3] = GPIOB->PUPDR;
// 	portB[4] = GPIOB->AFR[0];
// 	portB[5] = GPIOB->AFR[1];

// 	portC[0] = GPIOC->MODER;
// 	portC[1] = GPIOC->OTYPER;
// 	portC[2] = GPIOC->OSPEEDR;
// 	portC[3] = GPIOC->PUPDR;
// 	portC[4] = GPIOC->AFR[0];
// 	portC[5] = GPIOC->AFR[1];

// 	portD[0] = GPIOD->MODER;
// 	portD[1] = GPIOD->OTYPER;
// 	portD[2] = GPIOD->OSPEEDR;
// 	portD[3] = GPIOD->PUPDR;
// 	portD[4] = GPIOD->AFR[0];
// 	portD[5] = GPIOD->AFR[1];

// 	portH[0] = GPIOH->MODER;
// 	portH[1] = GPIOH->OTYPER;
// 	portH[2] = GPIOH->OSPEEDR;
// 	portH[3] = GPIOH->PUPDR;
// 	portH[4] = GPIOH->AFR[0];
// 	portH[5] = GPIOH->AFR[1];
// #endif
// }

// void sleep_configure_io() {
// #if defined(TARGET_XDOT_L151CC)
//     // GPIO Ports Clock Enable
//     __GPIOA_CLK_ENABLE();
//     __GPIOB_CLK_ENABLE();
//     __GPIOC_CLK_ENABLE();
//     __GPIOH_CLK_ENABLE();

//     GPIO_InitTypeDef GPIO_InitStruct;

//     // UART1_TX, UART1_RTS & UART1_CTS to analog nopull - RX could be a wakeup source
//     GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_12;
//     GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//     // I2C_SDA & I2C_SCL to analog nopull
//     GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
//     GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//     // SPI_MOSI, SPI_MISO, SPI_SCK, & SPI_NSS to analog nopull
//     GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
//     GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//     // iterate through potential wake pins - leave the configured wake pin alone if one is needed
//     if (WAKE_PIN != WAKE || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_0;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//     }
//     if (WAKE_PIN != GPIO0 || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_4;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//     }
//     if (WAKE_PIN != GPIO1 || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_5;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//     }
//     if (WAKE_PIN != GPIO2 || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_0;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//     }
//     if (WAKE_PIN != GPIO3 || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_2;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//     }
//     if (WAKE_PIN != UART1_RX || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_10;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//     }
// #else
//     /* GPIO Ports Clock Enable */
//     __GPIOA_CLK_ENABLE();
//     __GPIOB_CLK_ENABLE();
//     __GPIOC_CLK_ENABLE();

//     GPIO_InitTypeDef GPIO_InitStruct;

//     // XBEE_DOUT, XBEE_DIN, XBEE_DO8, XBEE_RSSI, USBTX, USBRX, PA_12, PA_13, PA_14 & PA_15 to analog nopull
//     GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
//                 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
//     GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//     // PB_0, PB_1, PB_3 & PB_4 to analog nopull
//     GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4;
//     GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//     // PC_9 & PC_13 to analog nopull
//     GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_13;
//     GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//     // iterate through potential wake pins - leave the configured wake pin alone if one is needed
//     // XBEE_DIN - PA3
//     // XBEE_DIO2 - PA5
//     // XBEE_DIO3 - PA4
//     // XBEE_DIO4 - PA7
//     // XBEE_DIO5 - PC1
//     // XBEE_DIO6 - PA1
//     // XBEE_DIO7 - PA0
//     // XBEE_SLEEPRQ - PA11

//     if (WAKE_PIN != XBEE_DIN || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_3;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//     }

//     if (WAKE_PIN != XBEE_DIO2 || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_5;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//     }

//     if (WAKE_PIN != XBEE_DIO3 || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_4;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//     }

//          if (WAKE_PIN != XBEE_DIO4 || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_7;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//     }

//      if (WAKE_PIN != XBEE_DIO5 || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_1;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//     }

//      if (WAKE_PIN != XBEE_DIO6 || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_1;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//     }

//      if (WAKE_PIN != XBEE_DIO7 || dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_0;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//     }

//      if (WAKE_PIN != XBEE_SLEEPRQ|| dot->getWakeMode() == mDot::RTC_ALARM) {
//         GPIO_InitStruct.Pin = GPIO_PIN_11;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//     }
// #endif
// }

// void sleep_restore_io() {
// #if defined(TARGET_XDOT_L151CC)
//     xdot_restore_gpio_state();
// #else
//     GPIOA->MODER = portA[0];
//     GPIOA->OTYPER = portA[1];
//     GPIOA->OSPEEDR = portA[2];
//     GPIOA->PUPDR = portA[3];
//     GPIOA->AFR[0] = portA[4];
//     GPIOA->AFR[1] = portA[5];

//     GPIOB->MODER = portB[0];
//     GPIOB->OTYPER = portB[1];
//     GPIOB->OSPEEDR = portB[2];
//     GPIOB->PUPDR = portB[3];
//     GPIOB->AFR[0] = portB[4];
//     GPIOB->AFR[1] = portB[5];

//     GPIOC->MODER = portC[0];
//     GPIOC->OTYPER = portC[1];
//     GPIOC->OSPEEDR = portC[2];
//     GPIOC->PUPDR = portC[3];
//     GPIOC->AFR[0] = portC[4];
//     GPIOC->AFR[1] = portC[5];

//     GPIOD->MODER = portD[0];
//     GPIOD->OTYPER = portD[1];
//     GPIOD->OSPEEDR = portD[2];
//     GPIOD->PUPDR = portD[3];
//     GPIOD->AFR[0] = portD[4];
//     GPIOD->AFR[1] = portD[5];

//     GPIOH->MODER = portH[0];
//     GPIOH->OTYPER = portH[1];
//     GPIOH->OSPEEDR = portH[2];
//     GPIOH->PUPDR = portH[3];
//     GPIOH->AFR[0] = portH[4];
//     GPIOH->AFR[1] = portH[5];
// #endif
// }
