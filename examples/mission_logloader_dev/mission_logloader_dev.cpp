/**
 * brief Demonstrates how to import mission items from a QGroundControl plan,
 * and fly them using the MAVSDK.
 *
 * Steps to run this example:
 * 1.(a) Create a Mission in QGroundControl and save them to a file (.plan), or
 *   (b) Use a pre-created sample mission plan in
 *       "src/plugins/mission_raw/test_plans/qgroundcontrol_sample.plan".
 * 2. Run the example by passing the path of the QGC mission plan as argument.
 *
 * Example description:
 * 1. Imports QGC mission items from .plan file.
 * 2. Uploads mission items to vehicle.
 * 3. Starts mission from first mission item.
 * 4. Commands RTL once QGC Mission is accomplished.
 */

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/mission_raw/mission_raw.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/mission/mission.h>

#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <thread>

using namespace mavsdk;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

void usage(const std::string& bin_name)
{
    std::cerr << "Usage : " << bin_name << " <connection_url> <mission_plan_path>\n"
              << "Connection URL format should be :\n"
              << " For TCP : tcp://[server_host][:server_port]\n"
              << " For UDP : udp://[bind_host][:bind_port]\n"
              << " For Serial : serial:///path/to/serial/dev[:baudrate]\n"
              << " For example, to connect to the simulator use URL: udp://:14540\n";
}

constexpr float_t FCU_CONNECTION_TIMEOUT_SEC = 30.0;

int main(int argc, char** argv)
{
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    Mavsdk mavsdk{Mavsdk::Configuration{Mavsdk::ComponentType::CompanionComputer}};

    const ConnectionResult connection_result = mavsdk.add_any_connection(argv[1]);
    if (connection_result != ConnectionResult::Success) {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }

    auto system = mavsdk.first_autopilot(FCU_CONNECTION_TIMEOUT_SEC);
    if (!system) {
        std::cerr << "Timed out waiting for system\n";
        return 1;
    }

    auto action = Action{system.value()};
    auto mission_raw = MissionRaw{system.value()};
    auto mission = Mission{system.value()};
    auto telemetry = Telemetry{system.value()};

    while (!telemetry.health_all_ok()) {
        std::cout << "Waiting for system to be ready\n";
        sleep_for(milliseconds(500));
    }

    std::cout << "System ready\n";

    std::cout << "Downloading mission." << '\n';
    
    std::pair<MissionRaw::Result, std::vector<MissionRaw::MissionItem>> result = mission_raw.download_mission();

    if (result.first != MissionRaw::Result::Success) {
        std::cout << "Mission download failed (" << result.first << "), exiting." << '\n';
        return 1;
    }

    std::cout << "Mission downloaded (MissionItems: "
        << result.second.size()
        << ")" << '\n';

    for (int i = 0; i < result.second.size(); i++) {
        std::cout << result.second[i] << std::endl;
    }

    return 0;
}
