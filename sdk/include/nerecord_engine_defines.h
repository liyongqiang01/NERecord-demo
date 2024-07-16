/** @file nerecord_engine_defines.h
  * @brief NERecord SDK的一些定义
  * @copyright (c) 2015-2019, NetEase Inc. All rights reserved
  * @author Harrison
  * @date 2018/11/23
  */

#ifndef NERECORD_ENGINE_DEFINES_H
#define NERECORD_ENGINE_DEFINES_H

#include "nerecord_base_types.h"
#include <string>
#include <cstring>

#define kNERtcMaxPlaneCount         4       /**< Video max plane count。*/
#define kNERtcMaxDeviceIDLength     256     /**< 设备ID最大长度。*/
#define kNERtcMaxDeviceNameLength   256     /**< 设备名最大长度。*/
#define kNERtcMaxURILength          256     /**< URI最大长度。*/
#define kNERtcMaxTaskIDLength       64      /**< 任务ID最大长度。*/
#define kNERtcMaxBuffLength         1024    /**< 字符串缓存区最大长度。*/

#ifndef NULL
#define NULL nullptr
#endif

 /**
 * @namespace nerecord
 * @brief namespace nerecord
 */
namespace nerecord
{

typedef uint64_t uid_t;	/**< 64位无符号整数。建议设置范围：1到 2<sup>63</sup>-1，并保证唯一性。*/
typedef uint64_t channel_id_t;	/**< 64位无符号整数。建议设置范围：1到 2<sup>63</sup>-1，并保证唯一性。*/

/** 用户离开原因。*/
typedef enum 
{
    kNERtcSessionLeaveNormal = 0,       /**< 正常离开。*/
    kNERtcSessionLeaveForFailOver = 1,  /**< 用户断线导致离开。*/
    kNERtcSessionLeaveUpdate = 2,       /**< 用户 Failover 过程中产生的 leave。*/
    kNERtcSessionLeaveForKick = 3,      /**< 用户被踢导致离开。*/
    kNERtcSessionLeaveTimeOut = 4,      /**< 用户超时导致离开。*/
} NERtcSessionLeaveReason;

/** 连接状态 */
typedef enum
{
    kNERtcConnectionStateDisconnected = 1,      /**< 没加入频道。*/
    kNERtcConnectionStateConnecting = 2,        /**< 正在加入频道。*/
    kNERtcConnectionStateConnected = 3,         /**< 加入频道成功。*/
    kNERtcConnectionStateReconnecting = 4,      /**< 正在尝试重新加入频道。*/
    kNERtcConnectionStateFailed = 5,            /**< 加入频道失败。*/
} NERtcConnectionStateType;

/** 声音音量信息。一个数组，包含每个说话者的用户 ID 和音量信息。*/
struct NERtcAudioVolumeInfo
{
    uid_t uid;				/**< 说话者的用户 ID。如果返回的 uid 为 0，则默认为本地用户。*/
    unsigned int volume;	/**< 说话者的音量，范围为 0（最低）- 100（最高）。*/
};

/** 视频编码配置。用于衡量编码质量。*/
typedef enum
{
    kNERtcVideoProfileLowest = 0,       /**< 160x90/120, 15fps */
    kNERtcVideoProfileLow = 1,          /**< 320x180/240, 15fps */
    kNERtcVideoProfileStandard = 2,     /**< 640x360/480, 30fps */
    kNERtcVideoProfileHD720P = 3,       /**< 1280x720, 30fps */
    kNERtcVideoProfileHD1080P = 4,      /**< 1920x1080, 30fps */
    kNERtcVideoProfileNone = 5,      /**< 非法视频编码配置 */
    kNERtcVideoProfileMAX = kNERtcVideoProfileHD1080P,      /**< 最高视频编码配置,等同kNERtcVideoProfileHD1080P */
} NERtcVideoProfileType;


/** 视频流类型。*/
typedef enum {
    kNERtcRemoteVideoStreamTypeHigh     = 0, /**< 默认大流 */
    kNERtcRemoteVideoStreamTypeLow      = 1, /**< 小流 */
    kNERtcRemoteVideoStreamTypeNone     = 2, /**< 不订阅 */
} NERtcRemoteVideoStreamType;

typedef enum {
    /**
     * @if English
     * mainstream.
     * @endif
     * @if Chinese
     * 主流
     * @endif
     */
    kNERTCVideoStreamMain = 0,
    /**
     * @if English
     * Substream.
     * @endif
     * @if Chinese
     * 辅流
     * @endif
     */
    kNERTCVideoStreamSub = 1,
    
} NERtcVideoStreamType;

/** 音频类型。*/
typedef enum
{
    kNERtcAudioTypePCM16 = 0,    /**< PCM 音频格式。*/
} NERtcAudioType;

/** 录制音频格式 */
typedef enum
{
    kNERtcAudioFormatDefault = 0,                           /**<默认AAC格式, 扩展名*.m4a */
    kNERtcAudioFormatAAC = kNERtcAudioFormatDefault, /**<AAC格式,  扩展名*.m4a*/
    kNERtcAudioFormatPCM /**< PCM格式,  扩展名*.wav */
} NERtcAudioFormatType;

/** 录制视频格式 */
typedef enum
{
    kNERtcVideoFormatDefault = 0, /**< 默认H264格式, 扩展名*.mp4*/
    kNERtcVideoFormatH264 = kNERtcVideoFormatDefault, /**< H264格式, 扩展名*.mp4*/
    kNERtcVideoFormatYUV /**< YUV420格式, 扩展名*.yuv*/
} NERtcVideoFormatType;

/** 音频格式。*/
struct NERtcAudioFormat
{
    NERtcAudioType type;        /**< 音频类型。*/
    uint32_t     channels;      /**< 音频频道数量(如果是立体声，数据是交叉的)。单声道: 1；双声道 : 2。*/
    uint32_t     sample_rate;    /**< 采样率。*/
    uint32_t     bytes_per_sample;    /**< 每个采样点的字节数 : 对于 PCM 来说，一般使用 16 bit，即两个字节。*/
    uint32_t     samples_per_channel;    /**< 每个频道的样本数量。*/
    uint64_t     timestamp;             /**< 时间戳 */
};

/** 音频帧。*/
struct NERtcAudioFrame
{
    NERtcAudioFormat format;    /**< 音频格式。*/
    void *data;     /**< 数据缓冲区。有效数据长度为：samples_per_channel × channels × bytes_per_sample。*/
};


/** 视频类型。*/
typedef enum
{
    kNERtcVideoTypeI420 = 0,    /**< I420 视频格式。*/
    kNERtcVideoTypeNV12 = 1,    /**< NV12 视频格式。*/
    kNERtcVideoTypeNV21 = 2,    /**< NV21 视频格式。*/
    kNERtcVideoTypeBGRA = 3,    /**< BGRA 视频格式。*/
    kNERtcVideoTypeCVPixelBuffer = 4,    /**< oc capture native视频格式。不支持外部视频输入*/
} NERtcVideoType;

/** 视频旋转角度。*/
typedef enum
{
    kNERtcVideoRotation_0 = 0,      /**< 0 度。*/
    kNERtcVideoRotation_90 = 90,    /**< 90 度。*/
    kNERtcVideoRotation_180 = 180,  /**< 180 度。*/
    kNERtcVideoRotation_270 = 270,  /**< 270 度。*/
} NERtcVideoRotation;


/** 视频帧。*/
struct NERtcVideoFrame
{
    NERtcVideoType format;                       /**< 视频类型  详见: #NERtcVideoType*/
    uint32_t width;                              /**< 视频桢宽度 */
    uint32_t height;                             /**< 视频桢宽高 */
    uint32_t count;                              /**< 通道数，即offset及stride的数目 */
    uint32_t *offset;                            /**< 每通道偏移 */
    uint32_t *stride;                            /**< 每通道数据步进 */
    NERtcVideoRotation rotation;                 /**<  视频旋转角度 详见: #NERtcVideoRotation */
    uint64_t timestamp;                 /**< 时间戳 */
    void* buffer;                                /**<  视频桢数据 */
};


/** 音频profile类型。*/
enum NERtcAudioProfileType
{
    kNERtcAudioProfileDefault,      /**< 48kHz, NANO, 48kbps */
    kNERtcAudioProfileHight,     /**< 48kHz, NANO, 128kbps */
    kNERtcAudioProfileStero,     /**< 48kHz, STERO, 128kbps */
};

/** 视频窗口展示类型。*/
enum NERtcRenderMode
{
    kNERtcRenderModeHidden, /**< 优先保证视窗被填满 */
    kNERtcRenderModeFit /**< 优先保证视频内容全部显示 */
};


/** 背景数据结构。*/
struct NERtcBackground
{
    int border; /**<背景边界宽度 */
    char color[32];   /**<背景色, 如red, #ffffff */
    double alpha;   /**< 背景透明度, 范围[0, 1], 0 表示完全透明, 1表示完全不透明 */
};

/** 布局子窗口数据结构。*/
struct NERtcRegion
{
    uid_t uid;              /**<子窗口对应的用户ID */
    bool substream;              /**<子窗口画面是否为辅流, 默认主流 */
    double x;               /**<子窗口的相对横坐标, 取值范围[0,1] */
    double y;               /**<子窗口的相对纵坐标, 取值范围[0,1] */
    double width;      /**<子窗口相对宽度, 取值范围[0,1] */
    double height;     /**<子窗口相对高度, 取值范围[0,1] */
    double alpha;       /**<子窗口透明度, 取值范围[0,1], 0表示图像透明, 1表示图像完全不透明 */
    NERtcRenderMode renderMode;    /**<画面显示模式*/
    NERtcRegion()
    {
        substream = false;
    }
};


/** 水印类型。*/
enum NERtcWatermarkType
{
    kNERtcWatermarkTypeLitera,      /**< 文字水印 */
    kNERtcWatermarkTypeTimestamp, /**< 时间戳水印 */
    kNERtcWatermarkTypeImage                /**< 图片水印 */
};


/** 文字水印类型配置。*/
struct NERtcLiteraWatermarkConfig
{
    const char *wm_litera;      /**<文字水印内容*/
    const char  *font_file_path;       /**<字体格式文件路径 */
    uint32_t    font_size;       /**<字体大小 */
    uint32_t    offset_x;      /**<水印左上角x坐标 */
    uint32_t    offset_y;      /**<水印左上角y坐标 */
    NERtcBackground background;       /**<水印背景 */
};

/** 时间戳水印类型配置。*/
struct NERtcTimestampWatermarkConfig
{
    const char *font_file_path;       /**<字体格式文件路径 */
    uint32_t    font_size;      /**<字体大小 */
    uint32_t    offset_x;      /**<水印左上角x坐标 */
    uint32_t    offset_y;     /**<水印左上角y坐标 */
    NERtcBackground background;       /**<水印背景 */
};

/** 图片水印类型配置。*/
struct NERtcImageWatermarkConfig
{
    const char *image_path;      /**< png文件路径 */
    uint32_t    offset_x;      /**<水印左上角x坐标 */
    uint32_t    offset_y;     /**<水印左上角y坐标 */
    uint32_t    wm_width;       /**<水印宽度 */
    uint32_t    wm_height;      /**<水印高度 */
};


/** 水印配置联合体。*/
union NERtcWatermarkConfigUnion
{
    NERtcLiteraWatermarkConfig litera;       /**<文字水印 */
    NERtcTimestampWatermarkConfig timestamp;       /**<时间戳水印 */
    NERtcImageWatermarkConfig image;       /**<图片水印 */
};


/** 水印配置。*/
struct WatermarkConfig
{
    NERtcWatermarkType   type;       /**<水印类型 */
    NERtcWatermarkConfigUnion config;       /**<具体类型的配置 */
};


/** 布局类型枚举。*/
enum NERtcLayoutType
{
    kNERtcLayoutGrid,                /**< 自适应布局 */
    kNERtcLayoutCustom,      /**<用户自定义合流布局 */
};

/** 视频录制参数。*/
struct NERtcVideoRecordResolution
{
    int vanvasWidth;        /**<画布宽度 */
    int vanvasHeight;       /**< 画布高度 */
    int fps;       /**< 视频帧率 */
    int bps;       /**< 视频比特率 */

    NERtcVideoRecordResolution()
    : vanvasWidth(360)
    , vanvasHeight(640)
    , fps(15)
    , bps(400000)
    { }
};


/** 合流录制布局。*/
struct VideoLayout
{
    NERtcLayoutType layout; /**<布局模式, \ref LayoutType */
    const char *backgroundColor;    /**<画布背景色,格式为6位RGB值, 如"#000000" */
    uint32_t regionCount;   /**<画布里子窗口数量 */
    NERtcRegion *regions;        /**<每个子窗口的具体参数, 详见 \ref Region */
    const char *appData;    /**<应用程序自定义数据 */
    int appDataLength;      /**<应用程序自定义数据长度 */
    uint32_t wm_num;        /**<水印个数 */
    WatermarkConfig *wm_configs; /**<水印的详细设置, \ref WaterMarkConfig */

    VideoLayout()
    : layout(kNERtcLayoutGrid)
    ,  backgroundColor(NULL)
    , regionCount(0)
    , regions(NULL)
    , appData(NULL)
    , appDataLength(0)
    , wm_num(0)
    , wm_configs(NULL)
    {}
};

typedef VideoLayout VideoMixingLayout;


/** 日志级别。 */
typedef enum {
    kNERtcLogLevelFatal    = 0,        /**< Fatal 级别日志信息。 */
    kNERtcLogLevelError    = 1,        /**< Error 级别日志信息。 */
    kNERtcLogLevelWarning  = 2,        /**< Warning 级别日志信息。 */
    kNERtcLogLevelInfo     = 3,        /**< Info 级别日志信息。默认级别 */
    kNERtcLogLevelDetailInfo   = 4,    /**< Detail Info 级别日志信息。 */
    kNERtcLogLevelVerbos   = 5,        /**< Verbos 级别日志信息。 */
    kNERtcLogLevelDebug    = 6,        /**< Debug 级别日志信息。如果你想获取最完整的日志，可以将日志级别设为该等级。*/
    kNERtcLogLevelOff      = 7,        /**< 不输出日志信息。*/
} NERtcLogLevel;

/** 通话相关的统计信息。*/
struct NERtcStats
{
    uint32_t cpu_app_usage;     /**< 当前 App 的 CPU 使用率 (%)。*/
    uint32_t cpu_idle_usage;    /**< 当前系统的 CPU 空闲率 (%)。*/
    uint32_t cpu_total_usage;	/**< 当前系统的 CPU 使用率 (%)。*/
    uint32_t memory_app_usage;	/**< 当前App的内存使用率 (%)。*/
    uint32_t memory_total_usage;/**< 当前系统的内存使用率 (%)。*/
    uint32_t memory_app_kbytes;	/**< 当前App的内存使用量 (KB)。*/
    int total_duration;         /**< 通话时长（秒）。*/
    uint64_t tx_bytes;	        /**< 发送字节数，累计值。(bytes)*/
    uint64_t rx_bytes;	        /**< 接收字节数，累计值。(bytes)*/
    uint64_t tx_audio_bytes;	/**< 音频发送字节数，累计值。(bytes)*/
    uint64_t tx_video_bytes;	/**< 视频发送字节数，累计值。(bytes)*/
    uint64_t rx_audio_bytes;	/**< 音频接收字节数，累计值。(bytes)*/
    uint64_t rx_video_bytes;	/**< 视频接收字节数，累计值。(bytes)*/
    int tx_audio_kbitrate;	    /**< 音频发送码率。(kbps)*/
    int rx_audio_kbitrate;	    /**< 音频接收码率。(kbps)*/
    int tx_video_kbitrate;	    /**< 视频发送码率。(kbps)*/
    int rx_video_kbitrate;	    /**< 视频接收码率。(kbps)*/
    int up_rtt;	                /**< 上行平均往返时延rtt(ms) */
    int down_rtt;               /**< 下行平均往返时延rtt(ms)*/
    int tx_audio_packet_loss_rate;  /**< 本地上行音频实际丢包率。(%) */
    int tx_video_packet_loss_rate;  /**< 本地上行视频实际丢包率。(%) */
    int tx_audio_packet_loss_sum;   /**< 本地上行音频实际丢包数。 */
    int tx_video_packet_loss_sum;   /**< 本地上行视频实际丢包数。 */
    int tx_audio_jitter;            /**< 本地上行音频抖动计算。(ms) */
    int tx_video_jitter;            /**< 本地上行视频抖动计算。(ms) */
    int rx_audio_packet_loss_rate;  /**< 本地下行音频实际丢包率。(%) */
    int rx_video_packet_loss_rate;  /**< 本地下行视频实际丢包率。(%) */
    int rx_audio_packet_loss_sum;   /**< 本地下行音频实际丢包数。 */
    int rx_video_packet_loss_sum;   /**< 本地下行视频实际丢包数。 */
    int rx_audio_jitter;            /**< 本地下行音频抖动计算。(ms) */
    int rx_video_jitter;            /**< 本地下行视频抖动计算。(ms) */
};

/**
本地视频单条流上传统计信息。
 */
struct NERtcVideoLayerSendStats {
    int layer_type; /**< 流类型： 1、主流，2、辅流。*/
    int width; /**< 视频流宽（像素）。*/
    int height; /**< 视频流高（像素）。*/
    int capture_width; /**< 视频采集宽（像素）。*/
    int capture_height; /**< 视频采集高（像素）。*/
    int capture_frame_rate; /**< 视频采集帧率。*/
    int render_frame_rate; /**< 视频渲染帧率。*/
    int encoder_frame_rate; /**< 编码帧率。*/
    int sent_frame_rate; /**< 发送帧率。*/
    int sent_bitrate; /**< 发送码率(Kbps)。*/
    int target_bitrate; /**< 编码器目标码率(Kbps)。*/
    int encoder_bitrate; /**< 编码器实际编码码率(Kbps)。*/
    char codec_name[kNERtcMaxDeviceNameLength]; /**< 视频编码器名字。*/
    bool drop_bandwidth_strategy_enabled; /**< 超分降带宽策略是否开启*/
};
/** 本地视频流上传统计信息。*/
struct NERtcVideoSendStats
{
    NERtcVideoLayerSendStats* video_layers_list; /**< 视频流信息数组。*/
    int video_layers_count; /**< 视频流条数。*/
};

/** 远端视频单条流的统计信息。*/
struct NERtcVideoLayerRecvStats
{
    int layer_type; /**< 流类型： 1、主流，2、辅流。*/
    int width; /**< 视频流宽（像素）。*/
    int height; /**< 视频流高（像素）。*/
    int received_bitrate; /**< 接收到的码率(Kbps)。*/
    int received_frame_rate; /**< 接收到的帧率 (fps)。*/
    int decoder_frame_rate; /**< 解码帧率 (fps)。*/
    int render_frame_rate; /**< 渲染帧率 (fps)。*/
    int packet_loss_rate; /**< 下行丢包率(%)。*/
    int total_frozen_time; /**< 用户的下行视频卡顿累计时长(ms)。*/
    int frozen_rate; /**< 用户的下行视频平均卡顿率(%)。*/
    char codec_name[kNERtcMaxDeviceNameLength]; /**< 视频解码器名字。*/
    int peer_to_peer_delay; /**< 远端用户的视频流从采集到本地播放的延迟。(ms)*/
};

/** 远端视频流的统计信息。*/
struct NERtcVideoRecvStats
{
    uid_t uid;      /**< 用户 ID，指定是哪个用户的视频流。*/
    NERtcVideoLayerRecvStats* video_layers_list;	/**< 视频流信息数组。*/
    int video_layers_count;	                        /**< 视频流条数。*/
};

/** 音频流类型。*/
typedef enum {
    kNERtcAudioStreamTypeMain = 0, /**< 主流。*/
    kNERtcAudioStreamTypeSub = 1, /**< 辅流。*/
} NERtcAudioStreamType;

/** 本地单条音频流上传统计信息。*/
struct NERtcAudioLayerSendStats {
    NERtcAudioStreamType stream_type; /**< 音频流通道类型。*/
    int num_channels; /**< 当前采集声道数。*/
    int sent_sample_rate; /**< 地上行音频采样率。*/
    int sent_bitrate; /**< （上次统计后）发送码率(Kbps)。*/
    int audio_loss_rate; /**< 特定时间内的音频丢包率 (%)。*/
    int64_t rtt; /**< RTT。*/
    unsigned int volume; /**< 音量，范围为 0（最低）- 100（最高）。*/
    unsigned int cap_volume; /**< 采集音量，范围为 0（最低）- 100（最高）。*/
};

/** 本地音频流上传统计信息。*/
struct NERtcAudioSendStats
{
    NERtcAudioLayerSendStats* audio_layers_list; /**< 音频流信息数组。*/
    int audio_layers_count; /**< 音频流条数。*/
};

/**
远端用户单条音频流统计。
 */
struct NERtcAudioLayerRecvStats {
    NERtcAudioStreamType stream_type; /**< 音频频流通道类型。*/
    int received_bitrate; /**< （上次统计后）接收到的码率(Kbps)。*/
    int total_frozen_time; /**< 用户的下行音频卡顿累计时长(ms)。*/
    int frozen_rate; /**< 用户的下行音频平均卡顿率(%)。*/
    int audio_loss_rate; /**< 特定时间内的音频丢包率 (%)。*/
    unsigned int volume; /**< 音量，范围为 0（最低）- 100（最高）。*/
    int av_timestamp_diff; /**< 废弃字段。*/
    int peer_to_peer_delay; /**< 废弃字段*/
};
/** 远端用户的音频统计。*/
struct NERtcAudioRecvStats
{
    uid_t uid; /**< 用户 ID，指定是哪个用户的音频流。*/
    NERtcAudioLayerRecvStats* audio_layers_list; /**< 音频流信息数组。*/
    int audio_layers_count; /**< 音频流条数。*/
};


/** 录制属性 */
struct NERtcRecordingEngineProperties
{
    const char *storage_dir; /**<录制文件存储目录 */
};

/** 录制文件存储目录结构 */
enum NERtcRecordFileDirMode
{
    kNERtcRecordFileDirModeDefault,  /**<默认存储模式: 用户指定的目录/yymmdd/ChannelName_HHMMSS_MSUSNS/<ChannelName>-<uid>-<video/audio>-<timestamp>.<ext> */
    kNERtcRecordFileDirModeCustom  /**<自定义存储模式: 用户指定的目录<ChannelName>-uid-<video/audio>-<timestamp>.<ext> */
};

/** 私有化服务器配置项 */
struct NERtcServerAddresses {
    char channel_server[kNERtcMaxURILength]; /**< 获取通道信息服务器 */
    char record_channel_server[kNERtcMaxURILength]; /**< 获取录制sdk通道信息服务器 */
    char statistics_server[kNERtcMaxURILength]; /**< 统计上报服务器 */
    char statistics_dispatch_server[kNERtcMaxURILength]; /**< 统计调度服务器 */
    char statistics_backup_server[kNERtcMaxURILength]; /**< 统计备份服务器 */
    char room_server[kNERtcMaxURILength]; /**< roomServer服务器 */
    char compat_server[kNERtcMaxURILength]; /**< 兼容性配置服务器 */
    char nos_lbs_server[kNERtcMaxURILength]; /**< nos 域名解析服务器 */
    char nos_upload_sever[kNERtcMaxURILength]; /**< 默认nos 上传服务器 */
    char nos_token_server[kNERtcMaxURILength]; /**< 获取NOS token 服务器 */
    char cloud_proxy_server[kNERtcMaxURILength]; /**< 云代理服务器 */
    char websocket_proxy_server[kNERtcMaxURILength]; /**< websocket代理服务器*/
    char quic_proxy_server[kNERtcMaxURILength]; /**< quick代理服务器*/
    char media_proxy_server[kNERtcMaxURILength]; /**< 媒体代理服务器*/
    bool use_ipv6;
    
    NERtcServerAddresses() : use_ipv6(false) {
        memset(channel_server, 0, sizeof(channel_server));
        memset(record_channel_server, 0, sizeof(record_channel_server));
        memset(statistics_server, 0, sizeof(statistics_server));
        memset(statistics_dispatch_server, 0, sizeof(statistics_dispatch_server));
        memset(statistics_backup_server, 0, sizeof(statistics_backup_server));
        memset(room_server, 0, sizeof(room_server));
        memset(compat_server, 0, sizeof(compat_server));
        memset(nos_lbs_server, 0, sizeof(nos_lbs_server));
        memset(nos_upload_sever, 0, sizeof(nos_upload_sever));
        memset(nos_token_server, 0, sizeof(nos_token_server));
        memset(cloud_proxy_server, 0, sizeof(cloud_proxy_server));
        memset(websocket_proxy_server, 0, sizeof(websocket_proxy_server));
        memset(quic_proxy_server, 0, sizeof(quic_proxy_server));
        memset(media_proxy_server, 0, sizeof(media_proxy_server));
    }
};

/** 代理服务器配置项 */
struct NERtcAgentAddresses {
    char signal_server[kNERtcMaxURILength];    /**< 信令代理服务器 */
    char relay_server[kNERtcMaxURILength]; /**< relay服务器 */
};

/**
 * @if English
 * The length of the encryption key.
 * @endif
 * @if Chinese
 * 加密key的长度
 * @endif
 */
#define kNERecordEncryptByteLength 128
/**
 * @if Chinese
 * 自定义信息的长度
 * @endif
 */
#define kNERecordCustomInfoLength 128

/**
 * @if English
 * Media stream encryption mode.
 * @endif
 * @if Chinese
 * 媒体流加密模式。
 * @endif
 * */
typedef enum {
    /**
     * @if English
     * 128-bit SM4 encryption, ECB mode.
     * @endif
     * @if Chinese
     * 128 位 SM4 加密，ECB 模式。
     * @endif
     */
    kNERecordGMCryptoSM4ECB = 0,
   /**
   * @if English
   * Custom encryption mode.
   * @endif
   * @if Chinese
   * 自定义加密模式。
   * @endif
   */
    NERecordEncryptionModeCustom,
} NERecordEncryptionMode;

/**
 * @if Chinese
 * 自定义加密数据包。
 * @endif
 */
struct NERecordMediaPacket {
  // 需要发送或接收的数据的缓存地址
  const unsigned char* buffer;
  // 需要发送或接收的数据的缓存大小
  long size;
};

/**
 * @if Chinese
 * 自定义加密数据回调 observer。
 * @endif
 */
class INERecordPacketObserver {
 public:
  virtual ~INERecordPacketObserver() {}

  virtual bool onSendAudioPacket(NERecordMediaPacket& packet) = 0;
  virtual bool onSendVideoPacket(NERecordMediaPacket& packet) = 0;
  virtual bool onReceiveAudioPacket(NERecordMediaPacket& packet) = 0;
  virtual bool onReceiveVideoPacket(NERecordMediaPacket& packet) = 0;
};

/**
 * @if English
 * Media stream encryption scheme.
 * @endif
 * @if Chinese
 * 媒体流加密方案。
 * @endif
 */
struct NERecordEncryptionConfig {
    /**
     * @if English
     * Media stream encryption mode. For more information, see NERtcEncryptionMode.
     * @endif
     * @if Chinese
     * 媒体流加密模式。详细信息请参考 NERtcEncryptionMode。
     * @endif
     */
    NERecordEncryptionMode mode;
    /**
     * @if English
     * Media stream encryption key. The key is of string type. We recommend that you set the key to a string that
     * contains only letters.
     * @endif
     * @if Chinese
     * 媒体流加密密钥。字符串类型，推荐设置为英文字符串。
     * @endif
     */
    char key[kNERecordEncryptByteLength];
    /**
     * 自定义加密回调 observer, mode 为自定义加密时需要设置
     */
    INERecordPacketObserver* observer = nullptr;

    void reInit(){
        mode = kNERecordGMCryptoSM4ECB;
        memset(key, 0, sizeof(key)); 
    }

    NERecordEncryptionConfig() : mode(kNERecordGMCryptoSM4ECB) { memset(key, 0, sizeof(key)); }
};

/** 通过 JSON 配置 SDK 提供技术预览或特别定制功能。以标准化方式公开 JSON 选项。详见API setParameters*/
#define kNERtcKeyAutoSubscribeAudio         "auto_subscribe_audio"         /**< bool value，其他用户打开音频时，自动订阅。 默认值 true */
#define kNERtcKeyVideoSubscribeAudio        "auto_subscribe_vedio"         /**< bool value，其他用户打开视频时，自动订阅。 默认值 true */

} // namespace nerecord

#endif
