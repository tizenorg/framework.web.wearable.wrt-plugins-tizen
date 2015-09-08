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



#include <map>
#include <utility>
#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <Commons/Exception.h>
#include "plugin_config.h"

#define NFC_FEATURE_API_COMMON "http://tizen.org/privilege/nfc.common"
#define NFC_FEATURE_API_ADMIN "http://tizen.org/privilege/nfc.admin"
#define NFC_FEATURE_API_TAG "http://tizen.org/privilege/nfc.tag"
#define NFC_FEATURE_API_P2P "http://tizen.org/privilege/nfc.p2p"
#define NFC_FEATURE_API_CARD_EMULATION "http://tizen.org/privilege/nfc.cardemulation"

#define NFC_DEVICE_CAP_COMMON "nfc.common"
#define NFC_DEVICE_CAP_ADMIN "nfc.admin"
#define NFC_DEVICE_CAP_TAG "nfc.tag"
#define NFC_DEVICE_CAP_P2P "nfc.p2p"
#define NFC_DEVICE_CAP_CARD_EMULATION "nfc.cardemulation"

namespace DeviceAPI {
namespace NFC {

const char* NFC_FUNCTION_API_COMMON_FUNCS = "NFCCommonFunctions";
const char* NFC_FUNCTION_API_ADMIN_FUNCS = "NFCAdminFunctions";
const char* NFC_FUNCTION_API_TAG_FUNCS = "NFCTagFunctions";
const char* NFC_FUNCTION_API_P2P_FUNCS = "NFCP2PFunctions";
const char* NFC_FUNCTION_API_CARD_EMULATION_FUNCS ="NFCCardEmulationFunctions";

static WrtDeviceApis::Commons::FunctionMapping createNFCFunctions();

static WrtDeviceApis::Commons::FunctionMapping NFCFunctions =
    createNFCFunctions();

DEFINE_FUNCTION_GETTER(NFC, NFCFunctions);

static WrtDeviceApis::Commons::FunctionMapping createNFCFunctions()
{
    using namespace WrtDeviceApis::Commons;

    /**
     * Device capabilities
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_NFC_COMMON, NFC_DEVICE_CAP_COMMON);
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_NFC_ADMIN, NFC_DEVICE_CAP_ADMIN);
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_NFC_TAG, NFC_DEVICE_CAP_TAG);
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_NFC_P2P, NFC_DEVICE_CAP_P2P);
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_NFC_CARD_EMULATION, NFC_DEVICE_CAP_CARD_EMULATION);

    ACE_CREATE_DEVICE_CAPS_LIST(EMPTY_DEVICE_LIST);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_NFC_COMMON);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_NFC_COMMON, DEVICE_CAP_NFC_COMMON);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_NFC_ADMIN);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_NFC_ADMIN, DEVICE_CAP_NFC_ADMIN);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_NFC_TAG);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_NFC_TAG, DEVICE_CAP_NFC_TAG);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_NFC_P2P);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_NFC_P2P, DEVICE_CAP_NFC_P2P);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_NFC_CARD_EMULATION);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_NFC_CARD_EMULATION, DEVICE_CAP_NFC_CARD_EMULATION);

    /**
     * API features
     */
    ACE_CREATE_FEATURE(FEATURE_COMMON, NFC_FEATURE_API_COMMON);
    ACE_CREATE_FEATURE(FEATURE_ADMIN, NFC_FEATURE_API_ADMIN);
    ACE_CREATE_FEATURE(FEATURE_TAG, NFC_FEATURE_API_TAG);
    ACE_CREATE_FEATURE(FEATURE_P2P, NFC_FEATURE_API_P2P);
    ACE_CREATE_FEATURE(FEATURE_CARD_EMULATION, NFC_FEATURE_API_CARD_EMULATION);

    ACE_CREATE_FEATURE_LIST(NFC_FEATURES_NFC_COMMON);
    ACE_ADD_API_FEATURE(NFC_FEATURES_NFC_COMMON, FEATURE_COMMON);

    ACE_CREATE_FEATURE_LIST(NFC_FEATURES_NFC_ADMIN);
    ACE_ADD_API_FEATURE(NFC_FEATURES_NFC_ADMIN, FEATURE_ADMIN);

    ACE_CREATE_FEATURE_LIST(NFC_FEATURES_NFC_TAG);
    ACE_ADD_API_FEATURE(NFC_FEATURES_NFC_TAG, FEATURE_TAG);

    ACE_CREATE_FEATURE_LIST(NFC_FEATURES_NFC_P2P);
    ACE_ADD_API_FEATURE(NFC_FEATURES_NFC_P2P, FEATURE_P2P);

    ACE_CREATE_FEATURE_LIST(NFC_FEATURES_NFC_CARD_EMULATION);
    ACE_ADD_API_FEATURE(NFC_FEATURES_NFC_CARD_EMULATION, FEATURE_CARD_EMULATION);

   /**
     * Functions
     */
    FunctionMapping NFCMapping;

    AceFunction nfcCommonFuncs = ACE_CREATE_FUNCTION(
        FUNCTION_NFC_COMMON_FUNCTIONS,
        NFC_FUNCTION_API_COMMON_FUNCS,
        NFC_FEATURES_NFC_COMMON,
        DEVICE_LIST_NFC_COMMON);

    NFCMapping.insert(std::make_pair(
        NFC_FUNCTION_API_COMMON_FUNCS,
        nfcCommonFuncs));

    AceFunction nfcAdminFuncs = ACE_CREATE_FUNCTION(
        FUNCTION_NFC_ADMIN_FUNCTIONS,
        NFC_FUNCTION_API_ADMIN_FUNCS,
        NFC_FEATURES_NFC_ADMIN,
        DEVICE_LIST_NFC_ADMIN);

    NFCMapping.insert(std::make_pair(
       NFC_FUNCTION_API_ADMIN_FUNCS,
       nfcAdminFuncs));

    AceFunction nfcTagFuncs = ACE_CREATE_FUNCTION(
        FUNCTION_NFC_TAG_FUNCTIONS,
        NFC_FUNCTION_API_TAG_FUNCS,
        NFC_FEATURES_NFC_TAG,
        DEVICE_LIST_NFC_TAG);

    NFCMapping.insert(std::make_pair(
        NFC_FUNCTION_API_TAG_FUNCS,
        nfcTagFuncs));

    AceFunction nfcP2PFuncs = ACE_CREATE_FUNCTION(
        FUNCTION_NFC_P2P_FUNCTIONS,
        NFC_FUNCTION_API_P2P_FUNCS,
        NFC_FEATURES_NFC_P2P,
        DEVICE_LIST_NFC_P2P);

    NFCMapping.insert(std::make_pair(
        NFC_FUNCTION_API_P2P_FUNCS,
        nfcP2PFuncs));

    AceFunction nfcCardEmulationFuncs = ACE_CREATE_FUNCTION(
        FUNCTION_NFC_CARD_EMULATION_FUNCTIONS,
        NFC_FUNCTION_API_CARD_EMULATION_FUNCS,
        NFC_FEATURES_NFC_CARD_EMULATION,
        DEVICE_LIST_NFC_CARD_EMULATION);

    NFCMapping.insert(std::make_pair(
        NFC_FUNCTION_API_CARD_EMULATION_FUNCS,
        nfcCardEmulationFuncs));

    return NFCMapping;
}

} // namespace NFC
} // namespace DeviceAPI

#undef NFC_FEATURE_API_COMMON
#undef NFC_FEATURE_API_ADMIN
#undef NFC_FEATURE_API_TAG
#undef NFC_FEATURE_API_P2P
#undef NFC_FEATURE_API_CARD_EMULATION
#undef NFC_DEVICE_CAP_COMMON
#undef NFC_DEVICE_CAP_ADMIN
#undef NFC_DEVICE_CAP_TAG
#undef NFC_DEVICE_CAP_P2P
#undef NFC_DEVICE_CAP_CARD_EMULATION
