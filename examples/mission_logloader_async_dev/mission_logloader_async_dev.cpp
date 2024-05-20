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
#include <mavsdk/plugins/mission_raw/mission_raw.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <fstream>
#include <memory>
#include <thread>
#include <vector> 

#include <nlohmann/json.hpp>

using namespace mavsdk;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;
using json = nlohmann::json;

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

namespace missionJson {
    // a simple struct to model a person
    struct missionWaypoint {
                uint32_t seq;
                uint32_t frame;
                uint32_t command;
                uint32_t current;
                uint32_t autocontinue;
                float param1;
                float param2;
                float param3;
                float param4;
                int32_t x;
                int32_t y;
                float z;
                uint32_t mission_type;
    };
    struct mission_json {
        std::string name;
        std::string downloaded_time;
        std::vector<missionWaypoint> missionItems;
    };
            
}; // namespace missionJson

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

    auto mission_raw = MissionRaw{system.value()};
    auto telemetry = Telemetry{system.value()};

    while (!telemetry.health_all_ok()) {
        std::cout << "Waiting for system to be ready\n";

        sleep_for(seconds(1));
    }

    std::cout << "System ready\n";

    // std::cout << "Downloading mission." << '\n';

    mavsdk::MissionRaw::Result _result;
    std::vector<mavsdk::MissionRaw::MissionItem> _mission_item;
    
    mission_raw.download_mission_async([&](MissionRaw::Result result, std::vector<MissionRaw::MissionItem> mission_items) {
        
        if (result == MissionRaw::Result::Success) {
            std::cout << "Mission downloaded successfully!" << std::endl;
            _result = result;
            // Process the downloaded mission items here
            for (const auto& mission_item : mission_items) {
                // Print mission item details (e.g., type, coordinates)
                std::cout << "Bookmark Check: " << std::endl;
                std::cout << "Mission item: " << mission_item << std::endl;
                _mission_item.insert(_mission_item.end(),mission_item);
            }
        }

        //     // Process the downloaded mission items here
        //     for (const auto& mission_item : mission_items) {
        //         // Print mission item details (e.g., type, coordinates)
        //         std::cout << "Mission item: " << mission_item.seq << std::endl;
        //         // ... (extract other relevant data from mission_item)
        //     }
        // } else {
        //     std::cerr << "Mission download failed: " << result << std::endl;
        // }

    });
    
    json j;
    j["name"] = "Mock up name";
    j["downloaded_time"] = "16.57";

    int printing_cut = 1;

    while (true) {
            // std::cout << "Downloading Status: " << _result << std::endl;
            // std::cout << "Mission Information: " << _mission_item[0] << std::endl;
            // std::cout << "Mission Information Lenght :" << _mission_item.size() << std::endl;
            
            for (int i =0 ; i < _mission_item.size() ; i++ ){
                j["missionItems"][i]["seq"]             = _mission_item[i].seq;
                j["missionItems"][i]["frame"]           = _mission_item[i].frame;
                j["missionItems"][i]["command"]         = _mission_item[i].command;
                j["missionItems"][i]["current"]         = _mission_item[i].current;
                j["missionItems"][i]["autocontinue"]    = _mission_item[i].autocontinue;
                j["missionItems"][i]["param1"]          = _mission_item[i].param1;
                j["missionItems"][i]["param2"]          = _mission_item[i].param2;
                j["missionItems"][i]["param3"]          = _mission_item[i].param3;
                j["missionItems"][i]["param4"]          = _mission_item[i].param4;
                j["missionItems"][i]["x"]               = _mission_item[i].x;
                j["missionItems"][i]["y"]               = _mission_item[i].y;
                j["missionItems"][i]["z"]               = _mission_item[i].z;
                j["missionItems"][i]["mission_type"]    = _mission_item[i].mission_type;
            }
            //std::cout << "Printing_cut :" << printing_cut << std::endl;
            if(printing_cut < 3){
                // std::cout << "Json Output :" << j << std::endl;
                std::string jsonString = j.dump(4,' ',true);
                std::ofstream outputFile("data.json", std::ofstream::out);
                outputFile << jsonString;
                outputFile.close();
                printing_cut++;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    return 0;
}
