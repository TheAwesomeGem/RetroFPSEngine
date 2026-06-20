//
// Created by TheAwesomeGem on 12/14/2025.
//

#pragma once
#include <chrono>
#include <functional>
#include <random>
#include <uuid.h>

namespace RandomExt
{
    // Returns a seeded Mersenne Twister
    // Note: we'd prefer to return a std::seed_seq (to initialize a std::mt19937), but std::seed can't be copied, so it can't be returned by value.
    // Instead, we'll create a std::mt19937, seed it, and then return the std::mt19937 (which can be copied).
    inline std::mt19937 generate()
    {
        std::random_device rd{};

        // Create seed_seq with clock and 7 random numbers from std::random_device
        std::seed_seq ss
        {
            static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
            rd(), rd(), rd(), rd(), rd(), rd(), rd()
        };

        return std::mt19937{ ss };
    }

    // Here's our global std::mt19937 object.
    // The inline keyword means we only have one global instance for our whole program.
    inline std::mt19937 rng
    {
        generate() // generates a seeded std::mt19937 and copies it into our global object
    };

    // Generate a random int between [min, max] (inclusive)
    inline int range(int min, int max)
    {
        return std::uniform_int_distribution{ min, max }(rng);
    }

    inline float range(float min, float max)
    {
        return std::uniform_real_distribution{ min, max }(rng);
    }

    inline double range(double min, double max)
    {
        return std::uniform_real_distribution{ min, max }(rng);
    }

    // The following function templates can be used to generate random numbers
    // when min and/or max are not type int
    // See https://www.learncpp.com/cpp-tutorial/function-template-instantiation/
    // You can ignore these if you don't understand them

    // Generate a random value between [min, max] (inclusive)
    // * min and max have same type
    // * Return value has same type as min and max
    // * Supported types:
    // *    short, int, long, long long
    // *    unsigned short, unsigned int, unsigned long, or unsigned long long
    // Sample call: Random::get(1L, 6L);             // returns long
    // Sample call: Random::get(1u, 6u);             // returns unsigned int
    template <typename T>
    T range(T min, T max)
    {
        return std::uniform_int_distribution<T>{ min, max }(rng);
    }

    // Generate a random value between [min, max] (inclusive)
    // * min and max can have different types
    // * Must explicitly specify return type as template type argument
    // * min and max will be converted to the return type
    // Sample call: Random::get<std::size_t>(0, 6);  // returns std::size_t
    // Sample call: Random::get<std::size_t>(0, 6u); // returns std::size_t
    // Sample call: Random::get<std::int>(0, 6u);    // returns int
    template <typename R, typename S, typename T>
    R range(S min, T max)
    {
        return range<R>(static_cast<R>(min), static_cast<R>(max));
    }

    inline float index(float max)
    {
        return range(0.0F, max);
    }

    inline float perc()
    {
        return range(0.0F, 1.0F);
    }

    inline bool is_chance(float chance)
    {
        return chance >= perc();
    }

    inline size_t element(size_t max)
    {
        return range<size_t>(0L, max - 1L);
    }

    inline int index(int max)
    {
        return range(0, max);
    }

    inline int choice()
    {
        return index(1);
    }

    inline size_t get_random_index_by_weight(
        const std::vector<size_t>& indices,
        float random_variance_perc,
        const std::function<int(size_t)>& get_score
    )
    {
        std::vector<long> scores;

        long max_score = 0;

        for (size_t index : indices)
        {
            scores.insert(scores.begin() + (long)index, 0);
            scores[index] = get_score(index);

            if (scores[index] > max_score)
            {
                max_score = scores[index];
            }
        }

        for (long& score : scores)
        {
            score += (long)(random_variance_perc * range((float)-max_score * 2.0F, (float)+max_score * 2.0F));
            score = std::max<long>(score, 1);
        }

        if (scores.empty())
        {
            return indices[element(indices.size())];
        }

        std::discrete_distribution<> score_dist(scores.begin(), scores.end());

        return score_dist(rng);
    }

    inline uuids::uuid id()
    {
        uuids::uuid_random_generator gen{ rng };

        return gen();
    }
}
