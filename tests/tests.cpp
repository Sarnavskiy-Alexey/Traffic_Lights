#include "tests.hpp"

static void executeFirstPartOfTest(const int* objects, std::vector<TrafficLight>& all_lights)
{
    std::this_thread::sleep_for(std::chrono::seconds(10));
    addObjects(all_lights, objects, all_lights.size());
}

static void executeTest(const int* objects)
{
    executeFirstPartOfTest(objects, all_lights);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    for (size_t i = 0; i < all_lights.size(); i++)
    {
        ASSERT_EQ(all_lights[i].getWaitSize(), objects[i]);
    }

    reduceObjects(all_lights);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    for (const auto& tl : all_lights)
    {
        ASSERT_EQ(tl.getWaitSize(), 0);
    }
}

TEST(TestTrafficLight, Test_1_object)
{
    const int objects[12] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    executeTest(objects);
}

TEST(TestTrafficLight, Test_2_object)
{
    const int objects[12] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    executeTest(objects);
}

TEST(TestTrafficLight, Test_crossed_10_objects)
{
    const int objects[12] = { 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0 };
    executeTest(objects);
}

TEST(TestTrafficLight, Test_100_objects_2_TL)
{
    const int objects[12] = { 99, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    executeTest(objects);
}

TEST(TestTrafficLight, Test_120_objects_12_TL)
{
    const int objects[12] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
    executeTest(objects);
}
