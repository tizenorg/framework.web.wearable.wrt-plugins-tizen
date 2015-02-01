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
 * @file        FilesystemUtils.cpp
 */

#include "FilesystemUtils.h"

#include <map>
#include <string>
#include <iconv.h>
#include <PlatformException.h>
#include <Export.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <WidgetDB/WidgetDBMgr.h>
#include "Enums.h"
#include "JSErrors.h"


namespace DeviceAPI {
namespace Filesystem {
namespace Utils{

void DLL_EXPORT toUTF8String(const std::string& fromEncoding,
                             const char* from,
                             const size_t fromLength,
                             std::string &outputString)
{
    const char *fromEncodingSet = fromEncoding.c_str();
    char *outputBuf = NULL;
    char *buf = NULL;
    int ret = 0;
    iconv_t cd;
    size_t outputLength= 0;

    try {
        cd = iconv_open("UTF-8", fromEncodingSet);

        if (cd == (iconv_t) -1)
        {
            LOGW("charset conversion exception iconv -1");
            LOGW("throw UnknownException");
            throw DeviceAPI::Common::UnknownException(
                    "Couldn't allocate output buffer.");
        }

        if (fromLength == 0)
        {
            LOGW("from length 0");
            LOGW("throw UnknownException");
            //FIXME: Memory leak of cd
            throw DeviceAPI::Common::UnknownException(
                    "charset conversion exception");
        }

        outputBuf = new char[fromLength * 4 + 1];
        outputLength = fromLength * 4;
        memset(outputBuf, 0, outputLength + 1);
        buf = outputBuf;

        char** from_ = const_cast<char**>(&from);
        size_t* fromLength_ = const_cast<size_t*>(&fromLength);

        ret = iconv(cd, from_, fromLength_, &buf, &outputLength);

        LOGD("%d %d", fromLength, outputLength);

        if (ret < 0)
        {
            iconv_close(cd);
            LOGW("charset conversion exception ret %d", ret);
            LOGW("throw UnknownException");
            //FIXME: Memory leak of ouputBuf and cd
            throw DeviceAPI::Common::UnknownException(
                    "charset conversion exception");
        }

        iconv_close(cd);
        outputString = outputBuf;

        if (outputBuf)
            delete[] outputBuf;

    }
    catch(std::bad_alloc) {
        LOGW("Couldn't allocate output buffer.");
        LOGW("throw UnknownException");
        throw DeviceAPI::Common::UnknownException(
                "Couldn't allocate output buffer.");
    }
}
}
}
}
