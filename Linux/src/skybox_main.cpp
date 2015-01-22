#include "waterfall.h"

int main(int argc, char* argv[])
{
    try {
        WaterfallScene waterfall;
        waterfall.load();
        waterfall.run();
    }
    catch (std::exception& except) {
        cout << except.what() << endl;
    }
}
