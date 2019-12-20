//
// Created by Hoby Rakotoarivelo on 12/20/19.
//

#include "loader.h"

/* -------------------------------------------------------------------------- */
void Loader::loadFile(std::string const& input_file) {

  hid_t file = H5Fopen(input_file.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

  loadAttribute(file, "universe", "omega_b", 1);

  H5Fclose(file);
}

/* -------------------------------------------------------------------------- */
void Loader::storeFile(const std::string &output_file) {

  /*
  hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
  //H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
  hid_t file_id = H5Fcreate(output_file.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
*/

}


/* -------------------------------------------------------------------------- */
hid_t Loader::loadAttribute(hid_t const& in_file, std::string const& in_group_name,
                           std::string const& in_attrib_name, int num_elems) {

  hid_t group_id  = H5Gopen(in_file, in_group_name.c_str(), H5P_DEFAULT);
  hid_t attrib_id = H5Aopen(group_id, in_attrib_name.c_str(), H5P_DEFAULT);
  hid_t datatype  = H5Aget_type(attrib_id);
  auto const data_type_class = H5Tget_class(datatype);

  // set up and uncompressed data object
  void* data = nullptr;
  switch (data_type_class)
  {
    case H5T_INTEGER: data = new int[num_elems]; break;
    case H5T_FLOAT:   data = new double[num_elems]; break;
    case H5T_STRING:  data = new std::string[num_elems]; break;
    default: throw std::runtime_error("Bad group attribute type");
  }

  // read and store the group attribute
  herr_t status = H5Aread(attrib_id, datatype, data);

  if (data_type_class == H5T_FLOAT) {
    const auto* my_data = static_cast<double*>(data);

    for (int i = 0; i < num_elems; ++i) {
      std::cout << in_group_name <<"."<< in_attrib_name <<"["<< i <<"]: ";
      std::cout << my_data[i] << std::endl;
    }
  }

  H5Tclose(datatype);
  H5Aclose(attrib_id);
  H5Gclose(group_id);

  return status;
}

/* -------------------------------------------------------------------------- */

void Loader::storeAttribute(hid_t& in_file_id, std::string const& in_group_name,
                            std::string const& in_attrib_name, size_t num_elems,
                            void* data) {

  // create a HDF5 group
  hid_t group = H5Gcreate2(
    in_file_id, ("/" + in_group_name).c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT
  );

  auto write = [&](hid_t const& attrib, hid_t const& datatype, auto& item) -> hid_t {
    // export the given attribute data of arbitrary type
    if (datatype == H5T_NATIVE_INT) {
      return H5Awrite(attrib, datatype, (int *) data);
    } else if (datatype == H5T_NATIVE_DOUBLE) {
      return H5Awrite(attrib, datatype, (double*) data);
    } else {
      return H5Awrite(attrib, datatype, (char*) data);
    }
  };

  // write each attribute of the group
    hid_t dataspace;
    hid_t attrib;

    // retrieve attribute meta-data
    //std::string const& dataname = item.paramName;
    hid_t const datatype = H5T_NATIVE_DOUBLE;
    hsize_t const dims[] = { num_elems };
    bool const is_array  = (dims[0] > 1);

    // set right dataspace
    if (is_array) {
      dataspace = H5Screate(H5S_SIMPLE);
      H5Sset_extent_simple(dataspace, 1, dims, NULL);
      attrib = H5Acreate(group, in_attrib_name.c_str(), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT);
    }
    else {
      assert(datatype == H5T_NATIVE_INT or datatype == H5T_NATIVE_DOUBLE);
      dataspace = H5Screate(H5S_SCALAR);
      attrib = H5Acreate(group, in_attrib_name.c_str(), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT);
    }
    // write it
    herr_t const status = write(attrib, datatype, data);

    H5Aclose(attrib);
    H5Sclose(dataspace);
}
