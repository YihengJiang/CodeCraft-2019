//
// Created by jyh on 19-3-20.
//



#ifndef CODECRAFT_2019_CAR_H
#define CODECRAFT_2019_CAR_H


#include <iostream>
#include <cstring>
#include "Answer.h"


using namespace std;


class Car {

public:
    int id = -1;// -1 represent this car is not init
    int id2 = -1;//real id
    int from;
    int to;
    int speed;
    int planTime;
    bool endWait;//end=false,wait=true
    Answer answer;//current car'answer
    int position[4] = {-1, -1, -1, -1};//indexOfroadIdInAnswer,position,channel,index Of deque<deque<Car>> in road
    //indexOfAnswer is used for find the next roadId

    int direct[6] = {-1, -1, -1, -1, -1, -1};
    // the direction it has been tried,includes the direction it self posed.
    // the last two value is road id and cross id
    // 0: itself direction, 1 it has selected direction, -1 it has not been selected

    //#(id,from,to,speed,planTime)
    void goAwayGarage(int po2, int po3, int po4) {
        endWait = false;
        position[0] = 0;
        position[1] = po2;
        position[2] = po3;
        position[3] = po4;
    }

    void moveToOtherRoad(int po2, int po3, int po4) {
        endWait = false;
        position[0]++;
        position[1] = po2;
        position[2] = po3;
        position[3] = po4;
    }

    void setWaitingState() {
        endWait = true;
    }

    bool operator==(const Car &car) const {//used for find in deque
        return car.id == this->id;

    }


    Car(string s) {
        //s is a line in file
        string content = s.substr(1, s.length() - 2);
        //s is a line in file
        char *delim = const_cast<char *>(" ,");
        char *cs = const_cast<char *>(content.c_str());
        char *seg;
        seg = strtok(cs, delim);
        this->id = atoi(seg);
        seg = strtok(NULL, delim);
        this->from = atoi(seg);
        seg = strtok(NULL, delim);
        this->to = atoi(seg);
        seg = strtok(NULL, delim);
        this->speed = atoi(seg);
        seg = strtok(NULL, delim);
        this->planTime = atoi(seg);
        seg = strtok(NULL, delim);
        if (seg)
            this->id2 = atoi(seg);

    };

    Car() {};

//#(id,from,to,speed,planTime)
    string anser2string() {
        string s = "(" + to_string(this->id) + "," + to_string(this->from) + ","
                   + to_string(this->to) + "," + to_string(this->speed) + "," +
                   to_string(this->planTime) + "," + to_string(this->id2) + ")";
        return s;
    }
};

#endif //CODECRAFT_2019_CAR_H
