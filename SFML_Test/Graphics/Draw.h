#ifndef DRAW_H
#define DRAW_H

#include "dependencies.h"
#include "Composite.h"

class Graphic;
class Texture;
class RenderSurface;

class Draw {
public:
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) = 0;
};

class Graphic
: public Composite<Graphic>
, public Draw
{
public:
   virtual ~Graphic() {}

   // drawable interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {
      Graphic::const_iterator it;
      for (it = this->children_.begin(); it != this->children_.end(); ++it) {
         (*it)->draw(surface, render_states);
      }
   }

   // common sfml drawable interface
   virtual sf::FloatRect get_local_bounds() const = 0;
   virtual sf::FloatRect get_global_bounds() const = 0;

   virtual void set_position(float x, float y) = 0;
   virtual void set_position(const sf::Vector2f& pos) = 0;

   virtual void set_size(float x, float y) = 0 ;
   virtual void set_size(const sf::Vector2f& size) = 0;

   virtual void set_rotation(float angle) = 0;

   virtual void set_scale(float factorX, float factorY) = 0;
   virtual void set_scale(const sf::Vector2f& factors) = 0;

   virtual void set_origin(float x, float y) = 0;
   virtual void set_origin(const sf::Vector2f& origin) = 0;

   virtual void set_color(const sf::Color& color) = 0;

   virtual const sf::Vector2f& get_position() = 0;
   virtual sf::Vector2f get_size() const = 0;
   virtual const sf::Vector2f& get_scale() const = 0;
   virtual const sf::Vector2f& get_origin() const = 0;
   virtual const sf::Color& get_color() const = 0;
   virtual float get_rotation() const = 0;

   virtual void move(float offsetX, float offsetY) = 0;
   virtual void move(const sf::Vector2f& offset) = 0;

   virtual void rotate(float angle) = 0;

   virtual void scale(float factorX, float factorY) = 0;
   virtual void scale(const sf::Vector2f& factor) = 0;

   virtual const sf::Transform& transform() const = 0;

   // sprite/shape interface
   virtual void set_texture(Texture& texture) = 0;
   virtual void set_texture_rect(const sf::IntRect& rect) = 0;
   virtual void set_outline_color(const sf::Color& color) = 0;
   virtual void set_outline_thickness(float thickness) = 0;

   virtual Texture* get_texture() const = 0;
   virtual const sf::IntRect& get_texture_rect() const = 0;
   virtual const sf::Color& get_outline_color() const = 0;
   virtual float get_outline_thickness() const = 0;

   // sf::Text interface
   virtual void set_string(const std::string& s) = 0;
   virtual void set_font(sf::Font* font) = 0;
   virtual void set_character_size(unsigned int size) = 0;
   virtual void set_style(sf::Text::Style style) = 0;

   virtual std::string get_string() = 0;
   virtual const sf::Font* get_font() = 0;
   virtual unsigned int get_character_size() const = 0;
   virtual unsigned int get_em_width() const = 0;
   virtual sf::Vector2f find_character_pos(std::size_t index) const = 0;
};

class SpriteGraphic : public Graphic {
public:
   SpriteGraphic();
   SpriteGraphic(Texture& texture);
   virtual ~SpriteGraphic();

   // drawable interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // common sfml drawable interface
   virtual sf::FloatRect get_local_bounds() const;
   virtual sf::FloatRect get_global_bounds() const;

   virtual void set_position(float x, float y);
   virtual void set_position(const sf::Vector2f& pos);

   virtual void set_size(float x, float y);
   virtual void set_size(const sf::Vector2f& size);

   virtual void set_rotation(float angle);

   virtual void set_scale(float factorX, float factorY);
   virtual void set_scale(const sf::Vector2f& factors);

   virtual void set_origin(float x, float y);
   virtual void set_origin(const sf::Vector2f& origin);

   virtual void set_color(const sf::Color& color);

   virtual const sf::Vector2f& get_position();
   virtual sf::Vector2f get_size() const;
   virtual const sf::Vector2f& get_scale() const;
   virtual const sf::Vector2f& get_origin() const;
   virtual const sf::Color& get_color() const;
   virtual float get_rotation() const;

   virtual void move(float offsetX, float offsetY);
   virtual void move(const sf::Vector2f& offset);

   virtual void rotate(float angle);

   virtual void scale(float factorX, float factorY);
   virtual void scale(const sf::Vector2f& factor);

   virtual const sf::Transform& transform() const;
   
   // sprite/shape interface
   virtual void set_texture(Texture& texture);
   virtual void set_texture_rect(const sf::IntRect& rect);
   virtual void set_outline_color(const sf::Color& color);
   virtual void set_outline_thickness(float thickness);

   virtual Texture* get_texture() const;
   virtual const sf::IntRect& get_texture_rect() const;
   virtual const sf::Color& get_outline_color() const;
   virtual float get_outline_thickness() const;

   // sf::Text interface
   virtual void set_string(const std::string& s);
   virtual void set_font(sf::Font* font);
   virtual void set_character_size(unsigned int size);
   virtual void set_style(sf::Text::Style style);

   virtual std::string get_string();
   virtual const sf::Font* get_font();
   virtual unsigned int get_character_size() const;
   virtual unsigned int get_em_width() const;
   virtual sf::Vector2f find_character_pos(std::size_t index) const;

protected:
   Texture* texture_;
   sf::RectangleShape* drawable_;
};

class TextGraphic : public Graphic {
public:
   TextGraphic();
   TextGraphic(const char* text, sf::Font* font = nullptr, unsigned int size = 12);
   TextGraphic(const std::string& text, sf::Font* font = nullptr, unsigned int size = 12);
   virtual ~TextGraphic();

   // drawable interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // common sfml drawable interface
   virtual sf::FloatRect get_local_bounds() const;
   virtual sf::FloatRect get_global_bounds() const;

   virtual void set_position(float x, float y);
   virtual void set_position(const sf::Vector2f& pos);

   virtual void set_size(float x, float y);
   virtual void set_size(const sf::Vector2f& size);

   virtual void set_rotation(float angle);

   virtual void set_scale(float factorX, float factorY);
   virtual void set_scale(const sf::Vector2f& factors);

   virtual void set_origin(float x, float y);
   virtual void set_origin(const sf::Vector2f& origin);

   virtual void set_color(const sf::Color& color);

   virtual const sf::Vector2f& get_position();
   virtual sf::Vector2f get_size() const;
   virtual const sf::Vector2f& get_scale() const;
   virtual const sf::Vector2f& get_origin() const;
   virtual const sf::Color& get_color() const;
   virtual float get_rotation() const;

   virtual void move(float offsetX, float offsetY);
   virtual void move(const sf::Vector2f& offset);

   virtual void rotate(float angle);

   virtual void scale(float factorX, float factorY);
   virtual void scale(const sf::Vector2f& factor);

   virtual const sf::Transform& transform() const;
   
   // sprite/shape interface
   virtual void set_texture(Texture& texture);
   virtual void set_texture_rect(const sf::IntRect& rect);
   virtual void set_outline_color(const sf::Color& color);
   virtual void set_outline_thickness(float thickness);

   virtual Texture* get_texture() const;
   virtual const sf::IntRect& get_texture_rect() const;
   virtual const sf::Color& get_outline_color() const;
   virtual float get_outline_thickness() const;

   // sf::Text interface
   virtual void set_string(const std::string& s);
   virtual void set_font(sf::Font* font);
   virtual void set_character_size(unsigned int size);
   virtual void set_style(sf::Text::Style style);

   virtual std::string get_string();
   virtual const sf::Font* get_font();
   virtual unsigned int get_character_size() const;
   virtual unsigned int get_em_width() const;
   virtual sf::Vector2f find_character_pos(std::size_t index) const;

protected:
   sf::IntRect null_rect_;
   sf::Vector2f bounds_pos_;
   sf::Vector2f size_;

   sf::Text* em_;
   sf::Text* drawable_;
};

// TODO: if there is a way to refactor graphics hierarchy so that this disappears, please do it
class VertexGraphic : public Graphic {
public:
   VertexGraphic(sf::VertexArray* arr);
   virtual ~VertexGraphic();

   // drawable interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // common sfml drawable interface
   virtual sf::FloatRect get_local_bounds() const;
   virtual sf::FloatRect get_global_bounds() const;

   virtual void set_position(float x, float y) {}
   virtual void set_position(const sf::Vector2f& pos) {}

   virtual void set_size(float x, float y);
   virtual void set_size(const sf::Vector2f& size);

   virtual void set_rotation(float angle) {}

   virtual void set_scale(float factorX, float factorY) {}
   virtual void set_scale(const sf::Vector2f& factors) {}

   virtual void set_origin(float x, float y) {}
   virtual void set_origin(const sf::Vector2f& origin) {}

   virtual void set_color(const sf::Color& color) {}

   virtual const sf::Vector2f& get_position();
   virtual sf::Vector2f get_size() const;
   virtual const sf::Vector2f& get_scale() const;
   virtual const sf::Vector2f& get_origin() const;
   virtual const sf::Color& get_color() const;
   virtual float get_rotation() const;

   virtual void move(float offsetX, float offsetY);
   virtual void move(const sf::Vector2f& offset);

   virtual void rotate(float angle);

   virtual void scale(float factorX, float factorY);
   virtual void scale(const sf::Vector2f& factor);

   virtual const sf::Transform& transform() const;
   
   // sprite/shape interface
   virtual void set_texture(Texture& texture) {}
   virtual void set_texture_rect(const sf::IntRect& rect) {}
   virtual void set_outline_color(const sf::Color& color) {}
   virtual void set_outline_thickness(float thickness) {}

   virtual Texture* get_texture() const;
   virtual const sf::IntRect& get_texture_rect() const;
   virtual const sf::Color& get_outline_color() const;
   virtual float get_outline_thickness() const;

   // sf::Text interface
   virtual void set_string(const std::string& s) {}
   virtual void set_font(sf::Font* font) {}
   virtual void set_character_size(unsigned int size) {}
   virtual void set_style(sf::Text::Style style) {}

   virtual std::string get_string();
   virtual const sf::Font* get_font();
   virtual unsigned int get_character_size() const;
   virtual unsigned int get_em_width() const;
   virtual sf::Vector2f find_character_pos(std::size_t index) const;

protected:
   sf::RenderStates state_;
   sf::Vector2f pos_;
   sf::Vector2f size_;
   sf::Vector2f scale_;
   sf::IntRect nullrect_;
   sf::VertexArray* drawable_;
};

#endif
