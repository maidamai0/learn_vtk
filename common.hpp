#pragma once

/**
 * @file common.hpp
 * @author tonghao.yuan (tonghao.yuan@csdental.com)
 * @brief
 * @version 0.1
 * @date 2020-04-28
 *
 * @copyright Copyright (c) 2020 Carestream Dental,LLC. All Rights Reserved
 *
 */

#ifndef DATA_PATH
#  define DATA_PATH ""
#endif

#include <string>

/**
 * @brief Get the data full path
 *
 * @warning define DATA_PATH in cmake before use this header, or you will
 * propably get the wrong path
 *
 * @param shader_name       data file name
 * @return std::string      data file full path
 */
auto get_data_full_path(std::string&& data_name) -> std::string {
  return std::string(DATA_PATH) + data_name;
}