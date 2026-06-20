#include "Application.h"
#include "Log.h"

int main()
{
    Log::init();

    Application* app = new Application();
    app->run();

    delete app;

    return 0;
}
