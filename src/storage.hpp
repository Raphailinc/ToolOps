#pragma once

#include "domain.hpp"
#include <filesystem>
#include <string>

class Storage {
public:
    explicit Storage(std::filesystem::path data_dir);

    Snapshot load() const;
    void save(const Snapshot& snapshot) const;

private:
    std::filesystem::path machines_path_;
    std::filesystem::path requests_path_;
};

Machine* find_machine(Snapshot& snap, int id);
WorkRequest* find_request(Snapshot& snap, int id);

