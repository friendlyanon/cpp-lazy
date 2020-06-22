#pragma once

#include <iterator>


namespace lz {  namespace it { namespace detail {
    template<class Arithmetic>
    class RangeIterator {
        Arithmetic _iterator{};
        Arithmetic _end{};
        Arithmetic _step{};

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = Arithmetic;
        using difference_type = Arithmetic;
        using pointer = const Arithmetic*;
        using reference = Arithmetic;

        RangeIterator(Arithmetic begin, Arithmetic end, Arithmetic step) :
            _iterator(begin),
            _end(end),
            _step(step) {
        }

        value_type operator*() const {
            return _iterator;
        }

        pointer operator->() const {
            return &_iterator;
        }

        RangeIterator& operator++() {
            _iterator += _step;
            return *this;
        }

        RangeIterator& operator--() {
            _iterator -= _step;
            return *this;
        }

        RangeIterator& operator+=(const difference_type offset) {
            _iterator += (offset * _step);
            return *this;
        }

        RangeIterator operator+(const difference_type offset) const {
            auto tmp = *this;
            return tmp += offset;
        }

        RangeIterator& operator-=(const difference_type offset) {
            _iterator -= (offset * _step);
            return *this;
        }

        RangeIterator operator-(const difference_type other) const {
            auto tmp = *this;
            return tmp -= other;
        }

        difference_type operator-(const RangeIterator& other) const {
            return other._end - _iterator;
        }

        value_type operator[](const difference_type offset) const {
            return *(*this + offset);
        }

        bool operator!=(const RangeIterator& other) const {
            if (_step < 0) {
                return _iterator > other._end;
            }
            return _iterator < other._end;
        }

        bool operator==(const RangeIterator& other) const {
            return !(*this != other);
        }

        bool operator<(const RangeIterator& other) const {
            return _iterator < other._end;
        }

        bool operator>(const RangeIterator& other) const {
            return other < *this;
        }

        bool operator<=(const RangeIterator& other) const {
            return !(other < *this);
        }

        bool operator>=(const RangeIterator& other) const {
            return !(*this < other);
        }
    };
}}}