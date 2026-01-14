#pragma once

#include <chrono>
#include <string>
#include <vector>

struct Machine {
    int id{};
    std::string name;
    std::string status;      // e.g., idle, running, maintenance
    std::string location;
    std::string last_service; // ISO date
};

struct WorkRequest {
    int id{};
    int machine_id{};
    std::string description;
    std::string status; // open, in_progress, done
    std::string created_at;
};

struct Snapshot {
    std::vector<Machine> machines;
    std::vector<WorkRequest> requests;
};

std::string iso_now();
std::string trim(const std::string& s);
