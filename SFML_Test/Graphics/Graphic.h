#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "dependencies.h"
#include "Draw.h"
#include "Graphics.h"
#include "TextFactory.h"
#include "Texture.h"

class Graphic
: public Draw
{
public:
   Graphic() {}
   virtual ~Graphic() {}

   // draw interface
   virtual void draw(Graphics& graphics, Layer& layer) = 0;

   // common sfml drawable interface
   virtual sf::FloatRect get_local_bounds() const = 0;
   virtual sf::FloatRect get_global_bounds() const = 0;

   virtual void set_position(float x, float y) = 0;
   virtual void set_position(const sf::Vector2f& pos) = 0;

   virtual void set_size(float x, float y) = 0;
   virtual void set_size(const sf::Vector2f& size) = 0;

   virtual void set_rotation(float angle) = 0;
   
   virtual void set_scale(float factorX, float factorY) = 0;
   virtual void set_scale(const sf::Vector2f& factors) = 0;

   virtual void set_origin(float x, float y) = 0;
   virtual void set_origin(const sf::Vector2f& origin) = 0;

   virtual void set_color(const sf::Color& color) = 0;
   
   virtual const sf::Vector2f& get_position() const = 0;

   virtual sf::Vector2f get_size() const = 0;

   virtual float get_rotation() const = 0;
   
   virtual const sf::Vector2f& get_scale() const = 0;
   
   virtual const sf::Vector2f& get_origin() const = 0;

   virtual const sf::Color& get_color() const = 0;

   virtual void move(float offsetX, float offsetY) = 0;
   virtual void move(const sf::Vector2f& offset) = 0;

   virtual void rotate(float angle) = 0;

   virtual void scale(float factorX, float factorY) = 0;
   virtual void scale(const sf::Vector2f& factor) = 0;

   // sprite/shape interface
   virtual void set_texture(Texture& texture) {}
   virtual void set_texture_rect(const sf::IntRect& rect) {}
   virtual void set_fill_color(const sf::Color& color) {}
   virtual void set_outline_color(const sf::Color& color) {}
   virtual void set_outline_thickness(float thickness) {}

   virtual Texture* get_texture() const { return nullptr;} 
   virtual const sf::Color& get_fill_color() const { return sf::Color::Transparent; }
   virtual const sf::Color& get_outline_color() const { return sf::Color::Transparent; }
   virtual float get_outline_thickness() const { return 0; }

   // sf::Text interface
   virtual void set_string(const std::string& s) {}
   virtual void set_font(sf::Font* font) {}
   virtual void set_character_size(unsigned int size) {}
   virtual void set_style(sf::Text::Style style) {}

   virtual std::string get_string() { return ""; }
   virtual const sf::Font* get_font() { return nullptr; }
   virtual sf::Vector2f find_character_pos(std::size_t index) const { return sf::Vector2f(0, 0); }
};

class Text : public Graphic
{
public:
   Text()
   : drawable_(new sf::Text())
   {
   }

   Text(const char* s, sf::Font* font = nullptr, unsigned int size = 12)
   : drawable_(new sf::Text())
   {
      this->set_string(s);
      this->set_font(font);
      this->set_character_size(size);
   }

   Text(std::string& s, sf::Font* font = nullptr, unsigned int size = 12)
   : drawable_(new sf::Text())
   {
      this->set_string(s);
      this->set_font(font);
      this->set_character_size(size);
   }

   virtual ~Text() {
      delete this->drawable_;
   }

   virtual void draw(Graphics& graphics, Layer& layer) {
      graphics.draw(*this->drawable_, layer);
   }

   // common sfml drawable interface
   virtual sf::FloatRect get_local_bounds() const { return this->drawable_->getLocalBounds(); }
   virtual sf::FloatRect get_global_bounds() const { return this->drawable_->getGlobalBounds(); }

   virtual void set_position(float x, float y) { this->drawable_->setPosition(x, y); }
   virtual void set_position(const sf::Vector2f& pos) { this->drawable_->setPosition(pos); }

   virtual void set_size(float x, float y) {}
   virtual void set_size(const sf::Vector2f& size) {}

   virtual void set_rotation(float angle) { this->drawable_->setRotation(angle); }
   
   virtual void set_scale(float factorX, float factorY) { this->drawable_->setScale(factorX, factorY); }
   virtual void set_scale(const sf::Vector2f& factors) { this->drawable_->setScale(factors); }

   virtual void set_origin(float x, float y) { this->drawable_->setOrigin(x, y); }
   virtual void set_origin(const sf::Vector2f& origin) { this->drawable_->setOrigin(origin); }

   virtual void set_color(const sf::Color& color) { this->drawable_->setColor(color); }
   
   virtual const sf::Vector2f& get_position() const { return this->drawable_->getPosition(); }

   virtual sf::Vector2f get_size() const {
      sf::FloatRect bounds = this->get_local_bounds();
      sf::Vector2f size(bounds.width, bounds.height);

      return size;
   }

   virtual float get_rotation() const { return this->drawable_->getRotation(); }
   
   virtual const sf::Vector2f& get_scale() const { return this->drawable_->getScale(); }
   
   virtual const sf::Vector2f& get_origin() const { return this->drawable_->getOrigin(); }

   virtual const sf::Color& get_color() const { return this->drawable_->getColor(); }

   virtual void move(float offsetX, float offsetY) { this->drawable_->move(offsetX, offsetY); }
   virtual void move(const sf::Vector2f& offset) { this->drawable_->move(offset); }

   virtual void rotate(float angle) { this->drawable_->rotate(angle); }

   virtual void scale(float factorX, float factorY) { this->drawable_->scale(factorX, factorY); }
   virtual void scale(const sf::Vector2f& factor) { this->drawable_->scale(factor); }

   // sprite/shape interface
   virtual const sf::Color& get_fill_color() const { return this->drawable_->getColor(); }

   // sf::Text interface
   virtual void set_string(const std::string& s) { this->drawable_->setString(s); }
   virtual void set_font(sf::Font* font) {
      if (!font) {
         return;
      }
      this->drawable_->setFont(*font);
   }
   virtual void set_character_size(unsigned int size) { this->drawable_->setCharacterSize(size); }
   virtual void set_style(sf::Text::Style style) { this->drawable_->setStyle(style); }

   virtual std::string get_string() { return this->drawable_->getString(); }
   virtual const sf::Font* get_font() { return this->drawable_->getFont(); }
   virtual sf::Vector2f find_character_pos(std::size_t index) const { return this->drawable_->findCharacterPos(index); }

protected:
   sf::Text* drawable_;
};

class Sprite : public Graphic
{
public:
   Sprite()
   : drawable_(new sf::Sprite())
   {
   }

   Sprite(Texture& t)
   : drawable_(new sf::Sprite(t.get_texture()))
   {
   }

   virtual ~Sprite() {
      delete this->drawable_;
   }

   // draw interface
   virtual void draw(Graphics& graphics, Layer& layer) {
      graphics.draw(*this->drawable_, layer);
   }

   // common sfml drawable interface
   virtual sf::FloatRect get_local_bounds() const { return this->drawable_->getLocalBounds(); }
   virtual sf::FloatRect get_global_bounds() const { return this->drawable_->getGlobalBounds(); }

   virtual void set_position(float x, float y) { this->drawable_->setPosition(x, y); }
   virtual void set_position(const sf::Vector2f& pos) { this->drawable_->setPosition(pos); }

   virtual void set_size(float x, float y) { this->set_size(sf::Vector2f(x, y)); }
   virtual void set_size(const sf::Vector2f& size) {
      sf::FloatRect bounds = this->get_local_bounds();
      sf::Vector2f old_size(bounds.width, bounds.height);

      sf::Vector2f new_scale(size.x / old_size.x, size.y / old_size.y);
      sf::Vector2f old_scale = this->get_scale();

      this->set_scale(old_scale.x, old_scale.y);
   }

   virtual void set_rotation(float angle) { this->drawable_->setRotation(angle); }
   
   virtual void set_scale(float factorX, float factorY) { this->drawable_->setScale(factorX, factorY); }
   virtual void set_scale(const sf::Vector2f& factors) { this->drawable_->setScale(factors); }

   virtual void set_origin(float x, float y) { this->drawable_->setOrigin(x, y); }
   virtual void set_origin(const sf::Vector2f& origin) { this->drawable_->setOrigin(origin); }

   virtual void set_color(const sf::Color& color) { this->drawable_->setColor(color); }
   
   virtual const sf::Vector2f& get_position() const { return this->drawable_->getPosition(); }

   virtual sf::Vector2f get_size() const {
      sf::FloatRect bounds = this->get_local_bounds();
      sf::Vector2f size(bounds.width, bounds.height);

      return size;
   }

   virtual float get_rotation() const { return this->drawable_->getRotation(); }
   
   virtual const sf::Vector2f& get_scale() const { return this->drawable_->getScale(); }
   
   virtual const sf::Vector2f& get_origin() const { return this->drawable_->getOrigin(); }

   virtual const sf::Color& get_color() const { return this->drawable_->getColor(); }

   virtual void move(float offsetX, float offsetY) { this->drawable_->move(offsetX, offsetY); }
   virtual void move(const sf::Vector2f& offset) { this->drawable_->move(offset); }

   virtual void rotate(float angle) { this->drawable_->rotate(angle); }

   virtual void scale(float factorX, float factorY) { this->drawable_->scale(factorX, factorY); }
   virtual void scale(const sf::Vector2f& factor) { this->drawable_->scale(factor); }

   // sprite/shape interface
   virtual void set_texture(Texture& texture) {
      this->texture_ = &texture;
      this->drawable_->setTexture(texture.get_texture());
   }

   virtual void set_fill_color(const sf::Color& color) {
      this->drawable_->setColor(color);
   }

   virtual Texture* get_texture() const { return this->texture_;} 
   virtual const sf::Color& get_fill_color() const { return this->drawable_->getColor(); }

protected:
   Texture* texture_;
   sf::Sprite* drawable_;
};

class Shape : public Graphic
{
public:
   Shape(sf::Shape* shape)
   : drawable_(shape)
   {
   }

   virtual ~Shape() {
      delete this->drawable_;
   }

   // draw interface
   virtual void draw(Graphics& graphics, Layer& layer) {
      graphics.draw(*this->drawable_, layer);
   }

   // common sfml drawable interface
   virtual sf::FloatRect get_local_bounds() const { return this->drawable_->getLocalBounds(); }
   virtual sf::FloatRect get_global_bounds() const { return this->drawable_->getGlobalBounds(); }

   virtual void set_position(float x, float y) { this->drawable_->setPosition(x, y); }
   virtual void set_position(const sf::Vector2f& pos) { this->drawable_->setPosition(pos); }

   virtual void set_size(float x, float y) { this->set_size(sf::Vector2f(x, y)); }
   virtual void set_size(const sf::Vector2f& size) {
      // ugh, hax
      sf::RectangleShape* rs = dynamic_cast<sf::RectangleShape*>(this->drawable_);

      if (rs) {
         rs->setSize(size);
      }
   }

   virtual void set_rotation(float angle) { this->drawable_->setRotation(angle); }
   
   virtual void set_scale(float factorX, float factorY) { this->drawable_->setScale(factorX, factorY); }
   virtual void set_scale(const sf::Vector2f& factors) { this->drawable_->setScale(factors); }

   virtual void set_origin(float x, float y) { this->drawable_->setOrigin(x, y); }
   virtual void set_origin(const sf::Vector2f& origin) { this->drawable_->setOrigin(origin); }

   virtual void set_color(const sf::Color& color) { this->drawable_->setFillColor(color); }
   
   virtual const sf::Vector2f& get_position() const { return this->drawable_->getPosition(); }

   virtual sf::Vector2f get_size() const {
      sf::FloatRect bounds = this->get_local_bounds();
      sf::Vector2f size(bounds.width, bounds.height);

      return size;
   }

   virtual float get_rotation() const { return this->drawable_->getRotation(); }
   
   virtual const sf::Vector2f& get_scale() const { return this->drawable_->getScale(); }
   
   virtual const sf::Vector2f& get_origin() const { return this->drawable_->getOrigin(); }

   virtual const sf::Color& get_color() const { return this->drawable_->getFillColor(); }

   virtual void move(float offsetX, float offsetY) { this->drawable_->move(offsetX, offsetY); }
   virtual void move(const sf::Vector2f& offset) { this->drawable_->move(offset); }

   virtual void rotate(float angle) { this->drawable_->rotate(angle); }

   virtual void scale(float factorX, float factorY) { this->drawable_->scale(factorX, factorY); }
   virtual void scale(const sf::Vector2f& factor) { this->drawable_->scale(factor); }

   // sprite/shape interface
   virtual void set_texture(Texture& texture) {
      this->texture_ = &texture;
      this->drawable_->setTexture(&texture.get_texture());
   }
   
   virtual void set_texture_rect(const sf::IntRect& rect) { this->drawable_->setTextureRect(rect); }
   virtual void set_fill_color(const sf::Color& color) { this->drawable_->setFillColor(color); }
   virtual void set_outline_color(const sf::Color& color) { this->drawable_->setOutlineColor(color); }
   virtual void set_outline_thickness(float thickness) { this->drawable_->setOutlineThickness(thickness); }

   virtual Texture* get_texture() const { return nullptr;} 
   virtual const sf::Color& get_fill_color() const { return sf::Color::Transparent; }
   virtual const sf::Color& get_outline_color() const { return sf::Color::Transparent; }
   virtual float get_outline_thickness() const { return 0; }

protected:
   Texture* texture_;
   sf::Shape* drawable_;
};

#endif
