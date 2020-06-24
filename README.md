[![Build Status](https://travis-ci.com/MarcDirven/cpp-lazy.svg?branch=master)](https://travis-ci.com/MarcDirven/cpp-lazy) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
# cpp-lazy
Cpp-lazy is a fast lazy evaluation library for C++14/17/20. It makes extended use of STL iterators. Current supported iterators are:
- Enumerate
- Filter
- Map
- Range
- StringSplitter
- Take/slice/take range/take while
- Zip

Cpp-lazy is a set of iterators that allocate 0 bytes of memory on the heap (if >= C++17, else `lz::split` does allocate substrings), making it a very cheap and fast operation. Another reason the iterators are fast is because the iterators are random acces iterators where possible. This makes operations such as `std::distance` an O(1) operation. 

An example: if you want to iterate over multiple containers at the same time, you can use the `lz::zip` function to do this.
For C++17, [structured bindings](https://www.google.com/search?client=firefox-b-d&q=structured+bindings+c%2B%2B) can be used to acces the elements.
```cpp
std::vector<int> a;
std::vector<float> b;
const std::vector<double> c;

for (auto [i, f, d] : lz::zip(a, b, c)) {
  // i, f and d are accesed by reference. Assigning these values 
  // will alter the values in its corresponding container.
}
```
For C++14, this will work slightly different:
```cpp
std::vector<int> a;
std::vector<float> b;
const std::vector<double> c;

for (auto tup : lz::zip(a, b, c)) {
  std::get<0>(tup) // acces container element a (by reference)
  std::get<1>(tup) // acces container element b (by reference)
  std::get<2>(tup) // acces container element c (by reference)
}
```
All the iterator objects also contain a `toVector()` function to put the containers (in this case) into a tuple of `int, float, double`. For example:
```
std::vector<std::tuple<int, float, double>> v = lz::zip(a, b, c).toVector();
```


The library comes with other handy iterators aswell, for example the `lz::map` random acces iterator. This iterator selects a value from a given type, in this example we will use `Struct` as type, and select al `Struct::s` fields. Example:
```cpp
struct Struct {
  std::string data;
  int moreData;
};

std::vector<Struct> structs;
auto mapped = lz::map(structs, [](const Struct& s) { return s.data; });
for (const std::string& s : mapped) {
  // s contains Struct::s
}
```
If one would like to put the `mapped` into a vector, then this is accomplished by doing `mapped.toVector()` just like in the previous case, making the return type of the `toVector()` method a `std::vector<std::string>>`.


Another handy iterator is the `lz::split` forward iterator. This can be used to lazy-evaluate split a string on a given delimiter. For example in C++17 that will be:
```cpp
std::string toSplit = "Hello world";

for (std::string_view word : lz::split(toSplit, " ")) {
  std::cout << word << '\n';
}
// prints:
// Hello
// world
```
In C++14 however, that will be
```cpp
In C++14 that will be:
std::string toSplit = "Hello world";

for (std::string& word : lz::split(toSplit, " ")) {
  std::cout << word << '\n'; // one can use std::move(word) safely
}
// prints:
// Hello
// world
```
making this version a bit slower than the C++17 splitter, because this splitter needs to allocate an extra substring.


Every iterator (except zip) has a `"[iterator-name]range` and `[iterator-name]` equivalent. Example, the `enumerate` and the `enumeraterange` functions. The first funtion takes an iterable/container, the latter takes two iterators (begin and end).

# Benchmarks cpp-lazy
Time in nanoseconds

<div style="text-align:center"><img src="https://i.imgur.com/i8UNtz8.png" /></div>

<div style="text-align:center"><img src="https://i.imgur.com/tuKCTzM.png" /></div>

<div style="text-align:center"><img src="https://i.imgur.com/NG1gV8h.png" /></div>
