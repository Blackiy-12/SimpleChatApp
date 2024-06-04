#include "MessageBuffer.hpp"

#include <queue>

class MessageBufferUDP : public IMessageBuffer
{
public:
    MessageBufferUDP();

    ~MessageBufferUDP();

    void addMessage(std::vector<uint8_t> Bytes);

    void addMessage(std::shared_ptr<IMessage> Message);

    bool hasMessage();

    std::shared_ptr<IMessage> getMessage();

private:
    std::queue<std::shared_ptr<IMessage>> Buffer;
};