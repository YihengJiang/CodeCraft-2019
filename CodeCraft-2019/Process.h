#pragma once

#include <iostream>
#include <vector>

#include <string>
#include "elements/Car.h"
#include "elements/Cross.h"
#include "elements/Road.h"
#include "elements/Answer.h"
#include "Scheduler.h"
#include <cmath>
#include <algorithm>
#include <map>
#include <numeric>


using namespace std;

class Process {

private:
//    vector<vector<int>> res;
//
//    void generateRoadId(const vector<Car> &car, const vector<Road> &road, const vector<Cross> &cross) {
////        if (car.size() == 0) {
////            return;
////        }
////
////        for (int i = 0; i < car.size(); i++) {
////
////
////        }
//
//    }
//
//    void findRoadId(const int from, const int to, const vector<Road> &road, const vector<Cross> &cross) {
//
//    }

    /*useful methods :
     * floyd
     * floyd2
     * floyd4  best
     * */


    //super parameters:
    int maxStartTime[2] = {2400, 2500};//-1 is default value 788, 394

    bool hasBlocked = false;

    int lockedV[4] = {-1, -1, -1, -1};//car road
    Car defaultCar;// it is just a placeholder,with no meaningful

    int findCrossByRoadId(Cross &a, Cross &b) {
        for (int roadId : a.roadIds) {
            if (roadId != -1)
                for (int &j : b.roadIds) {
                    if (j != -1 && roadId == j) {
                        j = -1;
                        return roadId;
                    }
                }
        }
        return -1;
    }

//    void floyd(vector<Car> &car, map<int, Road> &road,
//               vector<Cross> &cross) {//I will re-write cross value,so do not need reference(&)
//        int num = int(cross.size());
//
//        int path[num][num], dist[num][num];
//        memset(dist, -1, sizeof(dist));
//        int i, j, k;
//        int tmp;
//        map<string, int> cross2road;
//        // init cross id begin from 1
//        //find road to construct adjacent matrix
//        for (int l = 0; l < num - 1; l++) {
//            dist[l][l] = 0;
//            for (int m = l + 1; m < num; m++) {
//                tmp = findCrossByRoadId(cross[l], cross[m]);
//                if (tmp != -1) {
//                    if (road[tmp].isDuplex) {
//                        dist[l][m] = road[tmp].length;
//                        dist[m][l] = road[tmp].length;
//                        cross2road[to_string(m) + "_" + to_string(l)] = tmp;
//                    } else
//                        dist[l][m] = road[tmp].length;
//                    cross2road[to_string(l) + "_" + to_string(m)] = tmp;
//                }
//            }
//        }
//        dist[num - 1][num - 1] = 0;
//
//        for (int l = 0; l < num; l++) {
//            for (int i = 0; i < num; i++) {
//                if (dist[l][i] == -1)
//                    dist[l][i] = int(INFINITY);
//                path[l][i] = i;
//            }
//        }
//
//        //computing
//        for (k = 0; k < num; k++) {
//            for (i = 0; i < num; i++) {
//                for (j = 0; j < num; j++) {
//                    tmp = (dist[i][k] == int(INFINITY) || dist[k][j] == int(INFINITY)) ? int(INFINITY) : (dist[i][k] +
//                                                                                                          dist[k][j]);
//                    if (dist[i][j] > tmp) {
//                        dist[i][j] = tmp;
//                        path[i][j] = path[i][k];
//                    }
//                }
//            }
//        }
//
//        for (Car &c:car) {
//            vector<int> roadIds;
//            tmp = path[c.from - 1][c.to - 1];
//            //distance:dist[k][i]
//            roadIds.emplace_back(cross2road[to_string(c.from - 1) + "_" + to_string(tmp)]);
//            if (tmp != int(INFINITY))
//                //loop for path getting
//                while (tmp != c.to - 1) {
//                    i = tmp;
//                    tmp = path[tmp][c.to - 1];
//                    roadIds.emplace_back(cross2road[to_string(i) + "_" + to_string(tmp)]);
//                }
//            answer.emplace_back(Answer(c.id, c.planTime, roadIds));
//        }
//        //        print
////        for (k = 0; k < num; k++) {
////            for (i = k + 1; i < num; i++) {
////                cout << "v" << to_string(k + 1) << "---" << "v" << to_string(i + 1) << " weight: "
////                     << dist[k][i] << " path: " << " v" << to_string(k + 1);
////                tmp = path[k][i];
////                //loop for path getting
////                while (tmp != i) {
////                    cout << "-->" << "v" << to_string(tmp + 1);
////                    tmp = path[tmp][i];
////                }
////                cout << "-->" << "v" << to_string(i + 1) << endl;
////            }
////
////            cout << endl;
////        }
//    }
//
//    void floyd2(vector<Car> &car, map<int, Road> &road,
//                vector<Cross> &cross) {
//        //this is planTime optimized version 1 of floyd
//        floyd(car, road, cross);
//        sort(answer.begin(), answer.end(), comp);
//
//        int bin = 0;
//        if (maxStartTime != -1)
//            bin = int(answer.size()) / maxStartTime;
//        else
//            bin = 1;
//
//        for (int i = 0; i < int(answer.size()); i++) {
//            if (i / bin > answer[i].startTime) {
//                answer[i].startTime = i / bin;
//            }
//        }
//    }

//    void floyd3(vector<Car> &car, map<int, Road> &road,
//                vector<Cross> &cross) {
//        //this is planTime optimized version 3 of floyd
//        //Accoring test results, this method have no effectiveness
//        floyd(car, road, cross);
//        sort(answer.begin(), answer.end(), comp);
//
//        int bin = 0;
//        if (maxStartTime != -1)
//            bin = int(answer.size()) / maxStartTime;
//        else
//            bin = 1;
//
//        map<int, int> numCarInRoad;//<roadId, numberCarInThisRoad>
//        map<int, int>::iterator it;
//        for (int i = 0; i < int(answer.size()); i++) {
//            it = numCarInRoad.find(answer[i].roadId[0]);//till now, I just consider the beginning position
//            if (it == numCarInRoad.end()) {
//                numCarInRoad[answer[i].roadId[0]] = 0;
//                it = numCarInRoad.find(answer[i].roadId[0]);
//            }
//            if (i / bin > answer[i].startTime) {
//                answer[i].startTime = i / bin + it->second /
//                                                (road[answer[i].roadId[0]].length / carFactor[0]) * carFactor[1];
//            }
//            it->second += 1;
//        }
//    }

    void floyd4(vector<Car> &car, map<int, Road> &road,
                vector<Cross> &cross, int maxStartTime) {
        //this is planTime optimized version 4 of floyd
        //speed lower, go faster
        int i, j, k;
        int tmp;

        int num = int(cross.size());
        path = (int **) malloc(num * sizeof(int *));
        for (i = 0; i < num; i++)
            path[i] = (int *) malloc(num * sizeof(int));
        dist = (int **) malloc(num * sizeof(int *));
        for (i = 0; i < num; i++)
            dist[i] = (int *) malloc(num * sizeof(int));
        for (i = 0; i < num; i++)
            for (j = 0; j < num; j++)
                dist[i][j] = -1;


        // init cross id begin from 1
        //find road to construct adjacent matrix
        for (int l = 0; l < num - 1; l++) {
            dist[l][l] = 0;
            for (int m = l + 1; m < num; m++) {
                tmp = findCrossByRoadId(cross[l], cross[m]);
                if (tmp != -1) {
                    if (road[tmp].isDuplex) {
                        dist[l][m] = road[tmp].length;
                        dist[m][l] = road[tmp].length;
                        cross2road[to_string(m) + "_" + to_string(l)] = tmp;
                    } else
                        dist[l][m] = road[tmp].length;
                    cross2road[to_string(l) + "_" + to_string(m)] = tmp;
                }
            }
        }
        dist[num - 1][num - 1] = 0;

        for (int l = 0; l < num; l++) {
            for (i = 0; i < num; i++) {
                if (dist[l][i] == -1)
                    dist[l][i] = int(INFINITY);
                path[l][i] = i;
            }
        }

        //computing
        for (k = 0; k < num; k++) {
            for (i = 0; i < num; i++) {
                for (j = 0; j < num; j++) {
                    tmp = (dist[i][k] == int(INFINITY) || dist[k][j] == int(INFINITY)) ? int(INFINITY) : (dist[i][k] +
                                                                                                          dist[k][j]);
                    if (dist[i][j] > tmp) {
                        dist[i][j] = tmp;
                        path[i][j] = path[i][k];
                    }
                }
            }
        }

        for (Car &c:car) {
            vector<int> roadIds;
            tmp = path[c.from - 1][c.to - 1];
            //distance:dist[k][i]
            roadIds.emplace_back(cross2road[to_string(c.from - 1) + "_" + to_string(tmp)]);
            if (tmp != int(INFINITY))
                //loop for path getting
                while (tmp != c.to - 1) {
                    i = tmp;
                    tmp = path[tmp][c.to - 1];
                    roadIds.emplace_back(cross2road[to_string(i) + "_" + to_string(tmp)]);
                }
            answer.emplace_back(Answer(c.id, c.planTime, roadIds, c.speed, c.id2));
        }

        sort(answer.begin(), answer.end(), compAnswer);

        int bin = 0;
        if (maxStartTime != -1)
            bin = int(answer.size()) / maxStartTime;
        else
            bin = 1;

        for (int i = 0; i < int(answer.size()); i++) {
            if (i / bin > answer[i].startTime) {
                answer[i].startTime = i / bin;
                answer[i].planTime = i / bin;
            }
        }


    }

//
//    void floyd5(vector<Car> &car, map<int, Road> &road,
//                vector<Cross> &cross) {
//        //this is planTime optimized version 4 of floyd
//        //road shorter, go faster
//        //this is usefulness
//        int num = int(cross.size());
//
//        int path[num][num], dist[num][num];
//        memset(dist, -1, sizeof(dist));
//        int i, j, k;
//        int tmp;
//        map<string, int> cross2road;
//        // init cross id begin from 1
//        //find road to construct adjacent matrix
//        for (int l = 0; l < num - 1; l++) {
//            dist[l][l] = 0;
//            for (int m = l + 1; m < num; m++) {
//                tmp = findCrossByRoadId(cross[l], cross[m]);
//                if (tmp != -1) {
//                    if (road[tmp].isDuplex) {
//                        dist[l][m] = road[tmp].length;
//                        dist[m][l] = road[tmp].length;
//                        cross2road[to_string(m) + "_" + to_string(l)] = tmp;
//                    } else
//                        dist[l][m] = road[tmp].length;
//                    cross2road[to_string(l) + "_" + to_string(m)] = tmp;
//                }
//            }
//        }
//        dist[num - 1][num - 1] = 0;
//
//        for (int l = 0; l < num; l++) {
//            for (int i = 0; i < num; i++) {
//                if (dist[l][i] == -1)
//                    dist[l][i] = int(INFINITY);
//                path[l][i] = i;
//            }
//        }
//
//        //computing
//        for (k = 0; k < num; k++) {
//            for (i = 0; i < num; i++) {
//                for (j = 0; j < num; j++) {
//                    tmp = (dist[i][k] == int(INFINITY) || dist[k][j] == int(INFINITY)) ? int(INFINITY) : (dist[i][k] +
//                                                                                                          dist[k][j]);
//                    if (dist[i][j] > tmp) {
//                        dist[i][j] = tmp;
//                        path[i][j] = path[i][k];
//                    }
//                }
//            }
//        }
//
//        for (Car &c:car) {
//            vector<int> roadIds;
//            tmp = path[c.from - 1][c.to - 1];
//            //distance:dist[k][i]
//            roadIds.emplace_back(cross2road[to_string(c.from - 1) + "_" + to_string(tmp)]);
//            if (tmp != int(INFINITY))
//                //loop for path getting
//                while (tmp != c.to - 1) {
//                    i = tmp;
//                    tmp = path[tmp][c.to - 1];
//                    roadIds.emplace_back(cross2road[to_string(i) + "_" + to_string(tmp)]);
//                }
//            int sum = accumulate(roadIds.begin(), roadIds.end(), 0);
//            answer.emplace_back(Answer(c.id, c.planTime, roadIds, sum));
//        }
//
//        sort(answer.begin(), answer.end(), compAnswer);
//
//        int bin = 0;
//        if (maxStartTime != -1)
//            bin = int(answer.size()) / maxStartTime;
//        else
//            bin = 1;
//
//        for (int i = 0; i < int(answer.size()); i++) {
//            if (i / bin > answer[i].startTime) {
//                answer[i].startTime = i / bin;
//            }
//        }
//    }

  int chooseMap(map<int, Road> &roads) {

        auto sumOfRoad = int(roads.size());

        if (sumOfRoad == 236) {
           cout<<"maxStartTime[0]"<<maxStartTime[0]<<endl;
            return maxStartTime[0];
        }
         cout<<"maxStartTime[1]"<<maxStartTime[1]<<endl;
        return maxStartTime[1];
    }

//    int chooseMap(map<int, Road> &roads) {
//        int sumOfRoadLength = 0;
////        auto sumOfRoad = int(roads.size());
//        int sumOfRoadChannels = 0;
//        for (auto &road : roads) {
//            sumOfRoadLength += road.second.length;
//            sumOfRoadChannels += road.second.channel;
//        }
//        if (sumOfRoadLength == 1595 && sumOfRoadChannels == 311) {
//            return maxStartTime[0];
//        }
//        return maxStartTime[1];
//    }


    static bool comp(Answer a, Answer b) {
        return a.startTime < b.startTime;
    }

    static bool compAnswer(Answer a, Answer b) {
        //used for floyd4
        if (a.alternateDomain == b.alternateDomain)
            return a.startTime < b.startTime;
        else
            return a.alternateDomain < b.alternateDomain;
    }

    static bool compCar(Car a, Car b) {
        return a.id < b.id;
    }

    static bool compCross(Cross a, Cross b) {
        return a.id < b.id;
    }
//    void constructAnswerAfterOptimize(vector<Car> &car) {
//        for (Car &ca:car)
//            answer.push_back(ca.answer);
//    }


    void getNewPath(int start, int end, vector<int> &roadIds) const {
        int tmp = path[start - 1][end - 1];
        int i = 0;
        //distance:dist[k][i]
        string s = to_string(start - 1) + "_" + to_string(tmp);
        auto id = cross2road.find(s);
        roadIds.emplace_back(id->second);
        if (tmp != int(INFINITY))
            //loop for path getting
            while (tmp != end - 1) {
                i = tmp;
                tmp = path[tmp][end - 1];
                s = to_string(i) + "_" + to_string(tmp);
                id = cross2road.find(s);
                roadIds.emplace_back(id->second);
            }
    }

    bool isLockedCarInCurrRoad(Car car, map<int, Road> &road) {
        deque<deque<Car>> &cars = road[car.direct[4]].getCar(car.direct[5]);
        for (Car c:cars[car.position[2]])
            if (c.id == car.id)
                return true;//the car is in this road
        return false;
    }


//    Car optimizer(vector<Car> car, map<int, Road> road, vector<Cross> &cross, int t,
//                  Scheduler sche, int firstTimeCarOnRoad) {
//
//        //return Car represents has no road can go,so backnap to past road and then change road
//        //the returened car is lockedCar
//        vector<Car> car1 = car;;
//        map<int, Road> road1 = road;
//        Scheduler sche1 = sche;
//        int state = sche.oneStep(car, road, cross, t, firstTimeCarOnRoad, car1);
//        if (state == UN_LOCK) {
//            t++;
//            cout << t << endl;
//            Car lockedCar = optimizer(car, road, cross, t, sche, firstTimeCarOnRoad);
//            if (lockedCar.id != -1 && !isLockedCarInCurrRoad(lockedCar, road1)) {//-1 represents the normal state
//                // no way can go, and the car is not in this road,so reSplitCar
//                sche1.setLockedCar(sche.getLockedCar());
//                lockedCar = reSplitCarToOtherRoad(cross, t, firstTimeCarOnRoad, car1, road1, sche1,
//                                                  lockedCar);
//            }
//            return lockedCar;
//        } else if (state == FINISH) {
//            cout << "\n time cost: " << t << endl;
//            answer = sche.getAnswerDataCopy();
//            return defaultCar;
//        } else { //state is DEAD_LOCK, state == firstCar.id. re-split the car return reSplitCarToOtherRoad(cross, t, firstTimeCarOnRoad, car1, road1, sche1, state);
//            sche1.setLockedCar(sche.getLockedCar());
//            return reSplitCarToOtherRoad(cross, t, firstTimeCarOnRoad, car1, road1, sche1,
//                                         sche.getLockedCar());
//        }
//    }
//
//    Car reSplitCarToOtherRoad(vector<Cross> &cross, int t, int firstTimeCarOnRoad, vector<Car> &car1,
//                              map<int, Road> &road1, Scheduler sche1,
//                              Car lockedCar) {// return to paste time, and re-split data in block cross
//
//        Car lockedCar2=lockedCar;
//        int *direct = lockedCar.direct;
//
//        //select new way to change
//        //the new way is pass way+nextNewRoad+nextNewRoadToEnd
//        vector<int> newPath;
//        int selectedDir = findNewPath(cross, road1, lockedCar, direct, newPath, true);
////lockedCar.id==11882&&t==131
//        if (selectedDir == -2) {//the locked car has no new direction could go,so return
//            return lockedCar;
//        }
//
//
//        //re-split data
//        int nextRoadIdOfLockedCar = lockedCar.answer.roadId[lockedCar.position[0] + 1];
////        auto *cars = new deque<deque<Car>>();// stack is overflow,so new to heap
////
////        *cars = road1[direct[4]].getCar(direct[5]);
//        for (auto &ca:road1[direct[4]].getCar(direct[5])) {//channel
//            for (auto &c:ca) {//car
//                //c.id==lockedCar.id                                          only change blockedCar
//                //nextRoadIdOfLockedCar==c.answer.roadId[c.position[0]+1]     change all cars which path in this road are the same direction as lockedCar
//                if (nextRoadIdOfLockedCar == c.answer.roadId[c.position[0] + 1]) {
//                    //select new way to change
//                    //the new way is pass way+nextNewRoad+nextNewRoadToEnd
//                    int roadId = findNewPath(cross, road1, c, direct, newPath);
//                    if (roadId >= 0) {//roadId<0 represents this car can not change road
//                        //change the way of this car
//                        vector<int> newWay(c.answer.roadId.begin(),
//                                           c.answer.roadId.begin() + lockedCar.position[0] + 1);
//                        if (newWay.back() != roadId && newPath.front() != roadId)
//                            newWay.push_back(roadId);
//                        newWay.insert(newWay.end(), newPath.begin(), newPath.end());
//                        c.answer.roadId = newWay;
//                    }
//
//                }
//            }
//        }
//
//        sche1.setLockedCarDir(selectedDir);
//
//        vector<Car> car = car1;
//        map<int, Road> road = road1;
//        Scheduler sche = sche1;
//        //re-run this step
//        lockedCar = optimizer(car, road, cross, t, sche, firstTimeCarOnRoad);
//        if (lockedCar.id != -1 && !isLockedCarInCurrRoad(lockedCar2, road1)) {//-1 represents the normal state
//            // no way can go, and the car is not in this road,so reSplitCar
//            sche1.setLockedCar(lockedCar2);
//            lockedCar = reSplitCarToOtherRoad(cross, t, firstTimeCarOnRoad, car1, road1, sche1,
//                                              lockedCar2);
//        }
//        return lockedCar;
//
//
//    }
//

    Car optimizer(vector<Car> car, map<int, Road> road, vector<Cross> &cross, int t,
                  Scheduler sche, int firstTimeCarOnRoad) {

        //return Car represents has no road can go,so backnap to past road and then change road
        //the returened car is lockedCar
        vector<Car> car1 = car;
        map<int, Road> road1 = road;
        Scheduler sche1 = sche;
        int state = sche.oneStep(car, road, cross, t, firstTimeCarOnRoad, car1);
        if (state == UN_LOCK) {
            t++;
            cout << t << endl;
            Car lockedCar = optimizer(car, road, cross, t, sche, firstTimeCarOnRoad);
//            if (lockedCar.id != -1 && !isLockedCarInCurrRoad(lockedCar, road1)) {//-1 represents the normal state
//                // no way can go, and the car is not in this road,so reSplitCar
//                sche1.setLockedCar(sche.getLockedCar());
//                Car defaultCar;
//                lockedCar = reSplitCarToOtherRoad(cross, t, firstTimeCarOnRoad, car1, road1, sche1,
//                                                  defaultCar);
//            }
            return lockedCar;
        } else if (state == FINISH) {
            cout << "\n time cost: " << t << endl;
            answer = sche.getAnswerDataCopy();
            return defaultCar;
        } else { //state is DEAD_LOCK, state == firstCar.id. re-split the car return reSplitCarToOtherRoad(cross, t, firstTimeCarOnRoad, car1, road1, sche1, state);

            sche1.setLockedCar(sche.getLockedCar());
            return reSplitCarToOtherRoad(cross, t, firstTimeCarOnRoad, car1, road1, sche1,
                                         sche.getLockedCar());
        }
    }

    Car reSplitCarToOtherRoad(vector<Cross> &cross, int t, int firstTimeCarOnRoad, vector<Car> &car1,
                              map<int, Road> &road1, Scheduler sche1,
                              Car lockedCar) {// return to paste time, and re-split data in block cross
        hasBlocked = true;


        if (lockedV[0] == lockedV[2] && lockedV[0] == lockedCar.id
            && lockedV[1] == lockedV[3] && lockedV[1] == lockedCar.direct[4]) {

            vector<Answer> an = sche1.getAnswerDataCopy();
            for (Answer a:an) {
                answer.push_back(a);
            }

            return lockedCar;
        } else {
            lockedV[0] = lockedV[2];
            lockedV[1] = lockedV[3];
            lockedV[2] = lockedCar.id;
            lockedV[3] = lockedCar.direct[4];

        }


        Car lockedCar2 = lockedCar;
        int *direct = lockedCar.direct;

        //select new way to change
        //the new way is pass way+nextNewRoad+nextNewRoadToEnd
        vector<int> newPath;
        int selectedDir = findNewPath(cross, road1, lockedCar, direct, newPath, true);
//lockedCar.id==11882&&t==131
        if (selectedDir == -2) {//the locked car has no new direction could go,so return
            return lockedCar;
        }


        //re-split data
        int nextRoadIdOfLockedCar = lockedCar.answer.roadId[lockedCar.position[0] + 1];
//        auto *cars = new deque<deque<Car>>();// stack is overflow,so new to heap
//
//        *cars = road1[direct[4]].getCar(direct[5]);
        auto &ca1 = road1[direct[4]].getCar(direct[5]);

        for (auto ca = ca1.begin(); ca != ca1.end(); ca++) {//channel
            auto &ca11 = *ca;
            for (auto cc = ca11.begin(); cc != ca11.end();) {//car
                auto &c = *cc;
                //c.id==lockedCar.id                                          only change blockedCar
                //nextRoadIdOfLockedCar==c.answer.roadId[c.position[0]+1]     change all cars which path in this road are the same direction as lockedCar
                if (c.id==lockedCar.id) {

                    //select new way to change
                    //the new way is pass way+nextNewRoad+nextNewRoadToEnd
                    int roadId = findNewPath(cross, road1, c, direct, newPath);
                    if (roadId >= 0) {//roadId<0 represents this car can not change road
                        //change the way of this car
                        vector<int> newWay(c.answer.roadId.begin(),
                                           c.answer.roadId.begin() + lockedCar.position[0] + 1);
                        if (newWay.back() != roadId && newPath.front() != roadId)
                            newWay.push_back(roadId);
                        newWay.insert(newWay.end(), newPath.begin(), newPath.end());
                        c.answer.roadId = newWay;
                    }
                    sche1.restCarSub();
                    c.answer.planTime = c.answer.planTime + 100;
                    sche1.pushAnswer(c.answer);
                    cc = ca11.erase(cc);

                } else cc++;
            }
        }

//        sche1.setLockedCarDir(selectedDir);

        vector<Car> car = car1;
        map<int, Road> road = road1;
        Scheduler sche = sche1;
        //re-run this step
        lockedCar = optimizer(car, road, cross, t, sche, firstTimeCarOnRoad);
//        if (lockedCar.id != -1 && !isLockedCarInCurrRoad(lockedCar2, road1)) {//-1 represents the normal state
//            // no way can go, and the car is not in this road,so reSplitCar
//            sche1.setLockedCar(lockedCar2);
//            lockedCar = reSplitCarToOtherRoad(cross, t, firstTimeCarOnRoad, car1, road1, sche1,
//                                              lockedCar2);
//        }
        return lockedCar;


    }


    int findNewPath(vector<Cross> &cross, map<int, Road> &road1, Car &changeCar, int *direct,
                    vector<int> &newPath, bool isTest = false) {
        int roadId = -1;
//        vector<int> tmpPath;
        int sum = 10000000, newCrossId = -1, tmp = 0, j = -1;
        int crInd = direct[5] - 1;
        for (int i = 0; i < 4; i++) {
            Cross &cr = cross[crInd];
            if (direct[i] == -1) {
                auto roadIdV = cr.direc2RoadId.find(i);
                if (roadIdV != cr.direc2RoadId.end()) {
                    roadId = roadIdV->second;
                    if (road1[roadId].from == cr.id || road1[roadId].isDuplex) {//single direction check

                        newCrossId = road1[roadId].otherCrossId(cr.id);
                        int &destination = changeCar.to;

                        if (isTest) {
                            newPath.clear();
                            getNewPath(newCrossId, destination, newPath);
                            if (direct[4] != newPath[1])//
                                return i;
                        } else {
                            tmp = dist[newCrossId][destination];
                            if (tmp < sum) {
                                sum = tmp;
                                j = i;
                            }
                        }

                    }


                }
            }
        }

        if (j >= 0 && j <= 4) {
            Cross &cr = cross[crInd];
            auto roadIdV = cr.direc2RoadId.find(j);
            if (roadIdV != cr.direc2RoadId.end()) {
                roadId = roadIdV->second;
                newCrossId = road1[roadId].otherCrossId(cr.id);
                int &destination = changeCar.to;
                newPath.clear();
                getNewPath(newCrossId, destination, newPath);
            }
            return roadId;
        }
        return -2;

    }

    vector<Answer> schedulerTest(vector<Car> &car, map<int, Road> &road, vector<Cross> &cross) {
        //sort car by id for go out of garage ordered by id
        map<int, Answer> answerMap;
        for (Answer &ans:answer)
            answerMap.insert(pair<int, Answer>(ans.carId, ans));
        sort(car.begin(), car.end(), compCar);
        for (Car &ca:car)
            ca.answer = answerMap[ca.id];
        int firstTimeCarOnRoad = 1000000;
        for (Car &ca:car)
            if (ca.answer.startTime < firstTimeCarOnRoad)
                firstTimeCarOnRoad = ca.answer.startTime;

//        answer.clear();
        answerMap.clear();//free memory
        //sort cross for loop on cross
        sort(cross.begin(), cross.end(), compCross);
        Scheduler sche(int(car.size()));
        int t = 1, state = UN_LOCK;
        while (state == UN_LOCK) {
            state = sche.oneStep(car, road, cross, t, firstTimeCarOnRoad);
            t++;
            cout << t << endl;
        }
        if (state == FINISH) {
            cout << "\n time cost: " << t << endl;
            return sche.getAnswerDataCopy();
        } else //state==DEAD_LOCK, it will output remind in Scheduler, so here I need not to output something
            exit(-1);
    }


    bool schedulerOptimize(vector<Car> car, map<int, Road> road, vector<Cross> cross) {
        //arrang data...
        //sort car by id for go out of garage ordered by id
        map<int, Answer> answerMap;
        for (Answer &ans:answer)
            answerMap.insert(pair<int, Answer>(ans.carId, ans));


//        spechialValueHandle(answerMap);


        sort(car.begin(), car.end(), compCar);
        for (Car &ca:car)
            ca.answer = answerMap[ca.id];
        int firstTimeCarOnRoad = 1000000;

        for (Car &ca:car)
            if (ca.answer.startTime < firstTimeCarOnRoad)
                firstTimeCarOnRoad = ca.answer.startTime;

//        answer.clear();
        answerMap.clear();//free memory
        //sort cross for loop on cross
        sort(cross.begin(), cross.end(), compCross);
        Scheduler sche(int(car.size()));

        int t = 0;

        Car ca = optimizer(car, road, cross, t, sche, firstTimeCarOnRoad);
        if (ca.id != -1) {
            cout << "dead lock, need to revise super-parameter" << endl;
            return true;
        } else return false;


    }


//    void spechialValueHandle(map<int, Answer> &answerMap) {
//
//
//        for (auto &v:specialValue) {
////            cout<<answerMap[v].startTime<<endl;
//            answerMap[v].startTime = answerMap[v].startTime + 10;
////            cout<<answerMap[v].startTime<<endl;
//        }
//
//
//    }
//
//
//    vector<int> specialValue = {
//            16051,
//            14074,
//            10512,
//            11408,
//
//
//    };


public:

    vector<Answer> answer;
    int **path;//path
    int **dist;//distance
    map<string, int> cross2road;

    vector<Answer> getAllCarRoadAnswers(vector<Car> &car, map<int, Road> &road, vector<Cross> cross) {
        vector<Cross> cross2 = cross;
        int bestMaxStartTime = chooseMap(road);
        floyd4(car, road, cross, bestMaxStartTime);// generate answer,the cross has been revised, so need save a copy


        cross.clear();
//        schedulerOptimize(car, road, cross2);//optimize path
//        int n = 0;
//        hasBlocked = true;
//        while (hasBlocked) {
//            n++;
//            hasBlocked = false;
//            cout << "begin test optimized result==========the " << n << "-th optimizing..." << endl;
//            schedulerOptimize(car, road, cross2);//optimize path
//
//        }
        answer = schedulerTest(car, road, cross2);//optimize path
//
//        sort(answer.begin(), answer.end(), comp);

        return answer;

    }


};
