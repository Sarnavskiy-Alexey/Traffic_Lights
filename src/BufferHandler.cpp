#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <queue>
#include <mutex>

#include "TrafficLight.hpp"

void bufferHandler(std::vector<TrafficLight>& vec)
{
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point next_time(start);
    std::chrono::system_clock::time_point end;

    while (true)
    {
        next_time += std::chrono::milliseconds(200);
        if (!message_buffer.empty())
        {
            message_buffer_mutex.lock();
            T_MESSAGE front = message_buffer.front();
            for (auto& tl : vec)
            {
                if (tl.getID() != front.sender_id)
                {
                    tl.getMessage(&front);
                }
            }
            message_buffer.pop();
            message_buffer_mutex.unlock();
        }

        end = std::chrono::system_clock::now();

        if (next_time > end)
        {
            std::this_thread::sleep_until(next_time);
        }
        else
        {
            cerr_mutex.lock();
            std::cerr << "Data exchange timeout\n";
            cerr_mutex.unlock();
            return;
        }
    }
}