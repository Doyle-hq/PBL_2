#ifndef POSITION_REPOSITORY_H
#define POSITION_REPOSITORY_H

#include "../Models/Position.h"
#include <vector>
#include <string>
using namespace std;

class PositionRepository{
private:
    string fileName;

public:
    PositionRepository(string fileName = "Data/Position.txt");

    vector<Position> getAll();
    bool saveAll(const vector<Position>& positions);

    bool add(const Position& position);
    bool update(string oldPHG_ID, string oldPositionName, const Position& newPosition);
    bool remove(string PHG_ID, string positionName);
    bool findByName(string PHG_ID, string positionName, Position& position);
};

#endif
