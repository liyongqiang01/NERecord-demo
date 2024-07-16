#ifndef RECORD_PACKAGE_OBSERVER_H
#define RECORD_PACKAGE_OBSERVER_H

#include "./sdk/include/nerecord_engine_defines.h"

using namespace nerecord;

class RecordPacketObserverImpl : public INERecordPacketObserver
{
public:
    RecordPacketObserverImpl();
    ~RecordPacketObserverImpl();

public:
    virtual bool onSendAudioPacket(NERecordMediaPacket& packet) override;
    virtual bool onSendVideoPacket(NERecordMediaPacket& packet) override;
    virtual bool onReceiveAudioPacket(NERecordMediaPacket& packet) override;
    virtual bool onReceiveVideoPacket(NERecordMediaPacket& packet) override;

 
};

#endif //RECORD_PACKAGE_OBSERVER_H