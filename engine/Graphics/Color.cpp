#include "Color.h"

Color::Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a /* = 255 */)
: color_(r, g, b, a)
{
}

Color::Color(const sf::Color& color)
: color_(color)
{
}

Color::Color(const Color& other)
: color_(other.color_)
{
}

Color::~Color() {
}

Color& Color::operator=(const Color& other) {
   Color tmp(other);
   this->swap(tmp);
   return *this;
}

void Color::swap(Color& other) {
   std::swap(this->color_, other.color_);
}

Color::operator sf::Color() const {
   return this->color_;
}

bool Color::operator==(const Color& other) {
   return (this->color_ == other.color_);
}

bool Color::operator!=(const Color& other) {
   return (this->color_ != other.color_);
}

Color Color::operator+(const Color& other) {
   return Color(this->color_ + other.color_);
}

Color Color::operator*(const Color& other) {
   return Color(this->color_ * other.color_);
}

Color& Color::operator+=(const Color& other) {
   this->color_ += other.color_;
   return *this;
}

Color& Color::operator*=(const Color& other) {
   this->color_ *= other.color_;
   return *this;
}

std::uint8_t Color::r() const {
   return static_cast<std::uint8_t>(this->color_.r);
}

std::uint8_t Color::g() const {
   return static_cast<std::uint8_t>(this->color_.g);
}

std::uint8_t Color::b() const {
   return static_cast<std::uint8_t>(this->color_.b);
}

std::uint8_t Color::a() const {
   return static_cast<std::uint8_t>(this->color_.a);
}

void Color::r(std::uint8_t color_component) {
   this->color_.r = color_component;
}

void Color::g(std::uint8_t color_component) {
   this->color_.g = color_component;
}

void Color::b(std::uint8_t color_component) {
   this->color_.b = color_component;
}

void Color::a(std::uint8_t color_component) {
   this->color_.a = color_component;
}

std::string Color::serialize(Serializer& s) {
   Serializer::SerialData data;

   data["r"] = std::to_string(this->r());
   data["g"] = std::to_string(this->g());
   data["b"] = std::to_string(this->b());
   data["a"] = std::to_string(this->a());

   return s.serialize(data);
}

void Color::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   this->r(std::stoi(data["r"]));
   this->g(std::stoi(data["r"]));
   this->b(std::stoi(data["r"]));
   this->a(std::stoi(data["r"]));
}
