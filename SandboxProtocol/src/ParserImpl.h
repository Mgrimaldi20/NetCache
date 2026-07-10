#pragma once

#include "../../NetBase/src/NetBaseAPI.h"

class ParserImpl : public ClientAPI::Parser
{
public:
	ParserImpl() = default;
	virtual ~ParserImpl() = default;

	ClientAPI::Parser::ParsedCmd Parse(const char *data, std::size_t length) override final;
};
