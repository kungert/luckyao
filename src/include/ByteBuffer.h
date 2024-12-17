#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cstring>

namespace luckyao
{
    // 网络库底层的缓冲器类型定义
    class ByteBuffer
    {
    public:
        static const size_t kInitialSize = 1024;

        explicit ByteBuffer(size_t initialSize = kInitialSize)
            : m_buffer(initialSize), m_rpos(0), m_wpos(0)
        {
        }
        ByteBuffer(char *arr, std::size_t size);

        ByteBuffer(ByteBuffer *byteBuffer);

        virtual ~ByteBuffer();

        std::size_t bytesRemaining();

        void clear();

        ByteBuffer *clone();

        bool equals(ByteBuffer *other);

        void resize(std::size_t newSize);

        void replace(char key, char rep, std::size_t start = 0, bool firstOccuranceOnly = false);

        char get();                                // Relative get method. Reads the char at the buffers current position then increments the position
        char get(std::size_t index);               // Absolute get method. Read char at index
        bool getBytes(char *buf, std::size_t len); // Absolute read into array buf of length len
        char getChar();                            // Relative
        char getChar(std::size_t index);           // Absolute
        double getDouble();
        double getDouble(std::size_t index);
        float getFloat();
        float getFloat(std::size_t index);
        int getInt();
        int getInt(std::size_t index);
        std::size_t getUint();
        long getLong();
        long getLong(std::size_t index);
        short getShort();
        short getShort(std::size_t index);

        // Write

        void put(ByteBuffer *src);                                  // Relative write of the entire contents of another ByteBuffer (src)
        void put(char b);                                           // Relative write
        void put(char b, std::size_t index);                        // Absolute write at index
        void putBytes(const char *b, std::size_t len);              // Relative write
        void putBytes(char *b, std::size_t len, std::size_t index); // Absolute write starting at index
        void putChar(char value);                                   // Relative
        void putChar(char value, std::size_t index);                // Absolute
        void putDouble(double value);
        void putDouble(double value, std::size_t index);
        void putFloat(float value);
        void putFloat(float value, std::size_t index);
        void putInt(int value);
        void putInt(int value, std::size_t index);
        void putUint(std::size_t value);
        void putLong(long value);
        void putLong(long value, std::size_t index);
        void putShort(short value);
        void putShort(short value, std::size_t index);

        // ByteBuffer Position Accessors & Mutators

        void setReadPos(std::size_t r) { m_rpos = r; }

        std::size_t getReadPos() { return m_rpos; }

        void setWritePos(std::size_t w) { m_wpos = w; }

        std::size_t getWritePos() { return m_wpos; }
        size_t readableBytes() const
        {
            return m_wpos - m_rpos;
        }

        size_t writableBytes() const
        {
            return m_buffer.size() - m_wpos;
        }

        size_t prependableBytes() const
        {
            return m_rpos;
        }

        std::size_t size() { return m_buffer.size(); }
        // 返回缓冲区中可读数据的起始地址
        char *peek()
        {
            return begin() + m_rpos;
        }

        // onMessage string <- ByteBuffer
        void retrieve(std::size_t len)
        {
            if (len < readableBytes())
            {
                m_rpos += len; // 应用只读取了刻度缓冲区数据的一部分，就是len，还剩下readerIndex_ += len -> m_wpos
            }
            else // len == readableBytes()
            {
                retrieveAll();
            }
        }

        void retrieveAll()
        {
            m_rpos = m_wpos = 0;
        }

        // 把onMessage函数上报的Buffer数据，转成string类型的数据返回
        std::string retrieveAllAsString()
        {
            return retrieveAsString(readableBytes()); // 应用可读取数据的长度
        }

        std::string retrieveAsString(size_t len)
        {
            std::string result(peek(), len);
            retrieve(len); // 上面一句把缓冲区中可读的数据，已经读取出来，这里肯定要对缓冲区进行复位操作
            return result;
        }

        // m_buffer.size() - m_wpos    len
        void ensureWriteableBytes(size_t len)
        {
            if (writableBytes() < len)
            {
                makeSpace(len); // 扩容函数
            }
        }

        // 把[data, data+len]内存上的数据，添加到writable缓冲区当中
        void append(const char *data, size_t len)
        {
            ensureWriteableBytes(len);
            std::copy(data, data + len, beginWrite());
            m_wpos += len;
        }
        template <typename T>
        void append(T data)
        {
            std::size_t s = sizeof(data);

            if (size() < (m_wpos + s))
                m_buffer.resize(m_wpos + s);
            memcpy((char *)&m_buffer[m_wpos], (char *)&data, s);
            // printf("writing %c to %i\n", (byte)data, m_wpos);

            m_wpos += s;
        }

        char *beginWrite()
        {
            return begin() + m_wpos;
        }

        // 从fd上读取数据
        ssize_t readFd(int fd, int *saveErrno);
        // 通过fd发送数据
        ssize_t writeFd(int fd, int *saveErrno);

        char *begin()
        {
            return &*m_buffer.begin();
        }
        void makeSpace(size_t len)
        {
            if (writableBytes() + prependableBytes() < len)
            {
                m_buffer.resize(m_wpos + len);
            }
            else
            {
                size_t readalbe = readableBytes();
                std::copy(begin() + m_rpos,
                          begin() + m_wpos,
                          begin());
                m_rpos = 0;
                m_wpos = m_rpos + readalbe;
            }
        }
        // Basic Searching (Linear)
        template <typename T>
        int find(T key, std::size_t start = 0)
        {
            int ret = -1;
            std::size_t len = m_buffer.size();
            for (std::size_t i = start; i < len; i++)
            {
                T data = read<T>(i);
                // Wasn't actually found, bounds of buffer were exceeded
                if ((key != 0) && (data == 0))
                    break;

                // Key was found in array
                if (data == key)
                {
                    ret = i;
                    break;
                }
            }
            return ret;
        }

        template <typename T>
        void insert(T data, std::size_t index)
        {
            if ((index + sizeof(data)) > size())
            {
                m_buffer.resize(size() + (index + sizeof(data)));
            }

            memcpy(&m_buffer[index], (char *)&data, sizeof(data));
            m_wpos = index + sizeof(data);
        }

    private:
        template <typename T>
        T read()
        {
            T data = read<T>(m_rpos);
            m_rpos += sizeof(T);
            return data;
        }

        template <typename T>
        T read(std::size_t index) const
        {
            if (index + sizeof(T) <= m_buffer.size())
                return *((T *)&m_buffer[index]);
            return 0;
        }

    private:
        std::vector<char> m_buffer;
        size_t m_rpos;
        size_t m_wpos;
    };
}