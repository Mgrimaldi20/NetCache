#include "Response.h"

Response &Response::ProtocolName(std::string pname) &
{
	this->protoname = std::move(pname);
	return *this;
}

Response &&Response::ProtocolName(std::string pname) &&
{
	this->protoname = std::move(pname);
	return std::move(*this);
}

Response &Response::Version(std::uint8_t ver) &
{
	this->version = ver;
	return *this;
}

Response &&Response::Version(std::uint8_t ver) &&
{
	this->version = ver;
	return std::move(*this);
}

Response &Response::RemainingLength(std::uint32_t rlen) &
{
	this->remaininglen = rlen;
	return *this;
}

Response &&Response::RemainingLength(std::uint32_t rlen) &&
{
	this->remaininglen = rlen;
	return std::move(*this);
}

Response &Response::Status(Response::StatusCode sc) &
{
	this->status = sc;
	return *this;
}

Response &&Response::Status(Response::StatusCode sc) &&
{
	this->status = sc;
	return std::move(*this);
}

Response &Response::Payload(std::string pl) &
{
	this->payload = std::move(pl);
	return *this;
}

Response &&Response::Payload(std::string pl) &&
{
	this->payload = std::move(pl);
	return std::move(*this);
}

std::string Response::Build() &&
{
	return std::string();
}
