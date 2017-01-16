#ifndef FILE_CHANNEL_H
#define FILE_CHANNEL_H

#include <fstream>

#include "Channel.h"

class FileChannel : public Channel {
public:
   FileChannel(std::string filename);
   virtual ~FileChannel();

   virtual bool send(std::string& data);
   virtual bool receive(std::string& data, unsigned int num_bytes);

   virtual void seek(int pos, Channel::Offset o = Channel::Offset::Beginning);
   virtual void flush();

private:
   std::fstream file_;
};

#endif
