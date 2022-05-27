#include "../../includes/response.hpp"


void resoponse::setBuffer(char *buffer)
{
	this->bufferSize = buffer_size;
}

void response::setBufferOffset(int buffer_offset)
{
	this->bufferOffset = buffer_offset;
}

void response::setResponseType(int responseType)
{
	this->responseType = responseType;
}

void response::setBodySize(int bodySize)
{
	this->bodySize = bodySize;
}

void response 