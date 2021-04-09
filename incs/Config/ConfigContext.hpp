#ifndef CONFIG_CONTEXT_HPP
#define CONFIG_CONTEXT_HPP

#include <string>
#include <map>
#include <list>

class IMethod;

class ConfigContext
{
	private:
		std::list<std::string> _directive_names;

		std::map<std::string, std::list<std::string> > _params;
		std::list<ConfigContext> _childs;
		std::map<int, std::string> _error_pages;
		std::list<std::string> _names;
		std::list<const IMethod*> _acceptedMethods;
		std::map<std::string, std::string> _cgi_exts;
		bool _autoIndex;

	private:
		size_t find_closing_bracket(const std::string& str, size_t start) const;
		void set_root_default();
		void set_max_body_size_default();
		void set_cgi_dir_default();
		void parse_server_name(const std::string& raw, const int i);
		void parse_error_page(const std::string& raw, const int i);
		void parse_methods(const std::string& raw, const int i);
		void parse_autoindex(const std::string& raw, const int i);
		void parse_cgi_ext(const std::string& raw, const int i);

	public:
		ConfigContext();
		ConfigContext(const ConfigContext& other);
		ConfigContext(const std::string& raw, const std::string& name = "", const ConfigContext* parent = NULL);
		virtual ~ConfigContext();
		ConfigContext& operator=(const ConfigContext& other);

		int getInt(const std::string& param, size_t token) const;
		std::string getString(const std::string& param, size_t token) const;
		std::list<ConfigContext>& getChilds();
		std::string getErrorPage(int code) const;
		std::string getErrorPagePath(int code, const std::string& path) const;
		const std::list<std::string>& getParam(const std::string& name) const;
		const std::list<std::string>& getParamPath(const std::string& name, const std::string& path) const;
		std::string rootPath(const std::string& path, int& base_depth) const;

		bool hasAutoIndex() const;
		bool hasAutoIndexPath(const std::string& path) const;
		const std::map<std::string, std::list<std::string> >& getParams() const;
		const std::map<int, std::string>& getErrorPages() const;
		const std::list<std::string>& getNames() const;
		const std::list<const IMethod*>& getAllowedMethodsPath(const std::string& path) const;
		const std::list<const IMethod*>& getAllowedMethods() const;
		const std::map<std::string, std::string>& getCGIExtensions() const;
		const std::map<std::string, std::string>& getCGIExtensionsPath(const std::string& path) const;
		std::string toString() const;
		std::string findFileWithRoot(const std::string& name) const;
};

#endif
