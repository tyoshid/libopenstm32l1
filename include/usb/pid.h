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
 * Device Class Definition for Physical Interface Devices (PID)
 *
 * 9/8/99
 * Version 1.0
 */

/* Physical Input Device Page */
#define USB_HID_PHYSICAL_INTERFACE_DEVICE	0x01
#define USB_HID_NORMAL				0x20
#define USB_HID_SET_EFFECT_REPORT		0x21
#define USB_HID_EFFECT_BLOCK_INDEX		0x22
#define USB_HID_PARAMETER_BLOCK_OFFSET		0x23
#define USB_HID_ROM_FLAG			0x24
#define USB_HID_EFFECT_TYPE			0x25
#define USB_HID_ET_CONSTANT_FORCE		0x26
#define USB_HID_ET_RAMP				0x27
#define USB_HID_ET_CUSTOM_FORCE_DATA		0x28
#define USB_HID_ET_SQUARE			0x30
#define USB_HID_ET_SINE				0x31
#define USB_HID_ET_TRIANGLE			0x32
#define USB_HID_ET_SAWTOOTH_UP			0x33
#define USB_HID_ET_SAWTOOTH_DOWN		0x34
#define USB_HID_ET_SPRING			0x40
#define USB_HID_ET_DAMPER			0x41
#define USB_HID_ET_INERTIA			0x42
#define USB_HID_ET_FRICTION			0x43
#define USB_HID_DURATION			0x50
#define USB_HID_SAMPLE_PERIOD			0x51
#define USB_HID_GAIN				0x52
#define USB_HID_TRIGGER_BUTTON			0x53
#define USB_HID_TRIGGER_REPEAT_INTERVAL		0x54
#define USB_HID_AXES_ENABLE			0x55
#define USB_HID_DIRECTION_ENABLE		0x56
#define USB_HID_DIRECTION			0x57
#define USB_HID_TYPE_SPECIFIC_BLOCK_OFFSET	0x58
#define USB_HID_BLOCK_TYPE			0x59
#define USB_HID_SET_ENVELOPE_REPORT		0x5a
#define USB_HID_ATTACK_LEVEL			0x5b
#define USB_HID_ATTACK_TIME			0x5c
#define USB_HID_FADE_LEVEL			0x5d
#define USB_HID_FADE_TIME			0x5e
#define USB_HID_SET_CONDITION_REPORT		0x5f
#define USB_HID_CP_OFFSET			0x60
#define USB_HID_POSITIVE_COEFFICIENT		0x61
#define USB_HID_NEGATIVE_COEFFICIENT		0x62
#define USB_HID_POSITIVE_SATURATION		0x63
#define USB_HID_NEGATIVE_SATURATION		0x64
#define USB_HID_DEAD_BAND			0x65
#define USB_HID_DOWNLOAD_FORCE_SAMPLE		0x66
#define USB_HID_ISOCH_CUSTOM_FORCE_ENABLE	0x67
#define USB_HID_CUSTOM_FORCE_DATA_REPORT	0x68
#define USB_HID_CUSTOM_FORCE_DATA		0x69
#define USB_HID_CUSTOM_FORCE_VENDOR_DEFINED	0x6a
#define USB_HID_SET_CUSTOM_FORCE_REPORT		0x6b
#define USB_HID_CUSTOM_FORCE_DATA_OFFSET	0x6c
#define USB_HID_SAMPLE_COUNT			0x6d
#define USB_HID_SET_PERIODIC_REPORT		0x6e
#define USB_HID_OFFSET				0x6f
#define USB_HID_MAGNITUDE			0x70
#define USB_HID_PHASE				0x71
#define USB_HID_PERIOD				0x72
#define USB_HID_SET_CONSTANT_FORCE_REPORT	0x73
#define USB_HID_SET_RAMP_FORCE_REPORT		0x74
#define USB_HID_RAMP_START			0x75
#define USB_HID_RAMP_END			0x76
#define USB_HID_EFFECT_OPERATION_REPORT		0x77
#define USB_HID_EFFECT_OPERATION		0x78
#define USB_HID_OP_EFFECT_START			0x79
#define USB_HID_OP_EFFECT_START_SOLO		0x7a
#define USB_HID_OP_EFFECT_STOP			0x7b
#define USB_HID_LOOP_COUNT			0x7c
#define USB_HID_DEVICE_GAIN_REPORT		0x7d
#define USB_HID_DEVICE_GAIN			0x7e
#define USB_HID_PID_POOL_REPORT			0x7f
#define USB_HID_RAM_POOL_SIZE			0x80
#define USB_HID_ROM_POOL_SIZE			0x81
#define USB_HID_ROM_EFFECT_BLOCK_COUNT		0x82
#define USB_HID_SIMULTANEOUS_EFFECTS_MAX	0x83
#define USB_HID_POOL_ALIGNMENT			0x84
#define USB_HID_PID_POOL_MOVE_REPORT		0x85
#define USB_HID_MOVE_SOURCE			0x86
#define USB_HID_MOVE_DESTINATION		0x87
#define USB_HID_MOVE_LENGTH			0x88
#define USB_HID_PID_BLOCK_LOAD_REPORT		0x89
#define USB_HID_BLOCK_LOAD_STATUS		0x8b
#define USB_HID_BLOCK_LOAD_SUCCESS		0x8c
#define USB_HID_BLOCK_LOAD_FULL			0x8d
#define USB_HID_BLOCK_LOAD_ERROR		0x8e
#define USB_HID_BLOCK_HANDLE			0x8f
#define USB_HID_PID_BLOCK_FREE_REPORT		0x90
#define USB_HID_TYPE_SPECIFIC_BLOCK_HANDLE	0x91
#define USB_HID_PID_STATE_REPORT		0x92
#define USB_HID_EFFECT_PLAYING			0x94
#define USB_HID_PID_DEVICE_CONTROL_REPORT	0x95
#define USB_HID_PID_DEVICE_CONTROL		0x96
#define USB_HID_DC_ENABLE_ACTUATORS		0x97
#define USB_HID_DC_DISABLE_ACTUATORS		0x98
#define USB_HID_DC_STOP_ALL_EFFECTS		0x99
#define USB_HID_DC_DEVICE_RESET			0x9a
#define USB_HID_DC_DEVICE_PAUSE			0x9b
#define USB_HID_DC_DEVICE_CONTINUE		0x9c
#define USB_HID_DEVICE_PAUSED			0x9f
#define USB_HID_ACTUATORS_ENABLED		0xa0
#define USB_HID_SAFETY_SWITCH			0xa4
#define USB_HID_ACTUATOR_OVERRIDE_SWITCH	0xa5
#define USB_HID_ACTUATOR_POWER			0xa6
#define USB_HID_START_DELAY			0xa7
#define USB_HID_PARAMETER_BLOCK_SIZE		0xa8
#define USB_HID_DEVICE_MANAGED_POOL		0xa9
#define USB_HID_SHARED_PARAMETER_BLOCKS		0xaa
#define USB_HID_CREATE_NEW_EFFECT_REPORT	0xab
#define USB_HID_RAM_POOL_AVAILABLE		0xac
