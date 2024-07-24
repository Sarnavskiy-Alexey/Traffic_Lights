#pragma once

#include <iostream>
#include <fstream>
#include <gtest/gtest.h>
#include "TrafficLight.hpp"

void bufferHandler(std::vector<TrafficLight>& vec);

void initialize_crossroad(std::vector<TrafficLight>& vec);

void work(TrafficLight& TL);

void reduceObjectsInTL(TrafficLight& TL);
