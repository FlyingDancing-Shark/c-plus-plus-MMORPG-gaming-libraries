InFlightPacket* DeliveryNotificationManager::WriteSequenceNumber(
 OutputMemoryBitStream& inPacket)
{
 PacketSequenceNumber sequenceNumber = mNextOutgoingSequenceNumber++;
 inPacket.Write(sequenceNumber);
 ++mDispatchedPacketCount;
 mInFlightPackets.emplace_back(sequenceNumber);
 return &mInFlightPackets.back();
 } 
