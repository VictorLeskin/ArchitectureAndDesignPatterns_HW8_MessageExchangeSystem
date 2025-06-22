///************************* OUTS HOMEWORK ****************************************
#ifndef CMESSAGE_HPP
#define CMESSAGE_HPP

#include <string>
#include "cvector.hpp"
#include "nlohmann/json_fwd.hpp"


class cGameId { public: std::string id; };
class cOjectId { public: std::string id; };
class cOperationId { public: std::string id; };

struct cOperationParameters
{
	int type;
	double dir;
	cVector pos;
};

void to_json(nlohmann::json& j, const cOperationParameters& operationParameters);
void from_json(const nlohmann::json& j, cOperationParameters& operationParameters);
void to_json(nlohmann::json& j, const cVector & name);
void from_json(const nlohmann::json& j, cVector &name);

class cMessage
{
public:
	static cMessage Create(cGameId &gameId, cOjectId &objId, cOperationId &id, cOperationParameters& operationParameters);

protected:
	std::string str;
};

#endif //#ifndef CMESSAGE_HPP
