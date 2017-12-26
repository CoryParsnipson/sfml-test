#ifndef CALLBACK_H
#define CALLBACK_H

#include <map>
#include <string>
#include <functional>

#include "sfml.h"

#include "Component.h"

// ----------------------------------------------------------------------------
// enum classes
// ----------------------------------------------------------------------------
enum class CallbackKey {
   MOUSE_IN, MOUSE_OUT, MOUSE_MOVE, MOUSE_WHEEL, LEFT_CLICK, RIGHT_CLICK,
   LEFT_RELEASE, RIGHT_RELEASE, CAMERA_RESIZE
};

// ----------------------------------------------------------------------------
// Callback
//
// This component keeps a list of function pointers specified by the ECS
// user to customize the behavior of entities when certain events occur.
// ----------------------------------------------------------------------------
class Callback : public Component {
public:
   using CallbackT = std::function<void()>;
   using Callbacks = std::map<CallbackKey, CallbackT>;

   explicit Callback(const std::string& id = "Callback Component");
   Callback(const Callback& other);
   virtual ~Callback();

   Callback& operator=(const Callback& other);
   void swap(Callback& other);

   float prev_mouse_wheel_pos() const;
   void prev_mouse_wheel_pos(float pos);

   sf::Vector2f prev_mouse_pos() const;
   void prev_mouse_pos(sf::Vector2f pos);

   void mouse_in(CallbackT callback);
   void mouse_in();

   void mouse_out(CallbackT callback);
   void mouse_out();

   void mouse_move(CallbackT callback);
   void mouse_move();

   void mouse_wheel(CallbackT callback);
   void mouse_wheel();

   void left_click(CallbackT callback);
   void left_click();

   void right_click(CallbackT callback);
   void right_click();

   void left_release(CallbackT callback);
   void left_release();

   void right_release(CallbackT callback);
   void right_release();

   void camera_resize(CallbackT callback);
   void camera_resize();

private:
   float prev_mouse_wheel_pos_;
   sf::Vector2f prev_mouse_pos_;
   Callbacks callbacks_;
};

#endif
