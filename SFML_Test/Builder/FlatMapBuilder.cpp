#include "FlatMapBuilder.h"

#include "TextSerializer.h"

#include "TileFactory.h"
#include "TextureManager.h"

#include "OrthographicGrid.h"

#include "PhysicsPart.h"

FlatMapBuilder::FlatMapBuilder(TextureManager& tm)
: MapBuilder(tm)
{
}

FlatMapBuilder::~FlatMapBuilder() {
}

void FlatMapBuilder::build() {
   this->build_map();

   if (!this->serializer_) {
      Service::get_logger().msg("FlatMapBuilder", Logger::ERROR, "Serializer has not been initialized!");
      return;
   }

   while (this->serializer_->next()) {
      Serializer::data_t d = this->serializer_->get();

      try {
         std::string type_token = d.at("type");

         // figure out type of serialized object
         if (type_token == "grid") {
            sf::Vector2f tile_size(std::stoi(d["tile_width"]), std::stoi(d["tile_height"]));
            sf::Vector2f origin(std::stoi(d["origin_x"]), std::stoi(d["origin_y"]));
            this->build_grid(d["id"], tile_size, origin);
         } else if (type_token == "tile") {
            this->build_tile(std::stoi(d["pos_x"]), std::stoi(d["pos_y"]), d["texture"]);
         } else {
            throw std::invalid_argument("unknown type token '" + type_token + "'");
         }
      } catch (const std::out_of_range& e) {
         Service::get_logger().msg("FlatMapBuilder", Logger::ERROR, "Serializer has encountered malformed line (type specifier token missing)");
         return;
      } catch (const std::invalid_argument& e) {
         Service::get_logger().msg("FlatMapBuilder", Logger::ERROR, "Serializer has encountered malformed line (" + std::string(e.what()) + ")");
         return;
      }
   }
}

void FlatMapBuilder::build_map() {
   this->map_ = new Map();
}

void FlatMapBuilder::build_tile(int x, int y, std::string texture) {
   if (!this->map_ || !this->map_->grid()) {
      Service::get_logger().msg("FlatMapBuilder", Logger::ERROR, "Need to define a grid for map before creating tiles.");
      return;
   }

   Entity* tile = TileFactory::inst()->create_tile(this->texture_manager_.get_texture(texture), sf::Vector2f(x, y));
   this->map_->add(tile);
}

void FlatMapBuilder::build_grid(const std::string& id, int tile_size) {
   if (!this->map_) {
      this->build_map();
   }

   this->map_->add(new OrthographicGrid(id, tile_size));
}

void FlatMapBuilder::build_grid(const std::string& id, const sf::Vector2f& tile_size) {
   if (!this->map_) {
      this->build_map();
   }

   this->map_->add(new OrthographicGrid(id, tile_size));
}

void FlatMapBuilder::build_grid(const std::string& id, const sf::Vector2f& tile_size, const sf::Vector2f& origin) {
   if (!this->map_) {
      this->build_map();
   }

   this->map_->add(new OrthographicGrid(id, tile_size));
   this->map_->grid()->origin(origin);
}
