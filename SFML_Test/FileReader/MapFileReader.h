#ifndef MAP_FILEREADER_H
#define MAP_FILEREADER_H

#include "../dependencies.h"

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

#endif