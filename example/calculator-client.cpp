#include <iostream>

#include <net/poettering/Calculator/client.hpp>

int main() {
    // Define a dbus path location to place the object.
    constexpr auto service = "net.poettering.Calculator";
    constexpr auto path = "/net/poettering/calculator";

    sdbusplus::net::poettering::client::Calculator CalcClient(service, path);

    std::cout << "7*7 = " << CalcClient.multiply(7, 7) << std::endl;
    std::cout << "70/7 = " << CalcClient.divide(70, 7) << std::endl;
    return 0;
}
