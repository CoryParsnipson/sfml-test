#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>

// ----------------------------------------------------------------------------
// Channel
//
// This is an abstract base class for stream like objects that can be read
// from and written to.
// ----------------------------------------------------------------------------
class Channel {
public:
   virtual bool send(std::string& data) = 0;
   virtual bool receive(std::string& data, unsigned int num_bytes = 0) = 0; 

   virtual void seek(int pos) = 0;
   virtual void flush() = 0;
};

#endif