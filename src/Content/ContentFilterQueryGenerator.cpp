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

#include "ContentFilterQueryGenerator.h"
#include <Logger.h>
#include <map>
#include "ContentUtility.h"

using namespace DeviceAPI::Tizen;

namespace DeviceAPI {
namespace Content {

const std::string STR_LEFT_BRACKET = " (";
const std::string STR_RIGHT_BRACKET = ") ";
const std::string STR_AND = " AND ";
const std::string STR_OR = " OR ";
const std::string STR_GREATER_THAN = ">";
const std::string STR_LESS_THAN = "<";
const std::string STR_EQUAL = "=";
const std::string STR_S_QUOTATION = "'";
const std::string STR_PERCENT = "%";

enum condition_e {
    EQUAL,
    NOT_EQUAL,
    GREATER_THAN,
    LESS_THAN,
    GREATER_THAN_OR_EQUAL,
    LESS_THAN_OR_EQUAL,
    LIKE,
    EXISTS,
    CONDITION_MAX
};

static std::string operatorKey[] =
{
        " = ",              //EQUAL
        " NOT IN ",     //NOT_EQUAL
        " > ",              //GREATER_THAN
        " < ",              //LESS_THAN
        " >= ",             //GREATER_THAN_OR_EQUAL
        " <= ",             //LESS_THAN_OR_EQUAL
        " LIKE ",       // like
        " is not null "         //EXISTS
};

//enum media_attribute_e {
//    MEDIA_ID = 0,
//    MEDIA_TITLE,
//    MEDIA_FILEPATH,
//    MEDIA_THUMBNAILPATH,
//    MEDIA_CREATEDDATE,
//    MEDIA_MODIFIEDDATE,
//    MEDIA_DESCRIPTION,
//    MEDIA_RATING
//};

std::map<std::string, std::string> ContentFilterQueryGenerator::attrEnumMap = {
        //media item attribues.
        {"id", "MEDIA_ID"},
        {"type", "MEDIA_TYPE"},
        {"mimeType", "MEDIA_MIME_TYPE"},
        {"name", "MEDIA_DISPLAY_NAME"},
        {"title", "MEDIA_TITLE"},
        {"contentURI", "MEDIA_PATH"},
        {"thumbnailURIs", "MEDIA_THUMBNAIL_PATH"},
        {"description", "MEDIA_DESCRIPTION"},
        {"rating", "MEDIA_RATING"},
        {"createdDate", "MEDIA_ADDED_TIME"},
        {"releaseDate", "MEDIA_DATETAKEN"},
        {"modifiedDate", "MEDIA_MODIFIED_TIME"},
        {"isFavorite", "MEDIA_FAVORITE"},
        //media image,video,audio attribues.
        {"geolocation.latitude", "MEDIA_LATITUDE"},
        {"geolocation.longitude", "MEDIA_LONGITUDE"},
        {"album", "MEDIA_ALBUM"},
        {"artists", "MEDIA_ARTIST"},
        {"width", "MEDIA_WIDTH"},
        {"height", "MEDIA_HEIGHT"},
        {"genres", "MEDIA_GENRE"},
        {"size", "MEDIA_SIZE"}
};

ContentFilterQueryGenerator::ContentFilterQueryGenerator()
{
    m_query = "";
}

ContentFilterQueryGenerator::~ContentFilterQueryGenerator()
{
}

void ContentFilterQueryGenerator::generate(AbstractFilterPtr src_filter)
{
    LOGD("Entered");

    if(!src_filter) {
        LOGW("src_filter is NULL");
    }
    else {
        LOGD("src_filter is %p, type:%d", src_filter.get(), src_filter->getFilterType());
    }

    for(FilterIterator it(src_filter); it.isEnd() == false; ++it) {

        switch(it.getState()) {
            case FIS_COMPOSITE_START: {
                LOGD("COMPOSITE START {");
                m_query.append(STR_LEFT_BRACKET);
            } break;
            case FIS_COMPOSITE_END: {
                LOGD("} COMPOSITE END");
                m_query.append(STR_RIGHT_BRACKET);
            } break;
            case FIS_ATTRIBUTE_FILTER:
            case FIS_ATTRIBUTE_RANGE_FILTER: {

                if(FIS_ATTRIBUTE_FILTER == it.getState()) {
                    LOGD("ATTRIBUTE Filter");
                    visitAttribute(castToAttributeFilter(*it));
                }
                else {
                    LOGD("ATTRIBUTE RANGE Filter");
                    visitAttributeRange(castToAttributeRangeFilter(*it));
                }

                if(it.isInsideCompositeFilter() && !it.isLastCompositeSubFilter()) {
                    CompositeFilterPtr c_filter = it.getCurrentCompositeFilter();

                    if(Tizen::UNION == c_filter->getType()) {
                        m_query.append(STR_OR);
                        LOGD("added OR operator");
                    }
                    else {
                        m_query.append(STR_AND);
                        LOGD("added AND operator");
                    }
                }
            } break;

            default:
                break;
        }
    }
}

std::string ContentFilterQueryGenerator::getResult() const
{
    return m_query;
}

void ContentFilterQueryGenerator::visitAttribute(AttributeFilterPtr attr_filter)
{
    LOGD("Entered");
    if(!attr_filter) {
        LOGW("AttributeFilterPtr is NULL");
        return;
    }

    const AnyPtr& match_value = attr_filter->getMatchValue();
    if (!match_value) {
        LOGW("match_value is NULL");
        return;
    }

    const std::string &attr_name = getPlatformAttr(attr_filter->getAttributeName());
    const Tizen::FilterMatchFlag &match_flag = attr_filter->getMatchFlag();
    m_query.append(convertAttribute(attr_name, match_value, match_flag));
}

void ContentFilterQueryGenerator::visitAttributeRange(
        AttributeRangeFilterPtr attr_range_filter)
{
    LOGD("Entered");
    if(!attr_range_filter) {
        return;
    }

    AnyPtr initial_value = attr_range_filter->getInitialValue();
    AnyPtr end_value = attr_range_filter->getEndValue();
    if(!initial_value || !end_value) {
        LOGD("initial_value or end_value is NULL");
        return;
    }

    const std::string& attr_name = getPlatformAttr(attr_range_filter->getAttributeName());

    std::string str;
    std::string initialValueStr;
    std::string endValueStr;

    if(!initial_value->isNullOrUndefined())
    {
        if(attr_name.compare("MEDIA_DATETAKEN")==0
                || attr_name.compare("MEDIA_MODIFIED_TIME")==0
                || attr_name.compare("MEDIA_ADDED_TIME")==0)
        {
            tm date = *initial_value->toDateTm();
            if(attr_name.compare("MEDIA_DATETAKEN")==0)
            {
                char buf[512];
                strftime(buf, sizeof(buf), "'%Y:%m:%d %H:%M:%S'", &date);
                initialValueStr = buf;
            }
            else
            {
                initialValueStr = toDateDbStr(date);
            }
        }
        else
        {
            initialValueStr = initial_value->toString();
        }
    }

    if (!end_value->isNullOrUndefined())
    {
        if(attr_name.compare("MEDIA_DATETAKEN")==0
                || attr_name.compare("MEDIA_MODIFIED_TIME")==0
                || attr_name.compare("MEDIA_ADDED_TIME")==0)
        {
            tm date = *end_value->toDateTm();
            if(attr_name.compare("MEDIA_DATETAKEN")==0)
            {
                char buf[512];
                strftime(buf, sizeof(buf), "'%Y:%m:%d %H:%M:%S'", &date);
                endValueStr = buf;
            }
            else
            {
                endValueStr = toDateDbStr(date);
            }
        }
        else
        {
            endValueStr = end_value->toString();
        }
    }

    if (!initial_value->isNullOrUndefined() && end_value->isNullOrUndefined())
    {
        str = STR_LEFT_BRACKET + attr_name + STR_GREATER_THAN + STR_EQUAL +
                STR_S_QUOTATION + initialValueStr + STR_S_QUOTATION + STR_RIGHT_BRACKET;
    }
    else if(initial_value->isNullOrUndefined() && !end_value->isNullOrUndefined())
    {
        str = STR_LEFT_BRACKET + attr_name + STR_LESS_THAN + STR_S_QUOTATION +
                endValueStr + STR_S_QUOTATION + STR_RIGHT_BRACKET;
    }
    else if (!initial_value->isNullOrUndefined() && !end_value->isNullOrUndefined())
    {
        str = STR_LEFT_BRACKET + attr_name + STR_GREATER_THAN + STR_EQUAL + STR_S_QUOTATION +
                initialValueStr + STR_S_QUOTATION + STR_AND + attr_name + STR_LESS_THAN + STR_S_QUOTATION +
                endValueStr + STR_S_QUOTATION + STR_RIGHT_BRACKET;
    }

    m_query.append(str);
}





//TODO: the code below should be refactored:

std::string ContentFilterQueryGenerator::getPlatformAttr(std::string attrName)
{
    std::string platformAttr;
    std::map<std::string, std::string>::iterator pos;

    pos = attrEnumMap.find(attrName);

    if(attrEnumMap.end() != pos)
    {
        platformAttr = pos->second;
    }
    else
    {
        std::string msg = "Attribute(";
        msg += attrName + ") is not available.";
        throw Common::UnknownException(msg.c_str());
    }
    return platformAttr;
}

std::string ContentFilterQueryGenerator::toDateDbStr(tm &date) const
{
    time_t time;
    time = mktime(&date);
    std::stringstream ss;
    ss << time;
    return ss.str();
}

std::string ContentFilterQueryGenerator::convertAttribute(const std::string &attrName, const Tizen::AnyPtr& matchValue, const Tizen::FilterMatchFlag& matchFlag)
{
    std::string valueStr;
    std::string operatorStr;
    std::string conditionStr;

    condition_e cond;
    std::string matchValueStr;

    if(attrName.compare("MEDIA_TYPE")==0)
    {
        if(matchValue->toString().compare("IMAGE")==0)
            matchValueStr = "0";
        else if(matchValue->toString().compare("VIDEO")==0)
            matchValueStr = "1";
        else if(matchValue->toString().compare("AUDIO")==0)
            matchValueStr = "3";
        else if(matchValue->toString().compare("OTHER")==0)
            matchValueStr = "4";
        else
        {
            std::string msg = "ContentType(";
            msg += matchValue->toString();
            msg += ") is not available.";
            throw Common::UnknownException(msg.c_str());
        }
    }
    else if(attrName.compare("MEDIA_PATH")==0) {
        std::string uri = matchValue->toString();
        if(ContentUtility::isContentUri(uri)) {
            matchValueStr = ContentUtility::convertUriToPath(uri);
        } else {
            matchValueStr = "";
        }
    }
    else if(attrName.compare("MEDIA_THUMBNAIL_PATH")==0)
    {
        matchValueStr = ContentUtility::convertUriToPath(matchValue->toString());
    }
    else
    {
        matchValueStr = matchValue->toString();
    }

    if(attrName.compare("MEDIA_DATETAKEN")==0
            || attrName.compare("MEDIA_MODIFIED_TIME")==0
            || attrName.compare("MEDIA_ADDED_TIME")==0)
    {
        tm date = *matchValue->toDateTm();
        if(attrName.compare("MEDIA_DATETAKEN")==0)
        {
            char buf[512];
            strftime(buf, sizeof(buf), "'%Y:%m:%d %H:%M:%S'", &date);
            valueStr = buf;
        }
        else
        {
            valueStr = toDateDbStr(date);
        }
        cond = EQUAL;
    }
    else if(attrName.compare("MEDIA_FAVORITE") == 0) {
        valueStr = "0";
        if (matchValue->toBool()) {
            cond = GREATER_THAN;
        }
        else {
            cond = EQUAL;
        }
    }
    else
    {
        //STARTSWITH('%, ENDSWITH, EXISTS
        //valueStr = matchValue->getString();

        std::ostringstream os;

        if(matchFlag == STARTSWITH)
        {
            cond = LIKE;
            os << STR_S_QUOTATION << matchValueStr << STR_PERCENT << STR_S_QUOTATION;
        }
        else if( matchFlag == ENDSWITH )
        {
            cond = LIKE;
            os << STR_S_QUOTATION <<  STR_PERCENT << matchValueStr << STR_S_QUOTATION;
        }
        else if(  matchFlag ==  CONTAINS )
        {
            cond = LIKE;
            os << STR_S_QUOTATION <<  STR_PERCENT << matchValueStr << STR_PERCENT << STR_S_QUOTATION;
        }
        else if( matchFlag ==  Tizen::EXISTS )
        {
            return STR_LEFT_BRACKET + attrName + operatorKey[EXISTS] + STR_RIGHT_BRACKET;
        }
        else
        {
            cond = EQUAL;
            os << STR_S_QUOTATION << matchValueStr << STR_S_QUOTATION;
        }

        valueStr = os.str();
    }
    operatorStr = operatorKey[cond];
    conditionStr = STR_LEFT_BRACKET + attrName + operatorStr + valueStr + STR_RIGHT_BRACKET;

    return conditionStr;
}

} // Content
} // DeviceAPI
