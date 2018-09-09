#include <iostream>
#include <algorithm>

#include "Settings.h"

Settings::Settings()
: framerate_limit_(60)
, framerate_smoothing_(0.98)
, default_window_width_(800)
, default_window_height_(600)
{
}

int Settings::framerate_limit() {
   return this->framerate_limit_;
}

void Settings::framerate_limit(int framerate_limit) {
   this->framerate_limit_ = std::max(framerate_limit, 0);
}

float Settings::framerate_smoothing() {
   return this->framerate_smoothing_;
}

void Settings::framerate_smoothing(float framerate_smoothing) {
   framerate_smoothing = std::min(framerate_smoothing, 1.f);
   framerate_smoothing = std::max(framerate_smoothing, 0.f);

   this->framerate_smoothing_ = framerate_smoothing;
}

int Settings::default_window_width() {
   return this->default_window_width_;
}

void Settings::default_window_width(int default_window_width) {
   this->default_window_width_ = std::max(default_window_width, 1);
}

int Settings::default_window_height() {
   return this->default_window_height_;
}

void Settings::default_window_height(int default_window_height) {
   this->default_window_height_ = std::max(default_window_height, 1);
}
