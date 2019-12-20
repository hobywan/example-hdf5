
#include <iostream>
#include "loader.h"

int main() {

  std::cout << "Hello world" << std::endl;

  Loader loader;
  loader.loadFile("../data/sample.h5");



  return EXIT_SUCCESS;
}