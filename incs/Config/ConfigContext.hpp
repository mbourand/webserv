#ifndef CONFIG_CONTEXT_HPP
#define CONFIG_CONTEXT_HPP

#include <string>
#include <map>
#include <list>

class ConfigContext
{
	private:
		std::list<std::string> _directive_names;

		std::map<std::string, std::list<std::string> > _params;
		std::list<ConfigContext> _childs;
		std::map<int, std::string> _error_pages;
		std::list<std::string> _names;

	private:
		size_t find_closing_bracket(const std::string& str, size_t start) const;

	public:
		ConfigContext();
		ConfigContext(const ConfigContext& other);
		ConfigContext(const std::string& raw, const std::string& name = "");
		virtual ~ConfigContext();
		ConfigContext& operator=(const ConfigContext& other);

		int getInt(const std::string& param, size_t token) const;
		std::string getString(const std::string& param, size_t token) const;
		std::list<ConfigContext>& getChilds();
		std::string getErrorPage(int code) const;
		const std::list<std::string>& getParam(const std::string& name) const;

		const std::map<std::string, std::list<std::string> >& getParams() const;
		const std::map<int, std::string>& getErrorPages() const;
		const std::list<std::string>& getNames() const;
		std::string toString() const;
};

#endif
