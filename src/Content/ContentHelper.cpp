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

#include <glib.h>
#include <list>

#include <Export.h>
#include <Logger.h>
#include <JSWebAPIError.h>
#include <Commons/StringUtils.h>
#include <GlobalContextManager.h>

#include "ContentUtility.h"
#include "ContentHelper.h"

#include "ContentGetDirectoryCallback.h"
#include "ContentScanFileCallback.h"
#include "ContentFindCallback.h"
#include "ContentFilterQueryGenerator.h"
#include "ContentUpdateBatchCallback.h"
#include "ContentChangeCallback.h"
#include <TimeTracer.h>



using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Content {

#define TAG_DELIMETER '/'
#define UNDEFINED_GEO -200.0

ContentHelper::ContentHelper()
{
}

ContentHelper::~ContentHelper()
{
}

bool ContentHelper::Connect()
{
    bool ret = false;
    int err = media_content_connect();
    if( MEDIA_CONTENT_ERROR_NONE == err)
    {
        LOGI("Content DB is connected.");
        ret =  true;
    }
    else {
        LOGE("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_content_connect()").c_str());
    }
    return ret;

}
bool ContentHelper::DisConnect()
{
    bool ret = false;
    int err = media_content_disconnect();
    if ( MEDIA_CONTENT_ERROR_NONE == err)
    {
        LOGI("Content DB is disconnected.");
        ret =  true;
    } else {
        LOGE("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_content_disconnect()").c_str());
    }
    return ret;

}

int ContentHelper::getCallbackCompleted(void *data)
{
    ContentCallbackPtr callbackPtr(static_cast<ContentCallback*>(data));
    try
    {
        callbackPtr->onSuccess();
    }
    catch(...)
    {
        callbackPtr->onError("This operation is failed by platform.");
    }

    LOGI("getCallbackCompleted");
    return false;
}

int ContentHelper::getCallbackCanceled(void *data)
{
    ContentCallbackPtr callbackPtr(static_cast<ContentCallback*>(data));

    callbackPtr->onError();
    return false;
}

gboolean ContentHelper::getDirectoryWork(void* data)
{
    ContentGetDirectoryCallback* callback = static_cast<ContentGetDirectoryCallback*>(data);
    DirectoryLst *directories = callback->getDirectories();

    filter_h filter = NULL;
    TIME_TRACER_ITEM_BEGIN("getDirectoriesPlatform", 1);
    int err = media_filter_create(&filter);
    if ( MEDIA_CONTENT_ERROR_NONE == err)
    {
        std::string condition="(FOLDER_STORAGE_TYPE = 0 OR FOLDER_STORAGE_TYPE = 1)";
        //set condition
        err = media_filter_set_condition(filter, condition.c_str(), MEDIA_CONTENT_COLLATE_DEFAULT);
        if ( MEDIA_CONTENT_ERROR_NONE != err)
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "media_filter_set_condition()").c_str());
        }

        err = media_folder_foreach_folder_from_db(filter, ContentHelper::getDirectoryCallback, directories);
        if( MEDIA_CONTENT_ERROR_NONE == err)
        {
            TIME_TRACER_ITEM_END("getDirectoriesPlatform", 1);
            if (G_IDLE_FAILED == g_idle_add(ContentHelper::getCallbackCompleted, callback)) {
                delete callback;
                callback = NULL;
                LOGE("g_idle addition failed");
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "media_folder_foreach_folder_from_db()").c_str());

            callback->setErrMsg("getDirectoryWork() is failed");
            if (G_IDLE_FAILED == g_idle_add(ContentHelper::getCallbackCanceled, callback)) {
                delete callback;
                callback = NULL;
                LOGE("g_idle addition failed");
            }
        }

        err = media_filter_destroy(filter);
        if ( MEDIA_CONTENT_ERROR_NONE != err)
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "media_filter_destroy()").c_str());
        }
    }
    else
    {
        LOGE("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_filter_create()").c_str());
    }
    return false;
}


bool ContentHelper::getDirectoryCallback(media_folder_h folder, void *user_data)
{
    int err;
    if (user_data != NULL) {
        if (folder != NULL) {
            char *str = NULL;
            ContentDirectoryPtr dirPtr = ContentDirectoryPtr(new ContentDirectory());

            err = media_folder_get_folder_id(folder, &str);
            if (MEDIA_CONTENT_ERROR_NONE == err) {
                if (str) {
                    dirPtr->setId(str);
                    free(str);
                    str = NULL;
                }
            }
            else
            {
                LOGE("%s", ContentUtility::getMediaContentLogMessage(
                        err, "media_folder_get_folder_id()").c_str());
            }

            err = media_folder_get_path(folder, &str);
            if (MEDIA_CONTENT_ERROR_NONE == err) {
                if (str) {
                    dirPtr->setDirectoryURI(str);
                    free(str);
                    str = NULL;
                }
            }
            else
            {
                LOGE("%s", ContentUtility::getMediaContentLogMessage(
                        err, "media_folder_get_path()").c_str());
            }

            err = media_folder_get_name(folder, &str);
            if (MEDIA_CONTENT_ERROR_NONE == err) {
                if (str) {
                    dirPtr->setTitle(str);
                    free(str);
                    str = NULL;
                }
            }
            else
            {
                LOGE("%s", ContentUtility::getMediaContentLogMessage(
                        err, "media_folder_get_name()").c_str());
            }

            {
                time_t date;
                err = media_folder_get_modified_time(folder, &date);
                if (MEDIA_CONTENT_ERROR_NONE == err) {
                    dirPtr->setModifiedDate(date);
                }
                else
                {
                    LOGE("%s", ContentUtility::getMediaContentLogMessage(
                            err, "media_folder_get_modified_time()").c_str());
                }
            }
            {
                media_content_storage_e storageType;
                err = media_folder_get_storage_type(folder, &storageType);
                if (MEDIA_CONTENT_ERROR_NONE == err) {
                    if (storageType == MEDIA_CONTENT_STORAGE_INTERNAL) {
                        dirPtr->setStorageType("INTERNAL");
                    }
                    else if (storageType == MEDIA_CONTENT_STORAGE_EXTERNAL) {
                        dirPtr->setStorageType("EXTERNAL");
                    }
                }
                else
                {
                    LOGE("%s", ContentUtility::getMediaContentLogMessage(
                            err, "media_folder_get_storage_type()").c_str());
                }
            }

            if (DirectoryLst* directories = static_cast<DirectoryLst*>(user_data)) {
                directories->push_back(dirPtr);
            }
            else {
                LOGE("User data is invalid.");
            }
        }
        else {
            LOGE("Get Directories Iterator is failed.");
            //return false;
        }
    }
    else {
        LOGD("user data is NULL");
        //return false;
    }
    return true;
}


gboolean ContentHelper::scanFileWork(void* data)
{
    LOGD("Entered");
    ContentScanFileCallback* callback = static_cast<ContentScanFileCallback*>(data);
    std::string uri = callback->getContentURI();

    TIME_TRACER_ITEM_BEGIN("scanFilePlatform", 1);

    int err = media_content_scan_file(uri.c_str());
    if( MEDIA_CONTENT_ERROR_NONE == err)
    {
        TIME_TRACER_ITEM_END("scanFilePlatform", 1);
        if (G_IDLE_FAILED == g_idle_add(ContentHelper::getCallbackCompleted, callback)) {
            delete callback;
            callback = NULL;
            LOGE("g_idle addition failed");
        }
    }
    else
    {
        LOGE("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_content_scan_file()").c_str());
        callback->setErrMsg("scanFile operation is failed.");
        if (G_IDLE_FAILED == g_idle_add(ContentHelper::getCallbackCanceled, callback)) {
            delete callback;
            callback = NULL;
            LOGE("g_idle addition failed");
        }
    }
    return false;
}

gboolean ContentHelper::findWork(void* data)
{
    ContentFindCallback* callback = static_cast<ContentFindCallback*>(data);
    ContentLst *contents = callback->getContents();

    TIME_TRACER_ITEM_BEGIN("findPlatform", 1);
    int err;
    try
    {

        filter_h filter = NULL;
        err = media_filter_create(&filter);
        if ( MEDIA_CONTENT_ERROR_NONE == err)
        {
            std::shared_ptr<ContentFilterQueryGenerator> query(new ContentFilterQueryGenerator());
            std::string condition;
            AbstractFilterPtr& jsfilter = callback->getFilter();

            if(jsfilter)
            {
                query->generate(jsfilter);
                condition = query->getResult();
                err = media_filter_set_condition(
                        filter, condition.c_str(), MEDIA_CONTENT_COLLATE_DEFAULT);
                if (MEDIA_CONTENT_ERROR_NONE != err)
                {
                    LOGE("ret: %d", err);
                    ContentUtility::throwMediaContentException<Common::UnknownException>(
                            err, "media_filter_set_condition()");
                }
            }

            SortModePtr& jssortMode = callback->getSortMode();
            if(jssortMode)
            {
                media_content_order_e order = MEDIA_CONTENT_ORDER_ASC;
                std::string attrName = query->getPlatformAttr(jssortMode->getAttributeName());
                if(jssortMode->getOrder() == Tizen::ASC)
                {
                    order = MEDIA_CONTENT_ORDER_ASC;
                }
                else if(jssortMode->getOrder() == Tizen::DESC)
                {
                    order = MEDIA_CONTENT_ORDER_DESC;
                }
                else
                {
                    throw Common::UnknownException("SortMode is invalid value.");
                }

                err = media_filter_set_order(
                        filter, order, attrName.c_str(), MEDIA_CONTENT_COLLATE_DEFAULT);
                if (MEDIA_CONTENT_ERROR_NONE != err)
                {
                    LOGE("ret: %d", err);
                    ContentUtility::throwMediaContentException<Common::UnknownException>(
                            err, "media_filter_set_order()");
                }
            }

            int count = callback->getCount();
            int offset = callback->getOffset();

            err = media_filter_set_offset(filter, offset, count);
            if ( MEDIA_CONTENT_ERROR_NONE != err)
            {
                LOGE("ret: %d", err);
                ContentUtility::throwMediaContentException<Common::UnknownException>(
                        err, "media_filter_set_offset()");
            }

            if(callback->getDirectoryId().empty())
            {
                err = media_info_foreach_media_from_db(filter,findCallback, contents);
                if( MEDIA_CONTENT_ERROR_NONE != err)
                {
                    LOGE("ret: %d", err);
                    ContentUtility::throwMediaContentException<Common::UnknownException>(
                            err, "media_info_foreach_media_from_db()");
                }
            }
            else
            {
                err = media_folder_foreach_media_from_db(
                        callback->getDirectoryId().c_str(),
                        filter,
                        findCallback,
                        contents);
                if( MEDIA_CONTENT_ERROR_NONE != err)
                {
                    LOGE("ret: %d", err);
                    ContentUtility::throwMediaContentException<Common::UnknownException>(
                            err, "media_folder_foreach_media_from_db()");
                }
            }
            TIME_TRACER_ITEM_END("findPlatform", 1);
            if (G_IDLE_FAILED == g_idle_add(ContentHelper::getCallbackCompleted, callback)) {
                delete callback;
                callback = NULL;
                LOGE("g_idle addition failed");
            }
        }
        else
        {
            LOGE("ret: %d", err);
            ContentUtility::throwMediaContentException<Common::UnknownException>(
                    err, "media_filter_create()");
        }

        int ret = media_filter_destroy(filter);
        if ( MEDIA_CONTENT_ERROR_NONE != ret){
            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                    ret, "media_filter_destroy()").c_str());
        }
    }
    catch (const BasePlatformException &err) {
        callback->setErrMsg(err.getMessage().c_str());
        if (G_IDLE_FAILED == g_idle_add(ContentHelper::getCallbackCanceled, callback)) {
            delete callback;
            callback = NULL;
            LOGE("g_idle addition failed");
        }
    }

    return false;
}

bool ContentHelper::findCallback(media_info_h info, void *user_data)
{
    int err;
    if (user_data != NULL) {
        if (info != NULL) {
            ContentLst* contents = static_cast<ContentLst*>(user_data);

            media_content_type_e type;

            err = media_info_get_media_type(info, &type);
            if (MEDIA_CONTENT_ERROR_NONE
                    == err) {
                if (type == MEDIA_CONTENT_TYPE_IMAGE) {
                    ImageContentPtr image = ImageContentPtr(new ImageContent());
                    ContentHelper::loadImageContentFromPlatform(info, image);
                    contents->push_back(image);
                }
                else if (type == MEDIA_CONTENT_TYPE_VIDEO) {
                    VideoContentPtr video = VideoContentPtr(new VideoContent());
                    ContentHelper::loadVideoContentFromPlatform(info, video);
                    contents->push_back(video);
                }
                else if (type == MEDIA_CONTENT_TYPE_MUSIC) {
                    AudioContentPtr audio = AudioContentPtr(new AudioContent());
                    ContentHelper::loadAudioContentFromPlatform(info, audio);
                    contents->push_back(audio);
                }
                else if (type == MEDIA_CONTENT_TYPE_OTHERS) {
                    ContentPtr content = ContentPtr(new Content());
                    ContentHelper::loadContentFromPlatform(info, content);
                    contents->push_back(content);
                }
            }
            else
            {
                LOGW("%s", ContentUtility::getMediaContentLogMessage(
                        err, "media_info_get_media_type()").c_str());
            }
        }
        else {
            LOGE("content handle is not valid.");
            //return false;
        }
    }
    else {
        LOGD("user data is NULL");
        //return false;
    }
    return true;
}

bool ContentHelper::update(ContentPtr content)
{
    bool ret = false;
    TIME_TRACER_ITEM_BEGIN("updatePlatform", 1);
    ret = updateContentToPlatform(content);
    TIME_TRACER_ITEM_END("updatePlatform", 1);

    return ret;
}

gboolean ContentHelper::updateBatchWork(void* data)
{
    ContentUpdateBatchCallback* callback = static_cast<ContentUpdateBatchCallback*>(data);
    ContentLst *contents = callback->getContents();

    TIME_TRACER_ITEM_BEGIN("updateBatchPlatform", 1);

    if(!(contents->empty()))
    {
        for(unsigned int i = 0 ; i < contents->size(); i++)
        {
            updateContentToPlatform(contents->at(i));
        }
        TIME_TRACER_ITEM_END("updateBatchPlatform", 1);
        if (G_IDLE_FAILED == g_idle_add(ContentHelper::getCallbackCompleted, callback)) {
            delete callback;
            callback = NULL;
            LOGE("g_idle addition failed");
        }
    }
    else
    {
        callback->setErrMsg("The Private object is null.");

        if (G_IDLE_FAILED == g_idle_add(ContentHelper::getCallbackCanceled, callback)) {
            delete callback;
            callback = NULL;
            LOGE("g_idle addition failed");
        }
    }

    return false;
}

bool ContentHelper::updateContentToPlatform(ContentPtr content)
{

    bool ret = true;
    int err;

    std::string id = content->getId();
    if(!id.empty())
    {
        media_info_h info = NULL;
        err = media_info_get_media_from_db(id.c_str(), &info);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            err = media_info_set_rating(info, content->getRating());
            if(MEDIA_CONTENT_ERROR_NONE != err)
            {
                LOGE("%s", ContentUtility::getMediaContentLogMessage(
                        err, "media_info_set_rating()").c_str());
            }

            err = media_info_set_description (info, content->getDescription().c_str());
            if(MEDIA_CONTENT_ERROR_NONE != err)
            {
                LOGE("%s", ContentUtility::getMediaContentLogMessage(
                        err, "media_info_set_description()").c_str());
            }

            err = media_info_set_display_name (info, content->getName().c_str());
            if(MEDIA_CONTENT_ERROR_NONE != err)
            {
                LOGE("%s", ContentUtility::getMediaContentLogMessage(
                        err, "media_info_set_display_name()").c_str());
            }

            err = media_info_set_favorite (info, content->getIsFavorite());
            if(MEDIA_CONTENT_ERROR_NONE != err)
            {
                LOGE("%s", ContentUtility::getMediaContentLogMessage(
                        err, "media_info_set_favorite()").c_str());
            }

            if(content->getType().compare("IMAGE") == 0)
            {
                image_meta_h img=NULL;
                ImageContentPtr imagePtr = std::dynamic_pointer_cast<ImageContent>(content);
                if(imagePtr)
                {
                    err = media_info_get_image(info, &img);
                    if(MEDIA_CONTENT_ERROR_NONE == err)
                    {
                        err = image_meta_set_orientation(img, imagePtr->getOrientationEnum());
                        if(MEDIA_CONTENT_ERROR_NONE != err)
                        {
                            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                                    err, "image_meta_set_orientation()").c_str());
                        }

                        double latitude = UNDEFINED_GEO;
                        double longitude = UNDEFINED_GEO;
                        if (imagePtr->isGeolocationSet()) {
                            latitude = imagePtr->getLatitude();
                            longitude = imagePtr->getLongitude();
                        }

                        err = media_info_set_latitude(info, latitude);
                        if(MEDIA_CONTENT_ERROR_NONE != err)
                        {
                            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                                    err, "media_info_set_latitude()").c_str());
                        }

                        err = media_info_set_longitude(info, longitude);
                        if(MEDIA_CONTENT_ERROR_NONE != err)
                        {
                            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                                    err, "media_info_set_longitude()").c_str());
                        }

                        err = image_meta_update_to_db(img);
                        if(MEDIA_CONTENT_ERROR_NONE != err)
                        {
                            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                                    err, "image_meta_update_to_db()").c_str());
                            ret = false;
                        }

                        err = image_meta_destroy(img);
                        if ( MEDIA_CONTENT_ERROR_NONE != err)
                        {
                            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                                    err, "image_meta_destroy()").c_str());
                        }
                    }
                    else
                    {
                        LOGE("%s", ContentUtility::getMediaContentLogMessage(
                                err, "media_info_get_image()").c_str());
                    }
                }
            }
            if(content->getType().compare("VIDEO") == 0)
            {
                video_meta_h video = NULL;
                VideoContentPtr videoPtr = std::dynamic_pointer_cast<VideoContent>(content);
                if(videoPtr)
                {
                    err = media_info_get_video(info, &video);
                    if ( MEDIA_CONTENT_ERROR_NONE == err)
                    {
                        double latitude = UNDEFINED_GEO;
                        double longitude = UNDEFINED_GEO;
                        if (videoPtr->isGeolocationSet()) {
                            latitude = videoPtr->getLatitude();
                            longitude = videoPtr->getLongitude();
                        }

                        err = media_info_set_latitude(info, latitude);
                        if(MEDIA_CONTENT_ERROR_NONE != err)
                        {
                            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                                    err, "media_info_set_latitude()").c_str());
                        }

                        err = media_info_set_longitude(info, longitude);
                        if(MEDIA_CONTENT_ERROR_NONE != err)
                        {
                            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                                    err, "media_info_set_longitude()").c_str());
                        }

                        err = video_meta_update_to_db(video);
                        if(MEDIA_CONTENT_ERROR_NONE != err)
                        {
                            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                                    err, "video_meta_update_to_db()").c_str());
                            ret = false;
                        }

                        err = video_meta_destroy(video);
                        if(MEDIA_CONTENT_ERROR_NONE != err)
                        {
                            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                                    err, "video_meta_destroy()").c_str());
                        }
                    }
                    else
                    {
                        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                                err, "media_info_get_video()").c_str());
                    }
                }
            }

            err = media_info_update_to_db(info);
            if(MEDIA_CONTENT_ERROR_NONE != err)
            {
                LOGE("%s", ContentUtility::getMediaContentLogMessage(
                        err, "media_info_update_to_db()").c_str());
                ret = false;
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "media_info_get_media_from_db()").c_str());
        }
        int error = media_info_destroy(info);
        if (error != MEDIA_CONTENT_ERROR_NONE) {
            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                    error, "media_info_destroy()").c_str());
        }
    }

    return ret;
}



void ContentHelper::changeCallbackWork(
        media_content_error_e error,
        int pid,
        media_content_db_update_item_type_e update_item,
        media_content_db_update_type_e update_type,
        media_content_type_e media_type,
        char *uuid,
        char *path,
        char *mime_type,
        void *user_data)
{
    ContentChangeCallbackPtrList listener = *(ContentChangeCallbackPtrList*)user_data;
    std::string err_msg;
    int err;
    if( error == MEDIA_CONTENT_ERROR_NONE)
    {

        err_msg = "";
        if( update_item == MEDIA_ITEM_FILE)
        {
            std::string condition = "MEDIA_ID=\"";
            condition += uuid;
            condition += "\"";

            ContentPtr content;
            if(update_type == MEDIA_CONTENT_INSERT ||
                    update_type == MEDIA_CONTENT_UPDATE) {

                filter_h filter = NULL;
                err = media_filter_create(&filter);
                if (MEDIA_CONTENT_ERROR_NONE == err) {

                    err = media_filter_set_condition(filter, condition.c_str(),
                            MEDIA_CONTENT_COLLATE_DEFAULT);
                    if ( MEDIA_CONTENT_ERROR_NONE != err)
                    {
                        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                                err, "media_filter_set_condition()").c_str());
                    }

                    if(media_type == MEDIA_CONTENT_TYPE_IMAGE) {
                        content = ContentPtr(new ImageContent());
                    }
                    else if(media_type == MEDIA_CONTENT_TYPE_VIDEO) {
                        content = ContentPtr(new VideoContent());
                    }
                    else if(media_type == MEDIA_CONTENT_TYPE_SOUND ||
                            media_type == MEDIA_CONTENT_TYPE_MUSIC) {
                        content = ContentPtr(new AudioContent());
                    }
                    else {
                        content = ContentPtr(new Content());
                    }

                    ContentHolder* content_ptr_holder = new ContentHolder();
                    content_ptr_holder->ptr = content;

                    err = media_info_foreach_media_from_db(
                            filter, loadContent, (void*)content_ptr_holder);
                    if( MEDIA_CONTENT_ERROR_NONE == err) {

                        content->setContentURI(ContentUtility::convertPathToUri(content->getContentURI()));
                        ContentChangeCallbackPtrList::iterator itr = listener.begin();
                        while( itr != listener.end() ) {
                            ContentChangeCallbackPtr callback = *itr;
                            if(callback) {
                                if(!GlobalContextManager::getInstance()->isAliveGlobalContext(
                                        callback->getContext())) {
                                    LOGW("context was closed, so skip");
                                    ++itr;
                                    continue;
                                }

                                if(update_type == MEDIA_CONTENT_INSERT) {
                                    callback->oncontentadded(content);
                                }
                                else {
                                    callback->oncontentupdated(content);
                                }
                            }
                            ++itr;
                        }
                    }
                    else {
                        delete content_ptr_holder;
                        content_ptr_holder = NULL;

                        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                                err, "media_info_foreach_media_from_db()").c_str());

                    }
                    err = media_filter_destroy(filter);
                    if (MEDIA_CONTENT_ERROR_NONE != err) {
                        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                                err, "media_filter_destroy()").c_str());
                    }
                }
                else
                {
                    LOGW("%s", ContentUtility::getMediaContentLogMessage(
                            err, "media_filter_create()").c_str());
                }
            }
            else if(update_type == MEDIA_CONTENT_DELETE)
            {
                ContentChangeCallbackPtrList::iterator itr = listener.begin();
                while( itr != listener.end() ) {
                    ContentChangeCallbackPtr callback = *itr;
                    if(callback) {
                        if(!GlobalContextManager::getInstance()->isAliveGlobalContext(
                                callback->getContext())) {
                            LOGW("context was closed, so skip");
                            ++itr;
                            continue;
                        }

                        callback->oncontentremoved(uuid);
                    }
                    ++itr;
                }
            }
        }
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                error, "media_content_set_db_updated_cb()").c_str());
    }

}



int ContentHelper::get_utc_offset()
{
    time_t zero = 24*60*60L;
    struct tm * timeptr;
    int gmtime_hours;

    /* get the local time for Jan 2, 1900 00:00 UTC */
    timeptr = localtime( &zero );
    gmtime_hours = timeptr->tm_hour;

    /* if the local time is the "day before" the UTC, subtract 24 hours
    from the hours to get the UTC offset */
    if( timeptr->tm_mday < 2 )
        gmtime_hours -= 24;

    return gmtime_hours;

}

bool ContentHelper::loadContent(media_info_h info, void* user_data)
{
    ContentHolder* hold = static_cast<ContentHolder*>(user_data);
    media_content_type_e type;

    int err;
    err = media_info_get_media_type(info, &type);
    if ( MEDIA_CONTENT_ERROR_NONE == err) {
        if ( type == MEDIA_CONTENT_TYPE_IMAGE) {
            loadImageContentFromPlatform(info,
                    std::dynamic_pointer_cast<ImageContent>(hold->ptr));
        }
        else if ( type == MEDIA_CONTENT_TYPE_VIDEO) {
            loadVideoContentFromPlatform(info,
                    std::dynamic_pointer_cast<VideoContent>(hold->ptr));
        }
        else if ( type == MEDIA_CONTENT_TYPE_MUSIC || type == MEDIA_CONTENT_TYPE_SOUND) {
            loadAudioContentFromPlatform(info,
                    std::dynamic_pointer_cast<AudioContent>(hold->ptr));
        }
        else if( type == MEDIA_CONTENT_TYPE_OTHERS) {
            loadContentFromPlatform(info, hold->ptr);
        }
    }
    else
    {
        LOGE("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_media_type()").c_str());
    }

    delete hold;
    hold = NULL;
    return false;
}

void ContentHelper::loadContentFromPlatform(media_info_h info, ContentPtr content)
{
    if(!content)
    {
        LOGE("Content creation is failed.");
        return;
    }

    if(info == NULL)
    {
        LOGE("find operation is failed.");
        return;
    }

    char* tmpStr = NULL;
    int tmpInt = 0;
    unsigned long long tmplong = 0;
    bool tmpBool = false;

    time_t tmpDate;


    int err = media_info_get_media_id(info, &tmpStr);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        if(tmpStr)
        {
            content->setId(tmpStr);
            free(tmpStr);
            tmpStr = NULL;
        }
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_media_id()").c_str());
    }

    err = media_info_get_mime_type(info, &tmpStr);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        if(tmpStr)
        {
            content->setMimeType(tmpStr);
            free(tmpStr);
            tmpStr = NULL;
        }
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_mime_type()").c_str());
    }

    err = media_info_get_display_name(info, &tmpStr);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        if(tmpStr)
        {
            content->setName(tmpStr);
            free(tmpStr);
            tmpStr = NULL;
        }
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_display_name()").c_str());
    }

    err = media_info_get_title(info, &tmpStr);
    if (MEDIA_CONTENT_ERROR_NONE == err)
    {
        if (tmpStr)
        {
            content->setTitle(tmpStr);
            free(tmpStr);
            tmpStr = NULL;
        }
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_title()").c_str());
    }

    err = media_info_get_file_path(info, &tmpStr);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        if(tmpStr)
        {
            content->setContentURI(ContentUtility::convertPathToUri(tmpStr));
            free(tmpStr);
            tmpStr = NULL;
        }
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_file_path()").c_str());
    }

    err = media_info_get_thumbnail_path (info, &tmpStr);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        if(tmpStr)
        {
            std::vector<std::string> thumbnailURIs;
            thumbnailURIs.push_back(ContentUtility::convertPathToUri(tmpStr));
            content->setThumbnailURIs(thumbnailURIs);
            free(tmpStr);
            tmpStr = NULL;
        }
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_thumbnail_path()").c_str());
    }

    err = media_info_get_description(info, &tmpStr);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        if(tmpStr)
        {
            content->setDescription(tmpStr);
            free(tmpStr);
            tmpStr = NULL;
        }
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_description()").c_str());
    }

    err = media_info_get_modified_time(info, &tmpDate);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        if(tmpDate)
        {
            content->setModifiedDate(tmpDate);
        }
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_modified_time()").c_str());
    }

    err = media_info_get_rating(info, &tmpInt);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        content->setRating(tmpInt);
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_rating()").c_str());
    }

    err = media_info_get_favorite(info, &tmpBool);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        content->setIsFavorite(tmpBool);
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_favorite()").c_str());
    }

    err = media_info_get_size(info, &tmplong);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        content->setSize(tmplong);
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_size()").c_str());
    }
}

ImageContentPtr ContentHelper::loadImageContentFromPlatform(media_info_h info,
        ImageContentPtr image)
{

    if(!image)
    {
        LOGE("Image Content creation is failed.");
        return ImageContentPtr();
    }

    if(info == NULL)
    {
        LOGE("find operation is failed.");
        return ImageContentPtr();
    }

    char* tmpStr = NULL;
    int tmpInt = 0;
    double tmpDouble;

    image_meta_h img;

    ContentHelper::loadContentFromPlatform(info, image);

    int err = media_info_get_image(info, &img);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        //created time
        err = image_meta_get_date_taken(img, &tmpStr);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            if(tmpStr )
            {
                struct tm *result = (struct tm *)calloc(1, sizeof(struct tm));
                if(strptime(tmpStr, "%Y:%m:%d %H:%M:%S", result) == NULL)
                {
                    LOGE( "Couldn't convert supplied date.");
                }

                time_t t = mktime( result );// + get_utc_offset() * 3600;
                image->setReleaseDate(t);
                free(tmpStr);
                free(result);
                tmpStr = NULL;
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "image_meta_get_date_taken()").c_str());
        }

        err = image_meta_get_width(img, &tmpInt);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            image->setWidth(tmpInt);
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "image_meta_get_width()").c_str());
        }

        err = image_meta_get_height(img, &tmpInt);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            image->setHeight(tmpInt);
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "image_meta_get_height()").c_str());
        }

        err = media_info_get_latitude(info, &tmpDouble);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            LOGD("Latitude: %f", tmpDouble);
            if (UNDEFINED_GEO != tmpDouble) {
                image->setLatitude(tmpDouble);
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "media_info_get_latitude()").c_str());
        }

        err = media_info_get_longitude(info, &tmpDouble);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            LOGD("Longitude: %f", tmpDouble);
            if (UNDEFINED_GEO != tmpDouble) {
                image->setLongitude(tmpDouble);
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "media_info_get_longitude()").c_str());
        }

        media_content_orientation_e orientation;
        err = image_meta_get_orientation(img, &orientation);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            image->setOrientation(orientation);
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "image_meta_get_orientation()").c_str());
        }

        err = image_meta_destroy(img);
        if(MEDIA_CONTENT_ERROR_NONE != err)
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "image_meta_destroy()").c_str());
        }
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_image()").c_str());
    }
    return image;
}

VideoContentPtr ContentHelper::loadVideoContentFromPlatform(media_info_h info,
        VideoContentPtr videoPtr)
{

    if(!videoPtr)
    {
        LOGE("Video Content creation is failed.");
        return VideoContentPtr();
    }

    if(info == NULL)
    {
        LOGE("find operation is failed.");
        return VideoContentPtr();
    }

    char* tmpStr;
    int tmpInt = 0;
    double tmpDouble;

    video_meta_h video;

    ContentHelper::loadContentFromPlatform(info, videoPtr);

    int err = media_info_get_video(info, &video);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        err = video_meta_get_recorded_date(video, &tmpStr);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            if(tmpStr)
            {
                struct tm *result=(struct tm *)calloc(1, sizeof(struct tm));
                if (strptime(tmpStr, "%Y:%m:%d %H:%M:%S", result) == NULL) {
                    LOGE( "Couldn't convert supplied date.");
                }
                time_t t = mktime( result ) + get_utc_offset() * 3600;
                videoPtr->setReleaseDate(t);
                free(tmpStr);
                free(result);
                tmpStr = NULL;
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "video_meta_get_recorded_date()").c_str());
        }

        err = video_meta_get_album(video, &tmpStr);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            if(tmpStr)
            {
                videoPtr->setAlbum(tmpStr);
                free(tmpStr);
                tmpStr = NULL;
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "video_meta_get_album()").c_str());
        }

        err = video_meta_get_artist(video, &tmpStr);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            if(tmpStr)
            {
                std::vector<std::string> artists = WrtDeviceApis::Commons::String::split(tmpStr, TAG_DELIMETER);
                videoPtr->setArtists(artists);
                free(tmpStr);
                tmpStr = NULL;
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "video_meta_get_artist()").c_str());
        }

        err = media_info_get_latitude(info, &tmpDouble);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            LOGD("Latitude: %f", tmpDouble);
            if (UNDEFINED_GEO != tmpDouble) {
                videoPtr->setLatitude(tmpDouble);
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "media_info_get_latitude()").c_str());
        }

        err = media_info_get_longitude(info, &tmpDouble);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            LOGD("Longitude: %f", tmpDouble);
            if (UNDEFINED_GEO != tmpDouble) {
                videoPtr->setLongitude(tmpDouble);
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "media_info_get_longitude()").c_str());
        }

        err = video_meta_get_width(video, &tmpInt);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            videoPtr->setWidth(tmpInt);
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "video_meta_get_width()").c_str());
        }

        err = video_meta_get_height(video, &tmpInt);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            videoPtr->setHeight(tmpInt);
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "video_meta_get_height()").c_str());
        }

        err = video_meta_get_duration(video, &tmpInt);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            videoPtr->setDuration(tmpInt);
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "video_meta_get_duration()").c_str());
        }

        err = video_meta_destroy(video);
        if(MEDIA_CONTENT_ERROR_NONE != err)
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "video_meta_destroy()").c_str());
        }
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_video()").c_str());
    }

    return videoPtr;
}

AudioContentPtr ContentHelper::loadAudioContentFromPlatform(media_info_h info,
        AudioContentPtr audioPtr)
{
    if(!audioPtr)
    {
        LOGE("Video Content creation is failed.");
        return AudioContentPtr();
    }

    if(info == NULL)
    {
        LOGE("find operation is failed.");
        return AudioContentPtr();
    }

    audio_meta_h audio;

    char* tmpStr;
    int tmpInt = 0;

    ContentHelper::loadContentFromPlatform(info, audioPtr);

    int err = media_info_get_audio(info, &audio);
    if(MEDIA_CONTENT_ERROR_NONE == err)
    {
        err = audio_meta_get_recorded_date(audio, &tmpStr);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            if(tmpStr)
            {
                struct tm *result = (struct tm *)calloc(1, sizeof(struct tm));
                if (strptime(tmpStr, "%Y:%m:%d %H:%M:%S", result) == NULL)
                {
                    LOGE( "Couldn't convert supplied date.");
                }
                time_t t = mktime( result ) + get_utc_offset() * 3600;
                audioPtr->setReleaseDate(t);
                free(tmpStr);
                free(result);
                tmpStr = NULL;
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "audio_meta_get_recorded_date()").c_str());
        }

        err = audio_meta_get_album(audio, &tmpStr);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            if(tmpStr)
            {
                audioPtr->setAlbum(tmpStr);
                free(tmpStr);
                tmpStr = NULL;
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "audio_meta_get_album()").c_str());
        }

        err = audio_meta_get_artist(audio, &tmpStr);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            if(tmpStr)
            {
                std::vector<std::string> artists = String::split(tmpStr, TAG_DELIMETER);
                audioPtr->setArtists(artists);
                free(tmpStr);
                tmpStr = NULL;
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "audio_meta_get_artist()").c_str());
        }

        err = audio_meta_get_composer(audio, &tmpStr);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            if(tmpStr)
            {
                std::vector<std::string> composers = String::split(tmpStr, TAG_DELIMETER);
                audioPtr->setComposers(composers);
                free(tmpStr);
                tmpStr = NULL;
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "audio_meta_get_composer()").c_str());
        }

        err = audio_meta_get_duration(audio, &tmpInt);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            audioPtr->setDuration(tmpInt);
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "audio_meta_get_duration()").c_str());
        }

        err = audio_meta_get_copyright(audio, &tmpStr);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            if(tmpStr)
            {
                audioPtr->setCopyright(tmpStr);
                free(tmpStr);
                tmpStr = NULL;
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "audio_meta_get_copyright()").c_str());
        }

        err = audio_meta_get_track_num(audio, &tmpStr);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            if(tmpStr)
            {
                audioPtr->setTrackNumber(atoi(tmpStr));
                free(tmpStr);
                tmpStr = NULL;
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "audio_meta_get_track_num()").c_str());
        }

        err = audio_meta_get_bit_rate(audio, &tmpInt);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            audioPtr->setBitrate(tmpInt);
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "audio_meta_get_bit_rate()").c_str());
        }

        err = audio_meta_get_genre(audio, &tmpStr);
        if(MEDIA_CONTENT_ERROR_NONE == err)
        {
            if(tmpStr)
            {
                std::vector<std::string> genres = String::split(tmpStr, TAG_DELIMETER);

                audioPtr->setGenres(genres);
                free(tmpStr);
                tmpStr = NULL;
            }
        }
        else
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "audio_meta_get_genre()").c_str());
        }

        err = audio_meta_destroy(audio);
        if(MEDIA_CONTENT_ERROR_NONE != err)
        {
            LOGW("%s", ContentUtility::getMediaContentLogMessage(
                    err, "audio_meta_destroy()").c_str());
        }
    }
    else
    {
        LOGW("%s", ContentUtility::getMediaContentLogMessage(
                err, "media_info_get_audio()").c_str());
    }

    return audioPtr;
}



} // Content
} // DeviceAPI
