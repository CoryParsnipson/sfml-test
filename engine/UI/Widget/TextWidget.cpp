#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/tokenizer.hpp>

#include "RenderSurface.h"
#include "TextWidget.h"

TextWidget::TextWidget(std::string id, const std::string& text, sf::Font* font /* = nullptr */)
: Widget(id)
, raw_text_(text)
, text_(new TextGraphic(text, font))
{
   this->text_->set_color(sf::Color::Blue);

   this->size_.x = this->text_->get_local_bounds().width;
   this->size_.y = this->text_->get_local_bounds().height;
}

TextWidget::~TextWidget() {
   delete this->text_;
}

void TextWidget::set_text(const std::string& text) {
   this->raw_text_ = text;

   this->text_->set_string(text);
   this->word_wrap();
}

const sf::Vector2f& TextWidget::get_position() {
   return this->text_->get_position();
}

void TextWidget::set_position(const sf::Vector2f& pos) {
   this->text_->set_position(pos);
}

sf::Vector2f TextWidget::get_size() {
   return this->size_;
}

void TextWidget::set_size(const sf::Vector2f& size) {
   this->size_ = size;
   this->word_wrap();
}

bool TextWidget::intersects(const sf::Vector2i& other) {
   return this->intersects(static_cast<sf::Vector2f>(other));
}

bool TextWidget::intersects(const sf::Vector2f& other) {
   sf::Transform transform = this->global_transform();
   return transform.transformRect(this->text_->get_local_bounds()).contains(other);
}

bool TextWidget::intersects(const sf::FloatRect& other) {
   sf::Transform transform = this->global_transform();
   return transform.transformRect(this->text_->get_local_bounds()).intersects(other);
}

bool TextWidget::intersects(const SceneObject& other) {
   // TODO: implement me
   return false;
}

void TextWidget::word_wrap() {
   std::string text;
   unsigned int line_length = 0;
   unsigned int chars_per_line = this->get_size().x / this->text_->get_em_width();

   boost::char_separator<char> whitespace(" \n\r\t");
   boost::tokenizer<boost::char_separator<char> > tokenizer(this->raw_text_, whitespace);

   for (boost::tokenizer<boost::char_separator<char> >::iterator it = tokenizer.begin(); it != tokenizer.end(); ++it) {
      if (line_length + it->length() + 1 > chars_per_line) {
         line_length = 0;
         text += "\n";
      }

      line_length += it->length() + 1;
      text += *it + " ";
   }

   this->text_->set_string(text);
}

void TextWidget::do_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->text_->draw(surface, render_states);
}
