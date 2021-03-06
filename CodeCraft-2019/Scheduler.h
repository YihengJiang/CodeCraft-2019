//
// Created by jyh on 19-3-23.
//

#ifndef CODECRAFT_2019_SCHEDULER_H
#define CODECRAFT_2019_SCHEDULER_H

#include <iostream>
#include "Process.h"
#include "elements/Utils.h"

#define DEAD_LOCK 0
#define UN_LOCK 1
#define FINISH 2
#define NOT_FINISH 3 //this state will not returned to Process class

using namespace std;


class Scheduler {

private:
    int maxLoops = 500;//train1 train2:{300,500}
    // it can not be so large, or it will be better than official result
    //it also can not be so small, or it will be worse than official result

    int restCars;//this is the rest number of car who do no arrive terminal
    vector<Answer> answer;// this is optimized answer
    Car blockCar;// used for save current block car in current time current road current cross
    int lockInfo[5] = {0};
    Car lockedCar;

    bool isConflict(map<int, Road> &road, const int &nextDir, Cross &cr, int &direcDiff, const int *diff) {
        int mayConflictDir = nextDir + diff[0] < 0 ? nextDir + diff[1] : nextDir + diff[0];
        auto mayConflictRoadId = cr.direc2RoadId.find(mayConflictDir);
        if (mayConflictRoadId == cr.direc2RoadId.end()) return false;// has no other road, so equal to un-conflict
        Car &firstCar = road[mayConflictRoadId->second].getFirstWaitCar(cr.id);
        if (firstCar.id == -1) return false;//this road has no cars, so equal to un-conflict
        direcDiff = mayConflictDir - cr.roadId2Direc[firstCar.answer.roadId[firstCar.position[0] + 1]];
        return direcDiff == diff[0] || direcDiff == diff[1];//true:conflict

    };

    int computeRunDistanceOfCurrentCarInCross(const Car &car, map<int, Road> &road, const int &roadId,
                                              const int &nextRoadId, int &s1, int &v2) {
        s1 = car.position[1];
        v2 = road[nextRoadId].speed > car.speed ? car.speed : road[nextRoadId].speed;
        return v2 - s1 > 0 ? v2 - s1 : 0;
    }


    bool getConflictState(map<int, Road> &road, int &nextDir, Cross &cr, int &direcDiff, int *dir2, int *dir1) {
        //return is conflict(true represents has no conflict)
        if (direcDiff == dir1[0] || direcDiff == dir1[1]) {//straight
            return true;
        } else if (direcDiff == dir2[0] || direcDiff == dir2[1]) {//left,so the straight may be conflicted
//                        int mayConflictDir = nextDir - 2 < 0 ? nextDir + 2 : nextDir - 2;//straight
//                        int mayConflictRoadId = cr.direc2RoadId[mayConflictDir];
//                        runDir(road, mayConflictRoadId, direcDiff, cr, mayConflictDir);
//                        if (direcDiff == 2 || direcDiff == -2)//conflict
//                            break;
            if (isConflict(road, nextDir, cr, direcDiff, dir1)) return false;
        } else {//right,direcDiff==-3||direcDiff==1,so the rest of all 2 roads may be conflicted
//                        int mayConflictDir = nextDir - 2 < 0 ? nextDir + 2 : nextDir - 2;//straight
//                        int mayConflictRoadId = cr.direc2RoadId[mayConflictDir];
//                        runDir(road, mayConflictRoadId, direcDiff, cr, mayConflictDir);
//                        if (direcDiff == 2 || direcDiff == -2)//conflict
//                            break;
            if (isConflict(road, nextDir, cr, direcDiff, dir1)) return false;
//                        int mayConflictDir2 = nextDir - 1 < 0 ? nextDir + 3 : nextDir - 1;//left
//                        int mayConflictRoadId2 = cr.direc2RoadId[mayConflictDir2];
//                        runDir(road, mayConflictRoadId2, direcDiff, cr, mayConflictDir2);
//                        if (direcDiff == -1 || direcDiff == 3)//conflict
//                            break;
            if (isConflict(road, nextDir, cr, direcDiff, dir2)) return false;
        }
        return true;
    }

    int moveCarInCurrentChannel(int &channel, int &speed, int &maxLen,
                                Road &currRoad, int &cross, deque<deque<Car>> &cars) {
        int count = 0;
        Car *firstCar = nullptr, *secondCar = nullptr;
        deque<Car> &carsInChannel = cars[channel];
        if (carsInChannel.empty()) return -1;
        for (auto it = carsInChannel.begin(); it != carsInChannel.end();) {
            if (!firstCar) {//the first car
                firstCar = &(*it);
                speed = firstCar->speed > currRoad.speed ? currRoad.speed : firstCar->speed;
                //if it has arrived terminal
                if (currRoad.id == firstCar->answer.roadId.back() &&
                    speed > firstCar->position[1]) {//arrived, if it is ==, this car will arrive at the next time
                    //this car will be removed, so position will not be added.
                    restCars--;
//                        cout<<restCars<<endl;
                    answer.push_back(firstCar->answer);
                    it = carsInChannel.erase(it);
                    firstCar = nullptr;
                    if (restCars == 0)
                        return FINISH;
                    continue;
                }
                //if it has not arrived terminal
                if (speed > firstCar->position[1]) {//wait
                    currRoad.setWaitStat(cross, *firstCar, count);
                } else {//move distance is equal to speed
                    currRoad.moveCar(cross, firstCar, speed);
                }
                it++;
                count++;
            } else {//the second car, before it, there has the first car, so it will always can not arrive to terminal.
                secondCar = &(*it);
                //maxLen is max step this car can go
                maxLen = secondCar->position[1] - firstCar->position[1] - 1;
                speed = secondCar->speed > currRoad.speed ? currRoad.speed : secondCar->speed;
                if (speed > maxLen) {
                    if (firstCar->endWait) {//wait block
                        currRoad.setWaitStat(cross, *secondCar, count);
                    } else {//end block
                        currRoad.moveCar(cross, secondCar, maxLen);
                    }
                } else {//no block
                    currRoad.moveCar(cross, secondCar, speed);
                }
                firstCar = secondCar;
                it++;
                count++;
            }
        }
        return 0;
    }


    int moveCarInCurrentRoad(int &channel, int &speed, int &maxLen,
                             Road &currRoad, int &cross, deque<deque<Car>> &cars) {//handling the first direction
        //fisrtly, need to clear waiting position
//        currRoad.clearWaitPosi(cross);

        currRoad.clearWaitPosi(cross);//currRoad.id==5049&&cross==34
        for (channel = 0; channel < currRoad.channel; channel++)
            if (moveCarInCurrentChannel(channel, speed, maxLen, currRoad, cross, cars) == FINISH)
                return FINISH;
        currRoad.sortWaitPosi(cross);//sort position, to ensure the order of waiting cars
        return NOT_FINISH;
    }


    void findPositionToSetCar(int &channel, int &length, Road &currRoad, int &crossId) {
        //this method will revise channel length car
        deque<deque<Car>> &carIncurrRoad = currRoad.getCar(crossId);

        for (channel = 0; channel < currRoad.channel; channel++) {
            if (carIncurrRoad[channel].empty()) {
                length = currRoad.length;//this road has no car, so could go the whole road
                break;
            } else {
                Car &blockCar = carIncurrRoad[channel].back();
                length = currRoad.length - blockCar.position[1] - 1;
                this->blockCar = blockCar;
                if (length != 0) break;
            }
        }
    }


    void setDirToCar(int &roadDir, int &nextDir, Car &car, int &crossId, int &roadId) {
        if (car.id == lockedCar.id && roadId == lockedCar.direct[4]) {
            lockedCar.endWait = car.endWait;
            lockedCar.answer = car.answer;
            lockedCar.position[0] = car.position[0];
            lockedCar.position[1] = car.position[1];
            lockedCar.position[2] = car.position[2];
            lockedCar.position[3] = car.position[3];
        } else {
            lockedCar = car;
        }
        lockedCar.direct[roadDir] = 0;
        lockedCar.direct[nextDir] = 1;
        lockedCar.direct[4] = roadId;
        lockedCar.direct[5] = crossId;

    }

public:

    int
    oneStep(vector<Car> &car, map<int, Road> &road, vector<Cross> &cross, const int t, const int firstTimeCarOnRoad) {
        // t is current time slop;map<int,Answer> &answer:int is carId
        //this is core of scheduler.
        //note: this method just run all car in a time slop.
        //we will loop this method in our optimizer for the whole arrangement time
        const auto num = int(road.size());
        int nextDir, roadDir, nextRoadId, s2, s1, v2, crossId, precedPosi, nextRoadChannel, roadIdOfFirstCar;
        int direcDiff, channel, length, otherCrossId, count, speed, maxLen;
        int dir1[] = {-2, 2}, dir2[] = {-1, 3}, loops[num * 2] = {0};


        if (t > firstTimeCarOnRoad) {
            //TODO 1. run all car in road and label it with waiting or ending state

//        int *firstCarPosi;
//        for (pair<int, Road> ro:road) {
            for (auto &ro : road) {
                //drive all car on current road to ending state, or label it as waiting state
                Road &currRoad = ro.second;
                //because it has all been ending state, so need not clear posi in road
                if (currRoad.isDuplex) {//handling the second direction
                    deque<deque<Car>> &cars = currRoad.getCar(currRoad.from);
                    if (moveCarInCurrentRoad(channel, speed, maxLen, currRoad, currRoad.from, cars) == FINISH)
                        return FINISH;
                }
                //handling the first direction
                deque<deque<Car>> &cars = currRoad.getCar(currRoad.to);
                if (moveCarInCurrentRoad(channel, speed, maxLen, currRoad, currRoad.to, cars) == FINISH)
                    return FINISH;
            }





            //TODO 2. drive all waiting cars


//        cout << "cross should have been sorted by id"
//                "and sorted by car Id; answer is map<int,Answer> &answer;"
//                "" <<
//             endl;

            while (true) {
                count = -1;
                for (auto &cr : cross) {
                    //handling each roads in this cross,if conflict, then break;
                    for (int roadId:cr.roadIds) {// 4 direction
                        if (roadId == -1)//road has no this direction
                            continue;
                        count++;
                        Road &currRoad = road[roadId];
                        roadDir = cr.roadId2Direc[roadId];

                        while (currRoad.getNumOfWaitCars(cr.id) != 0) {//0 represents has no wait car
                            //the first car
                            Car &firstCar = currRoad.getFirstWaitCar(cr.id);

                            deque<deque<Car>> &cars = currRoad.getCar(cr.id);// all cars in this road
                            deque<Car> &carInChannel = cars[firstCar.position[2]];

                            //the first wait car is not in the first position of this channel,so it can not go out of this road
                            if (firstCar.id != carInChannel[0].id) {
                                precedPosi = currRoad.getPrecedingPosiOfCar(cr.id, firstCar);
                                Car &blockCarInCurrRoad = carInChannel[precedPosi];
                                currRoad.moveCar(cr.id, firstCar,
                                                 firstCar.position[1] - blockCarInCurrRoad.position[1] - 1);
                                loops[count] = 0;
                                break;//loop to next road
                            }

                            //the first wait car is in the first position of this channel
                            nextRoadId = firstCar.answer.roadId[firstCar.position[0] + 1];
                            Road &nextRoad = road[nextRoadId];
                            nextDir = cr.roadId2Direc[nextRoadId];
                            direcDiff = roadDir - nextDir;

                            //isConflict
                            if (!getConflictState(road, nextDir, cr, direcDiff, dir2, dir1)) break;

                            //drive First Car to next road

                            //compute how long this car could go
                            s2 = computeRunDistanceOfCurrentCarInCross(firstCar, road, roadId, nextRoadId, s1, v2);
                            //check if has block car in next road, note this is go out this road, i.e. go away from current cross
                            otherCrossId = nextRoad.otherCrossId(cr.id);
                            findPositionToSetCar(nextRoadChannel, length, nextRoad, otherCrossId);
                            channel = firstCar.position[2];

                            //may be blocked to set first car,length and s2 is rest set could go
                            if (length < s2) {//block
                                if (blockCar.endWait) {//wait block
                                    firstCar.setWaitingState();
                                    loops[count]++;
                                    roadIdOfFirstCar = roadId;
                                } else {//end block
                                    loops[count] = 0;
                                    if (length != 0) {
                                        Car tmpCar = firstCar;
                                        currRoad.popFirstWaitCar(cr.id);
                                        nextRoad.receiveCarFromOtherRoad(otherCrossId, tmpCar, length,
                                                                         nextRoadChannel);
                                        roadIdOfFirstCar = nextRoad.id;
                                    } else {//car can not go out of current road
                                        currRoad.moveCar(cr.id, firstCar, firstCar.position[1]);
                                        roadIdOfFirstCar = roadId;
                                    }
                                }
                            } else {// no block,pop element and then push to next road,and set endWait=false
                                loops[count] = 0;
                                if (s2 != 0) {
                                    Car tmpCar = firstCar;
                                    currRoad.popFirstWaitCar(cr.id);
                                    nextRoad.receiveCarFromOtherRoad(otherCrossId, tmpCar, s2, nextRoadChannel);
                                    roadIdOfFirstCar = nextRoad.id;
                                } else {//car can not go out of current road
                                    currRoad.moveCar(cr.id, firstCar, firstCar.position[1]);
                                    roadIdOfFirstCar = roadId;
                                }
                            }

                            //let us see if it dead lock using our own method,this method is very inefficient,
                            //how much I want to know the better way to do it
                            if (loops[count] >= maxLoops) {
                                cout << "\ndead lock: cross: " << cr.id << " road: "
                                     << currRoad.id << " car: " << firstCar.id
                                     << " time: " << t << " loops: " << loops[count] << endl;
                                //save something useful for go back to past time slop
                                lockInfo[0] = cr.id;
                                lockInfo[1] = currRoad.id;
                                lockInfo[2] = firstCar.id;
                                lockInfo[3] = t;
                                lockInfo[4] = loops[count];
                                lockedCar = firstCar;
                                return DEAD_LOCK;//dead lock
                            }

                            //fisrtCar has not go out of this road
                            if (roadIdOfFirstCar == roadId) break;

                            //drive all car on current road to ending state, or label it as waiting state
                            //beacause may have some cars are waiting state in this road.
                            currRoad.clearWaitPosiInCurrChannel(cr.id, channel);
                            if (moveCarInCurrentChannel(channel, speed, maxLen, currRoad, cr.id, cars) == FINISH)
                                return FINISH;
                            currRoad.sortWaitPosi(cr.id);

//                            firstCar = currRoad.getFirstWaitCar(cr.id);

                        }
                    }
                }
//                int h = sum(loops, num * 2);
                if (sum(loops, num * 2) == 0) break; //all car is ending
            }
        }


        //TODO 3. drive car in garage, ordered by carId, all car on road are ending state
        if (!car.empty()) {
            for (auto it = car.begin(); it < car.end();) {
                Car &ca = *it;
                if (ca.answer.startTime == t) {
                    //drive this car

                    //check if it can go away from the garage
                    int roadId = ca.answer.roadId[0];
                    Road &currRoad = road[roadId];
//                    restMaxDis = currRoad.getMaxDisCanGo(otherCrossId);
                    crossId = currRoad.from == ca.from ? currRoad.to : currRoad.from;

                    findPositionToSetCar(channel, length, currRoad, crossId);

                    //move this car
                    speed = ca.speed > currRoad.speed ? currRoad.speed : ca.speed;


                    if (length < speed) {//block
                        if (length != 0) {
                            currRoad.receiveCar(crossId, ca, length, channel);
                            it = car.erase(it);
                        } else {// if length == 0 ,this car should wait for next time slop to go, so do not move it
                            ca.answer.startTime++;
                            it++;
                        }
                    } else {// no block,
                        currRoad.receiveCar(crossId, ca, speed, channel);
                        it = car.erase(it);
                    }
                } else it++;
            }
        }
        return UN_LOCK;
    }


    int
    oneStep(vector<Car> &car, map<int, Road> &road, vector<Cross> &cross, const int t, const int firstTimeCarOnRoad,
            vector<Car> &car1) {
        // t is current time slop;map<int,Answer> &answer:int is carId
        //this is core of scheduler.
        //note: this method just run all car in a time slop.
        //we will loop this method in our optimizer for the whole arrangement time
        const auto num = int(road.size());
        int nextDir, roadDir, nextRoadId, s2, s1, v2, crossId, precedPosi, nextRoadChannel, roadIdOfFirstCar;
        int direcDiff, channel, length, otherCrossId, count, speed, maxLen;
        int dir1[] = {-2, 2}, dir2[] = {-1, 3}, loops[num * 2] = {0};
//
//        if (t == 132) {
//            for (auto &ro : road) {
//                cout << ro.second.id << " ";
//            }
////            return 1;
//        }
        //map may insert trash data
//        for (auto roadOne = road.begin(); roadOne != road.end();) {
//            if (roadOne->first != 5000)
//                roadOne = road.erase(roadOne);
//            else break;
//        }


        if (t > firstTimeCarOnRoad) {
            //TODO 1. run all car in road and label it with waiting or ending state

//        int *firstCarPosi;
//        for (pair<int, Road> ro:road) {
//            int ttt = 0;
            for (auto &ro : road) {
                //drive all car on current road to ending state, or label it as waiting state
                Road &currRoad = ro.second;
//                ttt++;
//                if(currRoad.id<5000)
//                    cout<<ttt<<" "<<currRoad.id<<endl;
//                try{
//                    assert(currRoad.id>=5000);
//                }catch{
//
//                };
                //because it has all been ending state, so need not clear posi in road
                if (currRoad.isDuplex) {//handling the second direction
                    deque<deque<Car>> &cars = currRoad.getCar(currRoad.from);
                    if (moveCarInCurrentRoad(channel, speed, maxLen, currRoad, currRoad.from, cars) == FINISH)
                        return FINISH;
                }
                //handling the first direction
                deque<deque<Car>> &cars = currRoad.getCar(currRoad.to);
                if (moveCarInCurrentRoad(channel, speed, maxLen, currRoad, currRoad.to, cars) == FINISH)
                    return FINISH;
            }


//            //map may insert trash data
//            for (auto roadOne = road.begin(); roadOne != road.end();) {
//                if (roadOne->first != 5000)
//                    roadOne = road.erase(roadOne);
//                else break;
//            }

            //TODO 2. drive all waiting cars


//        cout << "cross should have been sorted by id"
//                "and sorted by car Id; answer is map<int,Answer> &answer;"
//                "" <<
//             endl;

            while (true) {
                count = -1;
                for (auto &cr : cross) {
                    //handling each roads in this cross,if conflict, then break;
                    for (int roadId:cr.roadIds) {// 4 direction
                        if (roadId == -1)//road has no this direction
                            continue;
                        count++;
                        Road &currRoad = road[roadId];
                        roadDir = cr.roadId2Direc[roadId];

                        while (currRoad.getNumOfWaitCars(cr.id) != 0) {//0 represents has no wait car
                            //the first car
                            Car &firstCar = currRoad.getFirstWaitCar(cr.id);

                            deque<deque<Car>> &cars = currRoad.getCar(cr.id);// all cars in this road
                            deque<Car> &carInChannel = cars[firstCar.position[2]];

                            //the first wait car is not in the first position of this channel,so it can not go out of this road
                            if (firstCar.id != carInChannel[0].id) {
                                precedPosi = currRoad.getPrecedingPosiOfCar(cr.id, firstCar);
                                Car &blockCarInCurrRoad = carInChannel[precedPosi];
                                currRoad.moveCar(cr.id, firstCar,
                                                 firstCar.position[1] - blockCarInCurrRoad.position[1] - 1);
                                loops[count] = 0;
                                break;//loop to next road
                            }

                            //the first wait car is in the first position of this channel
                            nextRoadId = firstCar.answer.roadId[firstCar.position[0] + 1];
                            Road &nextRoad = road[nextRoadId];
                            nextDir = cr.roadId2Direc[nextRoadId];
                            direcDiff = roadDir - nextDir;

                            //isConflict
                            if (!getConflictState(road, nextDir, cr, direcDiff, dir2, dir1)) break;

                            //drive First Car to next road

                            //compute how long this car could go
                            s2 = computeRunDistanceOfCurrentCarInCross(firstCar, road, roadId, nextRoadId, s1, v2);
                            //check if has block car in next road, note this is go out this road, i.e. go away from current cross
                            otherCrossId = nextRoad.otherCrossId(cr.id);
                            findPositionToSetCar(nextRoadChannel, length, nextRoad, otherCrossId);
                            channel = firstCar.position[2];

                            //may be blocked to set first car,length and s2 is rest set could go
                            if (length < s2) {//block
                                if (blockCar.endWait) {//wait block
                                    firstCar.setWaitingState();
                                    loops[count]++;
                                    roadIdOfFirstCar = roadId;
                                } else {//end block
                                    loops[count] = 0;
                                    if (length != 0) {
                                        Car tmpCar = firstCar;
//                                        setDirToCar(roadDir, nextDir, tmpCar,roadId);
                                        currRoad.popFirstWaitCar(cr.id);
                                        nextRoad.receiveCarFromOtherRoad(otherCrossId, tmpCar, length,
                                                                         nextRoadChannel);
                                        roadIdOfFirstCar = nextRoad.id;
                                    } else {//car can not go out of current road
                                        currRoad.moveCar(cr.id, firstCar, firstCar.position[1]);
                                        roadIdOfFirstCar = roadId;
                                    }
                                }
                            } else {// no block,pop element and then push to next road,and set endWait=false
                                loops[count] = 0;
                                if (s2 != 0) {
                                    Car tmpCar = firstCar;
//                                    setDirToCar(roadDir, nextDir, tmpCar);
                                    currRoad.popFirstWaitCar(cr.id);
                                    nextRoad.receiveCarFromOtherRoad(otherCrossId, tmpCar, s2, nextRoadChannel);
                                    roadIdOfFirstCar = nextRoad.id;
                                } else {//car can not go out of current road
                                    currRoad.moveCar(cr.id, firstCar, firstCar.position[1]);
                                    roadIdOfFirstCar = roadId;
                                }
                            }

                            //let us see if it dead lock using our own method,this method is very inefficient,
                            //how much I want to know the better way to do it
                            if (loops[count] >= maxLoops) {
//                                for (auto roadOne = road.begin(); roadOne != road.end();) {
//                                    if (roadOne->first != 5000)
//                                        roadOne = road.erase(roadOne);
//                                    else break;
//                                }
                                cout << "\ndead lock: cross: " << cr.id << " road: "
                                     << currRoad.id << " car: " << firstCar.id
                                     << " time: " << t << " loops: " << loops[count] << endl;
                                lockInfo[0] = cr.id;
                                lockInfo[1] = currRoad.id;
                                lockInfo[2] = firstCar.id;
                                lockInfo[3] = t;
                                lockInfo[4] = loops[count];
//                                lockedCar = firstCar;
                                setDirToCar(roadDir, nextDir, firstCar, cr.id, roadId);
                                return DEAD_LOCK;//dead lock
                            }

                            //fisrtCar has not go out of this road
                            if (roadIdOfFirstCar == roadId) break;

                            //drive all car on current road to ending state, or label it as waiting state
                            //beacause may have some cars are waiting state in this road.
                            currRoad.clearWaitPosiInCurrChannel(cr.id, channel);
                            if (moveCarInCurrentChannel(channel, speed, maxLen, currRoad, cr.id, cars) == FINISH)
                                return FINISH;
                            currRoad.sortWaitPosi(cr.id);

//                            firstCar = currRoad.getFirstWaitCar(cr.id);

                        }
                    }
                }
//                int h = sum(loops, num * 2);
                if (sum(loops, num * 2) == 0) break; //all car is ending
            }
        }


        //TODO 3. drive car in garage, ordered by carId, all car on road are ending state
        if (!car.empty()) {
            for (auto it = car.begin(); it < car.end();) {
                Car &ca = *it;
                if (ca.answer.startTime == t) {
                    //drive this car

                    //check if it can go away from the garage
                    int roadId = ca.answer.roadId[0];
                    Road &currRoad = road[roadId];
//                    restMaxDis = currRoad.getMaxDisCanGo(otherCrossId);
                    crossId = currRoad.from == ca.from ? currRoad.to : currRoad.from;

                    findPositionToSetCar(channel, length, currRoad, crossId);

                    //move this car
                    speed = ca.speed > currRoad.speed ? currRoad.speed : ca.speed;


                    if (length < speed) {//block
                        if (length != 0) {
                            currRoad.receiveCar(crossId, ca, length, channel);
                            it = car.erase(it);
                        } else {// if length == 0 ,this car should wait for next time slop to go, so do not move it
                            ca.answer.startTime++;
                            it++;
                        }
                    } else {// no block,
                        currRoad.receiveCar(crossId, ca, speed, channel);
                        it = car.erase(it);
                    }
                } else it++;
            }
        }
        return UN_LOCK;
    }

    vector<Answer> &getAnswer() {
        //the startTime may be modified, so restore from attribute:planTime
        for (Answer &an:answer)
            an.startTime = an.planTime;
        return answer;
    }

    vector<Answer> getAnswerDataCopy() {
        //the startTime may be modified, so restore from attribute:planTime
        for (Answer &an:answer)
            an.startTime = an.planTime;
        return answer;
    }

    Car getLockedCar() {
        return lockedCar;
    }

    void setLockedCar(Car car) {
        lockedCar = car;
    }

    void setLockedCarDir(int dir) {
        lockedCar.direct[dir] = 1;
    }
    void restCarSub(){
        restCars--;
    }
    void pushAnswer(Answer a){
        answer.push_back(a);
    }
    int *getDeadLockInfo() {
        return lockInfo;
    }

    explicit Scheduler(int n) : restCars(n) {}

    Scheduler() {}

};


#endif //CODECRAFT_2019_SCHEDULER_H
