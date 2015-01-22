#include "waterfall.h"

shared_ptr<WaterfallScene> waterfall = NULL;

int main(int argc, char* argv[])
{
    try {
        WaterfallScene* scene = new WaterfallScene();
        waterfall.reset(scene);
        waterfall->load();
        waterfall->run();
    }
    catch (std::exception& except) {
        cout << except.what() << endl;
    }

    waterfall.reset();
}