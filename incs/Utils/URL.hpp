#ifndef URL_HPP
#define URL_HPP

#include <string>

class URL
{
	public:
		std::string _scheme, _userinfo, _host, _port, _path, _query, _fragment;
		bool _is_directory;

	public:
		URL();
		URL(const std::string& other);
		URL(const URL& other);
		virtual ~URL();
		URL& operator=(const URL& other);

	private:
		void parse_full(const std::string& url);
		void parse_partial(const std::string& url);
		void parse_userinfo(size_t& i, const std::string& url);
};

#endif
