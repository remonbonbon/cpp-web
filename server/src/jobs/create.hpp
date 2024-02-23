#ifndef _MY_CREATE_JOB_
#define _MY_CREATE_JOB_

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

#define DATA_DIR "./data/"

namespace jobs {

void create(const Request &req, Response &res) {
  const std::string id = req.path_params.at("id");
  const json params = json::parse(req.body);
  const std::string filename = params["filename"];
  const std::string type = params["type"];
  const std::string dataBase64 = params["data"];

  std::string ext;
  if (params["type"] == "image/jpeg") {
    ext = ".jpg";
  } else if (params["type"] == "image/png") {
    ext = ".png";
  } else {
    throw std::invalid_argument("Unsupported file type");
  }

  // JSONファイルを保存
  {
    std::filesystem::path p = DATA_DIR;
    p.append(id + ".json");

    const auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count();

    json data;
    data["id"] = id;
    data["filename"] = filename;
    data["type"] = type;
    data["timestamp"] = now;

    std::ofstream f;
    f.open(p, std::ios::out);
    f << data.dump(2);
    f.close();
  }

  // 画像ファイルを保存
  {
    std::filesystem::path p = DATA_DIR;
    p.append(id + ext);

    // std::cout << "base64 size: " << dataBase64.length() << std::endl;
    std::vector<uint8_t> dataBinary = base64::decode(dataBase64);
    // std::cout << "binary size: " << dataBinary.size() << std::endl;

    std::ofstream f;
    f.open(p, std::ios::out | std::ios::binary);
    f.write((char *)&dataBinary[0], dataBinary.size() * sizeof(dataBinary[0]));
    f.close();
  }

  std::stringstream body;
  body << R"({"message":"OK"})";
  res.set_content(body.str(), "application/json");
}

} // namespace jobs

#endif
