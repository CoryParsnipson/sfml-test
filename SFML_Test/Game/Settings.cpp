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

int Settings::cur_width() {
   return this->width_;
}

void Settings::cur_width(int new_width) {
   this->width_ = new_width;
}

int Settings::cur_height() {
   return this->height_;
}

void Settings::cur_height(int new_height) {
   this->height_ = new_height;
}
