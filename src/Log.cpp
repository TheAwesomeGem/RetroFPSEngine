#include "Log.h"

void Log::init()
{
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] %^[%l] %v%$");
    spdlog::set_level(spdlog::level::debug);
}
