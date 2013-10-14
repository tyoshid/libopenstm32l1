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
 * Universal Serial Bus
 * Device Class Definition for Audio Devices
 * Release 1.0
 */

/* Audio Device Class Codes */

/* A.1 Audio Interface Class Code */
#define USB_CLASS_AUDIO				1

/* A.2 Audio Interface Subclass Codes */
#define USB_AUDIO_SUBCLASS_UNDEFINED		0
#define USB_AUDIO_SUBCLASS_AUDIOCONTROL		1
#define USB_AUDIO_SUBCLASS_AUDIOSTREAMING	2
#define USB_AUDIO_SUBCLASS_MIDISTREAMING	3

/* A.3 Audio Interface Protocol Codes */
#define USB_AUDIO_PROTO_UNDEFINED		0

/* A.4 Audio Class-Specific Descriptor Types */
#define USB_DT_CS_UNDEFINED			0x20
#define USB_DT_CS_DEVICE			0x21
#define USB_DT_CS_CONFIGURATION			0x22
#define USB_DT_CS_STRING			0x23
#define USB_DT_CS_INTERFACE			0x24
#define USB_DT_CS_ENDPOINT			0x25

/* A.5 Audio Class-Specific AC Interface Descriptor Subtypes */
#define USB_AUDIO_AC_UNDEFINED			0
#define USB_AUDIO_HEADER			1
#define USB_AUDIO_INPUT_TERMINAL		2
#define USB_AUDIO_OUTPUT_TERMINAL		3
#define USB_AUDIO_MIXER_UNIT			4
#define USB_AUDIO_SELECTOR_UNIT			5
#define USB_AUDIO_FEATURE_UNIT			6
#define USB_AUDIO_PROCESSING_UNIT		7
#define USB_AUDIO_EXTENSION_UNIT		8

/* A.6 Audio Class-Specific AS Interface Descriptor Subtypes */
#define USB_AUDIO_AS_UNDEFINED			0
#define USB_AUDIO_AS_GENERAL			1
#define USB_AUDIO_FORMAT_TYPE			2
#define USB_AUDIO_FORMAT_SPECIFIC		3

/* A.7 Processing Unit Process Types */
#define USB_AUDIO_PROCESS_UNDEFINED		0
#define USB_AUDIO_UPDOWNMIX_PROCESS		1
#define USB_AUDIO_DOLBY_PROLOGIC_PROCESS	2
#define USB_AUDIO_3D_STEREO_EXTENDER_PROCESS	3
#define USB_AUDIO_REVERBERATION_PROCESS		4
#define USB_AUDIO_CHORUS_PROCESS		5
#define USB_AUDIO_DYN_RANGE_COMP_PROCESS	6

/* A.8 Audio Class-specific Endpoint Descriptor Subtypes */
#define USB_AUDIO_EP_UNDEFINED			0
#define USB_AUDIO_EP_GENERAL			1

/* A.9 Audio Class-Specific Request Codes */
#define USB_AUDIO_REQUEST_UNDEFINED		0x00
#define USB_AUDIO_SET_CUR			0x01
#define USB_AUDIO_GET_CUR			0x81
#define USB_AUDIO_SET_MIN			0x02
#define USB_AUDIO_GET_MIN			0x82
#define USB_AUDIO_SET_MAX			0x03
#define USB_AUDIO_GET_MAX			0x83
#define USB_AUDIO_SET_RES			0x04
#define USB_AUDIO_GET_RES			0x84
#define USB_AUDIO_SET_MEM			0x05
#define USB_AUDIO_GET_MEM			0x85
#define USB_AUDIO_GET_STAT			0xff

/* A.10 Control Selector Codes */
/* A.10.1 Terminal Control Selectors */
#define USB_AUDIO_TE_CONTROL_UNDEFINED		0
#define USB_AUDIO_COPY_PROTECT_CONTROL		1

/* A.10.2 Feature Unit Control Selectors */
#define USB_AUDIO_FU_CONTROL_UNDEFINED		0
#define USB_AUDIO_MUTE_CONTROL			1
#define USB_AUDIO_VOLUME_CONTROL		2
#define USB_AUDIO_BASS_CONTROL			3
#define USB_AUDIO_MID_CONTROL			4
#define USB_AUDIO_TREBLE_CONTROL		5
#define USB_AUDIO_GRAPHIC_EQUALIZER_CONTROL	6
#define USB_AUDIO_AUTOMATIC_GAIN_CONTROL	7
#define USB_AUDIO_DELAY_CONTROL			8
#define USB_AUDIO_BASS_BOOST_CONTROL		9
#define USB_AUDIO_LOUDNESS_CONTROL		10

/* A.10.3 Processing Unit Control Selectors */
/* A.10.3.1 Up/Down-mix Processing Unit Control Selectors */
#define USB_AUDIO_UD_CONTROL_UNDEFINED		0
#define USB_AUDIO_UD_ENABLE_CONTROL		1
#define USB_AUDIO_UD_MODE_SELECT_CONTROL	2

/* A.10.3.2 Dolby Prologic Processing Unit Control Selectors */
#define USB_AUDIO_DP_CONTROL_UNDEFINED		0
#define USB_AUDIO_DP_ENABLE_CONTROL		1
#define USB_AUDIO_DP_MODE_SELECT_CONTROL	2

/* A.10.3.3 3D Stereo Extender Processing Unit Control Selectors */
#define USB_AUDIO_3D_CONTROL_UNDEFINED		0
#define USB_AUDIO_3D_ENABLE_CONTROL		1
#define USB_AUDIO_SPACIOUSNESS_CONTROL		3

/* A.10.3.4 Reverberation Processing Unit Control Selectors */
#define USB_AUDIO_RV_CONTROL_UNDEFINED		0
#define USB_AUDIO_RV_ENABLE_CONTROL		1
#define USB_AUDIO_REVERB_LEVEL_CONTROL		2
#define USB_AUDIO_REVERB_TIME_CONTROL		3
#define USB_AUDIO_REVERB_FEEDBACK_CONTROL	4

/* A.10.3.5 Chorus Processing Unit Control Selectors */
#define USB_AUDIO_CH_CONTROL_UNDEFINED		0
#define USB_AUDIO_CH_ENABLE_CONTROL		1
#define USB_AUDIO_CHORUS_LEVEL_CONTROL		2
#define USB_AUDIO_CHORUS_RATE_CONTROL		3
#define USB_AUDIO_CHORUS_DEPTH_CONTROL		4

/* A.10.3.6 Dynamic Range Compressor Processing Unit Control Selectors */
#define USB_AUDIO_DR_CONTROL_UNDEFINED		0
#define USB_AUDIO_DR_ENABLE_CONTROL		1
#define USB_AUDIO_COMPRESSION_RATE_CONTROL	2
#define USB_AUDIO_DR_MAXAMPL_CONTROL		3
#define USB_AUDIO_THRESHOLD_CONTROL		4
#define USB_AUDIO_ATTACK_TIME			5
#define USB_AUDIO_RELEASE_TIME			6

/* A.10.4 Extension Unit Control Selectors */
#define USB_AUDIO_XU_CONTROL_UNDEFINED		0
#define USB_AUDIO_XU_ENABLE_CONTROL		1

/* A.10.5 Endpoint Control Selectors */
#define USB_AUDIO_EP_CONTROL_UNDEFINED		0
#define USB_AUDIO_SAMPLING_FREQ_CONTROL		1
#define USB_AUDIO_PITCH_CONTROL			2

/* Descriptors */

/* 4.3.2 Class-Specific AC Interface Descriptor */
/* Class-Specific AC Interface Header Descriptor */
#define USB_AUDIO_AC_IF_HEADER_DESCRIPTOR(n)		\
	struct usb_audio_ac_if_header_descriptor_##n {	\
		u8 bLength;				\
		u8 bDescriptorType;			\
		u8 bDescriptorSubtype;			\
		u16 bcdADC;				\
		u16 wTotalLength;			\
		u8 bInCollection;			\
		u8 baInterfaceNr[n];			\
	} __attribute__ ((packed))

/* 4.3.2.1 Input Terminal Descriptor */
struct usb_audio_input_terminal_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bTerminalID;
	u16 wTerminalType;
	u8 bAssocTerminal;
	u8 bNrChannels;
	u16 wChannelConfig;
	u8 iChannelNames;
	u8 iTerminal;
} __attribute__ ((packed));

#define USB_AUDIO_LEFT_FRONT			(1 << 0)
#define USB_AUDIO_RIGHT_FRONT			(1 << 1)
#define USB_AUDIO_CENTER_FRONT			(1 << 2)
#define USB_AUDIO_LOW_FREQ_ENHANCEMENT		(1 << 3)
#define USB_AUDIO_LEFT_SURROUND			(1 << 4)
#define USB_AUDIO_RIGHT_SURROUND		(1 << 5)
#define USB_AUDIO_LEFT_OF_CENTER		(1 << 6)
#define USB_AUDIO_RIGHT_OF_CENTER		(1 << 7)
#define USB_AUDIO_SURROUND			(1 << 8)
#define USB_AUDIO_SIDE_LEFT			(1 << 9)
#define USB_AUDIO_SIDE_RIGHT			(1 << 10)
#define USB_AUDIO_TOP				(1 << 11)

/* 4.3.2.2 Output Terminal Descriptor */
struct usb_audio_output_terminal_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bTerminalID;
	u16 wTerminalType;
	u8 bAssocTerminal;
	u8 bSourceID;
	u8 iTerminal;
} __attribute__ ((packed));

/* 4.3.2.3 Mixer Unit Descriptor */
#define USB_AUDIO_MIXER_UNIT_DESCRIPTOR(p, n)			\
	struct usb_audio_mixer_unit_descriptor_##p##_##n {	\
		u8 bLength;					\
		u8 bDescriptorType;				\
		u8 bDescriptorSubtype;				\
		u8 bUnitID;					\
		u8 bNrInPins;					\
		u8 baSourceID[p];				\
		u8 bNrChannels;					\
		u16 wChannelConfig;				\
		u8 iChannelNames;				\
		u8 bmControls[n];				\
		u8 iMixer;					\
	} __attribute__ ((packed))

/* 4.3.2.4 Selector Unit Descriptor */
#define USB_AUDIO_SELECTOR_UNIT_DESCRIPTOR(p)			\
	struct usb_audio_selector_unit_descriptor_##p {		\
		u8 bLength;					\
		u8 bDescriptorType;				\
		u8 bDescriptorSubtype;				\
		u8 bUnitID;					\
		u8 bNrInPins;					\
		u8 baSourceID[p];				\
		u8 iSelector;					\
	} __attribute__ ((packed))

/* 4.3.2.5 Feature Unit Descriptor */
#define USB_AUDIO_FEATURE_UNIT_DESCRIPTOR(ch, n)		\
	struct usb_audio_feature_unit_descriptor_##ch##_##n {	\
		u8 bLength;					\
		u8 bDescriptorType;				\
		u8 bDescriptorSubtype;				\
		u8 bUnitID;					\
		u8 bSourceID;					\
		u8 bControlSize;				\
		u8 bmaControls[ch + 1][n];			\
		u8 iFeature;					\
	} __attribute__ ((packed))

/* 4.3.2.6 Processing Unit Descriptor */

/* 4.3.2.6.1 Up/Down-mix Processing Unit Descriptor */
#define USB_AUDIO_UP_DOWN_MIX_PROCESSING_UNIT_DESCRIPTOR(n, m)		\
	struct usb_audio_up_down_mix_processing_unit_descriptor_##n##_##m {\
		u8 bLength;						\
		u8 bDescriptorType;					\
		u8 bDescriptorSubtype;					\
		u8 bUnitID;						\
		u16 wProcessType;					\
		u8 bNrInPins;						\
		u8 bSourceID;						\
		u8 bNrChannels;						\
		u16 wChannelConfig;					\
		u8 iChannelNames;					\
		u8 bControlSize;					\
		u8 bmControls[n];					\
		u8 iProcessing;						\
		u8 bNrModes;						\
		u16 waModes[m];						\
	} __attribute__ ((packed))

/* 4.3.2.6.2 Dolby Prologic Processing Unit Descripror */
#define USB_AUDIO_DOLBY_PROLOGIC_PROCESSING_UNIT_DESCRIPTOR(n, m)	\
	struct usb_audio_dolby_prologic_processing_unit_descriptor_##n##_##m {\
		u8 bLength;						\
		u8 bDescriptorType;					\
		u8 bDescriptorSubtype;					\
		u8 bUnitID;						\
		u16 wProcessType;					\
		u8 bNrInPins;						\
		u8 bSourceID;						\
		u8 bNrChannels;						\
		u16 wChannelConfig;					\
		u8 iChannelNames;					\
		u8 bControlSize;					\
		u8 bmControls[n];					\
		u8 iProcessing;						\
		u8 bNrModes;						\
		u16 waModes[m];						\
	} __attribute__ ((packed))

/* 4.3.2.6.3 3D-Stereo Extender Processing Unit Descriptor */
#define USB_AUDIO_3D_STEREO_EXTENDER_PROCESSING_UNIT_DESCRIPTOR(n)	\
	struct usb_audio_3d_stereo_extender_processing_unit_descriptor_##n {\
		u8 bLength;						\
		u8 bDescriptorType;					\
		u8 bDescriptorSubtype;					\
		u8 bUnitID;						\
		u16 wProcessType;					\
		u8 bNrInPins;						\
		u8 bSourceID;						\
		u8 bNrChannels;						\
		u16 wChannelConfig;					\
		u8 iChannelNames;					\
		u8 bControlSize;					\
		u8 bmControls[n];					\
		u8 iProcessing;						\
	} __attribute__ ((packed))

/* 4.3.2.6.4 Reverberation Processing Unit Descriptor */
#define USB_AUDIO_3D_REVERBERATION_PROCESSING_UNIT_DESCRIPTOR(n)	\
	struct usb_audio_reverberation_processing_unit_descriptor_##n {	\
		u8 bLength;						\
		u8 bDescriptorType;					\
		u8 bDescriptorSubtype;					\
		u8 bUnitID;						\
		u16 wProcessType;					\
		u8 bNrInPins;						\
		u8 bSourceID;						\
		u8 bNrChannels;						\
		u16 wChannelConfig;					\
		u8 iChannelNames;					\
		u8 bControlSize;					\
		u8 bmControls[n];					\
		u8 iProcessing;						\
	} __attribute__ ((packed))

/* 4.3.2.6.5 Chorus Processing Unit Descriptor */
#define USB_AUDIO_CHORUS_PROCESSING_UNIT_DESCRIPTOR(n)			\
	struct usb_audio_chorus_processing_unit_descriptor_##n {	\
		u8 bLength;						\
		u8 bDescriptorType;					\
		u8 bDescriptorSubtype;					\
		u8 bUnitID;						\
		u16 wProcessType;					\
		u8 bNrInPins;						\
		u8 bSourceID;						\
		u8 bNrChannels;						\
		u16 wChannelConfig;					\
		u8 iChannelNames;					\
		u8 bControlSize;					\
		u8 bmControls[n];					\
		u8 iProcessing;						\
	} __attribute__ ((packed))

/* 4.3.2.6.6 Dynamic Range Compressor Processing Unit Descriptor */
#define USB_AUDIO_DYNAMIC_RANGE_COMP_PROCESSING_UNIT_DESCRIPTOR(n)	\
	struct usb_audio_dynamic_range_comp_processing_unit_descriptor_##n {\
		u8 bLength;						\
		u8 bDescriptorType;					\
		u8 bDescriptorSubtype;					\
		u8 bUnitID;						\
		u16 wProcessType;					\
		u8 bNrInPins;						\
		u8 bSourceID;						\
		u8 bNrChannels;						\
		u16 wChannelConfig;					\
		u8 iChannelNames;					\
		u8 bControlSize;					\
		u8 bmControls[n];					\
		u8 iProcessing;						\
	} __attribute__ ((packed))

/* 4.3.2.7 Extension Unit Descriptor */
#define USB_AUDIO_EXTENSION_UNIT_DESCRIPTOR(p, n)		\
	struct usb_audio_extension_unit_descriptor_##p##_##n {	\
		u8 bLength;					\
		u8 bDescriptorType;				\
		u8 bDescriptorSubtype;				\
		u8 bUnitID;					\
		u16 wExtensionCode;				\
		u8 bNrInPins;					\
		u8 baSourceID[p];				\
		u8 bNrChannels;					\
		u16 wChannelConfig;				\
		u8 iChannelNames;				\
		u8 bControlSize;				\
		u8 bmControls[n];				\
		u8 iExtension;					\
	} __attribute__ ((packed))

/* 4.3.2.8 Associated Interface Descriptor */

/* 4.5.2 Class-Specific AS Interface Descriptor */
struct usb_audio_as_interface_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bTerminalLink;
	u8 bDelay;
	u16 wFormatTag;
} __attribute__ ((packed));

/* 4.6.1.1 Standard AS Isochronous Audio Data Endpoint Descriptor */
struct usb_audio_endpoint_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u8 bEndpointAddress;
	u8 bmAttributes;
	u16 wMaxPacketSize;
	u8 bInterval;
	u8 bRefresh;
	u8 bSynchAddress;
} __attribute__ ((packed));

/* 4.6.1.2 Class-Specific AS Isochronous Audio Data Endpoint Descriptor */
struct usb_audio_as_endpoint_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bmAttributes;
	u8 bLockDelayUnits;
	u16 wLockDelay;
} __attribute__ ((packed));

/*
 * Universal Serial Bus Device Class Definition for Audio Data Formats
 * Release 1.0
 */

/* Additional Audio Device Class Codes */

/* A.1 Audio Data Format Codes */
/* A.1.1 Audio Data Format Type I Codes */
#define USB_AUDIO_TYPE_I_UNDEFINED			0x0000
#define USB_AUDIO_PCM					0x0001
#define USB_AUDIO_PCM8					0x0002
#define USB_AUDIO_IEEE_FLOAT				0x0003
#define USB_AUDIO_ALAW					0x0004
#define USB_AUDIO_MULAW					0x0005

/* A.1.2 Audio Data Format Type II Codes */
#define USB_AUDIO_TYPE_II_UNDEFINED			0x1000
#define USB_AUDIO_MPEG					0x1001
#define USB_AUDIO_AC3					0x1002

/* A.1.3 Audio Data Format Type III Codes */
#define USB_AUDIO_TYPE_III_UNDEFINED			0x2000
#define USB_AUDIO_IEC1937_AC3				0x2001
#define USB_AUDIO_IEC1937_MPEG1_LAYER1			0x2002
#define USB_AUDIO_IEC1937_MPEG1_LAYER2_3		0x2003
#define USB_AUDIO_IEC1937_MPEG2_NOEXT			0x2003
#define USB_AUDIO_IEC1937_MPEG2_EXT			0x2004
#define USB_AUDIO_IEC1937_MPEG2_LAYER1_LS		0x2005
#define USB_AUDIO_IEC1937_MPEG2_LAYER2_3_LS		0x2006

/* A.2 Format Type Codes */
#define USB_AUDIO_FORMAT_TYPE_UNDEFINED			0x00
#define USB_AUDIO_FORMAT_TYPE_I				0x01
#define USB_AUDIO_FORMAT_TYPE_II			0x02
#define USB_AUDIO_FORMAT_TYPE_III			0x03

/* A.3 Format-Specific Control Selectors */
/* A.3.1 MPEG Control Selectors */
#define USB_AUDIO_MPEG_CONTROL_UNDEFINED		0x00
#define USB_AUDIO_MP_DUAL_CHANNEL_CONTROL		0x01
#define USB_AUDIO_MP_SECOND_STEREO_CONTROL		0x02
#define USB_AUDIO_MP_MULTILINGUAL_CONTROL		0x03
#define USB_AUDIO_MP_DYN_RANGE_CONTROL			0x04
#define USB_AUDIO_MP_SCALING_CONTROL			0x05
#define USB_AUDIO_MP_HILO_SCALING_CONTROL		0x06

/* A.3.2 AC-3 Control Selectors */
#define USB_AUDIO_AC_CONTROL_UNDEFINED			0x00
#define USB_AUDIO_AC_MODE_CONTROL			0x01
#define USB_AUDIO_AC_DYN_RANGE_CONTROL			0x02
#define USB_AUDIO_AC_SCALING_CONTROL			0x03
#define USB_AUDIO_AC_HILO_SCALING_CONTROL		0x04

/* Descriptors */

/* 2.2.5 Type I Format Type Descriptor */
/* Continuous Sampling Frequency */
struct usb_audio_type_i_format_type_descriptor_0 {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bFormatType;
	u8 bNrChannels;
	u8 bSubFrameSize;
	u8 bBitResolution;
	u8 bSamFreqType;
	u8 tLowerSamFreq[3];
	u8 tUpperSamFreq[3];
} __attribute__ ((packed));

/* Discrete Number of Sampling Frequencies */
#define USB_AUDIO_TYPE_I_FORMAT_TYPE_DESCRIPTOR(ns)		\
	struct usb_audio_type_i_format_type_descriptor_##ns {	\
		u8 bLength;					\
		u8 bDescriptorType;				\
		u8 bDescriptorSubtype;				\
		u8 bFormatType;					\
		u8 bNrChannels;					\
		u8 bSubFrameSize;				\
		u8 bBitResolution;				\
		u8 bSamFreqType;				\
		u8 tSamFreq[ns][3];				\
	} __attribute__ ((packed));

/* 2.3.6 Type II Format Type Descriptor */
/* Continuous Sampling Frequency */
struct usb_audio_type_ii_format_type_descriptor_0 {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bFormatType;
	u16 wMaxBitRate;
	u16 wSamplesPerFrame;
	u8 bSamFreqType;
	u8 tLowerSamFreq[3];
	u8 tUpperSamFreq[3];
} __attribute__ ((packed));

/* Discrete Number of Sampling Frequencies */
#define USB_AUDIO_TYPE_II_FORMAT_TYPE_DESCRIPTOR(ns)		\
	struct usb_audio_type_ii_format_type_descriptor_##ns {	\
		u8 bLength;					\
		u8 bDescriptorType;				\
		u8 bDescriptorSubtype;				\
		u8 bFormatType;					\
		u16 wMaxBitRate;				\
		u16 wSamplesPerFrame;				\
		u8 bSamFreqType;				\
		u8 tSamFreq[ns][3];				\
	} __attribute__ ((packed));

/* 2.3.8.1 MPEG Format-Specific Descriptor */
struct usb_audio_mpeg_format_specific_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u16 wFormatTag;
	u16 bmMPEGCapabilities;
	u8 bmMPEGFeatures;
} __attribute__ ((packed));

/* 2.3.8.2.1 AC-3 Format-Specific Descriptor */
struct usb_audio_ac3_format_specific_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u16 wFormatTag;
	u32 bmBSID;
	u8 bmAC3Features;
} __attribute__ ((packed));

/* 2.4.1 Type III Format Type Descriptor */
/* Continuous Sampling Frequency */
struct usb_audio_type_iii_format_type_descriptor_0 {
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bFormatType;
	u8 bNrChannels;
	u8 bSubframeSize;
	u8 bBitResolution;
	u8 bSamFreqType;
	u8 tLowerSamFreq[3];
	u8 tUpperSamFreq[3];
} __attribute__ ((packed));

/* Discrete Number of Sampling Frequencies */
#define USB_AUDIO_TYPE_III_FORMAT_TYPE_DESCRIPTOR(ns)		\
	struct usb_audio_type_iii_format_type_descriptor_##ns {	\
		u8 bLength;					\
		u8 bDescriptorType;				\
		u8 bDescriptorSubtype;				\
		u8 bFormatType;					\
		u8 bNrChannels;					\
		u8 bSubframeSize;				\
		u8 bBitResolution;				\
		u8 bSamFreqType;				\
		u8 tSamFreq[ns][3];				\
	} __attribute__ ((packed));

/*
 * Universal Serial Bus Device Class Definition for Terminal Types
 * Release 1.0
 */

/* 2.1 USB Terminal Types */
#define USB_AUDIO_USB_UNDEFINED			0x0100
#define USB_AUDIO_USB_STREAMING			0x0101
#define USB_AUDIO_USB_VENDOR			0x01ff

/* 2.2 Input Terminal Types */
#define USB_AUDIO_INPUT_UNDEFINED		0x0200
#define USB_AUDIO_MICROPHONE			0x0201
#define USB_AUDIO_DESKTOP_MICROPHONE		0x0202
#define USB_AUDIO_PERSONAL_MICROPHONE		0x0203
#define USB_AUDIO_OMNI_DIR_MICROPHONE		0x0204
#define USB_AUDIO_MICROPHONE_ARRAY		0x0205
#define USB_AUDIO_PROC_MICROPHONE_ARRAY		0x0206

/* 2.3 Output Terminal Types */
#define USB_AUDIO_OUTPUT_UNDEFINED		0x0300
#define USB_AUDIO_SPEAKER			0x0301
#define USB_AUDIO_HEADPHONES			0x0302
#define USB_AUDIO_HEAD_MOUNTED_DISPLAY_AUDIO	0x0303
#define USB_AUDIO_DESKTOP_SPEAKER		0x0304
#define USB_AUDIO_ROOM_SPEAKER			0x0305
#define USB_AUDIO_COMMUNICATION_SPEAKER		0x0306
#define USB_AUDIO_LOW_FREQ_EFFECTS_SPEAKER	0x0307

/* 2.4 Bi-directional Terminal Types */
#define USB_AUDIO_BIDIRECTIONAL_UNDEFINED	0x0400
#define USB_AUDIO_HANDSET			0x0401
#define USB_AUDIO_HEADSET			0x0402
#define USB_AUDIO_SPEAKERPHONE			0x0403
#define USB_AUDIO_ECHO_SUPPRES_SPEAKERPHONE	0x0404
#define USB_AUDIO_ECHO_CANCEL_SPEAKERPHONE	0x0405

/* 2.5 Telephony Terminal Types */
#define USB_AUDIO_TELEPHONY_UNDEFINED		0x0500
#define USB_AUDIO_PHONE_LINE			0x0501
#define USB_AUDIO_TELEPHONE			0x0502
#define USB_AUDIO_DOWN_LINE_PHONE		0x0503

/* 2.6 External Terminal Types */
#define USB_AUDIO_EXTERNAL_UNDEFINED		0x0600
#define USB_AUDIO_ANALOG_CONNECTOR		0x0601
#define USB_AUDIO_DIGITAL_AUDIO_INTERFACE	0x0602
#define USB_AUDIO_LINE_CONNECTOR		0x0603
#define USB_AUDIO_LEGACY_AUDIO_CONNECTOR	0x0604
#define USB_AUDIO_SPDIF_INTERFACE		0x0605
#define USB_AUDIO_1394_DA_STREAM		0x0606
#define USB_AUDIO_1394_DV_STREAM_SOUNDTRACK	0x0607

/* 2.7 Embedded Function Terminal Types */
#define USB_AUDIO_EMBEDDED_UNDEFINED		0x0700
#define USB_AUDIO_LEVEL_CAL_NOISE_SOURCE	0x0701
#define USB_AUDIO_EQUALIZATION_NOISE		0x0702
#define USB_AUDIO_CD_PLAYER			0x0703
#define USB_AUDIO_DAT				0x0704
#define USB_AUDIO_DCC				0x0705
#define USB_AUDIO_MINIDISK			0x0706
#define USB_AUDIO_ANALOG_TAPE			0x0707
#define USB_AUDIO_PHONOGRAPH			0x0708
#define USB_AUDIO_VCR_AUDIO			0x0709
#define USB_AUDIO_VIDEO_DISK_AUDIO		0x070a
#define USB_AUDIO_DVD_AUDIO			0x070b
#define USB_AUDIO_TV_TUNER_AUDIO		0x070c
#define USB_AUDIO_SATELLITE_RECEIVER_AUDIO	0x070d
#define USB_AUDIO_CABLE_TUNER_AUDIO		0x070e
#define USB_AUDIO_DSS_AUDIO			0x070f
#define USB_AUDIO_RADIO_RECEIVER		0x0710
#define USB_AUDIO_RADIO_TRANSMITTER		0x0711
#define USB_AUDIO_MULTITRACK_RECORDER		0x0712
#define USB_AUDIO_SYNTHESIZER			0x0713
