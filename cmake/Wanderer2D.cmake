add_subdirectory(src)
add_executable(wanderer2d ${WANDERER2D_SRC})

set_target_properties(wanderer2d PROPERTIES CXX_STANDARD 17)
set_target_properties(wanderer2d PROPERTIES CXX_STANDARD_REQUIRED ON)

target_precompile_headers(wanderer2d PUBLIC src/Globals.h)

find_package(SDL REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_image REQUIRED)

target_include_directories(wanderer2d PUBLIC ${SDL_INCLUDE_DIR} ${SDL_TTF_INCLUDE_DIRS} ${SDL_IMAGE_INCLUDE_DIRS})
target_link_libraries(wanderer2d ${SDL_LIBRARY} ${SDL_TTF_LIBRARIES} ${SDL_IMAGE_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
