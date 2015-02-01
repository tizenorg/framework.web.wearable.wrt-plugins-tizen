//
// Tizen Web Device API
// Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
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

#include "AbstractFilter.h"
#include "JSAttributeFilter.h"
#include "JSAttributeRangeFilter.h"
#include "JSCompositeFilter.h"
#include <PlatformException.h>
#include <Logger.h>
#include <JSUtil.h>
#include <algorithm>

namespace DeviceAPI {
namespace Tizen {

using namespace Common;

AbstractFilter::AbstractFilter(FilterType filter_type) :
        m_filter_type(filter_type)
{
}

AbstractFilter::~AbstractFilter()
{
}

FilterType AbstractFilter::getFilterType() const
{
    return m_filter_type;
}

JSValueRef AbstractFilter::makeJSValue(JSContextRef context,
        AbstractFilterPtr native)
{
    LOGD("Entered");

    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    switch (native->getFilterType()) {
        case ATTRIBUTE_FILTER:
            return JSAttributeFilter::makeJSObject(context, native);
        case ATTRIBUTE_RANGE_FILTER:
            return JSAttributeRangeFilter::makeJSObject(context, native);
        case COMPOSITE_FILTER:
            return JSCompositeFilter::makeJSObject(context, native);
        default:
            LOGE("Unsupported filter type");
            throw UnknownException("Unsupported filter type");
    }
}

AbstractFilterPtr AbstractFilter::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    JSObjectRef object = JSUtil::JSValueToObject(context, value);

    if (!JSValueIsObjectOfClass(context, value, JSAttributeFilter::getClassRef()) &&
        !JSValueIsObjectOfClass(context, value, JSAttributeRangeFilter::getClassRef()) &&
        !JSValueIsObjectOfClass(context, value, JSCompositeFilter::getClassRef())) {
        LOGE("JSObjectRef:%p - Object type is not JSAttributeFilter, "
                "JSAttributeRangeFilter nor JSCompositeFilter", object);
        throw TypeMismatchException("Object type is not valid filter");
    }

    AbstractFilterHolder* priv =
            static_cast<AbstractFilterHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    if (!(priv->ptr)) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    return priv->ptr;
}


bool AbstractFilter::isMatching(const FilterableObject* const tested_object) const
{
    LOGE("Calling isMatching on AbstractFilter!");
    throw UnknownException("Method not supported");
}

AttributeFilterPtr castToAttributeFilter(AbstractFilterPtr from)
{
    if(ATTRIBUTE_FILTER != from->getFilterType()) {
        LOGE("Trying to get AttributeFilterPtr but filter's type is: %d",
                from->getFilterType());
        return AttributeFilterPtr();
    }

    return std::dynamic_pointer_cast<AttributeFilter>(from);
}

AttributeRangeFilterPtr castToAttributeRangeFilter(AbstractFilterPtr from)
{
    if(ATTRIBUTE_RANGE_FILTER != from->getFilterType()) {
        LOGE("Trying to get AttributeRangeFilterPtr but filter's type is: %d",
                from->getFilterType());
        return AttributeRangeFilterPtr();
    }

    return std::dynamic_pointer_cast<AttributeRangeFilter>(from);
}

CompositeFilterPtr castToCompositeFilter(AbstractFilterPtr from)
{
    if(COMPOSITE_FILTER != from->getFilterType()) {
        LOGE("Trying to get CompositeFilterPtr but filter's type is: %d",
                from->getFilterType());
        return CompositeFilterPtr();
    }

    return std::dynamic_pointer_cast<CompositeFilter>(from);
}

namespace {

inline std::string convertToLowerCase(const std::string& input_string)
{
    std::string output_string = input_string;
    std::transform(output_string.begin(), output_string.end(), output_string.begin(),
            ::tolower);
    return output_string;
}

} // Anonymous namespace

bool FilterUtils::isStringMatching(const std::string& key,
        const std::string& value,
        Tizen::FilterMatchFlag flag)
{
    switch(flag)
    {
        case Tizen::ENDSWITH: {
            if (key.empty()) {
                return false;
            }
            if (key.size() > value.size()) {
                return false;
            }
            std::string lvalue = convertToLowerCase(value);
            std::string lkey = convertToLowerCase(key);
            return lvalue.substr(lvalue.size() - lkey.size(), lkey.size()) == lkey;
        }

        case Tizen::EXACTLY: {
            return key == value;
        }

        case Tizen::STARTSWITH: {
            if (key.empty()) {
                return false;
            }
            if (key.size() > value.size()) {
                return false;
            }
            std::string lvalue = convertToLowerCase(value);
            std::string lkey = convertToLowerCase(key);
            return lvalue.substr(0, lkey.size()) == lkey;
        }

        case Tizen::CONTAINS: {
            if (key.empty()) {
                return false;
            }
            if (key.size() > value.size()) {
                return false;
            }
            std::string lvalue = convertToLowerCase(value);
            std::string lkey = convertToLowerCase(key);
            return lvalue.find(lkey) != std::string::npos;
        }

        default: {
            LOGE("Unknown match flag");
            return false;
        }
    }
}

bool FilterUtils::isAnyStringMatching(const std::string& key,
        const std::vector<std::string>& values,
        Tizen::FilterMatchFlag flag)
{
    for(auto it = values.begin(); it != values.end(); ++it) {
        if(isStringMatching(key,*it,flag)) {
            return true;
        }
    }
    return false;
}

bool FilterUtils::isTimeStampInRange(const time_t& time_stamp,
        Tizen::AnyPtr& initial_value,
        Tizen::AnyPtr& end_value)
{
    time_t from_time = 0;
    time_t to_time = 0;

    bool initial_is_valid_time_value = false;
    if (initial_value && !initial_value->isNullOrUndefined()) {
        try {
            struct tm ftime = *initial_value->toDateTm();
            from_time = mktime(&ftime);
            initial_is_valid_time_value = true;
        }
        catch(...) {
            LOGE("Unknown exception occured during execution of Any::toDateTm()");
        }
    }
    if(!initial_is_valid_time_value) {
        LOGE("initialValue is not Time!");
        throw InvalidValuesException("initialValue is not Time!");
    }

    bool end_is_valid_time_value = false;
    if (end_value && !end_value->isNullOrUndefined()) {
        try {
            struct tm ttime = *end_value->toDateTm();
            to_time = mktime(&ttime);
            end_is_valid_time_value = true;
        }
        catch(...) {
            LOGE("Unknown exception occured during execution of Any::toDateTm()");
        }
    }
    if(end_is_valid_time_value) {
        LOGE("endValue is not Time!");
        throw InvalidValuesException("endValue is not Time!");
    }

    bool is_in_range = FilterUtils::isBetweenTimeRange(time_stamp, from_time, to_time);

    LOGD("%d is%s in time range <%d, %d>", time_stamp, (is_in_range ? "" : " NOT"),
            from_time, to_time);

    return is_in_range;
}

} //Tizen
} //DeviceAPI
