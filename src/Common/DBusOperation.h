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

#ifndef _DEVICEAPI_COMMON_DBUSOPERATION_H_
#define _DEVICEAPI_COMMON_DBUSOPERATION_H_

#include <dbus/dbus.h>
#include <string>
#include <vector>
#include <set>

namespace DeviceAPI {
namespace Common {

class DBusOperation;

class DBusOperationArguments
{
public:
	DBusOperationArguments();
	virtual ~DBusOperationArguments();

	void addArgumentBool(bool val);
	void addArgumentInt32(int val);
	void addArgumentUint32(unsigned int val);
	void addArgumentUint64(uint64_t val);
	void addArgumentString(std::string val);

private:
	enum ArgType
	{
		TYPE_BOOL, TYPE_INT32, TYPE_UINT32, TYPE_UINT64, TYPE_STRING
	};
	typedef std::pair<ArgType, void*> ArgumentElement;
	typedef std::vector<ArgumentElement> Arguments;

	Arguments m_arguments;

	friend class DBusOperation;

	void appendVariant(DBusMessageIter *busMsgIter);
};

class DBusOperationListener
{
public:
	DBusOperationListener();
	virtual ~DBusOperationListener();

	virtual void OnDBusSignal(int value) = 0;
};

class DBusOperation
{
public:
	DBusOperation(std::string destination, std::string path, std::string interface);
	virtual ~DBusOperation();

	int invokeSyncGetInt(std::string method, DBusOperationArguments *args);

	void registerSignalListener(std::string signalName, DBusOperationListener* listener);
	void unregisterSignalListener(std::string signalName, DBusOperationListener* listener);

private:
	std::string m_destination;
	std::string m_path;
	std::string m_interface;

	typedef std::pair<std::string, DBusOperationListener*> SignalListenerPair;
	typedef std::set<SignalListenerPair> SignalListenerSet;
	SignalListenerSet m_listeners;

	DBusConnection *m_conn;
	std::string m_rule;

	void addDBusSignalFilter();
	void removeDBusSignalFilter();

	DBusHandlerResult dbusSignalFilter(DBusConnection *conn, DBusMessage *message);
	static DBusHandlerResult _dbus_signal_filter(DBusConnection *conn, DBusMessage *message, void *user_data);

	static std::set<DBusOperation *> s_objects;
};

} // Common
} // DeviceAPI

#endif // _DEVICEAPI_COMMON_DBUSOPERATION_H_
