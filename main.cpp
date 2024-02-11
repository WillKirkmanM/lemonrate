#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <iterator>
#include <thread>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>


std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main() {
  while (true) {
    std::string line;
    std::ifstream file("/sys/class/power_supply/ACAD/online");

    if (file.is_open()) {
      while (getline(file, line)) {
        
      std::string max_rate = exec("xrandr | awk '{print $2}' | sort -rn | sed q");
        
       if (line == "1") {
          system(("xrandr --rate " + max_rate).c_str());
        } else {
          system("xrandr --rate 60");
        }
      }
      file.close();
    } else {
      std::cout << "Unable to open file" << std::endl;
    }

  }

  return 0;
}
