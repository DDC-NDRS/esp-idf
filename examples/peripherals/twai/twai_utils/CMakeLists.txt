# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.22)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# "Trim" the build. Include the minimal set of components, main, and anything it depends on.
idf_build_set_property(MINIMAL_BUILD ON)
project(twai_utils)

# Define the ELF file path
set(ELF_FILE "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf")

# Combined post-build commands
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}.elf             # Refers to the main application target
    POST_BUILD

    # POST_COMMAND_1: Create list file
    COMMAND ${CMAKE_COMMAND} -E echo "##### Post build step: Build completed for ${CMAKE_PROJECT_NAME} #####"
    COMMAND ${CMAKE_OBJDUMP} --source --line-numbers -d ${ELF_FILE} > "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.lst"
)
