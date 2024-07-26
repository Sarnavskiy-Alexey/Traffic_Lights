#include "tests.hpp"

std::vector<TrafficLight> all_lights;
std::vector<std::thread> lights_threads;

void initializeCrossroad(std::vector<TrafficLight>& vec)
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

void addObjects(std::vector<TrafficLight>& vec, const int* objects, const size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        for (int j = 0; j < objects[i]; j++)
        {
            vec[i].addNewObjectInWait();
        }
    }
}

void reduceObjects(std::vector<TrafficLight>& vec)
{
    bool end_flag = false;
    while (true)
    {
        end_flag = true;
        for (auto& tl : vec)
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
        for (auto& tl : vec)
        {
            if (tl.getWaitSize() && tl.getColor() == green)
            {
                reduceObjectsInTL(tl);
            }
        }
    }
}

void work(TrafficLight& TL)
{
    TL.work();
}

void reduceObjectsInTL(TrafficLight& TL)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    TL.deleteObjectFromWait();
}

int main()
{
    initializeCrossroad(all_lights);
    for (size_t i = 0; i < 12; i++)
    {
        lights_threads.emplace_back(work, std::ref(all_lights[i]));
    }
    
    lights_threads.emplace_back(bufferHandler, std::ref(all_lights));

    for (auto& t : lights_threads)
    {
        t.detach();
    }

    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}