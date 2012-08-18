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
namespace boost_po = boost::program_options;

struct ProgramArguments {
    bool keep_case;
    bool keep_nonword;
    string string1;
    string string2;
} g_program_args;

void Fatal(const string& msg) {
    cerr << "check_anagram: error: " << msg << endl;
    exit(EXIT_FAILURE);
}

void Usage(const boost_po::options_description& options_desc) {
    cerr << "usage: check_anagram [options] string1 string2\n\n"
            "Prints \"Yes\" if string1 and string2 are anagrams, \"No\" otherwise.\n\n";
    cerr << options_desc;
}

void ParseProgramArguments(int argc, char** argv) {
    bool show_usage;

    boost_po::options_description visible_options("Options");
    visible_options.add_options()
        ("help,h", boost_po::bool_switch(&show_usage), "show usage info")
        ("keep-case,c", boost_po::bool_switch(&g_program_args.keep_case),
                "do not ignore case")
        ("keep-nonword,w", boost_po::bool_switch(&g_program_args.keep_nonword),
                "do not ignore non-word characters");

    // Put the positional arguments in a separate group so they can be omitted
    // from Usage().
    boost_po::options_description hidden_options;
    hidden_options.add_options()
        ("string1", boost_po::value<string>(&g_program_args.string1))
        ("string2", boost_po::value<string>(&g_program_args.string2));

    boost_po::positional_options_description positional_args;
    positional_args.add("string1", 1);
    positional_args.add("string2", 1);

    boost_po::options_description options;
    options.add(visible_options);
    options.add(hidden_options);

    boost_po::command_line_parser parser(argc, argv);
    parser.options(options);
    parser.positional(positional_args);
    boost_po::variables_map vars_map;
    try {
        boost_po::store(parser.run(), vars_map);
    } catch (boost_po::error_with_option_name& e) {
        Fatal(e.what());
    } catch (boost_po::too_many_positional_options_error& e) {
        Fatal("too many inputs");
    }
    boost_po::notify(vars_map);

    if (show_usage) {
        Usage(visible_options);
        exit(EXIT_SUCCESS);
    }

    if (g_program_args.string1.empty() || g_program_args.string2.empty())
        Fatal("not enough inputs");
}

string StripInput(string input) {
    if (!g_program_args.keep_case)
        boost::to_lower(input);
    if (!g_program_args.keep_nonword) {
        boost::regex strip_regex("\\W");
        input = boost::regex_replace(input, strip_regex, "");
    }
    return input;
}

bool CheckAnagrams(string a, string b) {
    // Fast path: a and b cannot be anagrams if their lengths differ.
    if (a.length() != b.length())
        return false;

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    return a.compare(b) == 0;
}

int main(int argc, char** argv) {
    ParseProgramArguments(argc, argv);

    string stripped_string1 = StripInput(g_program_args.string1);
    string stripped_string2 = StripInput(g_program_args.string2);

    if (CheckAnagrams(stripped_string1, stripped_string2))
        cout << "Yes";
    else
        cout << "No";
    cout << endl;
    return 0;
}

