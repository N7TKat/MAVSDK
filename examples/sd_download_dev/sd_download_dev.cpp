/*
Next Features
1. Check Vehicle Status
2. Duplicate file checking and not to Downloading
    2.1 listing file in local directories
    2.2 filter logic to not to download before download
3. Dump file to folder by date
4. Start-up script

** File Failed when stop downloading **
*/

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/ftp/ftp.h>

#include <functional>
#include <future>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <filesystem>

using namespace mavsdk;
using std::chrono::seconds;

void usage(const std::string& bin_name)
{
    std::cerr
        << "Usage : " << bin_name
        << " <connection_url> <server component id>\n"
        << '\n'
        << "Connection URL format should be :\n"
        << " For TCP : tcp://[server_host][:server_port]\n"
        << " For UDP : udp://[bind_host][:bind_port]\n"
        << " For Serial : serial:///path/to/serial/dev[:baudrate]\n"
        << "For example, to connect to the simulator use URL: udp://:14540\n"
        << '\n'
        << "Server component id is for example 1 for autopilot or 195 for companion computer,\n"
        << "which is being used if you run the ftp_server example\n";
}

std::pair<Ftp::Result, std::vector<std::string>>
list_directory(Ftp& ftp, const std::string& path)
{
    std::cerr << "List directory: " << path << '\n';
    auto prom = std::promise<std::pair<Ftp::Result, std::vector<std::string>>>{};
    auto future_result = prom.get_future();
    ftp.list_directory_async(path, [&prom](Ftp::Result result, std::vector<std::string> list) {
        prom.set_value(std::pair<Ftp::Result, std::vector<std::string>>(result, list));
    });

    std::pair<Ftp::Result, std::vector<std::string>> result = future_result.get();
    if (result.first == Ftp::Result::Success) {
        for (auto entry : result.second) {
            std::cerr << entry << '\n';
        }
    }
    return result;
}

Ftp::Result
download_file(Ftp& ftp, const std::string& remote_file_path, const std::string& local_path)
{
    std::cerr << "Download file: " << remote_file_path << " to " << local_path << '\n';
    auto prom = std::promise<Ftp::Result>{};
    auto future_result = prom.get_future();
    ftp.download_async(
        remote_file_path,
        local_path,
        false,
        [&prom](Ftp::Result result, Ftp::ProgressData progress) {
            if (result == Ftp::Result::Next) {
                int percentage = progress.total_bytes > 0 ?
                                     progress.bytes_transferred * 100 / progress.total_bytes :
                                     0;
                std::cerr << "\rDownloading [" << std::setw(3) << percentage << "%] "
                          << progress.bytes_transferred << " of " << progress.total_bytes;
                if (progress.bytes_transferred >= progress.total_bytes) {
                    std::cerr << '\n';
                }
            } else {
                prom.set_value(result);
            }
        });

    return future_result.get();
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    Mavsdk mavsdk{Mavsdk::Configuration{Mavsdk::ComponentType::GroundStation}};
    ConnectionResult connection_result = mavsdk.add_any_connection(argv[1]);

    if (connection_result != ConnectionResult::Success) {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }

    auto system = mavsdk.first_autopilot(3.0);
    if (!system) {
        std::cerr << "Timed out waiting for system\n";
        return 1;
    }

    // Instantiate plugins.
    
    auto ftp = Ftp{system.value()};
    try {
        ftp.set_target_compid(std::stoi(argv[2]));
    } catch (...) {
        std::cerr << "Invalid argument: " << argv[2] << '\n';
        return 1;
    }
    
    std::string mission_log_path = "/fs/microsd/mission_log";

    std::cout << "Connected Successfully" << std::endl;

    while (true) {
        
        // Iterate in /fs/microsd/mission_log
        std::pair<Ftp::Result, std::vector<std::string>> listing_result = list_directory(ftp, std::string(mission_log_path));
        if (listing_result.second.size() != 0){
            for (int i = 0 ; i < listing_result.second.size() ; i++){
                // std::cout << "mission_log directory: " << listing_result.second[i] << std::endl;
                // filter for lastest day folder
                // if (i != listing_result.second.size() - 1){
                //     std::cout << "This folder is not lastest: " << listing_result.second[i] << std::endl;
                //     continue;
                // }
                std::string(day_folder) = listing_result.second[i].erase(0,1);
                // std::cout << "lastest day folder: " << day_folder << std::endl;
                
                // Listing All files in day folder
                std::pair<Ftp::Result, std::vector<std::string>> files_result = list_directory(ftp, std::string(mission_log_path + "/" + day_folder));
                if (files_result.second.size() != 0){
                    int j = 0;
                    for (j ; j < files_result.second.size() ; j++){
                        // if (j != listing_result.second.size() - 1){
                        // std::cout << "This folder is not lastest: " << listing_result.second[i] << std::endl;
                        // continue;
                        // }
                        // std::cout << "mission_log files: " << files_result.second[i].erase(0,1) << 
                        //             "\nSize: " << files_result.second[i].erase(0,13) << " kB" << std::endl;

                        files_result.second[j].erase(13); 
                        files_result.second[j].erase(0,1);
                        
                        std::string(mission_log_file_path) = mission_log_path + "/" + day_folder + "/" + files_result.second[j];
                        std::cout << "mission_log files path: " << mission_log_file_path << std::endl;

                        //Download the mission_log file from mission_log_file_path
                        auto res = download_file(ftp, mission_log_file_path, "."); // download to same directory 
                        if (res == Ftp::Result::Success) {
                            std::cerr << "File downloaded.\n";
                        } else {
                            std::cerr << "File download error: " << res << '\n';
                            return (res == Ftp::Result::FileDoesNotExist) ? 2 : 1;
                        }
                    }
                } else {
                    std::cout << "There is no files in mission day Directory" << std::endl;
                }
            }
        } else {
            std::cout << "There is no files in mission_log Directory" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
