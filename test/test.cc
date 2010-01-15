/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <test/test.hh>

#include <cstdlib>
#include <iostream>
#include <list>
#include <sstream>

namespace test
{
    std::list<const TestCase *> test_cases;

    TestCase::TestCase(const std::string & name) :
        _name(name)
    {
        test_cases.push_back(this);
    }

    TestCase::~TestCase()
    {
    }

    std::string
    TestCase::name() const
    {
        return _name;
    }

    TestCaseFailedException::TestCaseFailedException(int line, const std::string & file, const std::string & reason) :
        _line(line),
        _file(file),
        _reason(reason)
    {
    }

    const std::string &
    TestCaseFailedException::reason() const
    {
        return _reason;
    }

    std::string
    TestCaseFailedException::where() const
    {
        std::stringstream ss;
        ss << _line;

        return _file + ":" + ss.str();
    }
}

int main(int argc, char ** argv)
{
    int result(EXIT_SUCCESS);

    for (std::list<const test::TestCase *>::const_iterator i(test::test_cases.begin()),
            i_end(test::test_cases.end()) ; i != i_end ; ++i)
    {
        std::cout << "Running test case '" << (*i)->name() << "'" << std::endl;

        try
        {
            (*i)->run();
        }
        catch (test::TestCaseFailedException & e)
        {
            std::cout << "Test case failed: " << std::endl << e.where() << ":" << e.reason() << std::endl;
            result = EXIT_FAILURE;

            continue;
        }
        catch (std::exception & e)
        {
            std::cout << "Test case threw exception: " << e.what() << std::endl;
            result = EXIT_FAILURE;

            continue;
        }
    }

    return result;
}