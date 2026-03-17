#ifndef ARITHMETIC_CODER_H
#define ARITHMETIC_CODER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Symbol.h"

typedef long long int LL;

class ArithmeticCoder
{
public:
    ArithmeticCoder();
    ~ArithmeticCoder();

    // Main methods
    void encodeString(const std::string &input);
    std::string decodeValue();

    // Getters
    const std::vector<Symbol> &getFrequencyTable() const;
    const std::vector<Symbol> &getSubIntervals() const;
    double getEncodedValue() const;
    const std::string &getDecodedString() const;
    const std::string &getOriginalString() const;

private:
    std::vector<Symbol> frequency_table;
    std::unordered_map<char, std::pair<double, double>> init_intervals;
    std::vector<Symbol> sub_intervals;
    double encoded_str;
    std::string decoded_str;
    std::string original_string;

    // Helper methods (preserved from original code)
    void build_frequency_table(const std::string &s);
    void initial_intervals_per_symbol(const std::string &s);
    void get_init_intervals();
    void encode(const std::string &s, LL depth, double curr_low_lim, double curr_hi_lim);
    double select_value_from_interval();
    bool compare_intervals(const Symbol &a, const Symbol &b);
    char compare(double &value);
    void decode(double curr_low_lim, double curr_hi_lim, const std::string &s, LL depth);
    void reset();
};

#endif // ARITHMETIC_CODER_H
