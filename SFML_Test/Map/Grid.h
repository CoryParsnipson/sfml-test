#ifndef GRID_H
#define GRID_H

#include "dependencies.h"
#include "Draw.h"

class Grid
: public Draw
{
public:
   Grid(const std::string& id)
   : id_(id)
   , origin_(0, 0)
   , tile_width_(0)
   , tile_height_(0)
   , show_debug_info_(false)
   {
   }

   Grid(const std::string& id, int tile_size)
   : id_(id)
   , origin_(0, 0)
   , tile_width_(tile_size)
   , tile_height_(tile_size)
   , show_debug_info_(false)
   {
   }

   Grid(const std::string& id, const sf::Vector2f& tile_size)
   : id_(id)
   , origin_(0, 0)
   , tile_width_(tile_size.x)
   , tile_height_(tile_size.y)
   , show_debug_info_(false)
   {
   }

   virtual ~Grid() {};

   virtual std::string to_string() { return "[Grid id: " + this->id_ + "]"; }

   virtual void origin(const sf::Vector2f& origin) {
      this->origin_.x = origin.x;
      this->origin_.y = origin.y;
   }
   const sf::Vector2f& origin() { return this->origin_; }

   void tile_width(int width) {
      this->tile_width_ = width;

      // re-calculate origin mod
      this->origin_mod_.x = (int)this->origin_.x % this->tile_width_;
   }
   int tile_width() { return this->tile_width_; }

   void tile_height(int height) {
      this->tile_height_ = height;
      
      // re-calculate origin mod
      this->origin_mod_.y = (int)this->origin_.y % this->tile_height_;
   }
   int tile_height() { return this->tile_height_; }

   bool show_debug() { return this->show_debug_info_; }
   void show_debug(bool show_debug_info) { this->show_debug_info_ = show_debug_info; }

   virtual sf::Vector2f coord_to_screen(const sf::Vector2f& coord) = 0;

   virtual void move(const sf::Vector2f& delta) = 0;
   virtual void set_position(const sf::Vector2f& pos) = 0;

   virtual sf::Vector2f floor(const sf::Vector2f& pos) = 0;
   virtual sf::Vector2f round(const sf::Vector2f& pos) = 0;
   virtual sf::Vector2f ceil(const sf::Vector2f& pos) = 0;

   // draw interface 
   virtual void draw(Graphics& graphics) = 0;

protected:
   std::string id_;
   sf::Vector2f origin_;
   sf::Vector2f origin_mod_;

   int tile_width_;
   int tile_height_;
   
   bool show_debug_info_;
   sf::Vector2f pan_delta_;
};

#endif
