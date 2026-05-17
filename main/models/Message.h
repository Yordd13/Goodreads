#pragma once
#include <string>

class Message
{
private:

	std::string nameSender;
	std::string nameReceiver;
	std::string content;
	bool isRead;

	void setNameSender(const std::string& nameSender);
	void setNameReceiver(const std::string& nameReceiver);
	void setContent(const std::string& content);

public:

	Message() = delete;
	Message(const std::string& nameSender, const std::string& nameReceiver, const std::string& content);

	std::string getNameSender() const;
	std::string getNameReceiver() const;
	std::string getContent() const;
	bool isMessageRead() const;

	void markAsRead();

private:
	void toSerial(std::ostream& os) const;
	static Message fromSerial(std::istream& is);
};

