#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <set>
#include <random>
#include <algorithm>

typedef long long int LL;

using namespace std;

struct Symbol
{
    LL frequency;
    double low_lim;
    double hi_lim;
    char symbol;
    int idx;

    Symbol(char s, int i, LL f)
    {
        frequency = f;
        low_lim = 0;
        hi_lim = 0;
        idx = i;
        symbol = s;
    }

    Symbol(char s, double low, double hi)
    {
        symbol = s;
        low_lim = low;
        hi_lim = hi;
        idx = 0;
        frequency = 0;
    }
};

void build_frequency_table(vector<Symbol> &frequency_table, string s)
{
    set<char> seen_symbols;
    unordered_map<char, int> indexes;
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

void initial_intervals_per_symbol(vector<Symbol> &frequency_table, string s)
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

void get_init_intervals(unordered_map<char, pair<double, double>> &init_intervals, vector<Symbol> &frequency_table)
{
    for (auto &s : frequency_table)
    {
        init_intervals[s.symbol] = {s.low_lim, s.hi_lim};
    }
}

void encode(unordered_map<char, pair<double, double>> init_intervals, vector<Symbol> &sub_intervals, double curr_low_lim, double curr_hi_lim, string s, LL depth)
{
    if (depth >= s.size())
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
    encode(init_intervals, sub_intervals, new_low_lim, new_hi_lim, s, depth + 1);
}

double select_value_from_interval(vector<Symbol> sub_intervals)
{
    double low_lim_inclusive = sub_intervals[sub_intervals.size() - 1].low_lim;
    double hi_lim_exclusive = sub_intervals[sub_intervals.size() - 1].hi_lim;

    // select random value in interval [low_lim_inclusive, hi_lim_exclusive)
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<> dis(low_lim_inclusive, hi_lim_exclusive);
    return dis(gen);
}

bool compare_intervals(const Symbol &a, const Symbol &b)
{
    if (a.low_lim == b.low_lim)
        return a.hi_lim < b.hi_lim;
    return a.low_lim < b.low_lim;
}

char compare(double &value, vector<Symbol> &frequency_table)
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

void decode(vector<Symbol> &frequency_table, vector<Symbol> &sub_intervals, double encoded_str, double curr_low_lim, double curr_hi_lim, string &decoded_str, string s, LL depth)
{
    if (depth >= s.size())
        return;

    double probability = (encoded_str - curr_low_lim) / (curr_hi_lim - curr_low_lim);
    char symbol = compare(probability, frequency_table);

    decoded_str += symbol;

    double new_low_lim = sub_intervals[depth].low_lim;
    double new_hi_lim = sub_intervals[depth].hi_lim;

    decode(frequency_table, sub_intervals, encoded_str, new_low_lim, new_hi_lim, decoded_str, s, depth + 1);
}

int main()
{
    string s;
    getline(cin, s);

    vector<Symbol> frequency_table;
    unordered_map<char, pair<double, double>> init_intervals;
    vector<Symbol> sub_intervals;

    build_frequency_table(frequency_table, s);

    initial_intervals_per_symbol(frequency_table, s);

    cout << "\nFREQUENCY TABLE\nSi\tFi\tIi\n";
    for (auto c : frequency_table)
    {
        cout << c.symbol << "\t" << c.frequency << "\t [" << c.low_lim << ", " << c.hi_lim << ")\n";
    }

    // create auxilirary map to store symbols and their initial intervals
    get_init_intervals(init_intervals, frequency_table);

    // encode string
    encode(init_intervals, sub_intervals, 0, 1, s, 0);

    // subintervals
    cout << "\n Subintervals\n";
    for (auto s : sub_intervals)
    {
        cout << s.symbol << ": [" << s.low_lim << ", " << s.hi_lim << ")\n";
    }

    // selected value to encode string
    double encoded_str = select_value_from_interval(sub_intervals);
    cout << s << " = " << encoded_str;

    // decode value

    string decoded_str = "";

    decode(frequency_table, sub_intervals, encoded_str, 0, 1, decoded_str, s, 0);

    cout << "\nDecoded value: " << decoded_str << "\n";

    return 0;
}