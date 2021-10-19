#include <thread>

#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;

  std::thread displayThread(NCursesDisplay::Display, std::ref(system), 10);
  displayThread.join();

}

#if 0
  System system;
  NCursesDisplay::Display(system);

  std::thread displayThread(NCursesDisplay::Display, system);

  displayThread.join();

#endif