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

/**
 * @file        IListenerManager.h
 * @version     0.1
 * @brief
 */

#ifndef _TIZEN_COMMONS_ILISTENER_MANAGER_H_
#define _TIZEN_COMMONS_ILISTENER_MANAGER_H_

#include <map>
#include <JavaScriptCore/JavaScript.h>
#include <memory>
#include "Singleton.h"
#include "Export.h"
#include "Logger.h"

namespace DeviceAPI {
namespace Common {

class IListenerItem;
typedef std::shared_ptr<IListenerItem> IListenerItemPtr;

class IListenerItem
{
public:
	IListenerItem(JSContextRef context, JSObjectRef object, long watchId) :
		m_context(context),
		m_object(object),
		m_watchId(watchId)
	{
	}

	virtual ~IListenerItem()
	{
	}

	virtual void protectObject()
	{
		LOGD("Protect object: %p", m_object);

		JSValueProtect(m_context, m_object);
	}

	virtual void unprotectObject()
	{
		LOGD("Unprotect object: %p", m_object);

		JSValueUnprotect(m_context, m_object);
	}

	virtual void cancelListener()
	{
		LOGW("IListenerItem MUST be used as an inherited shape.");
		LOGW("If this log has been printed, it must be used with wrong usage.");
	}

	virtual bool equal(const DeviceAPI::Common::IListenerItemPtr &other) const
	{
		if(!other)
			return false;

		if(m_object == other->m_object && m_watchId == other->m_watchId)
			return true;

		return false;
	}

protected:
	JSContextRef	m_context;
	JSObjectRef		m_object;
	long			m_watchId;
};

class IListenerController
{
public:
	typedef std::multimap<JSContextRef, IListenerItemPtr>   ListenerMap;
	typedef typename ListenerMap::iterator                  ListenerMapIter;
	typedef std::pair<JSContextRef, IListenerItemPtr>       ListenerPair;
	typedef std::pair<ListenerMapIter, ListenerMapIter>     ListenerMapIterPair;

	IListenerController()
	{
	}

	virtual ~IListenerController()
	{
	}

	virtual void registerListener(IListenerItemPtr &canceller, const JSContextRef context)
	{
		LOGD("Registering a listener on context: %p", context);

		canceller->protectObject();
		m_map.insert(ListenerPair(context, canceller));
	}

	virtual void unregisterContext(const JSContextRef context)
	{
		LOGD("Unregistering all listeners on context: %p", context);

		ListenerMapIterPair iterPair = m_map.equal_range(context);

		for(ListenerMapIter i=iterPair.first; i!=iterPair.second; i++)
		{
			LOGD("Unregistering a listener");
			i->second->cancelListener();
			i->second->unprotectObject();
		}

		m_map.erase(context);
	}

	virtual void unregisterListener(const IListenerItemPtr &canceller)
	{
		LOGD("Unregistering a listener");

		for(ListenerMapIter i=m_map.begin(); i!=m_map.end(); i++)
		{
			if(i->second->equal(canceller))
			{
				LOGD("Found object");
				i->second->unprotectObject();

				m_map.erase(i);

				break;
			}
		}
	}

private:
	ListenerMap m_map;
};

} // Common
} // DeviceAPI

#endif // _TIZEN_COMMONS_ILISTENER_MANAGER_H_
