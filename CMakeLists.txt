cmake_minimum_required(VERSION 3.10)

project(app)

include_directories(${PROJECT_NAME}
    include
)

file(GLOB PROJECT_SRC src/*.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SRC})