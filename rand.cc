#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>

int main() {
  std::mt19937 generator;
  std::uniform_int_distribution<std::int_fast8_t> distribution(-128, 127);

  for (std::size_t number_of_integers = 0; number_of_integers < 30;
       ++number_of_integers)
    std::cout << +distribution(generator) << "\n";
}
