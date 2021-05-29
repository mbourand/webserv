#ifndef TEST_CATEGORY_HPP
#define TEST_CATEGORY_HPP

#include <string>
#include <iostream>
#include <vector>
#include "Test.hpp"

class TestCategory
{
	protected:
		std::string m_name;
		uint8_t m_nesting;
		std::vector<TestCategory> m_subcategories;
		std::vector<Test> m_tests;

	public:
		TestCategory() = default;
		virtual ~TestCategory() = default;
		TestCategory(const TestCategory& other) = default;
		TestCategory(const std::string& name, uint8_t nesting) : m_name(name), m_nesting(nesting) {}


		void addSubcategory(const TestCategory& category)
		{
			m_subcategories.push_back(category);
			m_subcategories.back().m_nesting++;
		}

		template<auto TestType, class... Args>
		void addTest(const std::string& label, Args... args)
		{
			m_tests.push_back(Test(label, m_tests.size() + 1));
			m_tests.back().check<TestType, Args...>(args...);
		}

		void run()
		{
			std::cout << *this << std::endl;
			for (auto& test : m_tests)
				std::cout << std::string((m_nesting + 1) * 2, ' ') << test << std::endl;
			for (auto& sub : m_subcategories)
				sub.run();
		}

	friend std::ostream& operator<<(std::ostream& out, const TestCategory& category);
};

std::ostream& operator<<(std::ostream& out, const TestCategory& category)
{
	out << std::string(category.m_nesting * 2, ' ') << "[" << category.m_name << "]";
	return out;
}

#endif
