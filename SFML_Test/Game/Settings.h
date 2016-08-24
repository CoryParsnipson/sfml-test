#ifndef SETTINGS_H
#define SETTINGS_H

class Settings {
public:
	static Settings* Instance();

	const int FRAMERATE_LIMIT = 60;
   const float FRAMERATE_SMOOTHING = 0.98;

	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 500;

	const float SCALE_X = 1; // factor
	const float SCALE_Y = 1; // factor

	const float TILE_WIDTH = SCALE_X * 40; // px
	const float TILE_HEIGHT = SCALE_Y * 40; // px
	const float TILE_HEIGHT_RHOMBUS = SCALE_Y * 20; // px
	const float TILE_HEIGHT_OVERLAP = TILE_HEIGHT_RHOMBUS / 2;

   int cur_width();
   void cur_width(int new_width);

   int cur_height();
   void cur_height(int new_height);

protected:
   int width_;
   int height_;

private:
	Settings()
   : width_(SCREEN_WIDTH)
   , height_(SCREEN_HEIGHT)
   {};
	Settings(const Settings& g) {}; // seal off copy constructor

	static Settings* instance;
};

#endif
