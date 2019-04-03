//
// Created by jyh on 19-3-20.
//

#ifndef CODECRAFT_2019_ROAD_H
#define CODECRAFT_2019_ROAD_H

#include <iostream>
#include <cstring>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include "Car.h"

using namespace std;

//typedef struct  RoadInfo{
//	int id;
//	int length;
//	int speed;
//	int channel;
//	int isDuplex;
//
//}RoadInfo;

class Road {

public:
    int id;
    int id2 = -1;//real id
    int length;
    int speed;
    int channel;
    int from;
    int to;
    int isDuplex;

    Car ca;//this is an empty object,it is used for return an empty object reference
    int tmp = 0, tmp2 = 0;//just for tmp using
    int *tmpPosi, *restPosi;// has not been used
    deque<deque<Car>> car;//car in this road, it is begining cross "from",it has superiority,the first one is the first superiority
    deque<deque<Car>> car2;//car in this road of reverse direction, it is begining cross "to"
    //the row is channel, column is length

    list<int> posi;//wait position,it is a number,for length 3,channel 2,number 3 represents [3%2,3/2],i.e. row 1 column 1
    list<int> posi2;
    map<int, int> posi2carInd;
    map<int, int> posi2carInd2;


    void receiveCar(int &currCrossId, Car &ca, int len, int cha) {
        //this is used for receive a new car comes from garage
        //it is always ending state, so do not update posi

        if (currCrossId == this->to) {//it also has crossId == this-> from
            ca.goAwayGarage(length - len, cha, int(car[cha].size() - 1));
            car[cha].push_back(ca);
        } else {
            ca.goAwayGarage(length - len, cha, int(car2[cha].size() - 1));
            car2[cha].push_back(ca);
        }
    }

//    void sortPosi(int &currCrossId) {
//        //this is used for receive a new car comes from garage
//        if (currCrossId == this->to) posi.sort();
//        else posi2.sort();
//    }

    Car &firstWaitCar(deque<deque<Car>> &car, list<int> &posi, map<int, int> &posi2carInd) {
        if (!posi.empty()) {
            tmp = posi.front();
            tmp2 = posi2carInd.at(tmp);
            return car[tmp % channel][tmp2];
        } else return ca;
        //return first channel who is not empty

    }

    void popFirstWaitCarCore(deque<deque<Car>> &car, list<int> &posi, map<int, int> &posi2carInd) {
        if (!posi.empty()) {
            tmp = posi.front();
            car[tmp % channel].pop_front();
            posi2carInd.erase(tmp);
            posi.pop_front();
        }

    }

    Car &getFirstWaitCar(int &currCrossId) {
        if (currCrossId == this->to) //it also has crossId == this-> from
            return firstWaitCar(car, posi, posi2carInd);
        else
            return firstWaitCar(car2, posi2, posi2carInd2);
    }

    void popFirstWaitCar(int &currCrossId) {
        if (currCrossId == this->to) //it also has crossId == this-> from
            popFirstWaitCarCore(car, posi, posi2carInd);
        else
            popFirstWaitCarCore(car2, posi2, posi2carInd2);

    }


//    int *getFirstCarPosition(int &currCrossId) {
//        if (currCrossId == this->to)//it also has crossId == this-> from
//            tmp = posi.front();
//        else
//            tmp = posi2.front();;
//        tmpPosi[0] = tmp / channel;//length
//        tmpPosi[1] = tmp % channel;//channel
//        return tmpPosi;
//    }

    void receiveCarFromOtherRoad(int &currCrossId, Car &ca, int &goMaxDis, int &cha) {
        //this is used for car move to an other road, it will be end state
        if (currCrossId == this->to)//it also has crossId == this-> from
        {
            ca.moveToOtherRoad(length - goMaxDis, cha, int(car[cha].size() - 1));
            car[cha].push_back(ca);
        } else {
            ca.moveToOtherRoad(length - goMaxDis, cha, int(car2[cha].size() - 1));
            car2[cha].push_back(ca);
        }

    }

    deque<deque<Car>> &getCar(int &currCrossId) {//currCrossId is ending cross of current road
        if (currCrossId == this->to)//it also has crossId == this-> from
            return car;
        else
            return car2;
    }

    int getPrecedingPosiOfCar(int &currCrossId, Car &ca) {//currCrossId is ending cross of current road
        tmp = ca.position[1] * channel + ca.position[2];
        if (currCrossId == this->to)//it also has crossId == this-> from
            return posi2carInd.at(tmp) - 1;
        else
            return posi2carInd2.at(tmp) - 1;
    }

    void setWaitStat(int &currCrossId, Car &ca, int indexOfVector) {
        ca.setWaitingState();
        tmp = ca.position[1] * channel + ca.position[2];
        if (currCrossId == this->to) {//it also has crossId == this-> from
            posi.push_back(tmp);
            posi2carInd.insert(pair<int, int>{tmp, indexOfVector});
        } else {
            posi2.push_back(tmp);
            posi2carInd2.insert(pair<int, int>{tmp, indexOfVector});
        }
    }

    void clearWaitPosi(int &currCrossId) {
        if (currCrossId == this->to) {//it also has crossId == this-> from
            posi.clear();
            posi2carInd.clear();
        } else {
            posi2.clear();
            posi2carInd2.clear();
        }
    }

    void clearWaitPosiInCurrChannel(int &currCrossId, int &cha) {
//        for (tmp = 0; tmp < length; tmp++) {
//            tmp2 = tmp * channel + cha;
//            if (currCrossId == this->to) {//it also has crossId == this-> from
//                posi.remove(tmp2);
//                posi2carInd.erase(tmp2);
//
//            } else {
//                posi2.remove(tmp2);
//                posi2carInd2.erase(tmp2);
//            }
//        }
        if (currCrossId == this->to) {//it also has crossId == this-> from
            if (posi.empty()) return;
        } else {
            if (posi2.empty()) return;
        }


        for (tmp = 0; tmp < length; tmp++) {
            if (currCrossId == this->to) {//it also has crossId == this-> from
                if (posi2carInd.count(tmp * channel + cha) > 0) {
                    posi.remove(tmp * channel + cha);
                    posi2carInd.erase(tmp * channel + cha);
                }
            } else {
                if (posi2carInd2.count(tmp * channel + cha) > 0) {
                    posi2.remove(tmp * channel + cha);
                    posi2carInd2.erase(tmp * channel + cha);
                }
            }
        }
    }

    int getNumOfWaitCars(int &currCrossId) {
        if (currCrossId == this->to)//it also has crossId == this-> from
            return int(posi.size());
        else
            return int(posi2.size());
    }

    void sortWaitPosi(int &currCrossId) {
        if (currCrossId == this->to)//it also has crossId == this-> from
            posi.sort();
        else
            posi2.sort();
    }
//    int *getMaxDisCanGo(int &currCrossId) {
//
//        if (currCrossId == this->to)//it also has crossId == this-> from
//            for (tmp = 0; tmp < channel; tmp++) {
//                if (car[tmp].empty()) tmp2 = length;
//                else tmp2 = length - car[tmp].back().position[1] - 1;
//                restPosi[tmp] = tmp2;
//            }
//        else
//            for (tmp = 0; tmp < channel; tmp++) {
//                if (car2[tmp].empty()) tmp2 = length;
//                else tmp2 = length - car2[tmp].back().position[1] - 1;
//                restPosi[tmp] = tmp2;
//            }
//        return restPosi;
//
//    }

    void moveCar(int &currCrossId, Car &ca, int len) {//len is length the car would go
        //this is used for cross scheduler and the car can not go out of this road,it may not be the first car
        tmp = ca.position[1] * channel + ca.position[2];
        if (currCrossId == this->to) {//it also has crossId == this-> from
            posi.remove(tmp);
            posi2carInd.erase(tmp);
        }
//            posi.pop_front();
        else {
//            posi2.pop_front();
            posi2.remove(tmp);
            posi2carInd2.erase(tmp);
        }
        ca.endWait = false;
        ca.position[1] = ca.position[1] - len;// run to position
    }

    void moveCar(int &currCrossId, Car *ca, int len) {//len is length the car would go
        //this is used for cross scheduler and the car can not go out of this road,it may not be the first car
        tmp = ca->position[1] * channel + ca->position[2];
        if (currCrossId == this->to) {//it also has crossId == this-> from
            posi.remove(tmp);
            posi2carInd.erase(tmp);
        } else {
            posi2.remove(tmp);
            posi2carInd2.erase(tmp);
        }
        ca->endWait = false;
        ca->position[1] = ca->position[1] - len;// run to position
    }

    int otherCrossId(int &currCrossId) {
        if (currCrossId == to) return from;
        else return to;
    }

//#(id,length,speed,channel,from,to,isDuplex)
    Road(string
         s) {
        string content = s.substr(1, s.length() - 2);
//s is a line in file
        char *delim = const_cast<char *>(" ,");
        char *cs = const_cast<char *>(content.c_str());
        char *seg;
        seg = strtok(cs, delim);
        this->
                id = atoi(seg);
        seg = strtok(NULL, delim);
        this->
                length = atoi(seg);
        seg = strtok(NULL, delim);
        this->
                speed = atoi(seg);
        seg = strtok(NULL, delim);
        this->
                channel = atoi(seg);
        seg = strtok(NULL, delim);
        this->
                from = atoi(seg);
        seg = strtok(NULL, delim);
        this->
                to = atoi(seg);
        seg = strtok(NULL, delim);
        this->
                isDuplex = atoi(seg);
        if (!isDuplex) length = 10000;

        for (
                int i = 0;
                i < channel;
                i++) {
            car.
                    emplace_back(deque<Car>{}
            );
            car2.
                    emplace_back(deque<Car>{}
            );
        }

    }

    Road() {};

    //#(id,length,speed,channel,from,to,isDuplex)
    string anser2string() {
        string s = "(" + to_string(this->id) + "," + to_string(this->length) + ","
                   + to_string(this->speed) + "," + to_string(this->channel) + ","
                   + to_string(this->from) + "," + to_string(this->to) + ","
                   + to_string(this->isDuplex) + ")";
        return s;
    }
};

#endif //CODECRAFT_2019_ROAD_H
