///************************* OUTS HOMEWORK ****************************************

#include "cMessage.hpp"


void to_json(nlohmann::json& j, const cOperationParameters& operationParameters)
{
    j = nlohmann::json
    {
        { "type", operationParameters.type },
        { "pos", operationParameters.pos }
    };
}


void from_json(const nlohmann::json& j, cOperationParameters& operationParameters)
{
    j.at("type").get_to(operationParameters.type);
    j.at("pos").get_to(operationParameters.pos);
}


void to_json(nlohmann::json& j, const cVector& v)
{
    j = nlohmann::json
    {
        { "x", v.x },
        { "y", v.y }
    };
}


void from_json(const nlohmann::json& j, cVector& v)
{
    j.at("x").get_to(v.x);
    j.at("y").get_to(v.y);
}

