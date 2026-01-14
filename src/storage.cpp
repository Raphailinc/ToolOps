#include "storage.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

Storage::Storage(fs::path data_dir) {
    if (data_dir.empty()) {
        data_dir = fs::path("data");
    }
    if (!fs::exists(data_dir)) {
        fs::create_directories(data_dir);
    }
    machines_path_ = data_dir / "machines.tsv";
    requests_path_ = data_dir / "requests.tsv";
}

static std::vector<std::string> split(const std::string& line, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, delim)) {
        parts.push_back(item);
    }
    return parts;
}

Snapshot Storage::load() const {
    Snapshot snap;

    if (std::ifstream mf{machines_path_}) {
        std::string line;
        while (std::getline(mf, line)) {
            auto cols = split(line, '\t');
            if (cols.size() < 5) continue;
            Machine m;
            m.id = std::stoi(cols[0]);
            m.name = cols[1];
            m.status = cols[2];
            m.location = cols[3];
            m.last_service = cols[4];
            snap.machines.push_back(m);
        }
    }

    if (std::ifstream rf{requests_path_}) {
        std::string line;
        while (std::getline(rf, line)) {
            auto cols = split(line, '\t');
            if (cols.size() < 5) continue;
            WorkRequest r;
            r.id = std::stoi(cols[0]);
            r.machine_id = std::stoi(cols[1]);
            r.description = cols[2];
            r.status = cols[3];
            r.created_at = cols[4];
            snap.requests.push_back(r);
        }
    }

    return snap;
}

static void write_machines(const fs::path& path, const std::vector<Machine>& machines) {
    std::ofstream out(path, std::ios::trunc);
    if (!out) throw std::runtime_error("Cannot open " + path.string());
    for (auto& m : machines) {
        out << m.id << '\t' << m.name << '\t' << m.status << '\t' << m.location << '\t' << m.last_service << '\n';
    }
}

static void write_requests(const fs::path& path, const std::vector<WorkRequest>& reqs) {
    std::ofstream out(path, std::ios::trunc);
    if (!out) throw std::runtime_error("Cannot open " + path.string());
    for (auto& r : reqs) {
        out << r.id << '\t' << r.machine_id << '\t' << r.description << '\t' << r.status << '\t' << r.created_at << '\n';
    }
}

void Storage::save(const Snapshot& snapshot) const {
    write_machines(machines_path_, snapshot.machines);
    write_requests(requests_path_, snapshot.requests);
}

Machine* find_machine(Snapshot& snap, int id) {
    for (auto& m : snap.machines) {
        if (m.id == id) return &m;
    }
    return nullptr;
}

WorkRequest* find_request(Snapshot& snap, int id) {
    for (auto& r : snap.requests) {
        if (r.id == id) return &r;
    }
    return nullptr;
}
