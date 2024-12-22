/*
 * @Author: kunger
 * @Date: 2023-06-09 20:16:21
 * @Last Modified by: kunger
 * @Last Modified time: 2023-06-26 10:50:24
 */
#pragma once
namespace luckyao
{
    template <typename T>
    class Singleton
    {
    public:
        static T *instance()
        {
            if (m_instance == NULL)
                m_instance = new T();
            return m_instance;
        }

    private:
        Singleton() {}
        Singleton(const Singleton<T> &);
        Singleton<T> &operator=(const Singleton<T> &);
        ~Singleton() {}

    private:
        static T *m_instance;
    };

    template <typename T>
    T *Singleton<T>::m_instance = NULL;

}
