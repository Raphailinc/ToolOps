#include "minitest.hpp"
#include "storage.hpp"
#include <filesystem>

namespace fs = std::filesystem;

TEST(test_add_machine_and_request) {
    auto tmp = fs::temp_directory_path() / "toolops-test";
    fs::remove_all(tmp);
    Storage storage(tmp);
    Snapshot snap = storage.load();

    Machine m{.id = 1, .name = "Lathe A", .status = "idle", .location = "Shop1", .last_service = "2024-01-01"};
    snap.machines.push_back(m);
    storage.save(snap);

    auto loaded = storage.load();
    CHECK(loaded.machines.size() == 1);
    CHECK(loaded.machines[0].name == "Lathe A");

    WorkRequest r{.id = 1, .machine_id = 1, .description = "Oil change", .status = "open", .created_at = "2024-01-02"};
    loaded.requests.push_back(r);
    storage.save(loaded);

    auto loaded2 = storage.load();
    CHECK(loaded2.requests.size() == 1);
    CHECK(loaded2.requests[0].description == "Oil change");

    fs::remove_all(tmp);
}
