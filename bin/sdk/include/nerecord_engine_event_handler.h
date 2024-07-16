/** @file nerecord_engine_event_handler.h
* @brief NERECORD SDK回调接口头文件。
* NERECORD SDK所有接口参数说明: 所有与字符串相关的参数(char *)全部为UTF-8编码。
* @copyright (c) 2015-2019, NetEase Inc. All rights reserved
* @author Harrison
* @date 2018/11/20
*/

#ifndef NERECORD_ENGINE_EVENT_HANDLER_H
#define NERECORD_ENGINE_EVENT_HANDLER_H

#include "nerecord_base_types.h"
#include "nerecord_error_code.h"
#include "nerecord_warn_code.h"
#include "nerecord_engine_defines.h"

 /**
 * @namespace nerecord
 * @brief namespace nerecord
 */
namespace nerecord
{
/** IRecordEngineEventHandler 回调接口类用于 SDK 向 App 发送回调事件通知，App 通过继承该接口类的方法获取 SDK 的事件通知。

 接口类的所有方法都有缺省（空）实现，App 可以根据需要只继承关心的事件。在回调方法中，App 不应该做耗时或者调用可能会引起阻塞的 API（如开启音频或视频等），否则可能影响 SDK 的运行。
 */
class IRecordEngineEventHandler
{
public:
    virtual ~IRecordEngineEventHandler() {}

    /** 发生错误回调。

     该回调方法表示 SDK 运行时出现了（网络或媒体相关的）错误。通常情况下，SDK上报的错误意味着SDK无法自动恢复，需要 App 干预或提示用户。

     @param error_code 错误代码: #NERtcDMErrorCode.
     @param msg 错误描述。
     */
    virtual void onError(int error_code, const char* msg) {
        (void)error_code;
        (void)msg;
    }

    /** 发生警告回调。

     该回调方法表示 SDK 运行时出现了（网络或媒体相关的）警告。通常情况下，SDK 上报的警告信息 App 可以忽略，SDK 会自动恢复。

     @param warn_code 错误代码: #NERtcWarnCode.
     @param msg 警告描述。
     */
    virtual void onWarning(int warn_code, const char* msg) {
        (void)warn_code;
        (void)msg;
    }

    /** 网络连接中断回调 */
    virtual void onConnectInterrupted(){
    }

    /** 加入频道回调。

     @param cid  频道 ID。
     @param uid  用户 ID。
     @param result  返回结果。
     @param elapsed 从 joinChannel 开始到发生此事件过去的时间（毫秒）。
     */
    virtual void onJoinChannel(channel_id_t cid, uid_t uid, NERtcErrorCode result, uint64_t elapsed) {
        (void)cid;
        (void)uid;
        (void)result;
        (void)elapsed;
    }

    /** 重新加入频道回调。

	  有时候由于网络原因，客户端可能会和服务器失去连接，SDK会进行自动重连，自动重连后触发此回调方法。

     @param cid  频道 ID。
     @param uid  用户 ID。
     @param result  返回结果。
     @param elapsed 从开始重连到发生此事件过去的时间（毫秒）。
     */
    virtual void onRejoinChannel(channel_id_t cid, uid_t uid, NERtcErrorCode result, uint64_t elapsed) {
        (void)cid;
        (void)uid;
        (void)result;
    }

    /** 离开频道回调。

     App 调用 \ref IRtcEngine::leaveChannel "leaveChannel" 方法时，SDK提示 App 离开频道是否成功。

     @param result 返回结果。
     */
    virtual void onLeaveChannel(NERtcErrorCode result) {
        (void)result;
    }

    /** 掉线回调。

	  由于非网络原因，客户端可能会和服务器失去连接，此时SDK无需自动重连，直接触发此回调方法。

     @param reason  返回结果。
     */
    virtual void onDisconnect(NERtcErrorCode reason) {
        (void)reason;
    }

    /** 远端用户加入当前频道回调。

     - 通信模式下，该回调提示有远端用户加入了频道，并返回新加入用户的 ID；如果加入之前，已经有其他用户在频道中了，新加入的用户也会收到这些已有用户加入频道的回调。

     @param uid 新加入频道的远端用户ID。
     @param user_name 新加入频道的远端用户名。
     */
    virtual void onUserJoined(uid_t uid, const char * user_name) {
        (void)uid;
        (void)user_name;
    }

    /** 远端用户离开当前频道回调。

     提示有远端用户离开了频道（或掉线）。

     @param uid 远端用户ID。
     @param reason 远端用户离开原因。
     */
    virtual void onUserLeft(uid_t uid, NERtcSessionLeaveReason reason) {
        (void)uid;
        (void)reason;
    }
    /** 远端用户开启音频回调。
    
     @param uid 远端用户ID。
     */
    virtual void onUserAudioStart(uid_t uid) {
        (void)uid;
    }
    /** 远端用户停用音频回调。

     @param uid 远端用户ID。
     */
    virtual void onUserAudioStop(uid_t uid) {
        (void)uid;
    }

    /** 当收到原始音频数据时，会触发该回调

     @param uid 远端用户ID。
     @param frame 收到的原始音频数据，格式为 PCM。详见 NERtcAudioFrame。
     */
    virtual void audioFrameReceived(uid_t uid, const NERtcAudioFrame *frame) {
        (void)uid;
        (void)frame;
    }

    /** 每当频道内音频混音完成后，会触发该回调

     @param frame 混合后的音频数据，格式为 PCM。详见 NERtcAudioFrame。
     */
    virtual void audioFrameMixed(const NERtcAudioFrame *frame) {
        (void)frame;
    }

    /** 远端用户是否静音回调。

     @param uid 远端用户ID。
     @param mute 是否静音。
     */
    virtual void onUserAudioMute(uid_t uid, bool mute) {
        (void)uid;
        (void)mute;
    }
    /** 远端用户开启视频回调。
    
     @param uid 远端用户ID。
     @param max_profile  远端用户支持的最高视频编码质量。
     */
    virtual void onUserVideoStart(uid_t uid, NERtcVideoProfileType max_profile) {
        (void)uid;
        (void)max_profile;
    }
    /** 远端用户停用视频回调。

     @param uid 远端用户ID。
     */
    virtual void onUserVideoStop(uid_t uid) {
        (void)uid;
    }

    /** 当收到原始视频数据时，会触发该回调

     @param uid 远端用户ID。
     @param frame 收到的原始视频数据。详见 NERtcVideoFrame。
     */
    virtual void videoFrameReceived(uid_t uid, const NERtcVideoFrame *frame) {
        (void)uid;
        (void)frame;
    }

    /** 当收到辅流原始视频数据时，会触发该回调

     @param uid 远端用户ID。
     @param frame 收到的辅流原始视频数据。详见 NERtcVideoFrame。
     */
    virtual void videoSubstreamFrameReceived(uid_t uid, const NERtcVideoFrame *frame) {
        (void)uid;
        (void)frame;
    }

    /** 远端用户是否禁视频流回调。

     @param uid 远端用户ID。
     @param mute 是否禁视频流回调。
     */
    virtual void onUserVideoMute(uid_t uid, bool mute) {
        (void)uid;
        (void)mute;
    }

        /** 远端用户开启辅流视频回调。

     @param uid 远端用户ID。
     @param max_profile 最大分辨率。
     */
    virtual void onUserSubStreamVideoStart(uid_t uid, NERtcVideoProfileType max_profile) {
        (void)uid;
        (void)max_profile;
    }
    /** 远端用户停用辅流视频回调。

     @param uid 远端用户ID。
     */
    virtual void onUserSubStreamVideoStop(uid_t uid) {
        (void)uid;
    }

    /** 提示频道内谁正在说话及说话者瞬时音量的回调。

     该回调默认禁用。可以通过 enableAudioVolumeIndication 方法开启；
     开启后，无论频道内是否有人说话，SDK 都会按 enableAudioVolumeIndication 方法中设置的时间间隔触发该回调。

     在返回的 speakers 数组中:

     - 如果有 uid 出现在上次返回的数组中，但不在本次返回的数组中，则默认该 uid 对应的远端用户没有说话。
     - 如果volume 为 0，表示该用户没有说话。
     - 如果speakers 数组为空，则表示此时远端没有人说话。

     @param speakers 每个说话者的用户 ID 和音量信息的数组: NERtcAudioVolumeInfo
     @param speaker_number speakers 数组的大小，即说话者的人数。
     @param total_volume （混音后的）总音量，取值范围为 [0,100]。
     */
    virtual void onRemoteAudioVolumeIndication(const NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume) {
        (void) speakers;
        (void) speaker_number;
        (void) total_volume;
    }

    /** 检测到活跃用户回调。

     该回调默认禁用。可以通过 enableAudioVolumeIndication 方法开启；
     开启后，如果SDK检测到活跃的用户则触发该回调。


     @param uid 当前时段说话声音最大的用户
     */
    virtual void onActiveSpeaker(uid_t uid) {
        (void) uid;
    }

        /** 已接收到远端音频首帧回调。

     @param uid 发送音频帧的远端用户的用户 ID。
     */
    virtual void onFirstAudioDataReceived(uid_t uid) {
        (void)uid;
    }

    /** 已显示首帧远端视频回调。

    第一帧远端视频显示在视图上时，触发此调用。

     @param uid 用户 ID，指定是哪个用户的视频流。
     */
    virtual void onFirstVideoDataReceived(uid_t uid) {
        (void)uid;
    } 

    /** 已解码远端音频首帧的回调。

     @param uid 远端用户 ID。
     */
    virtual void onFirstAudioFrameDecoded(uid_t uid) {
        (void)uid;
    }

    /** 已接收到远端视频并完成解码回调。

    引擎收到第一帧远端视频流并解码成功时，触发此调用。 App 可在此回调中设置该用户的 video canvas。

     @param uid 用户 ID，指定是哪个用户的视频流。
     @param width 视频流宽（px）。
     @param height 视频流高（px）。

     */
    virtual void onFirstVideoFrameDecoded(uid_t uid, uint32_t width, uint32_t height) {
        (void)uid;
        (void)width;
        (void)height;
    } 

    virtual void onFirstVideoFrameDecoded(NERtcVideoStreamType type, uid_t uid, uint32_t width, uint32_t height) {
        (int)type;
        (void)uid;
        (void)width;
        (void)height;
    } 

    /** 当前录制统计回调。

    SDK 定期向 App 报告当前录制的统计信息，每 2 秒触发一次。

    系统相关统计信息暂时不支持, 敬请期待。

     @param stats NERECORD 引擎统计数据: NERtcStats
     */
    virtual void onRecordStats(const NERtcStats *stats) {
        (void)stats;
    }  

    /** 本地音频流统计信息回调。

    该回调描述本地设备发送音频流的统计信息，每 2 秒触发一次。

     @param stats 本地音频流统计信息。详见 NERtcAudioSendStats.
     */
    virtual void onLocalAudioStats(const NERtcAudioSendStats *stats) {
        (void)stats;
    }

    /** 通话中远端音频流的统计信息回调。

     该回调描述远端用户在通话中端到端的音频流统计信息，每 2 秒触发一次。

     @param stats 每个远端用户音频统计信息的数组。详见 NERtcAudioRecvStats.
     @param user_count stats 数组的大小。
     */
    virtual void onRemoteAudioStats(const NERtcAudioRecvStats *stats, unsigned int user_count) {
        (void)stats;
        (void)user_count;
    }

    /** 本地视频流统计信息回调。

    该回调描述本地设备发送视频流的统计信息，每 2 秒触发一次。

     @param stats 本地视频流统计信息。详见 NERtcVideoSendStats.
     */
    virtual void onLocalVideoStats(const NERtcVideoSendStats *stats) {
        (void)stats;
    }

    /** 通话中远端视频流的统计信息回调。

     该回调描述远端用户在通话中端到端的视频流统计信息，每 2 秒触发一次。

     @param stats 每个远端用户视频统计信息的数组。详见 NERtcVideoRecvStats.
     @param user_count stats 数组的大小。
     */
    virtual void onRemoteVideoStats(const NERtcVideoRecvStats *stats, unsigned int user_count) {
        (void)stats;
        (void)user_count;
    }
    
    /** 连接状态改变回调。

	  网络连接状态发生改变时触发此回调方法。
     
     @param connection_state 改变后的网络状态。
     @param reason  触发原因。
     */
    virtual void onConnectionStateChanged(NERtcConnectionStateType connection_state, NERtcErrorCode reason) {
        (void) connection_state;
        (void) reason;
    }
    
    /** 录制布局改变回调。

	  混合录制布局发生改变(不论是主动调用setVideoMixingLayout还是内置自适应布局)时触发此回调方法。
     
     @param regionCount 布局子窗口数。
     @param regions  子窗口。
     */
    virtual void onMixingLayoutChanged(uint32_t regionCount, const NERtcRegion *regions) {
        (void) regionCount;
        (void) regions;
    }
};
} //namespace nerecord

#endif
