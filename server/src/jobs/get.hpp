#ifndef _MY_JOBS_RUN_
#define _MY_JOBS_RUN_

#include <filesystem>
#include <fstream>
#include <thread>

#include <httplib.h>
using Request = httplib::Request;
using Response = httplib::Response;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "./_define.h"

namespace jobs {

void get(const Request &req, Response &res) {
  std::stringstream body;
  const std::string id = req.path_params.at("id");
  {
    std::filesystem::path p = DATA_DIR;
    p.append(id + ".json");

    std::ifstream f(p);
    body << f.rdbuf();
    f.close();
  }
  res.set_content(body.str(), "application/json");
}

} // namespace jobs

#endif
