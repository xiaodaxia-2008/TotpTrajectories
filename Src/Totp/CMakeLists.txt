add_library(Totp Path.cpp Trajectory.cpp)
target_link_libraries(Totp PUBLIC Eigen3::Eigen)