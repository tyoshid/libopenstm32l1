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

#include <libopencm3.h>

/*
 * Device Class Definition for Human Interface Devices (HID)
 *
 * Firmware Specification--6/27/01
 * Version 1.11
 */

/* HID Interface Class Code */
#define USB_CLASS_HID				3

/* HID Interface Subclass Codes */
#define USB_HID_SUBCLASS_UNDEFINED		0
#define USB_HID_SUBCLASS_BOOT			1

/* HID Interface Protocol Codes */
#define USB_HID_PROTO_UNDEFINED			0
#define USB_HID_PROTO_KEYBOARD			1
#define USB_HID_PROTO_MOUSE			2

/* HID Class Descriptor Types */
#define USB_DT_HID				0x21
#define USB_DT_REPORT				0x22
#define USB_DT_PHYSICAL				0x23

/* HID Descriptor */
#define USB_HID_DESCRIPTOR(n)					\
	struct usb_hid_descriptor_##n {				\
		u8 bLength;					\
		u8 bDescriptorType;				\
		u16 bcdHID;					\
		u8 bCountryCode;				\
		u8 bNumDescriptors;				\
		struct {					\
			u8 bDescriptorType;			\
			u16 wDescriptorLength;			\
		} __attribute__ ((packed)) descriptor[n];	\
	} __attribute__ ((packed))

/* Report Descriptor */

/* Short Items */
#define USB_HID_ITEM_SIZE			(3 << 0)
#define USB_HID_ITEM_TYPE			(3 << 2)
#define USB_HID_ITEM_TAG			(15 << 4)

/* bSize */
#define USB_HID_SIZE_0				(0 << 0)
#define USB_HID_SIZE_1				(1 << 0)
#define USB_HID_SIZE_2				(2 << 0)
#define USB_HID_SIZE_4				(3 << 0)

/* bType */
#define USB_HID_MAIN				(0 << 2)
#define USB_HID_GLOBAL				(1 << 2)
#define USB_HID_LOCAL				(2 << 2)

/* Long Items */
#define USB_HID_ITEM_LONG			0xfc

/* Main Items */
#define USB_HID_INPUT				((8 << 4) | USB_HID_MAIN)
#define USB_HID_OUTPUT				((9 << 4) | USB_HID_MAIN)
#define USB_HID_FEATURE				((11 << 4) | USB_HID_MAIN)
#define USB_HID_COLLECTION			((10 << 4) | USB_HID_MAIN)
#define USB_HID_END_COLLECTION			((12 << 4) | USB_HID_MAIN)

/* Input, Output, and Feature Items */
#define USB_HID_DATA				(0 << 0)
#define USB_HID_CONSTANT			(1 << 0)
#define USB_HID_ARRAY				(0 << 1)
#define USB_HID_VARIABLE			(1 << 1)
#define USB_HID_ABSOLUTE			(0 << 2)
#define USB_HID_RELATIVE			(1 << 2)
#define USB_HID_NO_WRAP				(0 << 3)
#define USB_HID_WRAP				(1 << 3)
#define USB_HID_LINEAR				(0 << 4)
#define USB_HID_NON_LINEAR			(1 << 4)
#define USB_HID_PREFERRED_STATE			(0 << 5)
#define USB_HID_NO_PREFERRED			(1 << 5)
#define USB_HID_NO_NULL_POSITION		(0 << 6)
#define USB_HID_NULL_STATE			(1 << 6)
#define USB_HID_NON_VOLATILE			(0 << 7)
#define USB_HID_VOLATILE			(1 << 7)
#define USB_HID_BIT_FIELD			(0 << 8)
#define USB_HID_BUFFERED_BYTES			(1 << 8)

/* Collection, End Collenction Items */
#define USB_HID_PHYSICAL			0x00
#define USB_HID_APPLICATION			0x01
#define USB_HID_LOGICAL				0x02
#define USB_HID_REPORT				0x03
#define USB_HID_NAMED_ARRAY			0x04
#define USB_HID_USAGE_SWITCH			0x05
#define USB_HID_USAGE_MODIFIER			0x06
#define USB_HID_COLLECTION_VENDOR		0x80

/* Global Items */
#define USB_HID_USAGE_PAGE			((0 << 4) | USB_HID_GLOBAL)
#define USB_HID_LOGICAL_MIN			((1 << 4) | USB_HID_GLOBAL)
#define USB_HID_LOGICAL_MAX			((2 << 4) | USB_HID_GLOBAL)
#define USB_HID_PHYSICAL_MIN			((3 << 4) | USB_HID_GLOBAL)
#define USB_HID_PHYSICAL_MAX			((4 << 4) | USB_HID_GLOBAL)
#define USB_HID_UNIT_EXPONENT			((5 << 4) | USB_HID_GLOBAL)
#define USB_HID_UNIT				((6 << 4) | USB_HID_GLOBAL)
#define USB_HID_REPORT_SIZE			((7 << 4) | USB_HID_GLOBAL)
#define USB_HID_REPORT_ID			((8 << 4) | USB_HID_GLOBAL)
#define USB_HID_REPORT_COUNT			((9 << 4) | USB_HID_GLOBAL)
#define USB_HID_PUSH				((10 << 4) | USB_HID_GLOBAL)
#define USB_HID_POP				((11 << 4) | USB_HID_GLOBAL)

/* Unit */
#define USB_HID_UNIT_SYSTEM			(0xf << 0)
#define USB_HID_UNIT_LENGTH			(0xf << 4)
#define USB_HID_UNIT_MASS			(0xf << 8)
#define USB_HID_UNIT_TIME			(0xf << 12)
#define USB_HID_UNIT_TEMPERATURE		(0xf << 16)
#define USB_HID_UNIT_CURRENT			(0xf << 20)
#define USB_HID_UNIT_LUMINOUS_INTENSITY		(0xf << 24)
/* System part */
#define USB_HID_UNIT_NONE			0
#define USB_HID_UNIT_SI_LINEAR			1 /* cm, g, s, K, A, cd */
#define USB_HID_UNIT_SI_ROTATION		2 /* rad, g, s, K, A, cd */
#define USB_HID_UNIT_ENGLISH_LINEAR		3 /* in, slug, s, F, A, cd */
#define USB_HID_UNIT_ENGLISH_ROTATION		4 /* degree, slug, s, F, A,
						     cd */
#define USB_HID_UNIT_VENDOR			0xf

/* Local Items */
#define USB_HID_USAGE				((0 << 4) | USB_HID_LOCAL)
#define USB_HID_USAGE_MIN			((1 << 4) | USB_HID_LOCAL)
#define USB_HID_USAGE_MAX			((2 << 4) | USB_HID_LOCAL)
#define USB_HID_DESIGNATOR_INDEX		((3 << 4) | USB_HID_LOCAL)
#define USB_HID_DESIGNATOR_MIN			((4 << 4) | USB_HID_LOCAL)
#define USB_HID_DESIGNATOR_MAX			((5 << 4) | USB_HID_LOCAL)
#define USB_HID_STRING_INDEX			((7 << 4) | USB_HID_LOCAL)
#define USB_HID_STRING_MIN			((8 << 4) | USB_HID_LOCAL)
#define USB_HID_STRING_MAX			((9 << 4) | USB_HID_LOCAL)
#define USB_HID_DELIMITER			((10 << 4) | USB_HID_LOCAL)


/* Class-Specific Requests */
#define USB_HID_GET_REPORT			0x01
#define USB_HID_GET_IDLE			0x02
#define USB_HID_GET_PROTOCOL			0x03
#define USB_HID_SET_REPORT			0x09
#define USB_HID_SET_IDLE			0x0a
#define USB_HID_SET_PROTOCOL			0x0b

#define USB_HID_REPORT_TYPE_INPUT		1
#define USB_HID_REPORT_TYPE_OUTPUT		2
#define USB_HID_REPORT_TYPE_FEATURE		3

/* Physical Descriptors */

struct usb_hid_phsical_descriptor0 {
	u8 bNumber;
	u8 bLength;
} __attribute__ ((packed));

#define USB_HID_PHYSICAL_DESCRIPTOR(n)				\
	struct usb_hid_physical_descriptor_##n {		\
		u8 bPhysicalInfo;				\
		struct {					\
			u8 bDesignator;				\
			u8 bFlags;				\
		} __attribute__ ((packed)) dPhysical[n];	\
	} __attribute__ ((packed))

/* Physical Information */
#define USB_HID_BIAS_MASK			0xe0
#define USB_HID_PREFERENCE_MASK			0x1f

/* Bias */
#define USB_HID_BIAS_NOT_APPLICABLE		0
#define USB_HID_BIAS_RIGHT_HAND			1
#define USB_HID_BIAS_LEFT_HAND			2
#define USB_HID_BIAS_BOTH_HANDS			3
#define USB_HID_BIAS_EITHER_HAND		4

/* Designator */
#define USB_HID_DESIGNATOR_NONE			0x00
#define USB_HID_DESIGNATOR_HAND			0x01
#define USB_HID_DESIGNATOR_EYEBALL		0x02
#define USB_HID_DESIGNATOR_EYEBROW		0x03
#define USB_HID_DESIGNATOR_EYELID		0x04
#define USB_HID_DESIGNATOR_EAR			0x05
#define USB_HID_DESIGNATOR_NOSE			0x06
#define USB_HID_DESIGNATOR_MOUTH		0x07
#define USB_HID_DESIGNATOR_UPPER_LIP		0x08
#define USB_HID_DESIGNATOR_LOWER_LIP		0x09
#define USB_HID_DESIGNATOR_JAW			0x0a
#define USB_HID_DESIGNATOR_NECK			0x0b
#define USB_HID_DESIGNATOR_UPPER_ARM		0x0c
#define USB_HID_DESIGNATOR_ELBOW		0x0d
#define USB_HID_DESIGNATOR_FOREARM		0x0e
#define USB_HID_DESIGNATOR_WRIST		0x0f
#define USB_HID_DESIGNATOR_PALM			0x10
#define USB_HID_DESIGNATOR_THUMB		0x11
#define USB_HID_DESIGNATOR_INDEX_FINGER		0x12
#define USB_HID_DESIGNATOR_MIDDLE_FINGER	0x13
#define USB_HID_DESIGNATOR_RING_FINGER		0x14
#define USB_HID_DESIGNATOR_LITTLE_FINGER	0x15
#define USB_HID_DESIGNATOR_HEAD			0x16
#define USB_HID_DESIGNATOR_SHOULDER		0x17
#define USB_HID_DESIGNATOR_HIP			0x18
#define USB_HID_DESIGNATOR_WAIST		0x19
#define USB_HID_DESIGNATOR_THIGH		0x1a
#define USB_HID_DESIGNATOR_KNEE			0x1b
#define USB_HID_DESIGNATOR_CALF			0x1c
#define USB_HID_DESIGNATOR_ANKLE		0x1d
#define USB_HID_DESIGNATOR_FOOT			0x1e
#define USB_HID_DESIGNATOR_HEEL			0x1f
#define USB_HID_DESIGNATOR_BALL_OF_FOOT		0x20
#define USB_HID_DESIGNATOR_BIG_TOE		0x21
#define USB_HID_DESIGNATOR_SECOND_TOE		0x22
#define USB_HID_DESIGNATOR_THIRD_TOE		0x23
#define USB_HID_DESIGNATOR_FOURTH_TOE		0x24
#define USB_HID_DESIGNATOR_LITTLE_TOE		0x25
#define USB_HID_DESIGNATOR_BROW			0x26
#define USB_HID_DESIGNATOR_CHEEK		0x27

/* Flags */
#define USB_HID_QUALIFIER_MASK			0xe0
#define USB_HID_EFFORT_MASK			0x1f

/* Qualifier */
#define USB_HID_QUALIFIER_NOT_APPLICABLE	0
#define USB_HID_QUALIFIER_RIGHT			1
#define USB_HID_QUALIFIER_LEFT			2
#define USB_HID_QUALIFIER_BOTH			3
#define USB_HID_QUALIFIER_EITHER		4
#define USB_HID_QUALIFIER_CENTER		5
