#include <string>
#include <chrono>
#include <cassert>

#include "format.h"

using std::string;


/**
 * @brief Calculate the elapese time in hours minutes and seconds
 * @param seconds
 * @return String in the format 'HH:MM:SS'
 */
string Format::ElapsedTime(long seconds) { 

    assert(seconds < 36000);
    std::chrono::seconds totalSeconds {seconds};
    int hrs = 0;

    if (seconds >= 3600)
    {
      // Find the total number of hours
       hrs = std::chrono::duration_cast<std::chrono::hours>(totalSeconds).count();
    }    
 
    // Caluclate the remaining seconds
    seconds -= std::chrono::seconds(std::chrono::hours(hrs)).count();

    std::chrono::seconds rem {seconds};

    int min = 0;
    int sec = seconds;

    // Find the number of minutes 
    if (seconds >= 60)
    {
        min = std::chrono::duration_cast<std::chrono::minutes>(rem).count();
       // Calculate the number of seconds
        sec = seconds - std::chrono::seconds(std::chrono::minutes(min)).count();
    }


    // format the string
    char formatOutput[10];
    sprintf(formatOutput, "%02d:%02d:%02d", hrs, min, sec);

    std::string formatted = std::string(formatOutput);

    return formatted; 
 }
 