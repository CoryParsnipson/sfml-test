#include "FileChannel.h"

#include <iostream>

FileChannel::FileChannel(std::string filename) {
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

bool FileChannel::send(std::string data) {
   this->file_.write(data.c_str(), data.length());
   return (bool)this->file_;
}

bool FileChannel::receive(std::string& data, unsigned int num_bytes) {
   unsigned int length = std::max(num_bytes, (unsigned int)1);
   char* buffer = new char [length];

   this->file_.read(buffer, length);
   data.assign(buffer, this->file_.gcount());
   delete buffer;

   return (bool)this->file_;
}

void FileChannel::seek(int pos, Channel::Offset o /* = Channel::Offset::Beginning */) {
   std::ios_base::seekdir dir;

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

   this->file_.seekg(pos, dir);
   this->file_.seekp(pos, dir);
}

void FileChannel::flush() {
   this->file_.flush();
}
