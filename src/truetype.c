/*
Copyright (c) 2015, Michael Droettboom
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/

#include "truetype.h"
#include "doc/truetype.h"

#include "constants.h"

#include FT_TRUETYPE_IDS_H


/****************************************************************************
 Setup
*/


static PyTypeObject Py_TT_PLATFORM_Type;
ftpy_ConstantType Py_TT_PLATFORM_ConstantType;
#define TT_PLATFORM_CONST(name) DEF_CONST(name, TT_PLATFORM)
static constant_def TT_PLATFORM_constants[] = {
    TT_PLATFORM_CONST(APPLE_UNICODE),
    TT_PLATFORM_CONST(MACINTOSH),
    TT_PLATFORM_CONST(ISO),
    TT_PLATFORM_CONST(MICROSOFT),
    TT_PLATFORM_CONST(CUSTOM),
    TT_PLATFORM_CONST(ADOBE),
    {NULL}
};


static PyTypeObject Py_TT_APPLE_ID_Type;
ftpy_ConstantType Py_TT_APPLE_ID_ConstantType;
#define TT_APPLE_ID_CONST(name) DEF_CONST(name, TT_APPLE_ID)
static constant_def TT_APPLE_ID_constants[] = {
    TT_APPLE_ID_CONST(DEFAULT),
    TT_APPLE_ID_CONST(UNICODE_1_1),
    TT_APPLE_ID_CONST(ISO_10646),
    TT_APPLE_ID_CONST(UNICODE_2_0),
    TT_APPLE_ID_CONST(UNICODE_32),
    TT_APPLE_ID_CONST(VARIANT_SELECTOR),
    {NULL}
};


static PyTypeObject Py_TT_ISO_ID_Type;
ftpy_ConstantType Py_TT_ISO_ID_ConstantType;
#define TT_ISO_ID_CONST(name) DEF_CONST(name, TT_ISO_ID)
static constant_def TT_ISO_ID_constants[] = {
    {"ISO_7BIT_ASCII", TT_ISO_ID_7BIT_ASCII},
    {"ISO_10646", TT_ISO_ID_10646},
    {"ISO_8859_1", TT_ISO_ID_8859_1},
    {NULL}
};


static PyTypeObject Py_TT_MAC_ID_Type;
ftpy_ConstantType Py_TT_MAC_ID_ConstantType;
#define TT_MAC_ID_CONST(name) DEF_CONST(name, TT_MAC_ID)
static constant_def TT_MAC_ID_constants[] = {
    TT_MAC_ID_CONST(ROMAN),
    TT_MAC_ID_CONST(JAPANESE),
    TT_MAC_ID_CONST(TRADITIONAL_CHINESE),
    TT_MAC_ID_CONST(KOREAN),
    TT_MAC_ID_CONST(ARABIC),
    TT_MAC_ID_CONST(HEBREW),
    TT_MAC_ID_CONST(GREEK),
    TT_MAC_ID_CONST(RUSSIAN),
    TT_MAC_ID_CONST(RSYMBOL),
    TT_MAC_ID_CONST(DEVANAGARI),
    TT_MAC_ID_CONST(GURMUKHI),
    TT_MAC_ID_CONST(GUJARATI),
    TT_MAC_ID_CONST(ORIYA),
    TT_MAC_ID_CONST(BENGALI),
    TT_MAC_ID_CONST(TAMIL),
    TT_MAC_ID_CONST(TELUGU),
    TT_MAC_ID_CONST(KANNADA),
    TT_MAC_ID_CONST(MALAYALAM),
    TT_MAC_ID_CONST(SINHALESE),
    TT_MAC_ID_CONST(BURMESE),
    TT_MAC_ID_CONST(KHMER),
    TT_MAC_ID_CONST(THAI),
    TT_MAC_ID_CONST(LAOTIAN),
    TT_MAC_ID_CONST(GEORGIAN),
    TT_MAC_ID_CONST(ARMENIAN),
    TT_MAC_ID_CONST(MALDIVIAN),
    TT_MAC_ID_CONST(SIMPLIFIED_CHINESE),
    TT_MAC_ID_CONST(TIBETAN),
    TT_MAC_ID_CONST(MONGOLIAN),
    TT_MAC_ID_CONST(GEEZ),
    TT_MAC_ID_CONST(SLAVIC),
    TT_MAC_ID_CONST(VIETNAMESE),
    TT_MAC_ID_CONST(SINDHI),
    TT_MAC_ID_CONST(UNINTERP),
    {NULL}
};


/* TT_ISO_ID is not included, since their use is deprecated. */


static PyTypeObject Py_TT_MS_ID_Type;
ftpy_ConstantType Py_TT_MS_ID_ConstantType;
#define TT_MS_ID_CONST(name) DEF_CONST(name, TT_MS_ID)
static constant_def TT_MS_ID_constants[] = {
    TT_MS_ID_CONST(SYMBOL_CS),
    TT_MS_ID_CONST(UNICODE_CS),
    TT_MS_ID_CONST(SJIS),
    TT_MS_ID_CONST(GB2312),
    TT_MS_ID_CONST(BIG_5),
    TT_MS_ID_CONST(WANSUNG),
    TT_MS_ID_CONST(JOHAB),
    TT_MS_ID_CONST(UCS_4),
    {NULL}
};


static PyTypeObject Py_TT_ADOBE_ID_Type;
ftpy_ConstantType Py_TT_ADOBE_ID_ConstantType;
#define TT_ADOBE_ID_CONST(name) DEF_CONST(name, TT_ADOBE_ID)
static constant_def TT_ADOBE_ID_constants[] = {
    TT_ADOBE_ID_CONST(STANDARD),
    TT_ADOBE_ID_CONST(EXPERT),
    TT_ADOBE_ID_CONST(CUSTOM),
    TT_ADOBE_ID_CONST(LATIN_1),
    {NULL}
};


static PyTypeObject Py_TT_MAC_LANGID_Type;
ftpy_ConstantType Py_TT_MAC_LANGID_ConstantType;
#define TT_MAC_LANGID_CONST(name) DEF_CONST(name, TT_MAC_LANGID)
static constant_def TT_MAC_LANGID_constants[] = {
    TT_MAC_LANGID_CONST(ENGLISH),
    TT_MAC_LANGID_CONST(FRENCH),
    TT_MAC_LANGID_CONST(GERMAN),
    TT_MAC_LANGID_CONST(ITALIAN),
    TT_MAC_LANGID_CONST(DUTCH),
    TT_MAC_LANGID_CONST(SWEDISH),
    TT_MAC_LANGID_CONST(SPANISH),
    TT_MAC_LANGID_CONST(DANISH),
    TT_MAC_LANGID_CONST(PORTUGUESE),
    TT_MAC_LANGID_CONST(NORWEGIAN),
    TT_MAC_LANGID_CONST(HEBREW),
    TT_MAC_LANGID_CONST(JAPANESE),
    TT_MAC_LANGID_CONST(ARABIC),
    TT_MAC_LANGID_CONST(FINNISH),
    TT_MAC_LANGID_CONST(GREEK),
    TT_MAC_LANGID_CONST(ICELANDIC),
    TT_MAC_LANGID_CONST(MALTESE),
    TT_MAC_LANGID_CONST(TURKISH),
    TT_MAC_LANGID_CONST(CROATIAN),
    TT_MAC_LANGID_CONST(CHINESE_TRADITIONAL),
    TT_MAC_LANGID_CONST(URDU),
    TT_MAC_LANGID_CONST(HINDI),
    TT_MAC_LANGID_CONST(THAI),
    TT_MAC_LANGID_CONST(KOREAN),
    TT_MAC_LANGID_CONST(LITHUANIAN),
    TT_MAC_LANGID_CONST(POLISH),
    TT_MAC_LANGID_CONST(HUNGARIAN),
    TT_MAC_LANGID_CONST(ESTONIAN),
    TT_MAC_LANGID_CONST(LETTISH),
    TT_MAC_LANGID_CONST(SAAMISK),
    TT_MAC_LANGID_CONST(FAEROESE),
    TT_MAC_LANGID_CONST(FARSI),
    TT_MAC_LANGID_CONST(RUSSIAN),
    TT_MAC_LANGID_CONST(CHINESE_SIMPLIFIED),
    TT_MAC_LANGID_CONST(FLEMISH),
    TT_MAC_LANGID_CONST(IRISH),
    TT_MAC_LANGID_CONST(ALBANIAN),
    TT_MAC_LANGID_CONST(ROMANIAN),
    TT_MAC_LANGID_CONST(CZECH),
    TT_MAC_LANGID_CONST(SLOVAK),
    TT_MAC_LANGID_CONST(SLOVENIAN),
    TT_MAC_LANGID_CONST(YIDDISH),
    TT_MAC_LANGID_CONST(SERBIAN),
    TT_MAC_LANGID_CONST(MACEDONIAN),
    TT_MAC_LANGID_CONST(BULGARIAN),
    TT_MAC_LANGID_CONST(UKRAINIAN),
    TT_MAC_LANGID_CONST(BYELORUSSIAN),
    TT_MAC_LANGID_CONST(UZBEK),
    TT_MAC_LANGID_CONST(KAZAKH),
    TT_MAC_LANGID_CONST(AZERBAIJANI),
    TT_MAC_LANGID_CONST(AZERBAIJANI_CYRILLIC_SCRIPT),
    TT_MAC_LANGID_CONST(AZERBAIJANI_ARABIC_SCRIPT),
    TT_MAC_LANGID_CONST(ARMENIAN),
    TT_MAC_LANGID_CONST(GEORGIAN),
    TT_MAC_LANGID_CONST(MOLDAVIAN),
    TT_MAC_LANGID_CONST(KIRGHIZ),
    TT_MAC_LANGID_CONST(TAJIKI),
    TT_MAC_LANGID_CONST(TURKMEN),
    TT_MAC_LANGID_CONST(MONGOLIAN),
    TT_MAC_LANGID_CONST(MONGOLIAN_MONGOLIAN_SCRIPT),
    TT_MAC_LANGID_CONST(MONGOLIAN_CYRILLIC_SCRIPT),
    TT_MAC_LANGID_CONST(PASHTO),
    TT_MAC_LANGID_CONST(KURDISH),
    TT_MAC_LANGID_CONST(KASHMIRI),
    TT_MAC_LANGID_CONST(SINDHI),
    TT_MAC_LANGID_CONST(TIBETAN),
    TT_MAC_LANGID_CONST(NEPALI),
    TT_MAC_LANGID_CONST(SANSKRIT),
    TT_MAC_LANGID_CONST(MARATHI),
    TT_MAC_LANGID_CONST(BENGALI),
    TT_MAC_LANGID_CONST(ASSAMESE),
    TT_MAC_LANGID_CONST(GUJARATI),
    TT_MAC_LANGID_CONST(PUNJABI),
    TT_MAC_LANGID_CONST(ORIYA),
    TT_MAC_LANGID_CONST(MALAYALAM),
    TT_MAC_LANGID_CONST(KANNADA),
    TT_MAC_LANGID_CONST(TAMIL),
    TT_MAC_LANGID_CONST(TELUGU),
    TT_MAC_LANGID_CONST(SINHALESE),
    TT_MAC_LANGID_CONST(BURMESE),
    TT_MAC_LANGID_CONST(KHMER),
    TT_MAC_LANGID_CONST(LAO),
    TT_MAC_LANGID_CONST(VIETNAMESE),
    TT_MAC_LANGID_CONST(INDONESIAN),
    TT_MAC_LANGID_CONST(TAGALOG),
    TT_MAC_LANGID_CONST(MALAY_ROMAN_SCRIPT),
    TT_MAC_LANGID_CONST(MALAY_ARABIC_SCRIPT),
    TT_MAC_LANGID_CONST(AMHARIC),
    TT_MAC_LANGID_CONST(TIGRINYA),
    TT_MAC_LANGID_CONST(GALLA),
    TT_MAC_LANGID_CONST(SOMALI),
    TT_MAC_LANGID_CONST(SWAHILI),
    TT_MAC_LANGID_CONST(RUANDA),
    TT_MAC_LANGID_CONST(RUNDI),
    TT_MAC_LANGID_CONST(CHEWA),
    TT_MAC_LANGID_CONST(MALAGASY),
    TT_MAC_LANGID_CONST(ESPERANTO),
    TT_MAC_LANGID_CONST(WELSH),
    TT_MAC_LANGID_CONST(BASQUE),
    TT_MAC_LANGID_CONST(CATALAN),
    TT_MAC_LANGID_CONST(LATIN),
    TT_MAC_LANGID_CONST(QUECHUA),
    TT_MAC_LANGID_CONST(GUARANI),
    TT_MAC_LANGID_CONST(AYMARA),
    TT_MAC_LANGID_CONST(TATAR),
    TT_MAC_LANGID_CONST(UIGHUR),
    TT_MAC_LANGID_CONST(DZONGKHA),
    TT_MAC_LANGID_CONST(JAVANESE),
    TT_MAC_LANGID_CONST(SUNDANESE),
    TT_MAC_LANGID_CONST(GALICIAN),
    TT_MAC_LANGID_CONST(AFRIKAANS),
    TT_MAC_LANGID_CONST(BRETON),
    TT_MAC_LANGID_CONST(INUKTITUT),
    TT_MAC_LANGID_CONST(SCOTTISH_GAELIC),
    TT_MAC_LANGID_CONST(MANX_GAELIC),
    TT_MAC_LANGID_CONST(IRISH_GAELIC),
    TT_MAC_LANGID_CONST(TONGAN),
    TT_MAC_LANGID_CONST(GREEK_POLYTONIC),
    TT_MAC_LANGID_CONST(GREELANDIC),
    TT_MAC_LANGID_CONST(AZERBAIJANI_ROMAN_SCRIPT),
    {NULL}
};


static PyTypeObject Py_TT_MS_LANGID_Type;
ftpy_ConstantType Py_TT_MS_LANGID_ConstantType;
#define TT_MS_LANGID_CONST(name) DEF_CONST(name, TT_MS_LANGID)
static constant_def TT_MS_LANGID_constants[] = {
    TT_MS_LANGID_CONST(ARABIC_GENERAL),
    TT_MS_LANGID_CONST(ARABIC_SAUDI_ARABIA),
    TT_MS_LANGID_CONST(ARABIC_IRAQ),
    TT_MS_LANGID_CONST(ARABIC_EGYPT),
    TT_MS_LANGID_CONST(ARABIC_LIBYA),
    TT_MS_LANGID_CONST(ARABIC_ALGERIA),
    TT_MS_LANGID_CONST(ARABIC_MOROCCO),
    TT_MS_LANGID_CONST(ARABIC_TUNISIA),
    TT_MS_LANGID_CONST(ARABIC_OMAN),
    TT_MS_LANGID_CONST(ARABIC_YEMEN),
    TT_MS_LANGID_CONST(ARABIC_SYRIA),
    TT_MS_LANGID_CONST(ARABIC_JORDAN),
    TT_MS_LANGID_CONST(ARABIC_LEBANON),
    TT_MS_LANGID_CONST(ARABIC_KUWAIT),
    TT_MS_LANGID_CONST(ARABIC_UAE),
    TT_MS_LANGID_CONST(ARABIC_BAHRAIN),
    TT_MS_LANGID_CONST(ARABIC_QATAR),
    TT_MS_LANGID_CONST(BULGARIAN_BULGARIA),
    TT_MS_LANGID_CONST(CATALAN_SPAIN),
    TT_MS_LANGID_CONST(CHINESE_GENERAL),
    TT_MS_LANGID_CONST(CHINESE_TAIWAN),
    TT_MS_LANGID_CONST(CHINESE_PRC),
    TT_MS_LANGID_CONST(CHINESE_HONG_KONG),
    TT_MS_LANGID_CONST(CHINESE_SINGAPORE),
    TT_MS_LANGID_CONST(CHINESE_MACAU),
    TT_MS_LANGID_CONST(CZECH_CZECH_REPUBLIC),
    TT_MS_LANGID_CONST(DANISH_DENMARK),
    TT_MS_LANGID_CONST(GERMAN_GERMANY),
    TT_MS_LANGID_CONST(GERMAN_SWITZERLAND),
    TT_MS_LANGID_CONST(GERMAN_AUSTRIA),
    TT_MS_LANGID_CONST(GERMAN_LUXEMBOURG),
    TT_MS_LANGID_CONST(GERMAN_LIECHTENSTEI),
    TT_MS_LANGID_CONST(GREEK_GREECE),
    TT_MS_LANGID_CONST(ENGLISH_GENERAL),
    TT_MS_LANGID_CONST(ENGLISH_UNITED_STATES),
    TT_MS_LANGID_CONST(ENGLISH_UNITED_KINGDOM),
    TT_MS_LANGID_CONST(ENGLISH_AUSTRALIA),
    TT_MS_LANGID_CONST(ENGLISH_CANADA),
    TT_MS_LANGID_CONST(ENGLISH_NEW_ZEALAND),
    TT_MS_LANGID_CONST(ENGLISH_IRELAND),
    TT_MS_LANGID_CONST(ENGLISH_SOUTH_AFRICA),
    TT_MS_LANGID_CONST(ENGLISH_JAMAICA),
    TT_MS_LANGID_CONST(ENGLISH_CARIBBEAN),
    TT_MS_LANGID_CONST(ENGLISH_BELIZE),
    TT_MS_LANGID_CONST(ENGLISH_TRINIDAD),
    TT_MS_LANGID_CONST(ENGLISH_ZIMBABWE),
    TT_MS_LANGID_CONST(ENGLISH_PHILIPPINES),
    TT_MS_LANGID_CONST(ENGLISH_INDONESIA),
    TT_MS_LANGID_CONST(ENGLISH_HONG_KONG),
    TT_MS_LANGID_CONST(ENGLISH_INDIA),
    TT_MS_LANGID_CONST(ENGLISH_MALAYSIA),
    TT_MS_LANGID_CONST(ENGLISH_SINGAPORE),
    TT_MS_LANGID_CONST(SPANISH_SPAIN_TRADITIONAL_SORT),
    TT_MS_LANGID_CONST(SPANISH_MEXICO),
    TT_MS_LANGID_CONST(SPANISH_SPAIN_INTERNATIONAL_SORT),
    TT_MS_LANGID_CONST(SPANISH_GUATEMALA),
    TT_MS_LANGID_CONST(SPANISH_COSTA_RICA),
    TT_MS_LANGID_CONST(SPANISH_PANAMA),
    TT_MS_LANGID_CONST(SPANISH_DOMINICAN_REPUBLIC),
    TT_MS_LANGID_CONST(SPANISH_VENEZUELA),
    TT_MS_LANGID_CONST(SPANISH_COLOMBIA),
    TT_MS_LANGID_CONST(SPANISH_PERU),
    TT_MS_LANGID_CONST(SPANISH_ARGENTINA),
    TT_MS_LANGID_CONST(SPANISH_ECUADOR),
    TT_MS_LANGID_CONST(SPANISH_CHILE),
    TT_MS_LANGID_CONST(SPANISH_URUGUAY),
    TT_MS_LANGID_CONST(SPANISH_PARAGUAY),
    TT_MS_LANGID_CONST(SPANISH_BOLIVIA),
    TT_MS_LANGID_CONST(SPANISH_EL_SALVADOR),
    TT_MS_LANGID_CONST(SPANISH_HONDURAS),
    TT_MS_LANGID_CONST(SPANISH_NICARAGUA),
    TT_MS_LANGID_CONST(SPANISH_PUERTO_RICO),
    TT_MS_LANGID_CONST(SPANISH_UNITED_STATES),
    TT_MS_LANGID_CONST(SPANISH_LATIN_AMERICA),
    TT_MS_LANGID_CONST(FINNISH_FINLAND),
    TT_MS_LANGID_CONST(FRENCH_FRANCE),
    TT_MS_LANGID_CONST(FRENCH_BELGIUM),
    TT_MS_LANGID_CONST(FRENCH_CANADA),
    TT_MS_LANGID_CONST(FRENCH_SWITZERLAND),
    TT_MS_LANGID_CONST(FRENCH_LUXEMBOURG),
    TT_MS_LANGID_CONST(FRENCH_MONACO),
    TT_MS_LANGID_CONST(FRENCH_WEST_INDIES),
    TT_MS_LANGID_CONST(FRENCH_REUNION),
    TT_MS_LANGID_CONST(FRENCH_CONGO),
    TT_MS_LANGID_CONST(FRENCH_ZAIRE),
    TT_MS_LANGID_CONST(FRENCH_SENEGAL),
    TT_MS_LANGID_CONST(FRENCH_CAMEROON),
    TT_MS_LANGID_CONST(FRENCH_COTE_D_IVOIRE),
    TT_MS_LANGID_CONST(FRENCH_MALI),
    TT_MS_LANGID_CONST(FRENCH_MOROCCO),
    TT_MS_LANGID_CONST(FRENCH_HAITI),
    TT_MS_LANGID_CONST(FRENCH_NORTH_AFRICA),
    TT_MS_LANGID_CONST(HEBREW_ISRAEL),
    TT_MS_LANGID_CONST(HUNGARIAN_HUNGARY),
    TT_MS_LANGID_CONST(ICELANDIC_ICELAND),
    TT_MS_LANGID_CONST(ITALIAN_ITALY),
    TT_MS_LANGID_CONST(ITALIAN_SWITZERLAND),
    TT_MS_LANGID_CONST(JAPANESE_JAPAN),
    TT_MS_LANGID_CONST(KOREAN_EXTENDED_WANSUNG_KOREA),
    TT_MS_LANGID_CONST(KOREAN_JOHAB_KOREA),
    TT_MS_LANGID_CONST(DUTCH_NETHERLANDS),
    TT_MS_LANGID_CONST(DUTCH_BELGIUM),
    TT_MS_LANGID_CONST(NORWEGIAN_NORWAY_BOKMAL),
    TT_MS_LANGID_CONST(NORWEGIAN_NORWAY_NYNORSK),
    TT_MS_LANGID_CONST(POLISH_POLAND),
    TT_MS_LANGID_CONST(PORTUGUESE_BRAZIL),
    TT_MS_LANGID_CONST(PORTUGUESE_PORTUGAL),
    TT_MS_LANGID_CONST(RHAETO_ROMANIC_SWITZERLAND),
    TT_MS_LANGID_CONST(ROMANIAN_ROMANIA),
    TT_MS_LANGID_CONST(MOLDAVIAN_MOLDAVIA),
    TT_MS_LANGID_CONST(RUSSIAN_RUSSIA),
    TT_MS_LANGID_CONST(RUSSIAN_MOLDAVIA),
    TT_MS_LANGID_CONST(CROATIAN_CROATIA),
    TT_MS_LANGID_CONST(SERBIAN_SERBIA_LATIN),
    TT_MS_LANGID_CONST(SERBIAN_SERBIA_CYRILLIC),
    TT_MS_LANGID_CONST(CROATIAN_BOSNIA_HERZEGOVINA),
    TT_MS_LANGID_CONST(BOSNIAN_BOSNIA_HERZEGOVINA),
    TT_MS_LANGID_CONST(SERBIAN_BOSNIA_HERZ_LATIN),
    TT_MS_LANGID_CONST(SERBIAN_BOSNIA_HERZ_CYRILLIC),
    TT_MS_LANGID_CONST(SLOVAK_SLOVAKIA),
    TT_MS_LANGID_CONST(ALBANIAN_ALBANIA),
    TT_MS_LANGID_CONST(SWEDISH_SWEDEN),
    TT_MS_LANGID_CONST(SWEDISH_FINLAND),
    TT_MS_LANGID_CONST(THAI_THAILAND),
    TT_MS_LANGID_CONST(TURKISH_TURKEY),
    TT_MS_LANGID_CONST(URDU_PAKISTAN),
    TT_MS_LANGID_CONST(URDU_INDIA),
    TT_MS_LANGID_CONST(INDONESIAN_INDONESIA),
    TT_MS_LANGID_CONST(UKRAINIAN_UKRAINE),
    TT_MS_LANGID_CONST(BELARUSIAN_BELARUS),
    TT_MS_LANGID_CONST(SLOVENE_SLOVENIA),
    TT_MS_LANGID_CONST(ESTONIAN_ESTONIA),
    TT_MS_LANGID_CONST(LATVIAN_LATVIA),
    TT_MS_LANGID_CONST(LITHUANIAN_LITHUANIA),
    TT_MS_LANGID_CONST(CLASSIC_LITHUANIAN_LITHUANIA),
    TT_MS_LANGID_CONST(TAJIK_TAJIKISTAN),
    TT_MS_LANGID_CONST(FARSI_IRAN),
    TT_MS_LANGID_CONST(VIETNAMESE_VIET_NAM),
    TT_MS_LANGID_CONST(ARMENIAN_ARMENIA),
    TT_MS_LANGID_CONST(AZERI_AZERBAIJAN_LATIN),
    TT_MS_LANGID_CONST(AZERI_AZERBAIJAN_CYRILLIC),
    TT_MS_LANGID_CONST(BASQUE_SPAIN),
    TT_MS_LANGID_CONST(SORBIAN_GERMANY),
    TT_MS_LANGID_CONST(MACEDONIAN_MACEDONIA),
    TT_MS_LANGID_CONST(SUTU_SOUTH_AFRICA),
    TT_MS_LANGID_CONST(TSONGA_SOUTH_AFRICA),
    TT_MS_LANGID_CONST(TSWANA_SOUTH_AFRICA),
    TT_MS_LANGID_CONST(VENDA_SOUTH_AFRICA),
    TT_MS_LANGID_CONST(XHOSA_SOUTH_AFRICA),
    TT_MS_LANGID_CONST(ZULU_SOUTH_AFRICA),
    TT_MS_LANGID_CONST(AFRIKAANS_SOUTH_AFRICA),
    TT_MS_LANGID_CONST(GEORGIAN_GEORGIA),
    TT_MS_LANGID_CONST(FAEROESE_FAEROE_ISLANDS),
    TT_MS_LANGID_CONST(HINDI_INDIA),
    TT_MS_LANGID_CONST(MALTESE_MALTA),
    TT_MS_LANGID_CONST(SAMI_NORTHERN_NORWAY),
    TT_MS_LANGID_CONST(SAMI_NORTHERN_SWEDEN),
    TT_MS_LANGID_CONST(SAMI_NORTHERN_FINLAND),
    TT_MS_LANGID_CONST(SAMI_LULE_NORWAY),
    TT_MS_LANGID_CONST(SAMI_LULE_SWEDEN),
    TT_MS_LANGID_CONST(SAMI_SOUTHERN_NORWAY),
    TT_MS_LANGID_CONST(SAMI_SOUTHERN_SWEDEN),
    TT_MS_LANGID_CONST(SAMI_SKOLT_FINLAND),
    TT_MS_LANGID_CONST(SAMI_INARI_FINLAND),
    TT_MS_LANGID_CONST(SAAMI_LAPONIA),
    TT_MS_LANGID_CONST(SCOTTISH_GAELIC_UNITED_KINGDOM),
    TT_MS_LANGID_CONST(IRISH_GAELIC_IRELAND),
    TT_MS_LANGID_CONST(YIDDISH_GERMANY),
    TT_MS_LANGID_CONST(MALAY_MALAYSIA),
    TT_MS_LANGID_CONST(MALAY_BRUNEI_DARUSSALAM),
    TT_MS_LANGID_CONST(KAZAK_KAZAKSTAN),
    TT_MS_LANGID_CONST(KIRGHIZ_KIRGHIZSTAN /* Cyrillic*/),
    TT_MS_LANGID_CONST(KIRGHIZ_KIRGHIZ_REPUBLIC),
    TT_MS_LANGID_CONST(SWAHILI_KENYA),
    TT_MS_LANGID_CONST(TURKMEN_TURKMENISTAN),
    TT_MS_LANGID_CONST(UZBEK_UZBEKISTAN_LATIN),
    TT_MS_LANGID_CONST(UZBEK_UZBEKISTAN_CYRILLIC),
    TT_MS_LANGID_CONST(TATAR_TATARSTAN),
    TT_MS_LANGID_CONST(BENGALI_INDIA),
    TT_MS_LANGID_CONST(BENGALI_BANGLADESH),
    TT_MS_LANGID_CONST(PUNJABI_INDIA),
    TT_MS_LANGID_CONST(PUNJABI_ARABIC_PAKISTAN),
    TT_MS_LANGID_CONST(GUJARATI_INDIA),
    TT_MS_LANGID_CONST(ORIYA_INDIA),
    TT_MS_LANGID_CONST(TAMIL_INDIA),
    TT_MS_LANGID_CONST(TELUGU_INDIA),
    TT_MS_LANGID_CONST(KANNADA_INDIA),
    TT_MS_LANGID_CONST(MALAYALAM_INDIA),
    TT_MS_LANGID_CONST(ASSAMESE_INDIA),
    TT_MS_LANGID_CONST(MARATHI_INDIA),
    TT_MS_LANGID_CONST(SANSKRIT_INDIA),
    TT_MS_LANGID_CONST(MONGOLIAN_MONGOLIA),
    TT_MS_LANGID_CONST(MONGOLIAN_MONGOLIA_MONGOLIAN),
    TT_MS_LANGID_CONST(TIBETAN_CHINA),
    TT_MS_LANGID_CONST(TIBETAN_BHUTAN),
    TT_MS_LANGID_CONST(WELSH_WALES),
    TT_MS_LANGID_CONST(KHMER_CAMBODIA),
    TT_MS_LANGID_CONST(LAO_LAOS),
    TT_MS_LANGID_CONST(BURMESE_MYANMAR),
    TT_MS_LANGID_CONST(GALICIAN_SPAIN),
    TT_MS_LANGID_CONST(KONKANI_INDIA),
    TT_MS_LANGID_CONST(MANIPURI_INDIA),
    TT_MS_LANGID_CONST(SINDHI_INDIA),
    TT_MS_LANGID_CONST(SINDHI_PAKISTAN),
    TT_MS_LANGID_CONST(SYRIAC_SYRIA),
    TT_MS_LANGID_CONST(SINHALESE_SRI_LANKA),
    TT_MS_LANGID_CONST(CHEROKEE_UNITED_STATES),
    TT_MS_LANGID_CONST(INUKTITUT_CANADA),
    TT_MS_LANGID_CONST(AMHARIC_ETHIOPIA),
    TT_MS_LANGID_CONST(TAMAZIGHT_MOROCCO),
    TT_MS_LANGID_CONST(TAMAZIGHT_MOROCCO_LATIN),
    TT_MS_LANGID_CONST(KASHMIRI_PAKISTAN),
    TT_MS_LANGID_CONST(KASHMIRI_SASIA),
    TT_MS_LANGID_CONST(KASHMIRI_INDIA),
    TT_MS_LANGID_CONST(NEPALI_NEPAL),
    TT_MS_LANGID_CONST(NEPALI_INDIA),
    TT_MS_LANGID_CONST(FRISIAN_NETHERLANDS),
    TT_MS_LANGID_CONST(PASHTO_AFGHANISTAN),
    TT_MS_LANGID_CONST(FILIPINO_PHILIPPINES),
    TT_MS_LANGID_CONST(DHIVEHI_MALDIVES),
    TT_MS_LANGID_CONST(DIVEHI_MALDIVES),
    TT_MS_LANGID_CONST(EDO_NIGERIA),
    TT_MS_LANGID_CONST(FULFULDE_NIGERIA),
    TT_MS_LANGID_CONST(HAUSA_NIGERIA),
    TT_MS_LANGID_CONST(IBIBIO_NIGERIA),
    TT_MS_LANGID_CONST(YORUBA_NIGERIA),
    TT_MS_LANGID_CONST(QUECHUA_BOLIVIA),
    TT_MS_LANGID_CONST(QUECHUA_ECUADOR),
    TT_MS_LANGID_CONST(QUECHUA_PERU),
    TT_MS_LANGID_CONST(SEPEDI_SOUTH_AFRICA),
    TT_MS_LANGID_CONST(SOTHO_SOUTHERN_SOUTH_AFRICA),
    TT_MS_LANGID_CONST(IGBO_NIGERIA),
    TT_MS_LANGID_CONST(KANURI_NIGERIA),
    TT_MS_LANGID_CONST(OROMO_ETHIOPIA),
    TT_MS_LANGID_CONST(TIGRIGNA_ETHIOPIA),
    TT_MS_LANGID_CONST(TIGRIGNA_ERYTHREA),
    TT_MS_LANGID_CONST(TIGRIGNA_ERYTREA),
    TT_MS_LANGID_CONST(GUARANI_PARAGUAY),
    TT_MS_LANGID_CONST(HAWAIIAN_UNITED_STATES),
    TT_MS_LANGID_CONST(LATIN),
    TT_MS_LANGID_CONST(SOMALI_SOMALIA),
    TT_MS_LANGID_CONST(YI_CHINA),
    TT_MS_LANGID_CONST(PAPIAMENTU_NETHERLANDS_ANTILLES),
    TT_MS_LANGID_CONST(UIGHUR_CHINA),
    TT_MS_LANGID_CONST(MAORI_NEW_ZEALAND),
    {NULL}
};


static PyTypeObject Py_TT_NAME_ID_Type;
ftpy_ConstantType Py_TT_NAME_ID_ConstantType;
#define TT_NAME_ID_CONST(name) DEF_CONST(name, TT_NAME_ID)
static constant_def TT_NAME_ID_constants[] = {
    TT_NAME_ID_CONST(COPYRIGHT),
    TT_NAME_ID_CONST(FONT_FAMILY),
    TT_NAME_ID_CONST(FONT_SUBFAMILY),
    TT_NAME_ID_CONST(UNIQUE_ID),
    TT_NAME_ID_CONST(FULL_NAME),
    TT_NAME_ID_CONST(VERSION_STRING),
    TT_NAME_ID_CONST(PS_NAME),
    TT_NAME_ID_CONST(TRADEMARK),
    TT_NAME_ID_CONST(MANUFACTURER),
    TT_NAME_ID_CONST(DESIGNER),
    TT_NAME_ID_CONST(DESCRIPTION),
    TT_NAME_ID_CONST(VENDOR_URL),
    TT_NAME_ID_CONST(DESIGNER_URL),
    TT_NAME_ID_CONST(LICENSE),
    TT_NAME_ID_CONST(LICENSE_URL),
    TT_NAME_ID_CONST(PREFERRED_FAMILY),
    TT_NAME_ID_CONST(PREFERRED_SUBFAMILY),
    TT_NAME_ID_CONST(MAC_FULL_NAME),
    TT_NAME_ID_CONST(SAMPLE_TEXT),
    TT_NAME_ID_CONST(CID_FINDFONT_NAME),
    TT_NAME_ID_CONST(WWS_FAMILY),
    TT_NAME_ID_CONST(WWS_SUBFAMILY),
    {NULL}
};


int setup_TrueType(PyObject *m)
{
    if (define_constant_namespace(
            m, &Py_TT_PLATFORM_Type, &Py_TT_PLATFORM_ConstantType,
            "freetypy.TT_PLATFORM",
            doc_TT_PLATFORM, TT_PLATFORM_constants) ||

        define_constant_namespace(
            m, &Py_TT_APPLE_ID_Type, &Py_TT_APPLE_ID_ConstantType,
            "freetypy.TT_APPLE_ID",
            doc_TT_APPLE_ID, TT_APPLE_ID_constants) ||

        define_constant_namespace(
            m, &Py_TT_ISO_ID_Type, &Py_TT_ISO_ID_ConstantType,
            "freetypy.TT_ISO_ID",
            doc_TT_ISO_ID, TT_ISO_ID_constants) ||

        define_constant_namespace(
            m, &Py_TT_MAC_ID_Type, &Py_TT_MAC_ID_ConstantType,
            "freetypy.TT_MAC_ID",
            doc_TT_MAC_ID, TT_MAC_ID_constants) ||

        define_constant_namespace(
            m, &Py_TT_MS_ID_Type, &Py_TT_MS_ID_ConstantType,
            "freetypy.TT_MS_ID",
            doc_TT_MS_ID, TT_MS_ID_constants) ||

        define_constant_namespace(
            m, &Py_TT_ADOBE_ID_Type, &Py_TT_ADOBE_ID_ConstantType,
            "freetypy.TT_ADOBE_ID",
            doc_TT_ADOBE_ID, TT_ADOBE_ID_constants) ||

        define_constant_namespace(
            m, &Py_TT_MAC_LANGID_Type, &Py_TT_MAC_LANGID_ConstantType,
            "freetypy.TT_MAC_LANGID",
            doc_TT_MAC_LANGID, TT_MAC_LANGID_constants) ||

        define_constant_namespace(
            m, &Py_TT_MS_LANGID_Type, &Py_TT_MS_LANGID_ConstantType,
            "freetypy.TT_MS_LANGID",
            doc_TT_MS_LANGID, TT_MS_LANGID_constants) ||

        define_constant_namespace(
            m, &Py_TT_NAME_ID_Type, &Py_TT_NAME_ID_ConstantType,
            "freetypy.TT_NAME_ID",
            doc_TT_NAME_ID, TT_NAME_ID_constants)) {

        return -1;
    }

    return 0;
}
