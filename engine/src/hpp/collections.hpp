#ifndef ENDGEN_COLLECTIONS
#define ENDGEN_COLLECTIONS

template <typename T>
class Singleton
{
    public:
        static T& Instance() {
            static T instance;
            return instance;
        };

        Singleton(const Singleton&) = delete;               // prohibit copy ctor
        Singleton(Singleton&&) = delete;                    // prohibit move ctor
        Singleton& operator=(const Singleton&) = delete;    // prohibit assignment

    protected:
        Singleton() = default;
        ~Singleton() = default;
};

#endif