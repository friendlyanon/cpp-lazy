#pragma once

#ifndef LZ_RANGE_HPP
#define LZ_RANGE_HPP

#include "detail/BasicIteratorView.hpp"
#include "detail/RangeIterator.hpp"


namespace lz {
    template<LZ_CONCEPT_ARITHMETIC Arithmetic>
    class Range final : public detail::BasicIteratorView<detail::RangeIterator<Arithmetic>> {
    public:
        using iterator = detail::RangeIterator<Arithmetic>;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using value_type = typename iterator::value_type;

    private:
        iterator _begin{};
        iterator _end{};

    public:

        /**
         * @brief Range iterator constructor from [start, end) with step.
         * @param start The start of the counting.
         * @param end The end of the counting.
         * @param step The step that gets added every iteration.
         */
        Range(const Arithmetic start, const Arithmetic end, const Arithmetic step) :
            _begin(start, step),
            _end(end, step) {
        }

        Range() = default;

        /**
         * @brief Returns the beginning of the random access Range iterator
         * @return The beginning of the random access Range iterator
         */
        iterator begin() const override {
            return _begin;
        }

        /**
         * @brief Returns the ending of the random access Range iterator
         * @return The ending of the random access Range iterator
         */
        iterator end() const override {
            return _end;
        }

        /**
         * @brief Returns the reverse beginning of the random access Range iterator
         * @return The reverse beginning of the random access Range iterator
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(begin());
        }

        /**
         * @brief Returns the reverse ending of the random access Range iterator
         * @return The reverse ending of the random access Range iterator
         */
        reverse_iterator rend() const {
            return reverse_iterator(end());
        }
    };

    // Start of group
    /**
     * @addtogroup ItFns
     * @{
     */

    /**
     * @brief Returns a random access range object with specified [start, end) and a step.
     * @details E.g. `lz::range(3, 20, 2)` will return all values between [3, 20) with a step of 2 when iterating over
     * its iterator.
     * @tparam Arithmetic Int is automatically assumed, may be any arithmetic type.
     * @param end Specifies when to stop iterator after `end` count. It is assumed from [start, end) with a step.
     * @param step The incrementing value after each loop.
     * @param start Specifies where to start counting.
     * @return A Range object that can be converted to an arbitrary container or can be iterated over using
     * `for (auto... lz::range(...))`.
     */
    template<LZ_CONCEPT_ARITHMETIC Arithmetic = int>
    Range<Arithmetic> range(const Arithmetic start, const Arithmetic end, const Arithmetic step = 1) {
        static_assert(std::is_arithmetic<Arithmetic>::value, "type must be of type arithmetic");
        if (step == 0) {
            throw std::range_error(LZ_FILE_LINE ": with a step size of 0, the sequence can never end");
        }
        if (start > end && step >= 1) {
            throw std::range_error(LZ_FILE_LINE ": with a step of 1 or bigger and begin greater than end, the sequence can never end");
        }
        if (end > start && step < 0) {
            throw std::range_error(
                LZ_FILE_LINE ": with a negative step size and begin start smaller than end, the sequence can never end");
        }
        return Range<Arithmetic>(start, end, step);
    }

    /**
     * @brief Returns a random access range object.
     * @details E.g. `lz::range(50)` will return all values between [0, 50) when iterating over its iterator.
     * @tparam Arithmetic Int is automatically assumed, may be any arithmetic type.
     * @param end Specifies when to stop iterator after `end` count. It is assumed from [0, end).
     * @return A Range object that can be converted to an arbitrary container or can be iterated over using
     * `for (auto... lz::range(...))`.
     */
    template<LZ_CONCEPT_ARITHMETIC Arithmetic = int>
    Range<Arithmetic> range(const Arithmetic end) {
        return range<Arithmetic>(0, end, 1);
    }

    // End of group
    /**
     * @}
     */
}

#endif