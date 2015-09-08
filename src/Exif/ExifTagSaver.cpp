//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#include "ExifTagSaver.h"

#include <libexif/exif-format.h>
#include <sstream>

#include <PlatformException.h>

#include "ExifUtil.h"

namespace DeviceAPI {

using namespace DeviceAPI::Common;

namespace Exif {

void ExifTagSaver::removeExifEntryWithTag(const ExifTag tag, ExifData* exif_data)
{
    LOGD("Entered tag:%d (0x%x)", tag, tag);
    ExifEntry* exif_entry = exif_data_get_entry(exif_data, tag);
    if (!exif_entry) {
        LOGD("Exif entry with tag:%d (0x%x) is not present", tag, tag);
        return;
    }

    exif_content_remove_entry(exif_entry->parent, exif_entry);
    LOGD("Removed exif entry with tag:%d (0x%x)", tag, tag);
}

void ExifTagSaver::saveToExif(long int value, ExifTag tag, ExifData* exif_data)
{
    ExifEntry* entry = prepareEntry(exif_data, tag);
    ExifByteOrder order = exif_data_get_byte_order(exif_data);

    switch (entry->format) {
        case EXIF_FORMAT_BYTE:
            entry->data[0] = static_cast<unsigned char>(value);
            break;
        case EXIF_FORMAT_SHORT:
            exif_set_short (entry->data, order, value);
            break;
        case EXIF_FORMAT_LONG:
            exif_set_long (entry->data, order, value);
            break;
        case EXIF_FORMAT_SLONG:
            exif_set_slong (entry->data, order, value);
            break;
        default:
            LOGE("Error: wrong format: %d \n", entry->format );
    }
}

void ExifTagSaver::saveToExif(const std::string& value, ExifTag tag, ExifData* exif_data,
        bool add_zero_character)
{
    LOGD("Entered saving string:[%s] %s to tag: %d(0x%x)", value.c_str(),
            add_zero_character ? "+ \\0" : "",
            tag, tag);

    ExifEntry* entry = prepareEntry(exif_data, tag);
    if (!value.empty()) {

        if (entry->data) {
            free(entry->data);
            entry->data = NULL;
        }

        size_t new_len = value.length();
        if(add_zero_character) {
            ++new_len;
        }

        if(EXIF_TAG_GPS_DATE_STAMP == tag) {
            LOGD("Fixing tag: %d(0x%x) - GPS_DATE_STAMP tag format to ASCII, was:%d (%s)",
                    EXIF_TAG_GPS_DATE_STAMP, EXIF_TAG_GPS_DATE_STAMP,
                    entry->format,
                    exif_format_get_name(entry->format));

            entry->format = EXIF_FORMAT_ASCII;
            if(new_len != 11) {
                LOGW("EXIF_TAG_GPS_DATE_STAMP should be exactly 11 bytes long but is:%d!",
                        new_len);
            }
        }

        entry->size = new_len;
        entry->components = new_len;

        entry->data = static_cast<unsigned char*>(malloc(entry->size));
        if (!(entry->data)) {
            LOGE("malloc returned null");
            throw UnknownException("malloc returned null");
        }
        memcpy(entry->data, value.c_str(), value.length());
        if(add_zero_character) {
            entry->data[value.length()] = '\0';
        }
    }
}

void ExifTagSaver::saveToExif(const Rational& value, ExifTag tag, ExifData* exif_data)
{
    ExifEntry* entry = prepareEntry(exif_data, tag);
    entry->format = EXIF_FORMAT_RATIONAL;

    if (ExifTypeInfo::RationalSize != entry->size) {
        if (entry->data) {
            free(entry->data);
            entry->data = NULL;
        }

        entry->size = ExifTypeInfo::RationalSize;
        entry->data = static_cast<unsigned char*>(malloc(entry->size));
        if (!(entry->data)) {
            LOGE("malloc returned null");
            throw UnknownException("malloc returned null");
        }
        memset(entry->data, 0, entry->size);
    }

    entry->components = 1;

    ExifByteOrder order = exif_data_get_byte_order(exif_data);
    ExifRational r;
    r.numerator = value.nominator;
    r.denominator = value.denominator;
    exif_set_rational(entry->data, order, r);
}

void ExifTagSaver::saveToExif(const Rationals& value, ExifTag tag, ExifData* exif_data)
{
    ExifEntry* entry = prepareEntry(exif_data, tag);
    ExifByteOrder order = exif_data_get_byte_order(exif_data);
    entry->format = EXIF_FORMAT_RATIONAL;

    const unsigned int required_size = ExifTypeInfo::RationalSize * value.size();
    if (required_size != entry->size) {
        if (entry->data) {
            free(entry->data);
            entry->data = NULL;
        }

        entry->size = required_size;
        entry->data = static_cast<unsigned char*>(malloc(entry->size));
        if (!(entry->data)) {
            LOGE("malloc returned null");
            throw UnknownException("malloc returned null");
        }
        memset(entry->data, 0, entry->size);
    }

    entry->components = value.size();
    for(size_t i = 0; i < value.size(); ++i)
    {
        ExifRational r;
        r.numerator = value[i].nominator;
        r.denominator = value[i].denominator;
        exif_set_rational(entry->data + i * ExifTypeInfo::RationalSize, order, r);
    }
}

void ExifTagSaver::saveToExif(std::vector<long long int>& value, ExifFormat store_as,
            ExifTag tag, ExifData* exif_data)
{
    ExifEntry* entry = prepareEntry(exif_data, tag);
    const ExifByteOrder order = exif_data_get_byte_order(exif_data);

    const size_t size_per_member = ExifUtil::getSizeOfExifFormatType(store_as);
    switch (store_as) {
        case EXIF_FORMAT_BYTE:
        case EXIF_FORMAT_SHORT:
        case EXIF_FORMAT_SSHORT:
        case EXIF_FORMAT_LONG:
        case EXIF_FORMAT_SLONG:
            break;
        default:
            LOGE("output ExifFormat: %d is not supported!");
            return;
    }
    entry->format = store_as;

    const size_t num_elements = value.size();
    const unsigned int required_size = size_per_member * num_elements;
    if (required_size != entry->size) {
        if (entry->data) {
            free(entry->data);
            entry->data = NULL;
        }

        entry->size = required_size;
        entry->data = static_cast<unsigned char*>(malloc(entry->size));
        if (!(entry->data)) {
            LOGE("malloc returned null");
            throw UnknownException("malloc returned null");
        }
        memset(entry->data, 0, entry->size);
    }
    entry->components = num_elements;


    switch (store_as) {
        case EXIF_FORMAT_BYTE: {
            for(size_t i = 0; i < num_elements; ++i) {
                entry->data[i] = static_cast<ExifByte>(value[i]);
            }
            break;
        }

        case EXIF_FORMAT_SHORT: {
            for(size_t i = 0; i < num_elements; ++i) {
                exif_set_short(entry->data + i * size_per_member, order,
                        static_cast<ExifShort>(value[i]));
            }
            break;
        }

        case EXIF_FORMAT_SSHORT: {
            for(size_t i = 0; i < num_elements; ++i) {
                exif_set_sshort(entry->data + i * size_per_member, order,
                        static_cast<ExifSShort>(value[i]));
            }
            break;
        }

        case EXIF_FORMAT_LONG: {
            for(size_t i = 0; i < num_elements; ++i) {
                exif_set_long(entry->data + i * size_per_member, order,
                        static_cast<ExifLong>(value[i]));
            }
            break;
        }

        case EXIF_FORMAT_SLONG: {
            for(size_t i = 0; i < num_elements; ++i) {
                exif_set_slong(entry->data + i * size_per_member, order,
                        static_cast<ExifSLong>(value[i]));
            }
            break;
        }
    }


    LOGD("entry after save:");
    ExifUtil::printExifEntryInfo(entry, exif_data);
}

void ExifTagSaver::saveGpsLocationToExif(const ExifGPSLocation& gps_info,
        ExifData* exif_data)
{
    if (gps_info.isSet(EXIF_GPS_LOCATION_ATTRIBUTE_LATITUDE)) {
        auto latitude = gps_info.getLatitude();
        LOGD("Saving latitude: %s", latitude.toDebugString().c_str());
        saveToExif(latitude.toRationalsVector(),
                static_cast<ExifTag>(EXIF_TAG_GPS_LATITUDE), exif_data);
    }

    if (gps_info.isSet(EXIF_GPS_LOCATION_ATTRIBUTE_LATITUDE_REF)) {
        std::string lat_ref =
                (gps_info.getLatitudeRef() == GPS_LOCATION_NORTH) ? "N" : "S";
        LOGD("Saving latitude ref: %s", lat_ref.c_str());
        saveToExif(lat_ref, static_cast<ExifTag>(EXIF_TAG_GPS_LATITUDE_REF), exif_data);
    }

    if (gps_info.isSet(EXIF_GPS_LOCATION_ATTRIBUTE_LONGITUDE)) {

        auto longitude = gps_info.getLongitude();
        LOGD("Saving longitude: %s", longitude.toDebugString().c_str());
        saveToExif(longitude.toRationalsVector(),
                static_cast<ExifTag>(EXIF_TAG_GPS_LONGITUDE), exif_data);
    }

    if (gps_info.isSet(EXIF_GPS_LOCATION_ATTRIBUTE_LONGITUDE_REF)) {
        std::string long_ref =
                (gps_info.getLongitudeRef() == GPS_LOCATION_WEST) ? "W" : "E";
        LOGD("Saving longitude ref: %s", long_ref.c_str());
        saveToExif(long_ref, static_cast<ExifTag>(EXIF_TAG_GPS_LONGITUDE_REF), exif_data);
    }
}

void ExifTagSaver::saveGpsTimeToExif(const ExifGPSTime& gps_time,
            ExifData* exif_data)
{
    if (gps_time.isTimeSet()) {
        const Rationals& time = gps_time.getTime();
        LOGD("Saving gps time: [%s]h [%s]m [%s]d",
                time[0].toString().c_str(),
                time[1].toString().c_str(),
                time[2].toString().c_str());

        saveToExif(time, static_cast<ExifTag>(EXIF_TAG_GPS_TIME_STAMP), exif_data);
    }

    if (gps_time.isDateSet()) {
        std::string date = gps_time.getDate();
        LOGD("Saving gps date: [%s]", date.c_str());

        saveToExif(date, static_cast<ExifTag>(EXIF_TAG_GPS_DATE_STAMP), exif_data);
    }
}

ExifEntry* ExifTagSaver::prepareEntry(ExifData* exif_data, ExifTag tag)
{
    const ExifIfd ifd = deduceIfdSection(tag);
    const ExifFormat format = deduceDataFormat(tag);
    LOGD("Entered m_tag: %d (0x%x) | ifd:%d format:%d (%s)", tag, tag, ifd,
            format, exif_format_get_name(format));

    ExifEntry* exif_entry = exif_data_get_entry(exif_data, tag);
    if (!exif_entry) {
        exif_entry = createNewTag(exif_data, ifd, format, tag );
    } else {
        LOGD("Using existing entry with tag:%d (0x%x) format:%d (%s)",
                exif_entry->tag, exif_entry->tag,
                exif_entry->format, exif_format_get_name(exif_entry->format));
    }

    if (!exif_entry) {
        LOGE("Couldn't create new Exif tag");
        throw UnknownException("Could not save Exif to file");
    }

    exif_entry_initialize(exif_entry, tag);

    if(exif_entry->format != format) {
        LOGW("format should be: %d (%s) is: %d (%s)!",
                format, exif_format_get_name(format),
                exif_entry->format, exif_format_get_name(exif_entry->format));
    }

    return exif_entry;
}

ExifEntry* ExifTagSaver::createNewTag(ExifData* exif_data, ExifIfd ifd,
        ExifFormat format, ExifTag tag)
{
    LOGD("Creating new tag: %d (0x%x) | ifd:%d format:%d (%s)", tag, tag, ifd,
            format, exif_format_get_name(format));

    ExifEntry* new_entry = exif_entry_new();
    if (!new_entry) {
        LOGE("exif_entry_new() returned null");
        throw UnknownException("exif_entry_new() returned null");
    }

    new_entry->tag = tag;
    new_entry->format = format;
    exif_content_add_entry(exif_data->ifd[ifd], new_entry);
    exif_entry_initialize(new_entry, tag);
    return new_entry;
}

ExifIfd ExifTagSaver::deduceIfdSection(ExifTag tag)
{
    switch (static_cast<unsigned int>(tag)) {
        //Tags in IFD_0 Section
        case EXIF_TAG_MAKE:
        case EXIF_TAG_MODEL:
        case EXIF_TAG_IMAGE_WIDTH:
        case EXIF_TAG_IMAGE_LENGTH:
        case EXIF_TAG_ORIENTATION:
            return EXIF_IFD_0;

        //Tags in IFD_EXIF Section
        case EXIF_TAG_USER_COMMENT:
        case EXIF_TAG_DATE_TIME_ORIGINAL:
        case EXIF_TAG_EXPOSURE_TIME:
        case EXIF_TAG_FNUMBER:
        case EXIF_TAG_EXPOSURE_PROGRAM:
        case EXIF_TAG_ISO_SPEED_RATINGS:
        case EXIF_TAG_WHITE_BALANCE:
        case EXIF_TAG_FLASH:
        case EXIF_TAG_FOCAL_LENGTH:
            return EXIF_IFD_EXIF;

        //Tags in IFD_GPS Section
        case EXIF_TAG_GPS_LATITUDE_REF:
        case EXIF_TAG_GPS_LONGITUDE_REF:
        case EXIF_TAG_GPS_LATITUDE:
        case EXIF_TAG_GPS_LONGITUDE:
        case EXIF_TAG_GPS_ALTITUDE:
        case EXIF_TAG_GPS_ALTITUDE_REF:
        case EXIF_TAG_GPS_TIME_STAMP:
        case EXIF_TAG_GPS_PROCESSING_METHOD:
        case EXIF_TAG_GPS_DATE_STAMP:
            return EXIF_IFD_GPS;

        //Tags in other sections
        default:
            LOGE("Unsupported tag: %d", tag);
            throw UnknownException("Unsupported tag");
    }
}

ExifFormat ExifTagSaver::deduceDataFormat(ExifTag tag)
{
    switch (static_cast<unsigned int>(tag)) {
        //Tags with byte type:
        case EXIF_TAG_GPS_ALTITUDE_REF:
            return EXIF_FORMAT_BYTE;

        //Tags with long type:
        case EXIF_TAG_IMAGE_WIDTH:
        case EXIF_TAG_IMAGE_LENGTH:
            return EXIF_FORMAT_LONG;

        //Tags with short type:
        case EXIF_TAG_ORIENTATION:
        case EXIF_TAG_EXPOSURE_PROGRAM:
        case EXIF_TAG_WHITE_BALANCE:
        case EXIF_TAG_FLASH:
            return EXIF_FORMAT_SHORT;

        //Tags with ASCII type:
        case EXIF_TAG_MAKE:
        case EXIF_TAG_MODEL:
        case EXIF_TAG_DATE_TIME_ORIGINAL:
        case EXIF_TAG_GPS_LATITUDE_REF:
        case EXIF_TAG_GPS_LONGITUDE_REF:
        case EXIF_TAG_GPS_DATE_STAMP:
            return EXIF_FORMAT_ASCII;

        //Tags with rational type:
        case EXIF_TAG_EXPOSURE_TIME:
        case EXIF_TAG_FNUMBER:
        case EXIF_TAG_FOCAL_LENGTH:
        case EXIF_TAG_GPS_LATITUDE:
        case EXIF_TAG_GPS_LONGITUDE:
        case EXIF_TAG_GPS_ALTITUDE:
        case EXIF_TAG_GPS_TIME_STAMP:
        case EXIF_TAG_ISO_SPEED_RATINGS:
            return EXIF_FORMAT_RATIONAL;

        //Tags with undefined type:
        case EXIF_TAG_USER_COMMENT:
        case EXIF_TAG_GPS_PROCESSING_METHOD:
            return EXIF_FORMAT_UNDEFINED;

        //Unsupported tags:
        default:
            LOGE("Unsupported tag: %d", tag);
            throw UnknownException("Unsupported tag");
    }
}

} // Exif
} // DeviceAPI
