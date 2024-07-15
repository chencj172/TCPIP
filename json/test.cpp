#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>
#include <jsoncpp/json/json.h>

int main(int argc, const char* argv[])
{
    Json::Value root;
    root["x"] = 100;
    root["y"] = 200;
    root["op"] = "+";

    // json 转字符串
    Json::FastWriter w;
    std::string res = w.write(root);
    std::cout << res << std::endl;

    res = root.toStyledString();
    std::cout << res << std::endl;

    Json::Value r[2];
    Json::Value arr;
    r[0]["x"] = 1;
    r[0]["y"] = 2;
    r[0]["op"] = "+";
    r[1]["x"] = 10;
    r[1]["y"] = 20;
    r[1]["op"] = "-";
    arr.append(r[0]);
    arr.append(r[1]);
    const std::string resarr = arr.toStyledString();
    std::cout << resarr << std::endl;

    // 字符串转json
    Json::Value root2;
    Json::Reader reader;
    if(reader.parse(res, root2))
    {
        int x = root2["x"].asInt();
        int y= root2["y"].asInt();
        std::string op = root2["op"].asString();

        std::cout << x << "  " << y << "  " << op << std::endl;
    }

    // 解析数组
    Json::CharReaderBuilder R;
    std::istringstream iss(resarr);
    Json::parseFromStream(R, iss, &root, nullptr);

    for(const auto& rem : root)
    {
        int x = root2["x"].asInt();
        int y= root2["y"].asInt();
        std::string op = root2["op"].asString();

        std::cout << x << "  " << y << "  " << op << std::endl;
    }
    return 0;
}