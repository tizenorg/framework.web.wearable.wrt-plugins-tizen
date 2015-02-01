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
 * @file        Stream.h
 */

#ifndef TIZENAPIS_FILESYSTEM_STREAM_H_
#define TIZENAPIS_FILESYSTEM_STREAM_H_

#include <cstddef>
#include <fstream>
#include <memory>
#include <Logger.h>
#include "Enums.h"
#include "FilesystemUtils.h"
#include <PlatformException.h>
#include <SecurityAccessor.h>

namespace DeviceAPI {
namespace Filesystem {
class Node;
typedef std::shared_ptr<Node> NodePtr;


class Stream;
typedef std::shared_ptr<Stream> StreamPtr;

struct StreamHolder {
    StreamPtr ptr;
};

class Stream : public std::enable_shared_from_this<Stream> , public Common::SecurityAccessor
{
    friend class Node;

public:
    ~Stream();

    StreamPtr write(bool arg);
    StreamPtr write(unsigned char arg);
    StreamPtr write(char arg);
    StreamPtr write(int arg);
    StreamPtr write(double arg);
    StreamPtr write(const std::string& arg);
    StreamPtr write(const unsigned char* arg, int size);

    StreamPtr read(bool& arg);
    StreamPtr read(unsigned char& arg);
    StreamPtr read(char& arg);
    StreamPtr read(int& arg);
    StreamPtr read(double& arg);
    StreamPtr read(std::string& arg);

    void setCharSet(const std::string &charSet);
    std::string getCharSet() const;

    /**
     *  Return value should be dealocated by programmer
     */
    unsigned char* getBytes(std::size_t num);

    std::size_t getCount() const;

    /**
     *  Return value should be dealocated by programmer
     */
    char* getChars(std::size_t num);

    std::string getLine();

    bool isOpen() const;
    bool isEof() const;

    void close();

    long getPosition() const;
    void setPosition(long position);

    int getMode() const;

    unsigned long long getSize() const;

private:
    template<typename T>
    StreamPtr read_(T& arg);
    template<typename T>
    StreamPtr write_(T arg);

    inline bool isReadable() const;
    inline bool isWriteable() const;
    void checkForReading() const;
    void checkForWriting() const;

    Stream(const NodePtr& parent,
            int mode);

    int m_mode;
    mutable std::fstream m_stream;
    std::string m_charSet;

    template <class taValue>
    taValue* getValues(std::size_t aNum, std::fstream& aStream);

};

} // Filesystem
} // TizenApis

#endif /* TIZENAPIS_FILESYSTEM_STREAM_H_ */
