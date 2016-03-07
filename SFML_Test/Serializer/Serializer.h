#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "dependencies.h"

class Grid;
class Entity;

class Serializer {
public:
   typedef std::map<std::string, std::string> SerializedObj;

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

   virtual SerializedObj get() = 0;
   virtual void set(SerializedObj& obj) = 0;

   virtual void comment(const std::string& comment) = 0;

   virtual bool next() = 0;
   virtual bool prev() = 0;

   virtual SerializedObj serialize(Entity& entity, const std::string& type_tag) = 0;
   virtual SerializedObj serialize(Grid& grid) = 0;
   //virtual SerializedObj serialize(Layer& layer) = 0;

   virtual void deserialize(Serializer::SerializedObj& obj, Entity*& entity) = 0;
   virtual void deserialize(Serializer::SerializedObj& obj, Grid*& grid) = 0;

protected:
   Game& game_;

   std::string filename_;

   std::ifstream in_file_;
   std::ofstream out_file_;
};

#endif
