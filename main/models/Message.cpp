#include "models/Message.h"

Message::Message(const std::string& nameSender, const std::string& nameReceiver, const std::string& content)
{
	//TODO
}

std::string Message::getNameSender() const
{
	return nameSender;
}

std::string Message::getNameReceiver() const
{
	return nameReceiver;
}

std::string Message::getContent() const
{
	return content;
}

bool Message::isMessageRead() const
{
	return isRead;
}

void Message::markAsRead()
{
	isRead = true;
}

void Message::toSerial(std::ostream& os) const
{
	//TODO
}

Message Message::fromSerial(std::istream& is)
{
	//TODO
	return Message("", "", "");
}
