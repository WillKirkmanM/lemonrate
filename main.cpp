#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <thread>
#include <chrono>
#include <iostream>

int main() {
  while (true) {
    std::string line;
    std::ifstream file("/sys/class/power_supply/AC/online");

    if (file.is_open()) {
      while (getline(file, line)) {
        std::string cmd = "xrandr | grep '*' | cut -d' ' -f9";
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) return 1;
        char buffer[128];
        std::string result = "";
        while(!feof(pipe)) {
          if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
        }
        pclose(pipe);
        std::istringstream iss(result);
        std::vector<std::string> rates((std::istream_iterator<std::string>(iss)),
                        std::istream_iterator<std::string>());
        std::string max_rate = *std::max_element(rates.begin(), rates.end());

        if (line == "1") {
          system(("xrandr --output eDP-1 --mode 1920x1080 --rate " + max_rate).c_str());
        } else {
          system("xrandr --output eDP-1 --mode 1920x1080 --rate 60");
        }
      }
      file.close();
    } else {
      std::cout << "Unable to open file" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  return 0;
}