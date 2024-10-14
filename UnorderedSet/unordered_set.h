#ifndef UNORDERED_SET
#define UNORDERED_SET

#include <algorithm>
#include <iterator>
#include <functional>
#include <list>
#include <vector>

template <typename KeyT, typename Hash = std::hash<KeyT>, typename KeyEqual = std::equal_to<KeyT> >
class UnorderedSet {
  Hash hasher_;
  KeyEqual equal_;
  std::vector<std::list<KeyT> > buckets_;
  size_t size_ = 0;
  size_t bucket_count_ = 0;

 public:
  UnorderedSet() = default;
  explicit UnorderedSet(size_t count) : buckets_(count), bucket_count_(count) {
  }
  template <typename Iter>
  UnorderedSet(const Iter& begin, const Iter& end) {
    Reserve(std::count(begin, end, *begin));
    for (Iter i = begin; i != end; ++i) {
      Insert(*i);
    }
  }
  UnorderedSet(const UnorderedSet& other) = default;
  UnorderedSet(UnorderedSet&& other) {
    buckets_ = std::move(other.buckets_);
    size_ = other.size_;
    bucket_count_ = other.bucket_count_;
    other.size_ = 0;
    other.bucket_count_ = 0;
  }
  UnorderedSet& operator=(const UnorderedSet& other) = default;
  UnorderedSet& operator=(UnorderedSet&& other) {
    buckets_ = std::move(other.buckets_);
    size_ = other.size_;
    bucket_count_ = other.bucket_count_;
    other.size_ = 0;
    other.bucket_count_ = 0;
    return *this;
  }
  ~UnorderedSet() = default;
  size_t Size() const {
    return size_;
  }
  bool Empty() const {
    return size_ == 0;
  }
  void Clear() {
    buckets_.clear();
    size_ = 0;
    bucket_count_ = 0;
  }
  void Insert(const KeyT& key) {
    size_t h = hasher_(key);
    if (size_ + 1 > bucket_count_) {
      Reserve(bucket_count_ == 0 ? 1 : bucket_count_ * 2);
    }
    buckets_[h % bucket_count_].emplace_back(key);
    ++size_;
  }
  void Insert(KeyT&& key) {
    size_t h = hasher_(key);
    if (size_ + 1 > bucket_count_) {
      Reserve(bucket_count_ == 0 ? 1 : bucket_count_ * 2);
    }
    buckets_[h % bucket_count_].emplace_back(std::move(key));
    ++size_;
  }
  void Erase(const KeyT& key) {
    if (bucket_count_ == 0) {
      return;
    }
    size_t h = hasher_(key);
    std::list<KeyT>& list = buckets_[h % bucket_count_];
    for (auto it = list.begin(); it != list.end(); ++it) {
      if (equal_(*it, key)) {
        list.erase(it);
        --size_;
        break;
      }
    }
  }
  bool Find(const KeyT& key) const {
    if (bucket_count_ == 0) {
      return false;
    }
    size_t h = hasher_(key);
    const std::list<KeyT>& list = buckets_[h % bucket_count_];
    for (auto it = list.begin(); it != list.end(); ++it) {
      if (equal_(*it, key)) {
        return true;
      }
    }
    return false;
  }
  void Rehash(size_t new_bucket_count) {
    if (new_bucket_count == bucket_count_ || new_bucket_count < size_) {
      return;
    }
    std::vector<std::list<KeyT> > tmp(new_bucket_count);
    for (size_t i = 0; i < bucket_count_; ++i) {
      std::list<KeyT>& src = buckets_[i];
      while (src.begin() != src.end()) {
        size_t h = hasher_(*src.begin());
        std::list<KeyT>& dst = tmp[h % new_bucket_count];
        dst.splice(dst.begin(), src, src.begin());
      }
    }
    std::swap(buckets_, tmp);
    bucket_count_ = new_bucket_count;
  }
  void Reserve(size_t new_bucket_count) {
    if (new_bucket_count <= bucket_count_) {
      return;
    }
    Rehash(new_bucket_count);
  }
  size_t BucketCount() const {
    return bucket_count_;
  }
  size_t BucketSize(size_t id) const {
    if (id >= bucket_count_) {
      return 0;
    }
    return buckets_[id].size();
  }
  size_t Bucket(const KeyT& key) const {
    return hasher_(key) % bucket_count_;
  }
  double LoadFactor() const {
    if (bucket_count_ == 0) {
      return 0;
    }
    return double(size_) / bucket_count_;
  }
};
#endif  // UNORDERED_SET
