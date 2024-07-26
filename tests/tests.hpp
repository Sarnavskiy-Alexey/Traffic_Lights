#pragma once

#include <iostream>
#include <fstream>
#include <gtest/gtest.h>
#include "TrafficLight.hpp"

void bufferHandler(std::vector<TrafficLight>& vec);

void initializeCrossroad(std::vector<TrafficLight>& vec);

void addObjects(std::vector<TrafficLight>& vec, const int* objects, const size_t size);

void reduceObjects(std::vector<TrafficLight>& vec);

void work(TrafficLight& TL);

void reduceObjectsInTL(TrafficLight& TL);

extern std::vector<std::thread> lights_threads;

extern std::vector<TrafficLight> all_lights;
