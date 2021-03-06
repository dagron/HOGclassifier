// This file is part of HOGclassifier.

// HOGclassifier is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// HOGclassifier is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with HOGclassifier.  If not, see <http://www.gnu.org/licenses/>.

#include "lasers_parser.h"
#include <iostream>
#include <fstream>
#include "logger.h"
#include "utils.h"
using namespace std;

LaserParser::LaserParser(const std::string &fileName)
{
    std::ifstream in;
    in.open(fileName.c_str());
    if (!in)
    {
        Logger::instance()->logError("LaserParser", Logger::FILE_NOT_OPENED);
        return;
    }
    std::string line;
    std::string IMAGE_NAME_TAG = "IMAGE_NAME";
    std::string IMAGE_POS_TAG = "IMAGE_POS";
    std::vector<double> tempAngles;
    std::vector<double> tempGpsX;
    std::vector<double> tempGpsY;
    double imageX, imageY;
    std::string tempImageName;

    while (std::getline(in, line)) // read file until there are new lines
    {
        if (line.find(IMAGE_NAME_TAG) == 0)
        {
            _angles[tempImageName] = tempAngles;
            _pointsX[tempImageName] = tempGpsX;
            _pointsY[tempImageName] = tempGpsY;
            _imagePosX[tempImageName] = imageX;
            _imagePosY[tempImageName] = imageY;
            tempAngles.clear();
            tempGpsX.clear();
            tempGpsY.clear();
            imageX = -1;
            imageY = -1;
            tempImageName = Utils::splitString(line, "\t")[1];
        }
        else if (line.find(IMAGE_POS_TAG) == 0)
        {
            std::vector<std::string> tempSplit = Utils::splitString(line, "\t");
            imageX = atof(tempSplit[2].c_str());
            imageY = atof(tempSplit[4].c_str());
        }
        else
        {
            std::vector<std::string> tempSplit = Utils::splitString(line, "\t");
            double angle = atof(tempSplit[3].c_str());
            double x = atof(tempSplit[5].c_str());
            double y = atof(tempSplit[7].c_str());
            tempAngles.push_back(angle);
            tempGpsX.push_back(x);
            tempGpsY.push_back(y);
        }
    }
}

void LaserParser::getPointsForImageFov(
    const std::string &imageName,
    const double &fovStart,
    const double &fovEnd,
    std::vector<double> &xVec,
    std::vector<double> &yVec)
{
    std::cout << "image name " << imageName << std::endl;
    xVec.clear();
    yVec.clear();
    std::vector<double> tempPointsX;
    std::vector<double> tempPointsY;
    tempPointsX = _pointsX[imageName];
    tempPointsY = _pointsY[imageName];
    int counter = 0;
    for (const auto &angle : _angles[imageName])
    {
        if (angle > fovStart && angle < fovEnd)
        {
            xVec.push_back(tempPointsX[counter]);
            yVec.push_back(tempPointsY[counter]);
        }
        counter++;
    }
}

void LaserParser::getImagePos(
    const std::string &imageName,
    double &x, double &y)
{
    x = _imagePosX[imageName];
    y = _imagePosY[imageName];
}
