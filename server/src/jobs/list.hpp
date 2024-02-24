#ifndef _MY_JOBS_LIST_
#define _MY_JOBS_LIST_

#include <filesystem>
#include <iostream>
#include <string>

#include <httplib.h>
using Request = httplib::Request;
using Response = httplib::Response;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "./_define.h"

namespace jobs {

void list(const Request &req, Response &res) {
  std::vector<std::string> jobs;
  // ファイル列挙
  for (const std::filesystem::directory_entry &entry :
       std::filesystem::directory_iterator(DATA_DIR)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    const auto ext = entry.path().extension();
    if (ext != ".json") {
      continue;
    }

    const auto id = entry.path().stem().string();
    jobs.push_back(id);
  }

  // 降順ソート
  std::sort(jobs.begin(), jobs.end(), std::greater<std::string>{});

  // 最新10件
  json dest;
  dest["jobs"] = std::vector<std::string>(
      jobs.cbegin(), jobs.cbegin() + std::min((long)jobs.size(), 10L));
  ;
  res.set_content(dest.dump(2), "application/json");
}

} // namespace jobs

#endif
