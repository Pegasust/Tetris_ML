#include "string_manip.h"

std::string Common::ryu_d2s(const double& d) {
    static char arr[Common::RYU_BUFFER_SIZE];
    d2s_buffered(d, arr);
    std::string retval = std::string(arr);
    return retval;
}

std::string Common::precise_to_string(double d) {
    std::ostringstream stream;
    stream << std::setprecision(std::numeric_limits<double>::max_digits10) << d;
    return stream.str();
}
double Common::crack_atof(const char* num) {
    if (!num || !*num) {
        return 0;
    }

    int sign = 1;
    double integerPart = 0.0;
    double fractionPart = 0.0;
    bool hasFraction = false;
    bool hasExpo = false;

    /*Take care of +/- sign*/
    if (*num == '-') {
        ++num;
        sign = -1;
    } else if (*num == '+') {
        ++num;
    }

    while (*num != '\0') {
        if (*num >= '0' && *num <= '9') {
            integerPart = integerPart * 10 + (*num - '0');
        } else if (*num == '.') {
            hasFraction = true;
            ++num;
            break;
        } else if (*num == 'e') {
            hasExpo = true;
            ++num;
            break;
        } else {
            return sign * integerPart;
        }
        ++num;
    }

    if (hasFraction) {
        double fractionExpo = 0.1;

        while (*num != '\0') {
            if (*num >= '0' && *num <= '9') {
                fractionPart += fractionExpo * (*num - '0');
                fractionExpo *= 0.1;
            } else if (*num == 'e') {
                hasExpo = true;
                ++num;
                break;
            } else {
                return sign * (integerPart + fractionPart);
            }
            ++num;
        }
    }

    // parsing exponent part
    double expPart = 1.0;
    if (*num != '\0' && hasExpo) {
        int expSign = 1;
        if (*num == '-') {
            expSign = -1;
            ++num;
        } else if (*num == '+') {
            ++num;
        }

        int e = 0;
        while (*num != '\0' && *num >= '0' && *num <= '9') {
            e = e * 10 + *num - '0';
            ++num;
        }

        expPart = Common::pow10(expSign * e);
    }
    // Accuracy drop is not from sign.
    return sign * (integerPart + fractionPart) * expPart;
}

double Common::crack_atof(const char* num, const char* const end) {
    if (!num || !end || end <= num) {
        return 0;
    }

    int sign = 1;
    double int_part = 0.0;
    double frac_part = 0.0;
    bool has_frac = false;
    bool has_exp = false;

    // +/- sign
    if (*num == '-') {
        ++num;
        sign = -1;
    } else if (*num == '+') {
        ++num;
    }

    while (num != end) {
        if (*num >= '0' && *num <= '9') {
            int_part = int_part * 10 + (*num - '0');
        } else if (*num == '.') {
            has_frac = true;
            ++num;
            break;
        } else if (*num == 'e') {
            has_exp = true;
            ++num;
            break;
        } else {
            return sign * int_part;
        }
        ++num;
    }

    if (has_frac) {
        double frac_exp = 0.1;

        while (num != end) {
            if (*num >= '0' && *num <= '9') {
                frac_part += frac_exp * (*num - '0');
                frac_exp *= 0.1;
            } else if (*num == 'e') {
                has_exp = true;
                ++num;
                break;
            } else {
                return sign * (int_part + frac_part);
            }
            ++num;
        }
    }

    // parsing exponent part
    double exp_part = 1.0;
    if (num != end && has_exp) {
        int exp_sign = 1;
        if (*num == '-') {
            exp_sign = -1;
            ++num;
        } else if (*num == '+') {
            ++num;
        }

        int e = 0;
        while (num != end && *num >= '0' && *num <= '9') {
            e = e * 10 + *num - '0';
            ++num;
        }

        exp_part = Common::pow10(exp_sign * e);
    }

    return sign * (int_part + frac_part) * exp_part;
}

std::vector<std::string> Common::split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}