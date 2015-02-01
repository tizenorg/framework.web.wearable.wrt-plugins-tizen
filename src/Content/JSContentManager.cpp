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

#include <SecurityExceptions.h>

#include <JSUtil.h>
#include <JSWebAPIError.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>
#include <Export.h>

#include "plugin_config.h"
#include "ContentCallback.h"

#include "JSPlaylist.h"
#include "JSContentManager.h"
#include "JSContent.h"
#include "JSImageContent.h"
#include "JSVideoContent.h"
#include "JSAudioContent.h"

#include "Content.h"
#include "ContentGetDirectoryCallback.h"
#include "ContentScanFileCallback.h"
#include "ContentFindCallback.h"
#include "ContentUpdateBatchCallback.h"

#include "ContentUtility.h"

#include <AbstractFilter.h>
#include <JSSortMode.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Content {

std::map<JSObjectRef,ContentChangeCallbackPtr> JSContentManager::s_callback_map;

JSClassDefinition JSContentManager::m_classInfo = {
        0,
        kJSClassAttributeNone,
        "ContentManager",
        NULL, //ParentClass
        NULL, //StaticValues
        m_function, //StaticFunctions
        initialize, //Initialize
        finalize, //Finalize
        NULL, //HasProperty,
        NULL, //GetProperty,
        NULL, //SetProperty,
        NULL, //DeleteProperty,
        NULL, //GetPropertyNames,
        NULL, //CallAsFunction,
        NULL, //CallAsConstructor,
        NULL, //HasInstance,
        NULL //ConvertToType
};

JSStaticFunction JSContentManager::m_function[] = {
        { CONTENT_MANAGER_API_UPDATE, update, kJSPropertyAttributeNone },
        { CONTENT_MANAGER_API_UPDATE_BATCH, updateBatch, kJSPropertyAttributeNone },
        { CONTENT_MANAGER_API_GET_DIRECTORIES, getDirectories, kJSPropertyAttributeNone },
        { CONTENT_MANAGER_API_FIND, find, kJSPropertyAttributeNone },
        { CONTENT_MANAGER_API_SCAN_FILE, scanFile, kJSPropertyAttributeNone },
        { CONTENT_MANAGER_API_SET_CHANGE_LISTENER, setChangeListener, kJSPropertyAttributeNone },
        { CONTENT_MANAGER_API_UNSET_CHANGE_LISTENER, unsetChangeListener, kJSPropertyAttributeNone },
        { CONTENT_MANAGER_API_GET_PLAYLISTS, getPlaylists, kJSPropertyAttributeNone },
        { CONTENT_MANAGER_API_CREATE_PLAYLIST, createPlaylist, kJSPropertyAttributeNone },
        { CONTENT_MANAGER_API_REMOVE_PLAYLIST, removePlaylist, kJSPropertyAttributeNone },
        { 0, 0, 0 }
};

JSClassRef JSContentManager::m_jsClassRef = JSClassCreate(JSContentManager::getClassInfo());

const JSClassRef DLL_EXPORT JSContentManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSContentManager::getClassInfo()
{
    return &m_classInfo;
}

void JSContentManager::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
    JSObjectSetPrivate(object, static_cast<void*>(&(ContentManager::getInstance())));
}

void JSContentManager::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

JSValueRef JSContentManager::update(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(CONTENT_TIME_TRACER_SHOW);
    try {
        TIZEN_CHECK_ACCESS(context,
                exception,
                static_cast<void*>(&(ContentManager::getInstance())),
                CONTENT_MANAGER_API_UPDATE);

        ArgumentValidator validator(context, argumentCount, arguments);

        // content
        JSObjectRef contentObj = validator.toObject(0, JSContent::getClassRef());
        ContentPtr content;

        if(JSImageContent::isObjectOfClass(context,contentObj)){
            content = JSImageContent::getPrivateObject(context,contentObj);
        }else if(JSVideoContent::isObjectOfClass(context,contentObj)){
            content = JSVideoContent::getPrivateObject(context,contentObj);
        }else if(JSAudioContent::isObjectOfClass(context,contentObj)){
            content = JSAudioContent::getPrivateObject(context,contentObj);
        }else if(JSContent::isObjectOfClass(context,contentObj)){
            content = JSContent::getPrivateObject(context,contentObj);
        }

        if (!content) {
            throw TypeMismatchException("Content's private object is NULL.");
        }

        // perform
        ContentManager::getInstance().update(content);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ContentManager.update().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSContentManager::updateBatch(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(CONTENT_TIME_TRACER_SHOW);
    TIME_TRACER_ITEM_BEGIN(CONTENT_TT_UPDATE_BATCH_TOTAL, CONTENT_TIME_TRACER_SHOW);
    ContentUpdateBatchCallback *callback = NULL;
    try {

        TIZEN_CHECK_ACCESS(context,
                exception,
                static_cast<void*>(&(ContentManager::getInstance())),
                CONTENT_MANAGER_API_UPDATE_BATCH);

        ArgumentValidator validator(context, argumentCount, arguments);

        ContentLst contents;

        std::vector<JSValueRef> contentsRef = validator.toJSValueRefVector(0);

        if(contentsRef.size() == 0){
            LOGE("throwing TypeMismatchException: \"Content type mismatched.\"");
            throw TypeMismatchException("Content type mismatched.");
        }

        for(unsigned int i=0; i < contentsRef.size(); i++){

            ContentPtr contentPriv;
            if(JSImageContent::isObjectOfClass(context,contentsRef[i])){
                contentPriv = JSImageContent::getPrivateObject(context,JSUtil::JSValueToObject(context,contentsRef[i]));
            }else if(JSVideoContent::isObjectOfClass(context,contentsRef[i])){
                contentPriv = JSVideoContent::getPrivateObject(context,JSUtil::JSValueToObject(context,contentsRef[i]));
            }else if(JSAudioContent::isObjectOfClass(context,contentsRef[i])){
                contentPriv = JSAudioContent::getPrivateObject(context,JSUtil::JSValueToObject(context,contentsRef[i]));
            }else if(JSContent::isObjectOfClass(context,contentsRef[i])){
                contentPriv = JSContent::getPrivateObject(context,JSUtil::JSValueToObject(context,contentsRef[i]));
            }

            if(contentPriv){
                contents.push_back(contentPriv);
            }
            else {
                LOGE("throwing TypeMismatchException: \"Content type mismatched.\"");
                throw TypeMismatchException("Content type mismatched.");
            }
        }

        callback = new ContentUpdateBatchCallback(
                GlobalContextManager::getInstance()->getGlobalContext(context));

        // successCallback
        JSObjectRef successCallbackObj = validator.toFunction(1,true);
        if (successCallbackObj) {
            callback->setSuccessCallback(successCallbackObj);
        }

        // errorCallback
        JSObjectRef errorCallbackObj = validator.toFunction(2, true);

        if (errorCallbackObj) {
            callback->setErrorCallback(errorCallbackObj);
        }

        if(!contents.empty()){
            callback->setContents(contents);
        }

        // perform
        ContentManager::getInstance().updateBatch(callback);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ContentManager.updateBatch().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSContentManager::getDirectories(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(CONTENT_TIME_TRACER_SHOW);
    TIME_TRACER_ITEM_BEGIN(CONTENT_TT_GET_DIR_TOTAL, CONTENT_TIME_TRACER_SHOW);
    ContentCallback *callback = NULL;
    try {

        ArgumentValidator validator(context, argumentCount, arguments);

        callback = new ContentGetDirectoryCallback(GlobalContextManager::getInstance()->getGlobalContext(context));
        JSObjectRef successCallbackObj = validator.toFunction(0);
        if (successCallbackObj) {
            callback->setSuccessCallback(successCallbackObj);
        }

        // errorCallback
        JSObjectRef errorCallbackObj = validator.toFunction(1, true);
        if (errorCallbackObj) {
            callback->setErrorCallback(errorCallbackObj);
        }
        // perform
        ContentManager::getInstance().getDirectories(callback);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ContentManager.getDirectories().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSContentManager::find(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(CONTENT_TIME_TRACER_SHOW);
    TIME_TRACER_ITEM_BEGIN(CONTENT_TT_FIND_TOTAL, CONTENT_TIME_TRACER_SHOW);

    ContentFindCallback *callback = NULL;
    Tizen::AbstractFilterPtr filter;
    Tizen::SortModePtr sortMode;
    JSValueRef filterObj;
    JSValueRef sortModeObj;

    try {
        TIZEN_CHECK_ACCESS(context,
                exception,
                static_cast<void*>(&(ContentManager::getInstance())),
                CONTENT_MANAGER_API_FIND);

        ArgumentValidator validator(context, argumentCount, arguments);

        // successCallback
        callback = new ContentFindCallback(GlobalContextManager::getInstance()->getGlobalContext(context));
        JSObjectRef successCallbackObj = validator.toFunction(0);
        if (successCallbackObj) {
            callback->setSuccessCallback(successCallbackObj);
        }

        // errorCallback
        JSObjectRef errorCallbackObj = validator.toFunction(1, true);
        if (errorCallbackObj) {
            callback->setErrorCallback(errorCallbackObj);
        }

        // directoryId
        std::string directoryId = validator.toString(2, true);

        if(!directoryId.empty()){
            if(directoryId != "null" ){
                callback->setDirectoryId(directoryId);
            }
            else if(directoryId == "undefined"){
                throw InvalidValuesException("directoryId is not valid.");
            }
        }

        // filter
        //filterObj = validator.toObject(3, true);
        filterObj = validator.toJSValueRef(3, true);
        if(!JSValueIsNull(context, filterObj)) {
            filter = Tizen::AbstractFilter::getPrivateObject(context, filterObj);
            LOGD("Set filter with type: %d", filter->getFilterType());
        }

        // sortMode
        //sortModeObj = validator.toObject(4, true);
        sortModeObj = validator.toJSValueRef(4, true);
        if(!JSValueIsNull(context, sortModeObj)) {
            sortMode = Tizen::JSSortMode::getPrivateObject(context, sortModeObj);
            LOGD("Set sort mode: %d for attribute: %s", sortMode->getOrder(),
                    sortMode->getAttributeName().c_str());
        }

        // count
        if(argumentCount >= 6){
            if(!JSValueIsNull(context, arguments[5])){
                long count = validator.toLong(5, true, 0);
                if( count >= 0L ){
                    callback->setCount(count);
                }
                else{
                    throw InvalidValuesException( "count should be positive.");
                }
            }
        }

        // offset
        if(argumentCount >= 7){
            long offset = validator.toLong(6, true);
            if( offset >= 0L ){
                callback->setOffset(offset);
            }
            else{
                throw InvalidValuesException("offset should be positive.");
            }
        }
    } catch (const InvalidValuesException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());

        JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(context,
                err.getName(), err.getMessage());

        callback->onError(errobj);

        delete callback;
        callback = NULL;

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ContentManager.find().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    try
    {
        //ContentFilterConverterFactory::ConverterType fConverter = ContentFilterConverterFactory::getConverter(context);
        if(filterObj){
            //filter = fConverter->toFilter(filterObj);
            callback->setFilter(filter);
        }

        if(sortModeObj){
            //sortMode = fConverter->toSortMode(sortModeObj);
            callback->setSortMode(sortMode);
        }
        // perform
        ContentManager::getInstance().find(callback);

        return JSValueMakeUndefined(context);
    //TODO: to be changed in the future
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ContentManager.find().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSContentManager::scanFile(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(CONTENT_TIME_TRACER_SHOW);
    TIME_TRACER_ITEM_BEGIN(CONTENT_TT_SCAN_FILE_TOTAL, CONTENT_TIME_TRACER_SHOW);

    ContentCallback *callback = NULL;

    try {
        TIZEN_CHECK_ACCESS(context,
                exception,
                static_cast<void*>(&(ContentManager::getInstance())),
                CONTENT_MANAGER_API_SCAN_FILE);

        ArgumentValidator validator(context, argumentCount, arguments);

        // contentURI
        if ( validator.isUndefined(0) || validator.isNull(0)) {
            LOGE("File path is not valid.");
            throw InvalidValuesException("File path is not valid.");
        }

        JSObjectRef successCallbackObj = validator.toFunction(1, true);
        JSObjectRef errorCallbackObj = validator.toFunction(2, true);

        std::string contentURI = validator.toString(0);
        if(contentURI.empty()){
            LOGE("File path is not valid.");
            throw InvalidValuesException("File path is not valid.");
        }

        contentURI = ContentUtility::convertUriToPath(contentURI);

        callback = new ContentScanFileCallback(
                GlobalContextManager::getInstance()->getGlobalContext(context),contentURI);
        // successCallback
        if (successCallbackObj) {
            callback->setSuccessCallback(successCallbackObj);
        }
        // errorCallback
        if (errorCallbackObj) {
            callback->setErrorCallback(errorCallbackObj);
        }

        // perform
        ContentManager::getInstance().scanFile(callback);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ContentManager.scanFile().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSContentManager::setChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(CONTENT_TIME_TRACER_SHOW);

    try {

        TIZEN_CHECK_ACCESS(context,
                exception,
                static_cast<void*>(&(ContentManager::getInstance())),
                CONTENT_MANAGER_API_SET_CHANGE_LISTENER);

        ArgumentValidator validator(context, argumentCount, arguments);

        // changeCallback
        JSObjectRef changeCallbackObj = validator.toCallbackObject(0,false,"oncontentadded","oncontentupdated","oncontentremoved",NULL);
        ContentChangeCallbackPtr changeCallback =
                ContentChangeCallbackPtr(new ContentChangeCallback(
                        GlobalContextManager::getInstance()->getGlobalContext(context),
                        changeCallbackObj));

        ContentManager::getInstance().unsetChangeListener(s_callback_map[thisObject]);
        s_callback_map[thisObject] = changeCallback;
        // perform
        if(!(ContentManager::getInstance().setChangeListener(changeCallback))){
            throw DeviceAPI::Common::UnknownException( "Unknown exception is occured by platfrom");
        }

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ContentManager.setChangeListener().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSContentManager::unsetChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(CONTENT_TIME_TRACER_SHOW);

    try {
        TIZEN_CHECK_ACCESS(context,
                exception,
                static_cast<void*>(&(ContentManager::getInstance())),
                CONTENT_MANAGER_API_UNSET_CHANGE_LISTENER);

        // perform
        ContentManager::getInstance().unsetChangeListener(s_callback_map[thisObject]);

        s_callback_map.erase(thisObject);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ContentManager.unsetChangeListener().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSContentManager::getPlaylists(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    PlaylistArraySuccessCallbackUserData* callback = NULL;
    try {
        TIZEN_CHECK_ACCESS(context, exception, &ContentManager::getInstance(),
                CONTENT_MANAGER_API_GET_PLAYLISTS);

        ArgumentValidator validator(context, argument_count, arguments);

        JSContextRef g_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);
        callback = new(std::nothrow) PlaylistArraySuccessCallbackUserData(g_ctx);
        if(!callback) {
            LOGE("Couldn't allocate callback - PlaylistArraySuccessCallbackUserData!");
            throw UnknownException("Could not get playlists");
        }

        callback->setSuccessCallback(validator.toFunction(0));
        callback->setErrorCallback(validator.toFunction(1, true));

        ContentManager::getInstance().getPlaylists(callback);
    }
    catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        Common::UnknownException err("Unknown error in getPlaylists method");
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSContentManager::createPlaylist(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    CreatePlaylistCallbackUserData* callback = NULL;
    try {
        TIZEN_CHECK_ACCESS(context, exception, &ContentManager::getInstance(),
                CONTENT_MANAGER_API_CREATE_PLAYLIST);

        ArgumentValidator validator(context, argument_count, arguments);

        JSContextRef g_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);
        callback = new(std::nothrow) CreatePlaylistCallbackUserData(g_ctx);
        if(!callback) {
            LOGE("Couldn't allocate callback - CreatePlaylistCallbackUserData!");
            throw UnknownException("Could not create playlist");
        }

        callback->setPlaylistName(validator.toString(0));
        callback->setSuccessCallback(validator.toFunction(1));

        callback->setErrorCallback(validator.toFunction(2, true));

        JSObjectRef js_src_playlist = validator.toObject(3, true);
        if (js_src_playlist != NULL) {
            callback->setSourcePlaylist(
                    JSPlaylist::getPrivateObject(context, js_src_playlist));
        }

        ContentManager::getInstance().createPlaylist(callback);
    }
    catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        Common::UnknownException err("Unknown error in createPlaylist");
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSContentManager::removePlaylist(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    RemovePlaylistCallbackUserData* callback = NULL;

    try {
        TIZEN_CHECK_ACCESS(context, exception, &ContentManager::getInstance(),
                CONTENT_MANAGER_API_REMOVE_PLAYLIST);

        ArgumentValidator validator(context, argument_count, arguments);

        JSContextRef g_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);
        callback = new(std::nothrow) RemovePlaylistCallbackUserData(g_ctx);
        if(!callback) {
            LOGE("Couldn't allocate callback - RemovePlaylistCallbackUserData!");
            throw UnknownException("Could not remove playlist");
        }

        callback->setPlaylistId(validator.toString(0));
        callback->setSuccessCallback(validator.toFunction(1, true));
        callback->setErrorCallback(validator.toFunction(2, true));

        ContentManager::getInstance().removePlaylist(callback);
    }
    catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        Common::UnknownException err("Unknown error in removePlaylist");
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

} // Content
} // DeviceAPI
