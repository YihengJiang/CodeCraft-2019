//
// Created by jyh on 19-3-20.
//
#ifndef CODECRAFT_2019_IO_H
#define CODECRAFT_2019_IO_H

#include <iostream>
#include "Car.h"
#include "Cross.h"
#include "Road.h"
#include "Answer.h"
#include <vector>
#include <cassert>
#include <fstream>
#include <map>

using namespace std;

class IO {

public:

    template<typename T>
    vector<T> read(string dir) {//T: Car,Cross,Road
        vector<T> var;
        cout << dir;
        ifstream file(dir);
        string line;
        assert(file.is_open());
        assert(getline(file, line));
//		cout << line << endl;//first line
        while (getline(file, line)) {
//			cout << line << endl;
            var.emplace_back(T(line));
        }
        file.close();
        return var;
    }
template<typename T>
    map<int, T> readMap(string dir) {//T: Car,Cross,Road
        map<int, T> var;
        cout << dir;
        ifstream file(dir);
        string line;
        assert(file.is_open());
        assert(getline(file, line));
//		cout << line << endl;//first line
        while (getline(file, line)) {
//			cout << line << endl;
            T r(line);
            var.insert(pair<int, T>(r.id, r));
        }
        file.close();
        return var;
    }

    void write(string dir, vector<Answer> &answer) {
        //    #(carId,StartTime,RoadId...)
        //    (1001, 1, 501, 502, 503, 516, 506, 505, 518, 508, 509, 524)
        //    ....
        ofstream file(dir);
        //title:
        string line = "#(carId,StartTime,RoadId...)";
        file << line << "\n";
        for (Answer &i : answer) {
            file << i.anser2string() << "\n";
        }

        file << flush;
        file.close();

    }
template <typename T>
    void writeOther(string dir, vector<T> &answer) {
        //    #(carId,StartTime,RoadId...)
        //    (1001, 1, 501, 502, 503, 516, 506, 505, 518, 508, 509, 524)
        //    ....
        ofstream file(dir);
        //title:
        string line = "#.......";
        file << line << "\n";
        for (T &i : answer) {
            file << i.anser2string() << "\n";
        }

        file << flush;
        file.close();

    }
};


#endif //CODECRAFT_2019_IO_H
