#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/tokenizer.hpp>

#include "TextSerializer.h"

TextSerializer::TextSerializer() : Serializer() {}
TextSerializer::~TextSerializer() {}

Serialize::SerialObj TextSerializer::get() {
   return this->data_;
}

void TextSerializer::set(const Serialize::SerialObj& obj) {
   if (!this->is_open_outfile()) {
      if (this->filename_ == "") {
         Service::get_logger().msg("TextSerializer", Logger::ERROR, "Cannot write to data file. File is not open.");
         return;
      }

      this->open_outfile(this->filename_);
   }

   std::vector<std::string> tokens;
   std::string formatted_obj;

   Serialize::SerialObj::const_iterator it;
   for (it = obj.begin(); it != obj.end(); ++it) {
      tokens.push_back(it->first + ": " + it->second);
   }

   formatted_obj = boost::algorithm::join(tokens, ", ");
   Service::get_logger().msg("TextSerializer", Logger::INFO, "Writing -> " + formatted_obj);

   this->out_file_ << formatted_obj << std::endl;
   this->out_file_.flush();
}

void TextSerializer::comment(const std::string& comment) {
   if (!this->is_open_outfile()) {
      if (this->filename_ == "") {
         Service::get_logger().msg("TextSerializer", Logger::ERROR, "Cannot write to data file. File is not open.");
         return;
      }

      this->open_outfile(this->filename_);
   }

   std::string formatted_comment = "# " + comment;
   Service::get_logger().msg("TextSerializer", Logger::INFO, "Writing comment: " + formatted_comment);

   this->out_file_ << std::endl << formatted_comment << std::endl;
   this->out_file_.flush();
}

bool TextSerializer::next() {
   if (!this->is_open_infile()) {
      if (this->filename_ == "") {
         Service::get_logger().msg("TextSerializer", Logger::ERROR, "Cannot read from data file. File is not open.");
         return false;
      }

      this->open_infile(this->filename_);
   }

   bool has_next = true;
   std::string raw_line = "";
   std::string uncommented_line = "";

   // skip commented lines
   while (has_next && uncommented_line == "") {
      has_next = (bool) std::getline(this->in_file_, raw_line);

      uncommented_line = boost::trim_copy(this->remove_comments(raw_line));
   }

   if (has_next) {
      this->data_ = this->tokenize(raw_line);
   }
   return has_next;
}

bool TextSerializer::prev() {
   Service::get_logger().msg("TextSerializer", Logger::ERROR, "Prev not implemented.");
   return false;
}

std::string TextSerializer::remove_comments(std::string line) {
   std::string::size_type end_pos = line.find_first_of("#");

   if (end_pos == std::string::npos) {
      return line;
   }

   return boost::trim_copy(line.substr(0, end_pos));
}

Serialize::SerialObj TextSerializer::tokenize(std::string line) {
   line = this->remove_comments(line);
   assert(line != "");

   std::string logger_output = "tokens -> ";
   Serialize::SerialObj tokens;
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
