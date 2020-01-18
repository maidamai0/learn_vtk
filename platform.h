#if (_MSC_VER >= 1920)
#  include <filesystem>
namespace fs = std::filesystem;
#else
#  include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif