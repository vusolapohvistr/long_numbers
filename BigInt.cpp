//
// Created by news on 13.11.2019.
//

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "BigInt.h"


BigInt::BigInt(std::string &number) {
    is_negative = number[0] == '-';
    value = std::vector<int> (0);

    for (int i = number.size(); i > (is_negative ? 1 : 0); i-=basis_length) {
        if (i - basis_length >= 0) {
            value.push_back(std::abs(std::stoi(number.substr(i - basis_length, basis_length))));
        } else {
            value.push_back(std::abs(std::stoi(number.substr(0, i))));
        }
    }
    if (mod.value[mod.value.size() - 1] != 0) {
        *this = *this % mod;
    }
}

BigInt::BigInt(std::vector<int> &number, bool is_negativ) {
    value = number;
    is_negative = is_negativ;
    if (mod.value[mod.value.size() - 1] != 0) {
        *this = *this % mod;
    }
}

BigInt::BigInt(int arg) {
    if (arg == 0) {
        value = std::vector<int> (1,0);
    }
    if (arg < 0) {
        is_negative = true;
        arg *= -1;
    } else {
        is_negative = false;
    }
    while (arg > 0) {
        value.push_back(arg % basis);
        arg /= basis;
    }
}

std::string BigInt::to_string() const {
    std::string answer = this->is_negative ? "-" : "";

    answer.append(std::to_string(value[this->value.size() - 1]));

    for (int i = this->value.size() - 2; i >= 0 ; i--) {
        if (value[i] == 0 && i != 0) {
            for (int i = 0; i < basis_length; i++) {
                answer.append("0");
            }
        } else {
            for (int g = 10; basis > value[i] * g && basis > g; g *= 10) answer.append("0");
            answer.append(std::to_string(value[i]));
        }
    }

    return answer;
}

BigInt BigInt::operator+(BigInt const &arg) const {
    if (this->is_negative && !arg.is_negative) {
        return arg - *this;
    }
    if (!this->is_negative && arg.is_negative) {
        return *this - arg;
    }

    BigInt copy_this = *this;
    copy_this.is_negative = false;
    BigInt copy_arg = arg;
    copy_arg.is_negative = false;

    BigInt longer = copy_this > copy_arg ? *this : arg;
    BigInt smaller = copy_this < copy_arg ? *this : arg;

    std::vector<int> answer = std::vector<int> (longer.value.size());
    int a_val = 0;

    for (int i = 0; i < smaller.value.size(); i++) {
        int result = smaller.value[i] + longer.value[i] + a_val;
        answer[i] = result % basis;
        a_val = result / basis;
    }

    int last_pos = smaller.value.size();

    while (a_val > 0 || last_pos < longer.value.size()) {
        int result;
        if (longer.value.size() <= last_pos) {
            result = a_val;
        } else {
            result = longer.value[last_pos] + a_val;
        }
        if (answer.size() <= last_pos) {
            answer.push_back(result % basis);
        } else {
            answer[last_pos] = result % basis;
        }
        a_val = result / basis;
        last_pos++;
    }

    if (a_val > 0) answer.push_back(a_val);

    BigInt a = BigInt(answer, this->is_negative);

    if (mod.value[mod.value.size() - 1] != 0 && a > mod) a = a - mod;

    return a;
}

BigInt BigInt::operator-(BigInt const &arg) const {
    if (this->is_negative && arg.is_negative) {
        BigInt a = *this;
        BigInt b = arg;
        a.is_negative = false;
        b.is_negative = false;
        BigInt p = a + b;
        p.is_negative = true;
        return p;
    }

    BigInt copy_this = *this;
    copy_this.is_negative = false;
    BigInt copy_arg = arg;
    copy_arg.is_negative = false;

    BigInt longer = copy_this > copy_arg ? *this : arg;
    BigInt smaller = copy_this < copy_arg ? *this : arg;

    std::vector<int> answer = longer.value;

    for (int i = 0; i < smaller.value.size(); i++) {
        int result = answer[i] - smaller.value[i];
        if (result < 0) {
            if(answer[i + 1] == 0) {
                answer[i + 1] = basis - 1;
                --answer[i + 2];
                for (int j = i + 1; answer[j] == 0; j++) {
                    answer[j] = basis - 1;
                    --answer[j + 1];
                }
            } else {
                --answer[i + 1];
            }

            answer[i] = basis + answer[i] - smaller.value[i];
        } else {
            answer[i] = result;
        }
    }

    for (int i = answer.size() - 1; answer[i] == 0 && i!=0; i--) answer.pop_back();

    BigInt a = BigInt(answer, *this == smaller);
    if (mod.value[mod.value.size() - 1] != 0 && a.is_negative) a = a + mod;

    return a;
}

bool BigInt::operator>(BigInt const &arg) const {
    if (this->is_negative && !arg.is_negative) return false;
    if (!this->is_negative && arg.is_negative) return true;

    if (this->value.size() > arg.value.size()) return !arg.is_negative;
    if (this->value.size() < arg.value.size()) return arg.is_negative;

    for(int i = arg.value.size() - 1; i >= 0; i--) {
        if (this->value[i] > arg.value[i]) return true;
        if (this->value[i] < arg.value[i]) return false;
    }

    return false;
}

bool BigInt::operator<(BigInt const &arg) const {
    return arg > *this;
}

bool BigInt::operator==(BigInt const &arg) const {
    return !(arg > *this || arg < *this);
}

BigInt BigInt::operator*(BigInt const &arg) const {
    bool is_negativ = (this->is_negative || arg.is_negative) && (this->is_negative != arg.is_negative);

    BigInt answer = BigInt(0);
    if (arg == answer || *this == answer) return answer;

    for (int i = 0; i < this->value.size(); i++) {
        std::vector<int> step_value;
        long long add_v = 0; //additional_value
        for (int g = 0; g < arg.value.size(); g++) {
            long long g_value = this->value[i] * arg.value[g] + add_v;
            step_value.push_back(g_value % basis);
            add_v = g_value / basis + add_v;
        }
        if (add_v > 0) step_value.push_back(add_v);
        for (int k = i; k > 0; k--) step_value.insert(step_value.begin(), 0);
        answer = answer + BigInt(step_value, false);
    }

    answer.is_negative = is_negativ;
    if (mod.value[mod.value.size() - 1] != 0) {
        answer = answer - (answer / mod) * mod;
    }

    return answer;
}

BigInt BigInt::operator/(BigInt const &arg) const {
    bool is_negativ = (this->is_negative || arg.is_negative) && (this->is_negative != arg.is_negative);
    std::vector<int> answer = std::vector<int> (1,0); //non-reversed
    BigInt copy_arg = arg;
    copy_arg.is_negative = false;
    BigInt copy = *this;
    copy.is_negative = false;

    if (arg == *this) {
        answer.push_back(1);
        return BigInt(answer, is_negativ);
    }

    if (mod.value[mod.value.size() - 1] != 0) {
        return *this * extended_euclid(arg, mod);
    }

    if (copy_arg > copy) {
        std::vector<int> a = std::vector<int>(1, 0);
        return BigInt(a, false);
    }

    if (arg.value[arg.value.size() - 1] == 0) {
        throw std::exception();
    }

    int right_index = copy.value.size();
    int left_index = right_index - arg.value.size();
    bool previous_turn_zeros = false;
    int r_length = arg.value.size();
    int arg_size = arg.value.size();

    while (copy > arg || copy == arg) {
        right_index = copy.value.size();
        left_index = right_index - arg_size;
        // cut off part
        std::vector<int>::const_iterator right = copy.value.begin() + right_index;
        std::vector<int>::const_iterator left = copy.value.begin() + left_index;
        if (previous_turn_zeros) {
            for (int i = 0; i < arg_size - 1; i++) answer.push_back(0);
        }
        for (int i = 0; i < arg_size - r_length - 1; i++) answer.push_back(0);

        std::vector<int> part = std::vector<int> (left, right);

        if (BigInt(part, false) < copy_arg) {
            --left;
            --left_index;
            part = std::vector<int> (left, right);
            if (right_index != this->value.size() && previous_turn_zeros) answer.push_back(0);
        }

        double a = 1;
        double b = basis - 1;
        BigInt div_arg = BigInt(part, false);

        while (a < b) {
            int mid = (int) ((a + b) / 2);
            std::vector<int> h = std::vector<int> (1, mid);
            BigInt current_multiplier = BigInt(h, false);
            if (arg * current_multiplier == div_arg) {
                a = mid;
                break;
            }
            if ((arg * current_multiplier) > div_arg) {
                b = mid;
            } else {
                a = mid + 1;
            }
        }

        std::vector<int> h = std::vector<int> (1, a);
        BigInt current_multiplier = BigInt(h, false);
        if (arg * current_multiplier > div_arg) {
            --a;
            h[0] = a;
            current_multiplier = BigInt(h, false);
        }

        std::vector<int> m = (arg * current_multiplier).value;
        int size_m = m.size();
        for (int i = left_index; i > 0; i--) m.insert(m.begin(), 0);
        BigInt deduction = BigInt(m, false);

        copy = copy - deduction;
        answer.push_back(a);

        int copy_size = copy.value.size();
        if (copy.value[copy.value.size() - 1] == 0) copy_size = 0;
        previous_turn_zeros = right_index - size_m - copy_size >= 0;
        if (right_index - size_m - copy_size < 0) {
            r_length = -(right_index - size_m - copy_size);
            if (deduction.value.size() < right_index) r_length++;
        } else {
            r_length = arg_size;;
        }

        for (int i = 0; i < right_index - size_m - copy_size; i++) {
            answer.push_back(0);
        }
    }

    if (previous_turn_zeros && copy.value[copy.value.size() - 1] != 0) {
        for (int i = 0; i < arg_size - 1; i++) answer.push_back(0);
    }


    std::reverse(answer.begin(), answer.end());

    return BigInt(answer, is_negativ);
}

BigInt BigInt::operator%(BigInt const &arg) const {
    BigInt copy = *this;
    while (copy > arg || copy == arg) copy = copy - arg;
    return copy;
}

BigInt BigInt::sqrt() {
    std::vector<int> a = std::vector<int> (1, 2);
    BigInt two = BigInt(a, false);
    BigInt x_pre = *this / two;
    BigInt x_next = (x_pre + (*this / x_pre)) / two;
    while (!(x_next == x_pre)) {
        x_pre = x_next;
        x_next = (x_pre + (*this / x_pre)) / two;
    }
    return x_next;
}

BigInt BigInt::euclid(BigInt &a, BigInt &b) {
    BigInt X = a;
    BigInt Y = b;
    BigInt T = b;
    while (Y.value[Y.value.size() - 1] != 0) {
        T = Y;
        Y = Y % X;
        X = T;
    }
    return X;
}

BigInt BigInt::extended_euclid(BigInt const &b, BigInt const &m) {
    BigInt x[] = {BigInt(1), BigInt(0), mod};
    BigInt y[] = {BigInt(0), BigInt(1), b};
    while (true) {
        if (y[2] == BigInt(0)) return BigInt(0);
        if (y[2] == BigInt(1)) return y[1];
        BigInt g = x[2] / y[2];
        BigInt t[] = {x[0] - g * y[0], x[1] - g * y[1], x[2] - g * y[2]};
        x[0] = y[0]; x[1] = y[1]; x[2] = y[2];
        y[0] = t[0]; y[1] = t[1]; y[2] = t[2];
    }
}

void BigInt::set_mod(BigInt const &arg) {
    if (arg.is_negative)
        return;
    mod = arg;
}

BigInt::BigInt() {
    is_negative = false;
    value = std::vector<int> (1,0);
}

BigInt BigInt::mod = BigInt();

BigInt BigInt::operator^(BigInt const &arg) const {
    int argument = 0;
    try {
        std::string s = arg.to_string();
        argument = std::stoi(s);
    } catch (std::out_of_range a) {
        std::cout << a.what();
    }
    BigInt answer = *this;
    short count_iterations_multiply = 0;
    while (argument > 0) {
        int previous_argument = argument;
        argument = argument >> 1;
        argument = argument << 1;
        if (previous_argument != argument) {
            BigInt temp_multiplier = *this;
            for (int i = 0; i < count_iterations_multiply; i++) {
                temp_multiplier = temp_multiplier * temp_multiplier;
            }
            answer = answer * temp_multiplier;
        }
        count_iterations_multiply++;
        argument = argument >> 1;
    }
    return answer / *this;
}

BigInt BigInt::chinese_tea(const std::vector<BigInt> &r, const std::vector<BigInt> &m) {
    BigInt x = BigInt(0);
    BigInt M = BigInt(1);
    for (int i = 0; i < m.size(); i++) {
        M = M * (m[i]);
    }
    std::vector<BigInt> y = std::vector<BigInt> (m.size(), BigInt(0));
    std::vector<BigInt> s = std::vector<BigInt> (m.size(), BigInt(0));
    std::vector<BigInt> c = std::vector<BigInt> (m.size(), BigInt(0));
    for (int i = 0; i < m.size(); i++) {
        y[i] = M / m[i];
        BigInt::set_mod(m[i]);
        s[i] = BigInt::extended_euclid(y[i], m[i]);
        c[i] = r[i] * s[i];
        x = x + c[i] * y[i];
    }
    return x;
}
