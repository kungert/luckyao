#include "ByteBuffer.h"
#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace luckyao;

ByteBuffer::ByteBuffer(char *arr, std::size_t size)
    : m_rpos(0), m_wpos(0), m_buffer(size)
{
    m_buffer.reserve(size);
    // If the provided array is NULL, allocate a blank buffer of the provided size
    if (arr == NULL)
    {
    }
    else
    {
        putBytes(arr, size);
    }
}
ByteBuffer::ByteBuffer(ByteBuffer *byteBuffer)
{
    std::size_t size = byteBuffer->readableBytes();
    m_buffer.reserve(size);
    m_buffer.resize(size);
    m_rpos = 0;
    m_wpos = 0;
    putBytes(byteBuffer->peek(), size);
}

ByteBuffer::~ByteBuffer()
{
}
std::size_t ByteBuffer::bytesRemaining()
{
    return size() - m_rpos;
}
void luckyao::ByteBuffer::clear()
{
    m_rpos = 0;
    m_wpos = 0;
    memset(begin(), '\0', size());
}
/**
 * 从fd上读取数据  Poller工作在LT模式
 * Buffer缓冲区是有大小的！ 但是从fd上读数据的时候，却不知道tcp数据最终的大小
 */
ssize_t ByteBuffer::readFd(int fd, int *saveErrno)
{
    char extrabuf[65536] = {0}; // 栈上的内存空间  64K

    struct iovec vec[2];

    const size_t writable = writableBytes(); // 这是Buffer底层缓冲区剩余的可写空间大小
    vec[0].iov_base = begin() + m_wpos;
    vec[0].iov_len = writable;

    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;

    const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
    const ssize_t n = ::readv(fd, vec, iovcnt);
    if (n < 0)
    {
        *saveErrno = errno;
    }
    else if (n <= writable) // Buffer的可写缓冲区已经够存储读出来的数据了
    {
        m_wpos += n;
    }
    else // extrabuf里面也写入了数据
    {
        m_wpos = m_buffer.size();
        append(extrabuf, n - writable); // writerIndex_开始写 n - writable大小的数据
    }

    return n;
}

ssize_t ByteBuffer::writeFd(int fd, int *saveErrno)
{
    ssize_t n = ::write(fd, peek(), readableBytes());
    if (n < 0)
    {
        *saveErrno = errno;
    }
    return n;
}
// Read Functions

char ByteBuffer::get()
{
    return read<char>();
}

char ByteBuffer::get(std::size_t index)
{
    return read<char>(index);
}

bool ByteBuffer::getBytes(char *buf, std::size_t len)
{
    /*for (std::size_t i = 0; i < len; i++) {
        buf[i] = read<char>();
    }*/

    if (size() < (m_rpos + len))
        return false; // 读的内容太多了

    memcpy(buf, &m_buffer[m_rpos], len);

    m_rpos += len;
    return true;
}

char ByteBuffer::getChar()
{
    return read<char>();
}

char ByteBuffer::getChar(std::size_t index)
{
    return read<char>(index);
}

double ByteBuffer::getDouble()
{
    return read<double>();
}

double ByteBuffer::getDouble(std::size_t index)
{
    return read<double>(index);
}

float ByteBuffer::getFloat()
{
    return read<float>();
}

float ByteBuffer::getFloat(std::size_t index)
{
    return read<float>(index);
}

int ByteBuffer::getInt()
{
    return read<int>();
}

int ByteBuffer::getInt(std::size_t index)
{
    return read<int>(index);
}

std::size_t ByteBuffer::getUint()
{
    return read<std::size_t>();
}
long ByteBuffer::getLong()
{
    return read<long>();
}

long ByteBuffer::getLong(std::size_t index)
{
    return read<long>(index);
}

short ByteBuffer::getShort()
{
    return read<short>();
}

short ByteBuffer::getShort(std::size_t index)
{
    return read<short>(index);
}

// Write Functions

void ByteBuffer::put(ByteBuffer *src)
{
    int len = src->size();
    for (int i = 0; i < len; i++)
        append<char>(src->get(i));
}

void ByteBuffer::put(char b)
{
    append<char>(b);
}

void ByteBuffer::put(char b, std::size_t index)
{
    insert<char>(b, index);
}

void ByteBuffer::putBytes(const char *b, std::size_t len)
{
    // Insert the data one char at a time into the internal buffer at position i+starting index
    /*for (std::size_t i = 0; i < len; i++)
        append<char>(b[i]);*/

    if (size() < (m_wpos + len))
        m_buffer.resize(m_wpos + len);
    memcpy(begin() + m_wpos, b, len);
    // printf("writing %c to %i\n", (char)data, m_wpos);

    m_wpos += len;
}

void ByteBuffer::putBytes(char *b, std::size_t len, std::size_t index)
{
    m_wpos = index;

    // Insert the data one char at a time into the internal buffer at position i+starting index
    /*for (std::size_t i = 0; i < len; i++)
        append<char>(b[i]);*/
    putBytes(b, len);
}

void ByteBuffer::putChar(char value)
{
    append<char>(value);
}

void ByteBuffer::putChar(char value, std::size_t index)
{
    insert<char>(value, index);
}

void ByteBuffer::putDouble(double value)
{
    append<double>(value);
}

void ByteBuffer::putDouble(double value, std::size_t index)
{
    insert<double>(value, index);
}
void ByteBuffer::putFloat(float value)
{
    append<float>(value);
}

void ByteBuffer::putFloat(float value, std::size_t index)
{
    insert<float>(value, index);
}

void ByteBuffer::putInt(int value)
{
    append<int>(value);
}

void ByteBuffer::putInt(int value, std::size_t index)
{
    insert<int>(value, index);
}

void ByteBuffer::putUint(std::size_t value)
{
    append<std::size_t>(value);
}
void ByteBuffer::putLong(long value)
{
    append<long>(value);
}

void ByteBuffer::putLong(long value, std::size_t index)
{
    insert<long>(value, index);
}

void ByteBuffer::putShort(short value)
{
    append<short>(value);
}

void ByteBuffer::putShort(short value, std::size_t index)
{
    insert<short>(value, index);
}