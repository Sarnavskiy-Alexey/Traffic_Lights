#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <chrono>
#include <thread>

typedef unsigned char T_ID;

enum class T_TL_TYPE
{
    pedestrian,
    car
};

enum T_TL_COLOR
{
    green,
    yellow,
    red
};

/* структура сообщения */
struct T_MESSAGE
{
    T_ID sender_id;
    size_t wait_size;
    T_TL_COLOR color;
};

/* структура для хранения данных о других светофорах */
struct T_OTHER_TL
{
    T_ID id;
    size_t wait_size;
    T_TL_COLOR color;
};

class TrafficLight
{
    /* максимальное время ожидания зеленого: 60 тактов */
    const int max_time_limit = 60;
    /* минимальное время работы зеленого: 15 тактов */
    const int min_time_limit = 15;

private:
    /* таймер работы красного */
    int timer_red;

    /* таймер работы зеленого */
    int timer_green;

    /* уникальный идентификатор светофора */
    T_ID id;

    /* очередь сообщений */ 
    std::queue<T_MESSAGE> messages;

    /* мьютекс для очереди сообщений */
    std::mutex messages_mutex;

    /* тип светофора: пешеходный, автомобильный */
    T_TL_TYPE type;

    /* цвет светофора */
    T_TL_COLOR color;

    /* количество объектов, стоящих на светофоре */
    size_t wait_size;

    /* мьютекс для изменения значения wait_size */
    std::mutex wait_size_mutex;

    /* вектор других светофоров, с которыми текущий может быть одновременно зеленым */
    std::vector<T_ID> others_id;
    std::vector<T_OTHER_TL> others;

    /* вектор других светофоров, с которыми текущий НЕ может быть одновременно зеленым */
    std::vector<T_ID> others_2_id;
    std::vector<T_OTHER_TL> others_2;

    /* метод отправки сообщения */
    void sendMessage();

    /* процедура перевода светофора в зеленый */
    void setGreen();

    /* процедура перевода светофора в желтый */
    void setYellow();

    /* процедура перевода светофора в красный */
    void setRed();

    /* проверка на наличие зеленого светофора среди others_2 */
    bool check_green_in_others_2();

    /* проверка на наличие светофора среди others_2 с большой очередью */
    bool check_waiters_in_others_2();

    /* проверка на ожидание зеленого среди others_2 */
    bool check_red_in_others_2();

    /* проверка на только красные светофоры среди others_2 */
    bool check_all_reds_in_others_2();

    /* проверка на количество ожидающих зеленого светофора среди others_2 */
    bool check_waiters_in_all_red_others_2();

    /* проверка на количество ожидающих зеленого светофора среди others */
    bool check_waiters_in_all_red_others();

    /* обработчик сообщений */
    void messageHandler();

public:
    /*  конструктор */
    TrafficLight(const T_ID id, const T_TL_TYPE type);

    /* конструктор копирования */
    TrafficLight(const TrafficLight& TL);

    /* добавление записи в очередь */
    void getMessage(const T_MESSAGE *message);

    /* метод добавления объекта в очередь ожидания на светофоре */
    void addNewObjectInWait() noexcept;

    /* метод удаления объекта из очереди ожидания на светофоре */
    void deleteObjectFromWait() noexcept;

    /* метод получения состояния */
    T_TL_COLOR getColor() const noexcept;

    /* главный метод работы светофора */
    void work();

    /* метод установки id светофоров, которые могут быть зелеными одновременно с текущим */
    void setOthers(const std::vector<T_ID> &ids);

    /* метод установки id светофоров, которые НЕ могут быть зелеными одновременно с текущим */
    void setOthers_2(const std::vector<T_ID>& ids);

    /* метод получения ID светофора */
    T_ID getID() const noexcept;

    /* метод получения ID светофора */
    size_t getWaitSize() const noexcept;
};

extern std::queue<T_MESSAGE> message_buffer;
extern std::mutex message_buffer_mutex;







extern std::mutex cerr_mutex;




