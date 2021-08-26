/******************************************************************************
 * @file     vio_IMXRT1050-EVKB.c
 * @brief    Virtual I/O implementation for board IMXRT1050-EVKB
 * @version  V1.0.1
 * @date     21. August 2021
 ******************************************************************************/
/*
 * Copyright (c) 2021 Arm Limited (or its affiliates). All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*! \page vio_IMXRT1050-EVKB Physical I/O Mapping
The table below lists the physical I/O mapping of this CMSIS-Driver VIO implementation.
Virtual Resource  | Variable       | Physical Resource on IMXRT1050-EVKB            |
:-----------------|:---------------|:-----------------------------------------------|
vioBUTTON0        | vioSignalIn.0  | WAKEUP SW8(USER_BUTTON)                        |
vioLED0           | vioSignalOut.0 | GPIO_AD_B0_09 (USER_LED)                       |
vioMotionAccelero | vioValueXYZ[1] | 3-Axis Accelerometer (FXOS8700CQ)              |
vioMotionMagneto  | vioValueXYZ[2] | 3-Axis Magnetometer (FXOS8700CQ)               |
*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "cmsis_vio.h"

#include "RTE_Components.h"
#include CMSIS_device_header

#if !defined CMSIS_VOUT || !defined CMSIS_VIN
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#if !defined VIO_SENSOR_DISABLE
#include "fsl_fxos.h"
#include "fsl_lpi2c.h"
#endif
#include "peripherals.h"
#include "pin_mux.h"
#include "board.h"
#endif

// VIO input, output definitions
#define VIO_PRINT_MAX_SIZE      64U     // maximum size of print memory
#define VIO_PRINTMEM_NUM         4U     // number of print memories
#define VIO_VALUE_NUM            3U     // number of values
#define VIO_VALUEXYZ_NUM         3U     // number of XYZ values
#define VIO_IPV4_ADDRESS_NUM     2U     // number of IPv4 addresses
#define VIO_IPV6_ADDRESS_NUM     2U     // number of IPv6 addresses

// VIO input, output variables
__USED uint32_t      vioSignalIn;                                       // Memory for incoming signal
__USED uint32_t      vioSignalOut;                                      // Memory for outgoing signal
__USED char          vioPrintMem[VIO_PRINTMEM_NUM][VIO_PRINT_MAX_SIZE]; // Memory for the last value for each level
__USED int32_t       vioValue   [VIO_VALUE_NUM];                        // Memory for value used in vioGetValue/vioSetValue
__USED vioValueXYZ_t vioValueXYZ[VIO_VALUEXYZ_NUM];                     // Memory for XYZ value for 3-D vector
__USED vioAddrIPv4_t vioAddrIPv4[VIO_IPV4_ADDRESS_NUM];                 // Memory for IPv4 address value used in vioSetIPv4/vioGetIPv4
__USED vioAddrIPv6_t vioAddrIPv6[VIO_IPV6_ADDRESS_NUM];                 // Memory for IPv6 address value used in vioSetIPv6/vioGetIPv6

#if !defined CMSIS_VOUT
// Add global user types, variables, functions here:

#endif

#if !defined CMSIS_VIN
// Global user types, variables, functions:
#if !defined VIO_SENSOR_DISABLE
  lpi2c_master_handle_t g_m_handle;
  const uint8_t g_accel_address = 0x1FU;
  fxos_handle_t g_fxosHandle = {0};
  fxos_config_t config = {0};
#endif
#endif

// Initialize test input, output.
void vioInit (void) {
#if !defined CMSIS_VOUT
// Add user variables here:

#endif
#if !defined CMSIS_VIN
// Add user variables here:

#endif

  vioSignalIn  = 0U;
  vioSignalOut = 0U;

  memset(vioPrintMem, 0, sizeof(vioPrintMem));
  memset(vioValue,    0, sizeof(vioValue));
  memset(vioValueXYZ, 0, sizeof(vioValueXYZ));
  memset(vioAddrIPv4, 0, sizeof(vioAddrIPv4));
  memset(vioAddrIPv6, 0, sizeof(vioAddrIPv6));

#if !defined CMSIS_VOUT
  // Initialize LEDs pins
  BOARD_InitUSER_LED();
#endif

#if !defined CMSIS_VIN
  // Initialize buttons pins
  BOARD_InitUSER_BUTTON();

#if !defined VIO_SENSOR_DISABLE
  BOARD_InitI2C();
  BOARD_Accel_I2C_Init();

  config.I2C_SendFunc    = BOARD_Accel_I2C_Send;
  config.I2C_ReceiveFunc = BOARD_Accel_I2C_Receive;
  config.slaveAddress    = g_accel_address;
  if (FXOS_Init(&g_fxosHandle, &config) != kStatus_Success)
    {
        while (1);
    }
#endif
#endif
}

// Print formated string to test terminal.
int32_t vioPrint (uint32_t level, const char *format, ...) {
  va_list args;
  int32_t ret;
#if !defined CMSIS_VOUT
// Add user variables here:

#endif

  if (level > vioLevelError) {
    return (-1);
  }

  if (level > VIO_PRINTMEM_NUM) {
    return (-1);
  }

  va_start(args, format);

  ret = vsnprintf((char *)vioPrintMem[level], sizeof(vioPrintMem[level]), format, args);

  va_end(args);

#if !defined CMSIS_VOUT
// Add user code here:

#endif

  return (ret);
}

// Set signal output.
void vioSetSignal (uint32_t mask, uint32_t signal) {
#if !defined CMSIS_VOUT
// Add user variables here:

#endif

  vioSignalOut &= ~mask;
  vioSignalOut |=  mask & signal;

#if !defined CMSIS_VOUT
  // Output signals to LEDs

  if (mask & vioLED0) {
    if (signal & vioLED0) {
      USER_LED_ON();
    } else {
      USER_LED_OFF();
    }
  }
#endif
}

// Get signal input.
uint32_t vioGetSignal (uint32_t mask) {
  uint32_t signal;
#if !defined CMSIS_VIN
// Add user variables here:

#endif

#if !defined CMSIS_VIN
  // Get input signals from buttons
  if (mask & vioBUTTON0) {
    if (!GPIO_PinRead (BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN)) {
      vioSignalIn |=  vioBUTTON0;
    } else {
      vioSignalIn &= ~vioBUTTON0;
    }
  }
#endif

  signal = vioSignalIn;

  return (signal & mask);
}

// Set value output.
void vioSetValue (uint32_t id, int32_t value) {
  uint32_t index = id;
#if !defined CMSIS_VOUT
// Add user variables here:

#endif

  if (index >= VIO_VALUE_NUM) {
    return;                             /* return in case of out-of-range index */
  }

  vioValue[index] = value;

#if !defined CMSIS_VOUT
// Add user code here:

#endif
}

// Get value input.
int32_t vioGetValue (uint32_t id) {
  uint32_t index = id;
  int32_t  value = 0;
#if !defined CMSIS_VIN
// Add user variables here:

#endif

  if (index >= VIO_VALUE_NUM) {
    return value;                       /* return default in case of out-of-range index */
  }

#if !defined CMSIS_VIN
// Add user code here:

//   vioValue[index] = ...;
#endif

  value = vioValue[index];

  return value;
}

// Set XYZ value output.
void vioSetXYZ (uint32_t id, vioValueXYZ_t valueXYZ) {
  uint32_t index = id;
#if !defined CMSIS_VOUT
// Add user variables here:

#endif

  if (index >= VIO_VALUEXYZ_NUM) {
    return;                             /* return in case of out-of-range index */
  }

  vioValueXYZ[index] = valueXYZ;

#if !defined CMSIS_VOUT
// Add user code here:

#endif
}

// Get XYZ value input.
vioValueXYZ_t vioGetXYZ (uint32_t id) {
  uint32_t index = id;
  vioValueXYZ_t valueXYZ = {0, 0, 0};
#if !defined CMSIS_VIN
  // Sensor variables
#if !defined VIO_SENSOR_DISABLE
  fxos_data_t fxos_data = {0};
#endif
#endif

  if (index >= VIO_VALUEXYZ_NUM) {
    return valueXYZ;                    /* return default in case of out-of-range index */
  }

#if !defined CMSIS_VIN
  // Get input xyz values from Sensor

#if !defined VIO_SENSOR_DISABLE
  if (id == vioMotionAccelero) {
    if (FXOS_ReadSensorData(&g_fxosHandle, &fxos_data) != kStatus_Success){
      while(1);
    }
    vioValueXYZ[index].X = (int32_t)((uint16_t)((uint16_t)fxos_data.accelXMSB << 8) | (uint16_t)fxos_data.accelXLSB) / 4U;
    vioValueXYZ[index].Y = (int32_t)((uint16_t)((uint16_t)fxos_data.accelYMSB << 8) | (uint16_t)fxos_data.accelYLSB) / 4U;
    vioValueXYZ[index].Z = (int32_t)((uint16_t)((uint16_t)fxos_data.accelZMSB << 8) | (uint16_t)fxos_data.accelZLSB) / 4U;
  }

  if (id == vioMotionMagneto) {
    if (FXOS_ReadSensorData(&g_fxosHandle, &fxos_data) != kStatus_Success){
      while(1);
    }
    vioValueXYZ[index].X = (int32_t)(int16_t)((uint16_t)((uint16_t)fxos_data.magXMSB << 8) | (uint16_t)fxos_data.magXLSB);
    vioValueXYZ[index].Y = (int32_t)(int16_t)((uint16_t)((uint16_t)fxos_data.magYMSB << 8) | (uint16_t)fxos_data.magYLSB);
    vioValueXYZ[index].Z = (int32_t)(int16_t)((uint16_t)((uint16_t)fxos_data.magZMSB << 8) | (uint16_t)fxos_data.magZLSB);
  }
#endif
#endif

  valueXYZ = vioValueXYZ[index];

  return valueXYZ;
}

// Set IPv4 address output.
void vioSetIPv4 (uint32_t id, vioAddrIPv4_t addrIPv4) {
  uint32_t index = id;
#if !defined CMSIS_VOUT
// Add user variables here:

#endif

  if (index >= VIO_IPV4_ADDRESS_NUM) {
    return;                             /* return in case of out-of-range index */
  }

  vioAddrIPv4[index] = addrIPv4;

#if !defined CMSIS_VOUT
// Add user code here:

#endif
}

// Get IPv4 address input.
vioAddrIPv4_t vioGetIPv4 (uint32_t id) {
  uint32_t index = id;
  vioAddrIPv4_t addrIPv4 = {0U, 0U, 0U, 0U};
#if !defined CMSIS_VIN
// Add user variables here:

#endif

  if (index >= VIO_IPV4_ADDRESS_NUM) {
    return addrIPv4;                    /* return default in case of out-of-range index */
  }

#if !defined CMSIS_VIN
// Add user code here:

//   vioAddrIPv4[index] = ...;
#endif

  addrIPv4 = vioAddrIPv4[index];

  return addrIPv4;
}

// Set IPv6 address output.
void vioSetIPv6 (uint32_t id, vioAddrIPv6_t addrIPv6) {
  uint32_t index = id;
#if !defined CMSIS_VOUT
// Add user variables here:

#endif

  if (index >= VIO_IPV6_ADDRESS_NUM) {
    return;                             /* return in case of out-of-range index */
  }

  vioAddrIPv6[index] = addrIPv6;

#if !defined CMSIS_VOUT
// Add user code here:

#endif
}

// Get IPv6 address input.
vioAddrIPv6_t vioGetIPv6 (uint32_t id) {
  uint32_t index = id;
  vioAddrIPv6_t addrIPv6 = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
                            0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U};
#if !defined CMSIS_VIN
// Add user variables here:

#endif

  if (index >= VIO_IPV6_ADDRESS_NUM) {
    return addrIPv6;                    /* return default in case of out-of-range index */
  }

#if !defined CMSIS_VIN
// Add user code here:

//   vioAddrIPv6[index] = ...;
#endif

  addrIPv6 = vioAddrIPv6[index];

  return addrIPv6;
}
