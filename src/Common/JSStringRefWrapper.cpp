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

#include "JSStringRefWrapper.h"

namespace DeviceAPI {
namespace Common {

JSStringRefWrapper::JSStringRefWrapper(JSStringRefWrapper &obj) : m_ref(NULL)
{
	JSStringRef ref = obj.m_ref;
	if(ref)
	{
		const JSChar* chars = JSStringGetCharactersPtr(ref);
		size_t numChars = JSStringGetLength(ref);

		m_ref = JSStringCreateWithCharacters(chars, numChars);
	}
}

JSStringRefWrapper::JSStringRefWrapper(JSStringRef ref) : m_ref(ref)
{
}

JSStringRefWrapper::JSStringRefWrapper(const std::string str)
{
	m_ref = JSStringCreateWithUTF8CString(str.c_str());
}

JSStringRefWrapper::JSStringRefWrapper(const char * str)
{
	m_ref = JSStringCreateWithUTF8CString(str);
}

JSStringRefWrapper::~JSStringRefWrapper()
{
	if (m_ref != NULL)
		JSStringRelease(m_ref);
}

JSStringRefWrapper & JSStringRefWrapper::operator=(const JSStringRefWrapper &obj)
{
	JSStringRef ref = obj.m_ref;

	if (m_ref != NULL)
		JSStringRelease(m_ref);

	if(ref)
	{
		const JSChar* chars = JSStringGetCharactersPtr(ref);
		size_t numChars = JSStringGetLength(ref);

		m_ref = JSStringCreateWithCharacters(chars, numChars);
	}
	else
	{
		m_ref = NULL;
	}

	return *this;
}

JSStringRef JSStringRefWrapper::get() const
{
	return m_ref;
}

} // Common
} // DeviceAPI
