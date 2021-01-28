/** @file NERecord-demo.cpp
  * @author Leeyong
  * @date 2021/01/27
  * @copyright (c) 2015-2021, NetEase Inc. All rights reserved
  * @brief 这个代码示例项目演示了如何快速集成 网易云信 Linux本地服务器录制（NERecord）SDK，实现单流自动录制功能。
  * 
  * 功能包含
  * 
  *    初始化录制引擎
  *    加入房间
  *    后台自动录制
  *    离开房间 
  * 
  * demo编译完成后运行方法
  * 
  *    path/of/NERecord-demo  \
                              -c ${CHANNEL_NAME} \
                              -u ${UID} \
                              -k ${APP_KEY} \
                              -o ${LOG_DIR} \
                              -t ${TOKEN} \
                              -e ${RTC_ENGINE_SO_PATH}
  */

#include <nerecord_engine.h>
#include <nerecord_engine_event_handler.h>

#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string>
#include <thread>

static struct
{
    std::string rtc_engine;
    std::string log_dir;
    std::string appkey;
    std::string channel;
    std::string token;
    uint64_t uid;
} cmdline_arg;


static void print_cmdline_arg()
{
    printf("Command Line arguments: \n");
    printf("\t rtc_engine: %s\n", cmdline_arg.rtc_engine.c_str());
    printf("\t log_dir: %s\n", cmdline_arg.log_dir.c_str());
    printf("\t appkey: %s\n", cmdline_arg.appkey.c_str());
    printf("\t token: %s\n", cmdline_arg.token.c_str());
    printf("\t channel: %s\n", cmdline_arg.channel.c_str());
    printf("\t uid: %lu\n", cmdline_arg.uid);
}

static const char *short_options = "o:k:c:t:u:e:";
static  const struct option long_options[] = {
    {"log",             1,  NULL, 'o' },
    {"appkey",      1,   NULL, 'k'},
    {"channel",           1,   NULL,   'c' },
    {"token",       1, NULL, 't'},
    {"uid",             1, NULL, 'u'},
    {"rtc-engine",             1, NULL, 'e'},
    {  NULL,      0,    NULL,   0  }
};

static void argment(int argc, char **argv)
{
    int c; 
    int sample_rate = 48000;
    int channel = 1;

      while((c = getopt_long (argc, argv, short_options, long_options, NULL)) != -1)      
      {
         switch (c)
         {
            case 'e':
            cmdline_arg.rtc_engine = optarg;
            break;

            case 'o':
            cmdline_arg.log_dir = optarg;
            break;

            case 'k':
            cmdline_arg.appkey = optarg;
            break;

            case 'c':
            cmdline_arg.channel = optarg;
            break;

            case 't':
            cmdline_arg.token = optarg;
            break;

            case 'u':
            {
                uint64_t uid;
                if(1 == sscanf(optarg, "%lu", &uid))
                {
                    cmdline_arg.uid = uid;
                }
            }
            break;

            default:
               printf("unknow option: %c!\n", c);
               break;
         }
      }
}


int main(int argc, char **argv)
{
    int ret = nerecord::kNERtcNoError;

    argment(argc, argv);
    printf("NERecord demo \n\n");
    print_cmdline_arg();
    printf("\n\n");

    #define CHECK_RETVAL(r) do { \
        ret = (r); \
        if (nerecord::kNERtcNoError != ret) \
        { \
            printf("%s: error code = %d\n", #r, ret);\
            goto out; \
        } \
    } while(0)

    /* 创建NERecord引擎实例 */
    nerecord::IRecordEngine *engine =  (nerecord::IRecordEngine *)createNERecordEngine();

    /* 配置初始化参数:  自动模式的单流录制, 混合音视频*/
    nerecord::NERecordEngineContext ctx;
    ctx.submode = nerecord::RECORDER_MODE; /* 使用录制功能必须将submode赋值为nerecord::RECORDER_MODE */
    ctx.nerecord_absolute_dir = cmdline_arg.rtc_engine.c_str();
    ctx.log_dir_path = cmdline_arg.log_dir.c_str();
    ctx.record_file_root_dir = cmdline_arg.log_dir.c_str();
    ctx.app_key = cmdline_arg.appkey.c_str();
    /* 自动开始录制 */
    ctx.auto_subscribe_audio = true;
    ctx.auto_subscribe_video = true;
    /* 使能音视频 */
    ctx.audio_enable = true;
    ctx.video_enable = true;
    /* 单流录制 */
    ctx.mixing_enable = false;
    /* 混合音视频, 音频和视频在同一个文件 */
    ctx.mixed_audio_video = true;

    /* 初始化NERecord引擎 */
    CHECK_RETVAL(engine->initialize(ctx));
    /* 加入房间 */
    CHECK_RETVAL(engine->joinChannel(cmdline_arg.token.c_str(), cmdline_arg.channel.c_str(), cmdline_arg.uid));

    /* 后台自动录制中, 阻塞主线程, 用户输入回车键后再退出 */
    std::this_thread::sleep_for(std::chrono::seconds(2));
    printf("\n\nPress \"Enter\" key to stop\n\n\n");
    getchar();

    /* 离开房间 */
    CHECK_RETVAL(engine->leaveChannel());
out:
    /* 释放NERecord实例 */
    if (nullptr != engine)
    {
        engine->release();
        destroyNERecordEngine((void *&)engine);
    }
    return ret;
}
