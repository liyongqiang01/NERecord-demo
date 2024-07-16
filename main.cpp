
#include "dlfcn.h"
#include "nerecordEventHandlerImpl.h"
#include "nerecordPacketObserverImpl.h"
#include "signal.h"
#include <vector>
#include <sstream>
#include "cmdline.h"
#include "httpwrapper.h"

#define TOKEN_SERVER_TEST "https://webtest.netease.im/nrtcproxy/demo/getRecordSdkCheckSum.action"
#define TOKEN_SERVER_ONLINE "https://nrtc.netease.im/demo/getRecordSdkCheckSum.action"

#define CMD_DEMO
#ifdef CMD_DEMO
nerecord::IRecordEngine *engine = nullptr;
#endif


// using namespace nerecord;

// #define TEST_ENV 
static struct
{
    std::string config;
    std::string appkey;
    std::string token_server;
    int log_level;
    std::string app_dir;
    std::string encrypt_key;
    bool auto_subscribe_audio;
    bool auto_subscribe_video;
} g_config;

RecordPacketObserverImpl * package_observer = new RecordPacketObserverImpl();
nerecord::NERecordEngineContext ctx;
std::map<std::string, nerecord::IRecordEngine *> engine_list;


std::vector<uint64_t> split_to_uint64(const std::string &s, char delimiter) {
    std::vector<uint64_t> tokens;
    std::string token;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != std::string::npos) {
        token = s.substr(start, end - start);
        tokens.push_back(std::stoull(token));
        start = end + 1;
        end = s.find(delimiter, start);
    }
    tokens.push_back(std::stoull(s.substr(start)));
    return tokens;
}

void join_channel_with_request_token(const std::string &engine_name, const std::string &appkey, const std::string &cname, uint64_t uid)
{
    printf("appkey:%s, cname:%s, uid:%lu, server:%s\n", appkey.c_str(), cname.c_str(), uid, g_config.token_server.c_str());
    std::string token  = HttpWrapper::getInstance().GetCheckSum(appkey, cname, uid, g_config.token_server);
    engine->joinChannel(token.c_str(), cname.c_str(), uid);
    printf("token:%s\n", token.c_str());
}

#define PATH_MAX 1024
static std::string GetExeDirPath() {
  char buf[PATH_MAX];
  int ret = readlink("/proc/self/exe", buf, PATH_MAX);
  if (ret < 0 || (ret >= PATH_MAX)) { // error or truncated
      return "";
  } else {
    auto SplitFileName = [](const std::string &str)->std::string {
      auto found = str.find_last_of('/');
      return str.substr(0, found);
    };
    
    return SplitFileName(std::string(buf));
  }
}

int read_config()
{
    std::ifstream ifstream(std::string(GetExeDirPath() + "/nrtc_config.txt"));
    std::string cfg_string((std::istreambuf_iterator<char>(ifstream)), std::istreambuf_iterator<char>());
    json_value * cfg_json = json_parse(cfg_string.c_str(), cfg_string.length());

    if (NULL == cfg_json) {
        fprintf(stderr, "%s 文件格式不合法\n", (GetExeDirPath() + "/nrtc_config.txt").c_str());
        fprintf(stderr, "%s", cfg_string.c_str());
        exit(-1);
    }

    g_config.config = cfg_string;
    g_config.appkey = (const char *)(*cfg_json)["appkey"];
    g_config.token_server = (const char *)(*cfg_json)["token_server"];
    g_config.log_level = (json_int_t)(*cfg_json)["log_level"];
    g_config.app_dir = GetExeDirPath();
    g_config.encrypt_key = (const char *)(*cfg_json)["encrypt_key"];

    int auto_subscribe_audio_video = false;
    // printf("是否自动订阅音视频:(yes:1  no:0)\n");
    // scanf("%d", &auto_subscribe_audio_video);
    // getchar();
    g_config.auto_subscribe_audio = auto_subscribe_audio_video;//(bool)(*cfg_json)["auto_subscribe_audio"];
    g_config.auto_subscribe_video = auto_subscribe_audio_video;//(bool)(*cfg_json)["auto_subscribe_video"];
    json_value_free(cfg_json);
    cfg_json = nullptr;

    return 0;
}

int enableEncrypt()
{
    int encrypt_mode = 1;
    printf("输入输入加密模式:(0:国密 1:自定义加密 2:不加密)\n");
    scanf("%d", &encrypt_mode);
    getchar();
    bool enable = false;
    NERecordEncryptionConfig config;
    if(0 == encrypt_mode) {
        enable = true;
        config.mode = kNERecordGMCryptoSM4ECB;
        if(kNERecordEncryptByteLength < g_config.encrypt_key.length())
        {
            g_config.encrypt_key = g_config.encrypt_key.substr(0, kNERecordEncryptByteLength);
        }

        printf("use encrypt key: %s\n", g_config.encrypt_key.c_str());
        memset(config.key, 0, kNERecordEncryptByteLength * sizeof(char));
        memcpy(config.key, g_config.encrypt_key.c_str(), g_config.encrypt_key.size() * sizeof(char));
    } else if(1 == encrypt_mode) {
        enable = true;
        config.mode = NERecordEncryptionModeCustom;
        config.observer = package_observer;
    }
    engine_list["nertc_engine"]->enableEncryption(enable, config);
    return 0;
}

void startService()
{
    if(g_config.auto_subscribe_audio)
    {
        printf("自动订阅模式不需要调用!!!");
        return;
    }
    if (engine_list["nertc_engine"])
    {
        engine_list["nertc_engine"]->startService();
        printf("-----startService-----\n");
    }
}

void stopService()
{
    if(g_config.auto_subscribe_audio)
    {
        printf("自动订阅模式不需要调用!!!");
        return;
    }
    if (engine_list["nertc_engine"])
    {
        engine_list["nertc_engine"]->stopService();
        printf("-----stopService-----\n");
    }
}

void create() {
    #ifdef TEST_ENV

    int file_mode = 1;
    int stream_mode = 1;
    // str_appkey = "eca23f68c66d4acfceee77c200200359";

    #else

    int file_mode = 1;
    printf("输入录制文件路径模式(input record file dir mode) :(1 -- default, 0 -- custom)\n");
    scanf("%d", &file_mode);
    getchar();

    int stream_mode = 0;
    printf("录制小流或者大流(select video high or low):(hight-1, low-0)\n");
    scanf("%d", &stream_mode);
    getchar();

    #endif

    //init
    ctx.app_key = g_config.appkey.c_str();
    ctx.record_file_dir_mode = (file_mode == 0) ? kNERtcRecordFileDirModeCustom : kNERtcRecordFileDirModeDefault;
    ctx.log_level = NERtcLogLevel::kNERtcLogLevelDebug;
    ctx.audio_enable = true; //使能音视频,只有开启后才能进行自动订阅和手动订阅
    ctx.video_enable = true;
    ctx.video_substream_split = false; //视频辅流独立录制
    /**< 使能合流录制 */
    ctx.mixing_enable = true;//true todo
    ctx.mixed_audio_video =true; //混合音视频, 音频和视频在同一个文件
    ctx.mixed_audio_substream = false;
    ctx.snapshot_enable = false; //截取视频图像
    ctx.snapshot_interval = 5; //截取视频图像时间间隔
    ctx.decodeAudio = kNERtcAudioFormatAAC;
    ctx.decodeVideo = kNERtcVideoFormatH264;
        /* 自动开始录制 */
    printf("auto_subscribe_audio:%d auto_subscribe_video:%d \n", g_config.auto_subscribe_audio, g_config.auto_subscribe_video);
    //3.9.19版本为false
    ctx.auto_subscribe_audio = g_config.auto_subscribe_audio; //false
    ctx.auto_subscribe_video = g_config.auto_subscribe_video; //false
    ctx.keep_last_frame = true;
    ctx.default_stream_type = (stream_mode == 0) ? kNERtcRemoteVideoStreamTypeLow : kNERtcRemoteVideoStreamTypeHigh;
    ctx.audioProfile = kNERtcAudioProfileHight;
    ctx.video_record_resolution.vanvasWidth = 1920;
    ctx.video_record_resolution.vanvasHeight = 1080;
    ctx.video_record_resolution.fps = 30;
    ctx.video_record_resolution.bps = 4725000;
    ctx.event_handler = new TestEventHandler();
    ctx.submode = nerecord::SubscribeMode::RECORDER_MODE;

    printf("ctx.appkey:%s\n", ctx.app_key);
    printf("log_dir_path:%s\n", (g_config.app_dir + "/Log").c_str());
    printf("video_stream_hight:%d\n", stream_mode);
}

void joinchannel()
{
    #ifdef TEST_ENV
    std::string cn = "test1111";
    uint64_t uid = 123;
    int core_num = 6;
    #else

    char cn[100];
    printf("输入房间号(input channel name)\n");
    scanf("%s", cn);
    getchar();
    uint64_t uid = 0;
    printf("输入uid(input uid)\n");
    scanf("%lu", &uid);
    getchar();
    int core_num = 1;
    printf("输入录制核心数(input record hardware concurrency):(default 1)\n");
    scanf("%d", &core_num);
    getchar();

    #endif

    std::string log_dir = g_config.app_dir + "/Log/";
    std::cout << "log_dir: " << log_dir << std::endl;
    ctx.log_dir_path = log_dir.c_str();
    ctx.record_file_root_dir = log_dir.c_str();
    std::string parameters = R"({"sdk.enable.debug.environment": true, 
        "auto_subscribe_audio": false, 
        "auto_subscribe_video": false, 
        "core_num":)" + std::to_string(core_num) + "}";

    std::cout << parameters << std::endl;
    ctx.parameters = parameters.c_str();

    ctx.audioProfile = kNERtcAudioProfileDefault;
    engine_list["nertc_engine"] = (nerecord::IRecordEngine *)createNERecordEngine();
    if (engine_list["nertc_engine"])
    {
        int ret = engine_list["nertc_engine"]->initialize(ctx);
        if (ret != 0) {
            printf(">>>initialize: %d\n", ret);
        } else {
            ret = enableEncrypt();
            printf("%s\n", ctx.log_dir_path);
            join_channel_with_request_token("nertc_engine", g_config.appkey.c_str(), cn, uid);
        }
    }

    // if(!g_config.auto_subscribe_audio)
    // {
    //     startService();
    // }
}

void leaveChannel()
{
    // if(!g_config.auto_subscribe_audio)
    // {
    //     stopService();
    // }

    if (engine_list["nertc_engine"])
    {
        int ret = engine_list["nertc_engine"]->leaveChannel();
        printf(">>>leavechannel: %d\n", ret);
    }
}

void subscribeRemoteAudioStream()
{
    if(g_config.auto_subscribe_audio)
    {
        printf("自动订阅模式不需要调用!!!");
        return;
    }

    uint64_t uid = 0;
    printf("输入uid(input uid)\n");
    scanf("%lu", &uid);
    getchar();

    uint64_t subscribe = 0;
    printf("输入subscribe(input subscribe 0:unsub 1:sub)\n");
    scanf("%lu", &subscribe);
    getchar();
    if (engine_list["nertc_engine"])
    {
        engine_list["nertc_engine"]->subscribeRemoteAudioStream(uid, subscribe == 0 ? false : true);
        printf("-----stopService-----\n");
    }
}

void updateSubscribeAudioUids() {
    if(g_config.auto_subscribe_audio)
    {
        printf("自动订阅模式不需要调用!!!");
        return;
    }

    char uids[128];
    printf("输入uids (eg: uid1,uid2,uid3,uid4)\n");
    scanf("%s", uids);
    getchar();
    std::string str_uids(uids);
    std::vector<uint64_t> result = split_to_uint64(str_uids, ',');
    nerecord::uid_t * uid_array = new nerecord::uid_t[result.size()];
    int index = 0;
    for (uint64_t uid : result) {
        std::cout << uid << std::endl;
        *(uid_array + index++ ) = uid;
    }
    if (engine_list["nertc_engine"])
    {
        engine_list["nertc_engine"]->updateSubscribeAudioUids(uid_array, result.size());
        printf("-----updateSubscribeAudioUids end-----\n");
    }
}

void subscribeRemoteVideoStream()
{
    if(g_config.auto_subscribe_audio)
    {
        printf("自动订阅模式不需要调用!!!");
        return;
    }

    uint64_t uid = 0;
    printf("输入uid(input uid)\n");
    scanf("%lu", &uid);
    getchar();

    int stream_mode = 0;
    printf("录制小流或者大流(select video high or low):(hight-1, low-0)\n");
    scanf("%d", &stream_mode);
    getchar();

    int subscribe = 0;
    printf("输入subscribe(input subscribe 0:unsub 1:sub)\n");
    scanf("%d", &subscribe);
    getchar();
    if (engine_list["nertc_engine"])
    {
        engine_list["nertc_engine"]->subscribeRemoteVideoStream(uid, 
                                    (stream_mode == 0) ? kNERtcRemoteVideoStreamTypeLow : kNERtcRemoteVideoStreamTypeHigh, 
                                    subscribe == 0 ? false : true);
        printf("-----subscribeRemoteVideoStream end-----\n");
    }
}

void subscribeRemoteVideoSubStream()
{
    if(g_config.auto_subscribe_audio)
    {
        printf("自动订阅模式不需要调用!!!");
        return;
    }

    uint64_t uid = 0;
    printf("输入uid(input uid)\n");
    scanf("%lu", &uid);
    getchar();

    int subscribe = 0;
    printf("输入subscribe(input subscribe 0:unsub 1:sub)\n");
    scanf("%d", &subscribe);
    getchar();

    if (engine_list["nertc_engine"])
    {
        engine_list["nertc_engine"]->subscribeRemoteVideoSubStream(uid, subscribe == 0 ? false : true);
        printf("-----subscribeRemoteVideoSubStream end-----\n");
    }
}

void updateSubscribeVideoUids() {
    if(g_config.auto_subscribe_audio)
    {
        printf("自动订阅模式不需要调用!!!");
        return;
    }
    
    char uids[128];
    printf("输入uids (eg: uid1,uid2,uid3,uid4)\n");
    scanf("%s", uids);
    getchar();
    std::string str_uids(uids);
    std::vector<uint64_t> result = split_to_uint64(str_uids, ',');
    nerecord::uid_t * uid_array = new nerecord::uid_t[result.size()];
    int index = 0;
    for (uint64_t uid : result) {
        std::cout << uid << std::endl;
        *(uid_array + index++ ) = uid;
    }
    if (engine_list["nertc_engine"])
    {
        engine_list["nertc_engine"]->updateSubscribeVideoUids(uid_array, result.size());
        printf("-----updateSubscribeVideoUids end-----\n");
    }
}


void release()
{
    if (engine_list["nertc_engine"])
    {
        engine_list["nertc_engine"]->release();
        printf(">>>release\n");
    }
}

void setLayout()
{
#ifndef CMD_DEMO
    char path[256];
    int file_type = 0;
    printf("选择布局文件 (1 --- layout.txt  2 --- layout_sub.txt)\n");
    scanf("%d", &file_type);
    getchar();
    std::string file_name = g_config.app_dir + "/layout.txt";
    if(file_type == 2) {
        file_name = g_config.app_dir + "/layout_sub.txt";
    }
    printf("input layout file path:%s\n", file_name.c_str());
    // memset(path, 0, 256 * sizeof(char));
    // memcpy(path, file_name.c_str(), file_name.size() * sizeof(char));
    FILE *fp = fopen(file_name.c_str(), "r");
#else
    char path[256];
    std::string file_name = GetExeDirPath() + "/layout.txt";
    FILE *fp = fopen(file_name.c_str(), "r");
#endif
    VideoMixingLayout layout;
    NERtcRegion regions[32];

#ifndef CMD_DEMO
    int layout_type = 0;
    printf("布局模式 (0: 自适应  1: 用户自定义)\n");
    scanf("%d", &layout_type);
    getchar();
    layout.layout = layout_type == 0 ? kNERtcLayoutGrid : kNERtcLayoutCustom;
#else
    layout.layout = kNERtcLayoutCustom;
#endif
    uint64_t uid;
    int substream;
    float x, y, w, h, alpha;
    char renderMode[32];
    int i = 0;
    if (fp)
    {
        /* 忽略第一行 */
        char line[256];
        fgets(line, sizeof(line), fp); 
        for (; i < 32; )
        {
            if (8 == fscanf(fp, "%lu,%d,%f,%f,%f,%f,%f,%s", &uid, &substream, &x, &y, &w, &h, &alpha, renderMode))
            {
                regions[i].uid = uid;
                regions[i].substream = substream;
                regions[i].x = x;
                regions[i].y = y;
                regions[i].width = w;
                regions[i].height = h;
                regions[i].alpha = alpha;
                regions[i].renderMode = (0 == strcmp(renderMode, "fit"))? nerecord::kNERtcRenderModeFit : nerecord::kNERtcRenderModeHidden;
                i++;
            }
            else
            {
                break;
            }
        }
        fclose(fp);
    }
    NERtcRegion copy_regions[i];
    for (int j = 0; j < i; ++j)
    {
        copy_regions[j].uid = regions[j].uid;
        copy_regions[j].substream = regions[j].substream;
        copy_regions[j].x = regions[j].x;
        copy_regions[j].y = regions[j].y;
        copy_regions[j].width = regions[j].width;
        copy_regions[j].height = regions[j].height;
        copy_regions[j].alpha = regions[j].alpha;
        copy_regions[j].renderMode = regions[j].renderMode;
    }

    layout.regionCount = i;
    layout.regions = copy_regions;
#ifndef CMD_DEMO   
    if (engine_list["nertc_engine"])
    {
        printf(">>>setVideoMixingLayout layout num: %d\n", layout.regionCount);
        engine_list["nertc_engine"]->setVideoMixingLayout(layout);
        printf(">>>setVideoMixingLayout end\n");
    }
#else
    if(engine)
    {
        printf(">>>setVideoMixingLayout layout num: %d\n", layout.regionCount);
        engine->setVideoMixingLayout(layout);
        printf(">>>setVideoMixingLayout end\n");
    }
#endif
}

void setWaterMark()
{
    WatermarkConfig wc;
    wc.type = kNERtcWatermarkTypeImage;
    std::string image_path = g_config.app_dir + "/watermark_1920_1080.png";
    std::cout << "image_path: " << image_path << std::endl;
    wc.config.image.image_path = image_path.c_str();
    wc.config.image.offset_x = 0;
    wc.config.image.offset_y = 0;
    wc.config.image.wm_width = 1920;
    wc.config.image.wm_height = 1080;
    if (engine_list["nertc_engine"]) {
        engine_list["nertc_engine"]->updateWatermarkConfigs(1, &wc);
        printf(">>>updateWatermarkConfigs, %s\n", wc.config.image.image_path);
    }
}

void startMixingRecord()
{
    if (engine_list["nertc_engine"])
    {
        engine_list["nertc_engine"]->startMixingRecord();
        printf(">>>startMixingRecord\n");
    }
}

void stopMixingRecord()
{
    if (engine_list["nertc_engine"])
    {
        engine_list["nertc_engine"]->stopMixingRecord();
        printf(">>>stopMixingRecord\n");
    }
}


int main(int argc, char** argv)
{
    cmdline::parser arg_parser;
    arg_parser.add<std::string>("appkey", 'a', "appkey", true, "6acf024e190215b685905444b6e57dd7");
    arg_parser.add<std::string>("cname", 'n', "channel name", true, "5566");
    arg_parser.add<int64_t>("uid", 'u', "user id", true, 1122);
    arg_parser.add<std::string>("tokenserver", 's', "token server", false, "https://nrtc.netease.im/demo/getRecordSdkCheckSum.action");
    arg_parser.add<uint64_t>("runtime", 't', "demo run time, minutes, 0 represent run forever", false, 0);

    arg_parser.parse_check(argc, argv);

    std::string appkey = arg_parser.get<std::string>("appkey");
    std::string cname = arg_parser.get<std::string>("cname");
    int64_t uid = arg_parser.get<int64_t>("uid");
    std::string tokenserver = arg_parser.get<std::string>("tokenserver");
    uint64_t runtime = arg_parser.get<uint64_t>("runtime");;
    std::cout << "appkey: " << appkey << std::endl;
    std::cout << "cname: " << cname << std::endl;
    std::cout << "uid: " << uid << std::endl;
    std::cout << "tokenserver: " << tokenserver << std::endl;
    std::cout << "runtime: " << runtime << std::endl;
    tokenserver = TOKEN_SERVER_ONLINE;
    std::cout << "tokenserver default: " << tokenserver << std::endl;

    engine =  (nerecord::IRecordEngine *)createNERecordEngine();
    nerecord::NERecordEngineContext ctx;
    ctx.submode = nerecord::RECORDER_MODE; // 使用录制功能必须将submode赋值为nerecord::RECORDER_MODE
    ctx.record_file_dir_mode = kNERtcRecordFileDirModeDefault;
    ctx.log_level = NERtcLogLevel::kNERtcLogLevelInfo;
    std::string log_dir = GetExeDirPath() + "/Log";
    ctx.log_dir_path = log_dir.c_str();
    ctx.record_file_root_dir = log_dir.c_str();
    ctx.app_key = appkey.c_str();

    ctx.audio_enable = true; // 使能音视频
    ctx.video_enable = true;

    ctx.auto_subscribe_audio = false;
    ctx.auto_subscribe_video = false;
    ctx.mixing_enable = true; // 使能合流录制
    ctx.mixed_audio_video = true; //混合音视频, 音频和视频在同一个文件
    ctx.mixed_audio_substream = false;
    ctx.snapshot_enable = false; //截取视频图像
    ctx.snapshot_interval = 5; //截取视频图像时间间隔
    ctx.decodeAudio = kNERtcAudioFormatAAC;
    ctx.decodeVideo = kNERtcVideoFormatH264;
    ctx.keep_last_frame = true;
    ctx.default_stream_type = kNERtcRemoteVideoStreamTypeHigh;
    ctx.audioProfile = kNERtcAudioProfileHight;
    ctx.video_record_resolution.vanvasWidth = 1920;
    ctx.video_record_resolution.vanvasHeight = 1080;
    ctx.video_record_resolution.fps = 30;
    ctx.video_record_resolution.bps = 4725000;
    ctx.event_handler = new TestEventHandler();

    std::string parameters = R"({"sdk.enable.debug.environment": false, 
        "auto_subscribe_audio": false, 
        "auto_subscribe_video": false, 
        "core_num":)" + std::to_string(6) + "}";
    ctx.parameters = parameters.c_str();
    ctx.audioProfile = kNERtcAudioProfileDefault;

    int ret = 0;
    ret = engine->initialize(ctx);
    std::cout << "initialize ret: " << ret << std::endl;
    
    g_config.token_server = tokenserver;
    join_channel_with_request_token("", appkey, cname, uid);
    sleep(10);

    setLayout();

    engine->startMixingRecord();
    if (runtime == 0) {
        sleep(60 * 60 * 24 * 30);
    } else {
        sleep(60 * runtime);
    }
    engine->stopMixingRecord();

    engine->leaveChannel();
    if (nullptr != engine)
    {
        engine->release();
        destroyNERecordEngine((void *&)engine);
    }

    return 0;
}

