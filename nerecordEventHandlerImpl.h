#ifndef RECORD_EVENT_HANDLER_H
#define RECORD_EVENT_HANDLER_H

#include <mcheck.h>
#include <unistd.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <atomic>
#include <map>
#include <sys/time.h>
#include <sys/mman.h>
#include "./sdk/include/nerecord_base_types.h"
#include "./sdk/include/nerecord_engine.h"
#include "./sdk/include/nerecord_engine_event_handler.h"
#include "./fjson/json_builder/fjson_builder.h"
#include "./fjson/json_parser/fjson_parser.h"
#include "./fjson/json_helper.h"
// #include <extension/encrypt/url_encode.h>
// #include <HttpClient.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <streambuf>

using namespace nerecord;

struct video_file_struct
{
    int fd;
    uint32_t height;
    uint32_t width;
};


class TestEventHandler : public IRecordEngineEventHandler
{
private:
    /* data */
public:
    TestEventHandler(/* args */) {}
    virtual ~TestEventHandler() {}

public:
    void setVerbose(bool enable){verbose = enable;}

public:
    virtual void onError(int error_code, const char* msg) override;
    virtual void onWarning(int warn_code, const char* msg)  override;
    virtual void onDisconnect(NERtcErrorCode reason) override;
    virtual void onConnectInterrupted() override;
    virtual void onJoinChannel(channel_id_t cid, uint64_t uid, NERtcErrorCode result, uint64_t elapsed) override;
    virtual void onRejoinChannel(channel_id_t cid, uint64_t uid, NERtcErrorCode result, uint64_t elapsed) override;
    virtual void onLeaveChannel(NERtcErrorCode result) override;
    virtual void onUserJoined(uint64_t uid, const char * user_name) override;
    virtual void onUserLeft(uint64_t uid, NERtcSessionLeaveReason reason) override;
    virtual void onUserAudioStart(uint64_t uid) override;
    virtual void onUserAudioStop(uint64_t uid) override;
    virtual void onUserAudioMute(nerecord::uid_t uid, bool mute) override;
    virtual void audioFrameReceived(nerecord::uid_t uid, const NERtcAudioFrame *frame) override;
    virtual void onUserVideoStart(uint64_t uid, NERtcVideoProfileType max_profile) override;
    virtual void onUserVideoStop(uint64_t uid) override;
    virtual void onUserVideoMute(nerecord::uid_t uid, bool mute) override;
    virtual void videoFrameReceived(nerecord::uid_t uid, const NERtcVideoFrame *frame) override;
    virtual void videoSubstreamFrameReceived(nerecord::uid_t uid, const NERtcVideoFrame *frame) override;
    virtual void onRemoteAudioVolumeIndication(const NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume) override;
    virtual void onActiveSpeaker(nerecord::uid_t uid) override;
    virtual void onFirstAudioDataReceived(nerecord::uid_t uid) override;
    virtual void onFirstVideoDataReceived(nerecord::uid_t uid) override;
    virtual void onFirstAudioFrameDecoded(nerecord::uid_t uid)  override;
    virtual void onFirstVideoFrameDecoded(nerecord::uid_t uid, uint32_t width, uint32_t height) override;
    virtual void onRecordStats(const NERtcStats *stats) override;
    virtual void onLocalAudioStats(const nerecord::NERtcAudioSendStats *stats) override;
    virtual void onRemoteAudioStats(const nerecord::NERtcAudioRecvStats *stats, unsigned int user_count) override;
    virtual void onLocalVideoStats(const nerecord::NERtcVideoSendStats *stats)  override;
    virtual void onRemoteVideoStats(const nerecord::NERtcVideoRecvStats *stats, unsigned int user_count) override;
    virtual void onConnectionStateChanged(nerecord::NERtcConnectionStateType connection_state, nerecord::NERtcErrorCode reason) override;
    virtual void onMixingLayoutChanged(uint32_t regionCount, const NERtcRegion *regions) override;

private:
    int get_video_file_fd(nerecord::uid_t uid, const NERtcVideoFrame *frame);

private:
    bool g_written_file = false;
    std::map<uint64_t, video_file_struct> g_video_file_map;
    std::map<uint64_t, int> g_audio_fd_map;
    char g_log_dir[200];
    bool verbose = false;
};


#endif