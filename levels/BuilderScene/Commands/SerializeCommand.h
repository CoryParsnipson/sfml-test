#ifndef SERIALIZE_COMMAND_H
#define SERIALIZE_COMMAND_H

#include <memory>

#include "Command.h"
#include "Channel.h"
#include "Serializer.h"
#include "Serializable.h"

// ----------------------------------------------------------------------------
// SerializeCommand
//
// This command serializes a given Serializable object and writes it to the
// provided channel.
// ----------------------------------------------------------------------------
class SerializeCommand : public Command {
public:
   SerializeCommand(Serializable& obj, std::shared_ptr<Serializer> serializer, Channel& c);
   virtual ~SerializeCommand();

   virtual void execute();
   virtual void unexecute();

protected:
   Serializable* serializable_;
   std::shared_ptr<Serializer> serializer_;
   Channel* channel_;
};

#endif
