#include <telkin/Print.h>
#include <TRU/TRU.h>

red::Registrar* TRU::getRegistrar() {
    static red::Registrar sRegistrar("TRU");
    return &sRegistrar;
}

void main() {
    tk::println("TightRopeU");
}

