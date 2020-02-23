/**
 * @file platform.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief common tools and some cross platfrom/compiler things.
 * @version 0.1
 * @date 2020-02-23
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "array"

#if (_MSC_VER >= 1920)
#  include <filesystem>
namespace fs = std::filesystem;
#else
#  include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

/**
 * @brief define boolean value for vtk
see vtkABI.h for detail
 *
 */
#define VTK_TRUE 1
#define VTK_FALSE 0

using pointType = std::array<double, 3>;
auto operator<<(std::ostream& os, const pointType& p) -> std::ostream& {
  os << "[" << p[0];
  for (std::size_t i = 1; i < p.size(); ++i) {
    os << "," << p[i];
  }
  os << "]";

  return os;
}