/*
 *
 * Copyright (c) 2014 by Michael Klemm <michael@klemm-dachau.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "config.h"
#include "core/debug.h"
#include "hardware/i2c/master/i2c_pca9685.h"
#include "protocols/ecmd/ecmd-base.h"

#define ECMD_I2C_PCA9685_ADDRESS (0u)
int16_t
parse_cmd_i2c_pca9685_set(char *cmd, char *output, uint16_t len)
{
	uint8_t led = 0;
	uint16_t value = 0;
	sscanf_P(cmd, PSTR("%hhu %hu"), &led, &value);

	uint16_t ret = i2c_pca9685_set_led(ECMD_I2C_PCA9685_ADDRESS, led, 0, value);

#ifdef ECMD_MIRROR_REQUEST
	return
			ECMD_FINAL(snprintf_P(output, len, PSTR("i2c pca9685 set 0x%02X"), ret));
#else
	return ECMD_FINAL(snprintf_P(output, len, PSTR("0x%02X"), ret));
#endif
}

int16_t
parse_cmd_i2c_pca9685_mode(char *cmd, char *output, uint16_t len)
{
	uint8_t outdrv = 0;
	uint8_t invrt = 0;
	uint16_t prescaler = 0;
	sscanf_P(cmd, PSTR("%hhu %hhu %hu"), &outdrv, &invrt, &prescaler);

	uint16_t ret = i2c_pca9685_set_mode(ECMD_I2C_PCA9685_ADDRESS, outdrv, invrt, prescaler);

#ifdef ECMD_MIRROR_REQUEST
	return
			ECMD_FINAL(snprintf_P(output, len, PSTR("i2c pca9685 mode 0x%02X"), ret));
#else
	return ECMD_FINAL(snprintf_P(output, len, PSTR("0x%02X"), ret));
#endif
}

/*
  -- Ethersex META --

  block([[I2C]] (TWI))
  ecmd_feature(i2c_pca9685_set, "i2c pca9685 set",LED VALUE,Set LED output VALUE (0-4096) on I2C chip)
  ecmd_feature(i2c_pca9685_mode, "i2c pca9685 mode",OUTDRV INVRT PRESCALER,Set mode for OUTDRV (0/1) and INVRT (0/1) on I2C chip)
 */
