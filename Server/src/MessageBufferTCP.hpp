#include "MessageBuffer.hpp"

#include <vector>

class MessageBufferTCP : public IMessageBuffer
{
public:
    MessageBufferTCP();

    ~MessageBufferTCP();

    void addMessage(std::vector<uint8_t> Bytes);

    void addMessage(std::shared_ptr<IMessage> Message);

    bool hasMessage();

    std::shared_ptr<IMessage> getMessage();

private:
    std::vector<uint8_t> Buffer;
};