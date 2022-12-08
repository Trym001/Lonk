//
// Created by zaime on 12/3/2022.
//

#ifndef LONK_JSON_HPP
#define LONK_JSON_HPP

#include <string>
#include <nlohmann/json.hpp>


using json = nlohmann::json;


struct json_parsing{
public:
    static json write_json(json msg1){
        std::string dump = msg1.dump();
        return dump;
    }

    static json read_json(std::string msg2){
        return json::parse(msg2);
    }

private:
};
#endif //LONK_JSON_HPP
