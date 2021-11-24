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
         mNextExpectedSequenceNumber = sequenceNumber + 1;
         AddPendingAck(sequenceNumber); 
         return true;
     }
                // arrived out of order, so drop expired packet
     else if(sequenceNumber < mNextExpectedSequenceNumber)   
     {
        return false;
     }
                // miss a previously packet, proceed to current one
     else if(sequenceNumber > mNextExpectedSequenceNumber)
    {
         mNextExpectedSequenceNumber = sequenceNumber + 1;
         AddPendingAck(sequenceNumber);
         return true;
    }
} 

