#ifndef UTILS_H
#define UTILS_H

template <typename Container>
inline int isize(const Container& c) {
    return static_cast<int>(c.size());
}

#endif