#ifndef _MY_JOBS_RUN_
#define _MY_JOBS_RUN_

#include <filesystem>
#include <fstream>
#include <thread>
using thread = std::thread;

#include <httplib.h>
using Request = httplib::Request;
using Response = httplib::Response;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "./_define.h"

#include "../utils/logger.hpp"

namespace jobs {

void _runJob(std::string id) {
  try {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::filesystem::path p = DATA_DIR;
    p.append(id + ".json");
    std::ifstream f(p);
    const json data = json::parse(f);

    logger::info(data.dump());
  } catch (std::exception &e) {
    logger::error(e.what());
  } catch (...) {
    logger::error("Unknown Exception");
  }
}

void run(const Request &req, Response &res) {
  const std::string id = req.path_params.at("id");

  thread t(_runJob, id);
  t.detach(); // スレッドを投げっぱなしにする

  std::stringstream body;
  body << R"({"message":"OK"})";
  res.set_content(body.str(), "application/json");
}

} // namespace jobs

#endif
