#include "tests.hpp"

TEST(TestTrafficLight, Test_1_object)
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
        t.detach();
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    const int objects[12] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    for (size_t i = 0; i < 12; i++)
    {
        for (int j = 0; j < objects[i]; j++)
        {
            all_lights[i].addNewObjectInWait();
        }
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(5));
    for (size_t i = 0; i < all_lights.size(); i++)
    {
        ASSERT_EQ(all_lights[i].getWaitSize(), objects[i]);
    }

    bool end_flag = false;
    while (true)
    {
        end_flag = true;
        for (auto& tl : all_lights)
        {
            if (tl.getWaitSize())
            {
                end_flag = false;
            }
        }
        if (end_flag)
        {
            break;
        }
        for (auto& tl : all_lights)
        {
            if (tl.getWaitSize() && tl.getColor() == green)
            {
                reduceObjectsInTL(tl);
            }
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    for (const auto& tl : all_lights)
    {
        ASSERT_EQ(tl.getWaitSize(), 0);
    }
}