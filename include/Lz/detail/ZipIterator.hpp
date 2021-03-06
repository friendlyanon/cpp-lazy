#pragma once

#ifndef LZ_ZIP_ITERATOR_HPP
#define LZ_ZIP_ITERATOR_HPP

#include <iterator>
#include <algorithm>
#include "LzTools.hpp"


namespace lz { namespace detail {
    template<LZ_CONCEPT_ITERATOR... Iterators>
    class ZipIterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = std::tuple<typename std::iterator_traits<Iterators>::value_type...>;
        using difference_type = std::ptrdiff_t;
        using reference = std::tuple<typename std::iterator_traits<Iterators>::reference...>;
        using pointer = FakePointerProxy<reference>;

    private:
        using MakeIndexSequenceForThis = MakeIndexSequence<sizeof...(Iterators)>;
        std::tuple<Iterators...> _iterators{};

        template<std::size_t... I>
        reference dereference(IndexSequence<I...>) const {
            return reference{*std::get<I>(_iterators)...};
        }

        template<std::size_t... I>
        void increment(IndexSequence<I...>) {
            const std::initializer_list<int> expand = {(++std::get<I>(_iterators), 0)...};
            static_cast<void>(expand);
        }

        template<std::size_t... I>
        void decrement(IndexSequence<I...>) {
            const std::initializer_list<int> expand = {(--std::get<I>(_iterators), 0)...};
            static_cast<void>(expand);
        }

        template<std::size_t... I>
        void plusIs(IndexSequence<I...>, const difference_type differenceType) {
            const std::initializer_list<int> expand = {
                (std::get<I>(_iterators) = std::next(std::get<I>(_iterators), differenceType), 0)...};
            static_cast<void>(expand);
        }

        template<std::size_t... I>
        void minIs(IndexSequence<I...>, const difference_type differenceType) {
            const std::initializer_list<int> expand = {
                (std::get<I>(_iterators) = std::prev(std::get<I>(_iterators), differenceType), 0)...};
            static_cast<void>(expand);
        }

        template<std::size_t... I>
        difference_type iteratorMin(IndexSequence<I...>, const ZipIterator& other) const {
            const std::initializer_list<difference_type> diff =
                {static_cast<difference_type>((std::distance(std::get<I>(other._iterators), std::get<I>(_iterators))))...};
            return static_cast<difference_type>(std::min(diff));
        }

        template<std::size_t... I>
        bool lessThan(IndexSequence<I...>, const ZipIterator& other) const {
            const std::initializer_list<difference_type> distances = {
                (std::distance(std::get<I>(_iterators), std::get<I>(other._iterators)))...};
            return std::find_if(distances.begin(), distances.end(), [](const difference_type diff) {
                return diff > 0;
            }) != distances.end();
        }

        template<std::size_t... I>
        bool notEqual(IndexSequence<I...>, const ZipIterator& other) const {
            const std::initializer_list<bool> boolValues = {(std::get<I>(_iterators) != std::get<I>(other._iterators))...};
            const auto* const end = boolValues.end();
            // Check if false not in boolValues
            return std::find(boolValues.begin(), end, false) == end;
        }

    public:
        explicit ZipIterator(const std::tuple<Iterators...>& iterators) :  // NOLINT(modernize-pass-by-value)
            _iterators(iterators) {
        }

        ZipIterator() = default;

        reference operator*() const {
            return dereference(MakeIndexSequenceForThis());
        }

        pointer operator->() const {
            return FakePointerProxy<decltype(**this)>(**this);
        }

        ZipIterator& operator++() {
            increment(MakeIndexSequenceForThis());
            return *this;
        }

        ZipIterator operator++(int) {
            ZipIterator tmp(*this);
            ++*this;
            return tmp;
        }

        ZipIterator& operator--() {
            decrement(MakeIndexSequenceForThis());
            return *this;
        }

        ZipIterator operator--(int) {
            auto tmp(*this);
            --*this;
            return tmp;
        }

        ZipIterator& operator+=(const difference_type offset) {
            plusIs(MakeIndexSequenceForThis(), offset);
            return *this;
        }

        ZipIterator operator+(const difference_type offset) const {
            ZipIterator tmp(*this);
            tmp += offset;
            return tmp;
        }

        ZipIterator& operator-=(const difference_type offset) {
            minIs(MakeIndexSequenceForThis(), offset);
            return *this;
        }

        ZipIterator operator-(const difference_type offset) const {
            ZipIterator tmp(*this);
            tmp -= offset;
            return tmp;
        }

        difference_type operator-(const ZipIterator& other) const {
            return iteratorMin(MakeIndexSequenceForThis(), other);
        }

        reference operator[](const difference_type offset) const {
            return *(*this + offset);
        }

        bool operator==(const ZipIterator& other) const {
            return !(*this != other);
        }

        bool operator!=(const ZipIterator& other) const {
            return notEqual(MakeIndexSequenceForThis(), other);
        }

        bool operator<(const ZipIterator& other) const {
            return lessThan(MakeIndexSequenceForThis(), other);
        }

        bool operator>(const ZipIterator& other) const {
            return other < *this;
        }

        bool operator<=(const ZipIterator& other) const {
            return !(other < *this);
        }

        bool operator>=(const ZipIterator& other) const {
            return !(*this < other);
        }
    };
}}

#endif