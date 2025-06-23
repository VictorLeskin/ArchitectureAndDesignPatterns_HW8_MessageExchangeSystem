///************************* OUTS HOMEWORK ****************************************
#ifndef CMESSAGE_HPP
#define CMESSAGE_HPP

#include <string>
#include "cvector.hpp"
#include "nlohmann/json.hpp"

class cGameId { public: std::string id; };
class cOjectId { public: std::string id; };
class cOperationId { public: std::string id; };

struct cOperationParameters
{
	int type;
	cVector pos;
};

void to_json(nlohmann::json& j, const cOperationParameters& operationParameters);
void from_json(const nlohmann::json& j, cOperationParameters& operationParameters);
void to_json(nlohmann::json& j, const cVector & name);
void from_json(const nlohmann::json& j, cVector &name);

class cMessage
{
public:
	cMessage(std::string str_) : str_(str_) {}

	template<typename OPERATION_PARAMETERS>
	static cMessage Create(const cGameId& gameId, const cOjectId& objId, const cOperationId& operationId,
		const OPERATION_PARAMETERS& operationParameters);

	const std::string& str() const { return str_; }

protected:
	std::string str_;
};

template<typename OPERATION_PARAMETERS>
inline cMessage cMessage::Create(const cGameId& gameId, const cOjectId& objId, const cOperationId& operationId,
	const OPERATION_PARAMETERS& operationParameters)
{
	using json = nlohmann::json;

	// Serialize to JSON
	json j;
	j["gameId"] = gameId.id;
	j["objId"] = objId.id;
	j["operationId"] = operationId.id;
	j["operationParmeters"] = operationParameters;

	return cMessage(j.dump());
}


#endif //#ifndef CMESSAGE_HPP
