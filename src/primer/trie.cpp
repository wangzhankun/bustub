#include "primer/trie.h"
#include <string_view>
#include "common/exception.h"

namespace bustub {

// template <typename T>
// inline std::shared_ptr<const TrieNode> cloneTrieNode(std::shared_ptr<const TrieNode> node, std::shared_ptr<T> value)
// {
//   return std::make_shared<const TrieNodeWithValue<T>>(node->Clone()->children_, value);
// }

// template <typename T>
// inline std::shared_ptr<const TrieNode> cloneTrieNode(std::shared_ptr<const TrieNode> node) {
//   if (node->is_value_node_) {
//     try{
//     auto p = node.get();
//     auto n = dynamic_cast<const TrieNodeWithValue<T> *>(p);
//     auto nn = n->Clone();
//     return nn;}
//     catch(std::bad_cast &e){
//       std::cout << e.what() << std::endl;
//       return nullptr;
//     }
//   } else {
//     return node->Clone();
//   }
// }

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  // throw NotImplementedException("Trie::Get is not implemented.");

  // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
  // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
  // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
  // Otherwise, return the value.
  auto node = root_;
  if (node == nullptr) {
    return nullptr;
  }
  for (auto c : key) {
    auto children = node->children_;
    auto it = children.find(c);
    if (it == children.end()) {
      return nullptr;
    }
    node = it->second;
  }
  if (node->is_value_node_ == false) {
    return nullptr;
  }
  auto value_node = dynamic_cast<const TrieNodeWithValue<T> *>(node.get());
  if (value_node)
    return value_node->value_.get();
  else
    return nullptr;
}
template <typename T>
std::shared_ptr<const TrieNode> _Put(std::shared_ptr<const TrieNode> curr_node, const char *it, const char *end,
                                     std::shared_ptr<T> value) {
  if (it == end) {
    if (curr_node == nullptr) {
      return std::make_shared<const TrieNodeWithValue<T>>(value);
    } else {
      return std::make_shared<const TrieNodeWithValue<T>>(curr_node->children_, value);
    }
  }
  auto old_child_node = [&curr_node, &it]() -> std::shared_ptr<const TrieNode> {
    if (curr_node == nullptr) return nullptr;
    auto node_it = curr_node->children_.find(*it);
    if (node_it == curr_node->children_.end()) return nullptr;
    return node_it->second;
  }();
  auto child_node = _Put(old_child_node, it + 1, end, value);
  if (curr_node == nullptr) {
    return std::make_shared<const TrieNode>(std::map<char, std::shared_ptr<const TrieNode>>{{*it, child_node}});
  } else {
    auto cloned_curr_node = curr_node->Clone();
    cloned_curr_node->children_[*it] = child_node;
    return std::move(cloned_curr_node);
  }
}
template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  // throw NotImplementedException("Trie::Put is not implemented.");

  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should create a new `TrieNodeWithValue`.
  auto new_root = _Put(this->root_, key.begin(), key.end(), std::make_shared<T>(std::move(value)));
  return Trie(new_root);
}

std::shared_ptr<const TrieNode> _Remove(std::shared_ptr<const TrieNode> curr_node, const char *it, const char *end) {
  if (it == end) {
    if (curr_node == nullptr) {
      return nullptr;
    } else {
      if (curr_node->children_.empty()) return nullptr;

      if (curr_node->is_value_node_) {
        return std::make_shared<const TrieNode>(curr_node->children_);
      } else {
        return curr_node;
      }
    }
  }
  if (curr_node == nullptr) {
    return nullptr;
  }

  auto old_child_node = [&curr_node, &it]() -> std::shared_ptr<const TrieNode> {
    auto node_it = curr_node->children_.find(*it);
    if (node_it == curr_node->children_.end()) return nullptr;
    return node_it->second;
  }();
  auto child_node = _Remove(old_child_node, it + 1, end);
  if (old_child_node == child_node) return curr_node;

  {
    auto cloned_curr_node = curr_node->Clone();
    if (child_node == nullptr) {
      cloned_curr_node->children_.erase(*it);
    } else {
      cloned_curr_node->children_[*it] = child_node;
    }
    if (cloned_curr_node->children_.empty() && cloned_curr_node->is_value_node_ == false) {
      return nullptr;
    } else {
      return std::move(cloned_curr_node);
    }
  }
}

auto Trie::Remove(std::string_view key) const -> Trie {
  // throw NotImplementedException("Trie::Remove is not implemented.");

  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
  auto new_root = _Remove(this->root_, key.begin(), key.end());
  return Trie(new_root);
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
