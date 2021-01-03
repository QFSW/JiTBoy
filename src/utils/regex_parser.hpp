#pragma once

#include <string>
#include <regex>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <error/parse_error.hpp>

namespace mips
{
    template <typename Inner, typename...Ts>
    class regex_parser
    {
    public:
        explicit regex_parser(std::regex&& pattern);
        explicit regex_parser(std::regex&& pattern, std::string&& err_string);
        explicit regex_parser(Inner&& inner, std::regex&&);
        explicit regex_parser(Inner&& inner, std::regex&&, std::string&& err_string);

        [[nodiscard]] std::tuple<Ts...> evaluate(const std::string& raw) const;
        bool try_evaluate(const std::string&, std::tuple<Ts...>& result) const;
        bool try_evaluate(const std::string&, std::tuple<Ts&...> result) const;
        bool try_evaluate(const std::string&, Ts&... results) const;

    private:
        Inner _inner;
        std::regex _regex;
        std::smatch _matches;
        std::string _err_string;
        std::function<bool(const std::string&, std::tuple<Ts...>&)> _evaluator;
        std::unordered_map<std::string, std::tuple<Ts...>> _parse_cache;
        std::unordered_set<std::string> _parse_fail_cache;

        static constexpr auto default_err = "Could not parse %s";

        template <typename A>
        std::tuple<A> parse(const std::smatch& m, int i);

        template <typename A1, typename A2, typename...As>
        std::tuple<A1, A2, As...> parse(const std::smatch& m, int i);
    };

    template <typename Inner, typename...Ts>
    regex_parser<Inner, Ts...>::regex_parser(std::regex&& pattern)
        : regex_parser(Inner(), std::move(pattern))
    { }

    template <typename Inner, typename...Ts>
    regex_parser<Inner, Ts...>::regex_parser(std::regex&& pattern, std::string&& err_string)
        : regex_parser(Inner(), std::move(pattern), std::move(err_string))
    { }

    template <typename Inner, typename...Ts>
    regex_parser<Inner, Ts...>::regex_parser(Inner&& inner, std::regex&& pattern)
        : regex_parser(Inner(), std::move(pattern), default_err)
    { }

    template <typename Inner, typename...Ts>
    regex_parser<Inner, Ts...>::regex_parser(Inner&& inner, std::regex&& pattern, std::string&& err_string)
        : _inner(inner)
        , _regex(pattern)
        , _err_string(err_string)
    {
        _evaluator = [&](const std::string& raw, std::tuple<Ts...>& result)
        {
            if (auto it = _parse_cache.find(raw); it != _parse_cache.end())
            {
                result = it->second;
                return true;
            }

            if (_parse_fail_cache.contains(raw))
                return false;

            if (std::regex_search(raw, _matches, _regex) && _matches.size() == sizeof...(Ts) + 1)
            {
                result = _parse_cache[raw] = parse<Ts...>(_matches, 1);
                return true;
            }

            _parse_fail_cache.emplace(raw);
            return false;
        };
    }

    template <typename Inner, typename...Ts>
    std::tuple<Ts...> regex_parser<Inner, Ts...>::evaluate(const std::string& raw) const
    {
        std::tuple<Ts...> result;
        if (_evaluator(raw, result))
            return result;

        throw parse_error(strtools::catf(_err_string.c_str(), raw.c_str()));
    }

    template <typename Inner, typename...Ts>
    bool regex_parser<Inner, Ts...>::try_evaluate(const std::string& raw, std::tuple<Ts...>& result) const
    {
        try
        {
            return _evaluator(raw, result);
        }
        catch (const parse_error&)
        {
            return false;
        }
    }

    template <typename Inner, typename...Ts>
    bool regex_parser<Inner, Ts...>::try_evaluate(const std::string& raw, std::tuple<Ts&...> result) const
    {
        if (std::tuple<Ts...> r; try_evaluate(raw, r))
        {
            result = std::move(r);
            return true;
        }

        return false;
    }

    template <typename Inner, typename...Ts>
    bool regex_parser<Inner, Ts...>::try_evaluate(const std::string& raw, Ts&... results) const
    {
        return try_evaluate(raw, std::tie(results...));
    }

    template <typename Inner, typename...Ts>
    template <typename A>
    std::tuple<A> regex_parser<Inner, Ts...>::parse(const std::smatch& m, int i)
    {
        return _inner.template parse<A>(m[i]);
    }

    template <typename Inner, typename...Ts>
    template <typename A1, typename A2, typename...As>
    std::tuple<A1, A2, As...> regex_parser<Inner, Ts...>::parse(const std::smatch& m, int i)
    {
        std::tuple<A1> t = parse<A1>(m, i);
        std::tuple<A2, As...> ts = parse<A2, As...>(m, i + 1);
        return std::tuple_cat(t, ts);
    }
}
