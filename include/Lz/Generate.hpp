#pragma once

#ifndef LZ_GENERATE_HPP
#define LZ_GENERATE_HPP

#include "detail/BasicIteratorView.hpp"
#include "detail/GenerateIterator.hpp"


namespace lz {
    template<LZ_CONCEPT_INVOCABLE GeneratorFunc>
    class Generate final : public internal::BasicIteratorView<internal::GenerateIterator<GeneratorFunc>> {
    public:
        using iterator = internal::GenerateIterator<GeneratorFunc>;
        using const_iterator = iterator;
        using value_type = typename std::iterator_traits<iterator>::value_type;

        /**
         * @brief Generator constructor.
         * @details Creates a generator object. Executes `func` `amount` of times, and returns the value of this
         * function.
         * @param func The function to execute `amount` of times.
         * @param amount The amount of times to execute. If `amount` is equal to `std::numeric_limits<size_t>::max()`
         * it is interpreted as a `while-true` loop.
         */
        Generate(const GeneratorFunc& func, const std::size_t amount):
            internal::BasicIteratorView<iterator>(iterator(0, func, amount == std::numeric_limits<std::size_t>::max()),
                                                  iterator(amount, func, amount == std::numeric_limits<std::size_t>::max()))
        {
        }

        Generate() = default;
    };

    /**
     * @addtogroup ItFns
     * @{
     */

    /**
     * @brief Returns a view to a generate iterator.
     * @details This object can be used to generate `amount` of values, generated by the `generatorFunc` function.
     * Example:
     * ```cpp
     * int a = 0;
     * size_t amount = 4;
     * auto vector = lz::generate([&a]() { return a++; }, amount).toVector();
     * // vector yields: { 0, 1, 2, 3 }
     * @param generatorFunc The function to execute `amount` of times. The return value of the function is the type
     * that is generated.
     * @param amount The amount of times to execute `generatorFunc`.
     * @return A generator random access iterator view object.
     */
    template<LZ_CONCEPT_INVOCABLE GeneratorFunc>
    Generate<GeneratorFunc> generate(const GeneratorFunc& generatorFunc, const std::size_t amount = std::numeric_limits<std::size_t>::max()) {
        return Generate<GeneratorFunc>(generatorFunc, amount);
    }


    // End of group
    /**
     * @}
     */
}

#endif