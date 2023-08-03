#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <ctime>  
#include <iostream>
#include <functional>
#include <future>
#include <chrono>
#include <iomanip>
#include <thread>


using namespace std::chrono_literals;


namespace Utils {

//logging
class Log {

    private:

        Log(){ };

    public:  

        static inline std::stringstream current;

    //Log size of obj in bytes 


        template <typename T> 
            static inline void get_size(T obj) { logMulti("Object bytes: ", sizeof(obj)); } 

    //Log generic


        template <typename T> 
            static inline void write (T arg)
            {
                
                std::time_t time_stamp = std::time(nullptr);

                std::ofstream log;

                log.open("Logs.txt", std::ofstream::app | std::ofstream::out);

                log << arg << "  ::Time stamp: " << std::ctime(&time_stamp) << "\n";  

                current << log.rdbuf();     

                log.close(); 

                std::cout << arg << std::endl;
            }

        template <typename T, typename T2 = T> 
            static inline void logMulti (T argA, T2 argB = T2())
            {
             
                std::time_t time_stamp = std::time(nullptr);

                std::ofstream log;

                log.open("Logs.txt", std::ofstream::app | std::ofstream::out);

                log << argA << " " << argB << "  ::Time stamp: " << std::ctime(&time_stamp) << "\n";  

                current << log.rdbuf();   

                log.close(); 
            }
        
};

//time
class Time {

    public:

        static inline std::atomic<bool> exitFlag{false};

        float m_lastTime;

        Time(float t = 0.00f): 
            m_lastTime(0.0f), 
            m_time(t)
        {
            srand(time(0));
        }

        operator float() const { return m_time; }

        inline float GetSeconds() const { return m_time; }
        
        inline float GetMilliseconds() const { return m_time * 1000; }


        static inline void RunClock(int milliseconds)
        {
        
            const auto timer_duration = std::chrono::minutes(milliseconds); //90mins
        
            const auto start = std::chrono::steady_clock::now();
        
            std::chrono::duration<double> time_left = timer_duration - (std::chrono::steady_clock::now() - start);
        
            while (time_left > 0s)
            {
        
                const auto hrs = std::chrono::duration_cast<std::chrono::hours>(time_left);
                const auto mins = std::chrono::duration_cast<std::chrono::minutes>(time_left - hrs);
                const auto secs = std::chrono::duration_cast<std::chrono::seconds>(time_left - hrs - mins);
        
                std::cout << std::setfill('0') << std::setw(2) << hrs.count() << ":"
                            << std::setfill('0') << std::setw(2) << mins.count() << ":"
                            << std::setfill('0') << std::setw(2) << secs.count() << "\n";
        
                std::this_thread::sleep_for(1s);
        
                time_left = timer_duration - (std::chrono::steady_clock::now() - start);
        
            }
        }

        static inline void DelayedCall(int milliseconds, std::function<void()> &&fn_ptr)
        {
        
            std::this_thread::sleep_for(std::chrono::milliseconds((milliseconds)));
            
            fn_ptr();
        
        }

  
        static inline void SetInterval(int milliseconds, std::function<void()> &&fn_ptr, std::mutex& m)
        {
      
         exitFlag = false;
         
          std::thread ([=, &m]
          (){
      
              m.lock();
      
              while(!exitFlag.load())
              {
      
                  if (exitFlag.load())
                      return;
      
                  std::this_thread::sleep_for(std::chrono::milliseconds((milliseconds)));
          
                  if (exitFlag.load())
                      return;
      
                  fn_ptr();
      
              }
      
              m.unlock();
      
          }).detach();     
      
      }

        void Update();
        void RunClock(int milliseconds);

    private:

        float m_time;
        
  };

}
