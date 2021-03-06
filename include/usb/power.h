/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2013 Toshiaki Yoshida <yoshida@mpc.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Usage Tables for HID Power Devices
 *
 * Release 1.0
 * November 1, 1997
 */

/* Power Device Usages */
#define USB_HID_POWER_DEVICE_PAGE		0x84
#define USB_HID_BATTERY_SYSTEM_PAGE		0x85

/* Power Device Page */
#define USB_HID_I_NAME				0x01
#define USB_HID_PRESENT_STATUS			0x02
#define USB_HID_CHANGED_STATUS			0x03
#define USB_HID_UPS				0x04
#define USB_HID_POWER_SUPPLY			0x05
#define USB_HID_BATTERY_SYSTEM			0x10
#define USB_HID_BATTERY_SYSTEM_ID		0x11
#define USB_HID_BATTERY				0x12
#define USB_HID_BATTERY_ID			0x13
#define USB_HID_CHARGER				0x14
#define USB_HID_CHARGER_ID			0x15
#define USB_HID_POWER_CONVERTER			0x16
#define USB_HID_POWER_CONVERTER_ID		0x17
#define USB_HID_OUTLET_SYSTEM			0x18
#define USB_HID_OUTLET_SYSTEM_ID		0x19
#define USB_HID_POWER_INPUT			0x1a
#define USB_HID_INPUT_ID			0x1b
#define USB_HID_POWER_OUTPUT			0x1c
#define USB_HID_OUTPUT_ID			0x1d
#define USB_HID_FLOW				0x1e
#define USB_HID_FLOW_ID				0x1f
#define USB_HID_OUTLET				0x20
#define USB_HID_OUTLET_ID			0x21
#define USB_HID_GANG				0x22
#define USB_HID_GANG_ID				0x23
#define USB_HID_POWER_SUMMARY			0x24
#define USB_HID_POWER_SUMMARY_ID		0x25
#define USB_HID_VOLTAGE				0x30
#define USB_HID_CURRENT				0x31
#define USB_HID_FREQUENCY			0x32
#define USB_HID_APPARENT_POWER			0x33
#define USB_HID_ACTIVE_POWER			0x34
#define USB_HID_PERCENT_LOAD			0x35
#define USB_HID_TEMPERATURE			0x36
#define USB_HID_HUMIDITY			0x37
#define USB_HID_BAD_COUNT			0x38
#define USB_HID_CONFIG_VOLTAGE			0x40
#define USB_HID_CONFIG_CURRENT			0x41
#define USB_HID_CONFIG_FREQUENCY		0x42
#define USB_HID_CONFIG_APPARENT_POWER		0x43
#define USB_HID_CONFIG_ACTIVE_POWER		0x44
#define USB_HID_CONFIG_PERCENT_LOAD		0x45
#define USB_HID_CONFIG_TEMPERATURE		0x46
#define USB_HID_CONFIG_HUMIDITY			0x47
#define USB_HID_SWITCH_ON_CONTROL		0x50
#define USB_HID_SWITCH_OFF_CONTROL		0x51
#define USB_HID_TOGGLE_CONTROL			0x52
#define USB_HID_LOW_VOLTAGE_TRANSFER		0x53
#define USB_HID_HIGH_VOLTAGE_TRANSFER		0x54
#define USB_HID_DELAY_BEFORE_REBOOT		0x55
#define USB_HID_DELAY_BEFORE_STARTUP		0x56
#define USB_HID_DELAY_BEFORE_SHUTDOWN		0x57
#define USB_HID_TEST				0x58
#define USB_HID_MODULE_RESET			0x59
#define USB_HID_AUDIBLE_ALARM_CONTROL		0x5a
#define USB_HID_PRESENT				0x60
#define USB_HID_GOOD				0x61
#define USB_HID_INTERNAL_FAILURE		0x62
#define USB_HID_VOLTAGE_OUT_OF_RANGE		0x63
#define USB_HID_FREQUENCY_OUT_OF_RANGE		0x64
#define USB_HID_OVERLOAD			0x65
#define USB_HID_OVER_CHARGED			0x66
#define USB_HID_OVER_TEMPERATURE		0x67
#define USB_HID_SHUTDOWN_REQUESTED		0x68
#define USB_HID_SHUTDOWN_IMMINENT		0x69
#define USB_HID_SWITCH_ON_OFF			0x6b
#define USB_HID_SWITCHABLE			0x6c
#define USB_HID_USED				0x6d
#define USB_HID_BOOST				0x6e
#define USB_HID_BUCK				0x6f
#define USB_HID_INITIALIZED			0x70
#define USB_HID_TESTED				0x71
#define USB_HID_AWAITING_POWER			0x72
#define USB_HID_COMMUNICATION_LOST		0x73
#define USB_HID_I_MANUFACTURER			0xfd
#define USB_HID_I_PRODUCT			0xfe
#define USB_HID_I_SERIAL_NUMBER			0xff

/* Battery System Page */
#define USB_HID_SMB_BATTERY_MODE		0x01
#define USB_HID_SMB_BATTERY_STATUS		0x02
#define USB_HID_SMB_ALARM_WARNING		0x03
#define USB_HID_SMB_CHARGER_MODE		0x04
#define USB_HID_SMB_CHARGER_STATUS		0x05
#define USB_HID_SMB_CHARGER_SPEC_INFO		0x06
#define USB_HID_SMB_SELECTOR_STATE		0x07
#define USB_HID_SMB_SELECTOR_PRESETS		0x08
#define USB_HID_SMB_SELECTOR_INFO		0x09
#define USB_HID_OPTIONAL_MFG_FUNCTION1		0x10
#define USB_HID_OPTIONAL_MFG_FUNCTION2		0x11
#define USB_HID_OPTIONAL_MFG_FUNCTION3		0x12
#define USB_HID_OPTIONAL_MFG_FUNCTION4		0x13
#define USB_HID_OPTIONAL_MFG_FUNCTION5		0x14
#define USB_HID_CONNECTION_TO_SMBUS		0x15
#define USB_HID_OUTPUT_CONNECTION		0x16
#define USB_HID_CHARGER_CONNECTION		0x17
#define USB_HID_BATTERY_INSERTION		0x18
#define USB_HID_USENEXT				0x19
#define USB_HID_OK_TO_USE			0x1a
#define USB_HID_BATTERY_SUPPORTED		0x1b
#define USB_HID_SELECTOR_REVISION		0x1c
#define USB_HID_CHARGING_INDICATOR		0x1d
#define USB_HID_MANUFACTURER_ACCESS		0x28
#define USB_HID_REMAINING_CAPACITY_LIMIT	0x29
#define USB_HID_REMAINING_TIME_LIMIT		0x2a
#define USB_HID_AT_RATE				0x2b
#define USB_HID_CAPACITY_MODE			0x2c
#define USB_HID_BROADCAST_TO_CHARGER		0x2d
#define USB_HID_PRIMARY_BATTERY			0x2e
#define USB_HID_CHARGE_CONTROLLER		0x2f
#define USB_HID_TERMINATE_CHARGE		0x40
#define USB_HID_TERMINATE_DISCHARGE		0x41
#define USB_HID_BELOW_REMAINING_CAPACITY_LIMIT	0x42
#define USB_HID_REMAINING_TIME_LIMIT_EXPIRED	0x43
#define USB_HID_CHARGING			0x44
#define USB_HID_DISCHARGING			0x45
#define USB_HID_FULLY_CHARGED			0x46
#define USB_HID_FULLY_DISCHARGED		0x47
#define USB_HID_CONDITIONING_FLAG		0x48
#define USB_HID_AT_RATE_OK			0x49
#define USB_HID_SMB_ERROR_CODE			0x4a
#define USB_HID_NEED_REPLACEMENT		0x4b
#define USB_HID_AT_RATE_TIME_TO_FULL		0x60
#define USB_HID_AT_RATE_TIME_TO_EMPTY		0x61
#define USB_HID_AVERAGE_CURRENT			0x62
#define USB_HID_MAXERROR			0x63
#define USB_HID_RELATIVE_STATE_OF_CHARGE	0x64
#define USB_HID_ABSOLUTE_STATE_OF_CHARGE	0x65
#define USB_HID_REMAINING_CAPACITY		0x66
#define USB_HID_FULL_CHARGE_CAPACITY		0x67
#define USB_HID_RUN_TIME_TO_EMPTY		0x68
#define USB_HID_AVERAGE_TIME_TO_EMPTY		0x69
#define USB_HID_AVERAGE_TIME_TO_FULL		0x6a
#define USB_HID_CYCLE_COUNT			0x6b
#define USB_HID_BATT_PACK_MODEL_LEVEL		0x80
#define USB_HID_INTERNAL_CHARGE_CONTROLLER	0x81
#define USB_HID_PRIMARY_BATTERY_SUPPORT		0x82
#define USB_HID_DESIGN_CAPACITY			0x83
#define USB_HID_SPECIFICATION_INFO		0x84
#define USB_HID_MANUFACTURER_DATE		0x85
#define USB_HID_SERIAL_NUMBER			0x86
#define USB_HID_I_MANUFACTURER_NAME		0x87
#define USB_HID_I_DEVICE_NAME			0x88
#define USB_HID_I_DEVICE_CHEMISTERY		0x89
#define USB_HID_MANUFACTURER_DATA		0x8a
#define USB_HID_RECHARGABLE			0x8b
#define USB_HID_WARNING_CAPACITY_LIMIT		0x8c
#define USB_HID_CAPACITY_GRANULARITY1		0x8d
#define USB_HID_CAPACITY_GRANULARITY2		0x8e
#define USB_HID_I_OEM_INFORMATION		0x8f
#define USB_HID_INHIBIT_CHARGE			0xc0
#define USB_HID_ENABLE_POLLING			0xc1
#define USB_HID_RESET_TO_ZERO			0xc2
#define USB_HID_AC_PRESENT			0xd0
#define USB_HID_BATTERY_PRESENT			0xd1
#define USB_HID_POWER_FAIL			0xd2
#define USB_HID_ALARM_INHIBITED			0xd3
#define USB_HID_THERMISTOR_UNDER_RANGE		0xd4
#define USB_HID_THERMISTOR_HOT			0xd5
#define USB_HID_THERMISTOR_COLD			0xd6
#define USB_HID_THERMISTOR_OVER_RANGE		0xd7
#define USB_HID_CHARGER_VOLTAGE_OUT_OF_RANGE	0xd8
#define USB_HID_CURRENT_OUT_OF_RANGE		0xd9
#define USB_HID_CURRENT_NOT_REGULATED		0xda
#define USB_HID_VOLTAGE_NOT_REGULATED		0xdb
#define USB_HID_MASTER_MODE			0xdc
#define USB_HID_CHARGER_SELECTOR_SUPPORT	0xf0
#define USB_HID_CHARGER_SPEC			0xf1
#define USB_HID_LEVEL2				0xf2
#define USB_HID_LEVEL3				0xf3
