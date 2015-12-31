#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include "TextSerializer.h"

#include "Entity.h"
#include "PhysicsPart.h"
#include "GraphicsPart.h"

TextSerializer::TextSerializer() {}
TextSerializer::~TextSerializer() {}

Serializer::data_t TextSerializer::get() {
   return this->data_;
}

bool TextSerializer::next() {
   if (!this->is_open()) {
      Service::get_logger().msg("TextSerializer", Logger::WARNING, "Cannot get next. File is not open.");
      return false;
   }

   bool has_next = true;
   std::string uncommented_line = "";

   // skip commented lines
   while (has_next && uncommented_line == "") {
      has_next = (bool) std::getline(this->file_, this->line_);

      uncommented_line = boost::trim_copy(this->remove_comments(this->line_));
   }

   if (has_next) {
      this->data_ = this->tokenize(this->line_);
   }
   return has_next;
}

bool TextSerializer::prev() {
   Service::get_logger().msg("TextSerializer", Logger::ERROR, "prev() is not implemented!");
   return false;
}

void TextSerializer::set(Entity* entity) {
   if (!this->is_open()) {
      Service::get_logger().msg("TextSerializer", Logger::ERROR, "Cannot write to data file. File is not open.");
      return;
   }

   // TODO: implement me
   // 1. convert entity to data_t
   // 2. write to file
}

std::string TextSerializer::remove_comments(std::string line) {
   std::string::size_type end_pos = line.find_first_of("#");
   
   if (end_pos == std::string::npos) {
      return line;
   }

   return boost::trim_copy(line.substr(0, end_pos));
}

Serializer::data_t TextSerializer::tokenize(std::string line) {

   line = this->remove_comments(line);
   assert(line != "");

   std::string logger_output = "tokens -> ";
   Serializer::data_t tokens;
   boost::char_separator<char> token_sep(",");
   boost::char_separator<char> kv_sep(":");
   boost::tokenizer<boost::char_separator<char> > tokenizer(line, token_sep);

   for (boost::tokenizer<boost::char_separator<char> >::iterator it = tokenizer.begin(); it != tokenizer.end(); ++it) {
      boost::tokenizer<boost::char_separator<char> > kv_tokens(*it, kv_sep);
      boost::tokenizer<boost::char_separator<char> >::iterator kv_it = kv_tokens.begin();

      assert(std::distance(kv_tokens.begin(), kv_tokens.end()) == 2);

      std::string key_str = boost::trim_copy(*kv_it);
      std::string val_str = boost::trim_copy(*++kv_it);

      tokens[key_str] = val_str;

      logger_output += key_str + ": " + val_str + ", ";
   }

   logger_output.erase(logger_output.end() - 2);
   Service::get_logger().msg("TextSerializer", Logger::INFO, logger_output);
   return tokens;
}
