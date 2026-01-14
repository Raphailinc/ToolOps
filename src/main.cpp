#include "domain.hpp"
#include "storage.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>

namespace fs = std::filesystem;

struct Args {
    std::string command;
    std::vector<std::string> params;
};

Args parse_args(int argc, char* argv[]) {
    Args a;
    if (argc > 1) {
        a.command = argv[1];
        for (int i = 2; i < argc; ++i) a.params.emplace_back(argv[i]);
    }
    return a;
}

int next_id(const std::vector<Machine>& list) {
    int max_id = 0;
    for (auto& m : list) max_id = std::max(max_id, m.id);
    return max_id + 1;
}

int next_request_id(const std::vector<WorkRequest>& list) {
    int max_id = 0;
    for (auto& r : list) max_id = std::max(max_id, r.id);
    return max_id + 1;
}

void cmd_list(const Snapshot& snap) {
    std::cout << "Машины (" << snap.machines.size() << "):\n";
    for (auto& m : snap.machines) {
        std::cout << "  [" << m.id << "] " << m.name << " · " << m.status << " · " << m.location
                  << " (посл. сервис: " << m.last_service << ")\n";
    }
    std::cout << "\nЗаявки (" << snap.requests.size() << "):\n";
    for (auto& r : snap.requests) {
        std::cout << "  [" << r.id << "] #" << r.machine_id << " · " << r.status << " · " << r.description
                  << " (" << r.created_at << ")\n";
    }
}

void cmd_add_machine(Storage& storage, Snapshot& snap, const std::vector<std::string>& params) {
    if (params.size() < 2) {
        std::cout << "Использование: add-machine <name> <location> [status]\n";
        return;
    }
    Machine m;
    m.id = next_id(snap.machines);
    m.name = params[0];
    m.location = params[1];
    m.status = params.size() > 2 ? params[2] : "idle";
    m.last_service = iso_now();
    snap.machines.push_back(m);
    storage.save(snap);
    std::cout << "Добавлена машина #" << m.id << "\n";
}

void cmd_add_request(Storage& storage, Snapshot& snap, const std::vector<std::string>& params) {
    if (params.size() < 2) {
        std::cout << "Использование: add-request <machine_id> <описание>\n";
        return;
    }
    int mid = std::stoi(params[0]);
    if (!find_machine(snap, mid)) {
        std::cout << "Машина #" << mid << " не найдена\n";
        return;
    }
    WorkRequest r;
    r.id = next_request_id(snap.requests);
    r.machine_id = mid;
    r.description = params[1];
    r.status = "open";
    r.created_at = iso_now();
    snap.requests.push_back(r);
    storage.save(snap);
    std::cout << "Создана заявка #" << r.id << "\n";
}

void cmd_update_status(Storage& storage, Snapshot& snap, const std::vector<std::string>& params) {
    if (params.size() < 2) {
        std::cout << "Использование: set-status <machine_id> <status>\n";
        return;
    }
    int mid = std::stoi(params[0]);
    auto* m = find_machine(snap, mid);
    if (!m) {
        std::cout << "Машина #" << mid << " не найдена\n";
        return;
    }
    m->status = params[1];
    storage.save(snap);
    std::cout << "Статус машины #" << mid << " изменён на " << m->status << "\n";
}

void cmd_close_request(Storage& storage, Snapshot& snap, const std::vector<std::string>& params) {
    if (params.size() < 1) {
        std::cout << "Использование: close-request <request_id>\n";
        return;
    }
    int rid = std::stoi(params[0]);
    auto* r = find_request(snap, rid);
    if (!r) {
        std::cout << "Заявка #" << rid << " не найдена\n";
        return;
    }
    r->status = "done";
    storage.save(snap);
    std::cout << "Заявка #" << rid << " закрыта\n";
}

void print_help() {
    std::cout << "Команды:\n"
              << "  list                            - показать машины и заявки\n"
              << "  add-machine <name> <loc> [st]   - добавить машину\n"
              << "  add-request <machine_id> <txt>  - создать заявку\n"
              << "  set-status <machine_id> <st>    - изменить статус машины\n"
              << "  close-request <request_id>      - закрыть заявку\n";
}

int main(int argc, char* argv[]) {
    fs::path data_dir = std::getenv("TOOLOPS_DATA") ? std::getenv("TOOLOPS_DATA") : "data";
    Storage storage(data_dir);
    Snapshot snap = storage.load();

    auto args = parse_args(argc, argv);
    if (args.command.empty() || args.command == "help" || args.command == "--help") {
        print_help();
        return 0;
    }

    if (args.command == "list") {
        cmd_list(snap);
    } else if (args.command == "add-machine") {
        cmd_add_machine(storage, snap, args.params);
    } else if (args.command == "add-request") {
        cmd_add_request(storage, snap, args.params);
    } else if (args.command == "set-status") {
        cmd_update_status(storage, snap, args.params);
    } else if (args.command == "close-request") {
        cmd_close_request(storage, snap, args.params);
    } else {
        std::cout << "Неизвестная команда: " << args.command << "\n";
        print_help();
        return 1;
    }

    return 0;
}
