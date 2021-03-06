#pragma once

#ifndef LZ_GENERATE_ITERATOR_HPP
#define LZ_GENERATE_ITERATOR_HPP

#include "LzTools.hpp"

#include <iterator>


namespace lz { namespace detail {
    template<LZ_CONCEPT_INVOCABLE GeneratorFunc>
    class GenerateIterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = FunctionReturnType<GeneratorFunc>;
        using difference_type = std::size_t;
        using reference = value_type;
        using pointer = FakePointerProxy<value_type>;

    private:
        std::size_t _current{};
        GeneratorFunc _generator{};
        bool _isWhileTrueLoop{};

    public:
        GenerateIterator() = default;

        GenerateIterator(const std::size_t start, const GeneratorFunc& generatorFunc, const bool isWhileTrueLoop) :  // NOLINT(modernize-pass-by-value)
            _current(start),
            _generator(generatorFunc),
            _isWhileTrueLoop(isWhileTrueLoop)
        {}

        value_type operator*() const {
            return _generator();
        }

        pointer operator->() const {
            return FakePointerProxy<decltype(**this)>(**this);
        }

        GenerateIterator& operator++() {
            if (!_isWhileTrueLoop) {
                ++_current;
            }
            return *this;
        }

        GenerateIterator operator++(int) {
            GenerateIterator tmp(*this);
            ++*this;
            return tmp;
        }

        GenerateIterator& operator--() {
            if (!_isWhileTrueLoop) {
                --_current;
            }
            return *this;
        }

        GenerateIterator operator--(int) {
            GenerateIterator tmp(*this);
            --*this;
            return tmp;
        }

        GenerateIterator& operator+=(const difference_type offset) {
            if (!_isWhileTrueLoop) {
                _current += offset;
            }
            return *this;
        }

        GenerateIterator& operator-=(const difference_type offset) {
            if (!_isWhileTrueLoop) {
                _current -= offset;
            }
            return *this;
        }

        GenerateIterator operator+(const difference_type offset) const {
            GenerateIterator tmp(*this);
            tmp += offset;
            return tmp;
        }

        GenerateIterator operator-(const difference_type offset) const {
            GenerateIterator tmp(*this);
            tmp -= offset;
            return tmp;
        }

        difference_type operator-(const GenerateIterator& other) const {
            return _current - other._current;
        }

        value_type operator[](const difference_type offset) const {
            return *(*this + offset);
        }

        bool operator==(const GenerateIterator& other) const {
            return !(*this != other);
        }

        bool operator!=(const GenerateIterator& other) const {
            return _current != other._current;
        }

        bool operator<(const GenerateIterator& other) const {
            return _current < other._current;
        }

        bool operator>(const GenerateIterator& other) const {
            return other < *this;
        }

        bool operator<=(const GenerateIterator& other) const {
            return !(other < *this);
        }

        bool operator>=(const GenerateIterator& other) const {
            return !(*this < other);
        }
    };
}}

#endif