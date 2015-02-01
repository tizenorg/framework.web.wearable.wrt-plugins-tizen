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
 * @file        StorageProperties.cpp
 */

#include "StorageProperties.h"



namespace DeviceAPI {
namespace Filesystem {
StorageProperties::StorageProperties() :
        id(0),
        label(""),
        type(0),
        state(0),
        fullpath("")
{
}

StorageProperties::StorageProperties(const StorageProperties& src) :
    id(src.id),
    label(src.label),
    type(src.type),
    state(src.state),
    fullpath(src.fullpath)
{
}

void StorageProperties::setId (const int Id)
{
    id = Id;
}

void StorageProperties::setLabel (const std::string &Label)
{
    label = Label;
}

void StorageProperties::setType (const short Type)
{
    type = Type;
}

void StorageProperties::setState (const short State)
{
    state = State;
}

void StorageProperties::setFullPath (const std::string &FullPath)
{
    fullpath = FullPath;
}

int StorageProperties::getId() const
{
    return id;
}

std::string StorageProperties::getLabel() const
{
    return label;
}

short StorageProperties::getType() const
{
    return type;
}

short StorageProperties::getState() const
{
    return state;
}

std::string StorageProperties::getFullPath() const
{
    return fullpath;
}
} // Filesystem
} // TizenApis
