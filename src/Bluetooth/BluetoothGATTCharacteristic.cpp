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
#include "BluetoothGATTCharacteristic.h"
#include <bluetooth.h>
#include <bluetooth_type.h>
#include <Logger.h>
#include "plugin_config_impl.h"
#include "BluetoothConverter.h"
#include <JSUtil.h>
#include "BluetoothLowEnergyAdapter.h"
#include "BluetoothUtil.h"
#include "BluetoothGATTDescriptor.h"

namespace DeviceAPI {
namespace Bluetooth {

int BluetoothGATTCharacteristic::m_listenerId = 0;

BluetoothGATTCharacteristic::BluetoothGATTCharacteristic(
    const BluetoothGattClientWrapper &wrapper, bt_gatt_h _handler, JSContextRef context) :
    BluetoothGATTBase(_handler),
    m_wrapper(wrapper)
{
    int check = bt_gatt_characteristic_foreach_descriptors(m_handle,
        descriptorsNativeCallback, this);
    if(check != BT_ERROR_NONE) {
        LOGE("Descriptors of characteristic couldn't be acquired");
    }

    int ret = bt_gatt_client_set_characteristic_value_changed_cb(m_handle,
            onCharacteristicChangeNativeCallback, static_cast<void*>(&m_pCallbacks));
    if(ret != BT_ERROR_NONE) {
        LOGE("Watch callback couldn't be registred");
    }

    BluetoothConverterFactory::ConverterType converter =
                BluetoothConverterFactory::getConverter(context);

    m_localProperty.setProperty(context,
            BLUETOOTH_GATT_LOW_ENERGY_CHARACTERISTIC_DESCRIPTORS, converter->toJSValueRef(
                m_descriptors));

    int propertyBits = 0;
    int err = bt_gatt_characteristic_get_properties(_handler, &propertyBits);
    if(err != BT_ERROR_NONE) {
        LOGE("Properties of characteristic couldn't be acquired");
    }

    setProperty(propertyBits, BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISBROADCAST,
        BT_GATT_PROPERTY_BROADCAST, context);
    setProperty(propertyBits, BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISREAD,
        BT_GATT_PROPERTY_READ, context);
    setProperty(propertyBits, BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISWRITENORESPONSE,
        BT_GATT_PROPERTY_WRITE_WITHOUT_RESPONSE, context);
    setProperty(propertyBits, BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISWRITE,
        BT_GATT_PROPERTY_WRITE, context);
    setProperty(propertyBits, BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISNOTIFY,
        BT_GATT_PROPERTY_NOTIFY, context);
    setProperty(propertyBits, BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISINDIE,
        BT_GATT_PROPERTY_INDICATE, context);
    setProperty(propertyBits, BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISSIGNEDWRITE,
        BT_GATT_PROPERTY_AUTHENTICATED_SIGNED_WRITES, context);
    setProperty(propertyBits, BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_HASEXTENDED,
        BT_GATT_PROPERTY_EXTENDED_PROPERTIES, context);
}

void BluetoothGATTCharacteristic::setProperty( int _propertyBits, const char* _propertyName,
    bt_gatt_property_e _property, JSContextRef _ctx) {
    LOGD("Entered");
    if( (_propertyBits & _property)  == 0) {
        m_localProperty.setProperty(_ctx,
            _propertyName, false);
    }
    else {
        m_localProperty.setProperty(_ctx,
            _propertyName, true);
    }
}

void BluetoothGATTCharacteristic::onCharacteristicChangeNativeCallback(bt_gatt_h characteristic,
            char *value, int len, void *user_data) {
    LOGD("Entered");
    auto pCallbacks = *static_cast<std::map<int,std::shared_ptr<Common::MultiCallbackUserData>>*>(user_data);
    std::map<int,std::shared_ptr<Common::MultiCallbackUserData>> pCallbacksCpy = pCallbacks;
    for( auto item = pCallbacksCpy.begin(); item != pCallbacksCpy.end(); ++item) {
        std::shared_ptr<Common::MultiCallbackUserData> callback = item->second;
        std::vector<long> data;
        for( int i =0; i < len; ++i) {
            data.push_back(static_cast<long>(value[i]));
        }
        JSValueRef jsResult = Common::JSUtil::toJSValueRef(callback->getContext(), data);
        callback->invokeCallback("onread", jsResult);
    }
}

bool BluetoothGATTCharacteristic::descriptorsNativeCallback(int total, int index, bt_gatt_h gatt_handle,
                    void *user_data) {
    LOGD("Entered");
    char *uuid = NULL;
    bt_gatt_get_uuid(gatt_handle, &uuid);
    LOGD("[descripter [%d / %d] (%s)", index, total, uuid);
    BluetoothGATTCharacteristic* This = static_cast<BluetoothGATTCharacteristic*>(user_data);
    This->m_descriptors.push_back(BluetoothGATTBasePtr(
        new BluetoothGATTDescriptor(This->m_wrapper, gatt_handle)));
    return true;
}

BluetoothGATTCharacteristic::~BluetoothGATTCharacteristic() {
    LOGD("Entered");
    int ret = bt_gatt_client_unset_characteristic_value_changed_cb(m_handle);
    if (ret != BT_ERROR_NONE ) {
        LOGE("Watch callback couldnt be unregistred");
    }
}

JSValueRef BluetoothGATTCharacteristic::getIsBroadCast(JSContextRef context) {

    JSValueRef isBroadCast = m_localProperty.getProperty(context,
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISBROADCAST);
    return isBroadCast;
}

JSValueRef BluetoothGATTCharacteristic::getIsHasExtended(JSContextRef context) {

    JSValueRef hasExtended = m_localProperty.getProperty(context,
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_HASEXTENDED);
    return hasExtended;
}

JSValueRef BluetoothGATTCharacteristic::getIsNotify(JSContextRef context) {

    JSValueRef isNotify = m_localProperty.getProperty(context,
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISNOTIFY);
    return isNotify;
}

JSValueRef BluetoothGATTCharacteristic::getIsIndie(JSContextRef context) {

    JSValueRef isIndie = m_localProperty.getProperty(context,
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISINDIE);
    return isIndie;
}

JSValueRef BluetoothGATTCharacteristic::getIsRead(JSContextRef context) {

    JSValueRef isRead = m_localProperty.getProperty(context,
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISREAD);
    return isRead;
}

JSValueRef BluetoothGATTCharacteristic::getIsSignedWrite(JSContextRef context) {

    JSValueRef isSignedWrite = m_localProperty.getProperty(context,
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISSIGNEDWRITE);
    return isSignedWrite;
}

JSValueRef BluetoothGATTCharacteristic::getIsWrite(JSContextRef context) {
    JSValueRef isWrite = m_localProperty.getProperty(context,
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISWRITE);
    return isWrite;
}

JSValueRef BluetoothGATTCharacteristic::getIsWriteNoResponse(JSContextRef context) {

    JSValueRef isWriteNoResponse = m_localProperty.getProperty(context,
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISWRITENORESPONSE);
    return isWriteNoResponse;
}

JSValueRef BluetoothGATTCharacteristic::getDescriptors(JSContextRef context) {
    JSValueRef descriptors = m_localProperty.getProperty(context,
        BLUETOOTH_GATT_LOW_ENERGY_CHARACTERISTIC_DESCRIPTORS);
    return descriptors;
}

long BluetoothGATTCharacteristic::addValueChangeListener(std::shared_ptr<Common::MultiCallbackUserData> _pCallback) {
    LOGD("Entered");
    m_pCallbacks.insert(std::make_pair(m_listenerId,_pCallback));
    ++m_listenerId;
    return m_listenerId;
}

void BluetoothGATTCharacteristic::removeValueChangeListener(long _id) {
    LOGD("Entered");
    auto pCallbackIt = m_pCallbacks.find(_id);
    if(pCallbackIt != m_pCallbacks.end()){
        m_pCallbacks.erase(pCallbackIt);
    }
}

} // namespace Bluetooth
} // namespace DeviceAPI

