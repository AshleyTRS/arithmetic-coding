#include "ArithmeticCoder.h"
#include <random>
#include <algorithm>
#include <set>

ArithmeticCoder::ArithmeticCoder() : encoded_str(0.0), decoded_str(""), original_string("")
{
}

ArithmeticCoder::~ArithmeticCoder()
{
}

void ArithmeticCoder::reset()
{
    frequency_table.clear();
    init_intervals.clear();
    sub_intervals.clear();
    encoded_str = 0.0;
    decoded_str = "";
    original_string = "";
}

void ArithmeticCoder::build_frequency_table(const std::string &s)
{
    std::set<char> seen_symbols;
    std::unordered_map<char, int> indexes;
    int current_idx = 0;
    for (auto c : s)
    {
        if (seen_symbols.find(c) != seen_symbols.end())
        { // if a symbol has already been found
            int idx = indexes[c];
            frequency_table[idx].frequency++;
        }
        else // new symbol is discovered
        {
            seen_symbols.insert(c);               // add to set
            indexes[c] = current_idx;             // map symbol to index
            Symbol new_symbol(c, current_idx, 1); // create new Symbol object
            frequency_table.push_back(new_symbol);
            current_idx++;
        }
    }
}

void ArithmeticCoder::initial_intervals_per_symbol(const std::string &s)
{
    LL len_string = s.size();
    double accum_low_lim = 0.0;
    double accum_hi_lim = 0.0;
    for (auto &c : frequency_table)
    {
        c.low_lim = accum_low_lim;
        accum_hi_lim += ((double)c.frequency / (double)len_string);
        c.hi_lim = accum_hi_lim;
        accum_low_lim += ((double)c.frequency / (double)len_string);
    }
}

void ArithmeticCoder::get_init_intervals()
{
    for (auto &s : frequency_table)
    {
        init_intervals[s.symbol] = {s.low_lim, s.hi_lim};
    }
}

void ArithmeticCoder::encode(const std::string &s, LL depth, double curr_low_lim, double curr_hi_lim)
{
    if (depth >= (LL)s.size())
    {
        return;
    }

    char curr_symbol = s[depth];
    double low_lim_sym = init_intervals[curr_symbol].first;
    double hi_lim_sym = init_intervals[curr_symbol].second;

    // calculate subinterval
    double new_low_lim = curr_low_lim + (low_lim_sym * (curr_hi_lim - curr_low_lim));
    double new_hi_lim = curr_low_lim + (hi_lim_sym * (curr_hi_lim - curr_low_lim));

    // keep track of symbol and subinterval
    Symbol sym(curr_symbol, new_low_lim, new_hi_lim);
    sub_intervals.push_back(sym);

    // calculate new subinterval
    encode(s, depth + 1, new_low_lim, new_hi_lim);
}

double ArithmeticCoder::select_value_from_interval()
{
    double low_lim_inclusive = sub_intervals[sub_intervals.size() - 1].low_lim;
    double hi_lim_exclusive = sub_intervals[sub_intervals.size() - 1].hi_lim;

    // select random value in interval [low_lim_inclusive, hi_lim_exclusive)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(low_lim_inclusive, hi_lim_exclusive);
    return dis(gen);
}

bool ArithmeticCoder::compare_intervals(const Symbol &a, const Symbol &b)
{
    if (a.low_lim == b.low_lim)
        return a.hi_lim < b.hi_lim;
    return a.low_lim < b.low_lim;
}

char ArithmeticCoder::compare(double &value)
{
    int ini = 0;
    int fin = frequency_table.size() - 1;

    while (ini <= fin)
    {
        int mid = (ini + fin) / 2;

        if (value >= frequency_table[mid].low_lim && value < frequency_table[mid].hi_lim)
        { // value does fall within current range [a, b)
            return frequency_table[mid].symbol;
        }
        else if (value >= frequency_table[mid].hi_lim)
        {
            ini = mid + 1;
        }
        else
        {
            fin = mid - 1;
        }
    }

    return '\0';
}

void ArithmeticCoder::decode(double curr_low_lim, double curr_hi_lim, const std::string &s, LL depth)
{
    if (depth >= (LL)s.size())
        return;

    double probability = (encoded_str - curr_low_lim) / (curr_hi_lim - curr_low_lim);
    char symbol = compare(probability);

    decoded_str += symbol;

    double sym_low = init_intervals[symbol].first;
    double sym_high = init_intervals[symbol].second;

    // recompute subinterval
    double new_low_lim = curr_low_lim + sym_low * (curr_hi_lim - curr_low_lim);
    double new_hi_lim = curr_low_lim + sym_high * (curr_hi_lim - curr_low_lim);

    decode(new_low_lim, new_hi_lim, s, depth + 1);
}

void ArithmeticCoder::encodeString(const std::string &input)
{
    reset();
    original_string = input;

    build_frequency_table(input);
    initial_intervals_per_symbol(input);
    get_init_intervals();

    // encode string
    encode(input, 0, 0, 1);

    // selected value to encode string
    encoded_str = select_value_from_interval();
}

std::string ArithmeticCoder::decodeValue()
{
    decoded_str = "";
    decode(0, 1, original_string, 0);
    return decoded_str;
}

const std::vector<Symbol> &ArithmeticCoder::getFrequencyTable() const
{
    return frequency_table;
}

const std::vector<Symbol> &ArithmeticCoder::getSubIntervals() const
{
    return sub_intervals;
}

double ArithmeticCoder::getEncodedValue() const
{
    return encoded_str;
}

const std::string &ArithmeticCoder::getDecodedString() const
{
    return decoded_str;
}

const std::string &ArithmeticCoder::getOriginalString() const
{
    return original_string;
}
