#include <iostream>
#include "../../src/Utils.hpp"

void fuckthis(int signum){ std::cout << "fuckthis\n"; exit(signum); }

int main(){
    Utils::install_SIGINT(fuckthis);
    raise(SIGINT);
    return 0;
}
