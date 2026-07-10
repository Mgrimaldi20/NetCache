#pragma once

#include <string>
#include <memory>

#include "../../NetBase/src/NetBaseAPI.h"

class ClientAPIImpl : public ClientAPI
{
public:
	ClientAPIImpl(NetBaseAPI *netbaseapi, std::string protoname);

	virtual ~ClientAPIImpl() = default;

	void RegisterCmds() override final;
	ClientAPI::Parser *GetParser() override final;
	const char *GetProtocolName() override final;

private:
	NetBaseAPI *netbaseapi;

	std::shared_ptr<ClientAPI::Parser> parser;
	std::string protoname;
};
