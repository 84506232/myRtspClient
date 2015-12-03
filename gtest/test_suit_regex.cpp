#include <stdio.h>
#include <sys/types.h>
#include <regex.h>
#include <memory.h>
#include <stdlib.h>
#include "myRegex.h"
#include <string>
#include <list>
#include <iostream>
#include <gtest/gtest.h>

using namespace std;

TEST(myRegex, Regex_InvalidInput)
{
    MyRegex Regex;

    char str[] = "ansersion";
    char pattern[] = "ansersion";
    list<string> group;
    bool IgnoreCase = true;

    /* int Regex(const char * str, const char * pattern, list<string> * groups, bool ignore_case = false); */
    EXPECT_EQ(Regex.Regex(NULL, pattern, &group, IgnoreCase), MyRegex::REGEX_FAILURE);
    EXPECT_EQ(Regex.Regex(str, NULL, &group, IgnoreCase), MyRegex::REGEX_FAILURE);
    EXPECT_EQ(Regex.Regex(str, pattern, NULL, IgnoreCase), MyRegex::REGEX_FAILURE);

    EXPECT_EQ(Regex.Regex(NULL, pattern, &group, !IgnoreCase), MyRegex::REGEX_FAILURE);
    EXPECT_EQ(Regex.Regex(str, NULL, &group, !IgnoreCase), MyRegex::REGEX_FAILURE);
    EXPECT_EQ(Regex.Regex(str, pattern, NULL,!IgnoreCase), MyRegex::REGEX_FAILURE);

    /* int Regex(const char * str, const char * pattern, bool ignore_case = false); */
    EXPECT_EQ(Regex.Regex(NULL, pattern, IgnoreCase), MyRegex::REGEX_FAILURE);
    EXPECT_EQ(Regex.Regex(str, NULL, IgnoreCase), MyRegex::REGEX_FAILURE);

    EXPECT_EQ(Regex.Regex(NULL, pattern, !IgnoreCase), MyRegex::REGEX_FAILURE);
    EXPECT_EQ(Regex.Regex(str, NULL, !IgnoreCase), MyRegex::REGEX_FAILURE);
}

TEST(myRegex, Regex_RegularInput)
{
    char str[] = "ansersion";
    char str_with_brackets[] = "an(sersion";
    char str_with_new_line[] = "ansersion\nwan_ansersion";
    char pattern[] = "anser(sion)";
    char pattern_with_brackets[] = "an\\(sersion";
    MyRegex Regex;
    list<string> group;

    EXPECT_EQ(Regex.Regex(str, pattern, &group), MyRegex::REGEX_SUCCESS);
    EXPECT_EQ(Regex.Regex(str_with_brackets, pattern_with_brackets, &group), MyRegex::REGEX_SUCCESS);
    EXPECT_EQ(Regex.Regex(str_with_new_line, pattern, &group), MyRegex::REGEX_SUCCESS);
}

TEST(myRegex, RegexLine_RegularInput)
{
    string str = "\
                                 Apache License\n\
                           Version 2.0, January 2004\n\
                        http://www.apache.org/licenses/\n\
			\n\
   TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION\n\
   ";
    string LineWillBeMatched("                        http://www.apache.org/licenses/"); 
    string PartWillBeMatched("http://www.apache");
    string GroupWillBeMatched("apache");


    string pattern = "http:.*(apache)";
    MyRegex Regex;
    list<string> group;

    while(Regex.RegexLine(&str, &pattern, &group)) {
	if(!group.empty()) {
	    EXPECT_EQ(true, LineWillBeMatched == group.front());
	    group.pop_front();
	    EXPECT_EQ(true, PartWillBeMatched == group.front());
	    group.pop_front();
	    EXPECT_EQ(true, GroupWillBeMatched == group.front());
	    group.pop_front();
	}
    }
}
