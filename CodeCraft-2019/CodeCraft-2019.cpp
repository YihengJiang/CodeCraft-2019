#include <iostream>
#include "elements/IO.h"
#include <string>
#include "Process.h"


using namespace std;

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

int main(int argc, char *argv[]) {

    if (argc < 5) {
        std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
        exit(1);
    }

    std::string carPath(argv[1]);
    std::string roadPath(argv[2]);
    std::string crossPath(argv[3]);
    std::string answerPath(argv[4]);

    std::cout << "carPath is " << carPath << std::endl;
    std::cout << "roadPath is " << roadPath << std::endl;
    std::cout << "crossPath is " << crossPath << std::endl;
    std::cout << "answerPath is " << answerPath << std::endl;

//	// TODO:read input filebuf
    IO io;
    vector<Car> car = io.read<Car>(carPath);
    map<int, Road> road = io.readMap<Road>(roadPath);
    vector<Cross> cross = io.read<Cross>(crossPath);

    sort(car.begin(), car.end(), compCar);
    sort(cross.begin(), cross.end(), compCross);


    map<int, Cross> crossMap;

    int n = 1;
    for (Cross &cr:cross) {
        cr.id2 = cr.id;
        cr.id = n;
        n++;
        crossMap.insert(pair<int, Cross>(cr.id2, cr));
    }
    n = 10000;
    for (Car &ca:car) {
        ca.id2 = ca.id;
        ca.id = n;
        n++;
        auto cr = crossMap.find(ca.from);
        ca.from = cr->second.id;
        cr = crossMap.find(ca.to);
        ca.to = cr->second.id;
//        carMap.insert(pair<int, Car>(ca.id2, ca));
    }
    vector<Road> road2;
    for (auto &ro:road) {
        auto cr = crossMap.find(ro.second.from);
        ro.second.from = cr->second.id;
        cr = crossMap.find(ro.second.to);
        ro.second.to = cr->second.id;
        road2.push_back(ro.second);
    }

//    io.writeOther<Car>(answerPath + "car", car);
//    io.writeOther<Cross>(answerPath + "caross", cross);
//    io.writeOther<Road>(answerPath + "road", road2);
//    car.clear();
//    cross.clear();
//    road.clear();
//    crossMap.clear();
//    road2.clear();
//
//
//    car = io.read<Car>(answerPath + "car");
//    road = io.readMap<Road>(answerPath + "road");
//    cross = io.read<Cross>(answerPath + "caross");


    //config_0~1 a little cars
    //config2~7 512 cars
    //config8~10 2048 cars


    // TODO:process

//
    Process process;
    vector<Answer> answer = process.getAllCarRoadAnswers(car, road, cross);



    // TODO:write output file
    io.write(answerPath, answer);


    return 0;
}