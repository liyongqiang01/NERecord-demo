#include "nerecordEventHandlerImpl.h"


void TestEventHandler::onError(int error_code, const char* msg)
{
    printf("*****************[%d]TestEventHandler::onError: %d - %s\n", getpid(), error_code, msg);
}

void TestEventHandler::onWarning(int warn_code, const char* msg)
{
    printf("*****************[%d]TestEventHandler::onWarning: %d - %s\n", getpid(), warn_code, msg);
}

void TestEventHandler::onDisconnect(NERtcErrorCode reason)
{
    printf("*****************[%d]TestEventHandler::onDisconnect\n", getpid());
}

void TestEventHandler::onConnectInterrupted()
{
    printf("*****************[%d]TestEventHandler::onConnectInterrupted\n", getpid());
}

void TestEventHandler::onJoinChannel(channel_id_t cid, uint64_t uid, NERtcErrorCode result, uint64_t elapsed)
{
    printf("*****************pid: [%d]TestEventHandler::onJoinChannel: cid:%lu - uid:%lu - result:%d -elapsed:%lu\n", getpid(), cid, uid, result, elapsed);
}

void TestEventHandler::onConnectionStateChanged(nerecord::NERtcConnectionStateType connection_state, nerecord::NERtcErrorCode reason)
{
    printf("*****************[%d]TestEventHandler::onConnectionStateChanged: state %u- reason %u\n", getpid(), connection_state, reason);
}

void TestEventHandler::onMixingLayoutChanged(uint32_t regionCount, const NERtcRegion *regions)
{
    printf("*****************[%d]TestEventHandler::onMixingLayoutChanged: region count %u- regions: %p\n", getpid(), regionCount, regions);
}

 void TestEventHandler::onRejoinChannel(channel_id_t cid, uint64_t uid, NERtcErrorCode result, uint64_t elapsed)
{
    printf("*****************[%d]TestEventHandler::onRejoinChannel: %lu - %lu - %d - %lu\n", getpid(), cid, uid, result, elapsed);
}
void TestEventHandler::onLeaveChannel(NERtcErrorCode result)
{
    printf("*****************[%d]TestEventHandler::onLeaveChannel: %d\n", getpid(), result);
}

void TestEventHandler::onUserJoined(uint64_t uid, const char * user_name)
{
    printf("*****************[%d]TestEventHandler::onUserJoined: %lu\n", getpid(), uid);
    if (g_written_file)
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        char log[256];
        sprintf(log, "%s%u-%lu-audio-%ld", g_log_dir, getpid(), uid, tv.tv_sec);
        printf("audio file:%s\n", log);
        g_audio_fd_map[uid] = open(log, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        
        if (g_audio_fd_map[uid] < 0)
        {
            printf("[%d]create audio file error!!!\n", getpid());
            g_video_file_map.erase(uid);
            return;
        }
    }
}


void TestEventHandler::onUserLeft(uint64_t uid, NERtcSessionLeaveReason reason)
{
    printf("*****************[%d]TestEventHandler::onUserLeft: %lu - %d\n", getpid(), uid, reason);
    if (g_written_file)
    {
        int ret = close(g_audio_fd_map[uid]);
        if (ret < 0)
        {
            printf("[%d]close file error!!!\n", getpid());
            return;
        }
        g_audio_fd_map.erase(uid);
        
        if (g_video_file_map.end() != g_video_file_map.find(uid))
        {
            close(g_video_file_map[uid].fd);
            g_video_file_map.erase(uid);
        }
    }
}

void TestEventHandler::onUserAudioStart(uint64_t uid)
{
    printf("*****************[%d]TestEventHandler::onUserAudioStart: %lu\n", getpid(), uid);
}

void TestEventHandler::onUserAudioStop(uint64_t uid)
{
    printf("*****************[%d]TestEventHandler::onUserAudioStop: %lu\n", getpid(), uid);
}

void TestEventHandler::onUserAudioMute(nerecord::uid_t uid, bool mute)
{
    printf("*****************[%d]TestEventHandler::onUserAudioMute: %lu - mute:%d\n", getpid(), uid, mute);
}

void TestEventHandler::audioFrameReceived(nerecord::uid_t uid, const NERtcAudioFrame *frame)
{
    if (verbose) printf("*****************[%d]TestEventHandler::audioFrameReceived: %lu [sample_rate: %d, ts: %lu, samples:%d]\n",
                                              getpid(), uid, frame->format.sample_rate, frame->format.timestamp, frame->format.samples_per_channel);
    // std::cout << "tid = " << std::this_thread::get_id() << "\t";
    //printf("[%s:%s:%d, uid=%lu] timestamp = %lu\n", __FILE__, __func__, __LINE__, uid, frame->format.timestamp);
    if (false && g_written_file)
    {
        int len = frame->format.channels * frame->format.channels * frame->format.samples_per_channel * sizeof(int16_t);
        // printf("[%d]audioFrameReceived: %lu - %d - cn %d  sam %d\n", getpid(), uid, len, frame->format.channels, frame->format.sample_rate);
        int ret = write(g_audio_fd_map[uid], (const void *)frame->data, len);
        if (ret < 0)
        {
            printf("*****************[%d]write data error!!!%d\n", getpid(), errno);
            return;
        }
     }
}


 void TestEventHandler::onUserVideoStart(uint64_t uid, NERtcVideoProfileType max_profile)
 {
    printf("*****************[%d]TestEventHandler::onUserVideoStart: %lu - max_profile = %d\n", getpid(), uid, max_profile);
 }


 void TestEventHandler::onUserVideoStop(uint64_t uid)
 {
    printf("*****************[%d]TestEventHandler::onUserVideoStop: %lu\n", getpid(), uid);
    
    if (g_written_file)
    {
        if (g_video_file_map.end() != g_video_file_map.find(uid))
        {
            close(g_video_file_map[uid].fd);
            g_video_file_map.erase(uid);
        }
    }
 }


 void TestEventHandler::onUserVideoMute(nerecord::uid_t uid, bool mute)
 {
    printf("*****************[%d]TestEventHandler::onUserVideoMute: %lu - mute = %s\n", getpid(), uid, mute? "true " : "false");
 }

void TestEventHandler::onRemoteAudioVolumeIndication(const NERtcAudioVolumeInfo *speakers, unsigned int speaker_number, int total_volume)
{
    if (verbose) printf("*****************[%d]TestEventHandler::onRemoteAudioVolumeIndication: total_volume = %d\n", getpid(), total_volume);
    for (unsigned int i = 0; i < speaker_number; ++i)
    {
        printf("\t%lu\tvol: %u\n", speakers[i].uid, speakers[i].volume);
    }
}

void TestEventHandler::onActiveSpeaker(nerecord::uid_t uid)
{
    if (verbose) printf("*****************[%d]TestEventHandler::onActiveSpeaker: uid = %lu\n", getpid(), uid);
}

void TestEventHandler::onFirstAudioDataReceived(nerecord::uid_t uid)
 {
    printf("*****************[%d]TestEventHandler::onFirstAudioDataReceived: %lu\n", getpid(), uid);
 }

void TestEventHandler::onFirstVideoDataReceived(nerecord::uid_t uid)
 {
    printf("*****************[%d]TestEventHandler::onFirstVideoDataReceived: %lu\n", getpid(), uid);
 }
 
void TestEventHandler::onFirstAudioFrameDecoded(nerecord::uid_t uid) 
 {
    printf("*****************[%d]TestEventHandler::onFirstAudioFrameDecoded: %lu\n", getpid(), uid);
 }
 
void TestEventHandler::onFirstVideoFrameDecoded(nerecord::uid_t uid, uint32_t width, uint32_t height) 
 {
    printf("*****************[%d]TestEventHandler::onFirstVideoFrameDecoded: %lu - %ux%u\n", getpid(), uid, width, height);
 }

 void TestEventHandler::onRecordStats(const nerecord::NERtcStats *stats)
 {
    if (verbose) printf("*****************[%d]TestEventHandler::onRtcStats\n", getpid());
    if (verbose) printf("RtcStats:\t duration: %dS\n"
        "   [cpu] app:%d%%,\t idle:%d%%,\t total:%d%%\n"
        "   [memory] app:%d%%(%dMB),\t total:%d%%\n"
        "   [TX] %lldKB(rtt:%dms),\t audio:%lldKB,\t video:%lldKB\n"
        "   [RX] %lldKB,\t audio:%lldKB,\t video:%lldKB\n"
        "   [BitRate] a_tx:%d a_rx:%dKbps,\t v_tx:%d v_rx:%dKbps\n"
        "   [TXloss] audio:%d%%(%d),\t video:%d%%(%d)\n"
        "   [RXloss] audio:%d%%(%d),\t video:%d%%(%d)\n"
        "   [Jitter] tx:a-%d v-%d,\t rx:a-%d v-%d\n",
        stats->total_duration,
        stats->cpu_app_usage,
        stats->cpu_idle_usage,
        stats->cpu_total_usage,
        stats->memory_app_usage,
        stats->memory_app_kbytes / 1024,
        stats->memory_total_usage,
        stats->tx_bytes / 1024, stats->up_rtt, stats->tx_audio_bytes / 1024, stats->tx_video_bytes / 1024,
        stats->rx_bytes / 1024, stats->rx_audio_bytes / 1024, stats->rx_video_bytes / 1024,
        stats->tx_audio_kbitrate, stats->rx_audio_kbitrate, stats->tx_video_kbitrate, stats->rx_video_kbitrate,
        stats->tx_audio_packet_loss_rate, stats->tx_audio_packet_loss_sum, stats->tx_video_packet_loss_rate, stats->tx_video_packet_loss_sum,
        stats->rx_audio_packet_loss_rate, stats->rx_audio_packet_loss_sum, stats->rx_video_packet_loss_rate, stats->rx_video_packet_loss_sum,
        stats->tx_audio_jitter, stats->tx_video_jitter, stats->rx_audio_jitter, stats->rx_video_jitter);
 }

void TestEventHandler::onLocalAudioStats(const nerecord::NERtcAudioSendStats *stats) 
 {
    //todo
 }

void TestEventHandler::onRemoteAudioStats(const nerecord::NERtcAudioRecvStats *stats, unsigned int user_count)
{

}


void TestEventHandler::onLocalVideoStats(const nerecord::NERtcVideoSendStats *stats)
 {
    
 }


void TestEventHandler::onRemoteVideoStats(const nerecord::NERtcVideoRecvStats *stats_vec, unsigned int user_count)
 {

 }


 void TestEventHandler::videoFrameReceived(nerecord::uid_t uid, const NERtcVideoFrame *frame)
{
   
}

void TestEventHandler::videoSubstreamFrameReceived(nerecord::uid_t uid, const NERtcVideoFrame *frame)
{
    if (verbose) printf("*****************[%d]TestEventHandler::videoSubstreamFrameReceived [ts:%lu]\n", getpid(), frame->timestamp);
}

int TestEventHandler::get_video_file_fd(nerecord::uid_t uid, const NERtcVideoFrame *frame)
{
    if (g_video_file_map.find(uid) == g_video_file_map.end())
    {
        char log[512];
        struct timeval tv;

        gettimeofday(&tv, NULL);
        sprintf(log, "%s%u-%lu-video-%ux%u-%ld", g_log_dir, getpid(), uid, frame->width, frame->height, tv.tv_sec);
        printf("video file:%s\n", log);
        video_file_struct vfd = video_file_struct();
        vfd.height = frame->height;
        vfd.width = frame->width;
        vfd.fd = open(log, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (vfd.fd < 0)
        {
            int errno_ = vfd.fd;
            printf("[%d]create video file error!!!\n", getpid());
            g_video_file_map.erase(uid);
            return errno_;
        }

        g_video_file_map[uid] = vfd;
    }

    if ((g_video_file_map[uid].height != frame->height) || (g_video_file_map[uid].width != frame->width))
    {
        if (verbose) printf("Frame format is mismatch: expect [%ux%u], frame[%ux%u]\n", g_video_file_map[uid].width, g_video_file_map[uid].height, frame->width, frame->height);
        return -1;
    }

    return g_video_file_map[uid].fd;
}