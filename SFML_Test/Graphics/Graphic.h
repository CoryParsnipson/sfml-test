#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "dependencies.h"
#include "Draw.h"
#include "TextFactory.h"
#include "Texture.h"

class Graphic
: public Draw
{
public:
   typedef std::vector<Graphic*> GraphicList;

   Graphic() {}
   virtual ~Graphic() {}

   // draw interface
   virtual void draw(RenderTarget& surface) = 0;

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

   virtual const sf::Vector2f& get_position() = 0;

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
   virtual const sf::IntRect& get_texture_rect() const { return this->null_rect_; }
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
   virtual unsigned int get_character_size() const { return 0; }
   virtual unsigned int get_em_width() const { return 0; }
   virtual sf::Vector2f find_character_pos(std::size_t index) const { return sf::Vector2f(0, 0); }

protected:
   sf::IntRect null_rect_;
};

class CompositeGraphic : public Graphic {
public:
   CompositeGraphic(Graphic::GraphicList* graphics)
   : local_bounds_(new sf::FloatRect())
   , global_bounds_(new sf::FloatRect())
   , default_color_(sf::Color::Transparent)
   , default_scale_(1.0, 1.0)
   {
      this->children_ = graphics;

      // calculate local and global bounds
      Graphic::GraphicList::const_iterator it;
      for (it = this->children_->begin(); it != this->children_->end(); ++it) {
         sf::FloatRect child_local_bounds = (*it)->get_local_bounds();
         sf::FloatRect child_global_bounds = (*it)->get_global_bounds();

         if (!this->local_bounds_) {
            this->local_bounds_->left = child_local_bounds.left;
            this->local_bounds_->top = child_local_bounds.top;
            this->local_bounds_->width = child_local_bounds.width;
            this->local_bounds_->height = child_local_bounds.height;
         } else {
            this->local_bounds_->left = std::min(this->local_bounds_->left, child_local_bounds.left);
            this->local_bounds_->top = std::min(this->local_bounds_->top, child_local_bounds.top);

            if ((this->local_bounds_->left + this->local_bounds_->width) < (child_local_bounds.left + child_local_bounds.width)) {
               this->local_bounds_->width = (child_local_bounds.left + child_local_bounds.width) - this->local_bounds_->left;
            }

            if ((this->local_bounds_->top + this->local_bounds_->height) < (child_local_bounds.top + child_local_bounds.height)) {
               this->local_bounds_->width = (child_local_bounds.top + child_local_bounds.height) - this->local_bounds_->top;
            }
         }

         if (!this->global_bounds_) {
            this->global_bounds_->left = child_global_bounds.left;
            this->global_bounds_->top = child_global_bounds.top;
            this->global_bounds_->width = child_global_bounds.width;
            this->global_bounds_->height = child_global_bounds.height;
         } else {
            this->global_bounds_->left = std::min(this->global_bounds_->left, child_global_bounds.left);
            this->global_bounds_->top = std::min(this->global_bounds_->top, child_global_bounds.top);

            if ((this->global_bounds_->left + this->global_bounds_->width) < (child_global_bounds.left + child_global_bounds.width)) {
               this->global_bounds_->width = (child_global_bounds.left + child_global_bounds.width) - this->global_bounds_->left;
            }

            if ((this->global_bounds_->top + this->global_bounds_->height) < (child_global_bounds.top + child_global_bounds.height)) {
               this->global_bounds_->height = (child_global_bounds.top + child_global_bounds.height) - this->global_bounds_->top;
            }
         }
      }

      this->pos_.x = this->global_bounds_->left;
      this->pos_.y = this->global_bounds_->top;

      this->size_.x = this->global_bounds_->width;
      this->size_.y = this->global_bounds_->height;

      Service::get_logger().msg("CompositeGraphic", Logger::INFO, "pos (" + std::to_string((int)this->pos_.x) + ", " + std::to_string((int)this->pos_.y) + ")");
      Service::get_logger().msg("CompositeGraphic", Logger::INFO, "size (" + std::to_string((int)this->size_.x) + ", " + std::to_string((int)this->size_.y) + ")");
   }
   virtual ~CompositeGraphic() {
      Graphic::GraphicList::const_iterator it;
      for (it = this->children_->begin(); it != this->children_->end(); ++it) {
         delete (*it);
      }
      this->children_->clear();

      delete this->local_bounds_;
      delete this->global_bounds_;

      delete this->children_;
      this->children_ = nullptr;
   }

   // draw interface
   virtual void draw(RenderTarget& surface) {
      Graphic::GraphicList::const_iterator it;
      for (it = this->children_->begin(); it != this->children_->end(); ++it) {
         (*it)->draw(surface);
      }
   }

   // common sfml drawable interface
   virtual sf::FloatRect get_local_bounds() const {
      return *this->local_bounds_;
   }
   virtual sf::FloatRect get_global_bounds() const {
      return *this->global_bounds_;
   }

   virtual void set_position(float x, float y) {
      this->pos_.x = x;
      this->pos_.y = y;

      this->global_bounds_->left = x;
      this->global_bounds_->top = y;

      Graphic::GraphicList::const_iterator it;
      for (it = this->children_->begin(); it != this->children_->end(); ++it) {
         (*it)->set_position(x, y);
      }
   }
   virtual void set_position(const sf::Vector2f& pos) { this->set_position(pos.x, pos.y); }

   virtual void set_size(float x, float y) {}
   virtual void set_size(const sf::Vector2f& size) {}

   virtual void set_rotation(float angle) {
      Graphic::GraphicList::const_iterator it;
      for (it = this->children_->begin(); it != this->children_->end(); ++it) {
         (*it)->set_rotation(angle);
      }
   }

   virtual void set_scale(float factorX, float factorY) {
      Graphic::GraphicList::const_iterator it;
      for (it = this->children_->begin(); it != this->children_->end(); ++it) {
         (*it)->set_scale(factorX, factorY);
      }
   }
   virtual void set_scale(const sf::Vector2f& factors) { this->set_scale(factors.x, factors.y); }

   virtual void set_origin(float x, float y) {}
   virtual void set_origin(const sf::Vector2f& origin) {}

   virtual void set_color(const sf::Color& color) {
      Graphic::GraphicList::const_iterator it;
      for (it = this->children_->begin(); it != this->children_->end(); ++it) {
         (*it)->set_color(color);
      }
   }

   virtual const sf::Vector2f& get_position() {
      return this->pos_;
   }

   virtual sf::Vector2f get_size() const {
      return this->size_;
   }
   virtual float get_rotation() const {
      float rotation = 0;
      if (this->children_->size() > 0) {
         rotation = this->children_->front()->get_rotation();
      }
      return rotation;
   }

   virtual const sf::Vector2f& get_scale() const {
      if (this->children_->size() > 0) {
         return this->children_->front()->get_scale();
      }
      return this->default_scale_;
   }

   virtual const sf::Vector2f& get_origin() const {
      return this->pos_;
   }

   virtual const sf::Color& get_color() const {
      if (this->children_->size() > 0) {
         return this->children_->front()->get_color();
      }
      return this->default_color_;
   }

   virtual void move(float offsetX, float offsetY) {
      Graphic::GraphicList::const_iterator it;
      for (it = this->children_->begin(); it != this->children_->end(); ++it) {
         (*it)->move(offsetX, offsetY);
      }

      this->global_bounds_->left += offsetX;
      this->global_bounds_->top += offsetY;
   }
   virtual void move(const sf::Vector2f& offset) { return this->move(offset.x, offset.y); }

   virtual void rotate(float angle) {
      Graphic::GraphicList::const_iterator it;
      for (it = this->children_->begin(); it != this->children_->end(); ++it) {
         (*it)->rotate(angle);
      }
   }

   virtual void scale(float factorX, float factorY) {
      Graphic::GraphicList::const_iterator it;
      for (it = this->children_->begin(); it != this->children_->end(); ++it) {
         (*it)->scale(factorX, factorY);
      }
   }
   virtual void scale(const sf::Vector2f& factor) { this->scale(factor.x, factor.y); }

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

protected:
   sf::Vector2f pos_;
   sf::Vector2f size_;
   sf::FloatRect* local_bounds_;
   sf::FloatRect* global_bounds_;

   sf::Color default_color_;
   sf::Vector2f default_scale_;

   Graphic::GraphicList* children_;
};

class Text : public Graphic
{
public:
   Text()
   : em_(new sf::Text())
   , drawable_(new sf::Text())
   {
   }

   Text(const char* s, sf::Font* font = nullptr, unsigned int size = 12)
   : em_(new sf::Text())
   , drawable_(new sf::Text())
   {
      this->em_->setString("m");

      this->set_string(s);
      this->set_font(font);
      this->set_character_size(size);
   }

   Text(const std::string& s, sf::Font* font = nullptr, unsigned int size = 12)
   : em_(new sf::Text())
   , drawable_(new sf::Text())
   {
      this->em_->setString("m");

      this->set_string(s);
      this->set_font(font);
      this->set_character_size(size);
   }

   virtual ~Text() {
      delete this->em_;
      delete this->drawable_;
   }

   virtual void draw(RenderTarget& surface) {
      surface.draw(*this->drawable_);
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

   virtual const sf::Vector2f& get_position() {
      sf::FloatRect bounds = this->get_global_bounds();

      this->bounds_pos_.x = bounds.left;
      this->bounds_pos_.y = bounds.top;

      return this->bounds_pos_;
   }

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
      this->em_->setFont(*font);
      this->drawable_->setFont(*font);
   }
   virtual void set_character_size(unsigned int size) {
      this->em_->setCharacterSize(size);
      this->drawable_->setCharacterSize(size);
   }
   virtual void set_style(sf::Text::Style style) {
      this->em_->setStyle(style);
      this->drawable_->setStyle(style);
   }

   virtual std::string get_string() { return this->drawable_->getString(); }
   virtual const sf::Font* get_font() { return this->drawable_->getFont(); }
   virtual unsigned int get_character_size() const { return this->drawable_->getCharacterSize(); }
   virtual unsigned int get_em_width() const { return this->em_->getLocalBounds().width; }
   virtual sf::Vector2f find_character_pos(std::size_t index) const { return this->drawable_->findCharacterPos(index); }

protected:
   sf::Vector2f bounds_pos_;
   sf::Text* em_;
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
   : texture_(&t)
   , drawable_(new sf::Sprite(t.get_texture()))
   {
   }

   virtual ~Sprite() {
      delete this->drawable_;
   }

   // draw interface
   virtual void draw(RenderTarget& surface) {
      surface.draw(*this->drawable_);
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

   virtual const sf::Vector2f& get_position() { return this->drawable_->getPosition(); }

   virtual sf::Vector2f get_size() const {
      sf::FloatRect bounds = this->get_local_bounds();
      sf::Vector2f scale = this->get_scale();
      sf::Vector2f size(scale.x * bounds.width, scale.y * bounds.height);

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
   virtual void set_texture_rect(const sf::IntRect& rect) {
      this->drawable_->setTextureRect(rect);
   }

   virtual void set_texture(Texture& texture) {
      this->texture_ = &texture;
      this->drawable_->setTexture(texture.get_texture());
   }

   virtual void set_fill_color(const sf::Color& color) {
      this->drawable_->setColor(color);
   }

   virtual Texture* get_texture() const { return this->texture_;}
   virtual const sf::IntRect& get_texture_rect() const { return this->drawable_->getTextureRect(); }
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
   virtual void draw(RenderTarget& surface) {
      surface.draw(*this->drawable_);
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

   virtual const sf::Vector2f& get_position() { return this->drawable_->getPosition(); }

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
   virtual void set_outline_thickness(float thickness) {
      // negative number makes outline on inside edge instead of outside edge
      this->drawable_->setOutlineThickness(-1 * thickness);
   }

   virtual Texture* get_texture() const { return nullptr;}
   virtual const sf::Color& get_fill_color() const { return sf::Color::Transparent; }
   virtual const sf::Color& get_outline_color() const { return sf::Color::Transparent; }
   virtual float get_outline_thickness() const { return 0; }

protected:
   Texture* texture_;
   sf::Shape* drawable_;
};

#endif
