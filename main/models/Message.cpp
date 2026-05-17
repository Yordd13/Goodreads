#include "models/Message.h"
#include "validator/MessageValidation.h"
#include <stdexcept>
#include <iostream>

void Message::setNameSender(const std::string& nameSender)
{
	if(messageValidation::validateName(nameSender)) {
		this->nameSender = nameSender;
	}
	else {
		throw std::invalid_argument("Invalid sender name");
	}
}

void Message::setNameReceiver(const std::string& nameReceiver)
{
	if(messageValidation::validateName(nameReceiver)) {
		this->nameReceiver = nameReceiver;
	}
	else {
		throw std::invalid_argument("Invalid receiver name");
	}
}

void Message::setContent(const std::string& content)
{
	if(messageValidation::validateContent(content)) {
		this->content = content;
	}
	else {
		throw std::invalid_argument("Invalid message content");
	}
}

Message::Message(const std::string& nameSender, const std::string& nameReceiver, const std::string& content) : isRead(false)
{
	setNameReceiver(nameReceiver);
	setNameSender(nameSender);
	setContent(content);
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

void Message::toSerial(std::ostream& out) const
{
	//   isRead(0/1)
	//   nameSender
	//   nameReceiver
	//   contentLength
	//   content

	out << (isRead ? 1 : 0) << '\n'
		<< nameSender << '\n'
		<< nameReceiver << '\n'
		<< content.size() << '\n'
		<< content << '\n';
}

Message Message::fromSerial(std::istream& in)
{
	int readFlag;
	in >> readFlag;
	in.ignore();

	std::string sender, receiver;
	std::getline(in, sender);
	std::getline(in, receiver);

	size_t contentLen;
	in >> contentLen;
	in.ignore();

	std::string content(contentLen, '\0');
	in.read(content.data(), static_cast<std::streamsize>(contentLen));
	in.ignore();

	Message msg(sender, receiver, content);
	if (readFlag) {
		msg.markAsRead();
	}

	return msg;
}
