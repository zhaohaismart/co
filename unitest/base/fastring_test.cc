#include "base/unitest.h"
#include "base/fastring.h"

namespace test {

DEF_test(fastring) {
    DEF_case(base) {
        fastring s;
        EXPECT(s.empty());
        EXPECT_EQ(s.size(), 0);        

        s = "xxx";
        EXPECT_EQ(s, "xxx");

        fastring t = s; // s and t refer to the same string
        EXPECT_EQ(*(void**)&s, *(void**)&t);
        EXPECT_EQ(s.data(), t.data());

        s.reserve(128);
        EXPECT_EQ(s, t);
        EXPECT_NE(s.data(), t.data());
    }

    DEF_case(append) {
        fastring s;
        s.append('x', 3);
        EXPECT_EQ(s.size(), 3);
        EXPECT_EQ(s, "xxx");

        s.append(s); // append self
        EXPECT_EQ(s, "xxxxxx");

        s.resize(5);
        EXPECT_EQ(s, "xxxxx");

        s = s.c_str() + 1;
        EXPECT_EQ(s, "xxxx");

        s.append(s.c_str() + 2);
        EXPECT_EQ(s, "xxxxxx");

        s.resize(3);

        s += "yyy";
        EXPECT_EQ(s, "xxxyyy");

        s += "zzz";
        EXPECT_EQ(s, "xxxyyyzzz");

        fastring s1 = s + "...";
        EXPECT_EQ(s1, "xxxyyyzzz...");

        s1 = s1 + "...";
        EXPECT_EQ(s1, "xxxyyyzzz......");

        s.swap(fastring());
        s.append('x');
        EXPECT_EQ(s, "x");
    }

    DEF_case(substr) {
        fastring s = "helloworld";
        EXPECT_EQ(s.substr(0), s);
        EXPECT_EQ(s.substr(0, 2), "he");
        EXPECT_EQ(s.substr(5), "world");
        EXPECT_EQ(s.substr(5, 2), "wo");
    }

    DEF_case(cmp) {
        fastring s = "88888888";
        EXPECT_EQ(s.size(), 8);
        EXPECT_EQ(s, "88888888");
        EXPECT_NE(s, "8888888");
        EXPECT_NE(s, "888888888");
        EXPECT_NE(s, "xxxxxx");

        EXPECT_GT(s, "7777777")
        EXPECT_GT(s, "77777777")
        EXPECT_GT(s, "777777777")
        EXPECT_LT(s, "9999999");
        EXPECT_LT(s, "99999999");
        EXPECT_LT(s, "999999999");
        EXPECT_GE(s, "88888888");
        EXPECT_GE(s, "777777777")
        EXPECT_LE(s, "88888888");
        EXPECT_LE(s, "9999999");
    }

    DEF_case(find) {
        fastring s("xxxyyyzzz");
        EXPECT_EQ(s.find('a'), s.npos);
        EXPECT_EQ(s.find('y'), 3);
        EXPECT_EQ(s.find('y', 4), 4);
        EXPECT_EQ(s.find('y', 32), s.npos);
        EXPECT_EQ(s.rfind('y'), 5);
        EXPECT_EQ(s.rfind("xyz"), s.npos);
        EXPECT_EQ(s.rfind("xy"), 2);
        EXPECT_EQ(s.rfind("yy"), 4);
        EXPECT_EQ(fastring("0123456789").rfind("0"), 0);
        EXPECT_EQ(fastring("0123456789").rfind("01"), 0);
        EXPECT_EQ(fastring("0123456789").rfind("012"), 0);
        EXPECT_EQ(fastring("0123456789").rfind("0123456789"), 0);
        EXPECT_EQ(fastring("0123456789").rfind("01234567890"), s.npos);
        EXPECT_EQ(fastring("0123456789").rfind("1234"), 1);
        EXPECT_EQ(fastring("0123456789").rfind("23456"), 2);
        EXPECT_EQ(fastring("0123456789").rfind("345678"), 3);
        EXPECT_EQ(fastring("0123456789").rfind("56789"), 5);
        EXPECT_EQ(fastring("0123456789").rfind("0124"), s.npos);

        EXPECT_EQ(s.find("xy"), 2);
        EXPECT_EQ(s.find("yy"), 3);
        EXPECT_EQ(s.find("yy", 4), 4);

        EXPECT_EQ(s.find_first_of("xy"), 0);
        EXPECT_EQ(s.find_first_of("yz"), 3);
        EXPECT_EQ(s.find_first_not_of('x'), 3);
        EXPECT_EQ(s.find_first_not_of("xy"), 6);

        s = "xyz";
        EXPECT_EQ(s.find_last_of("ax"), 0);
        EXPECT_EQ(s.find_last_of("xy"), 1);
        EXPECT_EQ(s.find_last_of("yz"), 2);
        EXPECT_EQ(s.find_last_of("xz"), 2);
        EXPECT_EQ(s.find_last_of("abc"), s.npos);
        EXPECT_EQ(s.find_last_not_of('x'), 2);
        EXPECT_EQ(s.find_last_not_of('z'), 1);
        EXPECT_EQ(s.find_last_not_of("yz"), 0);
        EXPECT_EQ(s.find_last_not_of("xyz"), s.npos);
    }

    DEF_case(replace) {
        fastring s("1122332211");
        const char* p = s.data();

        s.replace("22", "xx");
        EXPECT_EQ(s, "11xx33xx11");
        EXPECT_EQ(s.data(), p);

        s.replace("xx", "22", 1);
        EXPECT_EQ(s, "112233xx11");
        EXPECT_EQ(s.data(), p);

        s = "xxxxx";
        s.replace("xx", "yy");
        EXPECT_EQ(s, "yyyyx");

        s = "xxxxx";
        s.replace("xxx", "x");
        EXPECT_EQ(s, "xxx");

        s = "xxxxxxxxx";
        s.replace("xxx", "x");
        EXPECT_EQ(s, "xxx");
    }

    DEF_case(strip) {
        fastring s("xx1122332211");
        const char* p = s.data();

        s.strip("x", 'l');
        EXPECT_EQ(s, "1122332211");
        EXPECT_EQ(s.data(), p);

        s.strip("1", 'r');
        EXPECT_EQ(s, "11223322");
        EXPECT_EQ(s.data(), p);

        s.strip("12");
        EXPECT_EQ(s, "33");
        EXPECT_EQ(s.data(), p);

        s.strip("3");
        EXPECT_EQ(s, "");
        EXPECT_EQ(s.data(), p);

        s = "xxxyyy";
        s.strip("xy");
        EXPECT_EQ(s, "");

        s = "xxx";
        s.strip("xy", 'r');
        EXPECT_EQ(s, "");

        s = "xxx";
        s.strip("xy", 'l');
        EXPECT_EQ(s, "");
    }

    DEF_case(starts_ends) {
        fastring s("xxxyyyzzz");
        EXPECT(s.starts_with('x'));
        EXPECT(s.starts_with("xx"));
        EXPECT(s.starts_with("xxx"));
        EXPECT(s.starts_with("xxxy"));
        EXPECT(s.starts_with("xxxyyy"));
        EXPECT(s.starts_with("xxxyyy", 6));
        EXPECT(s.ends_with('z'));
        EXPECT(s.ends_with("zzz"));
        EXPECT(s.ends_with("zzz", 3));
        EXPECT(s.ends_with("yyzzz", 5));
    }

    DEF_case(upperlower) {
        fastring s("xYz88");
        EXPECT_EQ(s.upper(), "XYZ88");
        EXPECT_EQ(s.lower(), "xyz88");
        EXPECT_EQ(s, "xYz88");

        s.tolower();
        EXPECT_EQ(s, "xyz88");

        s.toupper();
        EXPECT_EQ(s, "XYZ88");
    }

    DEF_case(match) {
        fastring s("hello");
        EXPECT(s.match("*"));
        EXPECT(s.match("*hello"));
        EXPECT(s.match("*ello"));
        EXPECT(s.match("h*"));
        EXPECT(s.match("h*o"));
        EXPECT(s.match("he??o"));
        EXPECT(s.match("h?llo"));
        EXPECT(s.match("h?l*"));
        EXPECT(s.match("h*l?o"));
        EXPECT(!s.match("h?o"));
    }

    DEF_case(plus) {
        fastring a("hello");
        fastring b("world");
        const char* s = "again";

        EXPECT_EQ(a + b, "helloworld");
        EXPECT_EQ(b + a, "worldhello");
        EXPECT_EQ(a + s, "helloagain");
        EXPECT_EQ(s + a, "againhello");
    }
}

} // namespace test