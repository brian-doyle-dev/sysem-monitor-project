#include <thread>
#include "update.h"
#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;

  std::thread displayThread(NCursesDisplay::Display, std::ref(system), 50);
  std::thread updateThread(Update::Start, std::ref(system));
  
  displayThread.join();
  updateThread.join();
}
