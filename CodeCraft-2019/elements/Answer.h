//
// Created by jyh on 19-3-20.
//

#ifndef CODECRAFT_2019_ANSWER_H
#define CODECRAFT_2019_ANSWER_H

#include <iostream>
#include <utility>
#include <vector>


#include <iterator>
#include <sstream>


using namespace std;

class Answer {
public:
    int carId;
    int startTime;
    int planTime;//startTime may be modified by scheduler, so save the init value for answer submission
    vector<int> roadId;
    int alternateDomain;
    int carId2;// real Id
//    vector<int> direction;//direction in each cross

    //Answer(int id, int start, vector<int> road);
    Answer(int id, int start, vector<int> road, int id2) :
            carId(id), startTime(start), roadId(std::move(road)), carId2(id2) {
    }

    Answer(int id, int start, vector<int> road, int alternate, int id2) :
            carId(id), startTime(start), roadId(std::move(road)), alternateDomain(alternate), carId2(id2) {
        planTime = startTime;

    }

    Answer() {};

    string anser2string() {
        string s = "(" + to_string(this->carId2) + "," + to_string(this->startTime) + ",";
        ostringstream oss;
        copy(this->roadId.begin(), this->roadId.end(), ostream_iterator<int>(oss, ","));
        string raodIds = oss.str();
        s = s + raodIds.substr(0, raodIds.length() - 1) + ")";
        return s;
    }


};


#endif //CODECRAFT_2019_ANSWER_H
