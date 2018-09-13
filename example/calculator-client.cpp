#include <iostream>

#include <net/poettering/Calculator/client.hpp>

int main() {
    // Define a dbus path location to place the object.
    constexpr auto service = "net.poettering.Calculator";
    constexpr auto path = "/net/poettering/calculator";

    sdbusplus::net::poettering::client::Calculator CalcClient(service, path);

    std::cout << "7*7 = " << CalcClient.multiply(7, 7) << std::endl;
    std::cout << "70/7 = " << CalcClient.divide(70, 7) << std::endl;

    std::cout << std::endl << "Last result was: " << CalcClient.lastResult() << std::endl;

    auto status = CalcClient.status();
    std::cout << "Status was " << ((status == sdbusplus::net::poettering::client::Calculator::State::Success)
        ? "Success" : "Error") << std::endl;

    try {
        std::cout << std::endl << "10/0 = " << CalcClient.divide(70, 0);
    } catch(const sdbusplus::exception::SdBusError &exc) {
        std::cout << "Error in call: " << exc.what() << std::endl;
    }

    return 0;
}
