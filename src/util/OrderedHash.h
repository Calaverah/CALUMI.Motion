#pragma once
#include <QHash>
#include <QDebug>

namespace CALUMI::Motion
{
    template <typename Key, typename Value>
    class OrderedHash
    {
    public:
        template <bool IsConst>
        class Iterator
        {
        public:
            using MapType               = std::conditional_t<IsConst, const QHash<Key, Value>, QHash<Key, Value>>;
            using ListIter              = QList<Key>::const_iterator;
            using ValueRef              = std::conditional_t<IsConst, const Value&, Value&>;

            using iterator_category     = std::forward_iterator_tag;
            using difference_type       = qsizetype;
            using value_type            = std::pair<const Key&, ValueRef>;
            using pointer               = void;
            using reference             = value_type;

            // ReSharper disable once CppParameterMayBeConst
            constexpr explicit Iterator(ListIter list, MapType* map) noexcept : m_listIter(list), m_map(map) {}

            //implicit const conversion is desired
            // ReSharper disable once CppNonExplicitConvertingConstructor
            template <bool WasConst, typename = std::enable_if_t<IsConst && !WasConst>>
            constexpr Iterator(const Iterator<WasConst>& other) : m_listIter(other.m_it), m_map(other.m_map) {}

            constexpr Iterator() = default;

            reference operator*() const
            {
                const Key& k = *m_listIter;
                return {k, (*m_map)[k] };
            }

            const Key& key() const { return *m_listIter; }
            ValueRef value() const { return (*m_map)[*m_listIter]; }


            Iterator& operator++() {
                ++m_listIter;
                return *this;
            }

            Iterator operator++(int) {
                Iterator tmp = *this;
                ++*this;
                return tmp;
            }


            friend bool operator==(const Iterator& a, const Iterator& b) {
                return a.m_listIter == b.m_listIter;
            }

            friend bool operator!=(const Iterator& a, const Iterator& b) {
                return a.m_listIter != b.m_listIter;
            }

        private:
            friend class Iterator<true>;
            ListIter m_listIter = {};
            MapType* m_map = Q_NULLPTR;
        };

        using iterator = Iterator<false>;
        using const_iterator = Iterator<true>;

        iterator begin() { return iterator(m_order.cbegin(), &m_hash); }
        iterator end() { return iterator(m_order.cend(), &m_hash); }

        const_iterator begin() const { return const_iterator(m_order.cbegin(), &m_hash); }
        const_iterator end() const { return const_iterator(m_order.cend(), &m_hash); }
        const_iterator cbegin() const { return const_iterator(m_order.cbegin(), &m_hash); }
        const_iterator cend() const { return const_iterator(m_order.cend(), &m_hash); }

        [[nodiscard]] bool contains(const Key& key) const { return m_hash.contains(key); }

        iterator insert(const Key& key, const Value& value)
        {
            quint32 index{m_order.size()};

            if (!contains(key))
                m_order.append(key);
            else
            {
                index = m_order.indexOf(key);

                //something went wrong
                if (index < 0 || index >= m_order.size())
                {
                    qCritical() << Q_FUNC_INFO << " key: " << key
                        << " of type" << typeid(Key).name()
                        << " found in hash but not in order list...";
                    return cend();
                }
            }

            m_hash.insert(key, value);
            return Iterator(m_order.cbegin() + index, &m_hash);
        }

        Value value(const Key& key) const
        {
            return m_hash.value(key);
        }

        bool remove(const Key& key)
        {
            if (!m_hash.contains(key))
                return false;

            if (!m_hash.remove(key))
                return false;

            if (m_order.removeAll(key) == 0)
            {
                qCritical() << Q_FUNC_INFO << " attempt to remove key: "
                    << key << " of type: " << typeid(Key).name()
                    << " from order list resulted in no removals...";
            }
            return true;
        }

        QList<Key> keys() const { return m_order; }
        QList<Key> unorderedKeys() const { return m_hash.keys(); }


    private:
        QHash<Key, Value> m_hash;
        QList<Key> m_order;
    };


}
