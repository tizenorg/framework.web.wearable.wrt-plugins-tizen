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

#ifndef __TIZEN_COMMON_TIME_TRACER__
#define __TIZEN_COMMON_TIME_TRACER__

#ifdef __cplusplus

#if defined(ENABLE_TIME_TRACER) && defined(__cplusplus)
#include <string>
#endif

extern "C" {
#endif

typedef struct _time_tracer_item
{
	char* name;
	long long total_elapsed_time;
	long long min_elapsed_time;
	long long max_elapsed_time;
	long long first_timestamp;
	long long last_timestamp;
	long long timestamp;
	int on_tracing;
	long long invoke_count;
	long long mismatch_count;
} time_tracer_item;

/* COMMON */
int time_tracer_init(void);
int time_tracer_release(void);
void time_tracer_export_report(int direction, char* name);
int time_tracer_item_begin(const char* name, int show,const  char* filename, int line);
int time_tracer_item_end(const char* name, int show, const char* filename, int line);

#define TIME_TRACER_EXPORT_STDOUT	0
#define TIME_TRACER_EXPORT_STDERR	1
#define TIME_TRACER_EXPORT_FILE	2

#ifdef ENABLE_TIME_TRACER
//Initialize the time tracer
#define TIME_TRACER_INIT()	(time_tracer_init())
//Release the time tracer
#define TIME_TRACER_RELEASE()	(time_tracer_release())
//Export tracing report to stdout
#define TIME_TRACER_EXPORT_REPORT()		(time_tracer_export_report(TIME_TRACER_EXPORT_STDOUT,NULL))
//Export tracing report to user specific direction (stdout, stderr, file)
#define TIME_TRACER_EXPORT_REPORT_TO(x,y)	(time_tracer_export_report(x,y))
//Set tracing point to begin
#define TIME_TRACER_ITEM_BEGIN(name,show)	(time_tracer_item_begin(name,show,__FILE__,__LINE__))
//Set tracing point to end
#define TIME_TRACER_ITEM_END(name,show)	(time_tracer_item_end(name,show,__FILE__,__LINE__)	)
#else
#define TIME_TRACER_INIT()
#define TIME_TRACER_RELEASE()
#define TIME_TRACER_EXPORT_REPORT()
#define TIME_TRACER_EXPORT_REPORT_TO(x,y)
#define TIME_TRACER_ITEM_BEGIN(name,show)
#define TIME_TRACER_ITEM_END(name,show)
#endif

#ifdef __cplusplus
}
#endif

#if defined(ENABLE_TIME_TRACER) && defined(__cplusplus)
class TimeTracerItem {
public:
    TimeTracerItem(const char* file, const char* function, int line, int show)
        : m_file(file), m_function(function), m_show(show)
    {
        m_name = formatName(file, function);
        time_tracer_item_begin(m_name.c_str(), show, file, line);
    }

    virtual ~TimeTracerItem()
    {
        time_tracer_item_end(m_name.c_str(), m_show, m_file, 0);
    }

private:
    static std::string formatName(const char* file, const char* function)
    {
        static const std::string s_src_path_pattern("/src/");
        std::string filestr(file);
        size_t found_at = filestr.find(s_src_path_pattern);

        if(found_at != std::string::npos) {
            filestr = filestr.substr(found_at + s_src_path_pattern.length());
        }
        return filestr + ":" + std::string(function);
    }

    const char* m_file;
    const char* m_function;
    std::string m_name;
    int m_show;
};

#define SET_TIME_TRACER_ITEM(_show_) TimeTracerItem _time_tracer_item_(__FILE__, __FUNCTION__, __LINE__ , _show_)
#else
#define SET_TIME_TRACER_ITEM(_show_)
#endif // ENABLE_TIME_TRACER

#endif //__TIZEN_COMMON_TIME_TRACER__
