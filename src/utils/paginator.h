#pragma once

#include "iterator_range.h"

#include <numeric>
#include <iostream>
#include <vector>
#include <string>

template <typename Iterator>
class Paginator {
  std::vector<IteratorRange<Iterator>> pages;
  const size_t page_size_;

 public:
  explicit Paginator(Iterator begin, Iterator end, size_t page_size) : page_size_(page_size) {
    size_t dist = std::distance(begin, end);
    for (size_t i = 0; i < dist; i += page_size) {
      auto it_page_beg = begin + i;
      auto it_page_end = begin + std::min (dist, (i + page_size));
      pages.push_back(IteratorRange{it_page_beg, it_page_end});
    }
  }

  auto begin() {
    return pages.begin();
  }

  auto end() {
    return pages.end();
  }

  auto begin() const {
    return pages.begin();
  }

  auto end() const {
    return pages.end();
  }

  [[nodiscard]] size_t size() const {
    return pages.size();
  }
};

template <typename Container>
auto Paginate(Container& c, size_t page_size) {
  return Paginator{begin(c), end(c), page_size};
}