#pragma once
namespace luckyao
{
    class Uncopyable
    {
    public:
        Uncopyable(const Uncopyable &) = delete;
        Uncopyable &operator=(const Uncopyable &) = delete;

    protected:
        Uncopyable() = default;
        ~Uncopyable() = default;
    };
}
