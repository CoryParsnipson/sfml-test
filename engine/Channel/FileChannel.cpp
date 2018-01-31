#include "FileChannel.h"

#include <iostream>

FileChannel::FileChannel(std::string filename)
: filename_(filename)
, was_read_since_last_write_(false)
, was_written_since_last_read_(false)
{
   // create file if it does not exist
   this->file_.open(filename, std::fstream::out | std::fstream::app);
   this->file_.close();

   // now open the file for real
   this->file_.open(filename, std::fstream::binary | std::fstream::in | std::fstream::out);
}

FileChannel::~FileChannel() {
   this->file_.flush();
   this->file_.close();
}

void FileChannel::clear() {
   this->file_.clear();
}

bool FileChannel::send(std::string data) {
   if (this->was_read_since_last_write_) {
      // need to do a seekp if this stream was read before this function was called
      this->seek(0, Channel::Offset::Current);
      this->was_read_since_last_write_ = false;
   }
   this->was_written_since_last_read_ = true;

   this->file_.write(data.c_str(), data.length());
   return (bool)this->file_;
}

bool FileChannel::receive(std::string& data, unsigned int num_bytes) {
   unsigned int length = std::max(num_bytes, (unsigned int)1);
   char* buffer = new char [length];

   if (this->was_written_since_last_read_) {
      // need to flush if a write was done on the stream before this function was called
      this->flush();
      this->was_written_since_last_read_ = false;
   }
   this->was_read_since_last_write_ = true;

   this->file_.read(buffer, length);
   data.assign(buffer, this->file_.gcount());
   delete buffer;

   return (bool)this->file_;
}

void FileChannel::seek(int pos, Channel::Offset o /* = Channel::Offset::Beginning */) {
   std::ios_base::seekdir dir;

   // seek will not work if eofbit is set, clear it
   this->clear();

   switch (o) {
   case Channel::Offset::Beginning:
      dir = std::ios_base::beg;
      break;
   case Channel::Offset::End:
      dir = std::ios_base::end;
      break;
   default:
      dir = std::ios_base::cur;
      break;
   }

   this->file_.seekp(pos, dir);
}

void FileChannel::flush() {
   this->file_.flush();
}

void FileChannel::remove() {
   this->file_.close();
   
   this->file_.open(this->filename_, std::fstream::out | std::fstream::trunc);
   this->file_.close();

   this->file_.open(this->filename_, std::fstream::binary | std::fstream::in | std::fstream::out);
}

const std::string& FileChannel::filename() const {
   return this->filename_;
}
