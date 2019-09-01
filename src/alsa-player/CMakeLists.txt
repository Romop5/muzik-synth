add_executable(alsa alsa.cpp)
target_link_libraries(alsa PRIVATE muzik)
target_link_libraries(alsa PRIVATE asound)

