file(GLOB source_files "src/*.cpp")

set(khk_header_files "include/")
set(b2d_header_files "../third_party/box2d/")
set(raylib_header_files "../third_party/raylib/")
set(tinyxml2_header_files "../third_party/tinyxml2/")
set(confini_header_files "../third_party/confini/")
set(json_header_files "../third_party/json/")

ADD_LIBRARY(khkFramework STATIC ${source_files})

target_include_directories(khkFramework PUBLIC
  ${khk_header_files}
  ${b2d_header_files}
  ${raylib_header_files}
  ${tinyxml2_header_files}
  ${confini_header_files}
  ${json_header_files}
  )
