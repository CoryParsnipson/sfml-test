#ifndef COLOR_H
#define COLOR_H

#include <SFML/Graphics.hpp>

#include "Serializable.h"

// ----------------------------------------------------------------------------
// Color
//
// This is a wrapper class to describe colors. It contains 8 bits each for
// red, green, blue, and alpha.
// ----------------------------------------------------------------------------
class Color final : public Serializable {
public:
   Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
   explicit Color(const sf::Color& color);
   Color(const Color& other);
   ~Color();

   Color& operator=(const Color& other);
   void swap(Color& other);

   operator sf::Color() const;

   bool operator==(const Color& other);
   bool operator!=(const Color& other);
   Color operator+(const Color& other);
   Color operator*(const Color& other);
   Color& operator+=(const Color& other);
   Color& operator*=(const Color& other);

   std::uint8_t r() const;
   std::uint8_t g() const;
   std::uint8_t b() const;
   std::uint8_t a() const;

   void r(std::uint8_t color_component);
   void g(std::uint8_t color_component);
   void b(std::uint8_t color_component);
   void a(std::uint8_t color_component);

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

private:
   sf::Color color_;
};

#endif
