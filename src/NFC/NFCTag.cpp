//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#include "NFCTag.h"
#include "JSUtil.h"
#include "NFCUtil.h"
#include "JSNdefMessage.h"
#include <glib.h>
#include <Logger.h>
#include <JSWebAPIErrorFactory.h>
#include <PlatformException.h>
#include <GlobalContextManager.h>

namespace DeviceAPI {
namespace NFC {

bool NFCTag::transceive_idle = true;

NFCTag::NFCTag(nfc_tag_h handle):m_handle(handle),
        SecurityAccessor()
{
    LOGD("Entered");
}

NFCTag::~NFCTag()
{
    LOGD("Entered");
    m_handle = NULL;
}

nfc_tag_type_e NFCTag::getType()
{
    nfc_tag_type_e type = NFC_UNKNOWN_TARGET;
    int result = nfc_tag_get_type(m_handle, &type);
    if(NFC_ERROR_NONE != result) {
        LOGE("Unknown error while getting tag type: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "Unknown error while getting tag type.");
    }

    return type;
}

std::string NFCTag::getTypeString()
{
    return(NFCUtil::toStringNFCTag(getType()));
}

bool NFCTag::isSupportedNDEF()
{
    bool is_supported_NDEF = false;
    int result = nfc_tag_is_support_ndef(m_handle, &is_supported_NDEF);
    if(NFC_ERROR_NONE != result) {
        LOGE("Unknown error while getting is NDEF supported flag: %d, %s",
            result, NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result,
                "Unknown error while getting is NDEF supported flag.");
    }

    return is_supported_NDEF;
}

long NFCTag::getNdefSize()
{
    unsigned int ndef_size;
    int result = nfc_tag_get_ndef_size(m_handle, &ndef_size);
    if(NFC_ERROR_NONE != result) {
        LOGE("Unknown error while getting is NDEF size: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result,
                "Unknown error while getting is NDEF size.");
    }

    return static_cast<long>(ndef_size);
}

static bool tagPropertyCallback(const char *key,
        const unsigned char *value,
        int value_size,
        void *user_data)
{
    if (user_data) {
        UCharVector tag_info = NFCUtil::toVector(value, value_size);
        (static_cast<NFCTag*>(user_data))->addTagProperty(key, tag_info);
        return true;
    }

    return false;
}

void NFCTag::addTagProperty(const char *key, UCharVector &value)
{
    m_properties.push_back(std::make_pair(key,value));
}

std::vector<PropertyPair> NFCTag::getProperties()
{
    m_properties.clear();
    int result = nfc_tag_foreach_information(m_handle, tagPropertyCallback, this);
    if(NFC_ERROR_NONE != result) {
        LOGE("Unknown error while getting tag's properties: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result,
                "Unknown error while getting tag's properties.");
    }

    return m_properties;
}

bool NFCTag::isConnected()
{
    nfc_tag_h handle = NULL;
    int result = nfc_manager_get_connected_tag(&handle);
    if(NFC_ERROR_NONE != result) {
        LOGE("Unknown error while getting tag's is connected flag: %d, %s",
            result, NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result,
                "Unknown error while getting tag's is connected flag.");
    }

    if(m_handle == handle) {
        return true;
    }

    return false;
}

static gboolean readNDEFCompleteCB(void *data)
{
    LOGD("Entered");

    auto callback = static_cast<NdefMessageCallbackData*>(data);
    if(NULL == callback) {
        LOGE("Callback is null");
        return false;
    }

    JSContextRef context = callback->getContext();
    if (!Common::GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
        LOGE("context was closed");
        delete callback;
        callback = NULL;
        return false;
    }

    try {
        if(callback->isError()) {
            JSObjectRef error_obj = Common::JSWebAPIErrorFactory::makeErrorObject(
                    context,
                    callback->getErrorName(),
                    callback->getErrorMessage());
            callback->callErrorCallback(error_obj);
        }
        else {
            JSValueRef js_value = JSNdefMessage::makeJSObject(context,
                    callback->getMessage());
            callback->callSuccessCallback(js_value);
        }
    } catch (const Common::BasePlatformException &error) {
        LOGE("%s (%s)", (error.getName()).c_str(), (error.getMessage()).c_str());
    } catch (...) {
        LOGE("Unknown error when calling readNDEF.");
    }

    delete callback;
    callback = NULL;
    return false;
}

static void tagReadCallback(nfc_error_e result , nfc_ndef_message_h message , void *data)
{
    LOGD("Entered");

    if(NULL == data) {
        LOGE("Callback is null");
        return;
    }

    auto *callback = static_cast<NdefMessageCallbackData*>(data);

    if(NFC_ERROR_NONE != result) {
        callback->setError(NFCUtil::getNFCErrorString(result),
                NFCUtil::getNFCErrorMessage(result));

        if(0 == g_idle_add(readNDEFCompleteCB, static_cast<void *>(callback))) {
            LOGE("g_idle addition failed");
            delete callback;
            callback = NULL;
        }
        return;
    }

    unsigned char *raw_data = NULL;
    unsigned int size;
    int ret = nfc_ndef_message_get_rawdata(message, &raw_data, &size);
    if(NFC_ERROR_NONE != ret) {
        LOGE("Couldn't get record's raw data: %d, %s", ret,
            NFCUtil::getNFCErrorMessage(ret).c_str());
        callback->setError(Common::JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "Couldn't get record's raw data");

        if(0 == g_idle_add(readNDEFCompleteCB, static_cast<void *>(callback))) {
            LOGE("g_idle addition failed");
            delete callback;
            callback = NULL;
        }

        free(raw_data);
        return;
    }

    NdefMessagePtr ndefmessage = NdefMessagePtr(new NdefMessage(raw_data, size));
    callback->setMessage(ndefmessage);

    if(0 == g_idle_add(readNDEFCompleteCB, static_cast<void *>(callback))) {
        LOGE("g_idle addition failed");
        delete callback;
        callback = NULL;
    }

    free(raw_data);
}

void NFCTag::readNDEF(NdefMessageCallbackData *callback)
{
    LOGD("Entered");

    if(NULL == callback) {
        LOGE("Callback is null");
        throw Common::UnknownException("Callback is null");
    }

    int result = nfc_tag_read_ndef(m_handle, tagReadCallback, static_cast<void *>(callback));
    if(NFC_ERROR_NONE != result) {
        std::string errName = NFCUtil::getNFCErrorString(result);
        std::string errMessage = NFCUtil::getNFCErrorMessage(result);
        LOGE("%s: %s", errName.c_str(), errMessage.c_str());

        JSObjectRef err_obj = Common::JSWebAPIErrorFactory::makeErrorObject(
                callback->getContext(), errName.c_str(), errMessage.c_str());
        callback->callErrorCallback(err_obj);
        delete callback;
        callback = NULL;
    }
}

static gboolean writeNDEFCompleteCB(void* data)
{
    LOGD("Entered");

    NdefMessageCallbackData* callback =
            static_cast<NdefMessageCallbackData*>(data);
    if (!callback) {
        LOGE("Callback is null");
        return false;
    }

    JSContextRef context = callback->getContext();
    if (!Common::GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
        LOGE("context was closed");
        delete callback;
        callback = NULL;
        return false;
    }

    try {
        if (callback->isError()) {
            LOGD("Calling error callback");
            JSObjectRef errobj = Common::JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(),
                    callback->getErrorMessage());
            callback->callErrorCallback(errobj);
        } else {
            LOGD("Calling success callback");
            callback->callSuccessCallback();
        }
    } catch (const Common::BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
    } catch (...) {
        LOGE("Write NDEFMessage failed");
    }

    delete callback;
    callback = NULL;

    return false;
}

void NFCTag::writeNDEF(NdefMessageCallbackData* callback)
{
    LOGD("Entered");

    if(!callback){
        LOGE("Callback is null");
        throw Common::UnknownException("Callback is null");
    }
    NdefMessagePtr ndef_message = callback->getMessage();

    nfc_ndef_message_h message = ndef_message->toStruct();
    if(message){
        int result = nfc_tag_write_ndef(m_handle, message, NULL, NULL);
        //nfc_tag_write_ndef function does not call callback when result is an error
        //it is needed to check it and manually call callback
        NdefRecord::removeMessageHandle(message);
        if (NFC_ERROR_NONE != result){
            std::string errName = NFCUtil::getNFCErrorString(result);
            std::string errMessage = NFCUtil::getNFCErrorMessage(result);
            LOGE("%s: %s", errName.c_str(), errMessage.c_str());
            callback->setError(errName.c_str(), errMessage.c_str());
        }
    } else {
        callback->setError(Common::JSWebAPIErrorFactory::INVALID_VALUES_ERROR,
                "Message is not correct");
    }
    if (!g_idle_add(writeNDEFCompleteCB, static_cast<void*>(callback))) {
        LOGE("g_idle addition failed");
        delete callback;
        callback = NULL;
    }
}

static gboolean transceiveCompleteCB(void* data)
{
    ByteArraySuccessCallback *callback =
        static_cast<ByteArraySuccessCallback*>(data);
    if (NULL == callback) {
        LOGE("Callback is null");
        return false;
    }

    JSContextRef context = callback->getContext();
    if (!Common::GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
        LOGE("context was closed");
        delete callback;
        callback = NULL;
        return false;
    }

    try {
        if (!callback->isError()) {
            callback->callSuccessCallback(Common::JSUtil::toJSValueRef_(
                        context, callback->getOutputData()));
        } else {
            JSObjectRef errobj =
                Common::JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(),
                    callback->getErrorMessage());
            callback->callErrorCallback(errobj);
        }
    }
    catch (const Common::BasePlatformException& err) {
        LOGE("Error while calling transceive callback: %s (%s)",
                (err.getName()).c_str(),(err.getMessage()).c_str());
    }
    catch (...) {
        LOGE("Unknown error when calling transceive callback.");
    }

    delete callback;
    callback = NULL;

    return false;
}

void NFCTag::transceiveCB(nfc_error_e result,
        unsigned char* buffer,
        int buffer_size,
        void* user_data)
{
    // release idle flag
    NFCTag::transceive_idle = true;

    ByteArraySuccessCallback* callback =
        static_cast<ByteArraySuccessCallback*>(user_data);
    if (!callback) {
        LOGE("argument user_data is NULL");
        return;
    }

    if (NFC_ERROR_NONE != result) {
        callback->setError("NFC transceive error",
                NFCUtil::getNFCErrorMessage(result));
    }
    else {
        UCharVector data = NFCUtil::toVector(buffer, buffer_size);
        callback->setOutputData(data);
    }

    transceiveCompleteCB(static_cast<void *>(callback));
}

void NFCTag::transceive(ByteArraySuccessCallback* callback)
{
    if (!callback) {
        LOGE("argument callback is null");
        return;
    }

    // check if transceive is not busy
    if (transceive_idle) {
        transceive_idle = false;

        UCharVector buffer = callback->getInputData();

        int result = nfc_tag_transceive(m_handle, buffer.data(), buffer.size(),
                transceiveCB, static_cast<void*>(callback));

        if (NFC_ERROR_NONE != result) {
            transceive_idle = true;
            LOGE("NFC transceive error: %d, %s", result,
                NFCUtil::getNFCErrorMessage(result).c_str());
            callback->setError("NFC transceive error",
                    NFCUtil::getNFCErrorMessage(result));
        }
    }
    else {
        callback->setError("NFC transceive error",
                NFCUtil::getNFCErrorMessage(NFC_ERROR_DEVICE_BUSY));
    }

    if (callback->isError()) {
        if (0 == g_idle_add(transceiveCompleteCB,
                static_cast<void *>(callback))) {
            LOGE("g_idle addition failed");
            delete callback;
            callback = NULL;
        }
    }
}

} // NFC
} // DeviceAPI
