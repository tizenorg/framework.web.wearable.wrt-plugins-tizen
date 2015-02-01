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

#include "DBusOperation.h"

#include <dbus/dbus.h>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <dbus/dbus-glib-lowlevel.h>
#include "Logger.h"
#include "PlatformException.h"

#define DBUS_REPLY_TIMEOUT (-1)

namespace DeviceAPI {
namespace Common {

DBusOperationArguments::DBusOperationArguments()
{
}

DBusOperationArguments::~DBusOperationArguments()
{
    Arguments::iterator iter;
    for (iter = m_arguments.begin(); iter != m_arguments.end(); iter++)
    {
        ArgType type = iter->first;
        void *pVal = iter->second;

        switch (type)
        {
        case ArgType::TYPE_BOOL:
        case ArgType::TYPE_INT32:
            delete static_cast<int32_t *>(pVal);
            break;
        case ArgType::TYPE_UINT32:
            delete static_cast<uint32_t *>(pVal);
            break;
        case ArgType::TYPE_UINT64:
            delete static_cast<uint64_t *>(pVal);
            break;
        case ArgType::TYPE_STRING:
            delete[] static_cast<char *>(pVal);
            break;
        default:
            LoggerE("invalid type");
            break;
        }
    }

    m_arguments.clear();
}

void DBusOperationArguments::addArgumentBool(bool val)
{
    int32_t *pVal = new int32_t;
    *pVal = val;

    ArgumentElement element = ArgumentElement(ArgType::TYPE_BOOL, static_cast<void *>(pVal));

    m_arguments.push_back(element);
}

void DBusOperationArguments::addArgumentInt32(int val)
{
    int32_t *pVal = new int32_t;
    *pVal = val;

    ArgumentElement element = ArgumentElement(ArgType::TYPE_INT32, static_cast<void *>(pVal));

    m_arguments.push_back(element);
}

void DBusOperationArguments::addArgumentUint32(unsigned int val)
{
    uint32_t *pVal = new uint32_t;
    *pVal = val;

    ArgumentElement element = ArgumentElement(ArgType::TYPE_UINT32, static_cast<void *>(pVal));

    m_arguments.push_back(element);
}

void DBusOperationArguments::addArgumentUint64(uint64_t val)
{
    uint64_t *pVal = new uint64_t;
    *pVal = val;

    ArgumentElement element = ArgumentElement(ArgType::TYPE_UINT64, static_cast<void *>(pVal));

    m_arguments.push_back(element);
}

void DBusOperationArguments::addArgumentString(std::string val)
{
    int length = val.length();

    char *pVal = new char[length * 2];
    strncpy(pVal, val.c_str(), length * 2);

    ArgumentElement element = ArgumentElement(ArgType::TYPE_STRING, static_cast<void *>(pVal));

    m_arguments.push_back(element);
}

void DBusOperationArguments::appendVariant(DBusMessageIter *busMsgIter)
{
    Arguments::iterator iter;
    for (iter = m_arguments.begin(); iter != m_arguments.end(); iter++)
    {
        ArgType type = iter->first;
        void *pVal = iter->second;

        switch (type)
        {
        case ArgType::TYPE_BOOL:
            dbus_message_iter_append_basic(busMsgIter, DBUS_TYPE_BOOLEAN, pVal);
            break;
        case ArgType::TYPE_INT32:
            dbus_message_iter_append_basic(busMsgIter, DBUS_TYPE_INT32, pVal);
            break;
        case ArgType::TYPE_UINT32:
            dbus_message_iter_append_basic(busMsgIter, DBUS_TYPE_UINT32, pVal);
            break;
        case ArgType::TYPE_UINT64:
            dbus_message_iter_append_basic(busMsgIter, DBUS_TYPE_UINT64, pVal);
            break;
        case ArgType::TYPE_STRING:
            dbus_message_iter_append_basic(busMsgIter, DBUS_TYPE_STRING, &pVal);
            break;
        default:
            throw new DeviceAPI::Common::UnknownException("Wrong debug parameter type");
        }
    }
}

DBusOperationListener::DBusOperationListener()
{
}

DBusOperationListener::~DBusOperationListener()
{
}

std::set<DBusOperation *> DBusOperation::s_objects;

DBusOperation::DBusOperation(std::string destination, std::string path, std::string interface) :
        m_destination(destination), m_path(path), m_interface(interface), m_conn(NULL)
{
    LoggerD( "Created DBusOperator \"" << destination << "\" \"" << path << "\":\"" << interface << "\"");

    s_objects.insert(this);
}

DBusOperation::~DBusOperation()
{
    if (m_conn)
    {
        dbus_connection_close(m_conn);
        dbus_connection_unref(m_conn);
        m_conn = NULL;
    }

    std::set<DBusOperation *>::iterator iter = s_objects.find(this);
    if (iter != s_objects.end())
        s_objects.erase(iter);
    else
        LoggerE("Object is not existing in the static pool");
}

int DBusOperation::invokeSyncGetInt(std::string method, DBusOperationArguments *args)
{
    DBusMessage *msg;
    DBusMessage *reply;
    DBusMessageIter iter;
    DBusError err;
    int ret, result;

    if (!m_conn)
        m_conn = dbus_bus_get_private(DBUS_BUS_SYSTEM, NULL);

    if (!m_conn)
    {
        LoggerE("dbus_bus_get_private error");
        throw new DeviceAPI::Common::UnknownException("Failed to get dbus connection");
    }

    msg = dbus_message_new_method_call(m_destination.c_str(), m_path.c_str(), m_interface.c_str(), method.c_str());
    if (!msg)
    {
        LoggerE("dbus_message_new_method_call error");
        throw new DeviceAPI::Common::UnknownException("Failed to create dbus message");
    }

    dbus_message_iter_init_append(msg, &iter);
    try
    {
        if (args != NULL)
            args->appendVariant(&iter);
    }
    catch (DeviceAPI::Common::UnknownException &ex)
    {
        LoggerE("append_variant error");
        dbus_message_unref(msg);
        throw new DeviceAPI::Common::UnknownException("Failed to append dbus variable");
    }

    dbus_error_init(&err);
    reply = dbus_connection_send_with_reply_and_block(m_conn, msg, DBUS_REPLY_TIMEOUT, &err);
    dbus_message_unref(msg);
    if (!reply)
    {
        LoggerE("dbus_connection_send_with_reply_and_block error(%s) : \"%s\"", err.name, err.message);
        dbus_error_free(&err);
        throw new DeviceAPI::Common::UnknownException("Failed to send request via dbus");
    }

    ret = dbus_message_get_args(reply, &err, DBUS_TYPE_INT32, &result, DBUS_TYPE_INVALID);
    dbus_message_unref(reply);
    if (!ret)
    {
        LoggerE("dbus_message_get_args error(%s) : \"%s\"", err.name, err.message);
        dbus_error_free(&err);
        throw new DeviceAPI::Common::UnknownException("Failed to get reply from dbus");
    }

    return result;
}

void DBusOperation::registerSignalListener(std::string signalName, DBusOperationListener* listener)
{
    addDBusSignalFilter();

    SignalListenerPair pair(signalName, listener);

    m_listeners.insert(pair);
}

void DBusOperation::unregisterSignalListener(std::string signalName, DBusOperationListener* listener)
{
    LoggerD("unregister signal listener : " << signalName);

    bool signalFound = false;

    SignalListenerSet::iterator iter;
    for (iter = m_listeners.begin(); iter != m_listeners.end(); iter++)
    {
        if (iter->first == signalName && iter->second == listener)
        {
            LoggerD("Find and remove");
            m_listeners.erase(iter);

            signalFound = true;

            break;
        }
    }

    if (signalFound == false)
    {
        LoggerW("Failed to find signal handler");
        throw new DeviceAPI::Common::UnknownException("Failed to find signal handler");
    }

    if (m_listeners.size() == 0)
        removeDBusSignalFilter();
}

void DBusOperation::addDBusSignalFilter()
{
    DBusError err;

    std::stringstream rule;

    if (!m_conn)
        m_conn = dbus_bus_get_private(DBUS_BUS_SYSTEM, NULL);

    if (!m_conn)
    {
        LoggerE("dbus_bus_get_private error");
        throw new DeviceAPI::Common::UnknownException("Failed to get dbus connection");
    }

    dbus_connection_setup_with_g_main(m_conn, NULL);

    rule << "type='signal',sender='" << m_destination << "',path='" << m_path << "',interface='" << m_interface << "'";
    m_rule = rule.str();

    dbus_error_init(&err);
    dbus_bus_add_match(m_conn, m_rule.c_str(), &err);
    if (dbus_error_is_set(&err))
    {
        LoggerE("dbus_bus_add_match error(%s) : \"%s\"", err.name, err.message);
        dbus_error_free(&err);
        throw new DeviceAPI::Common::UnknownException("Failed to set rule for dbus signal");
    }

    if (dbus_connection_add_filter(m_conn, _dbus_signal_filter, this, NULL) == FALSE)
    {
        LoggerE("dbus_bus_add_match error(%s) : \"%s\"", err.name, err.message);
        throw new DeviceAPI::Common::UnknownException("Failed to set handler for dbus signal");
    }
}

void DBusOperation::removeDBusSignalFilter()
{
    DBusError err;

    dbus_error_init(&err);
    dbus_bus_remove_match(m_conn, m_rule.c_str(), &err);
    if (dbus_error_is_set(&err))
    {
        LoggerE("dbus_bus_remove_match error(%s) : \"%s\"", err.name, err.message);
        dbus_error_free(&err);
        throw new DeviceAPI::Common::UnknownException("Failed to remove rule for dbus signal");
    }

    dbus_connection_remove_filter(m_conn, _dbus_signal_filter, this);
}

DBusHandlerResult DBusOperation::dbusSignalFilter(DBusConnection *conn, DBusMessage *message)
{
    int val;
    DBusError err;

    dbus_error_init(&err);
    if (dbus_message_get_args(message, &err, DBUS_TYPE_INT32, &val, DBUS_TYPE_INVALID) == FALSE)
    {
        LoggerE("dbus_message_get_args error(%s) : \"%s\"", err.name, err.message);
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

    SLoggerD("val : " << val);

    SignalListenerSet::iterator iter;
    for (iter = m_listeners.begin(); iter != m_listeners.end(); iter++)
    {
        if (dbus_message_is_signal(message, m_interface.c_str(), iter->first.c_str()))
        {
            LoggerD("find to call : " << iter->first);
            iter->second->OnDBusSignal(val);
        }
    }

    return DBUS_HANDLER_RESULT_HANDLED;
}

DBusHandlerResult DBusOperation::_dbus_signal_filter(DBusConnection *conn, DBusMessage *message, void *user_data)
{
    LoggerD("invoked callback from dbus");

    DBusOperation *thisObj = static_cast<DBusOperation *>(user_data);

    std::set<DBusOperation *>::iterator iter = s_objects.find(thisObj);
    if (iter == s_objects.end())
    {
        LoggerE("Object is not existing in the static pool");
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

    return thisObj->dbusSignalFilter(conn, message);
}

} // Common
} // DeviceAPI
