#include "TrafficLight.hpp"




#include <iostream>
#include <sstream>
std::mutex cerr_mutex;
std::mutex cout_buffer_mutex;





/* буфер отправки сообщений всем адресатам */
std::queue<T_MESSAGE> message_buffer;

/* мьютекс буфера */
std::mutex message_buffer_mutex;

TrafficLight::TrafficLight(const T_ID id, const T_TL_TYPE type)
{
    this->type = type;
    this->id = id;
    this->color = red;
    this->wait_size = 0;
}

TrafficLight::TrafficLight(const TrafficLight& TL)
{
    this->type = TL.type;
    this->id = TL.id;
    this->color = TL.color;
    this->wait_size = TL.wait_size;
    this->others = TL.others;
    this->others_id = TL.others_id;
    this->others_2 = TL.others_2;
    this->others_2_id = TL.others_2_id;
}

void TrafficLight::getMessage(const T_MESSAGE* message)
{
    if (message)
    {
        this->messages_mutex.lock();
        messages.push(*message);
        this->messages_mutex.unlock();
    }
}

void TrafficLight::sendMessage()
{
    message_buffer_mutex.lock();
    T_MESSAGE pushed = {this->id, this->wait_size, this->color};
    message_buffer.push(pushed);
    message_buffer_mutex.unlock();
}

T_ID TrafficLight::getID() const noexcept
{
    return this->id;
}

void TrafficLight::setOthers(const std::vector<T_ID>& ids)
{
    this->others_id = ids;
    for (size_t i = 0; i < this->others_id.size(); i++)
    {
        this->others.emplace_back(this->others_id[i], 0, green);
    }
}

void TrafficLight::setOthers_2(const std::vector<T_ID>& ids)
{
    this->others_2_id = ids;
    for (size_t i = 0; i < this->others_2_id.size(); i++)
    {
        this->others_2.emplace_back(this->others_2_id[i], 0, green);
    }
}

T_TL_COLOR TrafficLight::getColor() const noexcept
{
    return this->color;
}

size_t TrafficLight::getWaitSize() const noexcept
{
    return this->wait_size;
}

void TrafficLight::addNewObjectInWait() noexcept
{
    wait_size_mutex.lock();
    this->wait_size++;
    wait_size_mutex.unlock();
}

void TrafficLight::deleteObjectFromWait() noexcept
{
    wait_size_mutex.lock();
    this->wait_size--;
    wait_size_mutex.unlock();
}

void TrafficLight::setGreen()
{
    if (this->color == red)
    {
        this->color = yellow;
    }
    else if (this->color == yellow)
    {
        this->color = green;
    }
}

void TrafficLight::setYellow()
{
    if (this->type == T_TL_TYPE::car)
    {
        this->color = yellow;
    }
}

void TrafficLight::setRed()
{
    if (this->color == green && this->timer_green >= this->min_time_limit)
    {
        this->color = yellow;
    }
    else if (this->color == yellow)
    {
        this->color = red;
    }
}

void TrafficLight::messageHandler()
{
    if (!messages.empty())
    {
        messages_mutex.lock();
        T_MESSAGE front = messages.front();
        messages.pop();
        messages_mutex.unlock();

        for (auto& other : this->others)
        {
            if (front.sender_id == other.id)
            {
                other.color = front.color;
                other.wait_size = front.wait_size;
                break;
            }
        }
        for (auto& other : this->others_2)
        {
            if (front.sender_id == other.id)
            {
                other.color = front.color;
                other.wait_size = front.wait_size;
                break;
            }
        }
    }
}

bool TrafficLight::check_green_in_others_2()
{
    for (const auto& tl : this->others_2)
    {
        if (tl.color == green)
        {
            return true;
        }
    }
    return false;
}

bool TrafficLight::check_waiters_in_others_2()
{
    for (const auto& tl : this->others_2)
    {
        if (tl.wait_size > this->wait_size * 2)
        {
            return true;
        }
    }
    return false;
}

bool TrafficLight::check_red_in_others_2()
{
    static std::vector<int> timers(this->others_2.size());
    
    if (timers.size() != this->others_2.size())
    {
        timers.clear();
        timers.resize(this->others_2.size());
    }

    for (size_t i = 0; i < timers.size(); i++)
    {
        if (this->others_2[i].wait_size > 0)
        {
            timers[i]++;
        }
    }

    for (size_t i = 0; i < timers.size(); i++)
    {
        if (timers[i] > max_time_limit)
        {
            return true;
        }
    }

    return false;
}

bool TrafficLight::check_all_reds_in_others_2()
{
    for (auto& o : this->others_2)
    {
        if (o.color != red)
        {
            return false;
        }
    }
    return true;
}

bool TrafficLight::check_waiters_in_all_red_others_2()
{
    for (auto& o : this->others_2)
    {
        if (o.wait_size > this->wait_size || o.wait_size == this->wait_size && o.id < this->id)
        {
            return true;
        }
    }
    return false;
}

bool TrafficLight::check_waiters_in_all_red_others()
{
    for (auto& o : this->others)
    {
        if (o.wait_size > this->wait_size || o.wait_size == this->wait_size && o.id < this->id)
        {
            return true;
        }
    }
    return false;
}

void TrafficLight::work()
{
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    start = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point next_second(start);
    std::chrono::system_clock::time_point end;
    T_TL_COLOR prev = this->color;

    while (true)
    {
        next_second += std::chrono::seconds(1);
        this->sendMessage();
        this->messageHandler();

        if (this->color == green || this->color == yellow && prev == green)
        {
            if (this->wait_size == 0)
            {
                this->setRed();
            }
            else if (this->check_green_in_others_2())
            {
                this->setRed();
            }
            else if (this->check_waiters_in_others_2())
            {
                this->setRed();
            }
        }
        else if (this->color == red || this->color == yellow && prev == red)
        {
            if (!(this->wait_size == 0 || this->check_green_in_others_2() || this->check_waiters_in_others_2()) &&
                this->check_red_in_others_2())
            {
                if (!check_waiters_in_all_red_others_2() && !check_waiters_in_all_red_others())
                {
                    this->setGreen();
                }
            }
        }
        prev = this->color;

        cout_buffer_mutex.lock();
        std::cout << "Traffic Light #" << int(this->id) << " -> " << end << '\n';
        cout_buffer_mutex.unlock();

        end = std::chrono::system_clock::now();

        if (next_second > end)
        {
            std::this_thread::sleep_until(next_second);
        }
        else
        {
            cerr_mutex.lock();
            std::cerr << "Traffic Light #" << this->id << " work timeout\n";
            cerr_mutex.unlock();

            return;
        }
    }
}
