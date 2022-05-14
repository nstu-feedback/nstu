#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <optional>
#include <utility>


template <typename Key = int, typename Compare = std::less<Key>>
class t234
{
private:
    struct Node;

public:
    struct Iterator;

public: // typedefs
    using value_type             = Key;
    using node_type              = Node;
    using size_type              = size_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using iterator               = Iterator;
    using const_iterator         = const iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    struct Iterator
    {
    public: // typedefs
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;

    private: // fields
        node_type* node_ = nullptr;
        size_t key_num_  = 0;

    public: // ctors
        Iterator(node_type* node, size_t key_num)
            : node_(node), key_num_(key_num) {}

        Iterator(const_iterator& other) = default;

    public: // operators
        iterator&
        operator++()
        {
            if (node_->is_leaf()) {
                if (key_num_ + 1 < node_->size()) {
                    ++key_num_;
                    return *this;
                }
            }

            *this = get_next(node_, key_num_);
            return *this;
        }

        iterator
        operator++(int)
        {
            auto old = *this;
            ++(*this);
            return old;
        }

        iterator&
        operator--()
        {
            // todo…
        }
        
        iterator
        operator--(int)
        {
            auto old = *this;
            --(*this);
            return old;
        }

        reference
        operator*()
        { return *node_->keys_[key_num_]; }

        const_reference
        operator*() const
        { return *node_->keys_[key_num_]; }

        pointer
        operator->()
        { return &*node_->keys_[key_num_]; }

        const_pointer
        operator->() const
        { return &*node_->keys_[key_num_]; }

        bool
        operator==(const_iterator& other)
        { return node_ == other.node_; }

    private:
        static iterator
        get_next(node_type* node, size_t key_num)
        {
            auto child = node->nodes_[key_num + 1];
            if (child != nullptr) {
                return iterator(child->get_leftest(), 0);
            }

            size_t index;
            do {
                if (node->parent_ == nullptr)
                    return iterator(nullptr, 0);

                index = index_of(node, node->parent_);
                node  = node->parent_;
            } while (index == node->size());

            return iterator(node, index);
        }

        static size_t
        index_of(const node_type* target, const node_type* parent)
        {
            size_t n = 0;

            for (; n < parent->nodes_.size(); ++n) {
                if (parent->nodes_[n] == target)
                    break;
            }

            return n;
        }
    };

private:
    struct Node
    {
        friend t234;

    private:
        using key_opt = std::optional<value_type>;
        using Keys    = std::array<key_opt, 3>;
        using Nodes   = std::array<node_type*, 4>;

        Keys keys_{};
        Nodes nodes_{};

        node_type* parent_ = nullptr;

    public:
        Node(Keys&& keys) : keys_(std::move(keys)) {}

        Node(value_type&& key)
        { keys_[0] = std::move(key); }

        Node(key_opt&& key)
        { keys_[0] = std::move(key); }

        bool
        is_leaf() const
        {
            size_t n = 0;
            for (; n < nodes_.size() && !nodes_[n]; ++n);
            return n == nodes_.size();
        }

        /**
         * @return number of sotred keys
         */
        size_t
        size() const
        {
            size_t n = 0;
            for (; n < keys_.size() && keys_[n]; ++n);
            return n;
        }

        /**
         * @brief check it's a 4-node
         */
        bool
        full() const
        {
            size_t n = 0;
            for (; n < keys_.size() && keys_[n]; ++n);
            return n == keys_.size();
        }

        /**
         * @brief checks on having a key
         *
         * @return index of the key or std::nullopt_t
         */
        std::optional<size_t>
        has(const_reference key) const
        {
            size_t n = 0;
            for (; (n < keys_.size()) && (keys_[n] != key); ++n);
            
            if (n != keys_.size())
                return n;
            return {};
        }

        /**
         * @details calling this method you guarantee the node is not
         * full and the node is not a leaf
         *
         * @return inserted key index
         */
        size_t
        raw_insert(value_type&& key)
        {
            size_t pos = 0;
            for (; (pos < keys_.size()) && keys_[pos]; ++pos);

            keys_[pos] = std::move(key);
            return sort_shift(pos);
        }

        /**
         * @brief splits one 4-node into two 2-nodes
         *
         * @details Every of 3 keys is being placed into its own node.
         * If original node doesn't have a parent, it will be created.
         * k0 stays in original node, optionally getting a new parent.
         * k1 becomes a new separating key for its neighbors.
         * k2 is being placed into a new node that will become
         * an original node's right sibling.
         *
         * @return pointer to node having become a new parent for splitted nodes
         */
        node_type*
        split()
        {
            size_t index = 0;

            if (parent_ == nullptr)
                parent_ = new node_type(std::exchange(keys_[1], {}));
            else
                index = parent_->raw_insert(*std::exchange(keys_[1], {}));

            auto right     = new node_type(std::exchange(keys_[2], {}));
            right->parent_ = parent_;

            right->nodes_[0] = std::exchange(nodes_[2], nullptr);
            if (right->nodes_[0] != nullptr)
                right->nodes_[0]->parent_ = right;

            right->nodes_[1] = std::exchange(nodes_[3], nullptr);
            if (right->nodes_[1] != nullptr)
                right->nodes_[1]->parent_ = right;

            parent_->nodes_[index]     = this; // left
            parent_->nodes_[index + 1] = right;

            return parent_;
        }

        /**
         * @brief shifts a key (with subnodes) to its order place
         *
         * @return index of the key's new position
         */
        size_t
        sort_shift(size_t pos)
        {
            size_t new_pos = pos;

            for (; pos > 0; --pos) {
                if (keys_[pos] < keys_[pos - 1]) {
                    keys_[pos - 1].swap(keys_[pos]);
                    std::swap(nodes_[pos], nodes_[pos + 1]);
                    --new_pos;
                }
            }

            return new_pos;
        }

        /**
         * @brief find node to insert the key
         *
         * @details there is no sense to consider the case when node is empty,
         * because the only case where it possible is node is root
         * (also calling this method you guarantee the key is not dublicate)
         *
         * @return pointer to insertion node
         */
        node_type*
        find_yourself(reference key)
        {
            if (key < keys_[0])
                return nodes_[0];

            if (keys_[1]) {
                if (key < keys_[1])
                    return nodes_[1];

                if (keys_[2]) {
                    if (key < keys_[2])
                        return nodes_[2];
                    return nodes_[3];
                }

                return nodes_[2];
            }

            return nodes_[1];
        }

        node_type*
        get_leftest()
        {
            return (
                nodes_.front() == nullptr
                ? this
                : nodes_.front()->get_leftest()
            );
        }

        node_type*
        get_rightest()
        {
            return (
                nodes_[size()] == nullptr
                ? this
                : nodes_[size()]->get_rightest()
            );
        }
    };

private:
    size_type size_  = 0;
    node_type* root_ = nullptr;

public:
    t234() = default;

    t234(const t234& other)
    {
        clear();
        for (const_reference item : other)
            insert(item);
    }

    ~t234()
    { clear(); }

    iterator
    insert(value_type&& key)
    {
        if (root_ == nullptr) {
            ++size_;
            root_ = new node_type(std::move(key));
            return iterator(root_, 0);
        }

        return insert(root_, std::move(key));
    }

    iterator
    insert(node_type* node, value_type&& key)
    {
        if (auto index = node->has(key))
            return iterator(node, *index);

        if (node->full()) {
            node_type* parent = node->split();

            if (node == root_)
                root_ = parent;

            return insert(parent, std::move(key));
        }

        if (node->is_leaf()) {
            ++size_;
            size_t index = node->raw_insert(std::move(key));
            return iterator(node, index);
        }

        node_type* insert_node = node->find_yourself(key);
        return insert(insert_node, std::move(key));
    }

    iterator
    begin()
    { return iterator(root_->get_leftest(), 0); }

    iterator
    end()
    { return iterator(nullptr, 0); }

    iterator
    find(const_reference key)
    {
        for (auto iter = begin(); iter != end(); ++iter) {
            if (key == *iter)
                return iter;
        }
        return end();
    }

    void
    erase(const_iterator pos)
    {
        // todo…
    }

    void
    clear()
    {
        // todo…
    }

    size_t
    size() const
    { return size_; }
};

