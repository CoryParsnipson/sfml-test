#ifndef SETTINGS_H
#define SETTINGS_H

// ----------------------------------------------------------------------------
// Settings
//
// This is an object that contains global settings and information
// for a game object.
// ----------------------------------------------------------------------------
class Settings {
public:
   // TODO: serialize and deserialize
   Settings();
   
   int framerate_limit();
   void framerate_limit(int framerate_limit);

   float framerate_smoothing();
   void framerate_smoothing(float framerate_smoothing);

   int default_window_width();
   void default_window_width(int default_window_width);

   int default_window_height();
   void default_window_height(int default_window_height);

private:
   int framerate_limit_;
   float framerate_smoothing_;

   int default_window_width_;
   int default_window_height_;
};

#endif
