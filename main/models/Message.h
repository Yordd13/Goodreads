#pragma once
#include <string>

enum class MessageType { 
	FOLLOW_NOTIFY, 
	JOB_OFFER, 
	BOOK_NOTIFY 
};

class Message{

	friend class Reader;
	friend class Author;

private:

	std::string nameSender;
	std::string nameReceiver;
	std::string content;
	MessageType type;
	bool isRead;

	void setNameSender(const std::string& nameSender);
	void setNameReceiver(const std::string& nameReceiver);
	void setContent(const std::string& content);

	static MessageType typeFromInt(int typeInt);

public:

	Message() = delete;
	Message(const std::string& nameSender, const std::string& nameReceiver, const std::string& content, MessageType type);

	std::string getNameSender() const;
	std::string getNameReceiver() const;
	std::string getContent() const;
	MessageType getType() const;
	bool isMessageRead() const;

	void markAsRead();

private:

	void toSerial(std::ostream& os) const;
	static Message fromSerial(std::istream& is);
};

