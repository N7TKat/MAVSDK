
#include <cstdint>
#include <future>
#include <iostream>
#include <thread>
#include <chrono>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/info/info.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

using namespace mavsdk;
using namespace std::this_thread;
using namespace std::chrono;

void usage(const std::string& bin_name)
{
    std::cerr << "Usage : " << bin_name << " <connection_url>\n"
              << "Connection URL format should be :\n"
              << " For TCP : tcp://[server_host][:server_port]\n"
              << " For UDP : udp://[bind_host][:bind_port]\n"
              << " For Serial : serial:///path/to/serial/dev[:baudrate]\n"
              << "For example, to connect to the simulator use URL: udp://:14540\n";
}

constexpr float_t FCU_CONNECTION_TIMEOUT_SEC = 15.0;

int main(int argc, char** argv)
{
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    Mavsdk mavsdk{Mavsdk::Configuration{Mavsdk::ComponentType::CompanionComputer}};
    ConnectionResult connection_result = mavsdk.add_any_connection(argv[1]);

    if (connection_result != ConnectionResult::Success) {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }

    auto system = mavsdk.first_autopilot(FCU_CONNECTION_TIMEOUT_SEC);
    if (!system) {
        std::cerr << "Timed out waiting for system\n";
        std::cerr << "Shutting Down ..............\n";
        return 1;
    }

    std::cerr << "Autopilot Connect result: " << connection_result << '\n';

    // initialize Plugin
    auto info = Info{system.value()};
    auto telemetry = mavsdk::Telemetry{system.value()};

    // initialize Variable
    mavsdk::Telemetry::Position latest_position;
    bool armed_status;

    // setting telemetry rate
    const Telemetry::Result set_rate_result = telemetry.set_rate_position(1.0);
    if (set_rate_result != Telemetry::Result::Success) {
        // handle rate-setting failure (in this case print error)
        std::cout << "Setting rate failed:" << set_rate_result << '\n';
    }
    
    // Get  & Print out the system Version struct
    // const Info::Version& system_version = info.get_version().second;
    // std::cout << system_version << std::endl;
    
    telemetry.subscribe_position([&](mavsdk::Telemetry::Position position) {
        // std::cout << "Relative_Altitude: " << position.relative_altitude_m << " m" << std::endl
        //       << "Latitude: " << position.latitude_deg << std::endl
        //       << "Longitude: " << position.longitude_deg << '\n';
        latest_position.relative_altitude_m = position.relative_altitude_m;
    });

    // telemetry.subscribe_health([](mavsdk::Telemetry::Health health) {
    //     std::cout << "Gyro: " << health.is_gyrometer_calibration_ok << std::endl
    //           << "Accelerometer: " << health.is_accelerometer_calibration_ok << std::endl
    //           << "Magnetometer: " << health.is_magnetometer_calibration_ok << '\n';
    // });

    telemetry.subscribe_armed([&](bool armed) {
        // std::cout << "************************Vehicle Arming Status: " << armed << std::endl;
        armed_status = armed;
    });

    // works as a server and never quit
    while (true) {
        std::cout << "Vehicle Armed Status:" << armed_status << std::endl;
        std::cout << "Relative_Altitude: " << latest_position.relative_altitude_m << " m" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}
