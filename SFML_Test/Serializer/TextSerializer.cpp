#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/tokenizer.hpp>

#include "TextSerializer.h"

#include "Game.h"
#include "Graphic.h"
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

void TextSerializer::set(Serializer::SerializedObj& obj) {
   if (!this->is_open_outfile()) {
      if (this->filename_ == "") {
         Service::get_logger().msg("TextSerializer", Logger::ERROR, "Cannot write to data file. File is not open.");
         return;
      }

      this->open_outfile(this->filename_);
   }

   std::vector<std::string> tokens;
   std::string formatted_obj;

   Serializer::SerializedObj::const_iterator it;
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

Serializer::SerializedObj TextSerializer::serialize(Entity& entity, const std::string& type_tag) {
   Serializer::SerializedObj obj;
   sf::Vector2f pos(0, 0);
   Texture* t;

   GraphicsPart* graphics = dynamic_cast<GraphicsPart*>(entity.get("graphics"));
   if (graphics) {
      t = graphics->get(0)->get_texture();
   }

   PhysicsPart* physics = dynamic_cast<PhysicsPart*>(entity.get("physics"));
   if (physics) {
      pos = physics->get_position();
   }

   obj["type"] = type_tag;
   obj["pos_x"] = std::to_string(pos.x);
   obj["pos_y"] = std::to_string(pos.y);
   obj["texture"] = t->id();

   return obj;
}

Serializer::SerializedObj TextSerializer::serialize(Grid& grid) {
   Serializer::SerializedObj obj;
   sf::Vector2f origin = grid.origin();

   obj["type"] = "grid";
   obj["class"] = grid.get_class();
   obj["id"] = grid.id();
   obj["tile_width"] = std::to_string(grid.tile_width());
   obj["tile_height"] = std::to_string(grid.tile_height());
   obj["origin_x"] = std::to_string((int)origin.x);
   obj["origin_y"] = std::to_string((int)origin.y);

   return obj;
}

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
