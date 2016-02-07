#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "dependencies.h"

#include "Draw.h"

class Viewport
{
public:
   typedef std::vector<Layer*> LayerList;
   
   Viewport(sf::Vector2f size);
   virtual ~Viewport();

   // viewport management
   void set_viewport(const sf::FloatRect& viewport);
   sf::FloatRect get_viewport();

   // layer management interface
   LayerList layers();
   Layer* layer(std::string id); // get layer

   Layer* add(std::string id);
   void insert(std::string id, int idx);
   void move(std::string id, int idx);
   void remove(std::string id);

   void resize(sf::Vector2f size);
   void recenter(sf::Vector2f center);

   void reset();

   // draw interface
   virtual void draw(Graphics& graphics, Layer& layer) { this->draw(graphics); }
   virtual void draw(Graphics& graphics);

protected:
   LayerList layers_;
   sf::Vector2f size_;
};

#endif
