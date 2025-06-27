///************************* OUTS HOMEWORK ****************************************
#ifndef CMESSAGE_HPP
#define CMESSAGE_HPP

#include <string>
#include "cvector.hpp"
#include "nlohmann/json.hpp"

class cGameId { public: std::string id; };
class cOjectId { public: std::string id; };
class cOperationId { public: std::string id; };
class cOperationParameters // base class of all types of operation parameters
{
};

class cMsgHeader
{
public:
	cMsgHeader() : pOperationParameters(nullptr) {}

public:
	virtual ~cMsgHeader() = default;

	cGameId gameId;
	cOjectId objId;
	cOperationId operationId;
	cOperationParameters* pOperationParameters;
};

inline void to_json(nlohmann::json& j, const cMsgHeader& h)
{
	j["gameId"] = h.gameId.id;
	j["objId"] = h.objId.id;
	j["operationId"] = h.operationId.id;
}

inline void from_json(const nlohmann::json& j, cMsgHeader& h)
{
	j.at("gameId").get_to(h.gameId.id);
	j.at("objId").get_to(h.objId.id);
	j.at("operationId").get_to(h.operationId.id);
}

class cGameOperation
{
public:
	cGameOperation() : pOperationParameters(nullptr) {}

public:
	virtual ~cGameOperation() = default;

	cGameId gameId;
	cOjectId objId;
	cOperationId operationId;
	cOperationParameters* pOperationParameters;
};


template<typename OPERATION_PARAMETERS>
class TGameOperation : public cGameOperation
{
public:
	TGameOperation()
	{
		pOperationParameters = &operationParameters;
	}
	OPERATION_PARAMETERS operationParameters;
};

template<typename OPERATION_PARAMETERS>
inline void to_json(nlohmann::json& j, const TGameOperation<OPERATION_PARAMETERS>& operation )
{
	j["gameId"] = operation.gameId.id;
	j["objId"] = operation.objId.id;
	j["operationId"] = operation.operationId.id;
	j["operationParmeters"] = operation.operationParameters;
}

template<typename OPERATION_PARAMETERS>
inline void from_json(const nlohmann::json& j, TGameOperation<OPERATION_PARAMETERS>& operation)
{
	j.at("gameId").get_to(operation.gameId.id);
	j.at("objId").get_to(operation.objId.id);
	j.at("operationId").get_to(operation.operationId.id);
	j.at("operationParmeters").get_to(operation.operationParameters);
}

void to_json(nlohmann::json& j, const cOperationParameters& operationParameters);
void from_json(const nlohmann::json& j, cOperationParameters& operationParameters);
void to_json(nlohmann::json& j, const cVector & name);
void from_json(const nlohmann::json& j, cVector &name);

class cMessage
{
public:
	cMessage() {}
	cMessage(std::string str_) : str_(str_) {}

	template<typename OPERATION_PARAMETERS>
	static void to_msg(cMessage& msg, const OPERATION_PARAMETERS& operation )
	{
		nlohmann::json j;
		to_json(j, operation);
		msg = cMessage(j.dump());
	}

	template<typename OPERATION_PARAMETERS>
	static void from_msg(const cMessage& msg, OPERATION_PARAMETERS& operation)
	{
		nlohmann::json j = nlohmann::json::parse(msg.str_);
		from_json(j, operation);
	}

	const std::string& str() const { return str_; }
	const std::string Header() const { return header; }

protected:
	std::string header;
	std::string operationParameters;
	std::string str_;
};


#endif //#ifndef CMESSAGE_HPP
