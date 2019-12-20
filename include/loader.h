//
// Created by Hoby Rakotoarivelo on 12/20/19.
//

#pragma once
/* -------------------------------------------------------------------------- */
#include "hdf5.h"
#include <string>
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <mpi.h>
/* -------------------------------------------------------------------------- */
class Loader {

public:
   Loader() = default;
  ~Loader() = default;

  void loadFile(std::string const& input_file);
  void storeFile(std::string const& output_file);


  hid_t loadAttribute(hid_t const& in_file, std::string const& in_group_name,
                     std::string const& in_attrib_name, int num_elems);



  void storeAttribute(hid_t& in_file_id, std::string const& in_group_name,
                      std::string const& in_attrib_name, size_t num_elems, void* data);

};
