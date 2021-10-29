#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "CppUnitTest.h"
#include "Pars.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	BEGIN_TEST_MODULE_ATTRIBUTE()
		TEST_MODULE_ATTRIBUTE(L"Date", L"2021/2/10")
		END_TEST_MODULE_ATTRIBUTE()

		TEST_MODULE_INITIALIZE(ModuleInitialize)
	{
		Logger::WriteMessage("In Module Initialize");
	}

	TEST_MODULE_CLEANUP(ModuleCleanup)
	{
		Logger::WriteMessage("In Module Cleanup");
	}

	TEST_CLASS(UnitTest1)
	{
    public:
        char **words;
        Parser test;
        int count_;
        string* err;
        bool* end;
        void free_w() {

            for (int i = 0; i < count_; ++i) {
                if (words[i] != NULL) {
                    free(words[i]);
                    words[i] = NULL;
                }
            }
            free(words);
            words = NULL;
        }
        UnitTest1()
        {
            Logger::WriteMessage("In UnitTest");
        }

        ~UnitTest1()
        {
            Logger::WriteMessage("In ~UnitTest");
        }

        TEST_CLASS_INITIALIZE(ClassInitialize)
        {
            Logger::WriteMessage("In Class Initialize");
        }

        TEST_CLASS_CLEANUP(ClassCleanup)
        {
            Logger::WriteMessage("In Class Cleanup");
        }
        TEST_METHOD(Char_to_int)
        {
          //  Assert::AreEqual(test.do_cmd("clear 11f", end, err), false);
            Assert::AreEqual(test.char_to_int("1111"), 1111);
            Assert::AreEqual(test.char_to_int("11d1"), -999);
            Assert::AreEqual(test.char_to_int("067"), 67);
        }
        TEST_METHOD(Clear)
        {
            Assert::AreEqual(test.do_cmb("clear 123123", end, err), true);
            Assert::AreEqual(test.do_cmd("clear 123123", end, err), false);
            Assert::AreEqual(test.do_cmd("clear 1231231", end, err), false);
            Assert::AreEqual(test.do_cmd("clear 12d123", end, err), false);
            Assert::AreEqual(test.do_cmd("clear 123123", end, err), false);
        }
        TEST_METHOD(Pixel)
        {
            Assert::AreEqual(test.do_cmb("pixel 123 123 123123", end, err), true);
            Assert::AreEqual(test.do_cmd("pixel 10d 100 123123", end, err), false);
            Assert::AreEqual(test.do_cmd("pixel 10d 100 123123 d", end, err), false);
        }
        TEST_METHOD(line)
        {
            Assert::AreEqual(test.do_cmb("line 123 123 123 123 123123", end, err), true);
            Assert::AreEqual(test.do_cmd("line 10d 100 100 100 123123", end, err), false);
            Assert::AreEqual(test.do_cmd("line 10d 100 100 100 123123 d", end, err), false);
        }
        TEST_METHOD(rect)
        {
            Assert::AreEqual(test.do_cmb("rect 123 123 123 123 123123", end, err), true);
            Assert::AreEqual(test.do_cmd("rect 10d 100 100 100 123123", end, err), false);
            Assert::AreEqual(test.do_cmd("rect 10d 100 100 100 123123 d", end, err), false);
        }
        TEST_METHOD(fill_rect)
        {
            Assert::AreEqual(test.do_cmb("fill_rect 123 123 123 123 123123", end, err), true);
            Assert::AreEqual(test.do_cmd("fill_rect 10d 100 100 100 123123", end, err), false);
            Assert::AreEqual(test.do_cmd("fill_rect 10d 100 100 100 123123 d", end, err), false);
        }
        TEST_METHOD(ell)
        {
            Assert::AreEqual(test.do_cmb("ell 123 123 123 123 123123", end, err), true);
            Assert::AreEqual(test.do_cmd("ell 10d 100 100 100 123123", end, err), false);
            Assert::AreEqual(test.do_cmd("ell 10d 100 100 100 123123 d", end, err), false);
        }
        TEST_METHOD(fill_ell)
        {
            Assert::AreEqual(test.do_cmb("fill_ell 123 123 123 123 123123", end, err), true);
            Assert::AreEqual(test.do_cmd("fill_ell 10d 100 100 100 123123", end, err), false);
            Assert::AreEqual(test.do_cmd("fill_ell 10d 100 100 100 123123 d", end, err), false);
        }
        TEST_METHOD(circle)
        {
            Assert::AreEqual(test.do_cmb("circle 123 123 123 123123", end, err), true);
            Assert::AreEqual(test.do_cmd("circle 10d 100 100 123123", end, err), false);
            Assert::AreEqual(test.do_cmd("circle 10d 100 100 123123 d", end, err), false);
        }
        TEST_METHOD(fill_circle)
        {
            Assert::AreEqual(test.do_cmb("fill_circle 123 123 123 123123", end, err), true);
            Assert::AreEqual(test.do_cmd("fill_circle 10d 100 100 123123", end, err), false);
            Assert::AreEqual(test.do_cmd("fill_circle 10d 100 100 123123 d", end, err), false);
        }

        TEST_METHOD(round_rect)
        {
            Assert::AreEqual(test.do_cmb("round_rect 123 123 123 123 12 123123", end, err), true);
            Assert::AreEqual(test.do_cmd("round_rect 10d 100 100 123 123123", end, err), false);
            Assert::AreEqual(test.do_cmd("round_rect 10d 100 100 123 12 123123 d", end, err), false);
        }
        TEST_METHOD(fill_round_rect)
        {
            Assert::AreEqual(test.do_cmb("fill_round_rect 123 123 123 123 12 123123", end, err), true);
            Assert::AreEqual(test.do_cmd("fill_round_rect 10d 100 100 123 123123", end, err), false);
            Assert::AreEqual(test.do_cmd("fill_round_rect 10d 100 100 123 12 123123 d", end, err), false);
        }
        TEST_METHOD(text)
        {
            Assert::AreEqual(test.do_cmb("text 123 123 12 12 Hello world 123123", end, err), true);
            Assert::AreEqual(test.do_cmd("text 123 123 12 12 Hello world!!! 123123", end, err), false);
            Assert::AreEqual(test.do_cmd("text 123d 123 12 12 Hello world 123123", end, err), false);
        }
        TEST_METHOD(img)
        {
            Assert::AreEqual(test.do_cmb("img 123 123 2 2 000000 000000 000000 111111", end, err), true);
            Assert::AreEqual(test.do_cmd("img 123 123 2 2 000000 000000 000000", end, err), false);
            Assert::AreEqual(test.do_cmd("img 123 123 2 3 000000 000000 000000 111111", end, err), false);
        }
        TEST_METHOD(get)
        {
            Assert::AreEqual(test.do_cmb("get width", end, err), true);
            Assert::AreEqual(test.do_cmb("get height", end, err), true);
            Assert::AreEqual(test.do_cmd("get w", end, err), false);
        }
	};
}
