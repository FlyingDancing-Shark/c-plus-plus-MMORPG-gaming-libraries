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
                // I want no.4 but you send me no.3 ,so...
     else if(sequenceNumber < mNextExpectedSequenceNumber)   
     {
        return false;
     }
                // I want no.4 but you send me no.5 ,so...
     else if(sequenceNumber > mNextExpectedSequenceNumber)
    {
         mNextExpectedSequenceNumber = sequenceNumber + 1;
         AddPendingAck(sequenceNumber);
         return true;
    }
} 


void DeliveryNotificationManager::AddPendingAck(
                                      PacketSequenceNumber inSequenceNumber)
{
     if(mPendingAcks.size() == 0 ||!mPendingAcks.back().ExtendIfShould(inSequenceNumber))
     {
          mPendingAcks.emplace_back(inSequenceNumber);
     }
} 




