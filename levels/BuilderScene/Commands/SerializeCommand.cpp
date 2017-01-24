#include "SerializeCommand.h"

SerializeCommand::SerializeCommand(Serializable& obj, std::shared_ptr<Serializer> serializer, Channel& c)
: serializable_(&obj)
, serializer_(std::move(serializer))
, channel_(&c)
{
}

SerializeCommand::~SerializeCommand() {
}

void SerializeCommand::execute() {
   this->channel_->seek(0);
   this->channel_->send(this->serializable_->serialize(*this->serializer_.get()));
}

void SerializeCommand::unexecute() {
}
