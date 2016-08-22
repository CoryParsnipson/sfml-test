#ifndef GRID_H
#define GRID_H

#include "dependencies.h"
#include "SceneObject.h"
#include "Serialize.h"
#include "MoveCommand.h"

class Grid
: public SceneObject
, public Moveable
, public Serialize
{
public:
   Grid(const std::string& id)
   : SceneObject()
   , id_(id)
   , origin_(0, 0)
   , tile_width_(10)
   , tile_height_(10)
   , show_debug_info_(false)
   , scale_factor_(1.0)
   , font_(nullptr)
   {
   }

   Grid(const std::string& id, int tile_size)
   : SceneObject()
   , id_(id)
   , origin_(0, 0)
   , tile_width_(tile_size > 0 ? tile_size : 10)
   , tile_height_(tile_size > 0 ? tile_size : 10)
   , show_debug_info_(false)
   , scale_factor_(1.0)
   , font_(nullptr)
   {
   }

   Grid(const std::string& id, const sf::Vector2f& tile_size)
   : SceneObject()
   , id_(id)
   , origin_(0, 0)
   , tile_width_(tile_size.x > 0 ? tile_size.x : 10)
   , tile_height_(tile_size.y > 0 ? tile_size.y : 10)
   , show_debug_info_(false)
   , scale_factor_(1.0)
   , font_(nullptr)
   {
   }

   virtual ~Grid() {};

   virtual const::std::string& id() { return this->id_; }
   virtual std::string to_string() { return "[Grid id: " + this->id_ + "]"; }

   virtual void origin(const sf::Vector2f& origin) {
      this->origin_.x = origin.x;
      this->origin_.y = origin.y;
   }
   const sf::Vector2f& origin() { return this->origin_; }

   virtual void tile_width(int width) {
      this->tile_width_ = width;

      // re-calculate origin mod
      this->origin_mod_.x = (int)this->origin_.x % this->tile_width_;
   }
   virtual int tile_width() { return this->tile_width_; }

   virtual void tile_height(int height) {
      this->tile_height_ = height;

      // re-calculate origin mod
      this->origin_mod_.y = (int)this->origin_.y % this->tile_height_;
   }
   virtual int tile_height() { return this->tile_height_; }

   bool show_debug() { return this->show_debug_info_; }
   void show_debug(bool show_debug_info) { this->show_debug_info_ = show_debug_info; }

   virtual sf::Vector2f coord_to_screen(const sf::Vector2f& coord) = 0;

   void font(sf::Font* font) {
      this->font_ = font;
   }

   // move interface
   virtual void move(const sf::Vector2f& delta) = 0;

   virtual void set_scale(float factor) = 0;
   virtual void set_position(const sf::Vector2f& pos) = 0;

   virtual sf::Vector2f floor(const sf::Vector2f& pos) = 0;
   virtual sf::Vector2f round(const sf::Vector2f& pos) = 0;
   virtual sf::Vector2f ceil(const sf::Vector2f& pos) = 0;

   // serialize interface
   virtual Serialize::SerialObj serialize() = 0;
   virtual void deserialize(Serialize::SerialObj& obj, const TextureAtlas* textures = nullptr) = 0;

protected:
   std::string id_;
   sf::Vector2f origin_;
   sf::Vector2f origin_mod_;

   int tile_width_;
   int tile_height_;

   bool show_debug_info_;
   sf::Vector2f pan_delta_;
   float scale_factor_;

   sf::Font* font_;
};

#endif
