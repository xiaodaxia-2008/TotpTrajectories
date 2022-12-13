add_executable(TotpExample TotpExample.cpp)
target_link_libraries(TotpExample PRIVATE Totp spdlog::spdlog fmt::fmt cxxopts::cxxopts)