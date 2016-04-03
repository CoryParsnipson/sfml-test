#include "TextWidget.h"
#include "TextFactory.h"

TextWidget::TextWidget(const std::string& text)
: Widget()
, text_(new Text(text, TextFactory::inst()->get_font("retro")))
{
   this->text_->set_color(sf::Color::Blue);
}

TextWidget::~TextWidget() {
   delete this->text_;
}

void TextWidget::set_text(const std::string& text) {
   this->text_->set_string(text);
}

const sf::Vector2f& TextWidget::get_position() {
   return this->text_->get_position();
}

void TextWidget::move(const sf::Vector2f& delta) {
   this->text_->move(delta);

   Widget::move(delta);
}

sf::Vector2f TextWidget::get_size() {
   return this->text_->get_size();
}

void TextWidget::set_size(const sf::Vector2f& size) {
   this->text_->set_size(size);
}

void TextWidget::draw(Graphics& graphics) {
   this->text_->draw(graphics);
   Widget::draw(graphics);
}
