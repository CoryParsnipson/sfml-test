#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

#include <SFML/Graphics.hpp>

class Global {
public:
	static Global* Instance();

	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 500;

	const int SCALE_X = 2; // factor
	const int SCALE_Y = 2; // factor

	const int TILE_WIDTH = SCALE_X * 40; // px
	const int TILE_HEIGHT = SCALE_Y * 40; // px
	const int TILE_HEIGHT_RHOMBUS = SCALE_Y * 20; // px
	const int TILE_HEIGHT_OVERLAP = TILE_HEIGHT_RHOMBUS / 2;

private:
	Global() {};
	Global(const Global& g) {}; // seal off copy constructor

	static Global* instance;
};

Global* Global::instance = NULL;

Global* Global::Instance() {
	if (!instance) {
		instance = new Global();
	}
	return instance;
}

template <class T> class Coordinate {
public:
	Coordinate();
	Coordinate(T x, T y);
	Coordinate(Coordinate& c);
	~Coordinate();

	T getX() const;
	T getY() const;

	double magnitude() const;

	// TODO: overload operators +, -, *, /

protected:
	T x;
	T y;
};

template <class T>
Coordinate<T>::Coordinate(T x, T y) :
x(x),
y(y)
{
}

template <class T>
Coordinate<T>::~Coordinate() {
}

template <class T>
T Coordinate<T>::getX() const {
	return this->x;
}

template <class T>
T Coordinate<T>::getY() const {
	return this->y;
}

template <class T>
double Coordinate<T>::magnitude() const {
	return std::sqrt(std::pow(this->x, 2) + std::pow(this->y, 2));
}

template <class T>
bool operator<(const Coordinate<T>& l, const Coordinate<T>& r) {
	return (l.magnitude() < r.magnitude());
}

template <class T>
bool operator==(const Coordinate<T>& l, const Coordinate<T>& r) {
	return (l.getX() == r.getX() && l.getY() == r.getY());
}

class ViewportCoordinate : public Coordinate < int > {
public:
	ViewportCoordinate(int x, int y);

	std::string to_string() const;
};

ViewportCoordinate::ViewportCoordinate(int x, int y) :
Coordinate(x, y)
{
}

std::string ViewportCoordinate::to_string() const {
	std::stringstream output;
	output << "Screen Coord. (" << this->x << ", " << this->y << ")";
	return output.str();
}

class MapCoordinate : public Coordinate<int> {
public:
	MapCoordinate(int x, int y);
	MapCoordinate(const MapCoordinate& other);

	ViewportCoordinate to_screen_coordinates();
	std::string to_string() const;
};

MapCoordinate::MapCoordinate(int x, int y)
: Coordinate<int>(x, y)
{
}

MapCoordinate::MapCoordinate(const MapCoordinate& other) :
Coordinate(other.getX(), other.getY())
{
}

ViewportCoordinate MapCoordinate::to_screen_coordinates() {
	// calculate x screen coordinate
	int screen_x = this->getX() * Global::Instance()->TILE_WIDTH + (this->getY() & 1) * Global::Instance()->TILE_WIDTH / 2;
	int screen_y = this->getY() * Global::Instance()->TILE_HEIGHT_OVERLAP - (Global::Instance()->TILE_HEIGHT - Global::Instance()->TILE_HEIGHT_RHOMBUS);
	return ViewportCoordinate(screen_x, screen_y);
}

std::string MapCoordinate::to_string() const {
	std::stringstream output;
	output << "Map Coord. (" << this->x << ", " << this->y << ")";
	return output.str();
}

bool operator<(const MapCoordinate& l, const MapCoordinate& r) {
	// a natural way for comparison would be to compare magnitudes, but it is common for two completely different 2d vectors
	// to have equal magnitude, causing < to return false. This is problematic to use as a map key...

	//return (l.magnitude() < r.magnitude());
	return (l.getX() < r.getX() || (l.getX() == r.getX() && l.getY() < r.getY()));
}

bool operator==(const MapCoordinate& l, const MapCoordinate& r) {
	return (l.getX() == r.getX() && l.getY() == r.getY());
}

class MapFileReader {
public:
	MapFileReader(std::string filename);
	~MapFileReader();
	
	bool readline(std::string* line);
	void restart_feed();

	void tokenize(std::vector<int>* tokens, std::string input_string, char delimiter = ',');

protected:
	std::ifstream* map_file;
};

MapFileReader::MapFileReader(std::string filename) :
map_file(new std::ifstream(filename))
{
}

MapFileReader::~MapFileReader() {
	this->map_file->close();
	delete this->map_file;
}

bool MapFileReader::readline(std::string* line) {
	if (this->map_file->eof()) {
		return false;
	}
	
	*line = "";
	while (line->empty()) {
		std::getline(*(this->map_file), *line);

		// omit comments
		if (line->find_first_of('#') != std::string::npos) {
			*line = line->substr(0, line->find_first_of('#'));
		}

		// trim leading whitespace
		while (line->length() && (*line)[0] == ' ') {
			*line = line->substr(1, line->length());
		}

		// trim trailing whitespace
		while (line->length() && (*line)[line->length() - 1] == ' ') {
			*line = line->substr(0, line->length() - 1);
		}
	}

	return true;
}

void MapFileReader::restart_feed() {
	this->map_file->clear();
	this->map_file->seekg(0, std::ios::beg);
}

void MapFileReader::tokenize(std::vector<int>* tokens, std::string input_string, char delimiter) {
	while (!input_string.empty()) {
		int end = input_string.find_first_of(delimiter);

		if (end == std::string::npos) {
			if (!input_string.empty()) {
				tokens->push_back(std::stoi(input_string));
			}
			break;
		}

		tokens->push_back(std::stoi(input_string.substr(0, end)));
		input_string = input_string.substr(end + 1);
	}
}

class Map {
public:
	Map();
	~Map();

	int register_texture(sf::Texture* texture);
	void load_mapfile(std::string map_filename);

	void draw(sf::RenderWindow& window);
	std::string to_string();

protected:
	std::string map_filename;

	std::vector<sf::Texture*> textures_tiles;
	std::map<MapCoordinate, sf::Sprite*> tiles;
};

Map::Map() {
}

Map::~Map() {
	// delete all sprite pointers in the tilemap
	std::map<MapCoordinate, sf::Sprite*>::const_iterator iterator;
	for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
		delete iterator->second;
	}
}

void Map::load_mapfile(std::string map_filename) {
	std::string mapfile_line;
	MapFileReader mfr(map_filename);

	std::vector<int>* tokens = new std::vector<int>();
	while (mfr.readline(&mapfile_line)) {
		mfr.tokenize(tokens, mapfile_line);

		if (!tokens->size()) {
			continue;
		}

		// generate map coordinates from mapfile line
		MapCoordinate map_coord((*tokens)[0], (*tokens)[1]);
		
		sf::Sprite* sprite = new sf::Sprite();
		sprite->setTexture(*(this->textures_tiles[(*tokens)[2]]));

		ViewportCoordinate viewport_coord = map_coord.to_screen_coordinates();
		sprite->setPosition((float)viewport_coord.getX(), (float)viewport_coord.getY());
		sprite->setScale(sf::Vector2f((float)Global::Instance()->SCALE_X, (float)Global::Instance()->SCALE_Y));

		this->tiles[map_coord] = sprite;

		tokens->clear();
	}

	delete tokens;
}

// returns an integer that corresponds to the index of the newly registered texture (to refer to it later)
// will return -1 on failure
int Map::register_texture(sf::Texture* texture) {
	this->textures_tiles.push_back(texture);
	return this->textures_tiles.size() - 1;
}

void Map::draw(sf::RenderWindow& window) {
	std::map<MapCoordinate, sf::Sprite*>::const_iterator iterator;
	for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
		window.draw(*iterator->second);
	}
}

std::string Map::to_string() {
	std::string output;

	std::map<MapCoordinate, sf::Sprite*>::const_iterator iterator;
	for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
		output += iterator->first.to_string() + "\n";
	}

	return output;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(Global::Instance()->SCREEN_WIDTH, Global::Instance()->SCREEN_HEIGHT), "SFML Test");
	window.setFramerateLimit(60);

	float screen_middle_x = Global::Instance()->SCREEN_WIDTH / (float)2;
	float screen_middle_y = Global::Instance()->SCREEN_HEIGHT / (float)2;

	// set view
	sf::View view(sf::Vector2f(screen_middle_x, screen_middle_y),
		          sf::Vector2f((float)Global::Instance()->SCREEN_WIDTH, (float)Global::Instance()->SCREEN_HEIGHT));

	// load tile textures
	sf::Image tile1_nomask;
	tile1_nomask.loadFromFile("tile1.gif");
	tile1_nomask.createMaskFromColor(sf::Color::Magenta);

	sf::Texture* tile1 = new sf::Texture();
	tile1->loadFromImage(tile1_nomask);
	tile1->setSmooth(true);

	sf::Image tile2_nomask;
	tile2_nomask.loadFromFile("tile_grass.gif");
	tile2_nomask.createMaskFromColor(sf::Color::Magenta);

	sf::Texture* tile2 = new sf::Texture();
	tile2->loadFromImage(tile2_nomask);
	tile2->setSmooth(true);

	Map map;
	map.register_texture(tile1);
	map.register_texture(tile2);
	map.load_mapfile("map_test.txt");

	// set initial mouse position
	sf::Mouse::setPosition(sf::Vector2i(screen_middle_x, screen_middle_y), window);

	// main loop
	while (window.isOpen())
	{
		sf::Vector2i mouse_position = sf::Mouse::getPosition();

		// move view depending on mouse position
		//view.setCenter(static_cast<sf::Vector2f>(mouse_position));
		view.move(static_cast<sf::Vector2f>(mouse_position) - view.getCenter());

		// attach your viewport to the window. This must be called every render cycle!
		window.setView(view);

		// check for user clicking on x in upper right corner
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();

		// draw everything here
		map.draw(window);

		// end the current frame
		window.display();
	}

	return 0;
}