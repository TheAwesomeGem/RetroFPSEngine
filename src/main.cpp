#include "Application.h"
#include "Log.h"

int main()
{
    Log::init();

    Application::AppState app = {};
    Application::run(app);

    return 0;
}
