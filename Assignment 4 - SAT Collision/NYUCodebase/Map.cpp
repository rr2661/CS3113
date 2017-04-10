#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

void placeEntity(string type, float placeX, float placeY) {

}

class Map {
	bool readHeader(std::ifstream &stream) {
		string line;
		mapWidth = -1;
		mapHeight = -1;
		while (getline(stream, line)) {
			if (line == "") { break; }
			istringstream sStream(line);
			string key, value;
			getline(sStream, key, '=');
			getline(sStream, value);
			if (key == "width") {
				mapWidth = atoi(value.c_str());
			}
			else if (key == "height") {
				mapHeight = atoi(value.c_str());
			}
		}
		if (mapWidth == -1 || mapHeight == -1) {
			return false;
		}
		else { // allocate our map data
			levelData = new unsigned char*[mapHeight];
			for (int i = 0; i < mapHeight; ++i) {
				levelData[i] = new unsigned char[mapWidth];
			}
			return true;
		}
	}

	bool readLayerData(std::ifstream &stream) {
		string line;
		while (getline(stream, line)) {
			if (line == "") { break; }
			istringstream sStream(line);
			string key, value;
			getline(sStream, key, '=');
			getline(sStream, value);
			if (key == "data") {
				for (int y = 0; y < mapHeight; y++) {
					getline(stream, line);
					istringstream lineStream(line);
					string tile;
					for (int x = 0; x < mapWidth; x++) {
						getline(lineStream, tile, ',');
						unsigned char val = (unsigned char)atoi(tile.c_str());
						if (val > 0) {
							// be careful, the tiles in this format are indexed from 1 not 0
							levelData[y][x] = val - 1;
						}
						else {
							levelData[y][x] = 0;
						}
					}
				}
			}
		}
		return true;
	}

	bool readEntityData(std::ifstream &stream) {
		string line;
		string type;
		while (getline(stream, line)) {
			if (line == "") { break; }
			istringstream sStream(line);
			string key, value;
			getline(sStream, key, '=');
			getline(sStream, value);
			if (key == "type") {
				type = value;
			}
			else if (key == "location") {
				istringstream lineStream(value);
				string xPosition, yPosition;
				getline(lineStream, xPosition, ',');
				getline(lineStream, yPosition, ',');
					float placeX = atoi(xPosition.c_str())*TILE_SIZE;
				float placeY = atoi(yPosition.c_str())*-TILE_SIZE;
				placeEntity(type, placeX, placeY);
			}
		}
		return true;
	}

	void readfile(string levelFile) {
		ifstream infile(levelFile);
		string line;
		while (getline(infile, line)) {
			if (line == "[header]") {
				if (!readHeader(infile)) {
					return;
				}
			}
			else if (line == "[layer]") {
				readLayerData(infile);
			}
			else if (line == "[ObjectsLayer]") {
				readEntityData(infile);
			}
		}
	}
	int TILE_SIZE;
	int mapHeight;
	int mapWidth;
	unsigned char** levelData;
};