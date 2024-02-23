#ifndef _MY_JOBS_RUN_
#define _MY_JOBS_RUN_

#include <chrono>
#include <filesystem>
#include <fstream>

#include <httplib.h>
using Request = httplib::Request;
using Response = httplib::Response;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <cppcodec/base64_rfc4648.hpp>
using base64 = cppcodec::base64_rfc4648;

#include "./_define.h"

#include "../utils/logger.hpp"

namespace jobs {

void _runJob(std::string id) {
  try {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::filesystem::path jsonPath = DATA_DIR;
    jsonPath.append(id + ".json");

    // JSONファイルを読み込み
    std::ifstream f(jsonPath);
    json data = json::parse(f);
    f.close();

    // JSONファイルを更新
    {
      const auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::system_clock::now().time_since_epoch())
                           .count();

      data["finishedAt"] = now;
      data["result"] = true;

      std::ofstream f(jsonPath);
      f << data.dump(2);
      f.close();
    }

    logger::info(data.dump());
  } catch (std::exception &e) {
    logger::error(e.what());
  } catch (...) {
    logger::error("Unknown Exception");
  }
}

} // namespace jobs

#endif
