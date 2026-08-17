//
// Created by Lee on 2022/9/3.
//

#ifndef MYCPPIMPLEMENT_MYRANDOM_H
#define MYCPPIMPLEMENT_MYRANDOM_H

#include <random>

class RandomNumberGenerator {
public:
    RandomNumberGenerator(int low, int high)
        : m_nd(low, high),
          m_ur(low, high),
          m_ui(low, high),
          mt_gen(r()) {}
    int GetRandomNumberByDefault() {
        return m_ur(e);
    }
    int GetRandomNumberByMt19937() {
        return m_ui(mt_gen);
    }
private:
    std::normal_distribution<> m_nd;
    std::uniform_real_distribution<double> m_ur;
    std::uniform_int_distribution<> m_ui;

    std::default_random_engine e; // 随机数引擎 范围内的每个数生成的概率都是相同的
    std::random_device r;
    std::mt19937 mt_gen;
};
#endif //MYCPPIMPLEMENT_MYRANDOM_H
