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

#ifndef _ARGUMENT_VALIDATOR_H_
#define _ARGUMENT_VALIDATOR_H_

#include <string>
#include <vector>
#include <map>
#include <JavaScriptCore/JavaScript.h>
#include <ctime>

namespace DeviceAPI {
namespace Common{

class ArgumentValidator{
public:
    ArgumentValidator(JSContextRef ctx, int argc, const JSValueRef* argv);
    ~ArgumentValidator();

    double toNumber(int index, bool nullable = false, double defaultvalue = 0.0) const;
    long toLong(int index, bool nullable = false, long defaultvalue = 0) const;
    unsigned long toULong(int index, bool nullable = false, unsigned long defaultvalue = 0) const;
    long long toLongLong(int index, bool nullable = false, long long defaultvalue = 0) const;
    unsigned long long toULongLong(int index, bool nullable = false, unsigned long long defaultvalue = 0) const;
    double toDouble(int index, bool nullable = false, double defaultvalue = 0.0) const;
    signed char toByte(int index, bool nullable = false, signed char defaultvalue = 0) const;
    unsigned char toOctet(int index, bool nullable = false, unsigned char defaultvalue = 0) const;
    std::string toString(int index, bool nullable = false, const std::string& defaultvalue = "") const;
    bool toBool(int index, bool nullable = false, bool defaultvalue = false) const;
    time_t toTimeT(int index, bool nullable = false, time_t defaultvalue = 0) const;
    JSObjectRef toObject(int index, bool nullable = false) const;
    JSObjectRef toObject(int index, JSClassRef info, bool nullable = false) const;
    JSObjectRef toFunction(int index, bool nullable = false) const;
    JSObjectRef toArrayObject(int index, bool nullable = false) const;
    JSValueRef toJSValueRef(int index, bool nullable = false) const;

    bool isOmitted(int index);
    bool isNull(int index);
    bool isUndefined(int index);

    JSObjectRef toCallbackObject(int index, bool nullable, const char *callback, ...) const;

    std::vector<std::string> toStringVector(int index, bool nullable = false) const;
    std::vector<long> toLongVector(int index, bool nullable = false) const;
    std::vector<double> toDoubleVector(int index, bool nullable = false) const;
    std::vector<time_t> toTimeTVector(int index, bool nullable = false) const;
    std::vector<bool> toBoolVector(int index, bool nullable = false) const;
    std::vector<unsigned char> toUCharVector(int index, bool nullable = false) const;
    std::vector<JSValueRef> toJSValueRefVector(int index, bool nullable= false) const;

    std::map<std::string, std::string> toStringMap(int index, bool nullable = false) const;

private:
    JSValueRef getArgument(int index, bool nullable) const;
    JSContextRef mContext;
    int mArgc;
    const JSValueRef* mArgv;

};

}
}

#endif //_ARGUMENT_VALIDATOR_H_


