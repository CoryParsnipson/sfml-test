#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/tokenizer.hpp>

#include "TextWidget.h"
#include "TextFactory.h"

TextWidget::TextWidget(const std::string& text)
: Widget()
, raw_text_(text)
, text_(new Text(text, TextFactory::inst()->get_font("retro")))
{
   this->text_->set_color(sf::Color::Blue);

   this->text_size_.x = this->text_->get_local_bounds().width;
   this->text_size_.y = this->text_->get_local_bounds().height;
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

void TextWidget::move(const sf::Vector2f& delta) {
   this->text_->move(delta);

   Widget::move(delta);
}

sf::Vector2f TextWidget::get_size() {
   return this->text_size_;
}

void TextWidget::set_size(const sf::Vector2f& size) {
   this->text_size_ = size;
   this->word_wrap();
}

void TextWidget::draw(RenderTarget& surface) {
   this->text_->draw(surface);
   Widget::draw(surface);
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
