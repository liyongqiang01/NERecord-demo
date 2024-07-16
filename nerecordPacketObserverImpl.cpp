#include "nerecordPacketObserverImpl.h"
// #include <openssl/rc4.h>
#include <memory>
#include <cstring>

RecordPacketObserverImpl::RecordPacketObserverImpl() {
    
}

RecordPacketObserverImpl::~RecordPacketObserverImpl() {
    
}

bool RecordPacketObserverImpl::onSendAudioPacket(NERecordMediaPacket& packet)
{
    return true;
}

bool RecordPacketObserverImpl::onSendVideoPacket(NERecordMediaPacket& packet)
{
    return true;
}

bool RecordPacketObserverImpl::onReceiveAudioPacket(NERecordMediaPacket& packet)
{
    // printf("-----------------audioPacket-----------------\n");
#if 1
    int size = packet.size;
    unsigned char *buffer = (unsigned char *)packet.buffer;
    for (int i = 0; i < size; ++i) {
        buffer[i] = ~(buffer[i]);
    }
#else
    std::string key = "I_AM_A_KEY";
    RC4_KEY rc4_key;
    RC4_set_key(&rc4_key, key.length(), (const unsigned char *)key.c_str());

    int length = packet.size;
    unsigned char *buffer = (unsigned char *)packet.buffer;
    std::unique_ptr<unsigned char[]> decrypt(new unsigned char[length]);
    RC4(&rc4_key, length, buffer, decrypt.get());
    memcpy(buffer, decrypt.get(), length);
#endif
    return true;
}

bool RecordPacketObserverImpl::onReceiveVideoPacket(NERecordMediaPacket& packet)
{
    // printf("-----------------VideoPacket-----------------\n");
#if 1
    int size = packet.size;
    unsigned char *buffer = (unsigned char *)packet.buffer;
    for (int i = 0; i < size; ++i) {
        buffer[i] = ~(buffer[i]);
    }
#else
    std::string key = "I_AM_A_KEY";
    RC4_KEY rc4_key;
    RC4_set_key(&rc4_key, key.length(), (const unsigned char *)key.c_str());

    int length = packet.size;
    unsigned char *buffer = (unsigned char *)packet.buffer;
    std::unique_ptr<unsigned char[]> decrypt(new unsigned char[length]);
    RC4(&rc4_key, length, buffer, decrypt.get());
    memcpy(buffer, decrypt.get(), length);
#endif
    return true;
}