#pragma once

#include <concepts>
#include <utility>
#include <algorithm>

namespace stl
{
    template <typename T>
    inline constexpr bool invalid_expected_type_v =
        std::is_reference_v<T> || std::is_function_v<T> || std::is_array_v<T> || (!std::is_same_v<T, void> && !std::is_destructible_v<T>) || std::is_same_v<T, std::in_place_t>;

    struct unexpect_t
    {
        explicit unexpect_t() = default;
    };
    inline constexpr unexpect_t unexpect{};

    namespace internal
    {
        template <typename T, typename E>
        class expected_storage;
    }

    template <typename T, typename E>
    class expected
    {
        static_assert(!invalid_expected_type_v<T>, "stl::expected<T,E> invalid T.");
        static_assert(!invalid_expected_type_v<E>, "stl::expected<T,E> invalid E.");

    public:
        using value_type = T;
        using error_type = E;
        using unexpected_type = unexpected<E>;

        constexpr expected() : expected(std::in_place)
        {
        }

        constexpr expected(const expected &other) : _storage(other._storage)
        {
        }

        constexpr expected(expected &&other) : _storage(std::move(other._storage))
        {
        }

        template <typename U>
        constexpr expected(U &&u)
        {
            if constexpr (std::constructible_from<T, U>)
                _storage.construct_expected(std::forward<U>(u));
            else if constexpr (std::constructible_from<E, U>)
                _storage.construct_unexpected(std::forward<U>(u));
            else
                static_assert(false, "stl::expected<T,U> ambiguous constructor.");
        }

        constexpr expected &operator=(const expected &other)
        {
            _storage = other._storage;
            return *this;
        }

        constexpr expected &operator=(expected &&other)
        {
            _storage = std::move(other._storage);
            return *this;
        }

        template <typename... Args>
        constexpr explicit expected(std::in_place_t, Args &&...args)
        {
            _storage.construct_expected(std::forward<Args>(args)...);
        }

        template <typename... Args>
        constexpr explicit expected(unexpect_t, Args &&...args)
        {
            _storage.construct_unexpected(std::forward<Args>(args)...);
        }

        constexpr const T *operator->() const noexcept
        {
            return std::addressof(_storage.get_expected());
        }

        constexpr T *operator->() noexcept
        {
            return std::addressof(_storage.get_expected());
        }

        constexpr decltype(auto) operator*() const & noexcept
        {
            return _storage.get_expected();
        }

        constexpr decltype(auto) operator*() & noexcept
        {
            return _storage.get_expected();
        }

        constexpr decltype(auto) operator*() const && noexcept
        {
            return _storage.get_expected();
        }

        constexpr decltype(auto) operator*() && noexcept
        {
            return _storage.get_expected();
        }

        constexpr explicit operator bool() const noexcept
        {
            return has_value();
        }

        constexpr bool has_value() const noexcept
        {
            return _storage.has_value();
        }

        constexpr decltype(auto) value() &
        {
            return _storage.get_expected();
        }
        constexpr decltype(auto) value() const &
        {
            return _storage.get_expected();
        }
        constexpr decltype(auto) value() &&
        {
            return _storage.get_expected();
        }
        constexpr decltype(auto) value() const &&
        {
            return _storage.get_expected();
        }

        constexpr decltype(auto) error() &
        {
            return _storage.get_unexpected();
        }
        constexpr decltype(auto) error() const &
        {
            return _storage.get_unexpected();
        }
        constexpr decltype(auto) error() &&
        {
            return _storage.get_unexpected();
        }
        constexpr decltype(auto) error() const &&
        {
            return _storage.get_unexpected();
        }

        template <typename U = std::remove_cv_t<T>>
        constexpr T value_or(U &&default_value) const &
        {
            return has_value() ? value() : std::forward<U>(default_value);
        }

        template <typename U = std::remove_cv_t<T>>
        constexpr T value_or(U &&default_value) &&
        {
            return has_value() ? value() : std::forward<U>(default_value);
        }

        template <typename U = std::remove_cv_t<E>>
        constexpr E error_or(U &&default_value) const &
        {
            return !has_value() ? error() : std::forward<U>(default_value);
        }

        template <typename U = std::remove_cv_t<E>>
        constexpr E error_or(U &&default_value) &&
        {
            return !has_value() ? error() : std::forward<U>(default_value);
        }

    private:
        internal::expected_storage<T, E> _storage;
    };

    namespace internal
    {
        template <typename T, typename E>
        class expected_storage
        {
            alignas(std::max({alignof(T), alignof(E)})) unsigned char _mem[std::max({sizeof(T), sizeof(E)})];
            bool _has_value : 4 = false;
            bool _has_error : 4 = false;

            void clear()
            {
                if constexpr (!std::is_trivially_destructible_v<T>)
                {
                    if (_has_value)
                        get_expected().~T();
                }

                if constexpr (!std::is_trivially_destructible_v<E>)
                {
                    if (_has_error)
                        get_unexpected().~E();
                }
            }

        public:
            constexpr expected_storage() {}

            constexpr expected_storage(expected_storage const &other)
            {
                *this = other;
            }

            constexpr expected_storage(expected_storage &&other)
            {
                *this = std::move(other);
            }

            constexpr expected_storage &operator=(expected_storage const &other)
            {
                clear();
                if constexpr (std::is_trivially_copyable_v<T> && std::is_trivially_copyable_v<E>)
                {
                    if (other._has_value || other._has_error)
                        memcpy(_mem, other._mem, std::size(_mem));
                }
                else
                {
                    if (other._has_value)
                        construct_expected(other.get_expected());
                    else if (other._has_error)
                        construct_unexpected(other.get_unexpected());
                }
                other.clear();
                return *this;
            }

            constexpr expected_storage &operator=(expected_storage &&other)
            {
                clear();
                if constexpr (std::is_trivially_copyable_v<T> && std::is_trivially_copyable_v<E>)
                {
                    if (other._has_value || other._has_error)
                        memcpy(_mem, other._mem, std::size(_mem));
                }
                else
                {
                    if (other._has_value)
                        construct_expected(std::move(other.get_expected()));
                    else if (other._has_error)
                        construct_unexpected(std::move(other.get_unexpected()));
                }
                other.clear();
                return *this;
            }

            constexpr ~expected_storage()
            {
                clear();
            }

            template <typename... Args>
            constexpr void construct_expected(Args &&...args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
            {
                static_assert(std::is_constructible_v<T, Args...>);
                new (_mem) T(std::forward<Args>(args)...);
                _has_value = true;
            }

            template <typename... Args>
            constexpr void construct_unexpected(Args &&...args) noexcept(std::is_nothrow_constructible_v<E, Args...>)
            {
                static_assert(std::is_constructible_v<E, Args...>);
                new (_mem) E(std::forward<Args>(args)...);
                _has_error = true;
            }

            constexpr bool has_value() const
            {
                return _has_value;
            }

            constexpr bool has_error() const
            {
                return _has_error;
            }

            constexpr T &get_expected() &
            {
                if (!_has_value)
                    throw std::logic_error{"No expected type has been constructed."};
                return *std::bit_cast<T *>((void *)_mem);
            }

            constexpr E &get_unexpected() &
            {
                if (!_has_error)
                    throw std::logic_error{"No unexpected type has been constructed."};
                return *std::bit_cast<E *>((void *)_mem);
            }

            constexpr T const &get_expected() const &
            {
                if (!_has_value)
                    throw std::logic_error{"No expected type has been constructed."};
                return *std::bit_cast<T const *>((void *)_mem);
            }

            constexpr E const &get_unexpected() const &
            {
                if (!_has_error)
                    throw std::logic_error{"No unexpected type has been constructed."};
                return *std::bit_cast<E const *>((void *)_mem);
            }

            constexpr T &&get_expected() &&
            {
                if (!_has_value)
                    throw std::logic_error{"No expected type has been constructed."};
                return std::move(*std::bit_cast<T const *>((void *)_mem));
            }

            constexpr E &&get_unexpected() &&
            {
                if (!_has_error)
                    throw std::logic_error{"No unexpected type has been constructed."};
                return std::move(*std::bit_cast<E const *>((void *)_mem));
            }
        };

        template <typename E>
        class expected_storage<void, E>
        {
            alignas(E) unsigned char _mem[sizeof(E)];
            bool _has_error = false;

            void clear()
            {
                if constexpr (!std::is_trivially_destructible_v<E>)
                {
                    if (_has_error)
                        get_unexpected().~E();
                }
            }

        public:
            constexpr expected_storage() {}

            constexpr expected_storage(expected_storage const &other)
            {
                *this = other;
            }

            constexpr expected_storage(expected_storage &&other)
            {
                *this = std::move(other);
            }

            constexpr expected_storage &operator=(expected_storage const &other)
            {
                clear();
                if constexpr (std::is_trivially_copyable_v<E>)
                {
                    if (other._has_error)
                        memcpy(_mem, other._mem, std::size(_mem));
                }
                else
                {
                    if (other._has_error)
                        construct_unexpected(other.get_unexpected());
                }
                return *this;
            }

            constexpr expected_storage &operator=(expected_storage &&other)
            {
                clear();
                if constexpr (std::is_trivially_copyable_v<E>)
                {
                    if (other._has_error)
                        memcpy(_mem, other._mem, std::size(_mem));
                }
                else
                {
                    if (other._has_error)
                        construct_unexpected(std::move(other.get_unexpected()));
                }
                return *this;
            }

            constexpr ~expected_storage()
            {
                clear();
            }

            template <typename... Args>
            constexpr void construct_expected(Args &&...args)
            {
            }

            template <typename... Args>
            constexpr void construct_unexpected(Args &&...args) noexcept(std::is_nothrow_constructible_v<E, Args...>)
            {
                static_assert(std::is_constructible_v<E, Args...>);
                new (_mem) E(std::forward<Args>(args)...);
                _has_error = true;
            }

            constexpr bool has_value() const
            {
                return false;
            }

            constexpr bool has_error() const
            {
                return _has_error;
            }

            constexpr void get_expected() const
            {
                static_assert(false, "stl::expected<T, E> can't get T, with T = void.");
            }

            constexpr E &get_unexpected() &
            {
                if (!_has_error)
                    throw std::logic_error{"No unexpected type has been constructed."};
                return *std::bit_cast<E *>((void *)_mem);
            }

            constexpr E const &get_unexpected() const &
            {
                if (!_has_error)
                    throw std::logic_error{"No unexpected type has been constructed."};
                return *std::bit_cast<E const *>((void *)_mem);
            }

            constexpr E &&get_unexpected() &&
            {
                if (!_has_error)
                    throw std::logic_error{"No unexpected type has been constructed."};
                return std::move(*std::bit_cast<E *>((void *)_mem));
            }
        };
    }
}
