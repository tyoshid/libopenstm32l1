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
 * USB Monitor Control Class Specification
 *
 * Revision 1.0
 * January 5, 1998
 */

/* Usage Pages */
#define USB_HID_USB_MONITOR			0x80
#define USB_HID_USB_ENUMERATED_VALUES		0x81
#define USB_HID_VESA_VIRTUAL_CONTROLS		0x82

/* USB Monitor Usage Page */
#define USB_HID_MONITOR_CONTROL			0x01
#define USB_HID_EDID_INFORMATION		0x02
#define USB_HID_VDIF_INFORMATION		0x03
#define USB_HID_VESA_VERSION			0x04

/* Monitor Enumerated Values */
#define USB_HID_ENUM(n)				n

/* VESA Virtual Control Usage Page */
/* Contiguous Controls */
#define USB_HID_BRIGHTNESS			0x10
#define USB_HID_CONTRAST			0x12
#define USB_HID_RED_VIDEO_GAIN			0x16
#define USB_HID_GREEN_VIDEO_GAIN		0x18
#define USB_HID_BLUE_VIDEO_GAIN			0x1a
#define USB_HID_FOCUS				0x1c
#define USB_HID_HORIZONTAL_POSITION		0x20
#define USB_HID_HORIZONTAL_SIZE			0x22
#define USB_HID_HORIZONTAL_PINCUSHION		0x24
#define USB_HID_HORIZONTAL_PINCUSHION_BALANCE	0x26
#define USB_HID_HORIZONTAL_MISCONVERGENCE	0x28
#define USB_HID_HORIZONTAL_LINEARITY		0x2a
#define USB_HID_HORIZONTAL_LINEARITY_BALANCE	0x2c
#define USB_HID_VERTICAL_POSITION		0x30
#define USB_HID_VERTICAL_SIZE			0x32
#define USB_HID_VERTICAL_PINCUSHION		0x34
#define USB_HID_VERTICAL_PINCUSHION_BALANCE	0x36
#define USB_HID_VERTICAL_MISCONVERGENCE		0x38
#define USB_HID_VERTICAL_LINEARITY		0x3a
#define USB_HID_VERTICAL_LINEARITY_BALANCE	0x3c
#define USB_HID_PARALLELOGRAM_DISTORTION	0x40
#define USB_HID_TRAPEZOIDAL_DISTORTION		0x42
#define USB_HID_TILT				0x44
#define USB_HID_TOP_CORNER_DISTORTION_CONTRL	0x46
#define USB_HID_TOP_CORNER_DISTORTION_BALANCE	0x48
#define USB_HID_BOTTOM_CORNER_DIST_CONTROL	0x4a
#define USB_HID_BOTTOM_CORNER_DIST_BALANCE	0x4c
#define USB_HID_HORIZONTAL_MOIRE		0x56
#define USB_HID_VERTICAL_MOIRE			0x58
#define USB_HID_RED_VIDEO_BLACK_LEVEL		0x6c
#define USB_HID_GREEN_VIDEO_BLACK_LEVEL		0x6e
#define USB_HID_BLUE_VIDEO_BLACK_LEVEL		0x70
/* Non-contiguous Controls(Read/Write) */
#define USB_HID_INPUT_LEVEL_SELECT		0x5e
#define USB_HID_INPUT_SOURCE_SELECT		0x60
#define USB_HID_ON_SCREEN_DISPLAY		0xca
#define USB_HID_STEREO_MODE			0xd4
/* Non-contiguous Controls(Read-only) */
#define USB_HID_AUTO_SIZE_CENTER		0xa2
#define USB_HID_POLARITY_HORIZONTAL_SYNC	0xa4
#define USB_HID_POLARITY_VERTICAL_SYNC		0xa6
#define USB_HID_SYNCHRONIZATION_TYPE		0xa8
#define USB_HID_SCREEN_ORIENTATION		0xaa
#define USB_HID_HORIZONTAL_FREQUENCY		0xac
#define USB_HID_VERTICAL_FREQUENCY		0xae
/* Non-contiguous Controls(Write-only) */
#define USB_HID_DEGAUSS				0x01
#define USB_HID_SETTINGS			0xb0
