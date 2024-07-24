#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>
#include "TrafficLight.hpp"

//std::mutex cerr_mutex;

static void initialize_crossroad(std::vector<TrafficLight>& vec)
{
    for (T_ID i = 1; i <= 4; i++)
    {
        vec.emplace_back(i, T_TL_TYPE::car);
    }

    for (T_ID i = 5; i <= 12; i++)
    {
        vec.emplace_back(i, T_TL_TYPE::pedestrian);
    }

    /* выставка id остальных светофоров */
    const std::vector<T_ID> TL_0_green = { vec[2].getID(), vec[6].getID(), vec[7].getID() };
    const std::vector<T_ID> TL_0_red = { vec[1].getID(), vec[3].getID(), vec[4].getID(), vec[5].getID(), vec[8].getID(), vec[9].getID(), vec[10].getID(), vec[11].getID() };
    vec[0].setOthers(TL_0_green);
    vec[0].setOthers_2(TL_0_red);

    const std::vector<T_ID> TL_1_green = { vec[3].getID(), vec[8].getID(), vec[9].getID() };
    const std::vector<T_ID> TL_1_red = { vec[0].getID(), vec[2].getID(), vec[4].getID(), vec[5].getID(), vec[6].getID(), vec[7].getID(), vec[10].getID(), vec[11].getID() };
    vec[1].setOthers(TL_1_green);
    vec[1].setOthers_2(TL_1_red);

    const std::vector<T_ID> TL_2_green = { vec[0].getID(), vec[10].getID(), vec[11].getID() };
    const std::vector<T_ID> TL_2_red = { vec[1].getID(), vec[3].getID(), vec[4].getID(), vec[5].getID(), vec[6].getID(), vec[7].getID(), vec[8].getID(), vec[9].getID() };
    vec[2].setOthers(TL_2_green);
    vec[2].setOthers_2(TL_2_red);

    const std::vector<T_ID> TL_3_green = { vec[1].getID(), vec[4].getID(), vec[5].getID() };
    const std::vector<T_ID> TL_3_red = { vec[0].getID(), vec[2].getID(), vec[6].getID(), vec[7].getID(), vec[8].getID(), vec[9].getID(), vec[10].getID(), vec[11].getID() };
    vec[3].setOthers(TL_3_green);
    vec[3].setOthers_2(TL_3_red);

    const std::vector<T_ID> TL_4_green = { vec[3].getID(), vec[5].getID(), vec[6].getID(), vec[7].getID(), vec[8].getID(), vec[9].getID(), vec[10].getID(), vec[11].getID() };
    const std::vector<T_ID> TL_4_red = { vec[0].getID(), vec[1].getID(), vec[2].getID() };
    vec[4].setOthers(TL_4_green);
    vec[4].setOthers_2(TL_4_red);

    const std::vector<T_ID> TL_5_green = { vec[3].getID(), vec[4].getID(), vec[6].getID(), vec[7].getID(), vec[8].getID(), vec[9].getID(), vec[10].getID(), vec[11].getID() };
    const std::vector<T_ID> TL_5_red = { vec[0].getID(), vec[1].getID(), vec[2].getID() };
    vec[5].setOthers(TL_5_green);
    vec[5].setOthers_2(TL_5_red);

    const std::vector<T_ID> TL_6_green = { vec[0].getID(), vec[4].getID(), vec[5].getID(), vec[7].getID(), vec[8].getID(), vec[9].getID(), vec[10].getID(), vec[11].getID() };
    const std::vector<T_ID> TL_6_red = { vec[1].getID(), vec[2].getID(), vec[3].getID() };
    vec[6].setOthers(TL_6_green);
    vec[6].setOthers_2(TL_6_red);

    const std::vector<T_ID> TL_7_green = { vec[0].getID(), vec[4].getID(), vec[5].getID(), vec[6].getID(), vec[8].getID(), vec[9].getID(), vec[10].getID(), vec[11].getID() };
    const std::vector<T_ID> TL_7_red = { vec[1].getID(), vec[2].getID(), vec[3].getID() };
    vec[7].setOthers(TL_7_green);
    vec[7].setOthers_2(TL_7_red);

    const std::vector<T_ID> TL_8_green = { vec[1].getID(), vec[4].getID(), vec[5].getID(), vec[6].getID(), vec[7].getID(), vec[9].getID(), vec[10].getID(), vec[11].getID() };
    const std::vector<T_ID> TL_8_red = { vec[0].getID(), vec[2].getID(), vec[3].getID() };
    vec[8].setOthers(TL_8_green);
    vec[8].setOthers_2(TL_8_red);

    const std::vector<T_ID> TL_9_green = { vec[1].getID(), vec[4].getID(), vec[5].getID(), vec[6].getID(), vec[7].getID(), vec[8].getID(), vec[10].getID(), vec[11].getID() };
    const std::vector<T_ID> TL_9_red = { vec[0].getID(), vec[2].getID(), vec[3].getID() };
    vec[9].setOthers(TL_9_green);
    vec[9].setOthers_2(TL_9_red);

    const std::vector<T_ID> TL_10_green = { vec[2].getID(), vec[4].getID(), vec[5].getID(), vec[6].getID(), vec[7].getID(), vec[8].getID(), vec[9].getID(), vec[11].getID() };
    const std::vector<T_ID> TL_10_red = { vec[0].getID(), vec[1].getID(), vec[3].getID() };
    vec[10].setOthers(TL_10_green);
    vec[10].setOthers_2(TL_10_red);

    const std::vector<T_ID> TL_11_green = { vec[2].getID(), vec[4].getID(), vec[5].getID(), vec[6].getID(), vec[7].getID(), vec[8].getID(), vec[9].getID(), vec[10].getID() };
    const std::vector<T_ID> TL_11_red = { vec[0].getID(), vec[1].getID(), vec[3].getID() };
    vec[11].setOthers(TL_11_green);
    vec[11].setOthers_2(TL_11_red);
}

void work(TrafficLight& TL)
{
    TL.work();
}

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

int main()
{
    std::vector<TrafficLight> all_lights;
    initialize_crossroad(all_lights);

    std::vector<std::thread> lights_threads;
    for (size_t i = 0; i < all_lights.size(); i++)
    {
        lights_threads.emplace_back(work, std::ref(all_lights[i]));
    }
    lights_threads.emplace_back(bufferHandler, std::ref(all_lights));

    for (auto& t : lights_threads)
    {
        t.join();
    }

    return 0;
}
