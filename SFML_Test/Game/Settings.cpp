#include "dependencies.h"
#include "Settings.h"

// instantiate singleton instance
Settings* Settings::instance = NULL;

Settings* Settings::Instance() {
	if (!instance) {
		instance = new Settings();
	}
	return instance;
}

int Settings::cur_width(int new_width) {
   if (new_width > 0) {
      this->width_ = new_width;
   }
   return this->width_;
}

int Settings::cur_height(int new_height) {
   if (new_height > 0) {
      this->height_ = new_height;
   }
   return this->height_;
}
