//
// Created by jyh on 19-3-20.
//

#ifndef CODECRAFT_2019_CROSS_H
#define CODECRAFT_2019_CROSS_H

#include <iostream>
#include <cstring>
#include <algorithm>
#include <map>

using namespace std;

class Cross {

public:
    int id;
    int id2 = -1;//real id
    int top;
    int right;
    int down;
    int left;
    int roadIds[4] = {0, 0, 0, 0};
    map<int, int> roadId2Direc;//[roadId, direction]
    map<int, int> direc2RoadId;//[direction, roadId]

    Cross() {}

    Cross(string s) {
        string content = s.substr(1, s.length() - 2);
        //s is a line in file
        char *delim = const_cast<char *>(" ,");
        char *cs = const_cast<char *>(content.c_str());
        char *seg;
        seg = strtok(cs, delim);
        this->id = atoi(seg);
        seg = strtok(NULL, delim);
        this->top = atoi(seg);
        this->roadIds[0] = atoi(seg);
        if (atoi(seg) != -1) {
            this->roadId2Direc[atoi(seg)] = 0;
            this->direc2RoadId[0] = atoi(seg);
        }//0 top
        seg = strtok(NULL, delim);
        this->right = atoi(seg);
        this->roadIds[1] = atoi(seg);
        if (atoi(seg) != -1) {
            this->roadId2Direc[atoi(seg)] = 1;
            this->direc2RoadId[1] = atoi(seg);
        }//1 top
        seg = strtok(NULL, delim);
        this->down = atoi(seg);
        this->roadIds[2] = atoi(seg);
        if (atoi(seg) != -1) {
            this->roadId2Direc[atoi(seg)] = 2;
            this->direc2RoadId[2] = atoi(seg);
        }//2 top
        seg = strtok(NULL, delim);
        this->left = atoi(seg);
        this->roadIds[3] = atoi(seg);
        if (atoi(seg) != -1) {
            this->roadId2Direc[atoi(seg)] = 3;
            this->direc2RoadId[3] = atoi(seg);
        }//3 top

        sort(begin(roadIds), end(roadIds), comp);
    }

    static const bool comp(int a, int b) {
        return a < b;//sort by roadId
    };

//#(id,roadId,roadId,roadId,roadId)
    string anser2string() {
        string s = "(" + to_string(this->id) + "," + to_string(this->roadIds[0]) + ","
                   + to_string(this->roadIds[1]) + "," + to_string(this->roadIds[2]) + ","
                   + to_string(this->roadIds[3]) + ")";
        return s;
    }
};


#endif //CODECRAFT_2019_CROSS_H
