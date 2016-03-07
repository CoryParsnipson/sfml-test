#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include "TextSerializer.h"

#include "Game.h"
#include "TileFactory.h"

#include "OrthographicGrid.h"

#include "Entity.h"
#include "PhysicsPart.h"
#include "GraphicsPart.h"

TextSerializer::TextSerializer(Game& game) : Serializer(game) {}
TextSerializer::~TextSerializer() {}

Serializer::SerializedObj TextSerializer::get() {
   return this->data_;
}

void TextSerializer::set(Entity* entity) {
   if (!this->is_open()) {
      Service::get_logger().msg("TextSerializer", Logger::ERROR, "Cannot write to data file. File is not open.");
      return;
   }

   // TODO: implement me
   // 1. convert entity to Serializer::SerializedObj
   // 2. write to file
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

Serializer::SerializedObj TextSerializer::serialize(Entity& entity) {}
Serializer::SerializedObj TextSerializer::serialize(Grid& grid) {}

void TextSerializer::deserialize(Serializer::SerializedObj& obj, Entity*& entity) {
   delete entity;
   entity = nullptr;

   try {
      if (obj["type"] == "tile") {
         entity = TileFactory::inst()->create_tile(
            this->game_.texture_manager.get_texture(obj["texture"]),
            sf::Vector2f(std::stoi(obj["pos_x"]), std::stoi(obj["pos_y"]))
         );
      } else {
         throw std::invalid_argument("unknown type token '" + obj["type"] + "'");
      }
   } catch (const std::out_of_range& e) {
      Service::get_logger().msg("TextSerializer", Logger::ERROR, "Serializer has encountered malformed line (type specifier token missing)");
      return;
   } catch (const std::invalid_argument& e) {
      Service::get_logger().msg("TextSerializer", Logger::ERROR, "Serializer has encountered malformed line (" + std::string(e.what()) + ")");
      return;
   }
}

void TextSerializer::deserialize(Serializer::SerializedObj& obj, Grid*& grid) {
   delete grid;
   grid = nullptr;

   try {
      if (obj["type"] == "grid") {
         if (obj["class"] == "OrthographicGrid") {
            Service::get_logger().msg("TextSerializer", Logger::INFO, "id: " + obj["id"]);
            Service::get_logger().msg("TextSerializer", Logger::INFO, "class: " + obj["class"]);
            Service::get_logger().msg("TextSerializer", Logger::INFO, "tile_width: " + obj["tile_width"]);
            Service::get_logger().msg("TextSerializer", Logger::INFO, "tile_height: " + obj["tile_height"]);

            grid = new OrthographicGrid(obj["id"], sf::Vector2f(std::stoi(obj["tile_width"]), std::stoi(obj["tile_height"])));
            grid->origin(sf::Vector2f(std::stoi(obj["origin_x"]), std::stoi(obj["origin_y"])));
         }
      } else {
         throw std::invalid_argument("unknown type token '" + obj["type"] + "'");
      }
   } catch (const std::out_of_range& e) {
      Service::get_logger().msg("TextSerializer", Logger::ERROR, "Serializer has encountered malformed line (type specifier token missing)");
      return;
   } catch (const std::invalid_argument& e) {
      Service::get_logger().msg("TextSerializer", Logger::ERROR, "Serializer has encountered malformed line (" + std::string(e.what()) + ")");
      return;
   }
}

std::string TextSerializer::remove_comments(std::string line) {
   std::string::size_type end_pos = line.find_first_of("#");

   if (end_pos == std::string::npos) {
      return line;
   }

   return boost::trim_copy(line.substr(0, end_pos));
}

Serializer::SerializedObj TextSerializer::tokenize(std::string line) {

   line = this->remove_comments(line);
   assert(line != "");

   std::string logger_output = "tokens -> ";
   Serializer::SerializedObj tokens;
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
