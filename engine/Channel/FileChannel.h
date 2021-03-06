#ifndef FILE_CHANNEL_H
#define FILE_CHANNEL_H

#include <fstream>
#include <algorithm>

#include "Channel.h"

class FileChannel : public Channel {
public:
   FileChannel(std::string filename);
   virtual ~FileChannel();

   virtual void clear();

   virtual bool send(std::string data);
   virtual bool receive(std::string& data, unsigned int num_bytes);

   virtual void seek(int pos, Channel::Offset o = Channel::Offset::Beginning);
   virtual void flush();

   virtual void remove();

   const std::string& filename() const;

private:
   std::fstream file_;
   std::string filename_;

   bool was_read_since_last_write_;
   bool was_written_since_last_read_;
};

#endif
