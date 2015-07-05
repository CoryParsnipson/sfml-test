#include "MapFileReader.h"

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