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


#ifndef _NFCCALLBACKDATA_H_
#define _NFCCALLBACKDATA_H_

#include <CallbackUserData.h>
#include <string>
#include <memory>
#include <glib.h>
#include "NFCUtil.h"
#include "NdefMessage.h"
#include "AIDData.h"
#include "HCEEventData.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace NFC {

class OperationCallbackData: public Common::CallbackUserData
{
public:
    OperationCallbackData(JSContextRef global_ctx);
    virtual ~OperationCallbackData();

    void setError(const std::string &err_name,
            const std::string &err_message);
    bool isError() const;
    std::string getErrorName() const;
    std::string getErrorMessage() const;

private:
    bool m_is_error;
    std::string m_err_name;
    std::string m_err_message;
};

class AdapterCallbackData : public OperationCallbackData
{
public:
    AdapterCallbackData(JSContextRef global_ctx, bool powered_state);
    virtual ~AdapterCallbackData();

    bool getPoweredState() const;

private:
    bool m_powered_state;

};

class NdefMessageCallbackData : public OperationCallbackData
{
public:
    NdefMessageCallbackData(JSContextRef global_ctx);
    virtual ~NdefMessageCallbackData();

    void setMessage(NdefMessagePtr message);
    NdefMessagePtr getMessage() const;

private:
    NdefMessagePtr m_ndef_message;
};

class ByteArraySuccessCallback : public OperationCallbackData
{
public:
    ByteArraySuccessCallback(JSContextRef global_ctx);
    virtual ~ByteArraySuccessCallback();

    void setInputData(const UCharVector& byte_array);
    UCharVector getInputData() const;

    void setOutputData(const UCharVector& byte_array);
    UCharVector getOutputData() const;

private:
    UCharVector m_input_data;
    UCharVector m_output_data;
};

class CardEmulationModeChangeCallback : public OperationCallbackData
{
public:
    CardEmulationModeChangeCallback(JSContextRef global_ctx);
    virtual ~CardEmulationModeChangeCallback();

    void setCardEmulationMode(nfc_se_card_emulation_mode_type_e mode);
    nfc_se_card_emulation_mode_type_e getCardEmulationMode() const;

    static gboolean complete(void *data);

private:
    nfc_se_card_emulation_mode_type_e m_mode;
};

typedef std::shared_ptr<CardEmulationModeChangeCallback>
    CardEmulationModeChangeCallbackPtr;

class TransactionEventCallback : public OperationCallbackData
{
public:
    TransactionEventCallback(JSContextRef global_ctx, nfc_se_type_e type);
    virtual ~TransactionEventCallback();

    void setAppletId(const UCharVector& applet_id);
    UCharVector getAppletId() const;

    void setData(const UCharVector& data);
    UCharVector getData() const;

    static gboolean complete(void *data);
private:
    nfc_se_type_e m_type;
    UCharVector m_applet_id;
    UCharVector m_data;
};

typedef std::shared_ptr<TransactionEventCallback>
    TransactionEventCallbackPtr;

class ActiveSecureElementChangeCallback : public OperationCallbackData
{
public:
    ActiveSecureElementChangeCallback(JSContextRef global_ctx);
    virtual ~ActiveSecureElementChangeCallback();

    void setActiveSecureElement(nfc_se_type_e type);
    nfc_se_type_e getActiveSecureElement() const;

    static gboolean complete(void *data);
private:
    nfc_se_type_e m_type;
};

typedef std::shared_ptr<ActiveSecureElementChangeCallback>
    ActiveSecureElementChangeCallbackPtr;

class HCEEventReceiveCallback
{
public:
    HCEEventReceiveCallback(JSContextRef global_ctx , JSObjectRef successCallbackObj );
    virtual ~HCEEventReceiveCallback();
    void onsuccess(JSValueRef jsResult);
    JSContextRef getContext();
    void setHCEEventData(HceEventDataPtr data);
    HceEventDataPtr getHCEEventData() const;
    static gboolean complete(void *data);
private:
    JSContextRef mContext;
    MultiCallbackUserData *mCallbackUserData;
    HceEventDataPtr m_data;
};

typedef std::shared_ptr<HCEEventReceiveCallback> HCEEventReceiveCallbackPtr;

class getAidsForCategoryCallback
{
public:
    getAidsForCategoryCallback(JSContextRef context, JSObjectRef successCallbackObj, JSObjectRef errorCallbackObj);
    virtual ~getAidsForCategoryCallback();
    void onsuccess(JSValueRef jsResult);
    void onerror(JSValueRef errorObj);
    JSContextRef getContext();
    static void RegisteredAidCallback(nfc_se_type_e se_type, const char *aid, bool read_only, void *data);
    static gboolean invokeRegisteredAidSuccessCB(void *data);
    void setCount(int count);
private:
    JSContextRef mContext;
    MultiCallbackUserData *mCallbackUserData;
    std::vector<AidDataPtr> mAidDataArray;
    int mCount;
};

typedef std::shared_ptr<getAidsForCategoryCallback> getAidsForCategoryCallbackPtr;

struct getAidsForCategoryCallbackHolder {
    getAidsForCategoryCallbackPtr ptr;
};
} // NFC
} // DeviceAPI

#endif // _NFCCALLBACKDATA_H_
