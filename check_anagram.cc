// Copyright 2012 Philip Puryear
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <algorithm>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

using namespace std;

struct ProgramArguments {
    bool keep_case;
    bool keep_nonword;
    string ignore_chars;
    string string1;
    string string2;
} g_program_args;

void Fatal(const string& msg) {
    cerr << "check_anagram: error: " << msg << endl;
    exit(EXIT_FAILURE);
}

string RegexEscape(const string& s) {
    const boost::regex special_chars_regex(
            "[\\^\\.\\$\\|\\{\\}\\(\\)\\[\\]\\*\\+\\?\\/\\\\]");
    const string replacement("\\\\\\1&");
    return boost::regex_replace(s, special_chars_regex, replacement,
            boost::match_default | boost::format_sed);
}

boost::regex CreateStripRegex() {
    string regex_string;
    if (!g_program_args.keep_nonword)
        regex_string += "\\W";
    if (!g_program_args.ignore_chars.empty())
        regex_string += RegexEscape(g_program_args.ignore_chars);

    if (!regex_string.empty()) {
        regex_string.insert(regex_string.begin(), '[');
        regex_string.push_back(']');
    }
    return boost::regex(regex_string.c_str());
}

string StripInput(string input, const boost::regex& strip_regex) {
    if (!g_program_args.keep_case)
        boost::to_lower(input);
    return boost::regex_replace(input, strip_regex, "");
}

bool CheckAnagrams(string a, string b) {
    // Fast path: a and b cannot be anagrams if their lengths differ.
    if (a.length() != b.length())
        return false;

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    return a.compare(b) == 0;
}

void Usage(boost::program_options::options_description options_desc) {
    cout << "usage: check_anagram [options] string1 string2\n\n";
    cout << "Determine if two strings are anagrams.\n\n";
    cout << options_desc << endl;
}

void ParseProgramArguments(int argc, char** argv) {
    namespace po = boost::program_options;

    bool show_usage;

    po::options_description visible_options("Options");
    visible_options.add_options()
        ("help,h", po::bool_switch(&show_usage), "show usage info")
        ("keep-case,c", po::bool_switch(&g_program_args.keep_case),
                "do not ignore case")
        ("keep-nonword,w", po::bool_switch(&g_program_args.keep_nonword),
                "do not ignore non-word characters")
        ("ignore-chars", po::value<string>(&g_program_args.ignore_chars),
                "ignore all of the characters in the given string");

    // Put the positional arguments in a separate group so they can be omitted
    // from Usage().
    po::options_description hidden_options;
    hidden_options.add_options()
        ("string1", po::value<string>(&g_program_args.string1))
        ("string2", po::value<string>(&g_program_args.string2));

    po::positional_options_description positional_args;
    positional_args.add("string1", 1);
    positional_args.add("string2", 1);

    po::options_description options;
    options.add(visible_options);
    options.add(hidden_options);

    po::command_line_parser parser(argc, argv);
    parser.options(options);
    parser.positional(positional_args);
    po::variables_map vars_map;
    try {
        po::store(parser.run(), vars_map);
    } catch (po::error_with_option_name& e) {
        Fatal(e.what());
    } catch (po::too_many_positional_options_error& e) {
        Fatal("too many inputs");
    }
    po::notify(vars_map);

    if (show_usage) {
        Usage(visible_options);
        exit(EXIT_SUCCESS);
    }

    if (g_program_args.string1.empty() || g_program_args.string2.empty())
        Fatal("not enough inputs");
}

int main(int argc, char** argv) {
    ParseProgramArguments(argc, argv);

    boost::regex strip_regex = CreateStripRegex();
    string stripped_string1 = StripInput(g_program_args.string1, strip_regex);
    string stripped_string2 = StripInput(g_program_args.string2, strip_regex);

    if (CheckAnagrams(stripped_string1, stripped_string2))
        cout << "Yes.";
    else
        cout << "No.";
    cout << endl;
    return 0;
}

