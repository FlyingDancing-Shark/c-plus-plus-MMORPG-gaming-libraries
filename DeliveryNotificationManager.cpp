InFlightPacket* DeliveryNotificationManager::WriteSequenceNumber(
 OutputMemoryBitStream& inPacket)
{
    PacketSequenceNumber sequenceNumber = mNextOutgoingSequenceNumber++;
    inPacket.Write(sequenceNumber);
    ++mDispatchedPacketCount;
    mInFlightPackets.emplace_back(sequenceNumber);
    return &mInFlightPackets.back();
} 


bool DeliveryNotificationManager::ProcessSequenceNumber(
                                      InputMemoryBitStream& inPacket)
{
     PacketSequenceNumber sequenceNumber;
     inPacket.Read(sequenceNumber);
     if(sequenceNumber == mNextExpectedSequenceNumber)
     {
//is this expected? add ack to the pending list and process packet
         mNextExpectedSequenceNumber = sequenceNumber + 1;
         AddPendingAck(sequenceNumber); 
         return true;
     }
 //is sequence number < current expected? Then silently drop old packet.
     else if(sequenceNumber < mNextExpectedSequenceNumber)
     {
        return false;
     }
 //otherwise, we missed some packets
     else if(sequenceNumber > mNextExpectedSequenceNumber)
    {
      //consider all skipped packets as dropped, so
      //our next expected packet comes after this one ...
         mNextExpectedSequenceNumber = sequenceNumber + 1;
 //add an ack for the packet and process it
//when the sender detects break it acks, it can resend
         AddPendingAck(sequenceNumber);
         return true;
    }
} 

