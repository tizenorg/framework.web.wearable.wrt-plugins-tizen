//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//


#ifndef __TIZEN_NFC_NFCUTIL_H_
#define __TIZEN_NFC_NFCUTIL_H_

#include <vector>
#include <string>
#include <nfc.h>

namespace DeviceAPI {
namespace NFC {

namespace {
const std::string GENERIC_TARGET = "GENERIC_TARGET";
const std::string ISO14443_A = "ISO14443_A";
const std::string ISO14443_4A = "ISO14443_4A";
const std::string ISO14443_3A = "ISO14443_3A";
const std::string MIFARE_MINI = "MIFARE_MINI";
const std::string MIFARE_1K = "MIFARE_1K";
const std::string MIFARE_4K = "MIFARE_4K";
const std::string MIFARE_ULTRA = "MIFARE_ULTRA";
const std::string MIFARE_DESFIRE = "MIFARE_DESFIRE";
const std::string ISO14443_B = "ISO14443_B";
const std::string ISO14443_4B = "ISO14443_4B";
const std::string ISO14443_BPRIME  = "ISO14443_BPRIME";
const std::string FELICA  = "FELICA";
const std::string JEWEL  = "JEWEL";
const std::string ISO15693  = "ISO15693";
const std::string UNKNOWN_TARGET = "UNKNOWN_TARGET";

const std::string ALWAYS_ON = "ALWAYS_ON";
const std::string OFF = "OFF";

const std::string ESE = "ESE";
const std::string UICC = "UICC";
const std::string HCE= "HCE";

const std::string PAYMENT = "PAYMENT";
const std::string OTHER= "OTHER";

const std::string DEACTIVATED = "DEACTIVATED";
const std::string ACTIVATED= "ACTIVATED";
const std::string APDU_RECEIVED = "APDU_RECEIVED";
const std::string UNKNOWN_EVENT_RECEIVED = "UNKNOWN_EVENT_RECEIVED";
}

typedef std::vector<unsigned char> UCharVector;

class NFCUtil
{
public:
    static UCharVector toVector(const unsigned char *ch, const int size);
    static void throwNFCException(const int errorCode, const char * message);
    static std::string getNFCErrorString(const int error_code);
    static std::string getNFCErrorMessage(const int error_code);
    static std::string toStringNFCTag(const nfc_tag_type_e tag_type);
    static nfc_tag_type_e toNfcTagString(const std::string& type_string);
    static std::string toStringCardEmulationMode(
        const nfc_se_card_emulation_mode_type_e mode);
    static nfc_se_card_emulation_mode_type_e toCardEmulationMode(
        const std::string& mode_string);
    static std::string toStringSecureElementType(const nfc_se_type_e type);
    static nfc_se_type_e toSecureElementType(const std::string& type_string);
    static void setDefaultFilterValues(std::vector<nfc_tag_type_e>& filter);
    static std::string toStringHceEventType( const nfc_hce_event_type_e type);
    static nfc_card_emulation_category_type_e toCardEmulationCategoryType(
        const std::string& type_string);
};

} // NFC
} // DeviceApi

#endif // __TIZEN_NFC_NFCUTIL_H_
