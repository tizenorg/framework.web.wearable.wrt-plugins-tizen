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
 * @file        Stream.cpp
 */

#include "Stream.h"

#include <stdio.h>
#include <new>
#include <Logger.h>
#include <PlatformException.h>

#include "Node.h"
#include "Path.h"



using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;


namespace DeviceAPI {
namespace Filesystem {
Stream::Stream(const NodePtr& parent,
        int mode) :
    m_mode(mode)
{
    LOGD("Stream created this %p", this);
    std::ios_base::openmode mode_ = std::ios_base::binary;
    if (mode & AM_READ) { mode_ |= std::ios_base::in; }
    if (mode & AM_WRITE) { mode_ |= std::ios_base::out; }
    else if (mode & AM_APPEND) {
        mode_ |= (std::ios_base::app | std::ios_base::out);
    }
    auto path = parent->getPath()->getFullPath();
    LOGD("Open stream path %s mode ios %d mode JS %d",
         path.c_str(),
         mode_,
         mode);
    m_stream.open(path.c_str(), mode_);
    if (!m_stream) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Could not open stream.");
    }
}

Stream::~Stream()
{
    LOGD("Stream destroyed %p", this);
    close();
}

StreamPtr Stream::write(bool arg)
{
    return write_(arg);
}

StreamPtr Stream::write(unsigned char arg)
{
    return write_(arg);
}

StreamPtr Stream::write(char arg)
{
    return write_(arg);
}

StreamPtr Stream::write(int arg)
{
    return write_(arg);
}

StreamPtr Stream::write(double arg)
{
    return write_(arg);
}

StreamPtr Stream::write(const std::string& arg)
{
    return write_(arg);
}

StreamPtr Stream::write(const unsigned char* arg, int size)
{
    checkForWriting();

    if (!(m_stream.write(reinterpret_cast<const char *>(arg), size))) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                "Error while writing to the stream.");
    }
    m_stream.flush();

    return shared_from_this();
}

StreamPtr Stream::read(bool& arg)
{
    return read_(arg);
}

StreamPtr Stream::read(unsigned char& arg)
{
    return read_(arg);
}

StreamPtr Stream::read(char& arg)
{
    return read_(arg);
}

StreamPtr Stream::read(int& arg)
{
    return read_(arg);
}

StreamPtr Stream::read(double& arg)
{
    return read_(arg);
}

StreamPtr Stream::read(std::string& arg)
{
    return read_(arg);
}

void Stream::setCharSet(const std::string &charSet)
{
    m_charSet = charSet;
}

std::string Stream::getCharSet() const
{
    return m_charSet;
}


template <class taValue>
taValue* Stream::getValues(std::size_t aNum, std::fstream& aStream)
{
    checkForReading();

    std::size_t maxChars = aNum + 1;
    taValue* result = NULL;
    try {
        result = new taValue[maxChars];
        memset(result, 0, maxChars * sizeof(taValue));

    } catch(std::bad_alloc) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Couldn't allocate output buffer.");
    }
    aStream.read(reinterpret_cast<char*>(result), aNum * sizeof(taValue));

    if (aStream.bad()) {
        LOGW("throw IOException");
        delete[] result;
        throw DeviceAPI::Common::IOException(
                "Error while reading from the stream.");
    }

    return result;
}

char* Stream::getChars(std::size_t num)
{
    return getValues<char>(num, m_stream);
}

unsigned char* Stream::getBytes(std::size_t num)
{
    return getValues<unsigned char>(num, m_stream);
}

std::size_t Stream::getCount() const
{
    if (!isOpen()) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Stream is closed.");
    }

    if (!isReadable()) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Stream is not readable.");
    }

    return m_stream.gcount();
}

std::string Stream::getLine()
{
    checkForReading();

    std::string result;
    std::getline(m_stream, result);

    return result;
}

bool Stream::isOpen() const
{
    return m_stream.is_open();
}

bool Stream::isEof() const
{
    return m_stream.eof();
}

void Stream::close()
{
    if (isOpen()) {
        m_stream.close();
    }
}

int Stream::getMode() const
{
    return m_mode;
}

long Stream::getPosition() const
{
    return static_cast<long>(m_stream.tellg());
}

void Stream::setPosition(long position)
{
    if (m_stream.rdstate() & std::ios_base::eofbit) {
        m_stream.clear();
    }
    if (!(m_stream.seekg(position)) || !(m_stream.seekp(position))) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Could not set position.");
    }
}

unsigned long long Stream::getSize() const
{
    std::fstream::streampos pos = m_stream.tellg();
    if (pos == -1) {
        return 0;
    }

    if (!m_stream.seekg(0, std::_S_end)) {
        return 0;
    }

    unsigned long long result = m_stream.tellg();
    m_stream.seekg(pos, std::_S_beg);

    return result + 1;
}

template<typename T>
StreamPtr Stream::write_(T arg)
{
    checkForWriting();

    if (!(m_stream << arg)) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                "Error while writing to the stream.");
    }
    m_stream.flush();

    return shared_from_this();
}

template<typename T>
StreamPtr Stream::read_(T& arg)
{
    checkForReading();

    if (!(m_stream >> arg)) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                "Error while reading from the stream.");
    }

    return shared_from_this();
}

bool Stream::isReadable() const
{
    return (m_mode & AM_READ);
}

bool Stream::isWriteable() const
{
    return ((m_mode & AM_WRITE) || (m_mode & AM_APPEND));
}

void Stream::checkForReading() const
{
    if (!isOpen()) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Stream is closed.");
    }

    if (isEof()) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Stream is marked as EOF.");
    }

    if (!isReadable()) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Stream is not readable.");
    }
}

void Stream::checkForWriting() const
{
    if (!isOpen()) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Stream is closed.");
    }

    if (!isWriteable()) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Stream is not writeable.");
    }
}
} // Filesystem
} // TizenApis

