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

#include "BluetoothLEAdvertiseData.h"
#include <Logger.h>
#include <JSUtil.h>
#include "plugin_config_impl.h"

namespace DeviceAPI {
namespace Bluetooth {

BluetoothLEAdvertiseData::BluetoothLEAdvertiseData() : m_includeName(false), m_includeTxPowerLevel(false) {
    // TODO Auto-generated constructor stub
    m_serviceData = new BluetoothLEServiceData();
    m_manufacturerData = new BluetoothLEManufacturerData();
}

BluetoothLEAdvertiseData::~BluetoothLEAdvertiseData() {
    // TODO Auto-generated destructor stub
}

void BluetoothLEAdvertiseData::setName(const bool includeName)
{
    m_includeName = includeName;
}

bool BluetoothLEAdvertiseData::getName() const
{
    return m_includeName;
}

void BluetoothLEAdvertiseData::setServiceUuids(
    const std::vector<std::string> &uuids)
{
    m_uuids = uuids;
}

std::vector<std::string> BluetoothLEAdvertiseData::getServiceUuids() const
{
    return m_uuids;
}

void BluetoothLEAdvertiseData::setSolicitationUuids(
    const std::vector<std::string> &solicitationeuuids)
{
    m_solicitationeuuids = solicitationeuuids;
}

std::vector<std::string> BluetoothLEAdvertiseData::getSolicitationUuids() const
{
    return m_solicitationeuuids;
}

void BluetoothLEAdvertiseData::setAppearance(const unsigned long appearance)
{
    m_appearance = appearance;
}

unsigned long BluetoothLEAdvertiseData::getAppearance() const
{
    return m_appearance;
}

void BluetoothLEAdvertiseData::setTxPowerLevel(const bool includeTxPowerLevel)
{
    m_includeTxPowerLevel = includeTxPowerLevel;
}

bool BluetoothLEAdvertiseData::getTxPowerLevel() const
{
    return m_includeTxPowerLevel;
}

BluetoothLEServiceData* BluetoothLEAdvertiseData::getServiceData() const
{
    return m_serviceData;
}

void BluetoothLEAdvertiseData::setServiceData(BluetoothLEServiceData* serviceData)
{
    m_serviceData = serviceData;
}

JSValueRef BluetoothLEAdvertiseData::getServiceDataJSValueRef(JSContextRef ctx) const {
    LOGD("Entered");
    JSObjectRef jsobject = JSObjectMake(ctx, NULL, NULL );
    if (!m_serviceData) {
        LOGD("m_serviceData is null");
        return jsobject;
    }
    Common::JSUtil::setProperty(ctx , jsobject, BLUETOOTH_LE_SERVICE_DATA_UUID,
        Common::JSUtil::toJSValueRef(ctx, m_serviceData->getUuid()), kJSPropertyAttributeNone, NULL);
    Common::JSUtil::setProperty(ctx , jsobject, BLUETOOTH_LE_SERVICE_DATA_DATA,
        Common::JSUtil::toJSValueRef(ctx, m_serviceData->getData()), kJSPropertyAttributeNone, NULL);
    return jsobject;
}

void BluetoothLEAdvertiseData::setManufacturerData(BluetoothLEManufacturerData* manufacturerData)
{
    m_manufacturerData = manufacturerData;
}

BluetoothLEManufacturerData* BluetoothLEAdvertiseData::getManufacturerData() const
{
    return m_manufacturerData;
}

JSValueRef BluetoothLEAdvertiseData::getManufacturerJSValueRef(JSContextRef ctx) const {
    LOGD("Entered");
    JSObjectRef jsobject = JSObjectMake(ctx, NULL, NULL );
    if (!m_manufacturerData) {
        LOGD("m_manufacturerData is null");
        return jsobject;
    }
    Common::JSUtil::setProperty(ctx , jsobject, BLUETOOTH_LE_MANUFACTURER_DATA_ID,
        Common::JSUtil::toJSValueRef(ctx, m_manufacturerData->getId()), kJSPropertyAttributeNone, NULL);
    Common::JSUtil::setProperty(ctx , jsobject, BLUETOOTH_LE_MANUFACTURER_DATA_DATA,
        Common::JSUtil::toJSValueRef(ctx, m_manufacturerData->getData()), kJSPropertyAttributeNone, NULL);
    return jsobject;
}

void BluetoothLEAdvertiseData::setAppearanceNull(bool value) {
    m_appearance_null = value;
}

bool BluetoothLEAdvertiseData::getAppearanceNull() {
    return m_appearance_null;
}

void BluetoothLEAdvertiseData::setUuidsNull(bool value) {
    m_uuids_null = value;
}

bool BluetoothLEAdvertiseData::getUuidsNull() {
    return m_uuids_null;
}

void BluetoothLEAdvertiseData::setSolicitationUuidsNull(bool value) {
    m_solicitationuuids_null = value;
}

bool BluetoothLEAdvertiseData::getSolicitationUuidsNull() {
    return m_solicitationuuids_null;
}

void BluetoothLEAdvertiseData::setServiceDataNull(bool value) {
    m_serviceData_null = value;
}

bool BluetoothLEAdvertiseData::getServiceDataNull() {
    return m_serviceData_null;
}

void BluetoothLEAdvertiseData::setManufacturerDataNull(bool value) {
    m_manufacturerData_null = value;
}

bool BluetoothLEAdvertiseData::getManufacturerDataNull() {
    return m_manufacturerData_null;
}

} // namespace Bluetooth
} // namespace DeviceAPI

