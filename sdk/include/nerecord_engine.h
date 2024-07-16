/** @file nerecord_engine.h
* @brief NERECORD SDK接口头文件。
* NERECORD SDK所有接口参数说明: 所有与字符串相关的参数(char *)全部为UTF-8编码。
* @copyright (c) 2015-2019, NetEase Inc. All rights reserved
* @author Harrison
* @date 2018/11/20
*/

/**
 @defgroup createNERecordEngine Create an NERECORD Engine
 */

#ifndef NERECORD_ENGINE_H
#define NERECORD_ENGINE_H

#include "nerecord_base.h"
#include "nerecord_base_types.h"
#include "nerecord_engine_defines.h"
#include "nerecord_engine_event_handler.h"

 /**
 * @namespace nerecord
 * @brief namespace nerecord
 */
namespace nerecord
{

    /** SDK模式 */
    enum SubscribeMode
    {
        ANTI_SPAM_MODE,     /**<反垃圾模式(默认) */
        RECORDER_MODE       /**<文件录制模式 */
    };

/** \ref IRecordEngine::initialize "IRecordEngine::initialize()"参数类型*/
struct NERecordEngineContext
{
    const char *app_key;		/**< 用户注册云信的APP Key。如果你的开发包里面缺少 APP Key，请申请注册一个新的 APP Key。*/
    IRecordEngineEventHandler *event_handler;	/**< 用于 SDK 向 App 发送回调事件通知。*/
    const char *log_dir_path;	/**< 日志目录的完整路径，采用UTF-8 编码。*/
    uint32_t log_file_max_size_KBytes; /**< 日志大小,单位KB*/
    const char *record_file_root_dir; /**< 录制文件存储根目录 */
    NERtcRecordFileDirMode record_file_dir_mode; /**<录制文件路径模式 */
    NERtcLogLevel log_level;	/**< 日志级别，默认级别为 kNERtcLogLevelInfo。*/
    const char *nerecord_absolute_dir; /**< libnertc.so绝对路径。*/
    
    const char *parameters; /**< parameters 通过 JSON 配置 SDK 提供技术预览或特别定制功能。以标准化方式公开 JSON 选项。 */

    SubscribeMode submode;  /**<订阅模式 */
    bool audio_enable;          /**< 使能音频录制 */
    bool video_enable;          /**< 使能视频录制 */
    bool video_substream_split;          /**< 视频辅流独立录制 */
    bool mixing_enable;         /**< 使能合流录制 */
    bool mixed_audio_video; /**< 合并音视频文件 */
    bool mixed_audio_substream; /**< 合并音频和辅流视频文件 */
    bool snapshot_enable; /**< 截屏快照开关 */
    unsigned snapshot_interval; /**< 截屏快照时间间隔, 单位秒, 默认为5, 不可为0 */
    NERtcAudioFormatType decodeAudio; /**<录制音频格式*/
    NERtcVideoFormatType decodeVideo; /**<录制视频格式*/

    /**是否自动录制音频 or 是否自动订阅音频
     * - true: 自动录制音频 
     * - false:手动模式需要通过\ref IRecordEngine::updateSubscribeAudioUids "updateSubscribeAudioUids"开始录制音频*/
    bool auto_subscribe_audio; 
     /**是否自动录制视频 or 是否自定订阅视频
      * - true: 自动录制视频 
      * - false:手动模式需要通过\ref IRecordEngine::updateSubscribeVideoUids "updateSubscribeVideoUids"开始录制视频*/
    bool auto_subscribe_video;
    bool keep_last_frame;     /**<用户关闭视频后, 是否显示其视频最后一帧,  - true: 显示最后一帧, - false: (默认)显示占位图或黑色画面*/

    NERtcRemoteVideoStreamType default_stream_type; /**< 自动订阅大流/小流 */
    NERtcAudioProfileType audioProfile; /**< 音频录制参数*/
    NERtcVideoRecordResolution video_record_resolution; /**< 视频录制参数*/
    VideoLayout *video_with_substream_layout;  /**< 单录有辅流时的视频布局 */

    bool mixing_param_split; /**< 合流录制使用独立的录制参数, 默认false*/
    NERtcAudioProfileType mixing_audioProfile; /**< 合流录制时音频录制参数, 仅在mixing_param_split为true时生效*/
    NERtcVideoRecordResolution mixing_video_record_resolution; /**< 合流录制时视频录制参数, 仅在mixing_param_split为true时生效*/

    NERtcServerAddresses *server_config;   /**< 私有化服务器地址。如果使用默认服务器则将该字段设置为NULL */
    NERtcAgentAddresses *agent_config;    /**< 代理服务器地址。如果不使用代理则将该字段设置为NULL */

    NERecordEngineContext()
    {
        app_key = nullptr;
        event_handler = nullptr;
        log_dir_path = nullptr;
        log_file_max_size_KBytes = 200 * 1024;
        nerecord_absolute_dir = nullptr;
        record_file_root_dir = nullptr;
        record_file_dir_mode = kNERtcRecordFileDirModeDefault;
        log_level = kNERtcLogLevelInfo;
        parameters = nullptr;
        submode = ANTI_SPAM_MODE;
        audio_enable = true;
        video_enable = true;
        video_substream_split = false;
        mixing_enable = false;
        mixed_audio_video = true;
        mixed_audio_substream = true;
        auto_subscribe_audio = true;
        auto_subscribe_video = true;
        decodeAudio = kNERtcAudioFormatDefault;
        decodeVideo = kNERtcVideoFormatDefault;
        snapshot_enable = true;
        snapshot_interval = 5;
        audioProfile = kNERtcAudioProfileDefault;
        default_stream_type = kNERtcRemoteVideoStreamTypeHigh;
        video_with_substream_layout = NULL;
        mixing_param_split = false;
        keep_last_frame = false;
        server_config = NULL;
    }
};

/** RecordEngine 类提供了供 App 调用的主要接口方法。

 IRecordEngine 是 NERECORD SDK 的基础接口类。创建一个 IRecordEngine 对象并调用这个对象的方法可以激活 NERECORD SDK 的通信功能。
 */
class IRecordEngine
{
public:
    virtual ~IRecordEngine() {}

    /** 初始化 NERECORD SDK 服务。

     该方法用来进行初始化 NERECORD 服务。传入云信为开发者签发的厂商秘钥进行初始化。 在调用 createNERecordEngine() 方法创建 IRecordEngine 
	 对象后，必须先调用该方法进行初始化，才能使用其他方法。初始化成功后，默认处于音视频通话模式。

     @warning
     - 必须使用同一个 App Key 才能进行通话。
     - 一个 IRecordEngine 实例对象只能使用一个 App Key。如需更换 App Key，必须先调用 \ref IRecordEngine::release "release" 方法销毁当前实例，再调用本方法重新创建实例。

     @param[in] context 传入的RECORD engine context对象: NERecordEngineContext.
     @return
     - 0: 方法调用成功；
     - 其他: 方法调用失败。
     */
    virtual int initialize(const NERecordEngineContext& context) = 0;

    /** 销毁 IRecordEngine 对象。
     */
    virtual void release() = 0;

    /** 初始化频道，录制SDK需要先预加入频道后再加入频道。

     该方法让用户加入通话频道，在同一个频道内的用户可以互相通话，多个用户加入同一个频道，可以群聊。 使用不同 App Key 的 App 是不能互通的。如果已在通话中，用户必须调用 \ref IRecordEngine::leaveChannel "leaveChannel" 退出当前通话，才能进入下一个频道。

     @note 频道内每个用户的用户 ID 必须是唯一的。
     
     @param[in] channel_name 标识通话的频道名称，长度在 64 字节以内的字符串。以下为支持的字符集范围（共 89 个字符）: a-z, A-Z, 0-9, space, !#$%&()+-:;&le;.,>? @[]^_{|}~”

     @return
     - 0: 方法调用成功；
     - 其他: 方法调用失败。
     */
    // virtual int initChannel(const char* channel_name) = 0;

    /** 加入频道。

     该方法让用户加入通话频道，在同一个频道内的用户可以互相通话，多个用户加入同一个频道，可以群聊。 使用不同 App Key 的 App 是不能互通的。如果已在通话中，用户必须调用 \ref IRecordEngine::leaveChannel "leaveChannel" 退出当前通话，才能进入下一个频道。

     @note 频道内每个用户的用户 ID 必须是唯一的。
     
     @param[in] token 动态秘钥。安全要求不高: 将值设为 空字符串。安全要求高: 将值设置为 Token。如果你已经启用了 App Certificate, 请务必使用 Token。
     @param[in] channel_name 标识通话的频道名称，长度在 64 字节以内的字符串。以下为支持的字符集范围（共 89 个字符）: a-z, A-Z, 0-9, space, !#$%&()+-:;&le;.,>? @[]^_{|}~”
     @param[in] uid 用户ID。

     @return
     - 0: 方法调用成功；
     - 其他: 方法调用失败。
     */
    virtual int joinChannel(const char* token, const char* channel_name, uid_t uid) = 0;
    /** 离开频道。

     离开频道，即挂断或退出通话。

     当调用 \ref IRecordEngine::joinChannel "joinChannel" 方法后，必须调用 \ref IRecordEngine::leaveChannel "leaveChannel" 结束通话，否则无法开始下一次通话。 不管当前是否在通话中，都可以调用 leaveChannel，没有副作用。该方法会把会话相关的所有资源释放掉。

     该方法是异步操作，调用返回时并没有真正退出频道。在真正退出频道后，SDK 会触发 \ref IRecordEngineEventHandler::onLeaveChannel "onLeaveChannel" 回调。

     @note
     - 如果你调用了 \ref IRecordEngine::leaveChannel "leaveChannel" 后立即调用 \ref IRecordEngine::release "release" , SDK 将无法触发 \ref IRecordEngineEventHandler::onLeaveChannel "onLeaveChannel" 回调。

     @return
     - 0: 方法调用成功；
     - 其他: 方法调用失败。
     */
    virtual int leaveChannel() = 0;

     /** 订阅／取消订阅指定音频流。

    @param[in] uid 指定用户的 ID
    @param[in] subscribe
    - true: 订阅指定音频流（默认）；
    - false: 取消订阅指定音频流。

    @return
    - 0: 方法调用成功；
    - 其他: 方法调用失败。

    */
    virtual int subscribeRemoteAudioStream(uid_t uid, bool subscribe) = 0;

    /** 订阅／取消订阅指定视频流。

    @param[in] uid 指定用户的 ID
    @param[in] max_profile 最大分辨率
    @param[in] subscribe
    - true: 订阅指定视频流；
    - false: 取消订阅指定视频流。

    @return
    - 0: 方法调用成功；
    - 其他: 方法调用失败。

    */
    virtual int subscribeRemoteVideoStream(uid_t uid, NERtcRemoteVideoStreamType stream_type, bool subscribe) = 0;

    /** 订阅／取消订阅指定用户视频辅流。

    @param[in] uid 指定用户的 ID
    @param[in] subscribe
    - true: 订阅指定视频辅流；
    - false: 取消订阅指定视频辅流。

    @return
    - 0: 方法调用成功；
    - 其他: 方法调用失败。

    */
    virtual int subscribeRemoteVideoSubStream(uid_t uid, bool subscribe)  = 0;

    /** 设置视频合流布局。

     视频合流模式下, 调用该函数设置布局模式。

     @return
     - 0: 方法调用成功；
     - 其他: 方法调用失败。
     */
    virtual int setVideoMixingLayout(const VideoMixingLayout &layout) = 0;

    /** 开始合流录制。

     合流模式下, 调用该函数开始录制。

     @return
     - 0: 方法调用成功；
     - 其他: 方法调用失败。
     */
    virtual int startMixingRecord() = 0;

    /** 结束合流录制。

     合流模式下, 调用该函数结束录制。
     */
    virtual void stopMixingRecord() = 0;

    /** 手动模式下批量更新订阅的音频流。
     
     不在数组中已定阅的流会被取消订阅

    @param[in] uids 指定用户的 ID数组
    @param[in] num uids数组大小

    @return
    - 0: 方法调用成功；
    - 其他: 方法调用失败。

    */
    virtual int updateSubscribeAudioUids(uid_t *uids, uint32_t num) = 0;

    /** 手动模式下批量更新订阅的视频流。
     
     不在数组中已定阅的流会被取消订阅

    @param[in] uids 指定用户的 ID数组
    @param[in] num uids数组大小

    @return
    - 0: 方法调用成功；
    - 其他: 方法调用失败。

    */
    virtual int updateSubscribeVideoUids(uid_t *uids, uint32_t num) = 0;

    /** 手动模式下开始录制。*/
    virtual void startService() = 0;

    /** 手动模式下停止录制。*/
    virtual void stopService() = 0;

    /** 更新合流录制视频水印。
     
     录制重启后水印会被取消

    @param[in] wm_num 水印个数
    @param[in] configs 水印数组 \ref  WatermarkConfig

    */
    virtual void updateWatermarkConfigs(uint32_t wm_num, const WatermarkConfig *configs) = 0;

    /** 更新单流录制视频水印。
     
     录制重启后水印会被取消

    @param[in] uid 用户uid
    @param[in] substream false - 更新主流水印, true - 更新辅流水印
    @param[in] wm_num 水印个数
    @param[in] configs 水印数组 \ref  WatermarkConfig

    */
    virtual void updateWatermarkConfigs(uid_t uid, bool substream, uint32_t wm_num, const WatermarkConfig *configs) = 0;

    /** 启用说话者音量提示。该方法允许 SDK 定期向 App 反馈当前谁在说话以及说话者的音量。
    
     启用该方法后，无论频道内是否有人说话，都会在"onRemoteAudioVolumeIndication" 回调中按设置的间隔时间返回音量提示。

     @param enable 是否启用说话者音量提示。
     @param interval 指定音量提示的时间间隔，单位为毫秒。必须设置为 100 毫秒的整数倍值。

     @return
     - 0: 方法调用成功；
     - 其他: 方法调用失败。
     */
    virtual int enableAudioVolumeIndication(bool enable, uint64_t interval) = 0;

    /** 设置指定用户的背景。当指定 uid 没有发送视频流时，会显示该背景图片。

     @param uid 指定用户uid。
     @param image 背景图片的路径。

     @return
     - 0: 方法调用成功；
     - 其他: 方法调用失败。
     */
    virtual int setUserBackground(uid_t uid, const char *image) = 0;


    /** 开启或关闭媒体流加密。
     * 在金融行业等安全性要求较高的场景下，您可以在加入房间前通过此方法设置媒体流加密模式。
     * @note 
     * - 请在加入房间前调用该方法，加入房间后无法修改加密模式与密钥。用户离开房间后，SDK 会自动关闭加密。如需重新开启加密，需要在用户再次加入房间前调用此方法。
     * - 同一房间内，所有开启媒体流加密的用户必须使用相同的加密模式和密钥，否则使用不同密钥的成员加入房间时会报错 kNERtcErrEncryptNotSuitable（30113）。 
     * - 安全起见，建议每次启用媒体流加密时都更换新的密钥。     
     * @param enable    是否开启媒体流加密。
     *                  - true: 开启
     *                  - false:（默认）关闭
     * @param config    媒体流加密方案。详细信息请参考 nerecord::NERtcEncryptionConfig 。
     * @return
     * - 0: 方法调用成功
     * - 其他: 调用失败
     * @endif
     */
    virtual int enableEncryption(bool enable, const NERecordEncryptionConfig &config) = 0;
    
    /** 获取录制属性。*/
    virtual const NERtcRecordingEngineProperties *getProperties() = 0;
};

} //namespace nerecord


////////////////////////////////////////////////////////
/** \addtogroup createNERecordEngine
 @{
 */
////////////////////////////////////////////////////////

/** 创建 RECORD 引擎对象并返回指针。

 @return RECORD 引擎对象的指针。
 */
NERECORD_API nerecord::IRecordEngine* NERECORD_CALL createNERecordEngine();

/** 销毁 RECORD 引擎对象。
 */
NERECORD_API void NERECORD_CALL destroyNERecordEngine(void *& nerecord_engine_inst);

////////////////////////////////////////////////////////
/** @} */
////////////////////////////////////////////////////////

#endif
