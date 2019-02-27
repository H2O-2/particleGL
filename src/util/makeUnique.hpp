#include <memory>

// Implementation of std::make_unique from https://herbsutter.com/gotw/_102/

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
