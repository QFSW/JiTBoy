#pragma once

#include <string>
#include <regex>
#include <functional>
#include <stdexcept>

namespace mips
{
    template <typename Inner, typename...Ts>
    class RegexParser
    {
    public:
        explicit RegexParser(std::regex&& pattern);
        explicit RegexParser(std::regex&& pattern, std::string&& err_string);
        explicit RegexParser(Inner&& inner, std::regex&&);
        explicit RegexParser(Inner&& inner, std::regex&&, std::string&& err_string);

        bool try_evaluate(const std::string&, std::tuple<Ts...>& result) const;
        bool try_evaluate(const std::string&, std::tuple<Ts&...> result) const;
        bool try_evaluate(const std::string&, Ts&... results) const;
        std::tuple<Ts...> evaluate(const std::string& raw) const;

    private:
        Inner _inner;
        std::regex _regex;
        std::smatch _matches;
        std::string _err_string;
        std::function<bool(const std::string&, std::tuple<Ts...>&)> _evaluator;

        static constexpr auto default_err = "Could not parse %s";

        template <typename A>
        std::tuple<A> parse(const std::smatch& m, int i);

        template <typename A1, typename A2, typename...As>
        std::tuple<A1, A2, As...> parse(const std::smatch& m, int i);
    };

    template <typename Inner, typename...Ts>
    RegexParser<Inner, Ts...>::RegexParser(std::regex&& pattern)
        : RegexParser(Inner(), std::move(pattern))
    { }

    template <typename Inner, typename...Ts>
    RegexParser<Inner, Ts...>::RegexParser(std::regex&& pattern, std::string&& err_string)
        : RegexParser(Inner(), std::move(pattern), std::move(err_string))
    { }

    template <typename Inner, typename...Ts>
    RegexParser<Inner, Ts...>::RegexParser(Inner&& inner, std::regex&& pattern)
        : RegexParser(Inner(), std::move(pattern), default_err)
    { }

    template <typename Inner, typename...Ts>
    RegexParser<Inner, Ts...>::RegexParser(Inner&& inner, std::regex&& pattern, std::string&& err_string)
        : _inner(inner)
        , _regex(pattern)
        , _err_string(err_string)
    {
        _evaluator = [&](const std::string& raw, std::tuple<Ts...>& result)
        {
            if (std::regex_search(raw, _matches, _regex) && _matches.size() == sizeof...(Ts) + 1)
            {
                result = parse<Ts...>(_matches, 1);
                return true;
            }

            return false;
        };
    }

    template <typename Inner, typename...Ts>
    bool RegexParser<Inner, Ts...>::try_evaluate(const std::string& raw, std::tuple<Ts...>& result) const
    {
        return _evaluator(raw, result);
    }

    template <typename Inner, typename...Ts>
    bool RegexParser<Inner, Ts...>::try_evaluate(const std::string& raw, std::tuple<Ts&...> result) const
    {
        if (std::tuple<Ts...> r; try_evaluate(raw, r))
        {
            result = std::move(r);
            return true;
        }

        return false;
    }

    template <typename Inner, typename...Ts>
    bool RegexParser<Inner, Ts...>::try_evaluate(const std::string& raw, Ts&... results) const
    {
        return try_evaluate(raw, std::tie(results...));
    }

    template <typename Inner, typename...Ts>
    std::tuple<Ts...> RegexParser<Inner, Ts...>::evaluate(const std::string& raw) const
    {
        std::tuple<Ts...> result;
        if (try_evaluate(raw, result))
            return result;

        throw std::invalid_argument(strtools::catf(_err_string.c_str(), raw.c_str()));
    }

    template <typename Inner, typename...Ts>
    template <typename A>
    std::tuple<A> RegexParser<Inner, Ts...>::parse(const std::smatch& m, int i)
    {
        return _inner.template parse<A>(m[i]);
    }

    template <typename Inner, typename...Ts>
    template <typename A1, typename A2, typename...As>
    std::tuple<A1, A2, As...> RegexParser<Inner, Ts...>::parse(const std::smatch& m, int i)
    {
        std::tuple<A1> t = parse<A1>(m, i);
        std::tuple<A2, As...> ts = parse<A2, As...>(m, i + 1);
        return std::tuple_cat(t, ts);
    }
}
