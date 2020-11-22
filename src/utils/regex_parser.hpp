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
        explicit RegexParser(const std::string& pattern);
        explicit RegexParser(Inner&& inner, const std::string& pattern);
        std::tuple<Ts...> evaluate(const std::string& raw) const;

    private:
        Inner _inner;
        std::regex _regex;
        std::function<std::tuple<Ts...>(const std::string&)> _evaluator;

        template <typename A>
        std::tuple<A> parse(const std::smatch& m, int i);

        template <typename A1, typename A2, typename...As>
        std::tuple<A1, A2, As...> parse(const std::smatch& m, int i);
    };

    template <typename Inner, typename...Ts>
    RegexParser<Inner, Ts...>::RegexParser(const std::string& pattern)
        : RegexParser(Inner(), pattern)
    { }

    template <typename Inner, typename...Ts>
    RegexParser<Inner, Ts...>::RegexParser(Inner&& inner, const std::string& pattern)
        : _inner(inner)
    {
        _regex = std::regex(pattern);
        _evaluator = [&](const std::string& raw)
        {
            std::smatch matches;
            if (std::regex_search(raw, matches, _regex) && matches.size() == sizeof...(Ts) + 1)
            {
                return parse<Ts...>(matches, 1);
            }

            throw std::logic_error("Could not parse " + raw);
        };
    }

    template <typename Inner, typename...Ts>
    std::tuple<Ts...> RegexParser<Inner, Ts...>::evaluate(const std::string& raw) const
    {
        return _evaluator(raw);
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
