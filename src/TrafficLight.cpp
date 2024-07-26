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
    this->timer_green = 0;
    this->timer_red = 0;
    this->initialized = true;
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
    this->timer_green = TL.timer_green;
    this->timer_red = TL.timer_red;
    this->initialized = TL.initialized;
}

void TrafficLight::getMessage(const T_MESSAGE* message)
{
    if (message)
    {
        this->messages_mutex.lock();
        this->messages.emplace(*message);
        this->messages_mutex.unlock();
    }
}

void TrafficLight::sendMessage()
{
    T_MESSAGE pushed = { this->id, this->wait_size, this->color };
    message_buffer_mutex.lock();
    message_buffer.emplace(pushed);
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
        this->others.emplace_back(this->others_id[i], 0, red);
    }
}

void TrafficLight::setOthers_2(const std::vector<T_ID>& ids)
{
    this->others_2_id = ids;
    for (size_t i = 0; i < this->others_2_id.size(); i++)
    {
        this->others_2.emplace_back(this->others_2_id[i], 0, red);
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
    if (this->type == T_TL_TYPE::car)
    {
        if (this->color == red && (this->timer_red >= this->min_time_limit || this->initialized))
        {
            this->color = yellow;
            this->initialized = false;
        }
        else if (this->color == yellow)
        {
            this->color = green;
        }
    }
    else
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
    if (this->type == T_TL_TYPE::car)
    {
        if (this->color == green)
        {
            this->color = yellow;
        }
        else if (this->color == yellow)
        {
            this->color = red;
        }
    }
    else
    {
        this->color = red;
    }
}

void TrafficLight::messageHandler()
{
    T_MESSAGE front;
    while (this->messages.size() != 0)
    {
        this->messages_mutex.lock();
        front = this->messages.front();
        this->messages_mutex.unlock();
        this->messages_mutex.lock();
        this->messages.pop();
        this->messages_mutex.unlock();

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

void TrafficLight::fillTimer()
{
    if (this->timers_red.size() != this->others_2.size())
    {
        this->timers_red.clear();
        this->timers_red.resize(this->others_2.size());
    }

    if (this->timers_green.size() != this->others_2.size())
    {
        this->timers_green.clear();
        this->timers_green.resize(this->others_2.size());
    }
    
    for (size_t i = 0; i < this->others_2.size(); i++)
    {
        if (this->others_2[i].wait_size > 0 && this->others_2[i].color == red)
        {
            this->timers_red[i]++;
        }
        else if (this->others_2[i].color != red)
        {
            this->timers_red[i] = 0;
        }

        if (this->others_2[i].wait_size > 0 && this->others_2[i].color == green)
        {
            this->timers_green[i]++;
        }
        else if (this->others_2[i].color != green)
        {
            this->timers_green[i] = 0;
        }
    }
}

bool TrafficLight::check_red_in_others_2()
{
    for (size_t i = 0; i < this->timers_red.size(); i++)
    {
        if (this->timers_red[i] > this->max_time_limit && this->timers_red[i] > this->timer_red)
        {
            return true;
        }
    }

    return false;
}

bool TrafficLight::check_red_in_others_2_1()
{
    for (size_t i = 0; i < this->timers_red.size(); i++)
    {
        if (this->timer_red > this->max_time_limit && this->timers_red[i] > this->timer_red)
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
        if (o.wait_size > 0 && ((o.wait_size > this->wait_size || o.wait_size == this->wait_size && o.id < this->id) && o.color == red))
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
        if (o.wait_size > 0 && ((o.wait_size > this->wait_size || o.wait_size == this->wait_size && o.id < this->id) && o.color == red))
        {
            return true;
        }
    }
    return false;
}

bool TrafficLight::check_yellow_others_2()
{
    for (size_t i = 0; i < this->others_2.size(); i++)
    {
        if (this->others_2[i].color == yellow || this->others_2[i].color == green)
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
    this->sendMessage();

    while (true)
    {
        next_second += std::chrono::seconds(1);
        this->messageHandler();
        this->fillTimer();

        if (this->wait_size > 0)
        {
            if (this->color == green)
            {
                if (this->check_green_in_others_2() ||
                        ((this->check_red_in_others_2() ||
                            this->check_waiters_in_all_red_others_2()) &&
                        this->timer_green >= this->min_time_limit))
                {
                    prev = this->color;
                    this->setRed();
                }
            }
            else if (this->color == yellow && prev == green)
            {
                prev = this->color;
                this->setRed();
            }
            else if (this->color == red)
            {
                if (this->timer_red >= this->max_time_limit)
                {
                    if (!this->check_red_in_others_2_1())
                    {
                        if (!this->check_yellow_others_2())
                        {
                            prev = this->color;
                            setGreen();
                        }
                        else
                        {
                            prev = this->color;
                            setRed();
                        }
                    }
                }
                else
                {
                    if (!this->check_waiters_in_all_red_others_2())
                    {
                        if (!this->check_waiters_in_all_red_others())
                        {
                            if (!this->check_yellow_others_2())
                            {
                                prev = this->color;
                                setGreen();
                            }
                            else
                            {
                                prev = this->color;
                                setRed();
                            }
                        }
                    }
                }
            }
            else if (this->color == yellow && prev == red)
            {
                if (!this->check_yellow_others_2())
                {
                    prev = this->color;
                    setGreen();
                }
                else
                {
                    prev = this->color;
                    setRed();
                }
            }
        }
        else
        {
            prev = this->color;
            this->setRed();
        }

        this->sendMessage();

        /* Для формирования лога в консоли и отслеживания работы светофоров */
        if (this->wait_size > 0)
        {
            std::stringstream cout_str;
            cout_str << int(this->id) << " -> " << this->wait_size << " -> " << this->color << "\n";
            cout_buffer_mutex.lock();
            std::cout << cout_str.str();
            cout_buffer_mutex.unlock();
        }

        if (this->color == red)
        {
            this->timer_red++;
            this->timer_green = 0;
        }
        else if (this->color == green)
        {
            this->timer_green++;
            this->timer_red = 0;
        }

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
