#include <chrono>
#include <iostream>
#include <thread>

#include <conway.hpp>

int main() {
  conway::grid_t<80, 80> grid = conway::make_grid();

  // Tournage de serviettes en cours...
  for (std::size_t i = 0; i < 400; i++) {
    std::cout << conway::print(grid);
    grid = conway::conway(grid);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }

  return 0;
}

