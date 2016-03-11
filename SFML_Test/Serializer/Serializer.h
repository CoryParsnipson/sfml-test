#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "dependencies.h"
#include "Serialize.h"

class Grid;
class Entity;

class Serializer {
public:

   Serializer(Game& game) : game_(game) {};
   virtual ~Serializer() {
      this->close_infile();
      this->close_outfile();
   }

   virtual void open_outfile(std::string filename) { // close in_file
      this->close_infile();

      this->out_file_.open(filename, std::fstream::out);

      if (!this->is_open_outfile()) {
         Service::get_logger().msg("Serializer", Logger::ERROR, "Opening '" + filename + "' failed.");
      }

      this->filename_ = filename;
   }

   virtual void open_infile(std::string filename) {
      // close out_file_
      this->close_outfile();

      this->in_file_.open(filename, std::fstream::in);

      if (!this->is_open_infile()) {
         Service::get_logger().msg("Serializer", Logger::ERROR, "Opening '" + filename + "' failed.");
      }

      this->filename_ = filename;
   }

   virtual bool is_open_outfile() {
      return this->out_file_.is_open();
   };

   virtual bool is_open_infile() {
      return this->in_file_.is_open();
   }

   virtual void close_infile() {
      this->in_file_.close();
   };

   virtual void close_outfile() {
      this->out_file_.close();
   };

   virtual Serialize::SerialObj get() = 0;
   virtual void set(const Serialize::SerialObj& obj) = 0;
   virtual void set(Serialize& serializable) = 0;

   virtual void comment(const std::string& comment) = 0;

   virtual bool next() = 0;
   virtual bool prev() = 0;

protected:
   Game& game_;

   std::string filename_;

   std::ifstream in_file_;
   std::ofstream out_file_;
};

#endif
