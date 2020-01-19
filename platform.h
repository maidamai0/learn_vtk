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