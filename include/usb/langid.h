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

/* Lnaguage Identifiers */
#define LANGID_AFRIKAANS			0x0436
#define LANGID_ALBANIAN				0x041c
#define LANGID_ARABIC_SAUDI_ARABIA		0x0401
#define LANGID_ARABIC_IRAQ			0x0801
#define LANGID_ARABIC_EGYPT			0x0c01
#define LANGID_ARABIC_LIBYA			0x1001
#define LANGID_ARABIC_ALGERIA			0x1401
#define LANGID_ARABIC_MOROCCO			0x1801
#define LANGID_ARABIC_TUNISIA			0x1c01
#define LANGID_ARABIC_OMAN			0x2001
#define LANGID_ARABIC_YEMEN			0x2401
#define LANGID_ARABIC_SYRIA			0x2801
#define LANGID_ARABIC_JORDAN			0x2c01
#define LANGID_ARABIC_LEBANON			0x3001
#define LANGID_ARABIC_KUWAIT			0x3401
#define LANGID_ARABIC_UAE			0x3801
#define LANGID_ARABIC_BAHRAIN			0x3c01
#define LANGID_ARABIC_QATAR			0x4001
#define LANGID_ARMENIAN				0x042b
#define LANGID_ASSAMESE				0x044d
#define LANGID_AZERI_LATIN			0x042c
#define LANGID_AZERI_CYRILLIC			0x082c
#define LANGID_BASQUE				0x042d
#define LANGID_BELARUSIAN			0x0423
#define LANGID_BENGALI				0x0445
#define LANGID_BULGARIAN			0x0402
#define LANGID_BURMESE				0x0455
#define LANGID_CATALAN				0x0403
#define LANGID_CHINESE_TRADITIONAL		0x0404
#define LANGID_CHINESE_SIMPLIFIED		0x0804
#define LANGID_CHINESE_HONGKONGQ		0x0c04
#define LANGID_CHINESE_SINGAPORE		0x1004
#define LANGID_CHINESE_MACAU			0x1404
#define LANGID_CROATIAN				0x041a
#define LANGID_CZECH				0x0405
#define LANGID_DANISH				0x0406
#define LANGID_DUTCH				0x0413
#define LANGID_DUTCH_BELGIAN			0x0813
#define LANGID_ENGLISH_US			0x0409
#define LANGID_ENGLISH_UK			0x0809
#define LANGID_ENGLISH_AUS			0x0c09
#define LANGID_ENGLISH_CAN			0x1009
#define LANGID_ENGLISH_NZ			0x1409
#define LANGID_ENGLISH_EIRE			0x1809
#define LANGID_ENGLISH_SOUTH_AFRICA		0x1c09
#define LANGID_ENGLISH_JAMAICA			0x2009
#define LANGID_ENGLISH_CARIBBEAN		0x2409
#define LANGID_ENGLISH_BELIZE			0x2809
#define LANGID_ENGLISH_TRINIDAD			0x2c09
#define LANGID_ENGLISH_ZIMBABWE			0x3009
#define LANGID_ENGLISH_PHILIPPINES		0x3409
#define LANGID_ESTONIAN				0x0425
#define LANGID_FAEROESE				0x0438
#define LANGID_FARSI				0x0429
#define LANGID_FINNISH				0x040b
#define LANGID_FRENCH				0x040c
#define LANGID_FRENCH_BELGIAN			0x080c
#define LANGID_FRENCH_CANADIAN			0x0c0c
#define LANGID_FRENCH_SWISS			0x100c
#define LANGID_FRENCH_LUXEMBOURG		0x140c
#define LANGID_FRENCH_MONACO			0x180c
#define LANGID_GEORGIAN				0x0437
#define LANGID_GERMAN				0x0407
#define LANGID_GERMAN_SWISS			0x0807
#define LANGID_GERMAN_AUSTRIAN			0x0c07
#define LANGID_GERMAN_LUXEMBOURG		0x1007
#define LANGID_GERMAN_LIECHTENSTEIN		0x1407
#define LANGID_GREEK				0x0408
#define LANGID_GUJARATI				0x0447
#define LANGID_HEBREW				0x040d
#define LANGID_HINDI				0x0439
#define LANGID_HUNGARIAN			0x040e
#define LANGID_ICELANDIC			0x040f
#define LANGID_INDONESIAN			0x0421
#define LANGID_ITALIAN				0x0410
#define LANGID_ITALIAN_SWISS			0x0810
#define LANGID_JAPANESE				0x0411
#define LANGID_KANNADA				0x044b
#define LANGID_KASHMIRI				0x0860
#define LANGID_KAZAK				0x043f
#define LANGID_KONKANI				0x0457
#define LANGID_KOREAN				0x0412
#define LANGID_KOREAN_JOHAB			0x0812
#define LANGID_LATVIAN				0x0426
#define LANGID_LITHUANIAN			0x0427
#define LANGID_LITHUANIAN_CLASSIC		0x0827
#define LANGID_MACEDONIAN			0x042f
#define LANGID_MALAY_MALAYSIA			0x043e
#define LANGID_MALAY_BRUNEI_DARUSSALAM		0x083e
#define LANGID_MALAYALAM			0x044c
#define LANGID_MANIPURI				0x0458
#define LANGID_MARATHI				0x044e
#define LANGID_NEPALI				0x0861
#define LANGID_NORWEGIAN_BOKMAL			0x0414
#define LANGID_NORWEGIAN_NYNORSK		0x0814
#define LANGID_ORIYA				0x0448
#define LANGID_POLISH				0x0415
#define LANGID_PORTUGUESE_BRAZILIAN		0x0416
#define LANGID_PORTUGUESE			0x0816
#define LANGID_PUNJABI				0x0446
#define LANGID_ROMANIAN				0x0418
#define LANGID_RUSSIAN				0x0419
#define LANGID_SANSKRIT				0x044f
#define LANGID_SERBIAN_CYRILLIC			0x0c1a
#define LANGID_SERBIAN_LATIN			0x081a
#define LANGID_SINDHI				0x0459
#define LANGID_SLOVAK				0x041b
#define LANGID_SLOVENIAN			0x0424
#define LANGID_SPANISH				0x040a
#define LANGID_SPANISH_MEXICAN			0x080a
#define LANGID_SPANISH_MODERN			0x0c0a
#define LANGID_SPANISH_GUATEMALA		0x100a
#define LANGID_SPANISH_COSTA_RICA		0x140a
#define LANGID_SPANISH_PANAMA			0x180a
#define LANGID_SPANISH_DOMINICAN_REPUBLIC	0x1c0a
#define LANGID_SPANISH_VENEZUELA		0x200a
#define LANGID_SPANISH_COLOMBIA			0x240a
#define LANGID_SPANISH_PERU			0x280a
#define LANGID_SPANISH_ARGENTINA		0x2c0a
#define LANGID_SPANISH_ECUADOR			0x300a
#define LANGID_SPANISH_CHILE			0x340a
#define LANGID_SPANISH_URUGUAY			0x380a
#define LANGID_SPANISH_PARAGUAY			0x3c0a
#define LANGID_SPANISH_BOLIVIA			0x400a
#define LANGID_SPANISH_EL_SALVADOR		0x440a
#define LANGID_SPANISH_HONDURAS			0x480a
#define LANGID_SPANISH_NICARAGUA		0x4c0a
#define LANGID_SPANISH_PUERTO_RICO		0x500a
#define LANGID_SUTU				0x0430
#define LANGID_SWAHILI				0x0441
#define LANGID_SWEDISH				0x041d
#define LANGID_SWEDISH_FINLAND			0x081d
#define LANGID_TAMIL				0x0449
#define LANGID_TATAR				0x0444
#define LANGID_TELUGU				0x044a
#define LANGID_THAI				0x041e
#define LANGID_TURKISH				0x041f
#define LANGID_UKRANIAN				0x0422
#define LANGID_URDU_PAKISTAN			0x0420
#define LANGID_URDU_INDIA			0x0820
#define LANGID_UZBEK_LATIN			0x0443
#define LANGID_UZBEK_CYRILLIC			0x0843
#define LANGID_VIETNAMESE			0x042a
#define LANGID_HID_USAGE_DATA_DESCRIPTOR	0x04ff
#define LANGID_HID_VENDOR_DEFINED_1		0xf0ff
#define LANGID_HID_VENDOR_DEFINED_2		0xf4ff
#define LANGID_HID_VENDOR_DEFINED_3		0xf8ff
#define LANGID_HID_VENDOR_DEFINED_4		0xfcff

/* Primary Language Identifiers */
#define LANG_ARABIC				0x01
#define LANG_BULGARIAN				0x02
#define LANG_CATALAN				0x03
#define LANG_CHINESE				0x04
#define LANG_CZECH				0x05
#define LANG_DANISH				0x06
#define LANG_GERMAN				0x07
#define LANG_GREEK				0x08
#define LANG_ENGLISH				0x09
#define LANG_SPANISH				0x0a
#define LANG_FINNISH				0x0b
#define LANG_FRENCH				0x0c
#define LANG_HEBREW				0x0d
#define LANG_HUNGARIAN				0x0e
#define LANG_ICELANDIC				0x0f
#define LANG_ITALIAN				0x10
#define LANG_JAPANESE				0x11
#define LANG_KOREAN				0x12
#define LANG_DUTCH				0x13
#define LANG_NORWEGIAN				0x14
#define LANG_POLISH				0x15
#define LANG_PORTUGUESE				0x16
#define LANG_ROMANIAN				0x18
#define LANG_RUSSIAN				0x19
#define LANG_CROATIAN				0x1a
#define LANG_SERBIAN				0x1a
#define LANG_SLOVAK				0x1b
#define LANG_ALBANIAN				0x1c
#define LANG_SWEDISH				0x1d
#define LANG_THAI				0x1e
#define LANG_TURKISH				0x1f
#define LANG_URDU				0x20
#define LANG_INDONESIAN				0x21
#define LANG_UKRANIAN				0x22
#define LANG_BELARUSIAN				0x23
#define LANG_SLOVENIAN				0x24
#define LANG_ESTONIAN				0x25
#define LANG_LATVIAN				0x26
#define LANG_LITHUANIAN				0x27
#define LANG_FARSI				0x29
#define LANG_VIETNAMESE				0x2a
#define LANG_ARMENIAN				0x2b
#define LANG_AZERI				0x2c
#define LANG_BASQUE				0x2d
#define LANG_MACEDONIAN				0x2f
#define LANG_AFRIKAANS				0x36
#define LANG_GEORGIAN				0x37
#define LANG_FAEROESE				0x38
#define LANG_HINDI				0x39
#define LANG_MALAY				0x3e
#define LANG_KAZAK				0x3f
#define LANG_SWAHILI				0x41
#define LANG_UZBEK				0x43
#define LANG_TATAR				0x44
#define LANG_BENGALI				0x45
#define LANG_PUNJABI				0x46
#define LANG_GUJARATI				0x47
#define LANG_ORIYA				0x48
#define LANG_TAMIL				0x49
#define LANG_TELUGU				0x4a
#define LANG_KANNADA				0x4b
#define LANG_MALAYALAM				0x4c
#define LANG_ASSAMESE				0x4d
#define LANG_MARATHI				0x4e
#define LANG_SANSKRIT				0x4f
#define LANG_KONKANI				0x57
#define LANG_MANIPURI				0x58
#define LANG_SINDHI				0x59
#define LANG_KASHMIRI				0x60
#define LANG_NEPALI				0x61
#define LANG_HID				0xff

/* SubLanguage Identifiers */
#define SUBLANG_ARABIC_SAUDI_ARABIA		0x01
#define SUBLANG_ARABIC_IRAQ			0x02
#define SUBLANG_ARABIC_EGYPT			0x03
#define SUBLANG_ARABIC_LIBYA			0x04
#define SUBLANG_ARABIC_ALGERIA			0x05
#define SUBLANG_ARABIC_MOROCCO			0x06
#define SUBLANG_ARABIC_TUNISIA			0x07
#define SUBLANG_ARABIC_OMAN			0x08
#define SUBLANG_ARABIC_YEMEN			0x09
#define SUBLANG_ARABIC_SYRIA			0x10
#define SUBLANG_ARABIC_JORDAN			0x11
#define SUBLANG_ARABIC_LEBANON			0x12
#define SUBLANG_ARABIC_KUWAIT			0x13
#define SUBLANG_ARABIC_UAE			0x14
#define SUBLANG_ARABIC_BAHRAIN			0x15
#define SUBLANG_ARABIC_QATAR			0x16
#define SUBLANG_AZERI_CYRILLIC			0x01
#define SUBLANG_AZERI_LATIN			0x02
#define SUBLANG_CHINESE_TRADITIONAL		0x01
#define SUBLANG_CHINESE_SIMPLIFIED		0x02
#define SUBLANG_CHINESE_HONGKONG		0x03
#define SUBLANG_CHINESE_SINGAPORE		0x04
#define SUBLANG_CHINESE_MACAU			0x05
#define SUBLANG_DUTCH				0x01
#define SUBLANG_DUTCH_BELGIAN			0x02
#define SUBLANG_ENGLISH_US			0x01
#define SUBLANG_ENGLISH_UK			0x02
#define SUBLANG_ENGLISH_AUS			0x03
#define SUBLANG_ENGLISH_CAN			0x04
#define SUBLANG_ENGLISH_NZ			0x05
#define SUBLANG_ENGLISH_EIRE			0x06
#define SUBLANG_ENGLISH_SOUTH_AFRICA		0x07
#define SUBLANG_ENGLISH_JAMAICA			0x08
#define SUBLANG_ENGLISH_CARIBBEAN		0x09
#define SUBLANG_ENGLISH_BELIZE			0x0a
#define SUBLANG_ENGLISH_TRINIDAD		0x0b
#define SUBLANG_ENGLISH_PHILIPPINES		0x0c
#define SUBLANG_ENGLISH_ZIMBABWE		0x0d
#define SUBLANG_FRENCH				0x01
#define SUBLANG_FRENCH_BELGIAN			0x02
#define SUBLANG_FRENCH_CANADIAN			0x03
#define SUBLANG_FRENCH_SWISS			0x04
#define SUBLANG_FRENCH_LUXEMBOURG		0x05
#define SUBLANG_FRENCH_MONACO			0x06
#define SUBLANG_GERMAN				0x01
#define SUBLANG_GERMAN_SWISS			0x02
#define SUBLANG_GERMAN_AUSTRIAN			0x03
#define SUBLANG_GERMAN_LUXEMBOURG		0x04
#define SUBLANG_GERMAN_LIECHTENSTEIN		0x05
#define SUBLANG_ITALIAN				0x01
#define SUBLANG_ITALIAN_SWISS			0x02
#define SUBLANG_KASHMIRI_INDIA			0x02
#define SUBLANG_KOREAN				0x01
#define SUBLANG_LITHUANIAN			0x01
#define SUBLANG_MALAY_MALAYSIA			0x01
#define SUBLANG_MALAY_BRUNEI_DARUSSALAM		0x02
#define SUBLANG_NEPALI_INDIA			0x02
#define SUBLANG_NORWEGIAN_BOKMAL		0x01
#define SUBLANG_NORWEGIAN_NYNORSK		0x02
#define SUBLANG_PORTUGUESE			0x01
#define SUBLANG_PORTUGUESE_BRAZILIAN		0x02
#define SUBLANG_SERBIAN_LATIN			0x02
#define SUBLANG_SERBIAN_CYRILLIC		0x03
#define SUBLANG_SPANISH				0x01
#define SUBLANG_SPANISH_MEXICAN			0x02
#define SUBLANG_SPANISH_MODERN			0x03
#define SUBLANG_SPANISH_GUATEMALA		0x04
#define SUBLANG_SPANISH_COSTA_RICA		0x05
#define SUBLANG_SPANISH_PANAMA			0x06
#define SUBLANG_SPANISH_DOMINICAN_REPUBLIC	0x07
#define SUBLANG_SPANISH_VENEZUELA		0x08
#define SUBLANG_SPANISH_COLOMBIA		0x09
#define SUBLANG_SPANISH_PERU			0x0a
#define SUBLANG_SPANISH_ARGENTINA		0x0b
#define SUBLANG_SPANISH_ECUADOR			0x0c
#define SUBLANG_SPANISH_CHILE			0x0d
#define SUBLANG_SPANISH_URUGUAY			0x0e
#define SUBLANG_SPANISH_PARAGUAY		0x0f
#define SUBLANG_SPANISH_BOLIVIA			0x10
#define SUBLANG_SPANISH_EL_SALVADOR		0x11
#define SUBLANG_SPANISH_HONDURAS		0x12
#define SUBLANG_SPANISH_NICARAGUA		0x13
#define SUBLANG_SPANISH_PUERTO_RICO		0x14
#define SUBLANG_SWEDISH				0x01
#define SUBLANG_SWEDISH_FINLAND			0x02
#define SUBLANG_URDU_PAKISTAN			0x01
#define SUBLANG_URDU_INDIA			0x02
#define SUBLANG_UZBEK_LATIN			0x01
#define SUBLANG_UZBEK_CYRILLIC			0x02
#define SUBLANG_HID_USAGE_DATA_DESCRIPTOR	0x01
#define SUBLANG_HID_VENDOR_DEFINED_1		0x3c
#define SUBLANG_HID_VENDOR_DEFINED_2		0x3d
#define SUBLANG_HID_VENDOR_DEFINED_3		0x3e
#define SUBLANG_HID_VENDOR_DEFINED_4		0x3f
